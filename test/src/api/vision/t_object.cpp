#include "test.hpp"

using namespace lib7842::Vision;

TEST_CASE("Object test") {

  GIVEN("an object") {
    Object obj;

    THEN("all the values should be zero-initialized") {
      CHECK(obj.sig == 0);
      CHECK(obj.x == 0);
      CHECK(obj.y == 0);
      CHECK(obj.width == 0);
      CHECK(obj.height == 0);
    }

    GIVEN("some data") {
      obj.sig = 1;
      obj.x = 100;
      obj.y = 50;
      obj.width = 150;
      obj.height = 75;

      THEN("all the data should be accesible") {
        CHECK(obj.get(Query::sig) == 1);
        CHECK(obj.get(Query::x) == 100);
        CHECK(obj.get(Query::y) == 50);
        CHECK(obj.get(Query::width) == 150);
        CHECK(obj.get(Query::height) == 75);
      }

      THEN("calculations on the data should work") {
        CHECK(obj.get(Query::area) == 11250);
        CHECK(obj.get(Query::avgDim) == 112.5);
        CHECK(obj.get(Query::centerX) == 175);
        CHECK(obj.get(Query::centerY) == 87.5);
      }

      THEN("alternative query method should work") {
        CHECK(Query::sig.getFrom(obj) == 1);
        CHECK(Query::x.getFrom(obj) == 100);
        CHECK(Query::y.getFrom(obj) == 50);
        CHECK(Query::width.getFrom(obj) == 150);
        CHECK(Query::height.getFrom(obj) == 75);
        CHECK(Query::area.getFrom(obj) == 11250);
        CHECK(Query::avgDim.getFrom(obj) == 112.5);
        CHECK(Query::centerX.getFrom(obj) == 175);
        CHECK(Query::centerY.getFrom(obj) == 87.5);
      }

      THEN("a custom query should work") {
        Query query = [](const Object& iobj) {
          return iobj.x + iobj.y;
        };

        CHECK(obj.get(query) == 150);
        CHECK(query.getFrom(obj) == 150);
      }
    }
  }
}