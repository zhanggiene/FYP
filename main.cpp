#include <iostream>
#include "App.h"
#include <glfw3.h>
#define WINDOW_SIZE 300
#include <Eigen/Dense>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
int main(int argc, char *argv[])
{
 /*

    Canvas testCanvas=Canvas();
    testCanvas.setSize(50);

    for(int i=10;i<30;i++) {
        testCanvas.Mask(10, i) = true;
        testCanvas.imageRed(10, i) = 1;
        testCanvas.imageGreen(10, i) = 1;
        testCanvas.imageBlue(10, i) = 1;
        testCanvas.counter += 1;
    }
    testCanvas.diffuse(1);
    std::cout<<"JIII"<<std::endl;
    */
    /*
    Eigen::Array<bool,Eigen::Dynamic,Eigen::Dynamic> Mask;
    Mask.resize(10,10);
    Mask.fill(false);
    Eigen::Array<float,Eigen::Dynamic,Eigen::Dynamic> imageRed;
    imageRed.resize(10,10);
    Mask(0,0)=true;
    Mask(3,3)=true;
    imageRed(0,0)=4.4;
    imageRed(3,3)=4.3;

    Eigen::ArrayXXf answerR = Eigen::ArrayXXf::Random(10,10);
    std::cout<<"before: ";
    std::cout<<answerR<<std::endl;
    std::cout<<imageRed<<std::endl;
    std::cout<<Mask<<std::endl;
    imageRed=Mask.select(imageRed,answerR);
    std::cout<<"after: ";
    std::cout<<answerR<<std::endl;
    std::cout<<imageRed<<std::endl;
    std::cout<<"after: ";
    answerR.abs(); //  the random number can be -1 to 0
    Eigen::Map<const Eigen::VectorXf> v1(answerR.data(), answerR.size());
    Eigen::Map<const Eigen::VectorXf> imageRedv(imageRed.data(),imageRed.size());
    Eigen::SparseMatrix<float,Eigen::RowMajor>  A;
    std::cout<<v1;
    A.resize(100,100);
    for(int i=0;i<100;i++)
    {
        A.coeffRef(i,i)=1;
    }
    Eigen::BiCGSTAB<Eigen::SparseMatrix<float> > solver;
    solver.compute(A);
    answerR = solver.solve(imageRedv).array();
    std::cout << "estimated error: " << solver.error();
    std::cout<<answerR;

    //Eigen::Map<const Eigen::VectorXd> imageRedv(imageRed.data(), imageRed.size());
    //answerR.abs(); //  the random number can be -1 to 0
     */
    App::initialize(WINDOW_SIZE, WINDOW_SIZE, "Intuitive Colouring");
    return App::run();
    return 1;
}






