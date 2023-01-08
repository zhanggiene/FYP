//
// Created by Zhang Zhuyan on 28/12/22.
//

#ifndef FYP_OUTERCLASS_H
#define FYP_OUTERCLASS_H

#include <string>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "Point.h"
#include "Canvas.h"
#include "VisualPoint.h"
class outerclass {
public:

    bool _toRenew;
    unsigned int _degree;
    float _step;
    std::vector<Point> _controlPoints;
    std::vector<Point> _interpolants;    // old way, for debugging purpose
    std::vector<int> knotVector;
    std::vector<Point> _gradientControlPoints;
    std::vector<Point> _centerPoints;
    std::vector<Point> _centerGradientPoints;
    std::vector<VisualPoint> _normalUp;   //
    std::vector<VisualPoint> _normalDown;
    std::vector<Point> _boundaryPoints;
    std::vector<VisualPoint> _startingBoundaryVisualControlPoints;  // 3
    std::vector<VisualPoint> _startingBoundaryVisualPoints;  // 3
    std::vector<VisualPoint> _endingBoundaryVisualControlPoints;  // 3
    std::vector<VisualPoint> _endingBoundaryVisualPoints;  // 3
    float _thickness;
    bool _straightLine;
    bool _visibleControlPoint;


    outerclass(){};
    outerclass(std::vector<Point> controlPoints)
            : _toRenew(true),
              _degree(controlPoints.size()-1),
              _step(0.01f),
              _controlPoints(controlPoints),_thickness(1),_straightLine(true),_visibleControlPoint(true) {


        std::cout<< "construct curves";
        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCallBack(std::bind(&outerclass::generate, this));
            //_controlPoints[i].setCallBack([](){std::cout<<"hi";});
        }


    }
    // Move assignment
    outerclass& operator=(outerclass&& other) noexcept
    {
        std::cout<<"outer class move assignment";
        _controlPoints=std::move(other._controlPoints);
        knotVector=std::move(other.knotVector);
        return *this;
    }

    outerclass (outerclass&& other) noexcept: _toRenew(true),_step(0.01f),_thickness(1),_straightLine(true),_visibleControlPoint(true)
    {
        std::cout<<"outer class move constructor";
        //_controlPoints.reserve(other._controlPoints.size());
        //std::move(other._controlPoints.begin(), std::next(other._controlPoints.begin(), other._controlPoints.size()),std::back_inserter(_controlPoints));
        _controlPoints=std::move(other._controlPoints); // ????
        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCallBack(std::bind(&outerclass::generate, this));
        }
        _degree=_controlPoints.size()-1,
        knotVector=std::move(other.knotVector);
        _gradientControlPoints=std::move(other._gradientControlPoints);
                _centerPoints=std::move(other._centerPoints);
        _centerGradientPoints=std::move(other._centerGradientPoints);
        _normalUp=std::move(other._normalUp);   //
        _normalDown=std::move(other._normalDown);
         _boundaryPoints=std::move(other._boundaryPoints);
         _startingBoundaryVisualControlPoints=std::move(other._startingBoundaryVisualControlPoints);  // 3
        _startingBoundaryVisualPoints=std::move(other._startingBoundaryVisualPoints);  // 3
         _endingBoundaryVisualControlPoints=std::move(other._endingBoundaryVisualControlPoints);  // 3
         _endingBoundaryVisualPoints=std::move(other._endingBoundaryVisualPoints);  // 3
        _thickness=other._thickness;
        _straightLine=other._straightLine;
        _visibleControlPoint=other._visibleControlPoint;

    }

    // copy assignment
    outerclass& operator=(const outerclass& other)
    {
        _controlPoints=other._controlPoints;
        knotVector=other.knotVector;
        return *this;

    }
    Point deCasteljau(const float t, std::vector<Point>& points)
    {
        if (points.size()==1) return points[0];

        for (size_t i = 0; i < points.size() -1; i++) {
            //std::cout<<"before"<<points[i]<<std::endl;
            //std::cout<<"before"<<points[i] * (1.0f-t)<<std::endl;
            //std::cout<<"before"<<points[i+1] <<std::endl;

            points[i] =  points[i] *(1.0f - t)  + points[i+1] * t;
            //std::cout<<"after "<<points[i]<<std::endl;

        }
        points.pop_back();
        //std::cout<<"DeCasterjau"<<std::endl;

        return deCasteljau(t,points);
    };


    void drawControlPoints()
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i=0;i<_controlPoints.size();i++) {
            _controlPoints[i].draw();
        }
    }

    void cleanDeletedPoints()
    {
        if (_controlPoints.size()>3)
        {
            auto start=_controlPoints.begin();
            while(start!=_controlPoints.end())
            {
                if ((*start).isDeleted)
                {
                    _controlPoints.erase (start);
                    _toRenew=true;
                    _degree-=1;

                }
                start+=1;
            }
        }
    }



    const Point interpolate(const float t)
    {

        std::vector<Point> controlPointCopy(_controlPoints);
        return deCasteljau(t,controlPointCopy);
    }


    void generateInterpolants()
    {
        this->_interpolants.clear();

        for (float t = 0.0f; t < 1.0f; t += this->_step) {
            this->_interpolants.push_back(interpolate(t));
        }
        this->_interpolants.push_back(interpolate(1.0f));
    }

    void generate2()
    {
        std::cout<<"size of control "<<_controlPoints.size()<<std::endl;


    }
    void generate()
    {
        // generateInterpolants();
        generateKnotVector();
        generateGradientControlPoint();
        generateCenterPoint();
        generateNormal();
        if (_straightLine) {
            generateEndingBoundary();
            generateStartingBoundary();
        }


    }

    void checkMouseSelection(float xpos, float ypos)
    {
        for (auto &p: _controlPoints) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.checkMouseSelection(xpos, ypos);
        }
    }

    void updatePosition(float xpos, float ypos)
    {
        for (auto &p: _controlPoints) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.updatePosition(xpos, ypos);
        }
        _toRenew=true;
    }
    void generateStartingBoundary()
    {
        _startingBoundaryVisualControlPoints.clear();
        _startingBoundaryVisualControlPoints.push_back(_normalUp.front());
        _startingBoundaryVisualControlPoints.push_back((_normalUp.front()+_normalDown.front())/(2.0));
        _startingBoundaryVisualControlPoints.push_back(_normalDown.front());
        _startingBoundaryVisualPoints.clear();
        for (float i = 0.0f; i < knotVector[_degree + 3]; i += 0.01f )
        {

            for (int j = 0; j < 3; j++)
            {
                if (i >= j)
                {
                    _startingBoundaryVisualPoints.push_back(deBoorClosing(_startingBoundaryVisualControlPoints,2, j, i));

                }
            }
        }

    }

    void generateEndingBoundary()
    {
        _endingBoundaryVisualControlPoints.clear();
        _endingBoundaryVisualControlPoints.push_back(_normalUp.back());
        _endingBoundaryVisualControlPoints.push_back((_normalUp.back()+_normalDown.back())/(2.0));
        _endingBoundaryVisualControlPoints.push_back(_normalDown.back());
        _endingBoundaryVisualPoints.clear();
        for (float i = 0.0f; i < knotVector[_degree + 3]; i += 0.01f )
        {

            for (int j = 0; j < 3; j++)
            {
                if (i >= j)
                {
                    _endingBoundaryVisualPoints.push_back(deBoorClosing(_endingBoundaryVisualControlPoints,2, j, i));

                }
            }
        }


    }

    VisualPoint deBoorClosing(std::vector<VisualPoint>& controlPoints,int r, int i, float u)
    {
        //r is degree, i is no of control points,

        if (r == 0)
        {
            return controlPoints[i];
        }
        else
        {

            float pre = (u - knotVector[i + r]) / (knotVector[i + _degree + 1] - knotVector[i + r]);
            return ((deBoorClosing(controlPoints,r - 1, i, u) * (1 - pre)) + (deBoorClosing(controlPoints,r - 1, i + 1, u) * (pre)));

        }
    }


    void generateNormal()
    {
        _normalUp.clear();
        _normalDown.clear();
        for (int i = 0; i< _centerPoints.size(); i++ ) {
            auto normalizedGradient = _centerGradientPoints[i].position.normalized(); // return a value

            Eigen::Vector2f rotateVectdown = Eigen::Vector2f(-normalizedGradient.y(), normalizedGradient.x());
            _normalDown.push_back(VisualPoint(_centerPoints[i].position + (_centerPoints[i].radius2 * -rotateVectdown),
                                              _centerPoints[i].color2));
            _normalUp.push_back(VisualPoint(_centerPoints[i].position + (_centerPoints[i].radius1 * (rotateVectdown)),
                                            _centerPoints[i].color1));
        }
    }


    void draw() {
        cleanDeletedPoints();

        if (_toRenew) {
            _toRenew = false;
            generate();
        }
        if (_visibleControlPoint) {drawControlPoints();
            drawSkeleton();}
        drawBoundary();
        drawEndBoundary();
    }
    void drawEndBoundary()
    {
        //std::cout<<"size of _endingBoundaryVisualPoint is "<<_endingBoundaryVisualPoints.size();
        for(int i=0;i< _endingBoundaryVisualPoints.size()-1;i++) {
            glBegin( GL_LINES);
            glColor3f(_endingBoundaryVisualPoints[i].color.x,_endingBoundaryVisualPoints[i].color.y,_endingBoundaryVisualPoints[i].color.z);
            glVertex2f(_endingBoundaryVisualPoints[i].position.x(),_endingBoundaryVisualPoints[i].position.y());
            glVertex2f(_endingBoundaryVisualPoints[i+1].position.x(),_endingBoundaryVisualPoints[i+1].position.y());
            glColor3f(1,1,1);
            glEnd();

        }
        for(int i=0;i< _startingBoundaryVisualPoints.size()-1;i++) {
            glBegin( GL_LINES);
            glColor3f(_startingBoundaryVisualPoints[i].color.x,_startingBoundaryVisualPoints[i].color.y,_startingBoundaryVisualPoints[i].color.z);
            glVertex2f(_startingBoundaryVisualPoints[i].position.x(),_startingBoundaryVisualPoints[i].position.y());
            glVertex2f(_startingBoundaryVisualPoints[i+1].position.x(),_startingBoundaryVisualPoints[i+1].position.y());
            glColor3f(1,1,1);
            glEnd();

        }

    }

    void drawSkeleton()
    {
        // draw skeleton, the center extrapolated thin line
        for(int i=0;i< _centerPoints.size()-1;i++)
        {
            glBegin( GL_LINES);
            glVertex2f(_centerPoints[i].position.x(),_centerPoints[i].position.y());
            glVertex2f(_centerPoints[i+1].position.x(),_centerPoints[i+1].position.y());
            glEnd();
        }

    }

    void drawBoundary()
    {
        for(int i=0;i< _normalUp.size()-1;i++)
        {
            glBegin( GL_LINES);
            //std::cout<<_normalUp[i].position<<std::endl;
            glColor3f(_normalUp[i].color.x,_normalUp[i].color.y,_normalUp[i].color.z);
            glVertex2f(_normalUp[i].position.x(),_normalUp[i].position.y());
            glVertex2f(_normalUp[i+1].position.x(),_normalUp[i+1].position.y());
            glEnd();
        }
        for(int i=0;i< _normalDown.size()-1;i++)
        {
            glBegin( GL_LINES);
            glColor3f(_normalDown[i].color.x,_normalDown[i].color.y,_normalDown[i].color.z);
            glVertex2f(_normalDown[i].position.x(),_normalDown[i].position.y());
            glVertex2f(_normalDown[i+1].position.x(),_normalDown[i+1].position.y());
            glColor3f(1,1,1);
            glEnd();
        }

        // std::cout<<"draw boundary,_normalUp.size()"<<_normalUp.size()<<std::endl;
    }
    void generateKnotVector()
    {
        //  needs to be tested
        knotVector.clear();
        int index=0;
        for (int i = 0; i < (_degree + _controlPoints.size() + 1); i++) // n+p+1
        {

            if (i<=_degree) knotVector.push_back(index);// 0
            else if (i<=_controlPoints.size()) knotVector.push_back(++index);
            else knotVector.push_back(index);
        }
    }
    void generateGradientControlPoint()
    {
        _gradientControlPoints.clear();
        for (int i = 0; i < _controlPoints.size() - 1; i++)
        {
            _gradientControlPoints.push_back((_controlPoints[i + 1] - _controlPoints[i]) / (knotVector[i + _degree + 1] - knotVector[i + 1]));
        }
    }


    // https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
    // maybe can be optimized
    Point deBoor(int r, int i, float t)  // r=degree i is number of control points
    {

        if (r == 0)
        {
            return _controlPoints[i];
        }
        else {

            float pre = (t - knotVector[i + r]) / (knotVector[i + _degree + 1] - knotVector[i + r]);
            return (deBoor(r - 1, i, t) * (1 - pre)) + (deBoor(r - 1, i + 1, t) * (pre));

        }
    }



    Point gradientDeboor(int p, int i, float t)
    {
        if (p == 0)
        {
            return _gradientControlPoints[i - 1];
        }
        else
        {
            float pre = (t - knotVector[i + p]) / (knotVector[i + _degree + 1] - knotVector[i + p]);
            return ((gradientDeboor(p - 1, i, t) * (1 - pre)) + (gradientDeboor(p - 1, i + 1, t) * (pre)));

        }
    }
    void generateCenterPoint() {
        _centerPoints.clear();
        _centerGradientPoints.clear();
        for (float i = 0.0f; i < knotVector[_degree + _controlPoints.size()]; i += 0.01f / _controlPoints.size()) {

            for (int j = 0; j < _controlPoints.size(); j++) {
                if (i >= j) {

                    _centerPoints.push_back(deBoor(_degree, j, i));
                    _centerGradientPoints.push_back(gradientDeboor(_degree - 1, j + 1, i) * (float) _degree);
                }
            }
        }
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
        ImGui::Text("curve attributes");

        if (node_open)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Button("Delete..")) ImGui::OpenPopup("Delete?");

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

                if (ImGui::Button("OK", ImVec2(120, 0))) {ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            ImGui::PushID(_controlPoints.size()); // Use field index as identifier.
            // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGuiTreeNodeFlags flags =
                    ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
            ImGui::TreeNodeEx("Field", flags, "Field_%d",10 );

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            ImGui::Checkbox("Control Point visibility", &_visibleControlPoint);
            ImGui::NextColumn();

            ImGui::PopID();
            for (int i = 0; i < _controlPoints.size(); i++)
            {

                /*
                */
                _controlPoints[i].showCruveproperty("Points",i);
            }
            ImGui::TreePop();
        }
        ImGui::PopID();

    }

    void drawCurve()
    {
        if (_toRenew) {
            _toRenew = false;
            generateInterpolants();
        }

        glBegin(GL_QUADS);
        for (size_t i = 0; i < _interpolants.size() - 1; i++) {
            // Get the normal
            Point normal = _interpolants[i + 1] - _interpolants[i];

            // Swap the x- and y-coordinates
            float temp = normal.position.x();
            normal.position.x() = -normal.position.y();
            normal.position.y()= temp;

            normal.position.normalize();
            normal *= _thickness;

            glColor3f(_interpolants[i].color1.x, _interpolants[i].color1.y, _interpolants[i].color1.z);
            glVertex2f(_interpolants[i].position.x() + normal.position.x(), _interpolants[i].position.y() + normal.position.y());

            glColor3f(_interpolants[i+1].color1.x, _interpolants[i+1].color1.y, _interpolants[i+1].color1.z);
            glVertex2f(_interpolants[i+1].position.x() + normal.position.x(), _interpolants[i+1].position.y() + normal.position.y());

            glColor3f(_interpolants[i+1].color2.x, _interpolants[i+1].color2.y, _interpolants[i+1].color2.z);
            glVertex2f(_interpolants[i+1].position.x() - normal.position.x(), _interpolants[i+1].position.y() - normal.position.y());

            glColor3f(_interpolants[i].color2.x, _interpolants[i].color2.y, _interpolants[i].color2.z);
            glVertex2f(_interpolants[i].position.x() - normal.position.x(), _interpolants[i].position.y() - normal.position.y());
        }
        glColor3f(1, 1, 1);
        glEnd();
    }


    friend void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, outerclass const& c )
    {
        jv ={
                {"points",c._controlPoints }};

    }

    template<class T>
    void extract( boost::json::object const& obj, T& t, std::string_view key )
    {
        t = boost::json::value_to<T>( obj.at( key ) );
    }

    friend outerclass tag_invoke( boost::json::value_to_tag< outerclass >, boost::json::value const& jv )
    {
        boost::json::object const& obj = jv.as_object();
        return outerclass (
                boost::json::value_to< std::vector< Point > >( obj.at( "points" ) ));
    }


};

#endif //FYP_OUTERCLASS_H
