// #include "test.hpp"
// #include "lib7842/path/compoundPath.hpp"

// class CompoundPathTest : public ::testing::Test {
// protected:
//   CompoundPath path;
//   Vector point1 {5_in, 3_in};
// };

// TEST_F(CompoundPathTest, Constructors) {
//   CompoundPath();
//   CompoundPath(std::make_shared<SimplePath>());
//   CompoundPath(std::make_shared<ReferencePath>());
// }

// TEST_F(CompoundPathTest, AddPaths) {
//   path.addPath(std::make_shared<SimplePath>());
//   path.addPath(std::make_shared<ReferencePath>());
//   path.importPath(SimplePath {point1});
//   path.importPath(SimplePath({point1, point1}));
// }

// TEST_F(CompoundPathTest, ExtractSegments) {
//   path.importPath(SimplePath({point1}));
//   path.importPath(SimplePath({point1, point1}));
//   path.addPath(std::make_shared<SimplePath>(SimplePath({point1})));

//   SimplePath ipath = path.extract();
//   ASSERT_EQ(ipath.get().size(), 4);
//   for (auto&& point : ipath.get()) {
//     ASSERT_EQ(point, point1);
//   }

//   ReferencePath iref = path.extractRef();
//   for (auto&& point : iref.get()) {
//     ASSERT_EQ(point.get(), point1);
//   }
// }

// TEST_F(CompoundPathTest, ExtractSegmentsRef) {
//   path.importPath(SimplePath({point1}));
//   path.importPath(SimplePath({point1, point1}));
//   path.addPath(std::make_shared<SimplePath>());
//   path.importPath(SimplePath({{5_in, 3_in}}));

//   ReferencePath ipath = path.extractRef();
//   ReferencePath ipath2 = path.extractRef();

//   for (size_t i = 0; i < ipath.get().size(); i++) {
//     ASSERT_EQ(&ipath.get()[i].get(), &ipath2.get()[i].get());
//   }
// }

// TEST_F(CompoundPathTest, StressTest) {
//   path.importPath(SimplePath({{1_in, 2_in}}));
//   path.importPath(SimplePath({{2_in, 3_in}, {3_in, 4_in}}));

//   Vector refPoint1 {4_in, 5_in};
//   Vector refPoint2 {5_in, 6_in};
//   CompoundPath segment1 {CompoundPath().importPath(ReferencePath({refPoint1, refPoint2}))};
//   CompoundPath segment2 {CompoundPath().importPath(SimplePath({{6_in, 7_in}}))};
//   CompoundPath segment3 {CompoundPath().importPath(SimplePath({{7_in, 8_in}}))};
//   CompoundPath segment3b {CompoundPath().importPath(segment3)};

//   CompoundPath segment4 {CompoundPath().copyPath(segment2).copyPath(segment3b)};
//   CompoundPath segment5 {CompoundPath().importPath(SimplePath({{8_in, 9_in}}))};

//   path
//     .addPath(std::shared_ptr<CompoundPath>(&segment1, [](AbstractPath*) {})) // empty deleter
//     .addPath(std::make_shared<CompoundPath>(std::move(segment4))) // move the local into shared
//     .addPath(std::make_shared<CompoundPath>(segment5)); // make copy

//   SimplePath ipath = path.extract();

//   // test point values
//   ASSERT_EQ(ipath.get().size(), 8);
//   for (size_t i = 0; i < ipath.get().size(); ++i) {
//     ASSERT_EQ(ipath.get()[i], (Vector {(i + 1) * inch, (i + 2) * inch}));
//   }

//   // test refpath addresses
//   ReferencePath iref = path.extractRef();
//   ASSERT_EQ(iref.get().size(), 8);
//   ASSERT_EQ(&refPoint1, &iref.get()[3].get());
//   ASSERT_EQ(&refPoint2, &iref.get()[4].get());
// }
