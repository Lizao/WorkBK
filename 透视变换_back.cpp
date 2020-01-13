#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;

int main( )
{
	
	Mat homo(3, 3, CV_32FC1);
    //=============================Back=============================
    Point2f srcTri_back[24];
	Point2f dstTri_back[24];
    Mat back, warpPerspective_back;
    back = imread("3.jpg", IMREAD_COLOR);
 
	/// Set the dst image the same type and size as src
	warpPerspective_back = Mat::zeros(back.rows, back.cols, back.type());
    srcTri_back[0] = Point2f(1393, 622);
	srcTri_back[1] = Point2f(1356, 619);
	srcTri_back[2] = Point2f(1290, 575);
	srcTri_back[3] = Point2f(1262, 573);

	srcTri_back[4] = Point2f(1208, 611);
	srcTri_back[5] = Point2f(1241, 613);
	srcTri_back[6] = Point2f(1159, 571);
	srcTri_back[7] = Point2f(1208, 538);

	srcTri_back[8] = Point2f(1120, 538);
	srcTri_back[9] = Point2f(1086, 536);
	srcTri_back[10] = Point2f(976, 534);
	srcTri_back[11] = Point2f(1142, 608);

	srcTri_back[12] = Point2f(939, 598);
	srcTri_back[13] = Point2f(889, 595);
	srcTri_back[14] = Point2f(866, 594);
	srcTri_back[15] = Point2f(795, 590);

	srcTri_back[16] = Point2f(775, 590);
	srcTri_back[17] = Point2f(878, 533);
	srcTri_back[18] = Point2f(946, 535);
	srcTri_back[19] = Point2f(1021, 581);
	srcTri_back[20] = Point2f(901, 562);
	srcTri_back[21] = Point2f(925, 541);
	srcTri_back[22] = Point2f(847, 560);
	srcTri_back[23] = Point2f(829, 560);
	
	
 //    // =============origin==============
	// dstTri_back[0] = Point2f(0, 0);
	// dstTri_back[1] = Point2f(20, 0);
	// dstTri_back[2] = Point2f(0, 40);
	// dstTri_back[3] = Point2f(20, 40);
	// dstTri_back[4] = Point2f(100, 0);
	// dstTri_back[5] = Point2f(80, 0);
	// dstTri_back[6] = Point2f(100, 40);
	// dstTri_back[7] = Point2f(0, 100);
	// dstTri_back[8] = Point2f(100, 100);
	// dstTri_back[9] = Point2f(140, 100);
	// dstTri_back[10] = Point2f(280, 100);
	// dstTri_back[11] = Point2f(140, 0);
	// dstTri_back[12] = Point2f(280, 0);
	// dstTri_back[13] = Point2f(320,0);
	// dstTri_back[14] = Point2f(340, 0);
	// dstTri_back[15] = Point2f(400, 0);
	// dstTri_back[16] = Point2f(420, 0);
	// dstTri_back[17] = Point2f(420, 100);
	// dstTri_back[18] = Point2f(320, 100);
	// dstTri_back[19] = Point2f(220, 200);
	// dstTri_back[20] = Point2f(340, 40);
	// dstTri_back[21] = Point2f(340, 80);
	// dstTri_back[22] = Point2f(400, 80);
	// dstTri_back[23] = Point2f(420, 80);
	

	// // // =============x add 2000, y add 1000==============
	// dstTri_back[0] = Point2f(200, 100);
	// dstTri_back[1] = Point2f(220, 100);
	// dstTri_back[2] = Point2f(200, 140);
	// dstTri_back[3] = Point2f(220, 140);
	// dstTri_back[4] = Point2f(300, 100);
	// dstTri_back[5] = Point2f(280, 100);
	// dstTri_back[6] = Point2f(300, 140);
	// dstTri_back[7] = Point2f(200, 200);
	// dstTri_back[8] = Point2f(300, 200);
	// dstTri_back[9] = Point2f(340, 200);
	// dstTri_back[10] = Point2f(480, 200);
	// dstTri_back[11] = Point2f(340, 100);
	// dstTri_back[12] = Point2f(480, 100);
	// dstTri_back[13] = Point2f(520, 100);
	// dstTri_back[14] = Point2f(540, 100);
	// dstTri_back[15] = Point2f(600, 100);
	// dstTri_back[16] = Point2f(620, 100);
	// dstTri_back[17] = Point2f(620, 200);
	// dstTri_back[18] = Point2f(520, 200);
	// dstTri_back[19] = Point2f(420, 300);

	// dstTri_back[20] = Point2f(540, 140);
	// dstTri_back[21] = Point2f(540, 180);
	// dstTri_back[22] = Point2f(600, 180);
	// dstTri_back[23] = Point2f(620, 180);

//==========================================
	// dstTri_back[0] = Point2f(2000, 1000);
	// dstTri_back[1] = Point2f(2200, 1000);
	// dstTri_back[2] = Point2f(2000, 1400);
	// dstTri_back[3] = Point2f(2200, 1400);
	// dstTri_back[4] = Point2f(3000, 1000);
	// dstTri_back[5] = Point2f(2800, 1000);
	// dstTri_back[6] = Point2f(3000, 1400);
	// dstTri_back[7] = Point2f(2000, 2000);
	// dstTri_back[8] = Point2f(3000, 2000);
	// dstTri_back[9] = Point2f(3400, 2000);
	// dstTri_back[10] = Point2f(5800, 2000);
	// dstTri_back[11] = Point2f(3400, 1000);
	// dstTri_back[12] = Point2f(4800, 1000);
	// dstTri_back[13] = Point2f(5200, 1000);
	// dstTri_back[14] = Point2f(5400, 1000);
	// dstTri_back[15] = Point2f(6000, 1000);
	// dstTri_back[16] = Point2f(6200, 1000);
	// dstTri_back[17] = Point2f(6200, 2000);
	// dstTri_back[18] = Point2f(5200, 2000);

	vector<Point2f> imgPoint_back, worldPoint_back;
    
    for (int i = 0; i< 24; i++){
        imgPoint_back.push_back(srcTri_back[i]);
        worldPoint_back.push_back(dstTri_back[i]);
    }
   
	// 计算12个二维点对之间的仿射变换矩阵（2行x3列）
	
	homo = findHomography(imgPoint_back, worldPoint_back, CV_RANSAC);
	cout << "back" << homo << endl;
    warpPerspective(back, warpPerspective_back, homo, Size(420, 100));
    namedWindow("back_src", WINDOW_NORMAL);
    imshow("back_src", warpPerspective_back);
	imwrite("back_out.jpg", warpPerspective_back);
    waitKey(0);

    return 0;
}
