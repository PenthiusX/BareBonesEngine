#pragma once

#include <QStaticText>
#include <QMatrix4x4>
#include <QPainter>
#include <QFontMetrics>
#include <QFontMetrics>

#include <glm/glm.hpp>
#include "_glwidget.h"

struct FontChar
{
    int width; /// @brief the width of the font
    uint textureID; /// @brief the texture id of the font billboard
    uint *vao; /// a vao for the font
};

class _Text
{
public:
    _Text();
    ~_Text();

    void setPainterWidgetContext(_GLWidget *q);
    void onResize(int w, int h);
    void render(_GLWidget *q , float fps, QString Name, glm::vec3 pos, glm::vec3 rotation);

    void createFont(_GLWidget *q);
    int nearestPowerOfTwo(uint n);

private:
    QPainter* painter;

    QStaticText m_text_layout;
    QMatrix4x4 m_window_normalised_matrix;
    QMatrix4x4 m_window_painter_matrix;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model_triangle;
    QMatrix4x4 m_model_text;

    uint width;
    uint height;

    bool runOnce;
};

