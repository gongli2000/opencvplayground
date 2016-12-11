//
//  perspectiveTransform.hpp
//  opencvplayground
//
//  Created by larry on 12/10/16.
//  Copyright © 2016 larry. All rights reserved.
//

#ifndef perspectiveTransform_hpp
#define perspectiveTransform_hpp

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
using namespace cv;

vector<Point>  tryToGetBoundingRectOfBoard(VideoCapture& cap);

Mat getPerspectiveMap(Size s, vector<cv::Point> &boardBoundingRect,int orientation,int dx);

//sdfsdfsdaf


#endif /* perspectiveTransform_hpp */
