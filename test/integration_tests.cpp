//
// Created by Zhang Zhuyan on 17/12/22.
//


#include "catch2/catch.hpp"
#include "ApprovalTests.hpp"
#include "Point.h"
#include "Curve.h"

TEST_CASE("Curve Formation")
{
std::vector<Point > temp_points;  // copy constructor as new object(vector as well) is being
Point point1(ImVec4(0.3,0.4,0.2,1),ImVec4(0.4,0.5,0.3,1),ImVec4(0.3,0.4,0.2,1),ImVec4(0.4,0.5,0.3,1),Eigen::Vector2f(30,60),1,1);
Point point2(ImVec4(0.3,0.4,0.2,1),ImVec4(0.4,0.5,0.3,1),ImVec4(0.3,0.4,0.2,1),ImVec4(0.4,0.5,0.3,1),Eigen::Vector2f(40,70),1,1);
Point point3(ImVec4(0.3,0.4,0.2,1),ImVec4(0.4,0.5,0.3,1),ImVec4(0.3,0.4,0.2,1),ImVec4(0.4,0.5,0.3,1),Eigen::Vector2f(50,80),1,1);
temp_points.push_back(point1);
temp_points.push_back(point2);
temp_points.push_back(point3);

std::vector<Curve > temp_curves;
Curve temp(temp_points);
temp.generate();
temp_curves.push_back(std::move(temp));

std::string jsonString=serialize( boost::json::value_from(temp_curves));
ApprovalTests::Approvals::verify(jsonString);
//temp.multigrid();
//temp._controlPoints[0].f_();
}