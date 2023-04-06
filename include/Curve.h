//
// Created by Zhang Zhuyan on 28/12/22.
//

#ifndef FYP_CURVE_H
#define FYP_CURVE_H

#include <string>
#include <utility>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "Point.h"
#include "VisualPoint.h"
class Curve {
public:

    bool _toRenew;
    bool _aboutDelete;
    int _degree;
    float _step;
    typedef std::function<void()> some_void_function_type;
    some_void_function_type cleandeleted_;
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
    std::vector<VisualPoint> _startingBoundaryVisualPoints;
    std::vector<VisualPoint> _endingBoundaryVisualControlPoints;  // 3
    std::vector<VisualPoint> _endingBoundaryVisualPoints;
    ImVec4 startOuterColor;
    ImVec4 endOuterColor;
    float _thickness;
    bool _straightLineEnd1;
    bool _straightLineEnd2;
    bool _visibleControlPoint;
    bool _lock;
    bool _isDeleted;
    bool _visibleCurve;


    Curve(){};
    Curve(const std::vector<Point>& controlPoints)
            : _toRenew(true),
            _aboutDelete(false),
              _degree(controlPoints.size()-1),
              _step(0.01f),
              _controlPoints(controlPoints),_thickness(1),_straightLineEnd1(true),_straightLineEnd2(true),_visibleControlPoint(true),_lock(false),_isDeleted(false),_visibleCurve(true) {


        std::cout<< "construct curves";
        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCallBack(std::bind(&Curve::generate, this));
            _controlPoints[i].setCleanCallBack(std::bind(&Curve::cleanDeletedPoints, this));
            //_controlPoints[i].setCallBack([](){std::cout<<"hi";});
        }

    }
    // Move assignment
    // used when we erase Curve
    Curve& operator=(Curve&& other) noexcept
    {
        //_controlPoints.reserve(other._controlPoints.size());
        //std::move(other._controlPoints.begin(), std::next(other._controlPoints.begin(), other._controlPoints.size()),std::back_inserter(_controlPoints));
        _controlPoints=std::move(other._controlPoints); // ????
        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCallBack(std::bind(&Curve::generate, this));
        }

        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCleanCallBack(std::bind(&Curve::cleanDeletedPoints, this));
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
        _straightLineEnd1=other._straightLineEnd1;
        _straightLineEnd2=other._straightLineEnd2;
        _visibleControlPoint=other._visibleControlPoint;
        _lock=other._lock;
        _isDeleted=other._isDeleted;
        cleandeleted_=other.cleandeleted_;
        _visibleCurve=other._visibleCurve;
        return *this;
    }

    Curve (Curve&& other) noexcept: _toRenew(true),_aboutDelete(false), _step(0.01f), _thickness(1), _straightLineEnd1(true), _straightLineEnd2(true), _visibleControlPoint(true)
    {
        //std::cout<<"outer class move constructor";
        //_controlPoints.reserve(other._controlPoints.size());
        //std::move(other._controlPoints.begin(), std::next(other._controlPoints.begin(), other._controlPoints.size()),std::back_inserter(_controlPoints));
        _controlPoints=std::move(other._controlPoints); // ????
        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCallBack(std::bind(&Curve::generate, this));
        }

        for(int i=0;i<_controlPoints.size();i++)
        {
            _controlPoints[i].setCleanCallBack(std::bind(&Curve::cleanDeletedPoints, this));
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
        _straightLineEnd1=other._straightLineEnd1;
        _straightLineEnd2=other._straightLineEnd2;
        _visibleControlPoint=other._visibleControlPoint;
        _lock=other._lock;
        _isDeleted=other._isDeleted;
        cleandeleted_=other.cleandeleted_;
        _visibleCurve=other._visibleCurve;

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


    void drawControlPoints() const
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        for (const auto & p:_controlPoints) {
            p.draw();
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

    void setCleanCallBack(some_void_function_type f)
    {
        cleandeleted_ = f;

    }
    void generate()
    {
        // generateInterpolants();
        generateKnotVector();
        generateGradientControlPoint();
        generateCenterPoint();
        generateNormal();
        if (_straightLineEnd1) {
            generateStartingBoundary();
        }
        else
        {

            _startingBoundaryVisualPoints.clear();
        }
        if (_straightLineEnd2)
        {

            generateEndingBoundary();
        }
        else
        {
            _endingBoundaryVisualPoints.clear();
        }


    }

    void setAboutDelete(bool newvalue)
    {
        _aboutDelete=newvalue;
    }
    void checkMouseSelection(float xpos, float ypos,bool& lock)
    {
        for (auto &p: _controlPoints) {   // important here , otherwise, it is a copy, not a reference
            // std::cout<<"drawing"<<std::endl;
            p.checkMouseSelection(xpos, ypos,lock);
        }
    }

    void updatePosition(float xpos, float ypos)
    {
        if (!_lock) {
            for (auto &p: _controlPoints) {   // important here , otherwise, it is a copy, not a reference
                // std::cout<<"drawing"<<std::endl;
                p.updatePosition(xpos, ypos);
            }
            _toRenew=true;
        }
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
            _normalDown.emplace_back(_centerPoints[i].position + (_centerPoints[i].radius2 * -rotateVectdown),
                                              _centerPoints[i].color2);
            _normalUp.emplace_back(_centerPoints[i].position + (_centerPoints[i].radius1 * (rotateVectdown)),
                                            _centerPoints[i].color1);
        }
    }


    void draw() {
        //cleanDeletedPoints();

        if (_toRenew) {
            _toRenew = false;
            generate();
        }
        if (_visibleCurve) {
            if (_visibleControlPoint) {
                drawControlPoints();
                drawSkeleton(_aboutDelete);
            }
            drawBoundary(_aboutDelete);
            drawEndBoundary(_aboutDelete);
        }
    }
    void drawEndBoundary(bool hightlight=false) const
    {
        //std::cout<<"size of _endingBoundaryVisualPoint is "<<_endingBoundaryVisualPoints.size();
        if (!_endingBoundaryVisualPoints.empty()) {
            for (int i = 0; i < _endingBoundaryVisualPoints.size() - 1; i++) {
                glBegin(GL_LINES);
                glColor3f(_endingBoundaryVisualPoints[i].color.x, _endingBoundaryVisualPoints[i].color.y,
                          _endingBoundaryVisualPoints[i].color.z);
                if (hightlight) glColor3f(1,0,0);
                glVertex2f(_endingBoundaryVisualPoints[i].position.x(), _endingBoundaryVisualPoints[i].position.y());
                glVertex2f(_endingBoundaryVisualPoints[i + 1].position.x(),
                           _endingBoundaryVisualPoints[i + 1].position.y());
                glColor3f(1, 1, 1);
                glEnd();

            }
        }
        if (!_startingBoundaryVisualPoints.empty())
        {
        for(int i=0;i< _startingBoundaryVisualPoints.size()-1;i++) {
            glBegin( GL_LINES);
            glColor3f(_startingBoundaryVisualPoints[i].color.x,_startingBoundaryVisualPoints[i].color.y,_startingBoundaryVisualPoints[i].color.z);
            if (hightlight) glColor3f(1,0,0);
            glVertex2f(_startingBoundaryVisualPoints[i].position.x(),_startingBoundaryVisualPoints[i].position.y());
            glVertex2f(_startingBoundaryVisualPoints[i+1].position.x(),_startingBoundaryVisualPoints[i+1].position.y());
            glColor3f(1,1,1);
            glEnd();

        }}

    }

    void drawSkeleton(bool isHighLight=false) const
    {
        // draw skeleton, the center extrapolated thin line
        for(int i=0;i< _centerPoints.size()-1;i++)
        {
            glBegin( GL_LINES);
            if (isHighLight)  glColor3f(1,0,0);
            glVertex2f(_centerPoints[i].position.x(),_centerPoints[i].position.y());
            glVertex2f(_centerPoints[i+1].position.x(),_centerPoints[i+1].position.y());
            glColor3f(1,1,1);
            glEnd();
        }

    }

    void drawBoundary(bool hightlight=false) const
    {
        for(int i=0;i< _normalUp.size()-1;i++)
        {
            glBegin( GL_LINES);
            //std::cout<<_normalUp[i].position<<std::endl;
            glColor3f(_normalUp[i].color.x,_normalUp[i].color.y,_normalUp[i].color.z);
            if (hightlight) glColor3f(1,0,0);
            glVertex2f(_normalUp[i].position.x(),_normalUp[i].position.y());
            glVertex2f(_normalUp[i+1].position.x(),_normalUp[i+1].position.y());
            glEnd();
        }
        for(int i=0;i< _normalDown.size()-1;i++)
        {
            glBegin( GL_LINES);
            glColor3f(_normalDown[i].color.x,_normalDown[i].color.y,_normalDown[i].color.z);
            if (hightlight) glColor3f(1,0,0);
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
        ImGui::Text("Curve attributes");

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
                setAboutDelete(true);
                ImGui::Text("This Curve and all the points will be deleted!!.\nThis operation cannot be undone!\n\n");
                ImGui::Separator();

                //static int unused_i = 0;
                //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button("OK", ImVec2(120, 0))) { setAboutDelete(false);_isDeleted=true;cleandeleted_();ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {  setAboutDelete(false);ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
            ImGui::PushID(_controlPoints.size()); // Use field index as identifier.
            // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGuiTreeNodeFlags flags =
                    ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
            ImGui::TreeNodeEx("Field", flags, "Visibility");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            ImGui::Checkbox("Control Point ##Control Point ", &_visibleControlPoint);
            ImGui::SameLine();
            ImGui::Checkbox("Curve ##Curve ", &_visibleCurve);
            ImGui::NextColumn();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Field", flags, "Straightline");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            if (ImGui::Checkbox("End near start", &_straightLineEnd1)) { std::cout<<"hi end near start"<<_straightLineEnd1;generate();}
            ImGui::SameLine();
            if (ImGui::Checkbox("End near end ", &_straightLineEnd2)) {std::cout<<"hi end near end"<<_straightLineEnd2;generate();}
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Field", flags, "outer color at both ends");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            ImGui::ColorEdit3(" near start ", (float *) &startOuterColor,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
            ImGui::SameLine();
            ImGui::ColorEdit3(" near end ", (float *) &endOuterColor,ImGuiColorEditFlags_PickerHueWheel |ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
            ImGui::NextColumn();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Field", flags, "Lock");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            ImGui::Checkbox("##lock", &_lock);
            ImGui::NextColumn();
            ImGui::TableNextRow();
            /*
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Field", flags, "Degree");

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

            if (ImGui::SliderInt("##degree", &_degree, 0, _controlPoints.size()-1 , "%d")) generate();
            ImGui::NextColumn();
            ImGui::TableNextRow();
            */
            ImGui::PopID();
            for (int i = 0; i < _controlPoints.size(); i++)
            {

                /*
                */
                _controlPoints[i].showCurveproperty("Points", i);
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


    friend void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, const Curve& c )
    {
        boost::json::array points_array;
        for (const auto& point : c._controlPoints)
        {
            boost::json::value point_value;
            tag_invoke(boost::json::value_from_tag{}, point_value, point);
            points_array.push_back(point_value);
        }
        jv = {{"points", points_array}};

    }

    template<class T>
    void extract( boost::json::object const& obj, T& t, std::string_view key )
    {
        t = boost::json::value_to<T>( obj.at( key ) );
    }

    friend Curve tag_invoke(boost::json::value_to_tag< Curve >, boost::json::value const& jv )
    {
        boost::json::object const& obj = jv.as_object();
        return Curve (
                boost::json::value_to< std::vector< Point > >( obj.at( "points" ) ));
    }


    Curve(const Curve&) = delete;
    Curve& operator=(const Curve&) =delete;


};

#endif //FYP_CURVE_H
