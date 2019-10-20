// #pragma once
// #include "main.h"
// #include "abstractPath.hpp"
// #include "simplePath.hpp"

// #include "lib7842/point/point.hpp"

// namespace lib7842 {

// class ReferencePath : public AbstractPath {

// public:
//   /**
//    * Default Constructors
//    */
//   ReferencePath() = default;
//   ReferencePath(const ReferencePath& ipath) = default;
//   virtual ~ReferencePath() = default;

//   explicit ReferencePath(const std::vector<VectorRef>& ipath);

//   std::vector<VectorRef>& get();
//   std::vector<VectorRef>& operator()();

//   *
//    * Sample the path and return a path with higher resolution
//    * @param  isteps the number of points to generate for each segment

//   virtual SimplePath generate(const int isteps = 1) const override;

//   /**
//    * Return shared pointer to copy of path
//    */
//   virtual std::shared_ptr<AbstractPath> copyPtr() const override;

//   /**
//    * Move the path into a shared pointer and return pointer
//    */
//   virtual std::shared_ptr<AbstractPath> movePtr() const override;

// protected:
//   std::vector<VectorRef> path {};
// };

// } // namespace lib7842