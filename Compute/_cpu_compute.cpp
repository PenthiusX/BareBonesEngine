#include "_cpu_compute.h"
#include <QDebug>
#include <_tools.h>

#define PI 3.1415926535897932384626433832795

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

void _Cpu_Compute::computeVoxelsModel(cv::Mat &input_img, cv::Mat &output_img, cv::Mat &texture_model_wrap, int rotation_step, glm::vec2 stage_center)
{
    static std::vector<cv::Mat> texture_cyl_voxels(100, cv::Mat(input_img.rows,input_img.cols, CV_8UC1));
    static bool init = true;
    cv::Mat texture_thres,texture_edge(input_img.rows,2,CV_32S),texture_model_wrap_8_bit;

    if(init)
    {
        for (int i = 0; i < 100; ++i) {
            texture_cyl_voxels[i]=255;
        }
        init = false;
    }

    //
    for (int i = 0; i < 100; ++i) {

    int id = (i+rotation_step)%100;
    double cosine = glm::cos(CV_PI*i/100);
    glm::tmat3x3<double> translate1={1,0,-stage_center.x,0,1,0,0,0,1},scale={1/cosine,0,0,0,1,0,0,0,1},translate2={1,0,(input_img.cols/2),0,1,0,0,0,1},final;

    final = translate1*scale*translate2;
    cv::Mat M = cv::Mat(2, 3, CV_64FC1,&final),temp_affine;

    threshold(input_img, texture_thres, 100, 255,cv::THRESH_BINARY_INV );

    cv::warpAffine( texture_thres,temp_affine,M, cv::Size(texture_thres.cols,texture_thres.rows),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(0));// (src, dst, M, dsize, flags = cv.INTER_LINEAR, borderMode = cv.BORDER_CONSTANT, borderValue = new cv.Scalar())

    cv::min(texture_cyl_voxels[id],temp_affine,texture_cyl_voxels[id]);

    cvtColor(input_img, output_img, cv::COLOR_GRAY2RGBA);

    computeRowWiseLeftEdge(texture_thres,texture_edge);

    //showImageInterval(temp_affine);

    texture_edge.colRange(0,1).copyTo(texture_model_wrap(cv::Range(0,texture_model_wrap.rows),cv::Range(rotation_step,rotation_step+1)));
    //texture_edge.colRange(1,2).copyTo(texture_model_wrap(cv::Range(0,texture_model_wrap.rows),cv::Range(((rotation_step+100)%200),((rotation_step+100)%200)+1)));

    //texture_model_wrap.at<int>(350,rotation_step) = 200;
    texture_model_wrap.convertTo(texture_model_wrap_8_bit,CV_8UC1);

    }
    for (int i=0;i<texture_edge.rows;i++) {

        int c = (input_img.rows/2) - texture_edge.at<int>(i,0);
        qDebug() << "i" <<i <<"xgdrsf"<<c;

        output_img.at<cv::Vec4b>(i,150) = cv::Vec4b(255,0,0,255);
    }
    showImageInterval(texture_cyl_voxels[rotation_step]);


    //qDebug() <<"type" << texture_edge.type() << texture_thres.type() << input_img.type();

    //showImageInterval(texture_in);
}

void _Cpu_Compute::computeRowWiseLeftEdge(cv::Mat& input_img,cv::Mat& output_img)
{
    //size of output image should be [height , 2]
    cv::Mat kernal = cv::Mat(1,2,CV_32F),input_img_signed,diff_img,img(input_img.cols,input_img.rows, CV_8UC1);
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
    cv::Mat img_row;

    cv::Point min_loc, max_loc;
    double min,max;
    for (int i=0;i<input_img.rows;i++) {
        img_row = diff_img.rowRange(i, i + 1);
        cv::minMaxLoc(img_row, &min, &max, &min_loc, &max_loc);
        output_img.at<int>(i,0) = (input_img.rows/2)-int(max_loc.x);
        output_img.at<int>(i,1) = int(min_loc.x)-(input_img.rows/2);
        //img.at<uchar>(i,output_img.at<int>(i,0)) = 255;
    }
    //showImageInterval(img);
}



