#include <iostream>
#include "App.h"
#include <glfw3.h>
#define WINDOW_SIZE 1028

int main(int argc, char *argv[])
{
    App::initialize(WINDOW_SIZE, WINDOW_SIZE, "CPSC 645 HW2 - Michael Kosler");

    return App::run();
}



