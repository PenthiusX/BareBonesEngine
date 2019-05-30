#include <iostream>
#include "_glwidget.h"
#include <QDebug>
#define PI 3.1415926535897932384626433832795

/*
 * The _GLWidget Class:
 * This class is the Controller is a typical MVC where the 
 * View and Controller are being handeled by the QT GUI frameowrk and 
 * its wrapper functions for creating Opengl and GUI windowing context.
 * its wrapper functions for creating Opengl and GUI windowing context.
 * Created: 5_02_2019
 * Author: Aditya,Saurabh
*/
/*
* Constructor:_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
* sets and passes the context for the Qopengl widget here for use.
* Created: 5_02_2019
*/
_GLWidget::_GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	id = 0;
}
_GLWidget::~_GLWidget()
{
	delete scene;
}
/*
* Function: initializeGL() overrides the 
* same function in the OpopenglFunctions class
* runs once the opengl context is initialised.
* Created: 5_02_2019
*/
void _GLWidget::initializeGL()
{
    //needs this to make the GL widgit have the strongest focus when switching widgets.
    //keeps the event callbacks working for the GL widget
    setFocusPolicy(Qt::StrongFocus);

	cam.setEyePosition(QVector3D(0.0, 0.0, -7.0));
	cam.setFocalPoint(QVector3D(0.0, 0.0, 0.0));

	s.setId(0);
	s.setShader(":/shaders/vshader1.glsl", ":/shaders/fshader1.glsl");
    s.setPosition(QVector3D(0.0,0.0,0.0));
	s.setScale(0.5);
	s.setModelData(":/models/torus.obj");

	s1.setId(1);
	s1.setShader(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
    s1.setPosition(QVector3D(0.0,0.0,0.0));
	s1.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));
    s1.setScale(0.5);
    s1.setModelData(":/models/cone.obj");

    s2.setId(2);
    s2.setShader(":/shaders/texVshader.glsl", ":/shaders/texFshader.glsl");//texture Compliable shader not complete//need to pass UVs externally//
    s2.setPosition(QVector3D(0.0,0.0,0.0));
    s2.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));
    s2.setScale(0.5);
    s2.setModelData(":/models/monkey.obj");
    s2.setTexturePath(":textures/eye.png");//needs a texture compliable shader attached too


    background_quad.setId(3);//keep the id it will be required while updating texture
    background_quad.setShader(":/shaders/vshader_background.glsl", ":/shaders/fshader_background.glsl");//texture Compliable shader not complete//need to pass UVs externally//

    //background quad is not affected by mvp hence this functions will not work :-
    background_quad.setPosition(QVector3D(0.0, 0.0, 0.0));
    background_quad.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));

    background_quad.setScale(0.1);

    /*Hard coded vertices of background quad the vertices are
     * directly asiigned to gl_Position(screen cordinates)
     * such that position of background always remain same*/
    std::vector<float> vertsV = {
        1.0,  1.0, 0.999f,	// top right
        1.0f, -1.0f, 0.999f,  // bottom right
        -1.0f, -1.0f, 0.999f, // bottom left
        -1.0f,  1.0f, 0.999f  // top left
       };
    /*Hard coded Indices*/
    std::vector<unsigned int> indiceV = {// note that we start from 0!
                                0, 1, 3,   // first triangle
                                1, 2, 3    // second triangle
                               };


    background_quad.setModelData(vertsV,indiceV);
    //background_quad.setModelData(":/models/monkey.obj");
    //background_quad.setTexturePath(":textures/eye.png");//do not set texture from file since format mismatch will occur


    /*Hard coded vertices of background quad the vertices are
     * directly asiigned to gl_Position(screen cordinates)
     * such that position of background always remain same*/

    ///////// temporary
    ///

    generated_model.setId(4);//keep the id it will be required while updating texture
    generated_model.setShader(":/shaders/generated_model_vertex_edge.glsl", ":/shaders/generated_model_fragment.glsl");//texture Compliable shader not complete//need to pass UVs externally//

    //background quad is not affected by mvp hence this functions will not work :-
    generated_model.setPosition(QVector3D(0.125,-1.045, -1.0));//1.045
    generated_model.setRotation(QQuaternion(QVector3D(0.0, 0.0, 0.0)));

    generated_model.setScale(0.524);//0.524

    std::vector<float> vertsG;
    std::vector<unsigned int> indiceG;

    glm::ivec2 resolution = glm::ivec2(200,576);//wrap texture size
    unsigned int index[4] = {0,0,0,0};

    for (unsigned int h = 0; h < resolution.y; h++) {
        for (unsigned int w = 0; w < resolution.x; w++) {

            glm::vec2 pixel_cord = glm::vec2(w,h);

            glm::vec2 texture_cord = glm::vec2((pixel_cord.x +0.5)/resolution.x,(pixel_cord.y +0.5)/resolution.y);

            //texture_positions
            vertsG.push_back(texture_cord.x);//x = s
            vertsG.push_back(texture_cord.y);//y = t
            vertsG.push_back(0.0);//z = 0.0

        }
    }

    glm::ivec2 step_size = glm::ivec2(2,2);

    for (unsigned int h = 0; h < resolution.y; h+=step_size.y) {
        for (unsigned int w = 0; w < resolution.x; w+=step_size.x) {

            glm::vec2 pixel_cord = glm::vec2(w,h);

            //indexes of neibhouring vertexes
            index[0] = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
            index[1] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,0),resolution);
            index[2] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,step_size.y),resolution);
            index[3] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(0,step_size.y),resolution);

