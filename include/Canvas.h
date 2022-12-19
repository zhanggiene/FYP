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
public:
    bool displayMode;
    unsigned  int size_;
    Eigen::Array<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> imageRed;  // row major because we want to flatten i
    Eigen::Array<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> imageGreen;
    Eigen::Array<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> imageBlue;
    Eigen::Array<bool,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Mask;
    Eigen::SparseMatrix<float>  A;
    bool _arePointsVisible;
    std::vector<Curve*> _curves;
    int counter;
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
    void drawFinalImage();
    void displayFinalImage();
};
#endif //FYP_CANVAS_H