/* Function : compute_stage_angle(cv::Mat& input_img,cv::Mat& output_img)
 *
 * this function calculates stage angle and center point of stage and returns it in a vec3 format :
 * glm::vec3(center_x,center_y,theta);
 * created : 17/06/2019
 *
*/
glm::vec3 _Cpu_Compute::compute_stage_angle_gradient(cv::Mat& input_img,cv::Mat& output_img,std::vector<float> params)
{
    //create temp images for operations
    imwrite( "Gray_Image.png", input_img );
    cv::Mat edge,dilated_img,angle,angle_x,angle_y,masked_img,mask=cv::Mat::zeros(input_img.rows,input_img.cols,CV_8UC1),grad_x, grad_y,thres_grad_x, thres_grad_y,abs_grad_x,abs_grad_y;

    //preprocessing filtering
    mask(cv::Rect(16,16,input_img.cols-32,input_img.rows-32)) = 255;
//    cv::bitwise_and(input_img, input_img, masked_img, mask);

        //cv::Mat grad;
        int ksize = 3;
        double scale = 1/4.0;
        int delta = 0;
        int ddepth = CV_32FC1;

    Sobel(input_img, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
    Sobel(input_img, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

    //take magnitude of edges
    convertScaleAbs( grad_x, thres_grad_x );
    convertScaleAbs( grad_y, thres_grad_y );

    cv::bitwise_and(thres_grad_x, thres_grad_x, abs_grad_x, mask);
    cv::bitwise_and(thres_grad_y, thres_grad_y, abs_grad_y, mask);

    //seletcing verticle gradients
    threshold(abs_grad_x, thres_grad_x, 40, 255,cv::THRESH_BINARY );
    threshold(abs_grad_y, thres_grad_y, 40, 255,cv::THRESH_BINARY );

    phase(grad_x , grad_y , angle, true);

    //multiply(thres_grad_x,angle,angle_x,1.0,CV_32FC1);
    //multiply(thres_grad_y,angle,angle_y,1.0,CV_32FC1);

    cv::bitwise_and(angle, angle, angle_x, thres_grad_x);
    cv::bitwise_and(angle, angle, angle_y, thres_grad_y);

    showImageInterval(angle_y);

    //seletcing horizontal gradients

    //find canny edge
//    cv::Canny(input_img, edge, 50, 200, 3);

//    //mask to remove anomalies at boundries


//    //show masked_edge image as output
//    cvtColor(input_img, output_img, cv::COLOR_GRAY2RGBA);

//    std::vector<cv::Vec4i> lines,h_lines,v_lines;
//    std::vector<glm::vec3> line_center_and_angle;

//    cv::Mat element = getStructuringElement( cv::MORPH_ELLIPSE,cv::Size( 3, 3 ),cv::Point(1,1));

//    cv::dilate(masked_edge, dilated_img, element, cv::Point(-1, -1), 1, 1, 1);

//    showImageInterval(dilated_img);

//    HoughLinesP(dilated_img, lines, 1, CV_PI/180,50, 50, 10);
////    for( size_t i = 0; i < lines.size(); i++ )
////    {
////      cv::Vec4i l = lines[i];
//      //cv::line( output_img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255,255), 1, cv::LINE_AA);
////    }
//    float delta = 5;
//    std::vector<cv::Point2f> points,centers,h_points,v_points;
//    centers.push_back(cv::Point2f(1.0,1.0));
//    centers.push_back(cv::Point2f(1.0,1.0));

//    //categorise lines in vertical and horizontal
//    for(auto line:lines)
//    {
//        if(cv::abs(line[2]-line[0])< delta)// x-values are near; line is vertical
//        {
//            v_lines.push_back(line);
//            v_points.push_back(cv::Point2f(line[0],line[1]));
//            v_points.push_back(cv::Point2f(line[2],line[3]));
//            cv::line( output_img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0,0,255,255), 1, cv::LINE_AA);
//        }

//        if(cv::abs(line[3]-line[1])< delta)//  y-values are near; line is horizontal
//        {
//            h_lines.push_back(line);
//            h_points.push_back(cv::Point2f(line[0],line[1]));
//            h_points.push_back(cv::Point2f(line[2],line[3]));
//            cv::line( output_img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255,0,0,255), 1, cv::LINE_AA);
//        }
//    }

//    std::vector<int> io_array(points.size());

////    for(auto h_line:h_lines)
////    {
////        for(auto v_line:v_lines)
////        {
////            auto pt =find_line_intersection(h_line,v_line);
////            points.push_back(pt);
////        }
////    }
//    //cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0);
//    //cv::kmeans(v_points, 2, io_array, criteria,10,cv::KMEANS_PP_CENTERS,centers);

//    //qDebug() << centers;
//    int n = h_points.size();

//    //fit a single horizontal line with slope and intercept = a, b respectively

//    double xsum=0,x2sum=0,ysum=0,xysum=0,a,b;                //variables for sums/sigma of xi,yi,xi^2,xiyi etc
//    for (auto pt:h_points)
//    {
//        xsum=xsum+pt.x;                        //calculate sigma(xi)
//        ysum=ysum+pt.y;                        //calculate sigma(yi)
//        x2sum=x2sum+pow(pt.x,2);                //calculate sigma(x^2i)
//        xysum=xysum+pt.x*pt.y;                    //calculate sigma(xi*yi)
//    }
//    a=(n*xysum-xsum*ysum)/(n*x2sum-xsum*xsum);            //calculate slope
//    b=(x2sum*ysum-xsum*xysum)/(x2sum*n-xsum*xsum);            //calculate intercept

//    //find intesections
//    float mid_x = (centers[0].x+centers[1].x)/2.0;
//    glm::vec3 stage_center_angle(mid_x,a*mid_x+b,glm::atan(a)*180.0/CV_PI);

//    cv::circle(output_img, cv::Point2f(stage_center_angle.x,stage_center_angle.y), 3, cv::Scalar(0,255,0,255), -1);
//    return stage_center_angle;
    return glm::vec3(1.0,1.0,1.0);
}



/* Function : compute_stage_angle(cv::Mat& input_img,cv::Mat& output_img)
 *
 * this function calculates stage angle and center point of stage and returns it in a vec3 format :
 * glm::vec3(center_x,center_y,theta);
 * created : 17/06/2019
 *
*/
glm::vec3 _Cpu_Compute::compute_stage_angle(cv::Mat& input_img,cv::Mat& output_img,std::vector<float> params)
{
    //create temp images for operations
    cv::Mat edge,dilated_img,masked_edge,mask=cv::Mat::zeros(input_img.rows,input_img.cols,CV_8UC1);

    //find canny edge
    cv::Canny(input_img, edge, 50, 200, 3);

    //mask to remove anomalies at boundries
    mask(cv::Rect(16,16,input_img.cols-32,input_img.rows-32)) = 255;
    cv::bitwise_and(edge, edge, masked_edge, mask);

    //show masked_edge image as output
    cvtColor(input_img, output_img, cv::COLOR_GRAY2RGBA);

    std::vector<cv::Vec4i> lines,h_lines,v_lines;
    std::vector<glm::vec3> line_center_and_angle;

    cv::Mat element = getStructuringElement( cv::MORPH_ELLIPSE,cv::Size( 3, 3 ),cv::Point(1,1));

    cv::dilate(masked_edge, dilated_img, element, cv::Point(-1, -1), 1, 1, 1);

    showImageInterval(dilated_img);

    HoughLinesP(dilated_img, lines, 1, CV_PI/180,50, 50, 10);
//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//      cv::Vec4i l = lines[i];
      //cv::line( output_img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255,255), 1, cv::LINE_AA);
//    }
    float delta = 5;
    std::vector<cv::Point2f> points,centers,h_points,v_points;
    centers.push_back(cv::Point2f(1.0,1.0));
    centers.push_back(cv::Point2f(1.0,1.0));

    //categorise lines in vertical and horizontal
    for(auto line:lines)
    {
        if(cv::abs(line[2]-line[0])< delta)// x-values are near; line is vertical
        {
            v_lines.push_back(line);
            v_points.push_back(cv::Point2f(line[0],line[1]));
            v_points.push_back(cv::Point2f(line[2],line[3]));
            cv::line( output_img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0,0,255,255), 1, cv::LINE_AA);
        }

        if(cv::abs(line[3]-line[1])< delta)//  y-values are near; line is horizontal
        {
            h_lines.push_back(line);
            h_points.push_back(cv::Point2f(line[0],line[1]));
            h_points.push_back(cv::Point2f(line[2],line[3]));
            cv::line( output_img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255,0,0,255), 1, cv::LINE_AA);
        }
    }

    std::vector<int> io_array(points.size());

//    for(auto h_line:h_lines)
//    {
//        for(auto v_line:v_lines)
//        {
//            auto pt =find_line_intersection(h_line,v_line);
//            points.push_back(pt);
//        }
//    }
    //cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0);
    //cv::kmeans(v_points, 2, io_array, criteria,10,cv::KMEANS_PP_CENTERS,centers);

    //qDebug() << centers;
    int n = h_points.size();

    //fit a single horizontal line with slope and intercept = a, b respectively

    double xsum=0,x2sum=0,ysum=0,xysum=0,a,b;                //variables for sums/sigma of xi,yi,xi^2,xiyi etc
    for (auto pt:h_points)
    {
        xsum=xsum+pt.x;                        //calculate sigma(xi)
        ysum=ysum+pt.y;                        //calculate sigma(yi)
        x2sum=x2sum+pow(pt.x,2);                //calculate sigma(x^2i)
        xysum=xysum+pt.x*pt.y;                    //calculate sigma(xi*yi)
    }
    a=(n*xysum-xsum*ysum)/(n*x2sum-xsum*xsum);            //calculate slope
    b=(x2sum*ysum-xsum*xysum)/(x2sum*n-xsum*xsum);            //calculate intercept

    //find intesections
    float mid_x = (centers[0].x+centers[1].x)/2.0;
    glm::vec3 stage_center_angle(mid_x,a*mid_x+b,glm::atan(a)*180.0/CV_PI);

    cv::circle(output_img, cv::Point2f(stage_center_angle.x,stage_center_angle.y), 3, cv::Scalar(0,255,0,255), -1);
    return stage_center_angle;
}