//            if((pixel_cord.y < resolution.y) && (pixel_cord.y > 80))
//            {


            if((pixel_cord.y < resolution.y))
            {
                //indexs of fisrt triangle in quad
                indiceG.push_back(index[0]);
                indiceG.push_back(index[1]);
                indiceG.push_back(index[2]);

                //indexs of second triangle in quad
                indiceG.push_back(index[0]);
                indiceG.push_back(index[2]);
                indiceG.push_back(index[3]);
            }
        }
    }

    //generated_model.setTexturePath(":/textures/cylinder_wrap.png");
    //generated_model.setTexturePath(":textures/eye.png");


    generated_model.setModelData(vertsG,indiceG);

    ///////// /temporary


	scene = new _Scene();
	scene->addCamera(cam);

    scene->addSceneObject(background_quad);

    //scene->addSceneObject(s);
    //scene->addSceneObject(s1);
    //scene->addSceneObject(s2);
    scene->addSceneObject(generated_model);

    initialised=true;

    rotateGeneratedModel(0);
}

/*
 * Function: resizeGL(int w, int h) overides the
 * function in OpopenglFunctions class.
 * pasees the current width and height 
 * of the layout via - int w and int h
 * Created: 5_02_2019
*/
void _GLWidget::resizeGL(int w, int h)
{
	this->width = w;
	this->height = h;
	scene->onResize(w, h);
}

/*
 * Function: paintGl() 7
 * ovveriding thes function in OpopenglFunctions
 * Your proprietory Draw function this run in a loop 
 * till the application ends.
 * Created: 5_02_2019
*/
void _GLWidget::paintGL()//the renderloop
{
	scene->render();
    this->update();
}
/*
* Function: mousePressEvent(QMouseEvent *e)
* this is a overriden function from the QWidget parent
* runs each time the mouse is pressed.
* Created: 5_02_2019
*/
void _GLWidget::mousePressEvent(QMouseEvent *e)
{
	mousePressPosition = QVector2D(e->localPos());
    m_lastPos = e->pos();
}
/*
* Function: mouseReleaseEvent(QMouseEvent *e)
* This is a overriden function from the QWidg       et parent
* runs each time the mouse is released.
* Created: 5_02_2019
*/
void _GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
}
/*
* Function: keyPressEvent(QKeyEvent * event)
* runns anytime a key is presses and returns which key through the
* event pointer of QKeyEvent object.
* Created: 25_02_2019
*/
void _GLWidget::keyPressEvent(QKeyEvent * event)
{
	if (event->text() == "q" || event->text() == "Q")
		id += 1;
        if (id >= scene->getSceneObjectsArray().size())
			id = 0;

	if (event->text() == "d" || event->text() == "D")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(0.0,0.1f,0.0));

	if (event->text() == "a" || event->text() == "A")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(0.0, -0.1f, 0.0));

    if (event->text() == "w" || event->text() == "W")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(0.1f, 0.0, 0.0));

	if (event->text() == "s" || event->text() == "S")
        for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
			if (scene->getSceneObjectsArray()[i]->getSceneEntity().getId() == id)
                scene->getSceneObjectsArray()[i]->updateTrasformations(QVector3D(-0.1f, 0.0, 0.0));
}


