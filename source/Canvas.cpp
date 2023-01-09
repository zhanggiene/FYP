//
// Created by Zhang Zhuyan on 4/12/22.
//

// https://stackoverflow.com/questions/21070076/opengl-generating-a-2d-texture-from-a-data-array-to-display-on-a-quad
#include "Canvas.h"
#include "App.h"

Canvas::Canvas()
        :
          displayMode(false),
          _arePointsVisible(true),
          counter(0)
{
}
void Canvas::setSize(unsigned int size)
{
    size_=size;
    imageRed.resize(size*size);
    imageBlue.resize(size*size);
    imageGreen.resize(size*size);
    Mask.resize(size*size);
    cleardiffuseImage();
    data.resize(imageRed.size()*3,0);
}

void Canvas::cleardiffuseImage()
{
    Mask.fill(false);
    imageRed.fill(0);
    imageBlue.fill(0);
    imageGreen.fill(0);
}

void Canvas::initializeTexture()
{

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}
void Canvas::clear()
{

}
void Canvas::setdisplayMode(bool bl_)
{
    displayMode=bl_;
}

Canvas::~Canvas()
{
    clear();
}

void Canvas::addCurve(outerclass&& curve) {
     // area for improvement
     //curve._controlPoints[0].f_();
    //_curves.push_back(std::forward(curve));
    //curve->setCallBack(std::bind(&Canvas::generate, this));


}
void Canvas::save()
{
    lTheSaveFileName = tinyfd_saveFileDialog(
            "let us save this password",
            "curve1.txt",
            1,
            lFilterPatterns,
            NULL);
    lIn = fopen(lTheSaveFileName, "w");
    if (!lIn)
    {
        tinyfd_messageBox(
                "Error",
                "Can not open this file in write mode",
                "ok",
                "error",
                1);
    }
    fputs(serialize( boost::json::value_from(_curves )).c_str(), lIn);
    fclose(lIn);
    std::cout<<" save this message"<<"";
}

void Canvas::loadJson()
{
    lTheOpenFileName = tinyfd_openFileDialog(
		"load control points",
		"",
		1,
		lFilterPatterns,
		NULL,
		0);
    if (! lTheOpenFileName)
    {
        tinyfd_messageBox(
                "Error",
                "Open file name is NULL",
                "ok",
                "error",
                1);
        return ;
    }

    lIn = fopen(lTheOpenFileName, "r");

    if (!lIn)
    {
        tinyfd_messageBox(
                "Error",
                "Can not open this file in read mode",
                "ok",
                "error",
                1);
        return ;
    }
    //lBuffer[0] = '\0';
    //boost::json::value jv = boost::json::parse( lBuffer );
    boost::json::stream_parser p;
    while(fgets(lBuffer,  sizeof(lBuffer), lIn) != NULL)
    {
        /* Do stuff. */
        p.write(lBuffer);
    }
    p.finish();
    fclose(lIn);
    _curves=boost::json::value_to< std::vector< outerclass > >( p.release() );
    addcallBack();
}

void Canvas::addcallBack()
{
    for(int i=0;i<_curves.size();i++)
    {
        _curves[i].setCleanCallBack(std::bind(&Canvas::cleanDeletedCurve, this));
        //_controlPoints[i].setCallBack([](){std::cout<<"hi";});
    }
}


void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, Canvas const& c )
{
    jv ={
            {"curves",c._curves }};

}

void Canvas::ShowPropertyEditor()
{
    ShowAppMainMenuBar();
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(App::getEditMode().c_str(),NULL,window_flags))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        // Iterate placeholder objects (all the same data)
        for (int obj_i = 0; obj_i < _curves.size(); obj_i++)
        {
            //ImGui::Separator();
           _curves[obj_i].showCruveproperty(("Curve"+std::to_string(obj_i)).c_str(),obj_i);
            //ShowPlaceholderObject("Object", obj_i);
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void Canvas::ShowAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { loadJson();}
            if (ImGui::MenuItem("save")) {save();}

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Clear", "CTRL+C")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Canvas::draw() {


    for (size_t i = 0; i < _curves.size(); i++) {
        _curves[i].draw();
    }
    // draw property windows as well
    ShowPropertyEditor();

}

