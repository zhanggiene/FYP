//
// Created by Zhang Zhuyan on 4/12/22.
//

#ifndef FYP_CANVAS_H
#define FYP_CANVAS_H
#include "outerclass.h"
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
#include "tinyfiledialogs.h"
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
    std::vector<outerclass> _curves;
    int counter;
    void loadImage();
    Canvas();
    ~Canvas();
    void setSize(unsigned  int size);
    void clear();
    void draw();
    void addCurve(outerclass&& curve);
    void addVisualPoint(const VisualPoint& p_);
    void addVisualPointXY(int x, int y,const ImVec4& color);
    void drawToImage(); // make the upper and lower boundary into 2 layers.
    void diffuse(int iteration);
    void checkMouseSelection(float xpos, float ypos,bool& lock);
    void updatePosition(float xpos, float ypos);
    void setdisplayMode(bool b_);
    void drawFinalImage();
    void displayFinalImage();
    void initializeTexture();
    GLuint texture;
    void cleardiffuseImage();
    void constructA(int size);
    void multigrid();
    void ShowPropertyEditor();
    void ShowPlaceholderObject(const char* prefix, int uid);
    void ShowAppMainMenuBar();
    friend void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, Canvas const& p );
    char const * lFilterPatterns[1] = { "*.txt"};
    char const * lTheSaveFileName;
	char const * lTheOpenFileName;
    void save();
    FILE * lIn;
    void loadJson();
    char lBuffer[1024];
    void cleanDeletedCurve();
    void addcallBack();
};
#endif //FYP_CANVAS_H
