//
// Created by Zhang Zhuyan on 3/12/22.
//

#ifndef FYP_POINT_H
#define FYP_POINT_H
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <iostream>
#include "imgui.h"
#include <Eigen/Dense>

#define POINT_SIZE 10
class Point
{
public:
    ImVec4 color1;
    ImVec4 color2;
    Eigen::Vector2f position;

    Point(ImVec4 color1_,ImVec4 color2_,float x_,float y_):
    color1(color1_), color2(color2_) ,position(x_,y_)
    {

    }
    Point(ImVec4 color1_,ImVec4 color2_,Eigen::Vector2f position_):
            color1(color1_), color2(color2_) ,position(position_)
    {

    }
    // Copy constructor
    Point(const Point& p1)
    {
        // std::cout<<"copy constructor used"<<std::endl;
        color1 = p1.color1;
        color2 = p1.color2;
        position=p1.position;
    }
    void draw()
    {
        //  for now, transparency is not used
        glBegin(GL_QUADS);
        glColor3f(color1.x, color1.y, color1.z);
        glVertex2f(position.x() - POINT_SIZE / 2.0f, position.y() + POINT_SIZE / 2.0f);
        glVertex2f(position.x() - POINT_SIZE / 2.0f, position.y() - POINT_SIZE / 2.0f);

        glColor3f(color2.x,color2.y, color2.z);
        glVertex2f(position.x() + POINT_SIZE / 2.0f, position.y() - POINT_SIZE / 2.0f);
        glVertex2f(position.x() + POINT_SIZE / 2.0f, position.y() + POINT_SIZE / 2.0f);
        glEnd();
    }

    Point& operator=(Point other)
    {
        std::cout << "copy assignment of Point\n";
        color1=other.color1;
        color2=other.color2;
        position=other.position;
        return *this;
    }

    Point operator+(const Point &p) const
    {
        std::cout<<"add operation"<<"before "<< p<< *this<<"after";
        Point ret=p;
        ret.position+=position;
        std::cout<<ret;
        return ret;
    }

    Point operator-(const Point &p) const
    {
        Point ret=p;
        ret.position-=position;
        return ret;
    }

    Point operator*(float s) const
    {
        std::cout<<" operator"<<s<<std::endl;
        Point ret(color1,color2,position);
        ret.position*=s;
        return ret;
    }

    Point operator/(float s) const
    {
        Point ret(color1,color2,position);
        ret.position/=s;
        return ret;
    }

    void operator+=(const Point &p)
    {
        position+=p.position;
    }

    void operator-=(const Point &p)
    {
        position-=p.position;
    }

    void operator*=(float s)
    {
        position*=s;
    }

    void operator/=(float s)
    {
        position/=s;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        os << p.position;
        return os << " }";
    }
};






#endif //FYP_POINT_H
