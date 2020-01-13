#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace cv;
using namespace std;
// 0: left up  1: right up  2:rigth down  3: left down

Mat flipClockwise(Mat &src, Mat &dst) {
    Mat temp;
    transpose(src, temp);
    flip(temp, dst, 1);
    // imshow("flip:1", dst);
    // waitKey();
    return dst;
}

Mat flipAntiClockwise(Mat &src, Mat &dst) {
    Mat temp;
    transpose(src, temp);
    flip(temp, dst, 0);
    // imshow("flip:1", dst);
    // waitKey();
    return dst;
}

int main() 
{
    Mat v[4];
    for (int i = 0; i < 4; i++)
    {
        char buf[10];
        sprintf(buf, "%d.jpg", i);
        v[i] = imread(buf); // 读取四幅图片
        string imageFileName;
        std::stringstream StrStm;
        StrStm<<i;
        StrStm>>imageFileName;
        imageFileName += ".jpg";
    }

    Size mSize, carSize, chessBordWidth;
    mSize = Size(v[3].cols, v[2].rows);
    
    Rect r[4];
    Mat m = Mat(mSize, CV_8UC3);
    r[0] = Rect(0, 0, v[0].cols, v[0].rows);  // left
    cv::rectangle(m, r[0], Scalar(255, 0, 0), 1, LINE_8,0);
    
    r[1] = Rect(0, 0, v[1].cols, v[1].rows);   // front
    cv::rectangle(m, r[1], Scalar(255, 255, 0), 1, LINE_8,0);
    
    r[2] = Rect(mSize.width -v[2].cols, 0, v[2].cols, v[2].rows); //right
    cv::rectangle(m, r[2], Scalar(0, 255, 0), 1, LINE_8,0);
    
    r[3] = Rect(0, mSize.height-v[3].rows, v[3].cols, v[3].rows); // back
    cv::rectangle(m, r[3], Scalar(0, 0, 255), 1, LINE_8,0);

    // Mat maskF = Mat(r[1].size(), CV_8UC1, Scalar(1));
    // Mat maskB = Mat(r[3].size(), CV_8UC1, Scalar(1));

    Mat maskL = Mat(r[0].size(), CV_8UC1, Scalar(1));
    Mat maskR = Mat(r[2].size(), CV_8UC1, Scalar(1));

    int maskHeigth = 190;
    /*make mask, need r */
    vector<vector<Point> > maskVec;
    /*forward*/
    // maskVec.push_back(vector<Point>());
    // maskVec[0].push_back(Point(0, r[1].height));
    // maskVec[0].push_back(Point(0, r[1].height - maskHeigth));
    // maskVec[0].push_back(Point(r[0].width, r[1].height));
    // maskVec.push_back(vector<Point>());
    // maskVec[1].push_back(Point(r[1].width, r[1].height));
    // maskVec[1].push_back(Point(r[1].width, r[1].height - maskHeigth-40));
    // maskVec[1].push_back(Point(r[1].width - r[2].width, r[1].height));
    // /*backward*/
    // maskVec.push_back(vector<Point>());
    // maskVec[2].push_back(Point(0, 0));
    // maskVec[2].push_back(Point(0, maskHeigth-140));
    // maskVec[2].push_back(Point(r[0].width, 0));
    // maskVec.push_back(vector<Point>());
    // maskVec[3].push_back(Point(mSize.width, 0));
    // maskVec[3].push_back(Point(mSize.width, maskHeigth-140));
    // maskVec[3].push_back(Point(mSize.width - r[2].width, 0));

    maskVec.push_back(vector<Point>());
    maskVec[0].push_back(Point(r[0].width, maskHeigth+50));
    maskVec[0].push_back(Point(0, 0));
    maskVec[0].push_back(Point(r[0].width, 0));


    
    maskVec.push_back(vector<Point>());
    maskVec[1].push_back(Point(r[0].width, r[0].height - maskHeigth-50));
    maskVec[1].push_back(Point(0, r[0].height));
    maskVec[1].push_back(Point(r[0].width, r[0].height));
    /*backward*/
    maskVec.push_back(vector<Point>());
    maskVec[2].push_back(Point(0, maskHeigth + 50));
    maskVec[2].push_back(Point(r[2].width, 0));
    maskVec[2].push_back(Point(0, 0));
    
    maskVec.push_back(vector<Point>());
    maskVec[3].push_back(Point(0, r[2].height - maskHeigth-50));
    maskVec[3].push_back(Point(r[2].width, r[2].height));
    maskVec[3].push_back(Point(0, r[2].height));

   /*draw  mask*/
    drawContours(maskL, maskVec, 0, Scalar(167), CV_FILLED);
    drawContours(maskL, maskVec, 1, Scalar(167), CV_FILLED);
    drawContours(maskR, maskVec, 2, Scalar(167), CV_FILLED);
    drawContours(maskR, maskVec, 3, Scalar(167), CV_FILLED);

    namedWindow("m_with_front", WINDOW_NORMAL);
    imshow("m_with_front", maskL);
    waitKey(0);

    namedWindow("m_with_front", WINDOW_NORMAL);
    imshow("m_with_front", maskR);
    waitKey(0);
  




    // int seq[4] = {0, 2, 1, 3};
    // for (int i = 0; i < 4; i++)
    // {
    //     if(!v[seq[i]].data)
    //     {
    //         continue;
    //     }
    //     switch (seq[i])
    //     {
    //         case 1:
    //             cout << seq[i] << endl;
    //             v[seq[i]].copyTo(m(r[seq[i]]), maskF);
    //             namedWindow("m_with_front", WINDOW_NORMAL);
    //             imshow("m_with_front", m);
    //             imwrite("m_with_front.jpg", m);
    //             waitKey(0);    
    //             break;
    //         case 3:
    //             cout << seq[i] << endl;
    //             v[seq[i]].copyTo(m(r[seq[i]]), maskB);
    //             namedWindow("m_with_back", WINDOW_NORMAL);
    //             imwrite("m_with_back.jpg", m);
    //             imshow("m_with_back", m);
    //             waitKey(0);    
    //             break;
    //         default:
    //            cout << seq[i] << endl;
    //            v[seq[i]].copyTo(m(r[seq[i]]));
    //            namedWindow("m_with_end", WINDOW_NORMAL);
    //            if (seq[i]==0) {
    //                imwrite("m_with_left.jpg", m);
    //            }
    //            else {
    //                 imwrite("m_with_right.jpg", m);
    //            }
    //            imshow("m_with_end", m);
    //            waitKey(0);    
    //            break;
    //     }
    // }
    imwrite("birdview.jpg", m);
    return 0;
}