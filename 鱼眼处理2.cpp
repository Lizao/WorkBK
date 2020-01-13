#include <opencv4/opencv2/opencv.hpp>
#include <fstream>
using namespace std;
using namespace cv;

string file_folder[4][3] = {
    "front_cam_img/", "front_cam_img/output/", "front_cam_img/output/result.txt",
    "back_cam_img/", "back_cam_img/output/", "back_cam_img/output/result.txt",
    "left_cam_img/", "left_cam_img/output/", "left_cam_img/output/result.txt",
    "right_cam_img/", "right_cam_img/output/", "right_cam_img/output/result.txt",
};


int image_count = 138;

void calibration(int pos)
{
    cout<<pos<<endl;
    ofstream fout(file_folder[pos][2]);  /**    保存定标结果的文件     **/
    /************************************************************************
    读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化
    *************************************************************************/
    cout << "开始提取角点………………" << endl;
    Size board_size = Size(6, 9);            /****    定标板上每行、列的角点数       ****/
    vector<Point2f> corners;                  /****    缓存每幅图像上检测到的角点       ****/
    vector<vector<Point2f>>  corners_Seq;    /****  保存检测到的所有角点       ****/
    vector<Mat>  image_Seq;
    int successImageNum = 0;                /****   成功提取角点的棋盘图数量    ****/

    int count = 0;
    for (int i = 0; i != image_count; i++)
    {
        cout << "img" << i + 1 << endl;
        string imageFileName;
        std::stringstream StrStm;
        StrStm << i + 1;
        StrStm >> imageFileName;
        imageFileName += ".jpg";
        cv::Mat image = imread(file_folder[pos][0]+imageFileName);
        cout<<file_folder[pos][0]<<endl;
        cout<<image.size()<<endl;
        if (image.empty()) continue;

        /*Mat image;//边界扩展后的图片
        int top = 0;
        int bottom = 0;
        int left = 400;
        int right = 400;
        copyMakeBorder(old_image,image,top,bottom,left,right,BORDER_CONSTANT);
        cout<<image.size()<<endl;*/


        /* 提取角点 */
        Mat imageGray;
        cvtColor(image, imageGray, COLOR_RGB2GRAY);
        bool patternfound = findChessboardCorners(image, board_size, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE +
            CALIB_CB_FAST_CHECK);
        if (!patternfound)
        {
            cout << "找不到角点，需删除图片文件" << imageFileName << "重新排列文件名，再次标定" << endl;
            //getchar();
            //exit(1);
        }
        else
        {
            /* 亚像素精确化 */
            cv::TermCriteria critia(3, 200, 1e-8);
            cornerSubPix(imageGray, corners, Size(11, 11), Size(-1, -1), critia);
            /* 绘制检测到的角点并保存 */
            Mat imageTemp = image.clone();
            for (int j = 0; j < corners.size(); j++)
            {
                circle(imageTemp, corners[j], 10, Scalar(0, 0, 255), 2, 8, 0);
            }
            string imageFileName;
            std::stringstream StrStm;
            StrStm << i + 1;
            StrStm >> imageFileName;
            imageFileName = file_folder[pos][0]+imageFileName+"_corner.jpg";
            imwrite(imageFileName, imageTemp);
            cout << "Frame corner#" << i + 1 << "...end" << endl;

            count = count + corners.size();
            successImageNum = successImageNum + 1;
            corners_Seq.push_back(corners);
            cout<<corners.size()<<endl;
        }
        image_Seq.push_back(image);
    }
    cout << "角点提取完成！\n";

    /************************************************************************
    摄像机定标
    *************************************************************************/
    cout << "开始定标………………" << endl;
    Size square_size = Size(50, 50);
    vector<vector<Point3f>>  object_Points;        /****  保存定标板上角点的三维坐标   ****/

    Mat image_points = Mat(1, count, CV_32FC2, Scalar::all(0));  /*****   保存提取的所有角点   *****/
    vector<int>  point_counts;
   
    /* 初始化定标板上角点的三维坐标 */
    for (int t = 0; t<successImageNum; t++)
    {
        vector<Point3f> tempPointSet;
        for (int i = 0; i<board_size.height; i++)
        {
            for (int j = 0; j<board_size.width; j++)
            {
                /* 假设定标板放在世界坐标系中z=0的平面上 */
                Point3f tempPoint;
                tempPoint.x = i*square_size.width;
                tempPoint.y = j*square_size.height;
                tempPoint.z = 0;
                tempPointSet.push_back(tempPoint);
            }
        }
        object_Points.push_back(tempPointSet);
    }

    for (int i = 0; i< successImageNum; i++)
    {
        point_counts.push_back(board_size.width*board_size.height);
    }

    /* 开始定标 */
    Size image_size = image_Seq[0].size();
    cv::Matx33d intrinsic_matrix;    /*****    摄像机内参数矩阵    ****/
    cv::Vec4d distortion_coeffs;     /* 摄像机的4个畸变系数：k1,k2,k3,k4*/
    std::vector<cv::Vec3d> rotation_vectors;                           /* 每幅图像的旋转向量 */
    std::vector<cv::Vec3d> translation_vectors;                        /* 每幅图像的平移向量 */
    int flags = 0;
    flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flags |= cv::fisheye::CALIB_CHECK_COND;
    flags |= cv::fisheye::CALIB_FIX_SKEW;
    fisheye::calibrate(object_Points, corners_Seq, image_size, intrinsic_matrix, distortion_coeffs, rotation_vectors, translation_vectors, flags, cv::TermCriteria(3, image_count, 1e-6));
    cout << "定标完成！\n";

    /************************************************************************
    对定标结果进行评价
    *************************************************************************/
 

    /************************************************************************
    保存定标结果
    *************************************************************************/
    cout << "开始保存定标结果………………" << endl;
    Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */

    //fout << "camera inside parameters：" << endl;
    fout << intrinsic_matrix << endl;
    cout << intrinsic_matrix << endl;
    //fout << "distort coefficient：\n";
    fout << distortion_coeffs << endl;
    cout << distortion_coeffs << endl;
    
    /*for (int i = 0; i<image_count; i++)
    {
        fout <<i + 1 << "image：" << endl;
        fout << rotation_vectors[i] << endl;

        Rodrigues(rotation_vectors[i], rotation_matrix);
        fout <<i + 1 << "image whirl matrix：" << endl;
        fout << rotation_matrix << endl;
        fout <<i + 1 << "image transition vector：" << endl;
        fout << translation_vectors[i] << endl;
    }
    */
    cout << "完成保存" << endl;
    fout << endl;
}

