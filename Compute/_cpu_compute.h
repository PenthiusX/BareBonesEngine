#ifndef _CPU_COMPUTE_H
#define _CPU_COMPUTE_H
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <QObject>
#include "opencv2/core.hpp"

class _Cpu_Compute : public QObject
{
    Q_OBJECT
public:
    _Cpu_Compute(QObject *parent = nullptr);
    static std::vector<int> compute_k_means(std::array<unsigned int, 256> arr);

    void genImg();
signals:
    void imageOut(QImage img);
protected:
    cv::Mat _frameOriginal;
    cv::Mat _frameProcessed;
};

#endif // _CPU_COMPUTE_H
