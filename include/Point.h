//
// Created by Zhang Zhuyan on 3/12/22.
//

#ifndef FYP_POINT_H
#define FYP_POINT_H
#include <glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <iostream>
#include "imgui.h"
#include <Eigen/Dense>
#include <math.h>

#define POINT_SIZE 10
class Point
{
private:
    typedef std::function<void()> some_void_function_type;
    some_void_function_type f_;
public:
    ImVec4 color1;
    ImVec4 color2;
    Eigen::Vector2f position;
    ImVec4 colorOuter1;
    ImVec4 colorOuter2;
    static const unsigned int r=5;
    bool isSelected;
    float radius1;
    float radius2;
    bool isDeleted;


    Point(ImVec4 color1_,ImVec4 color2_,float x_,float y_,float r1_,float r2_):
    color1(color1_), color2(color2_) ,position(x_,y_),isSelected(false),radius1(r1_),radius2(r2_),isDeleted(false)
    {
    }
    Point(ImVec4 color1_,ImVec4 color2_,Eigen::Vector2f position_,float r1_,float r2_):
            color1(color1_), color2(color2_) ,position(position_),isSelected(false),radius1(r1_),radius2(r2_),isDeleted(false)
    {
    }

    void setCallBack(some_void_function_type f)
    {
            f_ = f;
    }

    // Copy constructor
    Point(const Point& p1)
    {
        // std::cout<<"copy constructor used"<<std::endl;
        color1 = p1.color1;
        color2 = p1.color2;
        position=p1.position;
        radius1=p1.radius1;
        radius2=p1.radius2;
        isDeleted=false;


    }

    void showCruveproperty(const char* prefix, int uid)
    {
        ImGui::PushID(uid);

        // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("points attributes");

        if (node_open)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Button("Delete")) ImGui::OpenPopup("Delete?");

            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("This curve and all the points will be deleted!!.\nThis operation cannot be undone!\n\n");
                ImGui::Separator();

