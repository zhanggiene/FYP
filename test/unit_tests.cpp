//
// Created by Zhang Zhuyan on 25/1/23.
//
#include "catch2/catch.hpp"
#include "ApprovalTests.hpp"
#include "Point.h"
#include "VisualPoint.h"
#include "Curve.h"


class IsPointEqualMatcher : public Catch::MatcherBase<Point> {
    Point m_point;
public:
    IsPointEqualMatcher(Point p) : m_point(p){}

    bool match(Point const& current_point) const override {
        return m_point.color1.x == Approx(current_point.color1.x).margin(1e-3)
        && m_point.color1.y == Approx(current_point.color1.y).margin(1e-3)
        && m_point.color1.z == Approx(current_point.color1.z).margin(1e-3)
        && m_point.color2.x == Approx(current_point.color2.x).margin(1e-3)
        && m_point.color2.y == Approx(current_point.color2.y).margin(1e-3)
        && m_point.color2.z == Approx(current_point.color2.z).margin(1e-3)
        && m_point.radius1== Approx(current_point.radius1).margin(1e-3)
        && m_point.radius2== Approx(current_point.radius2).margin(1e-3)
        && m_point.position.x()== Approx(current_point.position.x()).margin(1e-3)
        && m_point.position.y()== Approx(current_point.position.y()).margin(1e-3);

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
        return m_point.color.x == Approx(current_point.color.x).margin(1e-3)
               && m_point.color.y == Approx(current_point.color.y).margin(1e-3)
               && m_point.color.z == Approx(current_point.color.z).margin(1e-3);
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

TEST_CASE("Multiplication Operation on point class") {
    // infers `double` for the argument type of the matcher
    ImVec4  color1(0.3,0.4,0.2,0.5);
    ImVec4  color2(0.6,0.8,0.4,1.0);
    Eigen::Vector2f position1(30,30);
    Eigen::Vector2f position2(60,60);
    Point point1(color1,color1,color1,color1,position1,1,1);
    point1*=2.0f;
    Point point2(color2,color2,color2,color2,position2,2,2);
    std::cout<<point1;
    std::cout<<point2;
    CHECK_THAT(point1, IsPointEqual(point2));

}

TEST_CASE("Division Operation on point class") {
    // infers `double` for the argument type of the matcher
    ImVec4  color1(0.3,0.4,0.2,0.5);
    ImVec4  color2(0.6,0.8,0.4,1.0);
    Eigen::Vector2f position1(30,30);
    Eigen::Vector2f position2(60,60);
    Point point1(color1,color1,color1,color1,position1,1,1);
    Point point2(color2,color2,color2,color2,position2,2,2);
    point2/=2.0f;
    CHECK_THAT(point1, IsPointEqual(point2));
    //CHECK(1==1);
}

TEST_CASE("Load Json to create multiple curves") {
    // infers `double` for the argument type of the matcher
    std::vector<Curve > temp_curves;
     boost::json::stream_parser p;
     p.write(R"([{"points":[{"position_x":30,"position_y":60,"color1_x":3.0000001192092896E-1,"color1_y":4.000000059604645E-1,"color1_z":2.0000000298023224E-1,"color2_x":4.000000059604645E-1,"color2_y":5E-1,"color2_z":3.0000001192092896E-1,"colorouter1_x":3.0000001192092896E-1,"colorouter1_y":4.000000059604645E-1,"colorouter1_z":2.0000000298023224E-1,"colorouter2_x":4.000000059604645E-1,"colorouter2_y":5E-1,"colorouter2_z":3.0000001192092896E-1,"radius1":1E0,"radius2":1E0},{"position_x":40,"position_y":70,"color1_x":3.0000001192092896E-1,"color1_y":4.000000059604645E-1,"color1_z":2.0000000298023224E-1,"color2_x":4.000000059604645E-1,"color2_y":5E-1,"color2_z":3.0000001192092896E-1,"colorouter1_x":3.0000001192092896E-1,"colorouter1_y":4.000000059604645E-1,"colorouter1_z":2.0000000298023224E-1,"colorouter2_x":4.000000059604645E-1,"colorouter2_y":5E-1,"colorouter2_z":3.0000001192092896E-1,"radius1":1E0,"radius2":1E0},{"position_x":50,"position_y":80,"color1_x":3.0000001192092896E-1,"color1_y":4.000000059604645E-1,"color1_z":2.0000000298023224E-1,"color2_x":4.000000059604645E-1,"color2_y":5E-1,"color2_z":3.0000001192092896E-1,"colorouter1_x":3.0000001192092896E-1,"colorouter1_y":4.000000059604645E-1,"colorouter1_z":2.0000000298023224E-1,"colorouter2_x":4.000000059604645E-1,"colorouter2_y":5E-1,"colorouter2_z":3.0000001192092896E-1,"radius1":1E0,"radius2":1E0}]}]
)");
     temp_curves = boost::json::value_to<std::vector<Curve> >(p.release());
    CHECK(temp_curves.size() == 1);
}

TEST_CASE("Load Json to create one Point") {
    // infers `double` for the argument type of the matcher
    Point point;
    ImVec4  color1(0.2,0.4,0.6,1.0);
    Eigen::Vector2f position1(30,60);
    Point point1(color1,color1,color1,color1,position1,10,20);
    boost::json::stream_parser p;
    p.write(R"({"position_x":30,"position_y":60,"color1_x":0.2,"color1_y":0.4,"color1_z":0.6,"color2_x":0.2,"color2_y":0.4,"color2_z":0.6,"colorouter1_x":0.2,"colorouter1_y":0.4,"colorouter1_z":0.6,"colorouter2_x":0.2,"colorouter2_y":0.4,"colorouter2_z":0.6,"radius1":10,"radius2":20}
)");
    point = boost::json::value_to<Point>(p.release());
    CHECK_THAT(point1, IsPointEqual(point));
}

TEST_CASE("Multiplication Operation on visual point class") {
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
}


TEST_CASE("Division Operation on visual point class") {
    // infers `double` for the argument type of the matcher
    ImVec4  color1(0.3,0.4,0.2,0.5);
    ImVec4  color2(0.6,0.8,0.4,1.0);
    Eigen::Vector2f position1(30,30);
    Eigen::Vector2f position2(60,60);
    VisualPoint point1(position1,color1);
    point1*=2.0f;
    VisualPoint point2(position2,color2);
    CHECK_THAT(point1, IsVisualPointEqual(point2));
}


