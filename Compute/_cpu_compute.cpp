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
    cv::Mat texture_thres,texture_edge(input_img.rows,input_img.cols,CV_32F);

    if(init)
    {
        for (int i = 0; i < 100; ++i) {

            texture_cyl_voxels[i]=255;
        }
        init = false;
    }

    threshold(input_img, texture_thres, 100, 255,cv::THRESH_BINARY_INV );

    cvtColor(texture_thres, output_img, cv::COLOR_GRAY2RGBA);

    computeRowWiseLeftEdge(texture_thres,texture_edge);
    showImageInterval(texture_edge);
    //qDebug() <<"type" << texture_edge.type() << texture_thres.type() << input_img.type();

    //showImageInterval(texture_in);

}

void _Cpu_Compute::computeRowWiseLeftEdge(cv::Mat& input_img,cv::Mat& output_img)
{
    cv::Mat kernal = cv::Mat(1,2,CV_32F),input_img_signed,diff_img;
    kernal.at<float>(0,0)=-1.0;
    kernal.at<float>(0,1)=1.0;
    //kernal = cv::Mat::ones( 3, 3, CV_32F )/ (float)(3*3);

    input_img.convertTo(input_img_signed,CV_32F);
    cv::Point anchor;
    double delta;
    int ddepth;
    anchor = cv::Point( -1, -1 );
    delta = 0;
    ddepth = -1;
    filter2D(input_img_signed, diff_img, ddepth , kernal, anchor, delta, cv::BORDER_DEFAULT );
    cv::Mat edge(input_img.rows,2,CV_32S),img_row;

    cv::Point min_loc, max_loc;
    double min,max;
    for (int i=0;i<input_img.rows;i++) {
        img_row = diff_img.rowRange(i, i + 1);
        cv::minMaxLoc(img_row, &min, &max, &min_loc, &max_loc);
        edge.at<int>(i,0) = int(max_loc.x);
        edge.at<int>(i,1) = int(min_loc.x);
        output_img.at<float>(i,max_loc.x) = 256;
        output_img.at<float>(i,min_loc.x) = 256;
    }
}

char *_Cpu_Compute::frameGray2RGB(char *img, unsigned int iwidth, unsigned int iheight)
{
    _frameOriginal = cv::Mat(iheight, iwidth, CV_8UC1, img);

    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_GRAY2RGBA);

    return (char*)_frameProcessed.data;
}
