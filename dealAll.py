import cv2
import numpy as np
from PIL import Image
import numpy as np
import pandas as pd
import os

class ImageMerge():
    def __init__(self):
        self.path = '/home/toby/Projects/SVM/SVMData/'
        self.path_front = '/home/toby/Projects/SVM/SVMData/front/'
        self.path_back = '/home/toby/Projects/SVM/SVMData/back/'
        self.path_left = '/home/toby/Projects/SVM/SVMData/left/'
        self.path_right = '/home/toby/Projects/SVM/SVMData/right/'
        self.path_result = '/home/toby/Projects/SVM/SVMData/result/'

    def cutImg(self, i, img):
        front_imgName = self.path_front + str(i) + ".jpg"
        left_imgName = self.path_left + str(i) + ".jpg"
        right_imgName = self.path_right + str(i) + ".jpg"
        back_imgName = self.path_back + str(i) + ".jpg"
        cropped = img[0:400, 0:640]
        cv2.imwrite(front_imgName, cropped)
        # print cropped.shape
        cropped = img[400:800,  0:640]
        cv2.imwrite(back_imgName, cropped)
        # print cropped.shape
        cropped = img[800:1200,  0:640]
        cv2.imwrite(left_imgName, cropped)
        # print cropped.shape
        cropped = img[1200:1600,  0:640]
        cv2.imwrite(right_imgName, cropped)
        # print cropped.shape

    def readVideo(self):
        videoName = "video0002_19700101_091000.h264"
        cap = cv2.VideoCapture(videoName)
        i = 0
        index = 0
        while 1:
            ret, frame = cap.read()
            print frame.shape
            frame_name = self.path_result + str(i) + ".jpg"
            print frame_name
            # self.cutImg(i, frame)
            # cv2.namedWindow(frame_name,cv2.WINDOW_NORMAL)
            # cv2.imshow(frame_name, frame)
            # cv2.waitKey()
            cv2.imwrite(frame_name, frame)
            i = i + 1
            print i

    def Cut1(self):
        filelist = os.listdir(self.path_result)
        total_num = len(filelist)
        i = 0
        for i in range(len(filelist)):
            name = str(i) + ".jpg"
            print name
            img = cv2.imread(name)
            cv2.imshow(name, img)
            cv2.waitKey()
            cropped = img[0:800, 0:1280]
            cv2.imwrite(self.path_front + name, cropped)
            print cropped.shape

        # for item in filelist:
        #     if item.endswith('.jpg'):
        #         print 'name %s' %(item)
        #         img = cv2.imread(item)
        #         # print img.shape
        #         # cv2.imshow("hello", img)
        #         # cv2.waitKey(0)
        #         imgName = item[13:]
        #         print imgName
        #         os.rename(self.path_right+item, self.path_right+imgName)
        #         front_imgName = self.path_front + imgName + ".jpg"
        #         # left_imgName = self.path_left + imgName + ".jpg"
        #         # right_imgName = self.path_right + imgName + ".jpg"
        #         # back_imgName = self.path_back + imgName + ".jpg"
        #         # print ' name %s' %(imgName)
        #         cropped = img[0:800, 0:1280]
        #         cv2.imwrite(front_imgName, cropped)
        #         print cropped.shape
                # cropped = img[800:1600, 0:1280]
                # cv2.imwrite(back_imgName, cropped)
                # print cropped.shape
                # cropped = img[0:800, 1280:2560]
                # cv2.imwrite(left_imgName, cropped)
                # print cropped.shape
                # cropped = img[800:1600, 1280:2560]
                # cv2.imwrite(right_imgName, cropped)
                # print cropped.shape
                # print 'name %s' %(imgName)
       
        i = i + 1

    # def Cut2(self):
    #     filelist = os.listdir(self.path_right)
    #     total_num = len(filelist)
    #     i = 0
    #     for item in filelist:
    #         if item.endswith('.jpg'):
    #             print 'name %s' %(item)
    #             img = cv2.imread(item)
    #             imgName = item[13:]
    #             print imgName
    #             os.rename(self.path_right+item, self.path_right+imgName)
    #             front_imgName = self.path_front + imgName + ".jpg"
    #             left_imgName = self.path_left + imgName + ".jpg"
    #             right_imgName = self.path_right + imgName + ".jpg"
    #             back_imgName = self.path_back + imgName + ".jpg"
    #             print ' name %s' %(imgName)
    #             cropped = img[0:400, 0:640]
    #             cv2.imwrite(front_imgName, cropped)
    #             print cropped.shape
    #             cropped = img[400:800,  0:640]
    #             cv2.imwrite(back_imgName, cropped)
    #             print cropped.shape
    #             cropped = img[800:1200,  0:640]
    #             cv2.imwrite(left_imgName, cropped)
    #             print cropped.shape
    #             cropped = img[1200:1600,  0:640]
    #             cv2.imwrite(right_imgName, cropped)
    #             print cropped.shape
    #             print 'name %s' %(imgName)
       
    #     i = i + 1

    def merge(self):
        filelist = os.listdir(self.path_front)
        i = 1
        for i in range(1, len(filelist)):
            name=str(i) + ".jpg"
            img_front = cv2.imread(self.path_front + name);
            img_back = cv2.imread(self.path_back + name);
            img_left = cv2.imread(self.path_left +name);
            img_right = cv2.imread(self.path_right + name);
            image_left = np.concatenate((img_front, img_back))
            image_right = np.concatenate((img_left, img_right))
            imgAll = np.concatenate([image_left, image_right], axis=1)
            cv2.imwrite(self.path_result+name, imgAll)
            print i
        i = i + 1

    def img2video(self):
        filelist = os.listdir(self.path_front)
        print len(filelist)
        fps = 30
        # size = (640, 400)
        size = (2560,1600)
        video0 = cv2.VideoWriter("front.avi", cv2.VideoWriter_fourcc('M', 'P', '4', '2'), fps, size)
        video1 = cv2.VideoWriter("back.avi", cv2.VideoWriter_fourcc('M', 'P', '4', '2'), fps, size)
        video2 = cv2.VideoWriter("left.avi", cv2.VideoWriter_fourcc('M', 'P', '4', '2'), fps, size)
        video3 = cv2.VideoWriter("right.avi", cv2.VideoWriter_fourcc('M', 'P', '4', '2'), fps, size)
        i = 1
        for i in range(1, len(filelist)):
            name = str(i) + ".jpg"
            img0 = cv2.imread(self.path_front + name)
            img1 = cv2.imread(self.path_back + name)
            img2 = cv2.imread(self.path_left + name)
            img3 = cv2.imread(self.path_right + name)
            video0.write(img0)
            video1.write(img1)
            video2.write(img2)
            video3.write(img3)
            print i
        i = i + 1
        

if __name__ == '__main__':
    hi = ImageMerge()
    hi.readVideo()
    # hi.Cut1()
    print "Cut End"
    # hi.merge()
    # # print "Merge End"
    # hi.img2video()
    # print "img2video End"


# img1 = cv2.imread("front.jpg");
# img2 = cv2.imread("back.jpg");

# img3 = cv2.imread("left.jpg");
# img4 = cv2.imread("right.jpg");

# image_left = np.concatenate((img1, img2))
# cv2.namedWindow("image_left",cv2.WINDOW_NORMAL)
# cv2.imshow('image_left', image_left)
# cv2.waitKey(0)

# image_right = np.concatenate((img3, img4))
# cv2.namedWindow("image_right",cv2.WINDOW_NORMAL)
# cv2.imshow('image_right', image_right)
# cv2.waitKey(0)

# imgAll = np.concatenate([image_left, image_right], axis=1)
# cv2.namedWindow("imgAll",cv2.WINDOW_NORMAL)
# cv2.imshow('imgAll', imgAll)
# cv2.waitKey(0)
