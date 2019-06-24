#ifndef _TEXTURE_H
#define _TEXTURE_H
#include <QString>
#include <QImage>
#include <QOpenGLExtraFunctions>
#include <map>

class _Texture : private QOpenGLExtraFunctions
{
public:
    //constructors to set texture
    _Texture();
    _Texture(QImage& img); //texture from QImage
    _Texture(QString qrcPathtoFile); //texture from image file path
    _Texture(char *img,uint w, uint h,uint colorFormat = GL_RGBA); //texture from char pointer array

    //update texture image
    void setImage(QString qrcPathtoFile); //from image file path
    void setImage(char* img); //from char pointer array and resolution of last image
    void setImage(char* img,uint iwidth,uint iheight);//from char pointer array with updated given resolution
    void setImage(QImage &img);//from QImage

    //setShader used if multiple texture bindings require differant slots
    inline uint setShader(uint prog){ shaderProgram = prog; }

    inline uint GetID(){ return m_ID;}
    //    void setSlotUniformName(QString name);//used if multiple texture bindings require differant slots get slot location of the given uniform name

    void bind();//bind texture to default slot(0)
    void bind(uint index);//bind texture default given slot index
    void bindForCompute(uint index = 0,GLenum format = GL_RGBA8,GLenum access = GL_READ_WRITE); //bind texture for compute shader operation
    void bindForFramebuffer(uint index = 0,GLenum operation = GL_FRAMEBUFFER);//bind texture for framebuffer drawing target

    void unbind();
    void addParameter(uint pname,uint param);//setup texture parameters

    //initialized texture loads texture image
    void load(GLenum format = GL_RGBA, GLenum datatype = GL_UNSIGNED_BYTE);

    uint getWidth() const;
    uint getHeight() const;

protected:
    std::map<uint,uint> parameters;
    char* image;

    uint m_ID = 0;
    uint shaderProgram;
    uint color_format = GL_RGBA;
    uint slot;
    uint slot_uniform;
    uint width;
    uint height;
};

#endif // _TEXTURE_H
