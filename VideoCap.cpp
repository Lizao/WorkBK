#include "VideoCap.h"
#include <stdio.h>
#include <sys/time.h>
using namespace std;
using namespace cv;

//==============FishEyeImg unDistort==================
cv::Matx33d intrinsic_matrix_f(561.4246444562791, 0, 949.5498847405873,
                                 0, 541.0178330270679, 578.6834122953826,
                                 0, 0, 1);
cv::Vec4d distortion_coeffs_f(-0.0292189, -0.0131399, 0.0088495, -0.00275281);
cv::Matx33d transform_matrix_f(426.6454813273333, 0, 955.8741797516922, 0, 411.1376585579857, 596.4814707013982, 0, 0, 1);


cv::Matx33d intrinsic_matrix_b(560.3160839891235, 0, 916.5222799377508,
                               0, 539.9203586467951, 562.480976221171,
                               0, 0, 1);
cv::Vec4d distortion_coeffs_b(-0.0442155, 0.0267382, -0.02667, 0.00775014);
cv::Matx33d transform_matrix_b(151.8357985450839, 0, 1008.486021088978, 0, 146.3089158930436, 524.5379210355152,0, 0, 1);

cv::Matx33d intrinsic_matrix_l(556.8067931254675, 0, 938.8831688099214,
                             0, 536.1146372877938, 565.4592246321504,
                             0, 0, 1);
cv::Vec4d distortion_coeffs_l(-0.0518639, 0.0347634, -0.0252865, 0.00556438);
cv::Matx33d transform_matrix_l(187.9034384060058, 0, 970.6090623734661, 0, 180.9205364767622, 540.2492934147239,0, 0, 1);


cv::Matx33d intrinsic_matrix_r(560.6126814462816, 0, 940.9207424520841,
                             0, 540.1527511360887, 564.8226978052469,
                             0, 0, 1);
cv::Vec4d distortion_coeffs_r(-0.0244398, -0.0236907, 0.0171895, -0.00491017);
cv::Matx33d transform_matrix_r(144.2567625127147, 0, 934.0073523106706, 0, 138.9920166276057, 581.5523049196689,0, 0, 1);

//==============WrapPeerspective==================
cv::Matx33d wrap_matrix_f(-0.2062371616273093, -1.215990860200039, 640.3417871778911,
 0.03055680318541585, -0.9072926464948965, 410.7684232137564,
 6.520666659100569e-05, -0.002958668442546747, 1);

cv::Matx33d wrap_matrix_b(0.1942857713339436, -0.8724055820651774, 197.9654898024444,
 -0.01813778594034251, 0.04135946518363309, -37.10149154349472,
 -5.206592452486131e-05, -0.002082405676417089, 1);

cv::Matx33d wrap_matrix_l(-2.545021899535207, 22.36662930973163, -5647.824952234198,
 -14.03427325658269, 17.99483980906433, 10672.55929603535,
 -0.007973053690503145, 0.04171185611722859, 1);

cv::Matx33d wrap_matrix_r(-0.02894879909372738, 0.123194788094976, -112.7542658666511,
 -0.4598734644292946, -0.886443252929899, 776.9889152784771,
 -0.0002143532323794598, -0.002051146157061052, 0.9999999999999999);

