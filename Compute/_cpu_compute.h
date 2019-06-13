#ifndef _CPU_COMPUTE_H
#define _CPU_COMPUTE_H
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <QObject>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <QImage>

class _Cpu_Compute : public QObject
{
    Q_OBJECT
public:
    _Cpu_Compute(QObject *parent = nullptr);
    static std::vector<int> compute_k_means(std::array<unsigned int, 256> arr);

    void genImg();
    char *frameGray2RGB(char *img, unsigned int iwidth, unsigned int iheight);
    void showImageStill(cv::Mat img);
    void showImageInterval(cv::Mat img, int interval=10);
    void computeVoxelsModel(cv::Mat &input_img, cv::Mat &output_img, cv::Mat &texture_model_wrap, int rotation_step, glm::vec2 stage_center);
    void computeRowWiseLeftEdge(cv::Mat &input_img, cv::Mat &output_img);
signals:
    void imageOut(QImage img);
protected:
    cv::Mat _frameOriginal;
    cv::Mat _frameProcessed;
};

#endif // _CPU_COMPUTE_H