void Canvas::drawFinalImage()
{
    std::cout<<"draw final image"<<std::endl;
    cleardiffuseImage();
    drawToImage();
    //diffuse(1);
    multigrid();

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,            // Internal colour format to convert to
                 size_,          // Image width  i.e. 640 for Kinect in standard mode
                 size_,          // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 GL_RGB, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                 GL_FLOAT,  // Image data type
                 data.data());        // The actual image data itself
    //displayFinalImage();


}
void Canvas::displayFinalImage()
{


    /*glBegin (GL_POINTS);
    for(int i=0;i<size_;i++)
    {
        for(int j=0;j<size_;j++)
        {

            glColor3f(imageRed(i,j), imageGreen(i,j), imageBlue(i,j));
            glVertex2i(i, j);
        }
    }
     */
    // this method is not good can be included in the report
    //glEnd ();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture);



    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0,   0);
    glTexCoord2i(0, 1); glVertex2i(0,   size_);
    glTexCoord2i(1, 1); glVertex2i(size_, size_);
    glTexCoord2i(1, 0); glVertex2i(size_, 0);
    glEnd();

    //glDeleteTextures(1, &texture);

    glDisable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);


}
void Canvas::constructA(int size_)
{
    A.resize(size_* size_,size_* size_);
    A.reserve(counter+size_*4);
    //std::cout<<" Size of Mask is "<<counter+size_*4<<std::endl;
    /*for(int i=0;i<size_*size_;i++)
    {
        A.coeffRef(i,i)=1;
    }*/
    for(unsigned  i =0;i<size_;i++)
    {
        for(unsigned  j=0;j<size_;j++) {
            if (Mask(i*size_+j))
            {A.coeffRef(i*size_+j,i*size_+j)=1;}
            else if (i ==0  || i == size_-1 || j==0 || j==size_-1 )
            {
                A.coeffRef(i*size_+j,i*size_+j)=1;
            }
            else{
                A.coeffRef(i*size_+j,i*size_+j)=4;
                A.coeffRef(i*size_+j,i*size_+j+1)=-1;
                A.coeffRef(i*size_+j,i*size_+j-1)=-1;
                A.coeffRef(i*size_+j,(i+1)*size_+j)=-1;
                A.coeffRef(i*size_+j,(i-1)*size_+j)=-1;
            }
        }
    }

    //std::cout<<A;
}
void Canvas::multigrid()
{
    constructA(size_);
    typedef amgcl::backend::builtin<float> Backend;
    typedef amgcl::make_solver<
            // Use AMG as preconditioner:
            amgcl::amg<
                    Backend,
                    amgcl::coarsening::smoothed_aggregation,
                    amgcl::relaxation::spai0
            >,
            // And BiCGStab as iterative solver:
            amgcl::solver::bicgstab<Backend>
    > Solver;
    boost::property_tree::ptree prm;
    prm.put("solver.tol", 1e-3);
    Solver solve(A,prm);
    std::cout << solve << std::endl;
    int    iters=0;
    float error=0;
    // setting the inistial value of result is the guess condition, very important, otheriwse , it does not converge
    // the solver will throw error.
    std::vector<float> result1(imageRed.data(), imageRed.data()+imageRed.size());
    std::vector<float> rhs(imageRed.data(), imageRed.data()+imageRed.size());
    std::tie(iters, error) = solve(rhs, result1);
    std::cout<<"error is "<<error<<std::endl;
    std::vector<float> result2(imageGreen.data(), imageGreen.data()+imageGreen.size());
    std::vector<float> rhs2(imageGreen.data(), imageGreen.data()+imageGreen.size());
    std::tie(iters, error) = solve(rhs2, result2);
    std::cout<<"error is "<<error<<std::endl;
    std::vector<float> result3(imageBlue.data(), imageBlue.data()+imageBlue.size());
    std::vector<float> rhs3(imageBlue.data(), imageBlue.data()+imageBlue.size());
    std::tie(iters, error) = solve(rhs3, result3);
    std::cout<<"error is "<<error<<std::endl;
    int comp=3;
    //std::fill(data.begin(), data.end(), 0);
    //uint32_t z = 0; glClearTexImage(texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &z);
    for (unsigned i = 0; i<size_;++i)
    {
        for (unsigned j = 0; j < size_; ++j)
        {
            data[(j * size_ * comp) + (i * comp) + 0] = result1[i*size_+j];
            data[(j * size_ * comp) + (i * comp) + 1] = result2[i*size_+j];
            data[(j * size_ * comp) + (i * comp) + 2] = result3[i*size_+j];
        }
    }
    std::cout<<iters<<"  ";
    std::cout<<"error is "<<error;


}
void Canvas::diffuse(int iteration){
    constructA(size_);


    /*
     for(unsigned A_row=0;A_row<size_* size_;A_row++)
    {

        if (Mask(i,j))
        {

        for(int temp=0;temp<size_;temp++)
        {
            A.coeffRef(A_row,temp) = 1;  // the first temp top one row
            A.coeffRef(A_row,size_*size_-temp-1) = 1; // the last one row
            A.coeffRef(A_row,temp*size_)=1;  // leftmost column
            A.coeffRef(A_row,temp*(size_-1))=1;

        }

    for(unsigned  i =1;i<size_-1;i++)
    {
        for(unsigned  j=1;j<size_-1;j++)
        {
         if (Mask(i,j))
         {
                A.coeffRef(A_row,i*j)=4;
                A.coeffRef(A_row,i*j-1)=-1;
                A.coeffRef(A_row,i*j+1)=-1;
                A.coeffRef(A_row,(i-1)*j)=-1;
                A.coeffRef(A_row,(i+1)*j)=-1;
            }
        }
    }
    */
    Eigen::ArrayXf answerR = Eigen::ArrayXf::Random(size_*size_);
    Eigen::ArrayXf answerG = Eigen::ArrayXf::Random(size_*size_);
    Eigen::ArrayXf answerB = Eigen::ArrayXf::Random(size_*size_);
    Eigen::BiCGSTAB<Eigen::SparseMatrix<float> > solver;
    solver.compute(A);
    solver.setTolerance(0.001);
    // TODO what is the idea tolerance ?
    solver.setMaxIterations(100);
    answerR=Mask.select(imageRed,answerR);
    answerG=Mask.select(imageGreen,answerG);
    answerB=Mask.select(imageBlue,answerB);
    //std::cout<<answerR<<"  ";
    answerR=answerR.abs(); //  the random number can be -1 to 0
    answerG=answerG.abs();
    answerB=answerB.abs();


    Eigen::Map<const Eigen::VectorXf> imageRedv(imageRed.data(), imageRed.size());// very important,
    // not just .array()
    Eigen::Map<const Eigen::VectorXf> imageGreenv(imageGreen.data(), imageGreen.size());
    Eigen::Map<const Eigen::VectorXf> imageBluev(imageBlue.data(), imageBlue.size());

    int temp_=0;
    for(temp_=0;temp_<iteration;temp_++)
    {
        Eigen::Map<const Eigen::VectorXf> v1(answerR.data(), answerR.size());
        Eigen::Map<const Eigen::VectorXf> v2(answerG.data(), answerG.size());
        Eigen::Map<const Eigen::VectorXf> v3(answerB.data(), answerB.size());

        answerR = solver.solveWithGuess(imageRedv,v1).array();
        answerG = solver.solveWithGuess(imageGreenv,v2).array();
        answerB = solver.solveWithGuess	(imageBluev,v3).array();


        answerR = Mask.select(imageRed, answerR);
        answerG = Mask.select(imageGreen, answerG);
        answerB = Mask.select(imageBlue, answerB);
        answerR = answerR.cwiseMax(0);
        answerG = answerG.cwiseMax(0);
        answerB = answerB.cwiseMax(0);


    }
    imageRed=answerR;
    imageGreen=answerG;
    imageBlue=answerB;
    int comp=3;
    //std::fill(data.begin(), data.end(), 0);
    //uint32_t z = 0; glClearTexImage(texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &z);
    for (unsigned i = 0; i<size_;++i)
    {
        for (unsigned j = 0; j < size_; ++j)
        {
            data[(j * size_ * comp) + (i * comp) + 0] = imageRed(i*size_+j);
            data[(j * size_ * comp) + (i * comp) + 1] = imageGreen(i*size_+j);
            data[(j * size_ * comp) + (i * comp) + 2] = imageBlue(i*size_+j);
        }
    }
}

