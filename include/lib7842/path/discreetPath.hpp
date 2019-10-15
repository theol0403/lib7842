// #pragma once
// #include "main.h"
// #include "abstractPath.hpp"
// #include "referencePath.hpp"

// namespace lib7842 {

// /**
//  * Interface Path Class
//  * A discreet path contains points,
//  *   so is able to return a reference to those points
//  */
// class DiscreetPath : public AbstractPath {

// public:
//   /**
//    * Default Constructors
//    */
//   DiscreetPath() = default;
//   DiscreetPath(const DiscreetPath& ipath) = default;
//   virtual ~DiscreetPath() = default;

//   /**
//    * Extract path of references to points
//    */
//   virtual ReferencePath extractRef() const override;

//   *
//    * Extract path of shared pointers

//   virtual SimplePath extract() const = 0;

//   /**
//    * Return shared pointer to copy of path
//    */
//   virtual std::shared_ptr<AbstractPath> copyPtr() const = 0;

//   /**
//    * Move the path into a shared pointer and return pointer
//    */
//   virtual std::shared_ptr<AbstractPath> movePtr() const = 0;

//   /**
//    * Sample the path and return a path with higher resolution
//    * @param  isteps the number of points to generate for each segment
//    */
//   virtual SimplePath generate(const size_t isteps) const = 0;
// };

// } // namespace lib7842