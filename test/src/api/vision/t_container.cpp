#include "test.hpp"

using namespace lib7842::Vision;

TEST_CASE("Object test") {

  GIVEN("a container") {
    Container container;

    THEN("the accessors should not throw") {
      container.get(0);
      container.get(100);
      container.get(-1);
      container.remove(0);
      container.remove(100);
      container.remove(-1);
      container.remove(0, 0);
      container.remove(0, 100);
      container.remove(-1, -1);
      container.remove(-1, 100);
    }

    GIVEN("an object with some data") {
      Object obj;
      obj.sig = 1;
      obj.x = 100;
      obj.y = 50;
      obj.width = 150;
      obj.height = 75;

      WHEN("adding the object to the container") {
        container.add(obj);

        THEN("the size should be one") {
          CHECK(container().size() == 1);
        }

        THEN("the accessor should work") {
          CHECK(container.get(0).sig == 1);
        }

        THEN("removing should work") {
          CHECK(container.remove(0)().size() == 0);
          CHECK(container.remove(0)().size() == 0);
        }

        THEN("removing range should work") {
          CHECK(container.remove(0, 100)().size() == 0);
          CHECK(container.remove(0, 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove(Query::x, std::equal_to<double>(), 50)().size() == 1);
          CHECK(container.remove(Query::x, std::equal_to<double>(), 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove([](const Object& iobj) { return iobj.x == 0; })().size() == 1);
          CHECK(container.remove([](const Object& iobj) { return iobj.x == 100; })().size() == 0);
        }

        THEN("totalling should work") {
          CHECK(container.total(Query::x) == 100);
        }
      }

      WHEN("adding the object multiple times") {
        container.add({obj, obj, obj});

        THEN("the size should be three") {
          CHECK(container().size() == 3);
        }

        THEN("the accessor should work") {
          CHECK(container.get(0).sig == 1);
          CHECK(container.get(1).sig == 1);
          CHECK(container.get(2, Query::sig) == 1);
        }

        THEN("removing should work") {
          CHECK(container.remove(2)().size() == 2);
          CHECK(container.remove(1)().size() == 1);
          CHECK(container.remove(0)().size() == 0);
        }

        THEN("removing range should work") {
          CHECK(container.remove(1, 100)().size() == 1);
          CHECK(container.remove(0, 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove(Query::x, std::equal_to<double>(), 50)().size() == 3);
          CHECK(container.remove(Query::x, std::equal_to<double>(), 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove([](const Object& iobj) { return iobj.x == 0; })().size() == 3);
          CHECK(container
                  .remove(
                    [](const Object& iobj) { return Query::x(iobj) > 5 && Query::x(iobj) < 105; })()
                  .size() == 0);
        }

        THEN("totalling should work") {
          CHECK(container.total(Query::x) == 300);
        }

        THEN("averaging should work") {
          CHECK(container.avg(Query::x) == 100);
        }

        THEN("trimming should work") {
          CHECK(container.trim(2)().size() == 2);
          CHECK(container.trim(10)().size() == 2);
        }

        THEN("resizing should work") {
          CHECK(container.resize(2)().size() == 2);
          CHECK(container.resize(10)().size() == 10);
        }

        THEN("resetting should work") {
          CHECK(container.reset()().size() == 0);
        }
      }
    }

    GIVEN("some random data") {
      container.add([]() {
        Object obj;
        obj.sig = 3;
        obj.x = 65;
        return obj;
      }());

      container.add([]() {
        Object obj;
        obj.sig = 1;
        obj.x = 22;
        return obj;
      }());

      container.add([]() {
        Object obj;
        obj.sig = 4;
        obj.x = 89;
        return obj;
      }());

      container.add([]() {
        Object obj;
        obj.sig = 2;
        obj.x = 45;
        return obj;
      }());

      WHEN("the container is sorted") {
        container.sort(Query::x, std::less<double>());

        THEN("the container should be sorted") {
          for (size_t i = 0; i < container().size(); i++) {
            CHECK(container.get(i).sig == i + 1);
          }
        }

        WHEN("the container is sorted again") {
          container.sort(Query::x, std::greater<double>());

          THEN("the container should be sorted") {
            for (size_t i = 0; i < container().size(); i++) {
              CHECK(container.get(i).sig == container().size() - i);
            }
          }
        }
      }
    }
  }
}