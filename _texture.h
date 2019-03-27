#ifndef _TEXTURE_H
#define _TEXTURE_H
#include <QString>
#include <QImage>
#include <QOpenGLExtraFunctions>
#include <map>

class _Texture : private QOpenGLExtraFunctions
{
public:
    _Texture();
    _Texture(unsigned int  typ);
    _Texture(QImage& img); //texture from file
    _Texture(char *img,unsigned int w, unsigned int h,unsigned int colorFormat = GL_RGBA);

    void setImage(QString qrcPathtoFile);
    void setImage(char* img);
    void setImage(char* img,unsigned int iwidth,unsigned int iheight);
    inline unsigned int setShader(unsigned int prog){ shaderProgram = prog; }
    inline unsigned int GetID(){ return m_ID;}
    void setSlotUniformName(QString name);//(self, slot=0, shader=None):

    void bind();
    void bind(unsigned int index);
    void bindForCompute(unsigned int index = 0,GLenum format = GL_RGBA8,GLenum access = GL_READ_WRITE);
    void bindForFramebuffer(unsigned int index = 0,GLenum operation = GL_FRAMEBUFFER);

    void unbind();
    void addParameter(unsigned int pname,unsigned int param);
    void load(GLenum format = GL_RGBA, GLenum datatype = GL_UNSIGNED_BYTE);

protected:
    std::map<unsigned int,unsigned int> parameters;
    char* image;
    bool updated=true;

    unsigned int m_ID = 0;
    unsigned int shaderProgram;
    unsigned int color_format = GL_RGBA;
    unsigned int slot;
    unsigned int slot_uniform;
    unsigned int width = 1360;
    unsigned int height = 1024;
    QString slot_uniform_name;
};

#endif // _TEXTURE_H
