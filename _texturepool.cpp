#include "_texturepool.h"

_TexturePool::_TexturePool()
{

}

_TexturePool::_TexturePool(GLenum color_format, GLenum data_type, GLenum internal_format) : _color_format(color_format),
_internal_format(color_format),
_data_type(data_type)
{

}

_TexturePool::_TexturePool(GLenum color_format, GLenum data_type, GLenum internal_format) : _color_format(color_format),
_internal_format(internal_format),
_data_type(data_type)

{

}
