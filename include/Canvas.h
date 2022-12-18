//
// Created by Zhang Zhuyan on 4/12/22.
//

#ifndef FYP_CANVAS_H
#define FYP_CANVAS_H
#include "Curve.h"
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>
class Canvas
{
private:
    bool displayMode;
    unsigned  int size_;
    Eigen::ArrayXXd imageRed;  // row major because we want to flatten i
    Eigen::ArrayXXd imageGreen;
    Eigen::ArrayXXd imageBlue;
    Eigen::Array<bool,Eigen::Dynamic,Eigen::Dynamic> Mask;         //  0 means there is a mask
    Eigen::SparseMatrix<double>  A;
    bool _arePointsVisible;
    std::vector<Curve*> _curves;
public:
    Canvas();
    ~Canvas();
    void setSize(unsigned  int size);
    void clear();
    void draw();
    void addCurve(Curve* curve);
    void addVisualPoint(const VisualPoint& p_);
    void addVisualPointXY(int x, int y,const ImVec4& color);
    void drawToImage(); // make the upper and lower boundary into 2 layers.
    void diffuse(int iteration);
    void checkMouseSelection(float xpos, float ypos);
    void updatePosition(float xpos, float ypos);
    void setdisplayMode(bool b_);
};
#endif //FYP_CANVAS_H
