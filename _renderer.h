#ifndef _RENDERER_H
#define _RENDERER_H
#include <QOpenGLExtraFunctions>

/*
 * The Renderer class
 * To create an abstraction for randering data
 * Extends the _glwidget class, will be included in the _glwidget class
*/
class _Renderer : protected QOpenGLExtraFunctions
{
public:
    _Renderer();
    ~_Renderer();

    void setShader(char* vpath, char* gpath);//takes a string literal and passes
    void setBuffers(std::vector<float>vertexArray,std::vector<int> indexArray);//take vertex and index data and binds it to object buffer
    void setTexture(char* texBitmap);//takes am image and binds it to object
private:


};

#endif // _RENDERER_H
