//
// Created by Zhang Zhuyan on 3/12/22.
//

#ifndef FYP_APP_H
#define FYP_APP_H

#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"


class App
{
public:
    static bool initialize(int width, int height, const char *title);
    static int run();
    static GLFWwindow* _window;
private:
};
#endif //FYP_APP_H
