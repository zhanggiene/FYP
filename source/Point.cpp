//
// Created by Zhang Zhuyan on 27/12/22.
//

#include "Point.h"
#include "imgui.h"
#include "App.h"


    Point::Point(ImVec4 color1_,ImVec4 color2_,ImVec4 colorOuter1_,ImVec4 colorOuter2_,float x_,float y_,float r1_,float r2_):
            color1(color1_), color2(color2_) ,colorOuter1(colorOuter1_),colorOuter2(colorOuter2_),position(x_,y_),isSelected(false),radius1(r1_),radius2(r2_),isDeleted(false)
    {
    }
    Point::Point(ImVec4 color1_,ImVec4 color2_,ImVec4 colorOuter1_,ImVec4 colorOuter2_,Eigen::Vector2f position_,float r1_,float r2_):
            color1(color1_), color2(color2_) ,colorOuter1(colorOuter1_),colorOuter2(colorOuter2_),position(position_),isSelected(false),radius1(r1_),radius2(r2_),isDeleted(false)
    {
    }
Point::Point(float color1_x,float color1_y, float color1_z,float color2_x,float color2_y, float color2_z,float colorOuter1_x,float colorOuter1_y, float colorOuter1_z,float colorOuter2_x,float colorOuter2_y, float colorOuter2_z,float x_,float y_,float r1_,float r2_):
color1(color1_x,color1_y,color1_z,1),color2(color2_x,color2_y,color2_z,1),colorOuter1(colorOuter1_x,colorOuter1_y,colorOuter1_z,1),colorOuter2(colorOuter2_x,colorOuter2_y,colorOuter2_z,1),position(x_,y_),isSelected(false),radius1(r1_),radius2(r2_),isDeleted(false)
{

}



    void Point::setCallBack(some_void_function_type f)
    {
        f_ = f;

    }
