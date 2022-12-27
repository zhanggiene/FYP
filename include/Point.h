//
// Created by Zhang Zhuyan on 3/12/22.
//

#ifndef FYP_POINT_H
#define FYP_POINT_H
#include <glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <iostream>
#include <Eigen/Dense>
#include <math.h>
#include <boost/json.hpp>
#define POINT_SIZE 10
class Point
{
private:
    typedef std::function<void()> some_void_function_type;
    some_void_function_type f_;
public:
    ImVec4 color1;
    ImVec4 color2;
    Eigen::Vector2f position;
    ImVec4 colorOuter1;
    ImVec4 colorOuter2;
    static const unsigned int r=5;
    bool isSelected;
    float radius1;
    float radius2;
    bool isDeleted;


    Point(ImVec4 color1_,ImVec4 color2_,float x_,float y_,float r1_,float r2_);
    Point(ImVec4 color1_,ImVec4 color2_,Eigen::Vector2f position_,float r1_,float r2_);

    void setCallBack(some_void_function_type f);

    // Copy constructor
    Point(const Point& p1);

    void showCruveproperty(const char* prefix, int uid);

    Point (Point&& other);
    void checkMouseSelection(float x,float y,bool rightClickFlag=false);

    void updatePosition(float x,float y);
    void drawR1R2();
    void draw();
    void drawFilledCircle(float cx, float cy, int num_segments=20);

    void drawCircle(float cx, float cy, int num_segments=100,int lineWidth=10,float radius=r,float r_c=0,float g=1,float b=0) ;

    Point& operator=(const Point& other);

    Point operator+(const Point &p) const;

    Point operator-(const Point &p) const;
    Point operator*(float s) const;

    Point operator/(float s) const;

    void operator+=(const Point &p);

    void operator-=(const Point &p);

    void operator*=(float s);

    void operator/=(float s);

    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};



#endif //FYP_POINT_H
