#include "_text.h"

_Text::_Text(){
}
_Text::~_Text(){
}
void _Text::setPainterWidgetContext(){
}
void _Text::onResize(int w,int h){
    width = w;
    height = h;

    m_window_normalised_matrix.setToIdentity();
    m_window_normalised_matrix.translate(w / 2.0, h / 2.0);
    m_window_normalised_matrix.scale(w / 2.0, -h / 2.0);

    m_window_painter_matrix.setToIdentity();
    m_window_painter_matrix.translate(w / 2.0, h / 2.0);

    m_text_layout.setTextWidth(std::max(w * 0.2, 80.0));

    m_projection.setToIdentity();
    m_projection.perspective(45.f, qreal(w) / qreal(h), 0.1f, 100.f);
}

void _Text::render(_GLWidget *q , float fps, QString Name, glm::vec3 pos, glm::vec3 rotation){

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    QString f1 = QString::number(fps);
    QString f2 = "FPS:";
    QStaticText fo(f2 + f1);
    //
    f1 = QString::number(pos.x);
    QString f11= QString::number(pos.y);
    QString f12 = QString::number(pos.z);
    f2 = "Pos:" + f1 + f11 + f12;
    QStaticText a(f2);
    //
    f1 = Name;
    f2 = "Name:" + f1;
    QStaticText b(f2);

    QStaticText c("Id: ");
    QStaticText d("Rot: ");

    QPainter p(q);
    p.begin(q);

    p.setWorldTransform(m_window_normalised_matrix.toTransform());
    //QMatrix4x4 mvp = m_projection * m_view * m_model_triangle;

    QTransform text_transform = (m_window_painter_matrix * m_view * m_model_text).toTransform();
    p.setTransform(text_transform, false);
    m_text_layout.prepare(text_transform);
    //Set the location of the text
    qreal x = -(width * 0.5) + 10;
    qreal y = -(height * 0.5) + 20 ;
    //
    p.setPen(Qt::red);
    p.setFont(QFont("Courier", 12));
    //
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //
    uint ofset= 2;
    m_text_layout = fo;
    p.drawStaticText(x, y - (10.0f  * ofset) , m_text_layout);
    m_text_layout = c;
    p.drawStaticText(x, y, c);
    m_text_layout = b;
    p.drawStaticText(x, y + (10.0f * ofset), m_text_layout);
    m_text_layout = a;
    p.drawStaticText(x, y + (20.0f * ofset), m_text_layout);
    m_text_layout = d;
    p.drawStaticText(x, y + (30.0f * ofset), m_text_layout);
    p.end();

    //m_model_text.rotate(2, 0, 2, 0);
    //m_model_text.translate(4.0,0.0,0.0);
}

/*
 * Created: 28_06_2019
*/
void _Text::createFont(_GLWidget *q)
{
    QFont _f;
    _f.setBold(true);
    // so first we grab the font metric of the font being used
    QFontMetrics metric(_f);
    // this allows us to get the height which should be the same for all
    // fonts of the same class as this is the total glyph height
    float fontHeight = metric.height();

    // loop for all basic keyboard chars we will use space to ~
    // should really change this to unicode at some stage
    const static char startChar=' ';
    const static char endChar='~';
    // Most OpenGL cards need textures to be in powers of 2 (128x512 1024X1024 etc etc) so
    // to be safe we will conform to this and calculate the nearest power of 2 for the glyph height
    // we will do the same for each width of the font below
    int heightPow2 = nearestPowerOfTwo(fontHeight);

    // we are now going to create a texture / billboard for each font
    // they will be the same height but will possibly have different widths
    for(char c=startChar; c<=endChar; ++c)
    {
        QChar ch(c);
        FontChar fc;
        // get the width of the font and calculate the ^2 size
        int width=metric.width(c);
        int widthPow2=nearestPowerOfTwo(width);
        // now we set the texture co-ords for our quad it is a simple
        // triangle billboard with tex-cords as shown
        //  s0/t0  ---- s1,t0
        //         |\ |
        //         | \|
        //  s0,t1  ---- s1,t1
        // each quad will have the same s0 and the range s0-s1 == 0.0 -> 1.0
        float s0=0.0;
        // we now need to scale the tex cord to it ranges from 0-1 based on the coverage
        // of the glyph and not the power of 2 texture size. This will ensure that kerns
        // / ligatures match
        float s1=width*1.0/widthPow2;
        // t0 will always be the same
        float t0=0.0;
        // this will scale the height so we only get coverage of the glyph as above
        float t1=metric.height()*-1.0/heightPow2;
        // we need to store the font width for later drawing
        fc.width=width;
        // now we will create a QImage to store the texture, basically we are going to draw
        // into the qimage then save this in OpenGL format and load as a texture.
        // This is relativly quick but should be done as early as possible for max performance when drawing
        QImage finalImage(nearestPowerOfTwo(width),nearestPowerOfTwo(fontHeight),QImage::Format_ARGB32);
        // set the background for transparent so we can avoid any areas which don't have text in them
        finalImage.fill(Qt::transparent);
        // we now use the QPainter class to draw into the image and create our billboards
        QPainter painter;
        painter.begin(&finalImage);
        // try and use high quality text rendering (works well on the mac not as good on linux)
        painter.setRenderHints(QPainter::HighQualityAntialiasing|QPainter::TextAntialiasing);
        // set the font to draw with
        painter.setFont(_f);
        // we set the glyph to be drawn in black the shader will override the actual colour later
        // see TextShader.h in src/shaders/
        painter.setPen(Qt::black);
        // finally we draw the text to the Image
        painter.drawText(0, metric.ascent(), QString(c));
        painter.end();
    }
}
/*
 * Created: 28_06_2019
*/
int _Text::nearestPowerOfTwo(uint n){
    unsigned count = 0;
    // First n in the below condition
    // is for the case where n is 0
    if(n && !(n & (n - 1)))
        return n;

    while(n != 0){
        n >>= 1;
        count += 1;}

    return 1 << count;
}
