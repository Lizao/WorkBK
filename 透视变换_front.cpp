#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;

int main( )
{
	Point2f srcTri_front[20];
	Point2f dstTri_front[20];
	Mat homo(3, 3, CV_32FC1);

	//==========================Front=============================
	Mat front, warpPerspective_front;
    front = imread("afterDistort.jpg", IMREAD_COLOR);
 
	/// Set the dst image the same type and size as src
	warpPerspective_front = Mat::zeros(front.rows, front.cols, front.type());

    srcTri_front[0] = Point2f(507, 533);
	srcTri_front[1] = Point2f(110, 674);
	srcTri_front[2] = Point2f(724, 538);
	srcTri_front[3] = Point2f(503, 693);
	srcTri_front[4] = Point2f(1252, 557);
	srcTri_front[5] = Point2f(1505, 568);
	srcTri_front[6] = Point2f(1515, 751);
	srcTri_front[7] = Point2f(1770, 705);
	srcTri_front[8] = Point2f(818, 541);
	srcTri_front[9] = Point2f(1152, 751);
	srcTri_front[10] = Point2f(1309, 741);
	srcTri_front[11] = Point2f(666, 707);

	srcTri_front[12] = Point2f(751, 710);
	srcTri_front[13] = Point2f(932, 720);
	srcTri_front[14] = Point2f(1018, 668);
	srcTri_front[15] = Point2f(880, 619);
	srcTri_front[16] = Point2f(952, 590);
	srcTri_front[17] = Point2f(1082, 628);
	srcTri_front[18] = Point2f(1099, 672);
	srcTri_front[19] = Point2f(903, 561);


   // origin 
	dstTri_front[0] = Point2f(0, 0);
	dstTri_front[1] = Point2f(0, 100);
	dstTri_front[2] = Point2f(100, 0);
	dstTri_front[3] = Point2f(100, 100);
	dstTri_front[4] = Point2f(320, 0);

	dstTri_front[5] = Point2f(420, 0);
	dstTri_front[6] = Point2f(320, 100);
	dstTri_front[7] = Point2f(400, 80);
	dstTri_front[8] = Point2f(140, 0);
	dstTri_front[9] = Point2f(280, 0);
	dstTri_front[10] = Point2f(280, 100);
	dstTri_front[11] = Point2f(140, 100);

	dstTri_front[12] = Point2f(160, 100);
	dstTri_front[13] = Point2f(200, 100);
	dstTri_front[14] = Point2f(220, 80);
	dstTri_front[15] = Point2f(80, 60);
	dstTri_front[16] = Point2f(200, 40);
	dstTri_front[17] = Point2f(240, 60);
	dstTri_front[18] = Point2f(240, 80);
	dstTri_front[19] = Point2f(180, 20);
	
	// // // =============x add 2000, y add 1000==============
	// dstTri_front[0] = Point2f(200, 100);
	// dstTri_front[1] = Point2f(200, 200);
	// dstTri_front[2] = Point2f(300, 100);
	// dstTri_front[3] = Point2f(300, 200);
	// dstTri_front[4] = Point2f(520, 100);
	// dstTri_front[5] = Point2f(620, 100);
	// dstTri_front[6] = Point2f(520, 200);
	// dstTri_front[7] = Point2f(600, 180);
	// dstTri_front[8] = Point2f(340, 100);
	// dstTri_front[9] = Point2f(480, 100);
	// dstTri_front[10] = Point2f(480, 200);
	// dstTri_front[11] = Point2f(340, 200);

	// dstTri_front[12] = Point2f(360, 200);
	// dstTri_front[13] = Point2f(400, 200);
	// dstTri_front[14] = Point2f(420, 180);
	// dstTri_front[15] = Point2f(280, 160);
	// dstTri_front[16] = Point2f(400, 140);
	// dstTri_front[17] = Point2f(440, 160);
	// dstTri_front[18] = Point2f(440, 180);
	// dstTri_front[19] = Point2f(380, 120);

	// // // =============x add 2900, y add 3300 (base x add 200, y add 100)==============
	// dstTri_front[0] = Point2f(490, 430);
	// dstTri_front[1] = Point2f(490, 530);
	// dstTri_front[2] = Point2f(590, 430);
	// dstTri_front[3] = Point2f(590, 530);
	// dstTri_front[4] = Point2f(810, 430);
	// dstTri_front[5] = Point2f(910, 430);
	// dstTri_front[6] = Point2f(810, 530);
	// dstTri_front[7] = Point2f(890, 510);
	// dstTri_front[8] = Point2f(630, 430);
	// dstTri_front[9] = Point2f(770, 430);
	// dstTri_front[10] = Point2f(770, 530);
	// dstTri_front[11] = Point2f(630, 530);

	// dstTri_front[12] = Point2f(650, 530);
	// dstTri_front[13] = Point2f(690, 530);
	// dstTri_front[14] = Point2f(710, 510);
	// dstTri_front[15] = Point2f(570, 490);
	// dstTri_front[16] = Point2f(690, 470);
	// dstTri_front[17] = Point2f(730, 490);
	// dstTri_front[18] = Point2f(730, 510);
	// dstTri_front[19] = Point2f(670, 450);


	vector<Point2f> imgPoint_front, worldPoint_front;
    
    for (int i = 0; i< 20; i++){
        imgPoint_front.push_back(srcTri_front[i]);
        worldPoint_front.push_back(dstTri_front[i]);
    }
   
	// 计算12个二维点对之间的仿射变换矩阵（2行x3列）
	
	homo = findHomography(imgPoint_front, worldPoint_front, CV_RANSAC);
	cout << "front" << homo << endl;
    warpPerspective(front, warpPerspective_front, homo, Size(420, 100));
    cout << "front" << homo << endl;
    

    double a11 = homo.at<double>(0,0);
    double a12 = homo.at<double>(0,1);
    double a13 = homo.at<double>(0,2);

    double a21 = homo.at<double>(1,0);
    double a22 = homo.at<double>(1,1);
    double a23 = homo.at<double>(1,2);

    double a31 = homo.at<double>(2,0);
    double a32 = homo.at<double>(2,1);
    double a33 = homo.at<double>(2,2);

    // cout << "a11=" << a11 << endl;
    // cout << "a12=" << a12 << endl;
    // cout << "a13=" << a13 << endl;
    // cout << "a21=" << a21 << endl;
    // cout << "a22=" << a22 << endl;
    // cout << "a23=" << a23 << endl;
    // cout << "a31=" << a31 << endl;
    // cout << "a32=" << a32 << endl;
    // cout << "a33=" << a33 << endl;
    

    vector<Point2f> ponits, points_trans;
    Point2f inputImg, birdImg;
    inputImg.x = 817;
    inputImg.y = 539;

	ponits.push_back(inputImg);

 
	perspectiveTransform(ponits, points_trans, homo);
    cout << "points_trans[0].x " << points_trans[0].x << endl;
    cout << "points_trans[0].y " << points_trans[0].y << endl;

    


    // birdImg.x = (a11*inputImg.x+a21*inputImg.y+a31)/(a13*inputImg.x+a23*inputImg.y+a33);
    // birdImg.y = (a12*inputImg.x+a22*inputImg.y+a32)/(a13*inputImg.x+a23*inputImg.y+a33);
    
    // cout << (a11*inputImg.x+a21*inputImg.y+a31) << endl;
    // cout << birdImg.y << endl;





    namedWindow("front_src", WINDOW_NORMAL);
    imshow("front_src", warpPerspective_front);
	imwrite("afterwrap.jpg", warpPerspective_front);
	waitKey(0);
    return 0;
}
