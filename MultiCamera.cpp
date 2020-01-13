#include "VideoCap.h"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <sys/time.h>

int main()
{
    vector<string> capture_source;
    std::string videofile[4] = {"left", "front", "right", "back"};
    std::string dirseq[4] = {"left_cam_img/", "front_cam_img/", "right_cam_img/", "back_cam_img/"};
	for (int camNum = 0; camNum < 4; camNum++) {
        // string imageFileName = dirseq[camNum] + videofile[camNum] + ".h264";
        string imageFileName = videofile[camNum] + ".h264";
        capture_source.push_back(imageFileName);
    }
	
	// //fish Camera indices
	vector<int> capture_index = {0, 1, 2, 3};

	// //Make an instance of VideoCapture
	struct timeval start;
    struct timeval end;
    float timer;
    gettimeofday(&start, NULL); 
	VideoCap cam(capture_source);
    gettimeofday(&end, NULL); 
    timer = end.tv_sec - start.tv_sec + (float)(end.tv_usec - start.tv_usec)/1000000;
    cout << "1000 img cost time :" << 1000*timer << "ms" << endl;
   
    cam.img2Video();

	return 0;
}
