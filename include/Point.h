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
#include <math.h>

#define POINT_SIZE 10
class Point
{
public:
    ImVec4 color1;
    ImVec4 color2;
    Eigen::Vector2f position;
    ImVec4 colorOuter1;
    ImVec4 colorOuter2;
    static const unsigned int r=10;
    bool isSelected;
    float radius1;
    float radius2;

    Point(ImVec4 color1_,ImVec4 color2_,float x_,float y_,float r1_,float r2_):
    color1(color1_), color2(color2_) ,position(x_,y_),isSelected(false),radius1(r1_),radius2(r2_)
    {
    }
    Point(ImVec4 color1_,ImVec4 color2_,Eigen::Vector2f position_,float r1_,float r2_):
            color1(color1_), color2(color2_) ,position(position_),isSelected(false),radius1(r1_),radius2(r2_)
    {

    }

    // Copy constructor
    Point(const Point& p1)
    {
        // std::cout<<"copy constructor used"<<std::endl;
        color1 = p1.color1;
        color2 = p1.color2;
        position=p1.position;
        radius1=p1.radius1;
        radius2=p1.radius2;


    }

    Point (Point&& other)
    {
        color1=std::move(other.color1);
        color2=std::move(other.color2);
        position=std::move(other.position);
        radius1=other.radius1;
        radius2=other.radius2;
    }

    void checkMouseSelection(float x,float y)
    {
        if(abs(position.x()-x)<2*r && abs(position.y()-y)<2*r)
        {
            // highlighted by the mouse
            isSelected=true;
        }
        else
        {
            isSelected=false;
        }

    }

    void updatePosition(float x,float y)
    {
        if (isSelected)
        {
            position.x()=x;
            position.y()=y;
        }
    }
    void draw()
    {
        if (isSelected) {
            drawFilledCircle(position.x(), position.y());
        }
        else drawCircle(position.x(),position.y());
    }

        void drawFilledCircle(float cx, float cy, int num_segments=20){
            //static float angle;
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glColor3ub(255, 0, 0);
            float twicePi = 2.0 * 3.142;
            glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
            glVertex2f(cx, cy); // center of circle
            for (int i = 0; i <= num_segments; i++)   {
                glVertex2f (
                        (cx + (r* cos(i * twicePi / num_segments))), (cy + (r * sin(i * twicePi / num_segments)))
                );
            }
            glColor3f(1, 1, 1);
            glEnd(); //END
    }

    void drawCircle(float cx, float cy, int num_segments=100) {
        glLineWidth(10);
        glEnable(GL_LINE_SMOOTH);
        glColor3ub(0, 255, 0);
        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < num_segments; ii++)   {
            float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
            float x = r * cosf(theta);//calculate the x component
            float y = r * sinf(theta);//calculate the y component
            glVertex2f(x + cx, y + cy);//output vertex
        }
        glLineWidth(1);
        glColor3f(1, 1, 1);
        glEnd();
    }

    Point& operator=(const Point& other)
    {
        if (this == &other)
            return *this;
        color1=other.color1;
        color2=other.color2;
        position=other.position;
        radius1=other.radius1;
        radius2=other.radius2;
        return *this;
    }

    Point operator+(const Point &p) const
    {
        Point ret=p;
        ret.position+=position;
        ret.color1.x+=(color1.x);
        ret.color1.y+=(color1.y);
        ret.color1.z+=(color1.z);
        ret.color2.x+=(color2.x);
        ret.color2.y+=(color2.y);
        ret.color2.z+=(color2.z);
        ret.radius1+=radius1;
        ret.radius2+=radius2;
        return ret;
    }

    Point operator-(const Point &p) const
    {
        Point ret=p;
        ret.position-=position;
        ret.color1.x-=(color1.x);
        ret.color1.y-=(color1.y);
        ret.color1.z-=(color1.z);
        ret.color2.x-=(color2.x);
        ret.color2.y-=(color2.y);
        ret.color2.z-=(color2.z);
        ret.radius1-=radius1;
        ret.radius2-=radius2;
        return ret;
    }

    Point operator*(float s) const
    {
        Point ret=*this;
        ret.position*=s;
        ret.color1.x*=s;
        ret.color1.y*=s;
        ret.color1.z*=s;
        ret.color2.x*=s;
        ret.color2.y*=s;
        ret.color2.z*=s;
        ret.radius1*=s;
        ret.radius2*=s;
        return ret;
    }

    Point operator/(float s) const
    {
        Point ret=*this;
        ret.position/=s;
        ret.color1.x/=s;
        ret.color1.y/=s;
        ret.color1.z/=s;
        ret.color2.x/=s;
        ret.color2.y/=s;
        ret.color2.z/=s;
        ret.radius1/=s;
        ret.radius2/=s;

        return ret;
    }

    void operator+=(const Point &p)
    {
        position+=p.position;

        color1.x+=(p.color1.x);
        color1.y+=(p.color1.y);
        color1.z+=(p.color1.z);
        color2.x+=(p.color2.x);
        color2.y+=(p.color2.y);
        color2.z+=(p.color2.z);
        radius1+=p.radius1;
        radius2+=p.radius2;

    }

    void operator-=(const Point &p)
    {
        position-=p.position;

        color1.x-=(p.color1.x);
        color1.y-=(p.color1.y);
        color1.z-=(p.color1.z);
        color2.x-=(p.color2.x);
        color2.y-=(p.color2.y);
        color2.z-=(p.color2.z);
        radius1-=p.radius1;
        radius2-=p.radius2;

    }

    void operator*=(float s)
    {
        position*=s;

        color1.x*=s;
        color1.y*=s;
        color1.z*=s;
        color2.x*=s;
        color2.y*=s;
        color2.z*=s;
        radius1*=s;
        radius2*=s;

    }

    void operator/=(float s)
    {
        position/=s;

        color1.x/=s;
        color1.y/=s;
        color1.z/=s;
        color2.x/=s;
        color2.y/=s;
        color2.z/=s;
        radius1/=s;
        radius2/=s;

    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        os << p.position<<"radius 1 "<<p.radius1<<" radius 2 "<<p.radius2<<".    ";
        return os << " }";
    }
};





#endif //FYP_POINT_H
