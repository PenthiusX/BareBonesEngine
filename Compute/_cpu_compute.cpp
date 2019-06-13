#include "_cpu_compute.h"
#include <QDebug>

_Cpu_Compute::_Cpu_Compute(QObject *parent) : QObject(parent)
{

}

/*calculates 2-means for clusters in histogram*/
std::vector<int> _Cpu_Compute::compute_k_means(std::array<unsigned int, 256> arr)
{
    float factor = 0.5;
    std::vector<int> k_means={20,230},distance_from_means={0,0},sigma_distance={0,0},sigma_number={0,0};
    std::vector<float> change={0,0};
    do{
    for (int i = 0; i < 256; ++i) {
        for (int k = 0; k < k_means.size(); ++k) {
            distance_from_means[k] = glm::abs(k_means[k]-i);
        }
        if(distance_from_means[0] < distance_from_means[1])
        {
            sigma_distance[0]+=arr[i]*i;
            sigma_number[0]+=arr[i];
        }
        else {
            sigma_distance[1]+=arr[i]*i;
            sigma_number[1]+=arr[i];
        }
    }
    for (int k = 0; k < k_means.size(); ++k) {
        if(sigma_number[k]!=0) change[k]= factor*(float(sigma_distance[k]/sigma_number[k])-k_means[k]);
        else change[k] = 0;
        k_means[k]=k_means[k]+change[k];
    }
    }while((glm::abs(change[0])+glm::abs(change[1]))>0.5);

    return k_means;
}

void _Cpu_Compute::genImg()
{
    _frameOriginal = cv::imread	("lena.jpg");
    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_BGR2GRAY);
//    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
//    cv::imshow( "Display window", _frameOriginal );                   // Show our image inside it.

//    cv::waitKey(0);
    QImage output((const unsigned char *)_frameProcessed.data, _frameProcessed.cols, _frameProcessed.rows,_frameProcessed.step, QImage::Format_Indexed8);
    emit imageOut(output);
}

void _Cpu_Compute::showImageStill(cv::Mat img)
{
        cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
        cv::imshow( "Display window", img );                   // Show our image inside it.
        cv::waitKey(0);
}

void _Cpu_Compute::showImageInterval(cv::Mat img,int interval)
{
        cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
        cv::imshow( "Display window", img );                   // Show our image inside it.
        cv::waitKey(interval);
}

void _Cpu_Compute::computeVoxelsModel(cv::Mat &input_img, cv::Mat &output_img, cv::Mat &texture_out_8_bit, int rotation_step, glm::vec2 stage_center)
{
    static std::vector<cv::Mat> texture_cyl_voxels(100, cv::Mat(input_img.cols,input_img.rows, CV_8UC1));
    static bool init = true;
    cv::Mat texture_thres;

    if(init)
    {
        for (int i = 0; i < 100; ++i) {

            texture_cyl_voxels[i]=255;
        }
        init = false;
    }

    threshold(input_img, texture_thres, 100, 255,cv::THRESH_BINARY_INV );

    cvtColor(texture_thres, output_img, cv::COLOR_GRAY2RGBA);

    //showImageInterval(texture_in);

}

void _Cpu_Compute::computeRowWiseLeftEdge(cv::Mat& input_img,cv::Mat& output_img)
{

}

char *_Cpu_Compute::frameGray2RGB(char *img, unsigned int iwidth, unsigned int iheight)
{
    _frameOriginal = cv::Mat(iheight, iwidth, CV_8UC1, img);

    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_GRAY2RGBA);

    return (char*)_frameProcessed.data;
}