int main(int argc, char** argv)
{
    /*fstream file;
    file.open(calibration_result, ios::in);
    if (!file)
    {
        calibration();
        file.open(calibration_result, ios::in);
    }*/
    int pos = atoi(argv[1]);
    //calibration(pos);

    Mat distort_img = imread(file_folder[pos][0]+"test.jpg", 1);
    if (distort_img.empty()) {
        cout<<"No test image available"<<endl;
        return 0;
    }

    Size image_size = distort_img.size();
    Mat R = Mat::eye(3, 3, CV_32F);
    Mat mapx = Mat(image_size, CV_32FC1);
    Mat mapy = Mat(image_size, CV_32FC1);
    //Mat mapx, mapy;

    cv::Matx33d intrinsic_matrix_f(561.4246444562791, 0, 949.5498847405873,
                                 0, 541.0178330270679, 578.6834122953826,
                                 0, 0, 1);
    cv::Vec4d distortion_coeffs_f(-0.0292189, -0.0131399, 0.0088495, -0.00275281);

    cv::Matx33d transform_matrix_f(320, 0, 960,0, 270, 540,0, 0, 1);


    cv::Matx33d intrinsic_matrix_b(560.3160839891235, 0, 916.5222799377508,
                                 0, 539.9203586467951, 562.480976221171,
                                 0, 0, 1);
    cv::Vec4d distortion_coeffs_b(-0.0442155, 0.0267382, -0.02667, 0.00775014);

    cv::Matx33d transform_matrix_b(462.6368652343751, 0, 928.2856588202471,0, 309.2515014648438, 561.3747226668472,0, 0, 1);


    cv::Matx33d intrinsic_matrix_l(557.230904357054, 0, 941.5658671132867,
                                 0, 537.8771389038239, 563.3069347841155,
                                 0, 0, 1);
    cv::Vec4d distortion_coeffs_l(-0.0512353, 0.0415878, -0.0386909, 0.0104396);

    cv::Matx33d transform_matrix_l(215.0725312132016, 0, 1186.852485787089,0, 207.6026236901515, 520.6422327379561,0, 0, 1);


    cv::Matx33d intrinsic_matrix_r(560.6126814462816, 0, 940.9207424520841,
                                 0, 540.1527511360887, 564.8226978052469,
                                 0, 0, 1);
    cv::Vec4d distortion_coeffs_r(-0.0244398, -0.0236907, 0.0171895, -0.00491017);

    cv::Matx33d transform_matrix_r(462.6368652343751, 0, 928.2856588202471,0, 309.2515014648438, 561.3747226668472,0, 0, 1);


    cv::Matx33d intrinsic_matrix[4];
    intrinsic_matrix[0] = intrinsic_matrix_f;
    intrinsic_matrix[1] = intrinsic_matrix_b;
    intrinsic_matrix[2] = intrinsic_matrix_l;
    intrinsic_matrix[3] = intrinsic_matrix_r;

    cv::Vec4d distortion_coeffs[4];
    distortion_coeffs[0] = distortion_coeffs_f;
    distortion_coeffs[1] = distortion_coeffs_b;
    distortion_coeffs[2] = distortion_coeffs_l;
    distortion_coeffs[3] = distortion_coeffs_r;

    cv::Matx33d transform_matrix[4];
    transform_matrix[0] = transform_matrix_f;
    transform_matrix[1] = transform_matrix_b;
    transform_matrix[2] = transform_matrix_l;
    transform_matrix[3] = transform_matrix_r;


    for (int i = 1; i <= image_count; i++)
    {
        string imageFileName;
        std::stringstream StrStm;
        StrStm << i;
        StrStm >> imageFileName;
        string inputimagename = imageFileName + ".jpg";
        Mat undistort_img5;
        cv::Mat distort_img_new = imread(file_folder[pos][0]+inputimagename, 1);
        if (distort_img_new.empty()) continue;
        fisheye::undistortImage(distort_img_new, undistort_img5, intrinsic_matrix[pos], distortion_coeffs[pos], knew_per[pos], image_size);
        imwrite(file_folder[pos][1]+imageFileName+"output.jpg", undistort_img5);
    }

    Mat undistort_img5;
    cv::Matx33d new_intrinsic_matrix;

    //new_intrinsic_matrix = intrinsic_matrix[pos];
    //new_intrinsic_matrix(0,0) = intrinsic_matrix[pos](0,0)/2;
    //new_intrinsic_matrix(1,1) = intrinsic_matrix[pos](1,1)/2;

    //new_intrinsic_matrix = cv::getOptimalNewCameraMatrix(intrinsic_matrix[pos], distortion_coeffs[pos], image_size, 0.5, image_size, 0);
    //new_intrinsic_matrix = cv::getOptimalNewCameraMatrix(intrinsic_matrix[pos], distortion_coeffs[pos], image_size, 0.3, image_size, 0);
    //new_intrinsic_matrix = cv::getOptimalNewCameraMatrix(intrinsic_matrix[pos], distortion_coeffs[pos], image_size, 3, image_size, 0);

    fisheye::estimateNewCameraMatrixForUndistortRectify(intrinsic_matrix[pos], distortion_coeffs[pos], image_size, Matx33d::eye(), new_intrinsic_matrix, 0.5);
    new_intrinsic_matrix(0,2) += 200;
    //new_intrinsic_matrix(0,2) += 200;

    //new_intrinsic_matrix = (342.4588818648537, 0, 956.4423715445446,0, 329.9932441736607, 515.9263153734173,0, 0, 1);

    cout<<new_intrinsic_matrix<<endl;
    fisheye::initUndistortRectifyMap(intrinsic_matrix[pos], distortion_coeffs[pos], Matx33d::eye(), new_intrinsic_matrix, image_size, CV_16SC2, mapx, mapy);
    cv::remap(distort_img,undistort_img5, mapx, mapy, INTER_LINEAR);

    //cout<<intrinsic_matrix[pos]<<endl;
    //cout<<distortion_coeffs[pos]<<endl;

    //fisheye::undistortImage(distort_img, undistort_img5, intrinsic_matrix[pos], distortion_coeffs[pos], intrinsic_matrix[pos], image_size);
    imwrite(file_folder[pos][1]+"output_v3.jpg", undistort_img5);

    return 0;
}
