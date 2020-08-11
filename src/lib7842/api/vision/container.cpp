#include "lib7842/api/vision/container.hpp"
#include <numeric>

namespace lib7842::Vision {

std::vector<Object>& Container::operator()() { return objects; }

const std::vector<Object>& Container::operator()() const { return objects; }

Container& Container::add(const Object& iobj) {
  objects.emplace_back(iobj);
  return *this;
}

Container& Container::add(const std::vector<Object>& iobjects) {
  objects.reserve(objects.size() + iobjects.size());
  objects.insert(objects.end(), iobjects.begin(), iobjects.end());
  return *this;
}

Container& Container::remove(size_t index) {
  if (index < objects.size()) objects.erase(objects.begin() + index);
  return *this;
}

Container& Container::remove(size_t startIndex, size_t endIndex) {
  if (endIndex > objects.size()) endIndex = objects.size();
  if (startIndex < objects.size()) {
    objects.erase(objects.begin() + startIndex, objects.begin() + endIndex);
  }
  return *this;
}

Object Container::get(size_t index) {
  if (index < objects.size()) { return objects.at(index); }
  return {};
}

double Container::get(size_t index, const Query& query) { return get(index).get(query); }

double Container::total(const Query& query) {
  return std::accumulate(objects.begin(), objects.end(), 0.0,
                         [&](double a, const Object& b) { return a + query(b); });
}

double Container::avg(const Query& query) {
  return total(query) / static_cast<double>(objects.size());
}

Container& Container::resize(size_t size) {
  objects.resize(size);
  return *this;
}

Container& Container::trim(size_t size) {
  if (size < objects.size()) resize(size);
  return *this;
}

Container& Container::reset() {
  resize(0);
  return *this;
}

Container& Container::remove(const Query& query, const Compare& comp, double value) {
  remove([&](const Object& obj) { return comp(query(obj), value); });
  return *this;
}

Container& Container::remove(const std::function<bool(const Object&)>& comp) {
  objects.erase(std::remove_if(objects.begin(), objects.end(), comp), objects.end());
  return *this;
}

Container& Container::sort(const Query& query, const Compare& comp) {
  std::stable_sort(objects.begin(), objects.end(),
                   [&](const Object& a, const Object& b) { return comp(query(a), query(b)); });
  return *this;
}

} // namespace lib7842::Vision

#include "lib7842/test.hpp"
namespace test {
using namespace lib7842::Vision;

TEST_CASE("Container") {

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

        THEN("the size should be one") { CHECK(container().size() == 1); }

        THEN("the accessor should work") { CHECK(container.get(0).sig == 1); }

        THEN("removing should work") {
          CHECK(container.remove(0)().size() == 0);
          CHECK(container.remove(0)().size() == 0);
        }

        THEN("removing range should work") {
          CHECK(container.remove(0, 100)().size() == 0);
          CHECK(container.remove(0, 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove(Query::x, std::equal_to<>(), 50)().size() == 1);
          CHECK(container.remove(Query::x, std::equal_to<>(), 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove([](const Object& iobj) { return iobj.x == 0; })().size() == 1);
          CHECK(container.remove([](const Object& iobj) { return iobj.x == 100; })().size() == 0);
        }

        THEN("totaling should work") { CHECK(container.total(Query::x) == 100); }
      }

      WHEN("adding the object multiple times") {
        container.add({obj, obj, obj});

        THEN("the size should be three") { CHECK(container().size() == 3); }

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
          CHECK(container.remove(Query::x, std::equal_to<>(), 50)().size() == 3);
          CHECK(container.remove(Query::x, std::equal_to<>(), 100)().size() == 0);
        }

        THEN("filtering should work") {
          CHECK(container.remove([](const Object& iobj) { return iobj.x == 0; })().size() == 3);
          CHECK(container
                  .remove(
                    [](const Object& iobj) { return Query::x(iobj) > 5 && Query::x(iobj) < 105; })()
                  .size() == 0);
        }

        THEN("totaling should work") { CHECK(container.total(Query::x) == 300); }

        THEN("averaging should work") { CHECK(container.avg(Query::x) == 100); }

        THEN("trimming should work") {
          CHECK(container.trim(2)().size() == 2);
          CHECK(container.trim(10)().size() == 2);
        }

        THEN("resizing should work") {
          CHECK(container.resize(2)().size() == 2);
          CHECK(container.resize(10)().size() == 10);
        }

        THEN("resetting should work") { CHECK(container.reset()().size() == 0); }
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
        container.sort(Query::x, std::less<>());

        THEN("the container should be sorted") {
          for (size_t i = 0; i < container().size(); i++) {
            CHECK(container.get(i).sig == i + 1);
          }
        }

        WHEN("the container is sorted again") {
          container.sort(Query::x, std::greater<>());

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
} // namespace test
