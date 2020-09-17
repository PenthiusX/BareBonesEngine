#include "_texture.h"
#include <QDebug>

/* Texture Class currently working for single texture per shader
 * documentation and slots functions will be completed later
 * Author: Aditya
 */

/* Constructor/Distructor:Texture Class constructor
 * initialize empty texture
 * Date:28_03_2019
*/
_Texture::_Texture() : QOpenGLExtraFunctions(QOpenGLContext::currentContext()){}
/*
 * Constructor: _Texture(char *img, uint w, uint h,uint colorFormat) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
 * initialize texture from char pointer array with given resolution
 * sets defualt parameters
*/
_Texture::_Texture(char *img, uint w, uint h,uint colorFormat) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    image = img;
    width = w;
    height = h;
}
/* Constructor: _Texture(QImage& img) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
 * initialize texture from QImage
 * sets defualt parameters
*/
_Texture::_Texture(QImage& img) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    qDebug() << "tex contruct";
    image = (char*)img.bits();
    width = img.width();
    height = img.height();
}
/* Function: setImage(QString pathtoTexture)
 *  Updates texture image from image file path
*/
void _Texture::setImage(QString pathtoTexture)
{
    QImage img = QImage(pathtoTexture);
    qDebug() << "tex contruct";
    image = (char*)img.bits();
    width = img.width();
    height = img.height();
}

/* Function:setImage(QImage& img)
 *
*/
void _Texture::setImage(QImage& img)
{
    image = (char*)img.bits();
    width = img.width();
    height = img.height();
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, image);
}

/* Updates texture from char pointer array and resolution of last image
*/
void _Texture::setImage(char* img)
{
    image = img;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, image);
}

/* Updates texture from char pointer array with updated given resolution
*/
void _Texture::setImage(char* img,uint iwidth,uint iheight)
{
    width = iwidth;
    height = iheight;
    setImage(img);
}

/* bind texture to memory
 * without any activeTexture slot specified
 * it will bind to 0th texture slot
*/
void _Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
}

/* bind texture to a slot defined by the index
 * no provided
 *
*/
void _Texture::bind(uint index)
{
    switch (index) {
           case 1: glActiveTexture(GL_TEXTURE1);
               break;
           case 2: glActiveTexture(GL_TEXTURE2);
               break;
           case 3: glActiveTexture(GL_TEXTURE3);
               break;
    }
    glBindTexture(GL_TEXTURE_2D,m_ID);
}
/* unbinds the default
*/
void _Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D,0);
}

/* initializes texture
 * applys parameters
 * loads the image
*/
void _Texture::load( GLenum format, GLenum datatype)
{
    if(m_ID==0)
    {
        glGenTextures(1,&m_ID);
    }
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    color_format = format;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, datatype, image);
//    glGenerateMipmap(GL_TEXTURE_2D);
}

void _Texture::load(QImage image,Type t,_Shader* s, GLenum format, GLenum datatype)
{
    char* img = (char*)image.bits();
    this->image = img;
    width = image.width();
    height = image.height();
    if(m_ID==0){
        glGenTextures(1,&m_ID);
    }
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    color_format = format;
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width(), image.height(), 0, color_format, datatype, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    s->useShaderProgram();//need to do this before setting the uniform
    switch (t) {//sets the Activetexture location of the sampler uniforms in the shader
           case Diffuse: glUniform1i(s->getUniformLocation("diffuseTex"), 1);
               break;
           case Specular: glUniform1i(s->getUniformLocation("specularTex"), 2);
               break;
           case Bump: glUniform1i(s->getUniformLocation("bumpTex"), 3);
               break;
    }
}

//return width of loaded texture image
uint _Texture::getWidth() const
{
    return width;
}

//return height of loaded texture image
uint _Texture::getHeight() const
{
    return height;
}