                //static int unused_i = 0;
                //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();isDeleted=true; }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
            for (int i = 0; i < 4; i++)
            {

                ImGui::PushID(i); // Use field index as identifier.
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags =
                        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                if (i==0) ImGui::TreeNodeEx("Color 1", flags, "Field_%d", i);
                else if (i==1 ) ImGui::TreeNodeEx("Color 2", flags, "Field_%d", i);
                else if (i==2 ) ImGui::TreeNodeEx("radius 1", flags, "Field_%d", i);
                else if (i==3 ) ImGui::TreeNodeEx("radius 2", flags, "Field_%d", i);
                //else if (i==3 ) ImGui::TreeNodeEx("radius 2", flags, "Field_%d", i);

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                // add callback!!https://stackoverflow.com/questions/20037427/pass-method-as-callback-from-one-class-to-other-class

                if (i==0 and ImGui::ColorPicker3(" color 1 ", (float *) &color1,ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoAlpha)) f_();
                else if (i==1 and  ImGui::ColorPicker3(" color 2 ", (float *) &color2,ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoAlpha)) f_();
                else if (i==2 and ImGui::DragFloat("##radius 1", &radius1, 1.0f, 0.0f,100.0f)) f_();
                else if (i==3 and ImGui::DragFloat("##radius 2", &radius2, 1.0f, 0.0f,100.0f)) f_();
                //else if (i==3) ImGui::SliderFloat("radius 2", &radius2, 0.0f, 100.0f);
                ImGui::NextColumn();

                ImGui::PopID();

            }
            ImGui::TreePop();
        }
        ImGui::PopID();

    }

    Point (Point&& other)
    {
        color1=std::move(other.color1);
        color2=std::move(other.color2);
        position=std::move(other.position);
        radius1=other.radius1;
        radius2=other.radius2;
        isDeleted=false;

    }

    void checkMouseSelection(float x,float y,bool rightClickFlag=false)
    {
        if(abs(position.x()-x)<4*r && abs(position.y()-y)<4*r)
        {
            // highlighted by the mouse
            isSelected=true;
        }
        else
        {
            isSelected=false;
        }


    }

    void updatePosition(float x,float y)
    {
        if (isSelected)
        {
            position.x()=x;
            position.y()=y;
        }
    }
    void drawMenu()
    {
        //ImGui::Begin("control point");
        // Create a window called "Hello, world!" and append into it.
        //ImGui::SliderFloat("radius 1 is = %f",&position.x(),0.0f,50.0f);

        //ImGui::BeginChild("blah");
        //ImGui::EndChild();r1
        //ImGui::End();
    }
    void drawR1R2()
    {
        drawCircle(position.x(), position.y(), 100,10,radius1,color1.x,color1.y,color1.z);
        drawCircle(position.x(), position.y(), 100,10,radius2,color2.x,color2.y,color2.z);

    }
    void draw()
    {
        if (!isSelected) {
            drawFilledCircle(position.x(), position.y());
        }
        else drawCircle(position.x(),position.y());
        drawR1R2();
    }

        void drawFilledCircle(float cx, float cy, int num_segments=20){
            //static float angle;
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glColor3f(1, 1, 1);
            float twicePi = 2.0 * 3.142;
            glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
            glVertex2f(cx, cy); // center of circle
            for (int i = 0; i <= num_segments; i++)   {
                glVertex2f (
                        (cx + (r* cos(i * twicePi / num_segments))), (cy + (r * sin(i * twicePi / num_segments)))
                );
            }
            glColor3f(1, 1, 1);
            glEnd(); //END
    }

    void drawCircle(float cx, float cy, int num_segments=100,int lineWidth=10,float radius=r,float r_c=0,float g=1,float b=0) {
        glLineWidth(lineWidth);
        glEnable(GL_LINE_SMOOTH);
        glColor3f(r_c, g, b);
        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < num_segments; ii++)   {
            float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
            float x = radius * cosf(theta);//calculate the x component
            float y = radius * sinf(theta);//calculate the y component
            glVertex2f(x + cx, y + cy);//output vertex
        }
        glLineWidth(1);
        glColor3f(1, 1, 1);
        glEnd();
    }

    Point& operator=(const Point& other)
    {
        if (this == &other)
            return *this;
        color1=other.color1;
        color2=other.color2;
        position=other.position;
        radius1=other.radius1;
        radius2=other.radius2;
        isDeleted=false;
        return *this;
    }

    Point operator+(const Point &p) const
    {
        Point ret=p;
        ret.position+=position;
        ret.color1.x+=(color1.x);
        ret.color1.y+=(color1.y);
        ret.color1.z+=(color1.z);
        ret.color2.x+=(color2.x);
        ret.color2.y+=(color2.y);
        ret.color2.z+=(color2.z);
        ret.radius1+=radius1;
        ret.radius2+=radius2;
        ret.isDeleted=false;
        return ret;
    }

    Point operator-(const Point &p) const
    {
        Point ret=p;
        ret.position-=position;
        ret.color1.x-=(color1.x);
        ret.color1.y-=(color1.y);
        ret.color1.z-=(color1.z);
        ret.color2.x-=(color2.x);
        ret.color2.y-=(color2.y);
        ret.color2.z-=(color2.z);
        ret.radius1-=radius1;
        ret.radius2-=radius2;
        ret.isDeleted=false;
        return ret;
    }

    Point operator*(float s) const
    {
        Point ret=*this;
        ret.position*=s;
        ret.color1.x*=s;
        ret.color1.y*=s;
        ret.color1.z*=s;
        ret.color2.x*=s;
        ret.color2.y*=s;
        ret.color2.z*=s;
        ret.radius1*=s;
        ret.radius2*=s;
        ret.isDeleted=false;
        return ret;
    }

    Point operator/(float s) const
    {
        Point ret=*this;
        ret.position/=s;
        ret.color1.x/=s;
        ret.color1.y/=s;
        ret.color1.z/=s;
        ret.color2.x/=s;
        ret.color2.y/=s;
        ret.color2.z/=s;
        ret.radius1/=s;
        ret.radius2/=s;
        ret.isDeleted=false;

        return ret;
    }

    void operator+=(const Point &p)
    {
        position+=p.position;

        color1.x+=(p.color1.x);
        color1.y+=(p.color1.y);
        color1.z+=(p.color1.z);
        color2.x+=(p.color2.x);
        color2.y+=(p.color2.y);
        color2.z+=(p.color2.z);
        radius1+=p.radius1;
        radius2+=p.radius2;
        isDeleted=false;

    }

    void operator-=(const Point &p)
    {
        position-=p.position;

        color1.x-=(p.color1.x);
        color1.y-=(p.color1.y);
        color1.z-=(p.color1.z);
        color2.x-=(p.color2.x);
        color2.y-=(p.color2.y);
        color2.z-=(p.color2.z);
        radius1-=p.radius1;
        radius2-=p.radius2;
        isDeleted=false;

    }

    void operator*=(float s)
    {
        position*=s;

        color1.x*=s;
        color1.y*=s;
        color1.z*=s;
        color2.x*=s;
        color2.y*=s;
        color2.z*=s;
        radius1*=s;
        radius2*=s;
        isDeleted=false;

    }

    void operator/=(float s)
    {
        position/=s;

        color1.x/=s;
        color1.y/=s;
        color1.z/=s;
        color2.x/=s;
        color2.y/=s;
        color2.z/=s;
        radius1/=s;
        radius2/=s;
        isDeleted=false;

    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        os << p.position<<"radius 1 "<<p.radius1<<" radius 2 "<<p.radius2<<".    ";
        return os << " }";
    }
};





#endif //FYP_POINT_H