VideoCap::VideoCap(vector<string> stream_source)
{
    source = stream_source;
    av_register_all();
 
    camera_source = stream_source;
    camera_count = camera_source.size();
    isFishCamera = false;
    
    intrinsic_matrix[0] = ::intrinsic_matrix_l;
    intrinsic_matrix[1] = ::intrinsic_matrix_f;
    intrinsic_matrix[2] = ::intrinsic_matrix_r;
    intrinsic_matrix[3] = ::intrinsic_matrix_b;

    distortion_coeffs[0] = ::distortion_coeffs_l;
    distortion_coeffs[1] = ::distortion_coeffs_f;
    distortion_coeffs[2] = ::distortion_coeffs_r;
    distortion_coeffs[3] = ::distortion_coeffs_b;
    
    transform_matrix[0] = ::transform_matrix_l;
    transform_matrix[1] = ::transform_matrix_f;
    transform_matrix[2] = ::transform_matrix_r;
    transform_matrix[3] = ::transform_matrix_b;

    wrap_matrix[0] = ::wrap_matrix_l;
    wrap_matrix[1] = ::wrap_matrix_f;
    wrap_matrix[2] = ::wrap_matrix_r;
    wrap_matrix[3] = ::wrap_matrix_b;
 
    posSize[0] = Size(380, 1100);
    posSize[1] = Size(820, 280);
    posSize[2] = Size(400, 1100);
    posSize[3] = Size(820, 350);

    videofile[0] = "left";
    videofile[1] = "front";
    videofile[2] = "right";
    videofile[3] = "back";

    dirseq[0] = "left_cam_img/";
    dirseq[1] = "front_cam_img/";
    dirseq[2] = "right_cam_img/";
    dirseq[3] = "back_cam_img/";
    dirseq[4] = "out/";

    mSize = Size(posSize[3].width, posSize[2].height);
    maskHeigth = 190;
    r[0] = Rect(0, 0, posSize[0].width, posSize[0].height);  // left
    r[1] = Rect(0, 0, posSize[1].width, posSize[1].height);   // front
    r[2] = Rect(mSize.width -posSize[2].width, 0, posSize[2].width, posSize[2].height); //right
    r[3] = Rect(0, mSize.height-posSize[3].height, posSize[3].width, posSize[3].height); // back

    m = Mat(mSize, CV_8UC3);
    carMask = imread("car.png");

    maskF = Mat(r[1].size(), CV_8UC1, Scalar(1));
    maskB = Mat(r[3].size(), CV_8UC1, Scalar(1));
    
    for(int i = 0; i<4;i++) {
        fishmapx[i] = Mat(image_size, CV_32FC1);
        fishmapy[i] = Mat(image_size, CV_32FC1);
        fisheye::initUndistortRectifyMap(intrinsic_matrix[i], distortion_coeffs[i], Matx33d::eye(), transform_matrix[i], image_size, CV_16SC2, fishmapx[i],  fishmapy[i]);
    }

    /*make mask, need r */
    
    /*forward*/
    maskVec.push_back(vector<Point>());
    maskVec[0].push_back(Point(0, r[1].height));
    maskVec[0].push_back(Point(0, r[1].height - maskHeigth));
    maskVec[0].push_back(Point(r[0].width, r[1].height));
    
    maskVec.push_back(vector<Point>());
    maskVec[1].push_back(Point(r[1].width, r[1].height));
    maskVec[1].push_back(Point(r[1].width, r[1].height - maskHeigth-40));
    maskVec[1].push_back(Point(r[1].width - r[2].width, r[1].height));
    /*backward*/
    maskVec.push_back(vector<Point>());
    maskVec[2].push_back(Point(0, 0));
    maskVec[2].push_back(Point(0, maskHeigth));
    maskVec[2].push_back(Point(r[0].width, 0));
    
    maskVec.push_back(vector<Point>());
    maskVec[3].push_back(Point(mSize.width, 0));
    maskVec[3].push_back(Point(mSize.width, maskHeigth));
    maskVec[3].push_back(Point(mSize.width - r[2].width, 0));

   /*draw  mask*/
    drawContours(maskF, maskVec, 0, Scalar(0), CV_FILLED);
    drawContours(maskF, maskVec, 1, Scalar(0), CV_FILLED);
    drawContours(maskB, maskVec, 2, Scalar(0), CV_FILLED);
    drawContours(maskB, maskVec, 3, Scalar(0), CV_FILLED);
 
    num = 500;
    startMultiCapture();
}

Result parse_K_B(Point p1 , Point p2) {
    Result result;
    result.k = (double(p2.y - p1.y)/double(p2.x - p1.x));
    result.b = p1.y - result.k*p1.x;
    return result; 
}

VideoCap::VideoCap(vector<int> index)
{
    camera_index = index;
    camera_count = index.size();
    isFishCamera = true;

    for(int i = 0; i<4;i++) {
        fishmapx[i] = Mat(image_size, CV_32FC1);
        fishmapy[i] = Mat(image_size, CV_32FC1);
    }

    startMultiCapture();
}

