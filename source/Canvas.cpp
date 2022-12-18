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
        _curves[i]->draw();
    }
    glFlush();

}

void Canvas::diffuse(int iteration){
    A=Eigen::SparseMatrix<double>(size_*size_,size_*size_);
    for(unsigned A_row=0;A_row<size_* size_;A_row++)
    {
        for(int temp=0;temp<size_;temp++)
        {
            A.coeffRef(A_row,0) = 1;
            A.coeffRef(A_row,size_*size_-1) = 1;
            A.coeffRef(A_row,temp*size_)=1;
            A.coeffRef(A_row,temp*size_-1)=1;

        }

    for(unsigned  i =1;i<size_-1;i++)
    {
        for(unsigned  j=1;j<size_-1;j++)
        {
         if (Mask(i,j)) A.coeffRef(i,j)=4;
         else
            {
                A.coeffRef(A_row,i*j)=4;
                A.coeffRef(A_row,i*j-1)=-1;
                A.coeffRef(A_row,i*j+1)=-1;
                A.coeffRef(A_row,(i-1)*j)=-1;
                A.coeffRef(A_row,(i+1)*j)=-1;
            }
        }
    }
    };
    Eigen::ArrayXXd answerR = Eigen::ArrayXXd::Random(size_,size_);
    Eigen::ArrayXXd answerG = Eigen::ArrayXXd::Random(size_,size_);
    Eigen::ArrayXXd answerB = Eigen::ArrayXXd::Random(size_,size_);
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double> > solver;
    solver.compute(A);
    answerR=Mask.select(imageRed,answerR);
    answerG=Mask.select(imageGreen,answerG);
    answerB=Mask.select(imageBlue,answerB);
    answerR.abs(); //  the random number can be -1 to 0
    answerG.abs();
    answerB.abs();
    int temp_=0;
    for(temp_=0;temp_<iteration;temp_++)
    {

        answerR = (solver.solveWithGuess	(imageRed.matrix(),answerR.matrix())).array();
        answerG = (solver.solveWithGuess	(imageGreen.matrix(),answerG.matrix())).array();
        answerB = (solver.solveWithGuess	(imageBlue.matrix(),answerB.matrix())).array();

        if (temp_<iteration-1)
        {
            answerR = Mask.select(imageRed, answerR);
            answerG = Mask.select(imageGreen, answerG);
            answerB = Mask.select(imageBlue, answerB);
        }
        answerR = answerR.cwiseMax(0);
        answerG = answerG.cwiseMax(0);
        answerB = answerB.cwiseMax(0);


    }
}

void Canvas::addVisualPoint(const VisualPoint &p_) {

    imageRed(int(p_.position.x()),int(p_.position.y())) = p_.color.x;
    imageGreen(int(p_.position.x()),int(p_.position.y())) = p_.color.y;
    imageBlue(int(p_.position.x()),int(p_.position.y())) = p_.color.z;
    Mask(int(p_.position.x()),int(p_.position.y()))=true;
}
void Canvas::drawToImage()
{
    for (auto curve : _curves)
    {
        for (int i=0; i<curve->_normalUp.size();i++)
        {
            int shiftx=-1;
            if (curve->_normalUp[i].position.x() < curve->_centerPoints[i].position.x()) {
                shiftx = 1;
            }
            int shifty=-1;
            if (curve->_normalUp[i].position.y() < curve->_centerPoints[i].position.y()) {
                shifty = 1;
            }
            int xpos= int (curve->_normalUp[i].position.x())+shiftx;
            int ypos= int (curve->_normalUp[i].position.y())+shifty;
            addVisualPointXY(xpos,ypos, curve->_normalUp[i].color);
            xpos= int (curve->_normalUp[i].position.x())-shiftx;
            ypos= int (curve->_normalUp[i].position.y())-shifty;
            addVisualPointXY(xpos,ypos, curve->_centerPoints[i].colorOuter1);

        }
        for (int i=0; i<curve->_normalDown.size();i++)
        {
            int shiftx=-1;
            if (curve->_normalDown[i].position.x() < curve->_centerPoints[i].position.x()) {
                shiftx = 1;
            }
            int shifty=-1;
            if (curve->_normalDown[i].position.y() < curve->_centerPoints[i].position.y()) {
                shifty = 1;
            }
            int xpos= int (curve->_normalDown[i].position.x())+shiftx;
            int ypos= int (curve->_normalDown[i].position.y())+shifty;
            addVisualPointXY(xpos,ypos, curve->_normalUp[i].color);
            xpos= int (curve->_normalDown[i].position.x())-shiftx;
            ypos= int (curve->_normalDown[i].position.y())-shifty;
            addVisualPointXY(xpos,ypos, curve->_centerPoints[i].colorOuter2);

        }



    }
}

void Canvas::addVisualPointXY(int x, int y, const ImVec4 &color) {


    imageRed(x,y) = color.x;
    imageGreen(x,y) = color.y;
    imageBlue(x,y) = color.z;
    Mask(x,y)=true;


}

void Canvas::checkMouseSelection(float xpos, float ypos)
{
    for (auto c: _curves) {
        // std::cout<<"drawing"<<std::endl;
        c->checkMouseSelection(xpos, ypos);
    }
}
