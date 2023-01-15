//
// Created by Zhang Zhuyan on 3/12/22.
//

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <iostream>

#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "App.h"
#include "internal_string.h"
#include <glfw3.h>
void key_callback(GLFWwindow* window_, int key, int scancode, int action, int mode)
{
    //If you press ESC and set windowShouldClose to True, the outer loop will close the application
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, GL_TRUE);
    std::cout<<"ESC"<<mode;
}

void App::draw( GLuint texture) {


    /*
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);   //  this is important

    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0,   0);
    glTexCoord2i(0, 1); glVertex2i(0,   size/2);
    glTexCoord2i(1, 1); glVertex2i(size/2, size/2);
    glTexCoord2i(1, 0); glVertex2i(size/2, 0);

    glEnd();

    // glDeleteTextures(1, &image_tex);
    glDisable(GL_TEXTURE_2D);


    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0);
    glVertex2f(0, 0);
    glVertex2f(size/2, 0);
    glVertex2f(size, size);
    glColor3f(1, 1, 1); // this is important
    // https://stackoverflow.com/questions/8956736/glcolor-coloring-all-textures
    glEnd();
    */
    if (finalImageBool) {
        _canvas.displayFinalImage();
    }

    _canvas.draw();
    for (int i=0; i<_points.size();i++) {
        _points[i].draw();
    }



}

std::string App::getEditMode() {
    if (editMode) {
        return STRING_LABEL::editMode;
    }
    return STRING_LABEL::createMode;
}
void App::clear()
{
    _points.clear();
}
bool App::initialize(int width, int height, const char *title) {

   if (!glfwInit()) {
        std::cout << "INITIALIZER: Failed to initialize GLFW!" << std::endl;
        return false;
    }

    _window = glfwCreateWindow(width, height, "test", NULL, NULL);
    size=width;

    if (_window == NULL) {
        std::cout << "INITIALIZER: Failed to open a window!" << std::endl;
        glfwTerminate();
        return false;
    }
    _canvas.setSize(width);
    glfwMakeContextCurrent(_window); // this one is important to be above opengl call
    //image_tex_=LoadImage();
    _canvas.initializeTexture();


    //Register callback function
    glfwSetKeyCallback(_window, key_callback);
    glfwSetCursorPosCallback(_window, cursor_position_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    glfwSetKeyCallback(_window, keyboard_press_callback);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ImGui::StyleColorsDark();

            ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL2_Init();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, height, 0, 1, -1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return true;

}

int App::run() {
    ImVec4 clear_color = ImVec4(0, 0, 0, 1.00f);
    while(!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow();


        // Create a window called "Hello, world!" and append into it.
        if (showPopUp)
        {ImGui::OpenPopup("Control Point attributes");}
        if (ImGui::BeginPopupModal("Control Point attributes", NULL))
        {
            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.70f;
            // Testing behavior of widgets stacking their own regular popups over the modal.
            ImGui::SetNextItemWidth(w);
            ImGui::ColorPicker3(" color 1 ", (float *) &_points.back().color1,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha); // Edit 3 floats representing a color
            ImGui::SetNextItemWidth(w);
            ImGui::ColorPicker3(" color 2 ", (float *)  &_points.back().color2,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha); // Edit 3 floats representing a color
            ImGui::SetNextItemWidth(w);

            ImGui::ColorPicker3(" color outer 1 ", (float *) &_points.back().colorOuter1,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha); // Edit 3 floats representing a color
            ImGui::SetNextItemWidth(w);
            ImGui::ColorPicker3(" color outer 2 ", (float *)  &_points.back().colorOuter2,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha); // Edit 3 floats representing a color
            ImGui::SetNextItemWidth(w);
            ImGui::SliderFloat("radius 1", &_points.back().radius1, 0.0f, 300.0f);
            ImGui::SetNextItemWidth(w);
            ImGui::SliderFloat("radius 2", &_points.back().radius2, 0.0f, 300.0f);

            if (ImGui::Button("Done")) {
                color1=_points.back().color1;
                color2=_points.back().color2;
                colorOuter1=_points.back().colorOuter1;
                colorOuter2=_points.back().colorOuter2;
                r1=_points.back().radius1;
                r2=_points.back().radius2;
                showPopUp=false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) { deleteLastPoint();showPopUp=false; ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
        //ImGui::BeginChild("blah");
        //ImGui::EndChild();

        int display_w, display_h;
        glfwGetFramebufferSize(_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        draw(image_tex_);

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /******Exchange buffer, update the content on the window******/
        glfwSwapBuffers(_window);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(_window);
    glfwTerminate();
    return 0;
}

void App::keyboard_press_callback(GLFWwindow *window,int key, int scancode, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if (!editMode)
        {
            _canvas.setdisplayMode(true);
            editMode=true;
        }
        else
        {
            _canvas.setdisplayMode(false);
            editMode=false;
        }
        std::cout<<"edit mode is "<<editMode;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {

            std::cout<<"space button clicked is "<<editMode;
            _canvas.drawFinalImage();
            finalImageBool=true;
        }

}

void App::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    //std::cout<<"x: "<<xpos<<"y: "<<ypos<<std::endl;
    ImGuiIO& io = ImGui::GetIO();

        for (auto &p: _points) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.checkMouseSelection(xpos, ypos,mouseSelectionLock);
        }
    _canvas.checkMouseSelection(xpos,ypos,mouseSelectionLock);   // check for points that is already part of the curve
        if (editMode && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                for (auto &p: _points) {   // important here , otherwise, it is a copy, not a reference
                    // std::cout<<"drawing"<<std::endl;
                    p.updatePosition(xpos, ypos);
                }
                _canvas.updatePosition(xpos,ypos);
            }


}

void App::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {

    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action);
    if (io.WantCaptureMouse) return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && editMode==false)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout<<r1<<std::endl;
        std::cout<<r2<<std::endl;
        showPopUp=true;
        _points.emplace_back(color1,color2,colorOuter1,colorOuter2,xpos,ypos,r1,r2);
        std::cout<<"points added "<<color1.x<<"  "<<color1.y<<"   "<<color1.z<<"   "<<xpos<<"   "<<ypos<<"  "<<r1<<"  "<<r2<<" ;";

    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && editMode==false)
    {
        addCurve();
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && editMode)
    {
        std::cout<<"IMGUI windows pop out";
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        for (auto &p: _points) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.checkMouseSelection(xpos, ypos,mouseSelectionLock);
        }


    }

}

void App::HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void App::addCurve() {
    if (!_points.empty())
        {
         std::vector<Point > temp_points=_points;  // make a copy
        outerclass temp(temp_points);
        temp._controlPoints[0].f_();
        std::cout<<"start to move the outer class";
        _canvas._curves.push_back(std::move(temp));  // move semantic
        _canvas.addcallBack();
        _points.clear();
    }
}

void App::cancel()
{
    if (!_points.empty())
    {
        _points.pop_back();
    }
}
void App::deleteLastPoint() {
    _points.pop_back();
}


GLFWwindow* App::_window;
std::vector<Point > App::_points;
Canvas App::_canvas;
ImVec4 App::color1(0.3,0.4,0.2,0.5);
ImVec4 App::color2(0.3,0,0.4,0.5);
ImVec4 App::colorOuter1(0,0,0,1);
ImVec4 App::colorOuter2(0,0,0,1);
float App::r1=10;
float App::r2=10;
bool App::editMode=false;
bool App::finalImageBool=false;
bool App::showPopUp=false;
GLuint App::image_tex_;
int App::size;
std::vector<float> App::data;
bool App::mouseSelectionLock=false;
