#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void* );
int testContours( Mat& src );
void capturecamera();
void dothresh(Mat& src, int threshold);
void capturecontours();
Mat getoneframe();
void findlargestcontour(Mat& src);
int maxcontourarea(vector<vector<Point>> &contours);

int main(int, char**)
{
    Mat src = imread("/Users/larry/sampleimages/lenna.png", 1);
    capturecontours();
    
    return 0;
}


void capturecamera()
{
    VideoCapture cap(0);
    if(!cap.isOpened())return;
    
    Mat edges;
    namedWindow("edges",1);
    for(;;){
        Mat frame;
        cap >> frame;
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }
}

void capturecontours()
{
    VideoCapture cap(0);
    if(!cap.isOpened())return;
    Mat canny_output,srcgray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat edges;
    namedWindow("edges",1);
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    for(;;){
        int nhulls = 6;
        vector<vector<Point>>  hulls;
        vector<Point> hull;
        for(int cnt =0; cnt < nhulls; cnt++){
            Mat frame;
            if(cap.read(frame)){
                cvtColor(frame, edges, COLOR_BGR2GRAY);
                GaussianBlur(edges, edges, Size(11,11), 1.5, 1.5);
                Canny(edges, canny_output, 0, 30, 3);
                
                findContours( canny_output, contours, hierarchy,
                             CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
                
                int max_area_index = maxcontourarea(contours);
                convexHull( Mat(contours[max_area_index]), hull, false );
                hulls.push_back(hull);
                
            }
        }
       

        Mat cframe;
        cap >> cframe;
        vector<vector<Point>> h(1);
        int i =maxcontourarea(hulls);
        double perimeter_length = cv::arcLength(hulls[i],true);
        cv:: approxPolyDP( hulls[i], h[0], 0.01*perimeter_length, true );
    
       
        //drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
        drawContours(cframe, h, 0, Scalar( 0, 0, 255 ), 4, 8,
                     vector<Vec4i>(), 0, Point() );
        imshow( "Contours", cframe );
        waitKey(1);
    }
}

int testContours( Mat& src )
{
 
    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
    
    /// Create Window
    string source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );
    
    createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );
    
   // waitKey(0);
    return(0);
}

void dothresh(Mat& src, int threshold)
{
    Mat canny_output,srcgray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    cvtColor( src, srcgray, CV_BGR2GRAY );
    blur( srcgray, srcgray, Size(3,3) );
    Canny( srcgray, canny_output, threshold, threshold*2, 3 );
    
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
  
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    
    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    waitKey(1);
}


int maxcontourarea(vector<vector<Point>> &contours)
{
    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;
    for( int i = 0; i< contours.size(); i++ )
    {
        double a=contourArea( contours[i],false);
        if(a>largest_area){
            largest_area=a;
            largest_contour_index=i;
            bounding_rect=boundingRect(contours[i]);
        }
    }
    return largest_contour_index;
}

void findlargestcontour(Mat& src){
    {
        int largest_area=0;
        int largest_contour_index=0;
        Rect bounding_rect;
        
        Mat thr(src.rows,src.cols,CV_8UC1);
        Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
        cvtColor(src,thr,CV_BGR2GRAY); //Convert to gray
        threshold(thr, thr,25, 255,THRESH_BINARY); //Threshold the gray
        
        vector<vector<Point>> contours; // Vector for storing contour
        vector<Vec4i> hierarchy;
        
        findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
        
        for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
        {
            double a=contourArea( contours[i],false);  //  Find the area of contour
            if(a>largest_area){
                largest_area=a;
                largest_contour_index=i;                //Store the index of largest contour
                bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
            }
            
        }
        
        Scalar color( 255,255,255);
        drawContours( dst, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
        rectangle(src, bounding_rect,  Scalar(0,255,0),1, 8,0);  
        imshow( "src", src );
        imshow( "largest Contour", dst );
        waitKey(0);
    }

}
void thresh_callback(int, void* )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    
    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}
