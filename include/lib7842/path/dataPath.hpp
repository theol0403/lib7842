// #pragma once
// #include "main.h"
// #include "simplePath.hpp"

// #include "lib7842/point/dataPoint.hpp"

// namespace lib7842 {

// class DataPath : public AbstractPath {

// public:
//   /**
//    * Default Constructors
//    */
//   DataPath() = default;
//   DataPath(const DataPath& ipath) = default;
//   virtual ~DataPath() = default;

//   /**
//    * Explicit Constructors
//    */
//   explicit DataPath(const std::initializer_list<DataPoint>& ipath);
//   explicit DataPath(const std::vector<DataPoint>& ipath);
//   explicit DataPath(const AbstractPath& ipath);

//   /**
//    * Explicit Functions
//    */
//   std::vector<DataPoint>& get();
//   std::vector<DataPoint>& operator()();

//   /**
//    * Extractors
//    */
//   virtual SimplePath extract() const override;
//   virtual ReferencePath extractRef() const override;
//   virtual std::shared_ptr<AbstractPath> copyPtr() const override;
//   virtual std::shared_ptr<AbstractPath> movePtr() const override;

// protected:
//   std::vector<DataPoint> path {};
// };

// } // namespace lib7842