void VideoCap::calFishMaps(int pos, Mat *mapx, Mat *mapy)
{
    fisheye::initUndistortRectifyMap(intrinsic_matrix[pos], distortion_coeffs[pos], Matx33d::eye(), transform_matrix[pos], image_size, CV_16SC2, fishmapx[pos],  fishmapy[pos]);
}

void VideoCap::processImg(int pos, Mat mapx, Mat mapy, int seq)
{  
    string inputFish = videofile[pos] + to_string(seq) + "_fish.ppm";
    Mat distort_img = imread(inputFish); 
    Mat undistort_img5;
    undistort_img5.create(distort_img.size(), distort_img.type());
    cv::remap(distort_img, undistort_img5, mapx, mapy, INTER_LINEAR);

    string afterWrapImg = dirseq[pos] + videofile[pos] + "_afterWrap.jpg";
    string afterMoveImg = videofile[pos] + "_" + to_string(seq) + "_afterWrap.jpg";

    Mat warpPerspective_dst = Mat::zeros(undistort_img5.rows, undistort_img5.cols, undistort_img5.type());
    warpPerspective(undistort_img5, warpPerspective_dst, wrap_matrix[pos], posSize[pos]);

    imwrite(afterWrapImg, warpPerspective_dst);
    rename(afterWrapImg.c_str(),  afterMoveImg.c_str());
    //remove(inputFish.c_str());
}

void VideoCap::img2Video()
{
    VideoWriter video("test_25fps.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, mSize);
    // String pattern = dirseq[4] + "*.jpg";
    // vector<String> fn;
    // glob(pattern, fn, false);
    String image_name;
    Mat image;
    for (int i = 1; i < num; i++) {
        image_name = dirseq[4] + to_string(i) + ".jpg";
        cout << image_name << endl;
        image = imread(image_name);
        if (image.empty()) {
            cout << "error in read pic" << endl;
            return ;
        }
        video << image;
    }
    cout << "处理完毕！" << endl;
    // 处理完之后会在得到一个名为test.avi的视频文件。
}

void VideoCap::SaveFrame(AVFrame *pFrame, int width, int height, int iFrame, int index , int timestamp) {

    FILE *pFile;        //文件指针
    string szFilename = videofile[index] + to_string(iFrame) + "_" + to_string(timestamp) + "ms_fish.ppm";//文件名（字符串）
    int y;              //
     
    pFile = fopen(szFilename.c_str(), "wb");            //打开文件，只写入
    if (pFile == NULL) {
        return;
    }
    //getch();

    fprintf(pFile,"P6\n%d %d\n255\n",width,height);//在文档中加入，必须加入，不然PPM文件无法读取
   // avframe_to_cvmat(pFrame);
    for (y = 0; y < height;y++) {
        fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width*3, pFile);
    }
    
    fclose(pFile);
}

void VideoCap::SaveFrame(AVFrame *pFrame, int width, int height, int iFrame, int index) {

    FILE *pFile;        //文件指针
    string szFilename = videofile[index] + to_string(iFrame) + "_fish.ppm";//文件名（字符串）
    int y;              //
     
    pFile = fopen(szFilename.c_str(), "wb");            //打开文件，只写入
    if (pFile == NULL) {
        return;
    }
    //getch();

    fprintf(pFile,"P6\n%d %d\n255\n",width,height);//在文档中加入，必须加入，不然PPM文件无法读取
   // avframe_to_cvmat(pFrame);
    for (y = 0; y < height;y++) {
        fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width*3, pFile);
    }
    
    fclose(pFile);
}

VideoCap::~VideoCap()
{
    cout << "~VideoCap " << endl; 
    stopMultiCapture();
}