cv::Point2f _Cpu_Compute::find_line_intersection(cv::Vec4i line1, cv::Vec4i line2)
{
// compute determinant

//Px = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4 - y3*x4))/((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    float x1 = line1[0],y1 = line1[1],x2 = line1[2],y2 = line1[3];
    float x3 = line2[0],y3 = line2[1],x4 = line2[2],y4 = line2[3];

    float x = (x1*((x1 - x2)*(y3 - y4) - (x3 - x4)*(y1 - y2)) - (x1 - x3)*((x1 - x2)*(y3 - y4) - (x3 - x4)*(y1 - y2)) + (x3 - x4)*((x1 - x2)*(y1 - y3) - (x1 - x3)*(y1 - y2)))/((x1 - x2)*(y3 - y4) - (x3 - x4)*(y1 - y2));
    float y = (x1*y2*y3 - x1*y2*y4 - x2*y1*y3 + x2*y1*y4 - x3*y1*y4 + x3*y2*y4 + x4*y1*y3 - x4*y2*y3)/(x1*y3 - x1*y4 - x2*y3 + x2*y4 - x3*y1 + x3*y2 + x4*y1 - x4*y2);

    return cv::Point2f(x, y);
}

char *_Cpu_Compute::frameGray2RGB(char *img, unsigned int iwidth, unsigned int iheight)
{
    _frameOriginal = cv::Mat(iheight, iwidth, CV_8UC1, img);

    cv::cvtColor(_frameOriginal, _frameProcessed, cv::COLOR_GRAY2RGBA);

    return (char*)_frameProcessed.data;
}

