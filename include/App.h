//
// Created by Zhang Zhuyan on 3/12/22.
// https://www.programmersought.com/article/85305491112/
// https://www.youtube.com/watch?v=Cut3_A0Xm3Y&ab_channel=AndresAlvarez 

#ifndef FYP_APP_H
#define FYP_APP_H

#include <string>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "Point.h"
#include "Canvas.h"

class App
{
public:
    static bool initialize(int width, int height, const char *title);
    static int run();
    static GLFWwindow* _window;
    static std::vector<Point > _points;
    static float r1;
    static float r2;
    static bool showPopUp;
    static Point& draggablePoint;
    static bool finalImageBool;
    static GLuint image_tex_;
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void keyboard_press_callback(GLFWwindow *window,int key, int scancode, int action, int mods);
    static void addCurve();
    static GLuint LoadImage();
    static void HelpMarker(const char* desc);
private:
    static void draw(GLuint texture);
    static Canvas _canvas;
    static ImVec4 color1;
    static ImVec4 color2;
    static bool editMode;
    static void clear();
    static int size;
    static std::vector<float> data;
};
#endif //FYP_APP_H
