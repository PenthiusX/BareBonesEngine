#include "_texture.h"
#include <QDebug>




/* Texture Class currently working for single texture per shader
 * documentation and slots functions will be completed later
 *
 * */
_Texture::_Texture() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}
_Texture::_Texture(QString texfile) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}

_Texture::_Texture(char *img, unsigned int w, unsigned int h,unsigned int colorFormat) : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
    image = img;
    width = w;
    height = h;

    //addParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    //addParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    addParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

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
void _Texture::setImage(char* img)
{
    image = img;
    qDebug() << "setting image" << GL_RED << color_format;
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, image);
    updated = true;
}
void _Texture::setImage(char* img,unsigned int iwidth,unsigned int iheight)
{
    width = iwidth;
    height = iheight;
    setImage(img);
}

void _Texture::set_slot_uniform_name(QString name)
{
    slot_uniform_name = name;
    slot_uniform = glGetUniformLocation(shaderProgram,slot_uniform_name.toStdString().c_str());
}

void _Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D,m_ID);

}

void _Texture::bind(unsigned int index)
{
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,m_ID);

}

void _Texture::bindForCompute(unsigned int index, GLenum format, GLenum access)
{
    glBindImageTexture(index, m_ID, 0, GL_FALSE, 0, access,format );
}

void _Texture::bindForFramebuffer(unsigned int index, GLenum operation)
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
    glFramebufferTexture2D(operation,GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, m_ID, 0);
}

void _Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D,0);
}

void _Texture::addParameter(unsigned int pname, unsigned int param)
{
    parameters[pname] = param;
}

void _Texture::load( GLenum format, GLenum datatype)
{
    qDebug() << "tex load";

    if(m_ID==0){
        qDebug() << "tex gen b" << m_ID;
        unsigned int t;
        glGenTextures(1,&m_ID);
        qDebug() << "tex gen" << m_ID;
    }

    bind();

    for (auto const& parameter : parameters)
    {
        glTexParameteri(GL_TEXTURE_2D,parameter.first, parameter.second);//second specifies value at key in map(dictionary)
    }

    if(updated)
    {
        color_format = format;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, datatype, image);
        updated = false;
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
}
