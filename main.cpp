#include <iostream>
#include "App.h"
#include <glfw3.h>
#define WINDOW_SIZE 700
#include <Eigen/Dense>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

namespace my_app {


struct customer
{
    int id;
    std::string name;
    bool current;
};

struct outer{
    customer c;

};
void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, customer const& c )
{
    jv = {
        { "id" , c.id },
        { "name", c.name },
        { "current", c.current }
    };

}
    void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, outer const& o )
    {
        jv = {
                { "ids" , o.c },
        };

    }
}// namespace my_app
int main(int argc, char *argv[])
{
my_app::customer c{ 1001, "Boost", true };
    my_app::outer oo{c};
std::cout << serialize( boost::json::value_from( oo ) );

    /*std::vector<Point> _points;
    ImVec4  color1(0.3,0.4,0.2,0.5);
    ImVec4  color2(0.3,0.4,0.2,0.5);
    Eigen::Vector2f position1(30,30);
    Eigen::Vector2f position2(34,34);
    Eigen::Vector2f position3(39,39);
    Point point1(color1,color2,position1,30,30);
    _points.emplace_back(color1,color2,position1,30,30);
    _points.emplace_back(color1,color2,position2,30,30);
    _points.emplace_back(color1,color2,position3,30,30);
    Curve testCurve(_points);
    std::cout << boost::json::serialize( boost::json::value_from( point1 ) );
    std::cout << boost::json::serialize( boost::json::value_from( testCurve ) );
     */
    /*
    Canvas testCanvas=Canvas();
    testCanvas.setSize(900);
    for(int i=10;i<30;i++) {
        testCanvas.Mask(10*testCanvas.size_+ i) = true;
        testCanvas.imageRed(10*testCanvas.size_ +i) = 1;
        testCanvas.imageGreen(10*testCanvas.size_+ i) = 1;
        testCanvas.imageBlue(10*testCanvas.size_+ i) = 1;
        testCanvas.counter += 1;
    }
    //testCanvas.constructA(50);
    //testCanvas.diffuse(1);
    //testCanvas.multigrid();
     */
    //std::cout<<"JIII"<<std::endl;
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
    //Eigen::setNbThreads(2);
    //int n = Eigen::nbThreads( );
    //std::cout<< n;
    //App::initialize(WINDOW_SIZE, WINDOW_SIZE, "Intuitive Colouring");
    //return App::run();
    return 1;
}






