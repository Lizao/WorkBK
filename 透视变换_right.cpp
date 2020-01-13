#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;

int main( )
{
	Mat homo(3, 3, CV_32FC1);
    //==============================Right===================
    Point2f srcTri_right[21];
	Point2f dstTri_right[21];
    Mat right, warpPerspective_right;
    right = imread("2.jpg", IMREAD_COLOR);
 
	/// Set the dst image the same type and size as src
	warpPerspective_right = Mat::zeros(right.rows, right.cols, right.type());

	srcTri_right[0] = Point2f(519, 660);
	srcTri_right[1] = Point2f(626, 661);
	srcTri_right[2] = Point2f(731, 568);
	srcTri_right[3] = Point2f(660, 569);
	srcTri_right[4] = Point2f(856, 594);
	srcTri_right[5] = Point2f(1008, 591);
	srcTri_right[6] = Point2f(986, 529);
	srcTri_right[7] = Point2f(880, 532);
	srcTri_right[8] = Point2f(1410, 548);
	srcTri_right[9] = Point2f(1513, 544);
	srcTri_right[10] = Point2f(1448, 564);

	srcTri_right[11] = Point2f(669, 581);
	srcTri_right[12] = Point2f(636, 637);
	srcTri_right[13] = Point2f(876, 594);
	srcTri_right[14] = Point2f(881, 577);
	srcTri_right[15] = Point2f(901, 577);
	srcTri_right[16] = Point2f(904, 563);
	srcTri_right[17] = Point2f(906, 551);
	srcTri_right[18] = Point2f(889, 551);
	srcTri_right[19] = Point2f(892, 540);
	srcTri_right[20] = Point2f(908, 540);
	
    // =============origin==============
	dstTri_right[0] = Point2f(0, 0);
	dstTri_right[1] = Point2f(0, 100);
	dstTri_right[2] = Point2f(100, 100);
	dstTri_right[3] = Point2f(100, 0);
	dstTri_right[4] = Point2f(60, 255);
	dstTri_right[5] = Point2f(60, 395);
	dstTri_right[6] = Point2f(160, 395);
	dstTri_right[7] = Point2f(160, 255);
	dstTri_right[8] = Point2f(90, 750);
	dstTri_right[9] = Point2f(90, 830);
	dstTri_right[10] = Point2f(70, 750);


	dstTri_right[11] = Point2f(80, 40);
	dstTri_right[12] = Point2f(20, 80);
	dstTri_right[13] = Point2f(60, 275);
	dstTri_right[14] = Point2f(80, 275);
	dstTri_right[15] = Point2f(80, 295);
	dstTri_right[16] = Point2f(100, 295);
	dstTri_right[17] = Point2f(120, 295);
	dstTri_right[18] = Point2f(120, 275);
	dstTri_right[19] = Point2f(140, 275);
	dstTri_right[20] = Point2f(140, 295);

	// =============x add 1000, y add 1000==============
	dstTri_right[0] = Point2f(100, 100);
	dstTri_right[1] = Point2f(100, 200);
	dstTri_right[2] = Point2f(200, 200);
	dstTri_right[3] = Point2f(200, 100);

	dstTri_right[4] = Point2f(160, 355);
	dstTri_right[5] = Point2f(160, 495);
	dstTri_right[6] = Point2f(260, 495);
	dstTri_right[7] = Point2f(260, 355);

	dstTri_right[8] = Point2f(190, 850);
	dstTri_right[9] = Point2f(190, 930);
	dstTri_right[10] = Point2f(170, 850);

	dstTri_right[11] = Point2f(180, 140);
	dstTri_right[12] = Point2f(120, 180);
	dstTri_right[13] = Point2f(160, 375);
	dstTri_right[14] = Point2f(180, 375);
	dstTri_right[15] = Point2f(180, 395);
	dstTri_right[16] = Point2f(200, 395);
	dstTri_right[17] = Point2f(220, 395);
	dstTri_right[18] = Point2f(220, 375);
	dstTri_right[19] = Point2f(240, 375);
	dstTri_right[20] = Point2f(240, 395);

	vector<Point2f> imgPoint_right, worldPoint_right;
    
    for (int i = 0; i< 21; i++){
        imgPoint_right.push_back(srcTri_right[i]);
        worldPoint_right.push_back(dstTri_right[i]);
    }
   
	// 计算12个二维点对之间的仿射变换矩阵（2行x3列）
	
	homo = findHomography(imgPoint_right, worldPoint_right, CV_RANSAC);
	cout << "right" << homo << endl;
    warpPerspective(right, warpPerspective_right, homo, Size(400, 1100));
    namedWindow("right_src", WINDOW_NORMAL);
    imshow("right_src", warpPerspective_right);
	imwrite("right_out.jpg", warpPerspective_right);

    waitKey(0);
    return 0;
}