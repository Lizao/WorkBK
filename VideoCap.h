#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "opencv2/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

extern "C"
{
	#include "libavcodec/avcodec.h"  
	#include "libavformat/avformat.h"  
	#include "libavutil/channel_layout.h"  
	#include "libavutil/common.h"  
	#include "libavutil/imgutils.h"  
	#include "libswscale/swscale.h"   
	#include "libavutil/imgutils.h"      
	#include "libavutil/opt.h"         
	#include "libavutil/mathematics.h"      
	#include "libavutil/samplefmt.h" 
}

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavcodec/version.h"
	#include "libavformat/avformat.h"
	// #include "libavfilter/avfiltergraph.h"
	// #include "libavfilter/buffersink.h"
	// #include "libavfilter/buffersrc.h"
	#include "libavutil/avutil.h"
	#include "libavutil/opt.h"
	#include "libavutil/pixdesc.h"
	#include "libswscale/swscale.h"
};

using namespace std;
using namespace cv;

struct Result{
    double k;
    double b;
};

const static int imgWidth = 1920;
const static int imgHeight = 1080;
const static Size image_size = Size(imgWidth, imgHeight);

class VideoCap{
public:
	vector<string> camera_source;
	vector<int> camera_index;
	vector<VideoCapture*> camera_capture;
	//this holds queue(s) which hold images from each camera
	vector<Mat> frame_queue;
	//this holds thread(s) which run the camera capture process
	vector<thread*> camera_thread;
    
    // this cal
    void calFishMaps(int pos, Mat *mapx, Mat *mapy);
    void birdView(int num);
    void  processImg(int pos, Mat mapx, Mat mapy, int seq);
    void backUpImg(Mat frame, int index, std::string filePath);
    void img2Video();
    double avelight(Mat M);
    Mat rejustPixel(Mat &src, double scale);
    void lightCompensation(Mat *v);
    void gapSmooth(Mat *v);
    void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame, int index, int timestamp);
    void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame, int index);

	//Constructor for video capture
	VideoCap(vector<string> source);
	//Constructor for USB Camera capture
	VideoCap(vector<int> index);
	~VideoCap();

public:
	cv::Matx33d intrinsic_matrix[4];
	cv::Vec4d distortion_coeffs[4];
	cv::Matx33d transform_matrix[4], wrap_matrix[4];
	cv::Mat fishmapx[4], fishmapy[4];
	cv::Size posSize[4];
	std::string videofile[4], dirseq[5];
	std::vector<std::string> videoPath;
	Mat carMask, m, maskF, maskB;
	int maskHeigth;
	Size mSize;
	Rect r[4];
	std::vector<std::vector<Point> > maskVec;
	Rect car;
	int num;

private:
	bool isFishCamera;
	int camera_count;
	void startMultiCapture();
	void stopMultiCapture();
	// void captureFrame(int index);
	void captureFrame(int index);
	vector<string> source;
};