void Point::setCleanCallBack(some_void_function_type f)
{
    cleandeleted_ = f;

}

    // Copy constructor
    Point::Point(const Point& p1)
    {
        // std::cout<<"copy constructor used"<<std::endl;
        color1 = p1.color1;
        color2 = p1.color2;
        colorOuter1 = p1.colorOuter1;
        colorOuter2 = p1.colorOuter2;
        position=p1.position;
        radius1=p1.radius1;
        radius2=p1.radius2;
        isDeleted=false;
    }

    void Point::showCruveproperty(const char* prefix, int uid)
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
                ImGui::Text("This point will be deleted.\nThis operation cannot be undone!\n\n");
                ImGui::Separator();

                //static int unused_i = 0;
                //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();isDeleted=true; cleandeleted_(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
            for (int i = 0; i < 5; i++)
            {

                ImGui::PushID(i); // Use field index as identifier.
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags =
                        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                if (i==0) ImGui::TreeNodeEx("Color 1", flags, "Field_%d", i);
                else if (i==1 ) ImGui::TreeNodeEx("Color 2", flags, "Field %d", i);
                else if (i==2 ) ImGui::TreeNodeEx("radius 1", flags, "Field %d", i);
                else if (i==3 ) ImGui::TreeNodeEx("radius 2", flags, "Field %d", i);
                else if (i==4 ) ImGui::TreeNodeEx("outer color", flags, "Field %d", i);

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                // add callback!!https://stackoverflow.com/questions/20037427/pass-method-as-callback-from-one-class-to-other-class

                if (i==0 and ImGui::ColorPicker3(" color 1 ", (float *) &color1,ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoAlpha)) f_();
                else if (i==1 and  ImGui::ColorPicker3(" color 2 ", (float *) &color2,ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoAlpha)) f_();
                else if (i==2 and ImGui::DragFloat("##radius 1", &radius1, 1.0f, 0.0f,100.0f)) f_();
                else if (i==3 and ImGui::DragFloat("##radius 2", &radius2, 1.0f, 0.0f,100.0f)) f_();
                else if (i==4)
                {
                    if (ImGui::ColorEdit3(" color outer 1 ", (float *) &colorOuter1,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) f_(); // Edit 3 floats representing a color
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3(" color outer 2 ", (float *)  &colorOuter2,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) f_(); // Edit 3 floats representing a color
                }
                ImGui::NextColumn();

                ImGui::PopID();

            }
            ImGui::TreePop();
        }
        ImGui::PopID();

    }

    // move constructor
    Point::Point (Point&& other) noexcept
    {
        color1=std::move(other.color1);
        color2=std::move(other.color2);
        colorOuter1=std::move(other.colorOuter1);
        colorOuter2=std::move(other.colorOuter2);
        position=std::move(other.position);
        radius1=other.radius1;
        radius2=other.radius2;
        isDeleted=false;
        //f_=other.f_;

    }

    void Point::checkMouseSelection(float x,float y,bool& lock)
    {
        if(abs(position.x()-x)<4*r && abs(position.y()-y)<4*r && lock==false)
        {
            // highlighted by the mouse
            isSelected=true;
            lock=true;
        }
        else
        {
            isSelected=false;
            lock=false;
        }


    }

    void Point::updatePosition(float x,float y)
    {
        if (isSelected)
        {
            position.x()=x;
            position.y()=y;
        }
    }
    void Point::drawR1R2()
    {
        drawCircle(position.x(), position.y(), 100,10,radius1,color1.x,color1.y,color1.z);
        drawCircle(position.x(), position.y(), 100,10,radius2,color2.x,color2.y,color2.z);

    }
    void Point::draw()
    {
        if (isSelected  && App::editMode) {
            drawCircle(position.x(),position.y());

        }
        else drawFilledCircle(position.x(), position.y());
        drawR1R2();
    }

    void Point::drawFilledCircle(float cx, float cy, int num_segments){
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

    void Point::drawCircle(float cx, float cy, int num_segments,int lineWidth,float radius,float r_c,float g,float b) {
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

    Point& Point::operator=(const Point& other)
    {
        if (this == &other)
            return *this;
        color1=other.color1;
        color2=other.color2;
        position=other.position;
        radius1=other.radius1;
        radius2=other.radius2;
        colorOuter1=other.colorOuter1;
        colorOuter2=other.colorOuter2;
        isDeleted=false;
        return *this;
    }
Point& Point::operator=(Point&& other)
{
    if (this == &other)
        return *this;
    //std::cout<<"point move assignment is used";
    color1=other.color1;
    color2=other.color2;
    colorOuter1=other.colorOuter1;
    colorOuter2=other.colorOuter2;
    position=other.position;
    radius1=other.radius1;
    radius2=other.radius2;
    isDeleted=false;
    return *this;
}

    Point Point::operator+(const Point &p) const
    {
        Point ret=p;
        ret.position+=position;
        ret.color1.x+=(color1.x);
        ret.color1.y+=(color1.y);
        ret.color1.z+=(color1.z);
        ret.color2.x+=(color2.x);
        ret.color2.y+=(color2.y);
        ret.color2.z+=(color2.z);
        ret.colorOuter1.x+=(colorOuter1.x);
        ret.colorOuter1.y+=(colorOuter1.y);
        ret.colorOuter1.z+=(colorOuter1.z);
        ret.colorOuter2.x+=(colorOuter2.x);
        ret.colorOuter2.y+=(colorOuter2.y);
        ret.colorOuter2.z+=(colorOuter2.z);
        ret.radius1+=radius1;
        ret.radius2+=radius2;
        ret.isDeleted=false;
        return ret;
    }

    Point Point::operator-(const Point &p) const
    {
        Point ret=p;
        ret.position-=position;
        ret.color1.x-=(color1.x);
        ret.color1.y-=(color1.y);
        ret.color1.z-=(color1.z);
        ret.color2.x-=(color2.x);
        ret.color2.y-=(color2.y);
        ret.color2.z-=(color2.z);
        ret.colorOuter1.x-=(colorOuter1.x);
        ret.colorOuter1.y-=(colorOuter1.y);
        ret.colorOuter1.z-=(colorOuter1.z);
        ret.colorOuter2.x-=(colorOuter2.x);
        ret.colorOuter2.y-=(colorOuter2.y);
        ret.colorOuter2.z-=(colorOuter2.z);
        ret.radius1-=radius1;
        ret.radius2-=radius2;
        ret.isDeleted=false;
        return ret;
    }

    Point Point::operator*(float s) const
    {
        Point ret=*this;
        ret.position*=s;
        ret.color1.x*=s;
        ret.color1.y*=s;
        ret.color1.z*=s;
        ret.color2.x*=s;
        ret.color2.y*=s;
        ret.color2.z*=s;
        ret.colorOuter1.x*=s;
        ret.colorOuter1.y*=s;
        ret.colorOuter1.z*=s;
        ret.colorOuter2.x*=s;
        ret.colorOuter2.y*=s;
        ret.colorOuter2.z*=s;
        ret.radius1*=s;
        ret.radius2*=s;
        ret.isDeleted=false;
        return ret;
    }

    Point Point::operator/(float s) const
    {
        Point ret=*this;
        ret.position/=s;
        ret.color1.x/=s;
        ret.color1.y/=s;
        ret.color1.z/=s;
        ret.color2.x/=s;
        ret.color2.y/=s;
        ret.color2.z/=s;
        ret.colorOuter1.x/=s;
        ret.colorOuter1.y/=s;
        ret.colorOuter1.z/=s;
        ret.colorOuter2.x/=s;
        ret.colorOuter2.y/=s;
        ret.colorOuter2.z/=s;
        ret.radius1/=s;
        ret.radius2/=s;
        ret.isDeleted=false;

        return ret;
    }

    void Point::operator+=(const Point &p)
    {
        position+=p.position;

        color1.x+=(p.color1.x);
        color1.y+=(p.color1.y);
        color1.z+=(p.color1.z);
        color2.x+=(p.color2.x);
        color2.y+=(p.color2.y);
        color2.z+=(p.color2.z);
        colorOuter1.x+=(p.colorOuter1.x);
        colorOuter1.y+=(p.colorOuter1.y);
        colorOuter1.z+=(p.colorOuter1.z);
        colorOuter2.x+=(p.colorOuter2.x);
        colorOuter2.y+=(p.colorOuter2.y);
        colorOuter2.z+=(p.colorOuter2.z);
        radius1+=p.radius1;
        radius2+=p.radius2;
        isDeleted=false;

    }

    void Point::operator-=(const Point &p)
    {
        position-=p.position;

        color1.x-=(p.color1.x);
        color1.y-=(p.color1.y);
        color1.z-=(p.color1.z);
        color2.x-=(p.color2.x);
        color2.y-=(p.color2.y);
        color2.z-=(p.color2.z);
        colorOuter1.x-=(p.colorOuter1.x);
        colorOuter1.y-=(p.colorOuter1.y);
        colorOuter1.z-=(p.colorOuter1.z);
        colorOuter2.x-=(p.colorOuter2.x);
        colorOuter2.y-=(p.colorOuter2.y);
        colorOuter2.z-=(p.colorOuter2.z);
        radius1-=p.radius1;
        radius2-=p.radius2;
        isDeleted=false;

    }

    void Point::operator*=(float s)
    {
        position*=s;

        color1.x*=s;
        color1.y*=s;
        color1.z*=s;
        color2.x*=s;
        color2.y*=s;
        color2.z*=s;
        colorOuter1.x*=s;
        colorOuter1.y*=s;
        colorOuter1.z*=s;
        colorOuter2.y*=s;
        colorOuter2.z*=s;
        radius1*=s;
        radius2*=s;
        isDeleted=false;

    }

    void Point::operator/=(float s)
    {
        position/=s;
        color1.x/=s;
        color1.y/=s;
        color1.z/=s;
        color2.x/=s;
        color2.y/=s;
        color2.z/=s;
        colorOuter1.x/=s;
        colorOuter1.y/=s;
        colorOuter1.z/=s;
        colorOuter2.x/=s;
        colorOuter2.y/=s;
        colorOuter2.z/=s;
        radius1/=s;
        radius2/=s;
        isDeleted=false;

    }
    std::ostream& operator<<(std::ostream &os, const Point &p)
    {
        os << p.position<<"radius 1 "<<p.radius1<<" radius 2 "<<p.radius2<<".    ";
        return os << " }";
    }
    void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, Point const& p )
{
    jv = {
            { "position_x" , int(p.position.x())},
              { "position_y" , int(p.position.y())},
            {"color1_x",p.color1.x},
             {"color1_y",p.color1.y},
              {"color1_z",p.color1.z},
               {"color2_x",p.color2.x},
                {"color2_y",p.color2.y},
                 {"color2_z",p.color2.z},
            {"colorouter1_x",p.colorOuter1.x},
            {"colorouter1_y",p.colorOuter1.y},
            {"colorouter1_z",p.colorOuter1.z},
            {"colorouter2_x",p.colorOuter2.x},
            {"colorouter2_y",p.colorOuter2.y},
            {"colorouter2_z",p.colorOuter2.z},

            {"radius1",p.radius1},
            {"radius2",p.radius2}


    };
}

 Point tag_invoke( boost::json::value_to_tag< Point >, boost::json::value const& jv )
{
    boost::json::object const& obj = jv.as_object();
    return Point {
            boost::json::value_to<float>( obj.at( "color1_x" ) ),
            boost::json::value_to<float>( obj.at( "color1_y" ) ),
            boost::json::value_to<float>( obj.at( "color1_z" ) ),
            boost::json::value_to<float>( obj.at( "color2_x" ) ),
            boost::json::value_to<float>( obj.at( "color2_y" ) ),
            boost::json::value_to<float>( obj.at( "color2_z" ) ),
            boost::json::value_to<float>( obj.at( "colorouter1_x" ) ),
            boost::json::value_to<float>( obj.at( "colorouter1_y" ) ),
            boost::json::value_to<float>( obj.at( "colorouter1_z" ) ),
            boost::json::value_to<float>( obj.at( "colorouter2_x" ) ),
            boost::json::value_to<float>( obj.at( "colorouter2_y" ) ),
            boost::json::value_to<float>( obj.at( "colorouter2_z" ) ),
            boost::json::value_to<float>( obj.at( "position_x" )),
            boost::json::value_to<float>( obj.at( "position_y" ) ),
            boost::json::value_to<float>( obj.at( "radius1" ) ),
            boost::json::value_to<float>( obj.at( "radius2" ) )
    };
}
template<class T>
void Point::extract( boost::json::object const& obj, T& t, std::string_view key )
{
    t = boost::json::value_to<T>( obj.at( key ) );
}



