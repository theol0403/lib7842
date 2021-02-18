#pragma once

#include "lib7842/api/async/async.hpp"
#include "lib7842/api/async/taskWrapper.hpp"
#include "lib7842/api/async/trigger.hpp"

#include "lib7842/api/gui/actions.hpp"
#include "lib7842/api/gui/graph.hpp"
#include "lib7842/api/gui/odom.hpp"
#include "lib7842/api/gui/page.hpp"
#include "lib7842/api/gui/screen.hpp"
#include "lib7842/api/gui/selector.hpp"
#include "lib7842/api/gui/visionDrawer.hpp"

#include "lib7842/api/odometry/customOdometry.hpp"
#include "lib7842/api/odometry/odomController.hpp"
#include "lib7842/api/odometry/odomXController.hpp"
#include "lib7842/api/odometry/settler.hpp"

#include "lib7842/api/other/global.hpp"
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/other/utility.hpp"

#include "lib7842/api/positioning/point/data.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"

#include "lib7842/api/positioning/spline/bezier.hpp"
#include "lib7842/api/positioning/spline/hermite.hpp"
#include "lib7842/api/positioning/spline/line.hpp"
#include "lib7842/api/positioning/spline/parametric.hpp"
#include "lib7842/api/positioning/spline/piecewise.hpp"
#include "lib7842/api/positioning/spline/spline.hpp"
#include "lib7842/api/positioning/spline/stepper.hpp"

#include "lib7842/api/purePursuit/pathFollower.hpp"
#include "lib7842/api/purePursuit/pathFollowerX.hpp"
#include "lib7842/api/purePursuit/pathGenerator.hpp"
#include "lib7842/api/purePursuit/pursuitLimits.hpp"
#include "lib7842/api/purePursuit/waypoint.hpp"

#include "lib7842/api/trajectory/generator.hpp"
#include "lib7842/api/trajectory/limits.hpp"
#include "lib7842/api/trajectory/profile.hpp"
#include "lib7842/api/trajectory/trapezoidal.hpp"

#include "lib7842/api/vision/container.hpp"
#include "lib7842/api/vision/object.hpp"
#include "lib7842/api/vision/query.hpp"
#include "lib7842/impl/vision/vision.hpp"