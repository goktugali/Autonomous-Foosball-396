/**
 * Top takibi islemlerini icerir.
 */

#ifndef BALL_TRACKER_HPP
#define BALL_TRACKER_HPP

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <cmath>
#include <pthread.h>
#include "../defines.hpp"
#include "../global.hpp"
#include <unistd.h>

using namespace cv;
using namespace std;

int init_camera_params();
void get_ball_position(uint16_t* ball_pos_x, uint16_t* ball_pos_y);
void normalize_coordinates(const cv::Point& resultPoint, uint16_t* ball_pos_x, uint16_t* ball_pos_y);

#endif