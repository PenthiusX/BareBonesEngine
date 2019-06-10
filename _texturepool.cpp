#include "_texturepool.h"

_TexturePool::_TexturePool()
{

}


_TexturePool::_TexturePool(GLenum color_format, GLenum data_type) : _color_format(color_format),
_internal_format(color_format),
_data_type(data_type)
{

}

_TexturePool::_TexturePool(GLenum color_format, GLenum data_type, GLenum internal_format) : _color_format(color_format),
_internal_format(internal_format),
_data_type(data_type)
{

}

_Texture *_TexturePool::textureFromPool(QString texture_name,unsigned int w,unsigned int h)
{
    bool present = false;
    if ( texture_pool_map.find(texture_name) == texture_pool_map.end() ) {
        //not found create or allocate texture
        for (auto& texture_pair : texture_pool_map)
        {
            //find if an unused texture is there in the pool
            if(!texture_pair.second->isUsed())
            {
                //if exists with same size then allocate it to current name
                if((texture_pair.second->getWidth()==w) && (texture_pair.second->getHeight()==h))
                {

                    auto it = texture_pool_map.find(texture_pair.first);
                    std::swap(texture_pool_map[texture_name],it->second);
                    texture_pool_map.erase(it);
                    present = true;
                    break;
                }
            }
        }
        if(!present) {
            //if not allocated then generate new texture
            texture_pool_map[texture_name] = new _Texture(nullptr,w,h,_color_format,_data_type,_internal_format);
            texture_pool_map[texture_name]->load();
        }
    }
    return texture_pool_map[texture_name];
}

_Texture *_TexturePool::operator[](QString key)
{
    return textureFromPool(key);
}

int _TexturePool::getTextureNumber() const
{
    return texture_numbers;
}

void _TexturePool::setTextureNumber(int value)
{
    texture_numbers = value;
}

unsigned int _TexturePool::getWidth() const
{
    return width;
}

void _TexturePool::setWidthHeight(unsigned int w, unsigned int h)
{
    width=w;
    height=h;
}

bool _TexturePool::doesExists(QString texture_name)
{
    return !(texture_pool_map.find(texture_name) == texture_pool_map.end());
}

_Texture *_TexturePool::textureFromPool(QString texture_name)
{
    return textureFromPool(texture_name,width,height);
}

unsigned int  _TexturePool::getHeight() const
{
    return height;
}
