//
// Created by Zhang Zhuyan on 4/12/22.
//

#ifndef FYP_CANVAS_H
#define FYP_CANVAS_H
#include "Curve.h"
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <amgcl/make_solver.hpp>
#include <amgcl/solver/bicgstab.hpp>
#include <amgcl/amg.hpp>
#include <amgcl/coarsening/smoothed_aggregation.hpp>
#include <amgcl/relaxation/spai0.hpp>
#include <amgcl/adapter/crs_tuple.hpp>
#include <amgcl/adapter/eigen.hpp>   // important
//https://gist.github.com/insaneyilin/038a022f2ece61c923315306ddcea081
// https://nehe.gamedev.net/tutorial/texture_mapping/12038/
class Canvas
{
public:
    bool displayMode;
    unsigned  int size_;
    Eigen::ArrayXf imageRed;  // row major because we want to flatten i
    Eigen::ArrayXf imageGreen;
    Eigen::ArrayXf imageBlue;
    Eigen::Array<bool,Eigen::Dynamic,1> Mask;
    std::vector<float> data; // for giving to opengl
    Eigen::SparseMatrix<float,Eigen::RowMajor>  A;
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
    void initializeTexture();
    GLuint texture;
    void cleardiffuseImage();
    void constructA(int size);
    void multigrid();
    void ShowPropertyEditor(bool* p_open);
    void ShowPlaceholderObject(const char* prefix, int uid);
};
#endif //FYP_CANVAS_H
