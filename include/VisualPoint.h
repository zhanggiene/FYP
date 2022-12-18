//
// Created by Zhang Zhuyan on 13/12/22.
//

#ifndef FYP_VISUALVisualPoint_H
#define FYP_VISUALVisualPoint_H
#include <Eigen/Dense>
#include <iostream>
#include "imgui.h"
// secondary VisualPoint that derived from main control VisualPoint

#define POINT_SIZE_v 3
class VisualPoint {
public:
    ImVec4 color;
    Eigen::Vector2f position;

    VisualPoint(Eigen::Vector2f position_,ImVec4 color_):
    color(color_),position(position_)
    {
    }
    VisualPoint(Eigen::Vector2f position_):
            position(position_)
    {
    }

    // prepare for chaining operation
    // copy the content  this already constructed object
    VisualPoint& operator=(const VisualPoint& other)
    {
        if (this == &other)
            return *this;
        color=other.color;
        position=other.position;
        return *this;
    }

    // copy constructor
    //https://stackoverflow.com/questions/11255027/why-user-defined-move-constructor-disables-the-implicit-copy-constructor
    VisualPoint (const VisualPoint & vp)
    {
        color=vp.color;
        position=vp.position;
    }

    // move semantics
    // enable move operator used by STL container
    VisualPoint& operator=(VisualPoint&& other)
    {
        if (this == &other)
            return *this;
        color=other.color;
        position=other.position;
        return *this;
    }

    VisualPoint operator+(const VisualPoint &p) const
    {
        VisualPoint ret=p;
        ret.position+=position;
        return ret;
    }

    VisualPoint operator-(const VisualPoint &p) const
    {
        VisualPoint ret=p;
        ret.position-=position;
        return ret;
    }

    VisualPoint operator*(float s) const
    {
        VisualPoint ret(position);
        ret.position*=s;
        return ret;
    }

    VisualPoint operator/(float s) const
    {
        VisualPoint ret(position);
        ret.position/=s;
        return ret;
    }

    void operator+=(const VisualPoint &p)
    {
        position+=p.position;
    }

    void operator-=(const VisualPoint &p)
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

    friend std::ostream &operator<<(std::ostream &os, const VisualPoint &p)
    {
        os << "position:"<<p.position<<"";
        return os << " }";
    }

    void draw()
    {
        glColor3ub(255, 0, 0);
        float twicePi = 2.0 * 3.142;
        glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
        glVertex2f(position.x(), position.y()); // center of circle
        for (int i = 0; i <= 20; i++)   {
            glVertex2f (
                    (position.x() + (POINT_SIZE_v* cos(i * twicePi / 20))), (position.y() + (POINT_SIZE_v * sin(i * twicePi / 20)))
            );
        }
        glEnd(); //END
    }
};
#endif //FYP_VISUALVisualPoint_H
