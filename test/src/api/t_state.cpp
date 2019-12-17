#include "test.hpp"

SCENARIO("State test") {

  GIVEN("a default-constructed state") {
    State state;
    THEN("the members should be equal to 0") {
      CHECK(state == (State {0_in, 0_in, 0_rad}));
    }
  }

  GIVEN("a state and a vector") {
    State state {5_in, 2_in, 3_rad};
    Vector point {5_in, 2_in};

    THEN("all the constructors should work") {
      State(5_in, 2_in);
      State();

      State pointCopy(point);
      State copy(state);
    }

    THEN("the state should contain the proper info") {
      CHECK(state.x == 5_in);
      CHECK(state.y == 2_in);
      CHECK(state.theta == 3_rad);
    }

    WHEN("the copy constructor is called") {
      State state2(state);

      THEN("the new state should be equal to the old state") {
        CHECK(state2.x == state.x);
        CHECK(state2.y == state.y);
        CHECK(state2.theta == state.theta);
      }
    }

    GIVEN("a state added to itself") {
      State state2 = state + state;

      THEN("the new state members should be twice the old state members") {
        CHECK(state2.x == state.x * 2);
        CHECK(state2.y == state.y * 2);
        CHECK(state2.theta == state.theta * 2);
      }

      THEN("the new state should be the old state times two") {
        CHECK(Vector(state2) == state * 2);
      }

      THEN("the new state should be the old state divided by .5") {
        CHECK(Vector(state2) == state / 0.5);
      }
    }

    GIVEN("a state substracted by itself") {
      State state2 = state - state;

      THEN("the new state members should be 0") {
        CHECK(state2.x == 0_in);
        CHECK(state2.y == 0_in);
        CHECK(state2.theta == 0_rad);
      }

      THEN("the new state should be equal to a default state") {
        CHECK(state2 == State());
      }

      THEN("the new state should be the old state times 0") {
        CHECK(Vector(state2) == state * 0);
      }
    }

    THEN("the state should be equal to itself") {
      CHECK(state == state);
    }

    THEN("the state plus another state should be equal to itself") {
      CHECK((state + State {1_in, 2_in, 4_rad}) == (state + State {1_in, 2_in, 4_rad}));
      CHECK((state + state) == (state + State {5_in, 2_in, 3_rad}));
    }

    THEN("the state should not be equal to some different states") {
      CHECK(state != state + state);
      CHECK(state != state + (State {0_in, 0_in, 4_rad}));
    }

    THEN("the accessor operators should work") {
      CHECK(state.at(0) == state.x);
      CHECK(state.at(1) == state.y);
      CHECK_THROWS_AS(state.at(2), std::runtime_error);
      CHECK_THROWS_AS(state.at(5), std::runtime_error);
      CHECK_THROWS_AS(state.at(-1), std::runtime_error);
    }

    THEN("conversions should work properly") {
      CHECK(Vector(State(point)) == point);
      CHECK(point == Vector(state));

      [&](const Vector& ipoint) {
        CHECK(ipoint == Vector(state));
      }(state);

      [&](const State& istate) {
        CHECK(State(Vector(istate)) == State(Vector(state)));
      }(state);

      [&](const State& istate) {
        CHECK(State(Vector(istate)) == State(point));
      }(State(point));
    }
  }
}
