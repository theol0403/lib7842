cmake_minimum_required(VERSION 3.6)
project(OkapiLibV5)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -Wall -Wextra -Wshadow -Wnull-dereference -Wno-psabi -Wno-unused-function -pthread -g -D THREADS_STD")

include_directories(include)

set(CMAKE_CPP_COMPILER gcc-10)

add_library(OkapiLibV5 STATIC
        include/okapi/api/chassis/controller/chassisController.hpp
        include/okapi/api/chassis/controller/chassisControllerIntegrated.hpp
        include/okapi/api/chassis/controller/chassisControllerPid.hpp
        include/okapi/api/chassis/controller/chassisScales.hpp
        include/okapi/api/chassis/controller/odomChassisController.hpp
        include/okapi/api/chassis/controller/defaultOdomChassisController.hpp
        include/okapi/api/chassis/model/chassisModel.hpp
        include/okapi/api/chassis/model/hDriveModel.hpp
        include/okapi/api/chassis/model/readOnlyChassisModel.hpp
        include/okapi/api/chassis/model/skidSteerModel.hpp
        include/okapi/api/chassis/model/threeEncoderSkidSteerModel.hpp
        include/okapi/api/chassis/model/threeEncoderXDriveModel.hpp
        include/okapi/api/chassis/model/xDriveModel.hpp
        include/okapi/api/control/async/asyncController.hpp
        include/okapi/api/control/async/asyncLinearMotionProfileController.hpp
        include/okapi/api/control/async/asyncMotionProfileController.hpp
        include/okapi/api/control/async/asyncPosIntegratedController.hpp
        include/okapi/api/control/async/asyncPositionController.hpp
        include/okapi/api/control/async/asyncPosPidController.hpp
        include/okapi/api/control/async/asyncVelIntegratedController.hpp
        include/okapi/api/control/async/asyncVelocityController.hpp
        include/okapi/api/control/async/asyncVelPidController.hpp
        include/okapi/api/control/async/asyncWrapper.hpp
        include/okapi/api/control/iterative/iterativeController.hpp
        include/okapi/api/control/iterative/iterativeMotorVelocityController.hpp
        include/okapi/api/control/iterative/iterativePositionController.hpp
        include/okapi/api/control/iterative/iterativePosPidController.hpp
        include/okapi/api/control/iterative/iterativeVelocityController.hpp
        include/okapi/api/control/iterative/iterativeVelPidController.hpp
        include/okapi/api/control/util/controllerRunner.hpp
        include/okapi/api/control/util/flywheelSimulator.hpp
        include/okapi/api/control/util/pathfinderUtil.hpp
        include/okapi/api/control/util/pidTuner.hpp
        include/okapi/api/control/util/settledUtil.hpp
        include/okapi/api/control/closedLoopController.hpp
        include/okapi/api/control/controllerInput.hpp
        include/okapi/api/control/controllerOutput.hpp
        include/okapi/api/control/offsettableControllerInput.hpp
        include/okapi/api/device/button/abstractButton.hpp
        include/okapi/api/device/button/buttonBase.hpp
        include/okapi/api/device/motor/abstractMotor.hpp
        include/okapi/api/device/rotarysensor/continuousRotarySensor.hpp
        include/okapi/api/device/rotarysensor/rotarySensor.hpp
        include/okapi/api/filter/averageFilter.hpp
        include/okapi/api/filter/composableFilter.hpp
        include/okapi/api/filter/demaFilter.hpp
        include/okapi/api/filter/ekfFilter.hpp
        include/okapi/api/filter/emaFilter.hpp
        include/okapi/api/filter/filter.hpp
        include/okapi/api/filter/filteredControllerInput.hpp
        include/okapi/api/filter/medianFilter.hpp
        include/okapi/api/filter/passthroughFilter.hpp
        include/okapi/api/filter/velMath.hpp
        include/okapi/api/odometry/odometry.hpp
        include/okapi/api/odometry/twoEncoderOdometry.hpp
        include/okapi/api/odometry/odomMath.hpp
        include/okapi/api/odometry/threeEncoderOdometry.hpp
        include/okapi/api/units/QAcceleration.hpp
        include/okapi/api/units/QAngle.hpp
        include/okapi/api/units/QAngularAcceleration.hpp
        include/okapi/api/units/QAngularJerk.hpp
        include/okapi/api/units/QAngularSpeed.hpp
        include/okapi/api/units/QArea.hpp
        include/okapi/api/units/QForce.hpp
        include/okapi/api/units/QFrequency.hpp
        include/okapi/api/units/QJerk.hpp
        include/okapi/api/units/QLength.hpp
        include/okapi/api/units/QMass.hpp
        include/okapi/api/units/QPressure.hpp
        include/okapi/api/units/QSpeed.hpp
        include/okapi/api/units/QTime.hpp
        include/okapi/api/units/QTorque.hpp
        include/okapi/api/units/QVolume.hpp
        include/okapi/api/units/RQuantity.hpp
        include/okapi/api/util/abstractRate.hpp
        include/okapi/api/util/logging.hpp
        include/okapi/api/util/timeUtil.hpp
        include/okapi/api/util/abstractTimer.hpp
        include/okapi/api/util/mathUtil.hpp
        include/okapi/api/util/supplier.hpp
        include/okapi/api/coreProsAPI.hpp
        src/api/chassis/controller/chassisControllerIntegrated.cpp
        src/api/chassis/controller/chassisControllerPid.cpp
        src/api/chassis/controller/chassisScales.cpp
        src/api/chassis/controller/chassisScales.cpp
        src/api/chassis/controller/odomChassisController.cpp
        src/api/chassis/controller/defaultOdomChassisController.cpp
        src/api/chassis/model/hDriveModel.cpp
        src/api/chassis/model/skidSteerModel.cpp
        src/api/chassis/model/threeEncoderSkidSteerModel.cpp
        src/api/chassis/model/threeEncoderXDriveModel.cpp
        src/api/chassis/model/xDriveModel.cpp
        src/api/control/async/asyncLinearMotionProfileController.cpp
        src/api/control/async/asyncMotionProfileController.cpp
        src/api/control/async/asyncPosIntegratedController.cpp
        src/api/control/async/asyncPosPidController.cpp
        src/api/control/async/asyncVelIntegratedController.cpp
        src/api/control/async/asyncVelPidController.cpp
        src/api/control/iterative/iterativeMotorVelocityController.cpp
        src/api/control/iterative/iterativePosPidController.cpp
        src/api/control/iterative/iterativeVelPidController.cpp
        src/api/control/util/flywheelSimulator.cpp
        src/api/control/offsettableControllerInput.cpp
        src/api/control/util/pidTuner.cpp
        src/api/control/util/settledUtil.cpp
        src/api/device/button/abstractButton.cpp
        src/api/device/button/buttonBase.cpp
        src/api/device/motor/abstractMotor.cpp
        src/api/device/rotarysensor/rotarySensor.cpp
        src/api/filter/composableFilter.cpp
        src/api/filter/demaFilter.cpp
        src/api/filter/ekfFilter.cpp
        src/api/filter/emaFilter.cpp
        src/api/filter/filter.cpp
        src/api/filter/passthroughFilter.cpp
        src/api/filter/velMath.cpp
        src/api/odometry/twoEncoderOdometry.cpp
        src/api/odometry/odomMath.cpp
        src/api/odometry/threeEncoderOdometry.cpp
        src/api/util/abstractRate.cpp
        src/api/util/abstractTimer.cpp
        src/api/util/logging.cpp
        src/api/util/timeUtil.cpp
        src/pathfinder/generator.c
        src/pathfinder/io.c
        src/pathfinder/mathutil.c
        src/pathfinder/spline.c
        src/pathfinder/trajectory.c
        src/pathfinder/fit/hermite.c
        src/pathfinder/followers/distance.c
        src/pathfinder/followers/encoder.c
        src/pathfinder/modifiers/swerve.c
        src/pathfinder/modifiers/tank.c
        include/okapi/api/odometry/point.hpp
        include/okapi/api/odometry/stateMode.hpp
        include/okapi/api/odometry/odomState.hpp
        src/api/odometry/odomState.cpp
        )