void _Cpu_Compute::generateModelMesh(int *wrap_frame, unsigned int iwidth, unsigned int iheight,QString filePath)
{
    std::vector<float> vertsG;
    std::vector<unsigned int> indiceG;

    unsigned int index[4] = {0,0,0,0};

    glm::ivec2 resolution = glm::ivec2(iwidth,iheight);//wrap texture size

    for (unsigned int h = 0; h < resolution.y; h++) {
        for (unsigned int w = 0; w < resolution.x; w++) {

            glm::vec2 pixel_cord = glm::vec2(w,h);

            //glm::vec2 texture_cord = glm::vec2((pixel_cord.x +0.5)/resolution.x,(pixel_cord.y +0.5)/resolution.y);

            int index = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
            //texture_positions
            float r = wrap_frame[index];
            float theta = 2 * PI * float(w) /resolution.x;


            vertsG.push_back(r*cos(theta));//x = s
            vertsG.push_back(r*sin(theta));//y = t
            vertsG.push_back(h);//z = 0.0
        }
    }

    glm::ivec2 step_size = glm::ivec2(1,1);

    glm::vec3 origin = glm::vec3(0,0,resolution.y/2);

    float volume = 0.0;
    for (unsigned int h = 0; h < resolution.y; h+=step_size.y) {
        for (unsigned int w = 0; w < resolution.x; w+=step_size.x) {

            glm::vec2 pixel_cord = glm::vec2(w,h);

            //indexes of neibhouring vertexes
            index[0] = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
            index[1] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,0),resolution);
            index[2] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,step_size.y),resolution);
            index[3] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(0,step_size.y),resolution);

//            if((pixel_cord.y < resolution.y) && (pixel_cord.y > 80))
//            {


            if((pixel_cord.y < resolution.y))
            {
                //indexs of fisrt triangle in quad
                indiceG.push_back(index[0]);
                indiceG.push_back(index[1]);
                indiceG.push_back(index[2]);

                //indexs of second triangle in quad
                indiceG.push_back(index[0]);
                indiceG.push_back(index[2]);
                indiceG.push_back(index[3]);
            }
        }
    }

    _Tools::SaveObjModel(vertsG,indiceG,filePath);

}
