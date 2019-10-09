// #pragma once
// #include "main.h"

// #include "lib7842/path/simplePath.hpp"
// #include "lib7842/path/packedPath.hpp"
// #include "lib7842/other/utility.hpp"

// namespace lib7842 {

// class PathGenerator {

//  public:
//   struct smoothGains {
//     const double weight;
//     const QLength tolerance;
//   };

//   struct velGains {
//     const QSpeed min;
//     const QSpeed max;
//     const QAcceleration accel;
//     /**
//      * How much to slow down around turns.
//      * This value is usually best around 0.5-2, 0.5 tends to slow down around almost any curvature in the
//      * path, and 2 tends to slow down around only a very sharp curvature.
//      */
//     const double curvatureK;
//   };

//   static PackedPath generate(
//     const AbstractPath& ipath,
//     const QLength& iresolution,
//     const smoothGains& ismoothGains,
//     const velGains& ivelGains);

//   static SimplePath insertPoints(const AbstractPath& ipath, const QLength& iresolution);
//   static SimplePath smoothen(const AbstractPath& ipath, const smoothGains& ismoothGains);

//   static void packDistances(PackedPath& ipath);
//   static void packCurvatures(PackedPath& ipath);

//   static void packVelocity(PackedPath& ipath, const velGains& ivelGains);
//   static void packLimitVelocity(PackedPath& ipath, const velGains& ivelGains);

//   static QCurvature
//     getCurvature(const QPoint& prevPoint, const QPoint& point, const QPoint& nextPoint);
// };

// } // namespace lib7842