#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;

int main( )
{
	Mat homo(3, 3, CV_32FC1);
    //==============================Left===================

    Point2f srcTri_left[30];
	Point2f dstTri_left[30];
    Mat left, warpPerspective_left;
    left = imread("0.jpg", IMREAD_COLOR);
 
	/// Set the dst image the same type and size as src
	warpPerspective_left = Mat::zeros(left.rows, left.cols, left.type());
    srcTri_left[0] = Point2f(1325, 509);
	srcTri_left[1] = Point2f(1452, 626);
	srcTri_left[2] = Point2f(1331, 624);
	srcTri_left[3] = Point2f(1232, 503);

	srcTri_left[4] = Point2f(297, 443);
	srcTri_left[5] = Point2f(243, 461);
	srcTri_left[6] = Point2f(1071, 529);
	srcTri_left[7] = Point2f(868, 519);

	srcTri_left[8] = Point2f(1065, 508);
	srcTri_left[9] = Point2f(1039, 507);
	srcTri_left[10] = Point2f(164, 457);
	srcTri_left[11] = Point2f(18, 556);
	srcTri_left[12] = Point2f(1016, 395);
	srcTri_left[13] = Point2f(988, 526);
	srcTri_left[14] = Point2f(988, 503);
	srcTri_left[15] = Point2f(1014, 504);
	srcTri_left[16] = Point2f(960, 524);
	srcTri_left[17] = Point2f(961, 501);
	srcTri_left[18] = Point2f(1328, 594);
	srcTri_left[19] = Point2f(1379, 595);
	srcTri_left[20] = Point2f(1309, 524);
	srcTri_left[21] = Point2f(1267, 521);
	srcTri_left[22] = Point2f(1345, 525);
	srcTri_left[23] = Point2f(1248, 520);

	srcTri_left[24] = Point2f(897, 428);
	srcTri_left[25] = Point2f(917, 444);
	srcTri_left[26] = Point2f(938, 462);
	srcTri_left[27] = Point2f(961, 481);
	srcTri_left[28] = Point2f(987, 483);
	srcTri_left[29] = Point2f(1033, 469);

	
    // =============origin==============
	dstTri_left[0] = Point2f(0, 0);
	dstTri_left[1] = Point2f(100, 0);
	dstTri_left[2] = Point2f(100, 100);
	dstTri_left[3] = Point2f(0, 100);
	dstTri_left[4] = Point2f(0, 750);
	dstTri_left[5] = Point2f(20, 750);
	dstTri_left[6] = Point2f(40, 255);
	dstTri_left[7] = Point2f(40, 355);
	dstTri_left[8] = Point2f(20, 255);
	dstTri_left[9] = Point2f(20, 275);
	dstTri_left[10] = Point2f(20, 790);
	dstTri_left[11] = Point2f(80, 750);
	dstTri_left[12] = Point2f(40, 395);
	dstTri_left[13] = Point2f(40, 415);
	dstTri_left[14] = Point2f(20, 415);
	dstTri_left[15] = Point2f(20, 395);
	dstTri_left[16] = Point2f(40, 435);
	dstTri_left[17] = Point2f(20, 435);
	dstTri_left[18] = Point2f(80, 80);
	dstTri_left[19] = Point2f(80, 40);
	dstTri_left[20] = Point2f(20, 40);
	dstTri_left[21] = Point2f(20, 80);
	dstTri_left[22] = Point2f(20, 0);
	dstTri_left[23] = Point2f(20, 100);

	dstTri_left[24] = Point2f(-60, 395);
	dstTri_left[25] = Point2f(-40, 375);
	dstTri_left[26] = Point2f(-20, 355);
	dstTri_left[27] = Point2f(0, 335);
	dstTri_left[28] = Point2f(0, 315);
	dstTri_left[29] = Point2f(20, 100);

	// =============x add 2000, y add 1000==============
	dstTri_left[0] = Point2f(200, 100);
	dstTri_left[1] = Point2f(300, 100);
	dstTri_left[2] = Point2f(300, 200);
	dstTri_left[3] = Point2f(200, 200);
	dstTri_left[4] = Point2f(200, 850);
	dstTri_left[5] = Point2f(220, 850);
	dstTri_left[6] = Point2f(240, 355);
	dstTri_left[7] = Point2f(240, 455);
	dstTri_left[8] = Point2f(220, 355);
	dstTri_left[9] = Point2f(220, 375);
	dstTri_left[10] = Point2f(220, 890);
	dstTri_left[11] = Point2f(280, 850);
	dstTri_left[12] = Point2f(240, 495);
	dstTri_left[13] = Point2f(240, 515);
	dstTri_left[14] = Point2f(220, 515);
	dstTri_left[15] = Point2f(220, 495);
	dstTri_left[16] = Point2f(240, 535);
	dstTri_left[17] = Point2f(220, 535);
	dstTri_left[18] = Point2f(280, 180);
	dstTri_left[19] = Point2f(280, 140);
	dstTri_left[20] = Point2f(220, 140);
	dstTri_left[21] = Point2f(220, 180);
	dstTri_left[22] = Point2f(220, 100);
	dstTri_left[23] = Point2f(220, 200);


	dstTri_left[24] = Point2f(140, 495);
	dstTri_left[25] = Point2f(160, 475);
	dstTri_left[26] = Point2f(180, 455);
	dstTri_left[27] = Point2f(200, 435);
	dstTri_left[28] = Point2f(200, 415);
	dstTri_left[29] = Point2f(220, 200);



	vector<Point2f> imgPoint_left, worldPoint_left;
    
    for (int i = 0; i< 30; i++){
        imgPoint_left.push_back(srcTri_left[i]);
        worldPoint_left.push_back(dstTri_left[i]);
    }
   
	// 计算12个二维点对之间的仿射变换矩阵（2行x3列）
	
	homo = findHomography(imgPoint_left, worldPoint_left, CV_RANSAC);
	cout << "left" << homo << endl;
    warpPerspective(left, warpPerspective_left, homo, Size(380, 1100));
    namedWindow("left_src", WINDOW_NORMAL);
    imshow("left_src", warpPerspective_left);
	imwrite("left_out.jpg", warpPerspective_left);
    waitKey(0);
    
    return 0;
}