void VideoCap::captureFrame(int index)
{
    AVFormatContext *pFormatCtx = NULL; //多媒体容器指针(结构体)
    
    int videoStream;    //视频流标记
    std::string filepath = source[index];
    if (avformat_open_input(&pFormatCtx, filepath.c_str(), NULL, NULL) !=0) {
        printf("打开视频失败");
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        return;
    }

    //打印文件容器信息（调试用）
    av_dump_format(pFormatCtx, 0, filepath.c_str(), 0);
    videoStream = -1;//默认值
    for (int i = 0; i < pFormatCtx->nb_streams;i++) {
        //读取【视频】码流编码信息（信息已在上一步填充，每个stream都记录了一条流的信息）
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            //这里判断是视频流，只对视频流操作
            videoStream = i;
            cout << "videoStream" << i << endl;
            break;
        }
    }
    if (videoStream == -1) {
        //读取失败
        return;
    }
    
    AVCodecContext *pCodecCtxOrig;  //多媒体容器原始指针
    AVCodec * pCodec;//视频流的解码器
    AVCodecContext * pCodecCtx; //拷贝原始指针

    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if (pCodec == NULL) {
        fprintf(stderr,"编码器无法支持该文件的编码\n");
        return;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);//释放内容
    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        fprintf(stderr, "无法复制编码器上下文\n");
        return;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        return;
    }
    
    AVFrame *pFrame = NULL; //处理用视频帧
    pFrame = av_frame_alloc();

    AVFrame *pFrameRGB;     //处理用视频帧
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB == NULL) {
        return;
    }
    
    //分配视频帧需要内存 (存放原始数据用)
    int numBytes;       //需要的内存大小
    uint8_t *buffer = NULL;

    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);//获取需要的内存大小
    buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    //关联frame和刚才分配的内容
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    
    //变量声明
    AVPacket packet;    //数据包变量
    int frameFinished;  //完整帧获取标记（获取到完整帧时，会被标记为真）
    struct SwsContext *sws_ctx = NULL;
    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    //读取帧并将前5帧保存到磁盘
    int i = 0;

    while (int x = av_read_frame(pFormatCtx, &packet) >= 0) {
        if (packet.stream_index == videoStream) {

            //解码视频帧（将数据包packet转成视频帧frame，每解码到一个完整帧，frameFinished会被设置为帧）
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
            AVStream *st  = pFormatCtx->streams[packet.stream_index];
            int fps = st->avg_frame_rate.num/st->avg_frame_rate.den;//每秒多少帧
           // cout << fps << endl;
            //如果是一个完整帧
            if (frameFinished) {
                //将视频帧原来的格式pCodecCtx->pix_fmt转换成RGB
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                             
                //只挑前【自定义】个完整帧进行保存
                if (++i <= num) {
                    int pts = i * ((1 / av_q2d(st->time_base)) / fps); 
                    int m_second= pts * av_q2d(st->time_base) * 1000;
                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i, index, m_second);
                   // SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i, index);
                   // Mat frame(pFrame->height, pFrame->width, CV_8UC3, pFrameRGB->data[0], pFrameRGB->linesize[0]);
                    // string name = to_string(i) + ".jpg";
                    // imwrite(name, frame);
                   // processImg(index, fishmapx[index], fishmapy[index], i);
                    
                    frameFinished = 0;
                }
                else {
                    cout << "we are losing frame now" << i << endl;
                    av_packet_unref(&packet);
                    break;
                }
            }
        }
        //释放由av_read_frame分配的数据包
        av_packet_unref(&packet);
    }

    //【释放内存等扫尾工作】
    av_free(buffer);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);
    avformat_close_input(&pFormatCtx);//对应avformat_open_input
    // getch();
}

void VideoCap::startMultiCapture()
{
    VideoCapture *capture;
    thread *t[4];
    thread *t5;
    for (int i = 0; i < camera_count; i++) {
        // if (!isFishCamera) {
  //           source[i]
        //  // capture = new VideoCapture(camera_source[i]);
  //  //          cout << "fps" << capture->get(CV_CAP_PROP_FPS) << endl;
        //  // cout << "Video read Setup: " << camera_source[i] << endl;
        // }
        // else {
        //  int idx = camera_index[i];
        //  capture = new VideoCapture(idx);
        //  cout << "Camera Setup: " << to_string(idx) << endl;
        // }
        // camera_capture.push_back(capture);
        t[i] = new thread(&VideoCap::captureFrame, this, i);
    }
    
    // t5 = new thread(&VideoCap::birdView, this, 1);
    for (int i = 0; i < 4; i++) {
        t[i]->join();
    }
   // t5->join();
}

