#ifndef _TEXTURE_H
#define _TEXTURE_H
#include <QString>
#include <QImage>
#include <QOpenGLExtraFunctions>
#include <map>

class _Texture : protected QOpenGLExtraFunctions
{
public:
    //constructors to set texture
    _Texture();
    _Texture(QImage& img); //texture from QImage
    _Texture(QString qrcPathtoFile); //texture from image file path
    _Texture(char *img,unsigned int w,unsigned int h, GLenum color_format,GLenum data_type,GLenum internal_format); //texture from char pointer array
    _Texture(char *img,unsigned int w,unsigned int h, GLenum color_format=GL_RGBA,GLenum data_type=GL_UNSIGNED_BYTE);
    ~_Texture();

    //update texture image
    void setImage(QString qrcPathtoFile); //from image file path
    void setImage(char* img); //from char pointer array and resolution of last image
    void setImage(char* img,unsigned int iwidth,unsigned int iheight);
    void setImage(char* img, unsigned int iwidth, unsigned int iheight, GLenum internal_format, GLenum format, GLenum data_type);
    void setImage(QImage &img);//from QImage

    //setShader used if multiple texture bindings require differant slots
    inline unsigned int setShader(unsigned int prog){ shaderProgram = prog;}

    inline unsigned int GetID(){ return m_ID;}
//    void setSlotUniformName(QString name);//used if multiple texture bindings require differant slots get slot location of the given uniform name

    void bind();//bind texture to default slot(0)
    void bind(unsigned int index);//bind texture default given slot index
    void bindForCompute(unsigned int index = 0,GLenum format = GL_RGBA8,GLenum access = GL_READ_WRITE); //bind texture for compute shader operation
    void bindForFramebuffer(unsigned int index = 0,GLenum operation = GL_FRAMEBUFFER);//bind texture for framebuffer drawing target

    void unbind();
    void addParameter(unsigned int pname,unsigned int param);//setup texture parameters

    //initialized texture loads texture image
    void load();
    void load(GLenum format, GLenum datatype);
    void load(GLenum internal_format, GLenum format, GLenum datatype); //if internal and image format is differant

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    unsigned int getColorformat() const;
    unsigned int getDataType() const;

    bool isUsed() const;
    void setForUse(bool value=true);
    void release();

    std::map<unsigned int,unsigned int> parameters;
    char* image;

    unsigned int m_ID = 0;
    unsigned int shaderProgram;
    unsigned int _color_format = GL_RGBA,_internal_format=GL_RGBA,_data_type = GL_UNSIGNED_BYTE;
    unsigned int slot;
    unsigned int slot_uniform;
    unsigned int width;
    unsigned int height;
    bool is_used = false;//for compute pool if this texture is currently being used by any compute shader operation
};

#endif // _TEXTURE_H
