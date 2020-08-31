#ifndef _TEXTURE_H
#define _TEXTURE_H
#include <QString>
#include <QImage>
#include <QOpenGLExtraFunctions>
#include <map>
#include "_shader.h"

class _Texture : private QOpenGLExtraFunctions
{
public:

    enum Type{
       Diffuse,
       Specular,
       Bump
    };

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

    inline uint GetID(){ return m_ID;}

    void bind();//bind texture to default slot(0)
    void bind(uint index);//bind texture default given slot index

    void unbind();

    //initialized texture loads texture image
    void load(GLenum format = GL_RGBA, GLenum datatype = GL_UNSIGNED_BYTE);
    void load(QImage image,Type t,_Shader* s, GLenum format = GL_RGBA, GLenum datatype = GL_UNSIGNED_BYTE);

    uint getWidth() const;
    uint getHeight() const;

protected:
    char* image;

    uint m_ID = 0;
    uint color_format = GL_RGBA;
    uint slot;
    uint slot_uniform;
    uint width;
    uint height;
};

#endif // _TEXTURE_H
