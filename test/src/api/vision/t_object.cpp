#include "test.hpp"

using namespace lib7842::Vision;

TEST_CASE("Object test") {

  GIVEN("an object") {
    Object obj;

    THEN("all the values should be zero-initialized") {
      CHECK(obj.sig == VISION_OBJECT_ERR_SIG);
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
        CHECK(Query::sig(obj) == 1);
        CHECK(Query::x(obj) == 100);
        CHECK(Query::y(obj) == 50);
        CHECK(Query::width(obj) == 150);
        CHECK(Query::height(obj) == 75);
        CHECK(Query::area(obj) == 11250);
        CHECK(Query::avgDim(obj) == 112.5);
        CHECK(Query::centerX(obj) == 175);
        CHECK(Query::centerY(obj) == 87.5);
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