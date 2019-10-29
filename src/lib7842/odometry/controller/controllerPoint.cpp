// #include "controller.hpp"

// namespace lib7842
// {

//   void OdomController::driveToPoint(Vector targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
//   {
//     resetPid();
//     QAngle lastTarget = odometry->getState().theta;
//     do
//     {
//       Vector closestPoint = closest(odometry->getState(), targetPoint);

//       QAngle angleToClose = angleToPoint(closestPoint);
//       if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg;

//       QLength distanceToClose = distanceToPoint(closestPoint);
//       if(angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

//       angleErr = angleToPoint(targetPoint);

//       QLength distanceToTarget = distanceToPoint(targetPoint);

//       if(distanceToTarget.abs() < pointRadius) {
//         angleErr = 0_deg;
//         distanceErr = distanceToClose;
//       } else {
//         angleErr = angleToPoint(targetPoint);
//         lastTarget = angleErr + odometry->getState().theta;
//         distanceErr = distanceToTarget;
//       }

//       angleErr = rollAngle90(angleErr);

//       double angleVel = angleController->step(-angleErr.convert(degree));
//       double distanceVel = distanceController->step(-distanceToClose.convert(millimeter));

//       driveVector(distanceVel, angleVel * turnScale);
//       runActions(actions);
//       pros::delay(10);
//     }
//     while(!settleFunc(this));

//     driveVector(0, 0);
//   }

//   void OdomController::driveToPoint2(Vector targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
//   {
//     resetPid();
//     settleFunc_t exitFunc = makeSettle(pointRadius);
//     do
//     {
//       angleErr = angleToPoint(targetPoint);
//       distanceErr = distanceToPoint(targetPoint);

//       if(angleErr.abs() > 90_deg) distanceErr = -distanceErr;
//       angleErr = rollAngle90(angleErr);

//       double angleVel = angleController->step(-angleErr.convert(degree));
//       double distanceVel = distanceController->step(-distanceErr.convert(millimeter));

//       driveVector(distanceVel, angleVel * turnScale);
//       runActions(actions);
//       pros::delay(10);
//     }
//     while(!(exitFunc(this) || settleFunc(this)));

//     driveDistanceAtAngle(distanceToPoint(targetPoint), angleCalc(angleToPoint(targetPoint)), turnScale, settleFunc, actions);
//     driveVector(0, 0);
//   }

//   void OdomController::drivePath(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions)
//   {
//     for(auto&& point : path.wayPoints)
//     {
//       driveToPoint(point, turnScale, moveOnSettle, actions);
//     }
//     driveToPoint(path.wayPoints.back(), turnScale, finalSettle, actions);
//   }

//   void OdomController::drivePath2(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions)
//   {
//     for(auto&& point : path.wayPoints)
//     {
//       driveToPoint2(point, turnScale, moveOnSettle, actions);
//     }
//     driveToPoint2(path.wayPoints.back(), turnScale, finalSettle, actions);
//   }

// }
