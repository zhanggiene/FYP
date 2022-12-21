//
// Created by Zhang Zhuyan on 4/12/22.
//

// https://stackoverflow.com/questions/21070076/opengl-generating-a-2d-texture-from-a-data-array-to-display-on-a-quad
#include "Canvas.h"
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
    imageRed.resize(size,size);
    imageBlue.resize(size,size);
    imageGreen.resize(size,size);
    Mask.resize(size,size);
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

void Canvas::addCurve(Curve* curve) {
     // area for improvement
    _curves.push_back(curve);

}

void Canvas::draw() {


    for (size_t i = 0; i < _curves.size(); i++) {
        if (_arePointsVisible) {
            _curves[i]->drawControlPoints();
        }
        _curves[i]->draw();
    }
    //glFlush();

}

void Canvas::drawFinalImage()
{
    std::cout<<"draw final image"<<std::endl;
    cleardiffuseImage();
    drawToImage();
    diffuse(2);

    int comp=3;
    std::cout<<imageRed.rows();
    std::cout<<imageRed.cols();
    //std::fill(data.begin(), data.end(), 0);
    //uint32_t z = 0; glClearTexImage(texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &z);
    for (unsigned i = 0; i<imageRed.rows();++i)
    {
        for (unsigned j = 0; j < imageRed.cols(); ++j)
        {
            data[(j * imageRed.rows() * comp) + (i * comp) + 0] = imageRed(i,j);
            data[(j * imageRed.rows() * comp) + (i * comp) + 1] = imageGreen(i,j);
            data[(j * imageRed.rows() * comp) + (i * comp) + 2] = imageBlue(i,j);
        }
    }

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
    // this method is not good can be included in the report
    glEnd ();
     */
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

void Canvas::diffuse(int iteration){

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
            if (Mask(i,j))
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
        // std::cout<<"one round "<<A_row<<std::endl<<std::flush;
    std::cout<<"A initialization is done ";
    */
    //std::cout<<A<<"A initialization is done ";
    Eigen::ArrayXXf answerR = Eigen::ArrayXXf::Random(size_,size_);
    Eigen::ArrayXXf answerG = Eigen::ArrayXXf::Random(size_,size_);
    Eigen::ArrayXXf answerB = Eigen::ArrayXXf::Random(size_,size_);
    Eigen::BiCGSTAB<Eigen::SparseMatrix<float> > solver;
    solver.compute(A);
    answerR=Mask.select(imageRed,answerR);
    answerG=Mask.select(imageGreen,answerG);
    answerB=Mask.select(imageBlue,answerB);
    //std::cout<<answerR<<"  ";
    answerR=answerR.abs(); //  the random number can be -1 to 0
    answerG=answerG.abs();
    answerB=answerB.abs();



    Eigen::Map<const Eigen::VectorXf> imageRedv(imageRed.data(), imageRed.size());
    Eigen::Map<const Eigen::VectorXf> imageGreenv(imageGreen.data(), imageGreen.size());
    Eigen::Map<const Eigen::VectorXf> imageBluev(imageBlue.data(), imageBlue.size());
    //std::cout<<imageRedv<<"  ";
    //std::cout<<answerR.size()<<"  ";
    int temp_=0;
    for(temp_=0;temp_<iteration;temp_++)
    {
        Eigen::Map<const Eigen::VectorXf> v1(answerR.data(), answerR.size());
        Eigen::Map<const Eigen::VectorXf> v2(answerG.data(), answerG.size());
        Eigen::Map<const Eigen::VectorXf> v3(answerB.data(), answerB.size());

        //std::cout<<"iteration "<< v1;
        answerR = solver.solveWithGuess(imageRedv,v1).array();
        answerG = solver.solveWithGuess(imageGreenv,v2).array();
        answerB = solver.solveWithGuess	(imageBluev,v3).array();
        //std::cout << "estimated error: " << solver.error();
        answerR.resize(size_,size_);
        answerG.resize(size_,size_);
        answerB.resize(size_,size_);

        answerR = Mask.select(imageRed, answerR);
        answerG = Mask.select(imageGreen, answerG);
        answerB = Mask.select(imageBlue, answerB);
        answerR = answerR.cwiseMax(0);
        answerG = answerG.cwiseMax(0);
        answerB = answerB.cwiseMax(0);


    }

    answerR.resize(size_,size_);
    imageRed=answerR;
    answerG.resize(size_,size_);
    imageGreen=answerG;
    answerB.resize(size_,size_);
    imageBlue=answerB;
    //std::cout<<imageRed<<"  ";

}

void Canvas::addVisualPoint(const VisualPoint &p_) {

    imageRed(int(p_.position.x()),int(p_.position.y())) = p_.color.x;
    imageGreen(int(p_.position.x()),int(p_.position.y())) = p_.color.y;
    imageBlue(int(p_.position.x()),int(p_.position.y())) = p_.color.z;
    Mask(int(p_.position.x()),int(p_.position.y()))=true;
    counter+=1;
}
void Canvas::drawToImage()
{
    for (auto curve : _curves)
    {
        for (int i=0; i<curve->_normalUp.size();i++)
        {
            int shiftx=-1;
            if (curve->_normalUp[i].position.x() < curve->_centerPoints[i].position.x()) {
                shiftx = 1;
            }
            int shifty=-1;
            if (curve->_normalUp[i].position.y() < curve->_centerPoints[i].position.y()) {
                shifty = 1;
            }
            int xpos= int (curve->_normalUp[i].position.x())+shiftx;
            int ypos= int (curve->_normalUp[i].position.y())+shifty;
            addVisualPointXY(xpos,ypos, curve->_normalUp[i].color);
            xpos= int (curve->_normalUp[i].position.x())-shiftx;
            ypos= int (curve->_normalUp[i].position.y())-shifty;
            //addVisualPointXY(xpos,ypos, curve->_centerPoints[i].colorOuter1);

        }
        // TODO if the error rate is small, no need more iteration
        for (int i=0; i<curve->_normalDown.size();i++)
        {
            int shiftx=-1;
            if (curve->_normalDown[i].position.x() < curve->_centerPoints[i].position.x()) {
                shiftx = 1;
            }
            int shifty=-1;
            if (curve->_normalDown[i].position.y() < curve->_centerPoints[i].position.y()) {
                shifty = 1;
            }
            int xpos= int (curve->_normalDown[i].position.x())+shiftx;
            int ypos= int (curve->_normalDown[i].position.y())+shifty;
            addVisualPointXY(xpos,ypos, curve->_normalDown[i].color);
            xpos= int (curve->_normalDown[i].position.x())-shiftx;
            ypos= int (curve->_normalDown[i].position.y())-shifty;
            //addVisualPointXY(xpos,ypos, curve->_centerPoints[i].colorOuter2);

        }



    }
}

void Canvas::addVisualPointXY(int x, int y, const ImVec4 &color) {


    imageRed(x,y) = color.x;
    imageGreen(x,y) = color.y;
    imageBlue(x,y) = color.z;
    Mask(x,y)=true;


}

void Canvas::checkMouseSelection(float xpos, float ypos)
{
    for (auto c: _curves) {
        c->checkMouseSelection(xpos, ypos);
    }
}


void Canvas::updatePosition(float xpos, float ypos)
{
    for (auto c: _curves) {
        c->updatePosition(xpos, ypos);
    }
}
