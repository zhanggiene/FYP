#include <iostream>
#include "App.h"
#include <glfw3.h>
#define WINDOW_SIZE 700
#include <Eigen/Dense>
int main(int argc, char *argv[])
{

        Eigen::Vector3d v(1,2,3);
        std::cout << "0.1 * v =\n" << 0.1 * v << std::endl;
        v*=0.3;
        std::cout << "Now v =\n" << v << std::endl;
    App::initialize(WINDOW_SIZE, WINDOW_SIZE, "Intuitive Colouring");

    return App::run();
}