double VideoCap::avelight(Mat M)
{
    Mat gray;
    cvtColor(M, gray, CV_BGR2GRAY);
    double sum = 0;
    double avg = 0;
    CvScalar scalar;
    int zeroPoint = 0;
    for (int i = 0; i< gray.rows; i+=1) {
        for (int j = 0; j<gray.cols; j++) {
            scalar = (int)gray.at<uchar>(i, j);
            if ((int)gray.at<uchar>(i, j) == 0) {
                zeroPoint ++;
            }
            sum += scalar.val[0];
        }
    }
    avg = sum / (gray.rows * gray.cols - zeroPoint);
    return avg;
}

Mat VideoCap::rejustPixel(Mat &src, double scale)
{
    Mat dst = Mat::zeros(src.size(), src.type());
    
    for (int i = 0; i < src.rows;i++) {
        for (int j = 0; j < src.cols;j++) {
            dst.at<Vec3b>(i,j)[0] = src.at<Vec3b>(i,j)[0]*scale > 255 ? 255 : src.at<Vec3b>(i,j)[0]*scale;
            dst.at<Vec3b>(i,j)[1] = src.at<Vec3b>(i,j)[1]*scale > 255 ? 255 : src.at<Vec3b>(i,j)[1]*scale;
            dst.at<Vec3b>(i,j)[2] = src.at<Vec3b>(i,j)[2]*scale > 255 ? 255 : src.at<Vec3b>(i,j)[2]*scale;          
        }
    }
    return dst;
}

void VideoCap::lightCompensation(Mat *v)
{
    double max = 0;
    double mean = 0;
    std::vector<double> gray;

    for (int i = 0; i < 4; i++) {
        mean = avelight(v[i]);
        if (max <= mean) {
            max = mean;
        }
        gray.push_back(mean);
    }

    for (int i = 0; i < 4; i++) {
        double ratio = max/gray[i];
        v[i] = rejustPixel(v[i], ratio);
    }
}

