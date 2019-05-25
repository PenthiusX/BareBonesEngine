#include "_texture.h"
#include <QDebug>




/* Texture Class currently working for single texture per shader
 * documentation and slots functions will be completed later
 * Created: 28_03_2019
 * Author:Saurabh
 */

/* Texture Class constructor
 * initialize empty texture
*/
_Texture::_Texture() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}

/* Texture Class constructor
 * initialize texture from char pointer array with given resolution
 * Created: 21_02_2019
 * sets defualt parameters
*/
_Texture::_Texture(char *img,unsigned int w,unsigned int h, GLenum color_format,GLenum data_type) :
    QOpenGLExtraFunctions(QOpenGLContext::currentContext()),
    _color_format(color_format),
    _internal_format(color_format),
    _data_type(data_type)
{
    image = img;
    width = w;
    height = h;

    //addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    //addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

_Texture::_Texture(char *img,unsigned int w,unsigned int h, GLenum color_format,GLenum data_type,GLenum internal_format) :
    QOpenGLExtraFunctions(QOpenGLContext::currentContext()),
    _color_format(color_format),
    _internal_format(internal_format),
    _data_type(data_type)
{
    image = img;
    width = w;
    height = h;

    //addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    //addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

_Texture::~_Texture()
{
    //delete image;
}


/* Texture Class constructor
 * initialize texture from QImage
 * Created: 21_02_2019
 * sets defualt parameters
*/
_Texture::_Texture(QImage& img) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    image = (char*)img.bits();

    width = img.width();
    height = img.height();

//    switch (img.) {
//    case value:

//        break;
//    default:
//        break;
//    }

//    addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
//    addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

/* Updates texture image from image file path
 * Created: 28_03_2019
*/
void _Texture::setImage(QString pathtoTexture)
{
    QImage img = QImage(pathtoTexture);

    image = (char*)img.bits();
    width = img.width();
    height = img.height();
    //qDebug() << "setting image" << GL_RED << color_format;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _color_format, width, height, 0, _color_format, GL_UNSIGNED_BYTE, image);

}


/* Updates texture from QImage
 * Created: 21_02_2019
*/
void _Texture::setImage(QImage& img)
{
    image = (char*)img.bits();
    width = img.width();
    height = img.height();
    //qDebug() << "setting image" << GL_RED << color_format;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _color_format, width, height, 0, _color_format, GL_UNSIGNED_BYTE, image);
}

/* Updates texture from char pointer array and resolution of last image
 * Created: 21_02_2019
*/
void _Texture::setImage(char* img)
{
    image = img;
    //qDebug() << "setting image" << GL_RED << color_format;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _color_format, width, height, 0, _color_format, GL_UNSIGNED_BYTE, image);

}

/* Updates texture from char pointer array with updated given resolution
 * Created: 21_02_2019
*/
void _Texture::setImage(char* img,unsigned int iwidth,unsigned int iheight)
{
    width = iwidth;
    height = iheight;
    setImage(img);
}

void _Texture::setImage(char *img, unsigned int iwidth, unsigned int iheight, GLenum internal_format, GLenum format,GLenum data_type)
{
    image = img;

    //qDebug() << "setting image" << GL_RED << color_format;
    _color_format = format;
    _internal_format = internal_format;
    _data_type = data_type;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, width, height, 0, _color_format, _data_type, image);
}

/* bind texture to default slot(0)
 * Created: 21_02_2019
*/
void _Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
}

/* bind texture default given slot index
 * Created: 21_02_2019
*/
void _Texture::bind(unsigned int index)
{
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,m_ID);

}

/* bind texture for compute shader operation
 *   index : binding index integer specified in shader
 *   format : data format eg. RGBA8,R8
 *   access : data access eg. GL_READ_ONLY,GL_WRITE_ONLY,GL_READ_WRITE,
 * Created: 21_02_2019
*/
void _Texture::bindForCompute(unsigned int index, GLenum format, GLenum access)
{
    glBindImageTexture(index, m_ID, 0, GL_FALSE, 0, access,format );
}

/* bind texture for framebuffer target
 *   index : attatchment index integer
 *   operation : framebuffer operation eg. GL_FRAMEBUFFER,GL_READ_FRAMEBUFFER,GL_DRAW_FRAMEBUFFER,
 * Created: 21_02_2019
*/
void _Texture::bindForFramebuffer(unsigned int index, GLenum operation)
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glFramebufferTexture2D(operation,GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, m_ID, 0);
}

/* unbinds the default
 * Created: 21_02_2019
*/
void _Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D,0);
}

/* add texture parameters manually
 * these parameters will be applied in load function
 * Created: 21_02_2019
*/
void _Texture::addParameter(unsigned int pname, unsigned int param)
{
    parameters[pname] = param;
}

void _Texture::load()
{
    if(m_ID==0){
        glGenTextures(1,&m_ID);
    }

    bind();

    for (auto const& parameter : parameters)
    {
        glTexParameteri(GL_TEXTURE_2D,parameter.first, parameter.second);//second specifies value at key in map(dictionary)
    }
        //qDebug() << "setting image in load" << GL_RED << color_format;
        glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, width, height, 0, _color_format, _data_type, image);

        //glGenerateMipmap(GL_TEXTURE_2D);
}

/* initializes texture
 * applys parameters
 * loads the image
 * Created: 21_02_2019
*/
void _Texture::load(GLenum format, GLenum datatype)
{
    if(m_ID==0){
        glGenTextures(1,&m_ID);
    }

    bind();

    for (auto const& parameter : parameters)
    {
        glTexParameteri(GL_TEXTURE_2D,parameter.first, parameter.second);//second specifies value at key in map(dictionary)
    }
        _color_format = format;
        _internal_format = format;
        _data_type = datatype;
        //qDebug() << "setting image in load" << GL_RED << color_format;
        glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, width, height, 0, _color_format, _data_type, image);

        //glGenerateMipmap(GL_TEXTURE_2D);
}

/* initializes texture
 * applys parameters
 * loads the image
 * Created: 21_02_2019
*/
void _Texture::load(GLenum internal_format, GLenum format, GLenum datatype)
{
    //qDebug() << "tex load";

    if(m_ID==0){
        glGenTextures(1,&m_ID);
    }

    bind();

    for (auto const& parameter : parameters)
    {
        glTexParameteri(GL_TEXTURE_2D,parameter.first, parameter.second);//second specifies value at key in map(dictionary)
    }
    _color_format = format;
    _internal_format = internal_format;
    _data_type = datatype;
    glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, width, height, 0, _color_format, _data_type, image);
    //glGenerateMipmap(GL_TEXTURE_2D);

}

//return width of loaded texture image
unsigned int _Texture::getWidth() const
{
    return width;
}

//return height of loaded texture image
unsigned int _Texture::getHeight() const
{
    return height;
}

unsigned int _Texture::getColorformat() const
{
    return _color_format;
}

unsigned int _Texture::getDataType() const
{
    return _data_type;
}

bool _Texture::isUsed() const
{
    return is_used;
}

void _Texture::setForUse(bool value)
{
    is_used = value;
}
void _Texture::release()
{
    is_used = false;
}
