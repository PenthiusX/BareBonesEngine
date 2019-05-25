#ifndef _TEXTUREPOOL_H
#define _TEXTUREPOOL_H

#include <_texture.h>
#include <map>

class _TexturePool
{
public:
    _TexturePool();
    _TexturePool(GLenum color_format,GLenum data_type);
    _TexturePool(GLenum color_format,GLenum data_type,GLenum internal_format);

    _Texture* textureFromPool(QString texture_name);

    std::map<QString,_Texture*> texture_pool_map;
    GLenum _color_format = GL_RGBA,_internal_format=GL_RGBA,_data_type = GL_UNSIGNED_BYTE;

};

#endif // _TEXTUREPOOL_H
