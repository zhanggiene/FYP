//
// Created by Zhang Zhuyan on 25/1/23.
//
#include "catch2/catch.hpp"
#include "Point.h"
#include "VisualPoint.h"


class IsPointEqualMatcher : public Catch::MatcherBase<Point> {
    Point m_point;
public:
    IsPointEqualMatcher(Point p) : m_point(p){}

    bool match(Point const& current_point) const override {
        return m_point.color1.x == Approx(current_point.color1.x).margin(1e-3);
    }

    std::string describe() const override {
        std::ostringstream ss;
        return ss.str();
    }
};


inline IsPointEqualMatcher IsPointEqual(Point p) {
    return IsPointEqualMatcher(p);
}


class IsVisualPointEqualMatcher : public Catch::MatcherBase<VisualPoint> {
    VisualPoint m_point;
public:
    IsVisualPointEqualMatcher(VisualPoint p) : m_point(p){}

    bool match(VisualPoint const& current_point) const override {
        return m_point.color.x == Approx(current_point.color.x).margin(1e-3);
    }

    std::string describe() const override {
        std::ostringstream ss;
        return ss.str();
    }
};


inline IsVisualPointEqualMatcher IsVisualPointEqual(VisualPoint p) {
    return IsVisualPointEqualMatcher(p);
}

// ...

TEST_CASE("Operation on point class") {
    // infers `double` for the argument type of the matcher
    ImVec4  color1(0.3,0.4,0.2,0.5);
    ImVec4  color2(0.6,0.8,0.4,1.0);
    Eigen::Vector2f position1(30,30);
    Eigen::Vector2f position2(60,60);
    Point point1(color1,color1,color1,color1,position1,1,1);
    point1*=2.0f;
    Point point2(color2,color2,color2,color2,position2,2,2);
    CHECK_THAT(point1, IsPointEqual(point2));
    point1/=2.0f;
    point2/=2.0f;
    CHECK_THAT(point1, IsPointEqual(point2));
    //CHECK(1==1);
}

TEST_CASE("Operation on visual point class") {
    // infers `double` for the argument type of the matcher
    ImVec4  color1(0.3,0.4,0.2,0.5);
    ImVec4  color2(0.6,0.8,0.4,1.0);
    Eigen::Vector2f position1(30,30);
    Eigen::Vector2f position2(60,60);
    VisualPoint point1(position1,color1);
    point1*=2.0f;
    VisualPoint point2(position2,color2);
    CHECK_THAT(point1, IsVisualPointEqual(point2));
    point1/=2.0f;
    point2/=2.0f;
    CHECK_THAT(point1, IsVisualPointEqual(point2));
    //CHECK(1==1);
}

