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
#include <boost/json.hpp>
class Curve {
private:
    friend class Canvas;
     Point deCasteljau(const float t, std::vector<Point>& points);
private:
    typedef std::function<void()> some_void_function_type;
    some_void_function_type f_;

public:
    Curve(const std::vector<Point>& controlPoints);
    Curve(const std::vector<Point>& controlPoints,int degree);
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
    bool _visibleControlPoint;
    void drawControlPoints();

    void cleanDeletedPoints();



    const Point interpolate(const float t);

    void setCallBack(some_void_function_type f);


    void generateInterpolants();

    void generate();

    void checkMouseSelection(float xpos, float ypos);

    void updatePosition(float xpos, float ypos);
    void generateStartingBoundary();

    void generateEndingBoundary();

   VisualPoint deBoorClosing(std::vector<VisualPoint>& controlPoints,int r, int i, float u);


    void generateNormal();


    void draw();
    void drawEndBoundary();

    void drawSkeleton();

    void drawBoundary();
    void generateKnotVector();
    void generateGradientControlPoint();


        // https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
        // maybe can be optimized
        Point deBoor(int r, int i, float t) ;



Point gradientDeboor(int p, int i, float t);
    void generateCenterPoint();
    void showCruveproperty(const char* prefix, int uid);

    void drawCurve();

};

#endif //FYP_CURVE_H
