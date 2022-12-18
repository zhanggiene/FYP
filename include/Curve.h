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
#include "VisualPoint.h"
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
            : Curve(controlPoints,controlPoints.size() - 1) {
    }
    Curve(const std::vector<Point>& controlPoints,int degree)
            : _toRenew(true),
              _degree(degree),
              _step(0.01f),
              _controlPoints(controlPoints),_thickness(1),_straightLine(true) {
    }
protected:
    bool _toRenew;
    unsigned int _degree;
    float _step;
    std::vector<Point> _controlPoints;
    std::vector<Point> _interpolants;    // old way, for debugging purpose
    std::vector<int> knotVector;
    std::vector<Point> _gradientControlPoints;
    std::vector<Point> _centerPoints;
    std::vector<Point> _centerGradientPoints;
    std::vector<VisualPoint> _normalUp;   //
    std::vector<VisualPoint> _normalDown;
    std::vector<Point> _boundaryPoints;
    std::vector<VisualPoint> _startingBoundaryVisualControlPoints;  // 3
    std::vector<VisualPoint> _startingBoundaryVisualPoints;  // 3
    std::vector<VisualPoint> _endingBoundaryVisualControlPoints;  // 3
    std::vector<VisualPoint> _endingBoundaryVisualPoints;  // 3
    float _thickness;
    bool _straightLine;
    void drawControlPoints()
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i=0;i<_controlPoints.size();i++) {
            _controlPoints[i].draw();
        }
    }


    const Point interpolate(const float t)
    {

        std::vector<Point> controlPointCopy(_controlPoints);
        return deCasteljau(t,controlPointCopy);
    }


    void generateInterpolants()
    {
        this->_interpolants.clear();

        for (float t = 0.0f; t < 1.0f; t += this->_step) {
            this->_interpolants.push_back(interpolate(t));
        }
        this->_interpolants.push_back(interpolate(1.0f));
    }

    void generate()
    {
        // generateInterpolants();
        generateKnotVector();
        generateGradientControlPoint();
        generateCenterPoint();
        generateNormal();
        if (_straightLine) {
            generateEndingBoundary();
            generateStartingBoundary();
        }


    }

    void checkMouseSelection(float xpos, float ypos)
    {
        for (auto &p: _controlPoints) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.checkMouseSelection(xpos, ypos);
        }
    }

    void updatePosition(float xpos, float ypos)
    {
        for (auto &p: _controlPoints) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.updatePosition(xpos, ypos);
        }
        _toRenew=true;
    }
    void generateStartingBoundary()
    {
        _startingBoundaryVisualControlPoints.clear();
        _startingBoundaryVisualControlPoints.push_back(_normalUp.front());
        _startingBoundaryVisualControlPoints.push_back((_normalUp.front()+_normalDown.front())/2);
        _startingBoundaryVisualControlPoints.push_back(_normalDown.front());
        _startingBoundaryVisualPoints.clear();
        for (float i = 0.0f; i < knotVector[_degree + 3]; i += 0.01f )
        {

            for (int j = 0; j < 3; j++)
            {
                if (i >= j)
                {
                    _startingBoundaryVisualPoints.push_back(deBoorClosing(_startingBoundaryVisualControlPoints,2, j, i));

                }
            }
        }

    }

    void generateEndingBoundary()
    {
        _endingBoundaryVisualControlPoints.clear();
        _endingBoundaryVisualControlPoints.push_back(_normalUp.back());
        _endingBoundaryVisualControlPoints.push_back((_normalUp.back()+_normalDown.back())/2);
        _endingBoundaryVisualControlPoints.push_back(_normalDown.back());
        _endingBoundaryVisualPoints.clear();
        for (float i = 0.0f; i < knotVector[_degree + 3]; i += 0.01f )
        {

            for (int j = 0; j < 3; j++)
            {
                if (i >= j)
                {
                    _endingBoundaryVisualPoints.push_back(deBoorClosing(_endingBoundaryVisualControlPoints,2, j, i));

                }
            }
        }


    }

   VisualPoint deBoorClosing(std::vector<VisualPoint>& controlPoints,int r, int i, float u)
    {
        //r is degree, i is no of control points,

        if (r == 0)
        {
            return controlPoints[i];
        }
        else
        {

            float pre = (u - knotVector[i + r]) / (knotVector[i + _degree + 1] - knotVector[i + r]);
            return ((deBoorClosing(controlPoints,r - 1, i, u) * (1 - pre)) + (deBoorClosing(controlPoints,r - 1, i + 1, u) * (pre)));

        }
    }


    void generateNormal()
    {
        _normalUp.clear();
        _normalDown.clear();
        for (int i = 0; i< _centerPoints.size(); i++ ) {
            auto normalizedGradient = _centerGradientPoints[i].position.normalized(); // return a value

            Eigen::Vector2f rotateVectdown = Eigen::Vector2f(-normalizedGradient.y(), normalizedGradient.x());
            _normalDown.push_back(VisualPoint(_centerPoints[i].position + (_centerPoints[i].radius2 * rotateVectdown),
                                              _centerPoints[i].color2));
            _normalUp.push_back(VisualPoint(_centerPoints[i].position + (_centerPoints[i].radius1 * (-rotateVectdown)),
                                            _centerPoints[i].color1));
        }
    }


    void draw() {
        if (_toRenew) {
            _toRenew = false;
            generate();
        }
        drawSkeleton();
        drawBoundary();
        drawEndBoundary();
    }
    void drawEndBoundary()
    {
        // std::cout<<"size of _endingBoundaryVisualPoint is "<<_endingBoundaryVisualPoints.size();
        for(int i=0;i< _endingBoundaryVisualPoints.size()-1;i++) {
            glBegin( GL_LINES);
            //std::cout<<_normalUp[i].position<<std::endl;
            glColor3f(_endingBoundaryVisualPoints[i].color.x,_endingBoundaryVisualPoints[i].color.y,_endingBoundaryVisualPoints[i].color.z);
            glVertex2f(_endingBoundaryVisualPoints[i].position.x(),_endingBoundaryVisualPoints[i].position.y());
            glVertex2f(_endingBoundaryVisualPoints[i+1].position.x(),_endingBoundaryVisualPoints[i+1].position.y());
            glEnd();

        }
        for(int i=0;i< _startingBoundaryVisualControlPoints.size()-1;i++) {
            glBegin( GL_LINES);
            //std::cout<<_normalUp[i].position<<std::endl;
            glColor3f(_startingBoundaryVisualControlPoints[i].color.x,_startingBoundaryVisualControlPoints[i].color.y,_startingBoundaryVisualControlPoints[i].color.z);
            glVertex2f(_startingBoundaryVisualControlPoints[i].position.x(),_startingBoundaryVisualControlPoints[i].position.y());
            glVertex2f(_startingBoundaryVisualControlPoints[i+1].position.x(),_startingBoundaryVisualControlPoints[i+1].position.y());
            glEnd();

        }

    }

    void drawSkeleton()
    {
        // draw skeleton, the center extrapolated thin line
        for(int i=0;i< _centerPoints.size()-1;i++)
        {
            // std::cout<<_centerPoints[i]<<std::endl;
            glBegin( GL_LINES);
            glVertex2f(_centerPoints[i].position.x(),_centerPoints[i].position.y());
            glVertex2f(_centerPoints[i+1].position.x(),_centerPoints[i+1].position.y());
            glEnd();
        }

    }

    void drawBoundary()
    {
        for(int i=0;i< _normalUp.size()-1;i++)
        {
            glBegin( GL_LINES);
            //std::cout<<_normalUp[i].position<<std::endl;
            glColor3f(_normalUp[i].color.x,_normalUp[i].color.y,_normalUp[i].color.z);
            glVertex2f(_normalUp[i].position.x(),_normalUp[i].position.y());
            glVertex2f(_normalUp[i+1].position.x(),_normalUp[i+1].position.y());
            glEnd();
        }
        for(int i=0;i< _normalDown.size()-1;i++)
        {
            glBegin( GL_LINES);
            glColor3f(_normalDown[i].color.x,_normalDown[i].color.y,_normalDown[i].color.z);
            glVertex2f(_normalDown[i].position.x(),_normalDown[i].position.y());
            glVertex2f(_normalDown[i+1].position.x(),_normalDown[i+1].position.y());
            glEnd();
        }
        // std::cout<<"draw boundary,_normalUp.size()"<<_normalUp.size()<<std::endl;
    }
    void generateKnotVector()
    {
        //  needs to be tested
        knotVector.clear();
        int index=0;
        for (int i = 0; i < (_degree + _controlPoints.size() + 1); i++) // n+p+1
        {

            if (i<=_degree) knotVector.push_back(index);// 0
            else if (i<=_controlPoints.size()) knotVector.push_back(++index);
            else knotVector.push_back(index);
        }

        for (auto x:knotVector)
        {
            std::cout<<"  "<<x;
        }
    }
    void generateGradientControlPoint()
    {
        _gradientControlPoints.clear();
        for (int i = 0; i < _controlPoints.size() - 1; i++)
        {
            _gradientControlPoints.push_back((_controlPoints[i + 1] - _controlPoints[i]) / (knotVector[i + _degree + 1] - knotVector[i + 1]));
        }
    }


        // https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
        // maybe can be optimized
        Point deBoor(int r, int i, float t)  // r=degree i is number of control points
        {

                if (r == 0)
                {
                    return _controlPoints[i];
                }
                else {

                    float pre = (t - knotVector[i + r]) / (knotVector[i + _degree + 1] - knotVector[i + r]);
                    return (deBoor(r - 1, i, t) * (1 - pre)) + (deBoor(r - 1, i + 1, t) * (pre));

                }
        }



Point gradientDeboor(int p, int i, float t)
    {
        if (p == 0)
        {
            return _gradientControlPoints[i - 1];
        }
        else
        {
            float pre = (t - knotVector[i + p]) / (knotVector[i + _degree + 1] - knotVector[i + p]);
            return ((gradientDeboor(p - 1, i, t) * (1 - pre)) + (gradientDeboor(p - 1, i + 1, t) * (pre)));

        }
    }
    void generateCenterPoint() {
        _centerPoints.clear();
        _centerGradientPoints.clear();
        for (float i = 0.0f; i < knotVector[_degree + _controlPoints.size()]; i += 0.01f / _controlPoints.size()) {

            for (int j = 0; j < _controlPoints.size(); j++) {
                if (i >= j) {

                    _centerPoints.push_back(deBoor(_degree, j, i));
                    _centerGradientPoints.push_back(gradientDeboor(_degree - 1, j + 1, i) * (float) _degree);
                }
            }
        }
    }


    void drawCurve()
    {
        if (_toRenew) {
            _toRenew = false;
            generateInterpolants();
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
