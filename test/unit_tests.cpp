//
// Created by Zhang Zhuyan on 25/1/23.
//
#include "catch2/catch.hpp"
#include "Point.h"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    std::vector<int> some_vec{ 1, 2, 3 };
    REQUIRE_THAT(some_vec, Catch::Matchers::UnorderedEquals(std::vector<int>{ 3, 2, 1 }));
}

class IsPointEqualMatcher : public Catch::MatcherBase<Point> {
    Point m_point;
public:
    IsPointEqualMatcher(Point p) : m_point(p){}

    bool match(Point const& current_point) const override {
        return true;
    }

    std::string describe() const override {
        std::ostringstream ss;
        ss << "is between ";
        return ss.str();
    }
};


inline IsPointEqualMatcher IsPointEqual(Point p) {
    return IsPointEqualMatcher(p);
}

// ...

TEST_CASE("Numbers are within range") {
    // infers `double` for the argument type of the matcher
    ImVec4  color1(0.3,0.4,0.2,0.5);
    Eigen::Vector2f position1(30,30);
    Point point1(color1,color1,color1,color1,position1,1,1);
    CHECK(3==3);
}

