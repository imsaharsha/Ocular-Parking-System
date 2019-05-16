#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

  if(argc != 3){
        cout <<" Enter two images as command-line arguments" << endl;
        return -1;
    }

  Mat img = imread(argv[1], IMREAD_COLOR);
  Mat img2 = imread(argv[2], IMREAD_COLOR);

  if(img.empty()||img2.empty()){
        // Check for invalid input
        cout << "Could not open or find the image" << endl;
        return -1;
    }

  cvtColor(img, img, COLOR_BGR2GRAY);        
  blur(img, img, Size(5,5));              

  //First Image
  Mat img_hor, img_ver;
  Sobel(img, img_hor, CV_8U, 0, 1, 3, 1, 0);  
  Sobel(img, img_ver, CV_8U, 1, 0, 3, 1, 0); 

  img = img_hor + img_ver;

  threshold(img, img, 0, 255, THRESH_OTSU+THRESH_BINARY);

  cvtColor(img2, img2, COLOR_BGR2GRAY);        
  blur(img2, img2, Size(5,5));              

  //Second image
  Mat img_hor2, img_ver2;
  Sobel(img2, img_hor2, CV_8U, 0, 1, 3, 1, 0);  
  Sobel(img2, img_ver2, CV_8U, 1, 0, 3, 1, 0); 

  img2 = img_hor2 + img_ver2;

  threshold(img2, img2, 0, 255, THRESH_OTSU+THRESH_BINARY);

  Mat img3 = img - img2;

  Mat kernel = getStructuringElement(MORPH_RECT, Size(30, 30));   
  morphologyEx(img3, img3, CV_MOP_CLOSE, kernel);

  Mat erode_element = getStructuringElement(MORPH_RECT, Size(30, 30));                    
  Mat dilate_element = getStructuringElement(MORPH_RECT, Size(27, 25));

  erode(img3, img3, erode_element);
  dilate(img3, img3, dilate_element);

  
  img3+=img2;

  //Inverse thresholding
  threshold(img3,img3, 0, 255, THRESH_BINARY_INV);

  Mat erode_element2 = getStructuringElement(MORPH_RECT, Size(5, 8));
  Mat dilate_element2 = getStructuringElement(MORPH_RECT, Size(6, 8));

  erode(img3, img3, erode_element2);
  dilate(img3, img3, dilate_element2);

  Mat kernel2 = getStructuringElement(MORPH_RECT, Size(2,1));   
  morphologyEx(img3, img3, CV_MOP_CLOSE, kernel2);

  Mat erode_element3 = getStructuringElement(MORPH_RECT, Size(20, 15));        
  Mat dilate_element3 = getStructuringElement(MORPH_RECT, Size(15, 5));

  erode(img3, img3, erode_element3);
  dilate(img3, img3, dilate_element3);
  

  //Print
  namedWindow("img", WINDOW_AUTOSIZE);
  imshow("img",img);

  namedWindow("img2", WINDOW_AUTOSIZE);
  imshow("img2",img2);

  namedWindow("img3", WINDOW_AUTOSIZE);
  imshow("img3",img3);

  waitKey(0);
  return 0;
}

//g++ temp.cpp `pkg-config --cflags --libs opencv` -o temp
//./temp img.jpg img_empty.jpg
