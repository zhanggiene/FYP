//
// Created by Zhang Zhuyan on 4/12/22.
//

#include "Canvas.h"
Canvas::Canvas()
        :
          displayMode(false),
          _arePointsVisible(true)
{
}
void Canvas::setSize(unsigned int size)
{
    size_=size;
}
void Canvas::clear()
{

}

Canvas::~Canvas()
{
    clear();
}

void Canvas::addCurve(Curve* curve) {
     // area for improvement
    _curves.push_back(curve);

}

void Canvas::draw() {

    for (size_t i = 0; i < _curves.size(); i++) {
        if (_arePointsVisible) {
            _curves[i]->drawControlPoints();
        }
        _curves[i]->drawCurve();
    }
    glFlush();

}
