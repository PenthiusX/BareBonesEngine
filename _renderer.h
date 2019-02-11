#ifndef _RENDERER_H
#define _RENDERER_H
#include <vector>
#include <qopenglextrafunctions.h>
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

    void init(float a);
    void setShader(char* vpath, char* gpath);//takes a string literal and passes
    void setBuffers(std::vector<float>vertexArray,std::vector<int> indexArray);//take vertex and index data and binds it to object buffer
    void draw();//takes am image and binds it to object
protected:
//void initializeGL() Q_DECL_OVERRIDE;
//void resizeGL(int w, int h)Q_DECL_OVERRIDE;
//void paintGL()Q_DECL_OVERRIDE;

private:
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
unsigned int shaderProgram;


bool move;
};

#endif // _RENDERER_H
