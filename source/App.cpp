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
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
// https://subscription.packtpub.com/book/game-development/9781838986193/2/ch02lvl1sec14/loading-images-with-stb
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
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


#ifdef __APPLE__
    /* We need to explicitly ask for a 3.2 context on OS X */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
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
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL2_Init();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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

        {
            static int counter = 0;

            ImGui::Begin("Control Point");                          // Create a window called "Hello, world!" and append into it.

            // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("r1", &r1, 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("r2", &r2, 0.0f, 100.0f);
            ImGui::ColorEdit3(" color 1 ", (float*)&color1); // Edit 3 floats representing a color
            ImGui::ColorEdit3(" color 2 ", (float*)&color2); // Edit 2

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        draw(image_tex_);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

        glfwMakeContextCurrent(_window);

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
            p.checkMouseSelection(xpos, ypos);
        }
    _canvas.checkMouseSelection(xpos,ypos);
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
        _points.emplace_back(color1,color2,xpos,ypos,r1,r2);
        std::cout<<"points added "<<color1.x<<"  "<<color1.y<<"   "<<color1.z<<"   "<<xpos<<"   "<<ypos<<"  "<<r1<<"  "<<r2<<" ;";

    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && editMode==false)
    {
        addCurve();
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && editMode)
    {

    }

}

void App::addCurve() {
    if (!_points.empty())
    {
        _canvas.addCurve(new Curve(_points));
        _points.clear();
    }
}
GLuint App::LoadImage()
{

    /*
    int w, h, comp;
    const uint8_t* img = stbi_load(  "/Users/zhangzhuyan/Desktop/mosQUIToes/coding/FYP/Mycode/Baseline-Example.jpg", &w, &h, &comp, 3);
    if (img == NULL) {
        std::cout<<" file not found";
    }
    unsigned int depth = 3;

    GLubyte *checkImage = new GLubyte[size * size * depth];

    for(unsigned int ix = 0; ix < size; ++ix)
    {
        for(unsigned int iy = 0; iy < size; ++iy)
        {
            float c=100;

            checkImage[ix * size * depth + iy * depth + 0] = c;   //red
            checkImage[ix * size * depth + iy * depth + 1] = c;   //green
            checkImage[ix * size * depth + iy * depth + 2] = c;   //blue
        }
    }
     */

    //delete [] checkImage;

    //GLuint texture;
    GLuint image_tex;
    glGenTextures(1, &image_tex);
    glBindTexture(GL_TEXTURE_2D,image_tex);
    glTexParameteri(image_tex, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    static Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> A(size,size);
    data.resize(A.size()*3,0);
    // https://github.com/libigl/libigl/blob/main/include/igl/png/writePNG.cpp
    int comp=3;
    for (unsigned i = 0; i<A.rows();++i)
    {
        for (unsigned j = 0; j < A.cols(); ++j)
        {
            data[(j * A.rows() * comp) + (i * comp) + 0] = 1;
            data[(j * A.rows() * comp) + (i * comp) + 1] = 0;
            data[(j * A.rows() * comp) + (i * comp) + 2] = 0;
        }
    }
    A.setConstant(size, size, 0.0f);
    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,            // Internal colour format to convert to
                 size,          // Image width  i.e. 640 for Kinect in standard mode
                 size,          // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 GL_RGB, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                 GL_FLOAT,  // Image data type
                 data.data());        // The actual image data itself
                 return image_tex;
}


GLFWwindow* App::_window;
std::vector<Point > App::_points;
Canvas App::_canvas;
ImVec4 App::color1(0.3,0.4,0.2,0.5);
ImVec4 App::color2(0.3,0,0.4,0.5);
float App::r1=5;
float App::r2=10;
bool App::editMode=false;
bool App::finalImageBool=false;
GLuint App::image_tex_;
int App::size;
std::vector<float> App::data;
