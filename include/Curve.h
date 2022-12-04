//
// Created by Zhang Zhuyan on 4/12/22.
//

#ifndef FYP_CURVE_H
#define FYP_CURVE_H
#include <iostream>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Point.h"

class Curve {
private:
    friend class Canvas;
     Point deCasteljau(const float t, std::vector<Point>& points)
    {
        if (points.size()==1) return points[0];

        for (size_t i = 0; i < points.size() -1; i++) {
            //std::cout<<"before"<<points[i]<<std::endl;
            //std::cout<<"before"<<points[i] * (1.0f-t)<<std::endl;
            //std::cout<<"before"<<points[i+1] <<std::endl;

            points[i] =  points[i] *(1.0f - t)  + points[i+1] * t;
            //std::cout<<"after "<<points[i]<<std::endl;

        }
        points.pop_back();
        //std::cout<<"DeCasterjau"<<std::endl;

        return deCasteljau(t,points);
    }

public:
    Curve(const std::vector<Point>& controlPoints)
            : _toRenew(true),
              _degree(controlPoints.size() - 1),
              _step(0.01f),
              _controlPoints(controlPoints),_thickness(1) {
    }
protected:
    bool _toRenew;
    unsigned int _degree;
    float _step;
    std::vector<Point> _controlPoints;
    std::vector<Point> _interpolants;
    float _thickness;
    void drawControlPoints()
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        for (auto x: _controlPoints) {
            x.draw();
        }
    }

    const Point interpolate(const float t)
    {

        std::vector<Point> controlPointCopy(_controlPoints);
        return deCasteljau(t,controlPointCopy);
    }


    void redraw()
    {
        this->_interpolants.clear();

        for (float t = 0.0f; t < 1.0f; t += this->_step) {
            this->_interpolants.push_back(interpolate(t));
        }
        this->_interpolants.push_back(interpolate(1.0f));
    }


    void drawCurve()
    {
        if (_toRenew) {
            _toRenew = false;
            redraw();
        }

        glBegin(GL_QUADS);
        for (size_t i = 0; i < _interpolants.size() - 1; i++) {
            // Get the normal
            Point normal = _interpolants[i + 1] - _interpolants[i];

            // Swap the x- and y-coordinates
            float temp = normal.position.x();
            normal.position.x() = -normal.position.y();
            normal.position.y()= temp;

            normal.position.normalize();
            normal *= _thickness;

            glColor3f(_interpolants[i].color1.x, _interpolants[i].color1.y, _interpolants[i].color1.z);
            glVertex2f(_interpolants[i].position.x() + normal.position.x(), _interpolants[i].position.y() + normal.position.y());

            glColor3f(_interpolants[i+1].color1.x, _interpolants[i+1].color1.y, _interpolants[i+1].color1.z);
            glVertex2f(_interpolants[i+1].position.x() + normal.position.x(), _interpolants[i+1].position.y() + normal.position.y());

            glColor3f(_interpolants[i+1].color2.x, _interpolants[i+1].color2.y, _interpolants[i+1].color2.z);
            glVertex2f(_interpolants[i+1].position.x() - normal.position.x(), _interpolants[i+1].position.y() - normal.position.y());

            glColor3f(_interpolants[i].color2.x, _interpolants[i].color2.y, _interpolants[i].color2.z);
            glVertex2f(_interpolants[i].position.x() - normal.position.x(), _interpolants[i].position.y() - normal.position.y());
        }
        glEnd();
    }
};

#endif //FYP_CURVE_H
