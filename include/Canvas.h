//
// Created by Zhang Zhuyan on 4/12/22.
//

#ifndef FYP_CANVAS_H
#define FYP_CANVAS_H
#include "Curve.h"
#include <vector>
class Canvas
{
private:
    bool displayMode;
    unsigned  int size_;
    bool _arePointsVisible;
    std::vector<Curve*> _curves;
public:
    Canvas();
    ~Canvas();
    void setSize(unsigned  int size);
    void clear();
    void draw();
    void addCurve(Curve* curve);
};
#endif //FYP_CANVAS_H
