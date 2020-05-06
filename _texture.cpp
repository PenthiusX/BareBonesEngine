#include "_texture.h"
#include <QDebug>

/* Texture Class currently working for single texture per shader
 * documentation and slots functions will be completed later
 */

/* Constructor/Distructor:Texture Class constructor
 * initialize empty texture
 * Date:28_03_2019
*/
_Texture::_Texture() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}
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

    //addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    //addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

    //    addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
    qDebug() << "setting image" << GL_RED << color_format;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, image);
}

/* Function:setImage(QImage& img)
 *
*/
void _Texture::setImage(QImage& img)
{
    image = (char*)img.bits();
    width = img.width();
    height = img.height();
    qDebug() << "setting image" << GL_RED << color_format;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, image);
}

/* Updates texture from char pointer array and resolution of last image
*/
void _Texture::setImage(char* img)
{
    image = img;
    qDebug() << "setting image" << GL_RED << color_format;
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

/* bind texture to default slot(0)
*/
void _Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
}

/* bind texture default given slot index
*/
void _Texture::bind(uint index)
{
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,m_ID);
}

/* bind texture for compute shader operation
 *   index : binding index integer specified in shader
 *   format : data format eg. RGBA8,R8
 *   access : data access eg. GL_READ_ONLY,GL_WRITE_ONLY,GL_READ_WRITE,
*/
void _Texture::bindForCompute(uint index, GLenum format, GLenum access)
{
    glBindImageTexture(index, m_ID, 0, GL_FALSE, 0, access,format );
}

/* bind texture for framebuffer target
 *   index : attatchment index integer
 *   operation : framebuffer operation eg. GL_FRAMEBUFFER,GL_READ_FRAMEBUFFER,GL_DRAW_FRAMEBUFFER,
*/
void _Texture::bindForFramebuffer(uint index, GLenum operation)
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glFramebufferTexture2D(operation,GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, m_ID, 0);
}

/* unbinds the default
*/
void _Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D,0);
}

/* add texture parameters manually
 * these parameters will be applied in load function
*/
void _Texture::addParameter(uint pname, uint param)
{
    parameters[pname] = param;
}

/* initializes texture
 * applys parameters
 * loads the image
*/
void _Texture::load( GLenum format, GLenum datatype)
{
//    qDebug() << "tex load";
    if(m_ID==0)
    {
//        qDebug() << "tex gen b" << m_ID;
        uint t;
        glGenTextures(1,&m_ID);
//        qDebug() << "tex gen" << m_ID;
    }
    bind();
    for (auto const& parameter : parameters)
    {
        glTexParameteri(GL_TEXTURE_2D,parameter.first, parameter.second);//second specifies value at key in map(dictionary)
    }
    color_format = format;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, datatype, image);
    //glGenerateMipmap(GL_TEXTURE_2D);
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
