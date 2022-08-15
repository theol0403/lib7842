#include "lib7842/api/purePursuit/pathFollower.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"

namespace lib7842 {

PathFollower::PathFollower(std::shared_ptr<ChassisModel> imodel,
                           std::shared_ptr<Odometry> iodometry, const ChassisScales& ichassisScales,
                           const QAngularSpeed& igearset, const QLength& ilookahead,
                           const std::optional<QLength>& idriveRadius) :
  model(std::move(imodel)),
  odometry(std::move(iodometry)),
  chassisScales(ichassisScales),
  gearset(igearset),
  lookahead(ilookahead),
  driveRadius(idriveRadius.value_or(ilookahead)) {}

void PathFollower::followPath(const std::vector<Waypoint>& path, const PursuitLimits& limits,
                              bool backwards, const std::optional<QSpeed>& startSpeed) {
  resetPursuit();

  auto rate = global::getTimeUtil()->getRate();
  auto timer = global::getTimeUtil()->getTimer();

  // assume the robot starts at minimum velocity unless otherwise specified
  QSpeed lastVelocity = startSpeed.value_or(limits.minVel);

  bool isFinished = false; // loop until the robot is considered to have finished the path
  while (!isFinished) {
    // get the robot position and heading
    State pos = State(odometry->getState(StateMode::CARTESIAN));

    auto closest = findClosest(path, pos); // get an iterator to the closest point
    Vector lookPoint = findLookaheadPoint(path, pos);

    // the robot is on the path if the distance to the closest point is smaller than the lookahead
    bool onPath = Vector::dist(pos, *closest) <= lookahead;

    // project the lookahead point onto the lookahead radius. When the lookahead point is further
    // than the lookahead radius, this can cause some problems with the robot curvature calculation.
    // The projected point will cause the robot to rotate more appropriately.
    Vector projectedLook = (MathPoint::normalize(lookPoint - pos) * lookahead.convert(meter)) + pos;

    // if the robot is on the path, use the normal lookahead. If not, use the projected.
    auto& finalLook = onPath ? lookPoint : projectedLook;

    // whether the robot is within the driveRadius of the end of the path. If correction.
    bool withinDriveRadius = Vector::dist(lookPoint, path.back()) < driveRadius &&
                             Vector::dist(pos, path.back()) < driveRadius &&
                             Vector::dist(*closest, path.back()) < driveRadius;

    // calculate the arc curvature for the robot to travel to the lookahead
    double curvature = withinDriveRadius ? 0 : calculateCurvature(pos, finalLook);

    // the angle to the end of the path
    QAngle angleToEnd = pos.angleTo(path.back()).abs();

    // we are done the path if the angle is opposite of the drive direction
    bool pastEnd = backwards ? angleToEnd < 90_deg : angleToEnd > 90_deg;

    // the robot is considered finished if it has passed the end
    isFinished = pastEnd && withinDriveRadius;

    // if the robot is on the path, choose the lowest of either the path velocity or the
    // curvature-based speed reduction. If the robot is not on the path, choose the lowest of
    // either the max velocity or the curvature-based speed reduction.
    QSpeed targetVel = 0_mps;
    if (onPath) {
      auto pathSpeed = closest->velocity;
      targetVel =
        limits.k ? std::min(pathSpeed, limits.k.value() / std::abs(curvature)) : pathSpeed;
    } else {
      targetVel =
        limits.k ? std::min(limits.maxVel, limits.k.value() / std::abs(curvature)) : limits.maxVel;
    }

    // add an upwards rate limiter to the robot velocity using the formula vf=vi+at
    targetVel = std::max(targetVel, limits.minVel); // add minimum velocity
    auto dT = timer->getDt();
    // get maximum allowable change in velocity
    QSpeed maxVelocity = lastVelocity + dT * limits.accel;
    // limit the velocity
    if (targetVel > maxVelocity) { targetVel = maxVelocity; }
    lastVelocity = targetVel;

    // calculate robot wheel velocities
    auto wheelVel = calculateVelocity(targetVel, curvature, chassisScales, limits);

    // if within the the of the path, ignore the default parameter and drive directly to the end.
    // We are past the end of the path if the angle is above 90, so drive backwards if so.
    bool driveBackward = withinDriveRadius ? angleToEnd > 90_deg : backwards;

    // negate velocities to drive backward
    if (driveBackward) {
      wheelVel[0] *= -1;
      wheelVel[1] *= -1;
    }

    // if the robot is within the drive radius, switch to a heading controller which seeks the exit
    // angle
    if (withinDriveRadius) {
      // get exit angle of the path
      auto endAngle = (path.end() - 2)->angleTo(path.back());
      // if backwards, exit angle is flipped
      if (backwards) { endAngle += 180_deg; }
      // get angle error
      QAngle error = util::wrapAngle90(endAngle - pos.theta);
      // get distance to lookahead
      QLength dist = Vector::dist(pos, lookPoint);
      // given robot velocity, approximate time to get to lookahead
      QTime time = dist / targetVel;
      // calculate angular velocity to reach the lookahead angle given the time
      QAngularSpeed rotation = error / time;
      // calculate what speed the wheels need to be moving at
      QAngularSpeed turnVel = rotation * chassisScales.wheelTrack / chassisScales.wheelDiameter;

      wheelVel[0] += turnVel;
      wheelVel[1] -= turnVel;
    }

    double left = (wheelVel[0] / gearset).convert(number);
    double right = (wheelVel[1] / gearset).convert(number);

    // normalize the sides
    double maxMag = std::max(std::abs(left), std::abs(right));
    if (maxMag > 1.0) {
      left /= maxMag;
      right /= maxMag;
    }

    if (mode == util::motorMode::voltage) {
      model->tank(left, right);
    } else {
      model->left(left);
      model->right(right);
    }

    rate->delayUntil(10_ms);
  }

  model->driveVector(0, 0); // apply velocity braking
}

void PathFollower::setMotorMode(util::motorMode imode) { mode = imode; }

PathFollower::pathIterator_t PathFollower::findClosest(const std::vector<Waypoint>& path,
                                                       const Vector& pos) {

  QLength closestDist {std::numeric_limits<double>::max()};
  // get the last closest point, or the beginning of the path if there is none
  auto closest = lastClosest.value_or(path.begin());

  // Optimization: limit the progression of the closest point. It considers the last closest point,
  // and all the options up to one point ahead of the lookahead. This improves performance and
  // prevents the robot from skipping ahead on the path.
  //
  // If the end of the path is within the lookahead, it considers all options up to the end of the
  // path. This is to make sure that the closest point is the end in the scenario where the
  // lookahead is not the end of the path.

  auto end =
    Vector::dist(pos, path.back()) > lookahead ? path.begin() + lastLookIndex + 2 : path.end();

  // loop from the last closest point to one point past the lookahead
  for (auto it = closest; it < end; it++) {
    if (it >= path.end()) { break; }
    QLength distance = Vector::dist(pos, *it);
    if (distance < closestDist) {
      closestDist = distance;
      closest = it;
    }
  }

  lastClosest = closest;
  return closest;
}

Vector PathFollower::findLookaheadPoint(const std::vector<Waypoint>& path, const Vector& pos) {

  // Optimization: if the robot starts within the end of the path, then the only intersection is
  // behind the robot, causing the robot to drive backwards when we want it to go straight to the
  // lookahead. To fix this, if the lookahead has not been found yet, but the robot is within the
  // end of the path, then jump the lookahead to the end of the path.
  if (!lastClosest && Vector::dist(pos, path.back()) < lookahead) {
    lastClosest = path.end() - 2;
    lastLookT = 1;
  }

  // lookahead intersection should not be behind closest
  size_t lastClosestIndex = lastClosest.value_or(path.begin()) - path.begin();

  // used for optimizing number of intersection searches
  size_t lastIntersect = 0;

  // loop through every segment looking for intersection
  for (size_t i = std::max(lastLookIndex, lastClosestIndex); i < path.size() - 1; i++) {
    auto& start = path[i];
    auto& end = path[i + 1];

    auto t = findIntersectT(start, end, pos, lookahead);
    if (t) {
      // If the segment is further along or the fractional index is greater, then this is the
      // correct point
      if (i > lastLookIndex || t.value() > lastLookT) {
        lastLookIndex = i;
        lastLookT = t.value();
        // if this is the second intersection that was found, we are done
        if (lastIntersect > 0) { break; }
        // record the index of the first intersection
        lastIntersect = i;
      }
    }

    // Optimization: if an intersection has been found, and the loop is checking distances from the
    // last intersection that are further than the lookahead, we are done.
    if (lastIntersect > 0 && Vector::dist(path[i], path[lastIntersect]) >= lookahead * 2) { break; }
  }

  const auto& start = path[lastLookIndex];
  const auto& end = path[lastLookIndex + 1];
  return start + ((end - start) * lastLookT);
}

std::optional<double> PathFollower::findIntersectT(const Vector& start, const Vector& end,
                                                   const Vector& pos, const QLength& lookahead) {
  Vector d = end - start;
  Vector f = start - pos;

  double a = MathPoint::dot(d, d);
  double b = MathPoint::dot(d, f) * 2.0;
  double c = MathPoint::dot(f, f) - std::pow(lookahead.convert(meter), 2);
  double dis = std::pow(b, 2) - (4.0 * (a * c));

  if (dis >= 0) {
    dis = std::sqrt(dis);
    double t1 = (-b - dis) / (2.0 * a);
    double t2 = (-b + dis) / (2.0 * a);

    // prioritize further down path
    if (t2 >= 0.0 && t2 <= 1.0) { return t2; }
    if (t1 >= 0.0 && t1 <= 1.0) { return t1; }
  }

  // no intersection on this interval
  return std::nullopt;
}

double PathFollower::calculateCurvature(const State& state, const Vector& lookPoint) {
  MathPoint pos(state);
  MathPoint look(lookPoint);
  MathPoint diff = look - pos;
  double head = ((state.theta * -1) + 90_deg).convert(radian);
  double a = -std::tan(head);
  double c = -a * pos.x - pos.y;
  double x = std::abs(a * look.x + 1.0 * look.y + c) / std::sqrt(std::pow(a, 2) + 1);
  int side = util::sgn(std::sin(head) * diff.x - std::cos(head) * diff.y);
  double curv = (2.0 * x) / std::pow(MathPoint::dist(state, lookPoint), 2);
  return std::pow(curv, 2) * side;
}

std::valarray<QAngularSpeed> PathFollower::calculateVelocity(const QSpeed& vel, double curvature,
                                                             const ChassisScales& chassisScales,
                                                             const PursuitLimits& limits) {
  QSpeed leftVel = vel * (2.0 + chassisScales.wheelTrack.convert(meter) * curvature) / 2.0;
  QSpeed rightVel = vel * (2.0 - chassisScales.wheelTrack.convert(meter) * curvature) / 2.0;

  leftVel = std::clamp(leftVel, limits.maxVel * -1, limits.maxVel);
  rightVel = std::clamp(rightVel, limits.maxVel * -1, limits.maxVel);

  QAngularSpeed leftWheel = (leftVel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;
  QAngularSpeed rightWheel = (rightVel / (1_pi * chassisScales.wheelDiameter)) * 360_deg;

  return {leftWheel, rightWheel};
}

void PathFollower::resetPursuit() {
  lastClosest = std::nullopt;
  lastLookIndex = 0;
  lastLookT = 0;
}

} // namespace lib7842

#include "lib7842/api/odometry/customOdometry.hpp"
#include "lib7842/test/mocks.hpp"
namespace test {
class MockPathFollower : public PathFollower {
public:
  using PathFollower::PathFollower;
  using PathFollower::lastLookIndex;
  using PathFollower::findClosest;
  using PathFollower::findLookaheadPoint;
  using PathFollower::calculateCurvature;
  using PathFollower::calculateVelocity;
};

TEST_CASE("PathFollower") {

  SUBCASE("given a model, odom, follower, and limits") {

    auto model = std::make_shared<MockThreeEncoderXDriveModel>();
    auto odom =
      std::make_shared<CustomOdometry>(model, ChassisScales({{4_in, 10_in, 5_in, 4_in}, 360}));

    auto follower = std::make_shared<MockPathFollower>(
      model, odom, ChassisScales({{4_in, 10_in}, 360}), 200_rpm, 6_in);

    PursuitLimits limits {0_mps, 0.5_mps2, 1_mps, 1_mps};

    SUBCASE("TestClosest") {
      std::vector<Waypoint> path(
        {{0_ft, 0_ft}, {1_ft, 1_ft}, {2_ft, 2_ft}, {3_ft, 3_ft}, {4_ft, 4_ft}});
      follower->lastLookIndex = 4;

      auto closest = follower->findClosest(path, {1_ft, 1_ft});
      CHECK(closest - path.begin() == 1);

      closest = follower->findClosest(path, {0_ft, 0_ft});
      CHECK(closest - path.begin() == 1);

      closest = follower->findClosest(path, {3_ft, 3.3_ft});
      CHECK(closest - path.begin() == 3);

      closest = follower->findClosest(path, {6_ft, 6_ft});
      CHECK(closest - path.begin() == 4);

      closest = follower->findClosest(path, {0_ft, 0_ft});
      CHECK(closest - path.begin() == 4);
    }

    SUBCASE("TestLookahead") {
      std::vector<Waypoint> path(
        {{0_ft, 0_ft}, {0_ft, 1_ft}, {0_ft, 2_ft}, {0_ft, 3_ft}, {0_ft, 4_ft}});

      Vector lookahead = follower->findLookaheadPoint(path, {0_ft, 1_ft});
      Vector estimated {0_ft, 1.5_ft};
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 1_ft});
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 2_ft});
      estimated = {0_ft, 2.5_ft};
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 0_ft});
      CHECK(lookahead == estimated);

      lookahead = follower->findLookaheadPoint(path, {0_ft, 3.5_ft});
      estimated = {0_ft, 4_ft};
      CHECK(lookahead.y.convert(foot) == Approx(estimated.y.convert(foot)));

      lookahead = follower->findLookaheadPoint(path, {0_ft, 4_ft});
      estimated = {0_ft, 4_ft};
      CHECK(lookahead.y.convert(foot) == Approx(estimated.y.convert(foot)));

      lookahead = follower->findLookaheadPoint(path, {0_ft, 2_ft});
      CHECK(lookahead.y.convert(foot) == Approx(estimated.y.convert(foot)));
    }

    SUBCASE("TestCurvature") {
      auto curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 0_deg}, {0_in, 5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 90_deg}, {5_in, 0_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 90_deg}, {-5_in, 0_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {5_in, 5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {-5_in, -5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 45_deg}, {10_in, 5_in});
      CHECK(std::abs(curvature) > 2);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, -45_deg}, {-5_in, 5_in});
      CHECK(std::abs(curvature) < 1e-4);

      curvature = MockPathFollower::calculateCurvature({0_in, 0_in, 200_deg}, {10_in, 5_in});
      CHECK(std::abs(curvature) > 4);
    }

    SUBCASE("TestVelocityStraight") {
      auto vel = MockPathFollower::calculateVelocity(
        1_mps, 0, ChassisScales({{1_m / 1_pi, 10_m}, 360}), {0_mps, 1_mps2, 10_mps, 1_mps});
      CHECK(vel[0] == 60_rpm);
      CHECK(vel[1] == 60_rpm);
    }

    SUBCASE("TestVelocityCurved") {
      auto vel = MockPathFollower::calculateVelocity(
        1_mps, 1, ChassisScales({{1_m / 1_pi, 10_m}, 360}), {0_mps, 1_mps2, 10_mps, 1_mps});
      CHECK(vel[0] > 60_rpm);
      CHECK(vel[1] < 60_rpm);
    }
  }

  SUBCASE("TestVelConversions") {
    QSpeed robotVel = 1_mps;
    QAngularSpeed leftWheel = (robotVel / 10_cm) * 360_deg;
    CHECK(leftWheel == 600_rpm);
  }

  SUBCASE("ReverseVelConversions") {
    QAngularSpeed wheel = (1_mps / (1_pi * 10_cm)) * 360_deg;
    QSpeed vel = (wheel * 1_pi * 10_cm) / 360_deg;
    CHECK(vel == 1_mps);
  }
}
} // namespace test
