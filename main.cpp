#include <iostream>
#include "App.h"
#include <glfw3.h>
#define WINDOW_SIZE 700
#include <Eigen/Dense>
int main(int argc, char *argv[])
{

    App::initialize(WINDOW_SIZE, WINDOW_SIZE, "Intuitive Colouring");

    return App::run();
}