void Canvas::addVisualPoint(const VisualPoint &p_) {

    imageRed(int(p_.position.x())*size_+int(p_.position.y())) = p_.color.x;
    imageGreen(int(p_.position.x())*size_+int(p_.position.y())) = p_.color.y;
    imageBlue(int(p_.position.x())*size_+int(p_.position.y())) = p_.color.z;
    Mask(int(p_.position.x())*size_+int(p_.position.y()))=true;
    counter+=1;
}

void Canvas::cleanDeletedCurve()
{

    _curves.erase(std::remove_if(_curves.begin(), _curves.end(), [](const outerclass &h) { return h._isDeleted; }), _curves.end());
        // erase is not working, as the curve number is

    }
void Canvas::drawToImage()
{
    for (auto &curve : _curves)
    {
        for (int i=0; i<curve._normalUp.size();i++)
        {
            int shiftx=-1;
            if (curve._normalUp[i].position.x() < curve._centerPoints[i].position.x()) {
                shiftx = 1;
            }
            int shifty=-1;
            if (curve._normalUp[i].position.y() < curve._centerPoints[i].position.y()) {
                shifty = 1;
            }
            int xpos= int (curve._normalUp[i].position.x())+shiftx;
            int ypos= int (curve._normalUp[i].position.y())+shifty;
            addVisualPointXY(xpos,ypos, curve._normalUp[i].color);
            xpos= int (curve._normalUp[i].position.x())-shiftx;
            ypos= int (curve._normalUp[i].position.y())-shifty;
            addVisualPointXY(xpos,ypos, curve._centerPoints[i].colorOuter1);

        }
        // TODO if the error rate is small, no need more iteration
        for (int i=0; i<curve._normalDown.size();i++)
        {
            int shiftx=-1;
            if (curve._normalDown[i].position.x() < curve._centerPoints[i].position.x()) {
                shiftx = 1;
            }
            int shifty=-1;
            if (curve._normalDown[i].position.y() < curve._centerPoints[i].position.y()) {
                shifty = 1;
            }
            int xpos= int (curve._normalDown[i].position.x())+shiftx;
            int ypos= int (curve._normalDown[i].position.y())+shifty;
            addVisualPointXY(xpos,ypos, curve._normalDown[i].color);
            xpos= int (curve._normalDown[i].position.x())-shiftx;
            ypos= int (curve._normalDown[i].position.y())-shifty;
            addVisualPointXY(xpos,ypos, curve._centerPoints[i].colorOuter2);

        }



    }
}

void Canvas::addVisualPointXY(int x, int y, const ImVec4 &color) {


    imageRed(x*size_+y) = color.x;
    imageGreen(x*size_+y) = color.y;
    imageBlue(x*size_+y) = color.z;
    Mask(x*size_+y)=true;


}

void Canvas::checkMouseSelection(float xpos, float ypos,bool& lock)
{
    for (auto &c: _curves) {
        c.checkMouseSelection(xpos, ypos,lock);
    }
}



void Canvas::updatePosition(float xpos, float ypos)
{
    for (auto &c: _curves) {
        c.updatePosition(xpos, ypos);
    }
}