void VideoCap::gapSmooth(Mat *v)
{
    int W = 16;
    double K1 = 0;
    double K2 = 0;
    double w_y = 0;
    double i_diff = 0;
    Result bResult = parse_K_B(maskVec[0][1], maskVec[0][2]);
    w_y = fabs(W/2*(sqrt(1 + bResult.k*bResult.k)));

    // 左前
    for (int x =  maskVec[0][1].x ; x < maskVec[0][2].x; x++) {
        double y = x*bResult.k + bResult.b;
        int j = (y - w_y);
        int y_max = y + w_y;
        for (j ; j <  y_max ; j ++) {
            i_diff = fabs(bResult.k*x + (bResult.b - w_y) - j)/sqrt(bResult.k*bResult.k + 1);
            K1 = (1 - i_diff/W);
            K2 = (1- K1);

            v[1].at<Vec3b>(j,x)[0] = int(v[1].at<Vec3b>(j,x)[0] *K1 + v[0].at<Vec3b>(j,x)[0] *K2);
            v[1].at<Vec3b>(j,x)[1] = int(v[1].at<Vec3b>(j,x)[1] *K1 + v[0].at<Vec3b>(j,x)[1] *K2);
            v[1].at<Vec3b>(j,x)[2] = int(v[1].at<Vec3b>(j,x)[2] *K1 + v[0].at<Vec3b>(j,x)[2] *K2);

            v[0].at<Vec3b>(j,x)[0] = int(v[1].at<Vec3b>(j,x)[0] *K1 + v[0].at<Vec3b>(j,x)[0] *K2);
            v[0].at<Vec3b>(j,x)[1] = int(v[1].at<Vec3b>(j,x)[1] *K1 + v[0].at<Vec3b>(j,x)[1] *K2);
            v[0].at<Vec3b>(j,x)[2] = int(v[1].at<Vec3b>(j,x)[2] *K1 + v[0].at<Vec3b>(j,x)[2] *K2);
        }
    }

    // 右前
    bResult = parse_K_B(maskVec[1][1], maskVec[1][2]);
    w_y = fabs(W/2*(sqrt(1 + bResult.k*bResult.k)));

    for (int x = maskVec[1][2].x; x< maskVec[1][1].x; x++) {
        double y = x*bResult.k + bResult.b;
        int j = (y - w_y);
        int y_max = y + w_y;
        for (j ; j <  y_max ; j ++) {
            i_diff = fabs(bResult.k*x + (bResult.b - w_y) - j)/sqrt(bResult.k*bResult.k + 1);
            K1 = (1 - i_diff/W);
            K2 = (1- K1);

            v[1].at<Vec3b>(j,x)[0] = int(v[1].at<Vec3b>(j,x)[0] *K1 + v[2].at<Vec3b>(j,x - maskVec[1][2].x)[0] *K2);
            v[1].at<Vec3b>(j,x)[1] = int(v[1].at<Vec3b>(j,x)[1] *K1 + v[2].at<Vec3b>(j,x - maskVec[1][2].x)[1] *K2);
            v[1].at<Vec3b>(j,x)[2] = int(v[1].at<Vec3b>(j,x)[2] *K1 + v[2].at<Vec3b>(j,x - maskVec[1][2].x)[2] *K2);

            v[2].at<Vec3b>(j,x - maskVec[1][2].x)[0] = int(v[1].at<Vec3b>(j,x)[0] *K1 + v[2].at<Vec3b>(j,x - maskVec[1][2].x)[0] *K2);
            v[2].at<Vec3b>(j,x - maskVec[1][2].x)[1] = int(v[1].at<Vec3b>(j,x)[1] *K1 + v[2].at<Vec3b>(j,x - maskVec[1][2].x)[1] *K2);
            v[2].at<Vec3b>(j,x - maskVec[1][2].x)[2] = int(v[1].at<Vec3b>(j,x)[2] *K1 + v[2].at<Vec3b>(j,x - maskVec[1][2].x)[2] *K2);
        }
    }
    // 右后
    bResult = parse_K_B(maskVec[3][2], maskVec[3][1]);
    w_y = fabs(W/2*(sqrt(1 + bResult.k*bResult.k)));
    for (int x = maskVec[3][2].x; x< maskVec[3][1].x; x++) {
        double y = x*bResult.k + bResult.b;
        int j = (y - w_y);
        int y_max = y + w_y;
        for (j ; j <  y_max ; j ++) {
            i_diff = fabs(bResult.k*x + bResult.b - w_y - j)/sqrt(bResult.k*bResult.k + 1);
            K1 = (1 - i_diff/W);
            K2 = (1- K1);
            v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[0] = int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[0] *K1 + v[3].at<Vec3b>(j,x)[0] *K2)> 255 ? 255 :int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[0] *K1 + v[3].at<Vec3b>(j,x)[0] *K2);
            v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[1] = int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[1] *K1 + v[3].at<Vec3b>(j,x)[1] *K2)> 255 ? 255 :int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[1] *K1 + v[3].at<Vec3b>(j,x)[1] *K2);
            v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[2] = int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[2] *K1 + v[3].at<Vec3b>(j,x)[2] *K2)> 255 ? 255 :int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[2] *K1 + v[3].at<Vec3b>(j,x)[2] *K2);

            v[3].at<Vec3b>(j,x)[0] = int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[0] *K1 + v[3].at<Vec3b>(j,x)[0] *K2)> 255 ? 255 :int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[0] *K1 + v[3].at<Vec3b>(j,x)[0] *K2);
            v[3].at<Vec3b>(j,x)[1] = int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[1] *K1 + v[3].at<Vec3b>(j,x)[1] *K2)> 255 ? 255 :int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[1] *K1 + v[3].at<Vec3b>(j,x)[1] *K2);
            v[3].at<Vec3b>(j,x)[2] = int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[2] *K1 + v[3].at<Vec3b>(j,x)[2] *K2)> 255 ? 255 :int(v[2].at<Vec3b>(j+ (v[2].rows -v[3].rows),x-(v[3].cols - v[2].cols))[2] *K1 + v[3].at<Vec3b>(j,x)[2] *K2);
        }
    }
    
    // 左后
    bResult = parse_K_B(maskVec[2][2], maskVec[2][1]);
    w_y = fabs(W/2*(sqrt(1 + bResult.k*bResult.k)));

    for (int x = maskVec[2][1].x; x< maskVec[2][2].x; x++) {
        double y = x*bResult.k + bResult.b;
        int j = (y - w_y);
        int y_max = y + w_y;
        for (j ; j <  y_max ; j ++) {
            i_diff = fabs(bResult.k*x + bResult.b - w_y - j)/sqrt(bResult.k*bResult.k + 1);
            K1 = (1 - i_diff/W);
            K2 = (1- K1);
            v[0].at<Vec3b>(j+ (v[0].rows -v[3].rows),x)[0] = int(v[0].at<Vec3b>(j+(v[0].rows -v[3].rows),x)[0] *K1 + v[3].at<Vec3b>(j,x)[0] *K2);
            v[0].at<Vec3b>(j+ (v[0].rows -v[3].rows),x)[1] = int(v[0].at<Vec3b>(j+(v[0].rows -v[3].rows),x)[1] *K1 + v[3].at<Vec3b>(j,x)[1] *K2);
            v[0].at<Vec3b>(j+ (v[0].rows -v[3].rows),x)[2] = int(v[0].at<Vec3b>(j+(v[0].rows -v[3].rows),x)[2] *K1 + v[3].at<Vec3b>(j,x)[2] *K2);

            v[3].at<Vec3b>(j,x)[0] = int(v[0].at<Vec3b>(j+(v[0].rows -v[3].rows),x)[0] *K1 + v[3].at<Vec3b>(j,x)[0] *K2);
            v[3].at<Vec3b>(j,x)[1] = int(v[0].at<Vec3b>(j+(v[0].rows -v[3].rows),x)[1] *K1 + v[3].at<Vec3b>(j,x)[1] *K2);
            v[3].at<Vec3b>(j,x)[2] = int(v[0].at<Vec3b>(j+(v[0].rows -v[3].rows),x)[2] *K1 + v[3].at<Vec3b>(j,x)[2] *K2);
        }
    }
}