/* Function : update_background_image(char *img, unsigned int w, unsigned int h)
 * upadtes the texture when new camera image is grabbed or saved image is to be displayed
 * to upadte the texture image 8 bit grayscale image is required hence do not set the texure
 * while creating scenentity else color format mismatch will occur between GL_RGBA and GL_RED
 * since default texture format is GL_RGBA
 *
 * created: 11_04_2019
 * Contributor : Saurabh
*/
void _GLWidget::update_background_image(char *img, unsigned int w, unsigned int h)
{
    static _Renderer *render_object = nullptr;
    for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
    {
        render_object = scene->getSceneObjectsArray()[i];

        if (render_object->getSceneEntity().getId() == background_quad.getId())
        {
            //make context active
            makeCurrent();

            if(render_object->isTexturePresent()){
                //updating predefined texture
                render_object->setTexture(img,w,h);
            }
            else {
                //setting up new 8 bit grayscale GL_RGBA texture for first time
                render_object->setupTexture(img,w,h,GL_RGBA);
            }

            doneCurrent();
        }
    }
}

void _GLWidget::showGeneratedModel(char *img, unsigned int w, unsigned int h)
{
    static _Renderer *render_object = nullptr;
    for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
    {
        render_object = scene->getSceneObjectsArray()[i];

        if (render_object->getSceneEntity().getId() == generated_model.getId())
        {
            //make context active
            makeCurrent();

            if(render_object->isTexturePresent()){
                //updating predefined texture
                render_object->setTexture(img,w,h);
            }
            else {
                //setting up new 8 bit grayscale GL_RGBA texture for first time
                render_object->setupTexture(img,w,h,GL_RED_INTEGER,GL_INT,GL_R32I);
            }
            doneCurrent();
        }
    }
}

void _GLWidget::rotateGeneratedModel(float angle)
{
    rotateGeneratedmodel(angle, glm::vec3(0.0f, 1.0f, 0.0f),true);
}
void _GLWidget::rotateGeneratedmodel(float angle,glm::vec3 axis,bool with_stage)
{
    static _Renderer *render_object = nullptr;

    if(initialised)
    {
    for (unsigned int i = 0; i < scene->getSceneObjectsArray().size(); i++)
    {
        render_object = scene->getSceneObjectsArray()[i];

        if (render_object->getSceneEntity().getId() == generated_model.getId())
        {
            if(with_stage){
                static glm::mat4x4 rot_mat = render_object->getModelMatrix();
                glm::mat4x4 rot_mat_local = glm::rotate(rot_mat, (-angle), axis);
                render_object->setModelMatrix(rot_mat_local);
                rot_mat = rot_mat_local;

            }
            else {

                glm::mat4x4 rot_mat_local = render_object->getModelMatrix();
                rot_mat_local = glm::translate(rot_mat_local, glm::vec3(0.0, 2.0, 0.0));
                rot_mat_local = glm::rotate(rot_mat_local, (-angle), axis);
                rot_mat_local = glm::translate(rot_mat_local, glm::vec3(0.0, -2.0, 0.0));
                render_object->setModelMatrix(rot_mat_local);
            }
        }
    }
    }
}

bool _GLWidget::isInitialised()
{
    return initialised;
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void _GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + dy);
        setYRotation(m_yRot + dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot +  dy);
        setZRotation(m_zRot +  dx);
    }
    m_lastPos = event->pos();
}

void _GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        //emit xRotationChanged(angle);
        rotateGeneratedmodel(float(angle)/18000.0,glm::vec3(1.0,0.0,0.0),false);
        update();
    }
}

void _GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        //emit yRotationChanged(angle);
        rotateGeneratedmodel(float(angle)/18000.0,glm::vec3(0.0,1.0,0.0),false);
        update();
    }
}

void _GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        //emit zRotationChanged(angle);
        rotateGeneratedmodel(float(angle)/18000.0,glm::vec3(0.0,0.0,1.0),false);
        update();
    }
}
