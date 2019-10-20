// #include "test.hpp"
// #include "lib7842/path/referencePath.hpp"

// class ReferencePathTest : public ::testing::Test {
// protected:
//   Vector point1 {5_in, 3_in};
// };

// TEST_F(ReferencePathTest, Constructors) {
//   ReferencePath();
//   ReferencePath({point1});
//   ReferencePath({point1, point1});
// }

// TEST_F(ReferencePathTest, ExtractReference) {
//   ReferencePath path({point1, point1});

//   ReferencePath ipath = path;
//   ASSERT_EQ(ipath.get().size(), 2);
//   for (auto&& point : ipath.get()) {
//     ASSERT_EQ(point.get(), point1);
//   }

//   ReferencePath ipath2 = path;
//   for (size_t i = 0; i < ipath2.get().size(); i++) {
//     ASSERT_EQ(&ipath.get()[i].get(), &ipath2.get()[i].get());
//     ASSERT_EQ(&ipath.get()[i].get(), &point1);
//     ASSERT_EQ(&ipath.get()[i].get(), &path.get()[i].get());
//   }
// }

// TEST_F(ReferencePathTest, ExtractPath) {
//   ReferencePath path({point1, point1, point1});

//   SimplePath ipath = path.generate();
//   ASSERT_EQ(ipath.get().size(), 3);
//   for (auto&& point : ipath.get()) {
//     ASSERT_EQ(*point, point1);
//     ASSERT_NE(point.get(), &point1);
//   }
// }