void VideoCap::birdView(int imgNum)
{
    int imageNum = 0;
    while(imageNum < num) {
        Mat v[4];
        bool imgReady = false;
        for (int i = 0; i < 4; i++) {
            string wrapImg = videofile[i] + "_" + to_string(imageNum+1) + "_afterWrap.jpg";
            v[i] = imread(wrapImg); // 读取四幅图片
            if(!v[i].data) {
                imgReady = false;
              //  cout << "img not found" << endl;
                break;
            }
            else {
                imgReady = true;
            }
        }

        if (!imgReady) {
            continue;
        }
       
       // if(imageNum / 5 == 0) {
       //    lightCompensation(v);
       // }
        //lightCompensation(v);
       // gapSmooth(v);

        int seq[4] = { 0, 2, 1, 3};
        // int seq[4] = {1, 3,  0, 2};

        for (int i = 0; i < 4; i++) {
            if(!v[seq[i]].data) {
                continue;
            }
            switch (seq[i]) {
                case 1:
                    v[seq[i]].copyTo(m(r[seq[i]]), maskF);
                   // v[seq[i]].copyTo(m(r[seq[i]]));
                    break;
                case 3:
                   v[seq[i]].copyTo(m(r[seq[i]]), maskB);
                   //v[seq[i]].copyTo(m(r[seq[i]]));
                    break;
                default:
                   v[seq[i]].copyTo(m(r[seq[i]]));
                   break;
            }
        }

         Rect car(int(0.5*(mSize.width-carMask.cols)), 270, carMask.cols, carMask.rows);
         carMask.copyTo(m(car));
        string name = dirseq[4] + to_string(imageNum+1) + ".jpg";
        imwrite(name, m);

        string deleteImg = to_string(imageNum+1) + "_afterWrap.jpg";

        for (int j =0; j<4;j++) {
            string deleteImg  = videofile[j] + "_" + to_string(imageNum+1) + "_afterWrap.jpg";
            remove(deleteImg.c_str());
        }
        
        
        imageNum ++;
    }
}

void VideoCap::stopMultiCapture()
{
    cout << "end" << endl;
//  VideoCapture *cap;
//  for (int i = 0; i < camera_count; i++) {
//      cap = camera_capture[i];
//      if (cap->isOpened()) {
//          cap->release();
//          cout << "Capture " << i << " released" << endl;
//      }
//  }
//}
}