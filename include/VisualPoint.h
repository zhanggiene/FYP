//
// Created by Zhang Zhuyan on 13/12/22.
//

#ifndef FYP_VISUALVisualPoint_H
#define FYP_VISUALVisualPoint_H
#include <Eigen/Dense>
#include <iostream>
#include "imgui.h"
// secondary VisualPoint that derived from main control VisualPoint

class VisualPoint {
private:
    static const unsigned int POINT_SIZE_v=3;
    // without const, it has to be out of class
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
    VisualPoint (VisualPoint&& other)
    {
        color=std::move(other.color);
        position=std::move(other.position);
    }

    VisualPoint operator+(const VisualPoint &p) const
    {
        VisualPoint ret=p;
        ret.position+=position;
        ret.color.x+=color.x;
        ret.color.y+=color.y;
        ret.color.z+=color.z;
        return ret;
    }

    VisualPoint operator-(const VisualPoint &p) const
    {
        VisualPoint ret=p;
        ret.position-=position;
        ret.color.x-=color.x;
        ret.color.y-=color.y;
        ret.color.z-=color.z;
        return ret;
    }

    VisualPoint operator*(float s) const
    {
        VisualPoint ret=*this;
        ret.position*=s;
        ret.color.x*=s;
        ret.color.y*=s;
        ret.color.z*=s;
        return ret;
    }

    VisualPoint operator/(float s) const
    {
        VisualPoint ret=*this;
        ret.position/=s;
        ret.color.x/=s;
        ret.color.y/=s;
        ret.color.z/=s;
        return ret;
    }

    void operator+=(const VisualPoint &p)
    {
        position+=p.position;
        color.x+=p.color.x;
        color.y+=p.color.y;
        color.z+=p.color.z;
    }

    void operator-=(const VisualPoint &p)
    {
        position-=p.position;
        color.x-=p.color.x;
        color.y-=p.color.y;
        color.z-=p.color.z;
    }

    void operator*=(float s)
    {
        position*=s;
        color.x*=s;
        color.y*=s;
        color.z*=s;
    }

    void operator/=(float s)
    {
        position/=s;
        color.x/=s;
        color.y/=s;
        color.z/=s;

    }

    friend std::ostream &operator<<(std::ostream &os, const VisualPoint &p)
    {
        os << "color "<<p.color.x<<""<<p.color.x<<p.color.z;
        return os << " }";
    }

    void draw()
    {
        glColor3f(1, 0, 0);
        float twicePi = 2.0 * 3.142;
        glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
        glVertex2f(position.x(), position.y()); // center of circle
        for (int i = 0; i <= 20; i++)   {
            glVertex2f (
                    (position.x() + (POINT_SIZE_v* cos(i * twicePi / 20))), (position.y() + (POINT_SIZE_v * sin(i * twicePi / 20)))
            );
        }
        glColor3f(1, 1, 1);
        glEnd(); //END
    }
};
#endif //FYP_VISUALVisualPoint_H
