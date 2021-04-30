#include <opencv2/video/tracking.hpp>
#include "ball_tracker.h"
#include "../control.h"

const bool predictKick = false;

// Camera frame
cv::Mat frame;

// >>>> Kalman Filter
int stateSize = 6;
int measSize = 4;
int contrSize = 0;

unsigned int type = CV_32F;
cv::KalmanFilter kf(stateSize, measSize, contrSize, type);

cv::Mat state(stateSize, 1, type);  // [x,y,v_x,v_y,w,h]
cv::Mat meas(measSize, 1, type);    // [z_x,z_y,z_w,z_h]
// Camera Index
int idx = 0;

// Camera Capture
cv::VideoCapture cap;

char ch = 0;
double ticks = 0;
bool found = false;
int notFoundCount = 0;
// >>>>> Main loop

cv::Point foundResult;
cv::Point predictResult;

int init_camera_params()
{
    cv::setIdentity(kf.transitionMatrix);
    kf.measurementMatrix = cv::Mat::zeros(measSize, stateSize, type);
    kf.measurementMatrix.at<float>(0) = 1.0f;
    kf.measurementMatrix.at<float>(7) = 1.0f;
    kf.measurementMatrix.at<float>(16) = 1.0f;
    kf.measurementMatrix.at<float>(23) = 1.0f;

    kf.processNoiseCov.at<float>(0) = 1e-2;
    kf.processNoiseCov.at<float>(7) = 1e-2;
    kf.processNoiseCov.at<float>(14) = 5.0f;
    kf.processNoiseCov.at<float>(21) = 5.0f;
    kf.processNoiseCov.at<float>(28) = 1e-2;
    kf.processNoiseCov.at<float>(35) = 1e-2;

    // Measures Noise Covariance Matrix R
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
    // <<<< Kalman Filter

    // >>>>> Camera Settings
    if (!cap.open(idx))
    {
        cout << "Webcam not connected.\n" << "Please verify\n";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(5,30);
    // <<<<< Camera Settings
}
void get_ball_position(uint16_t* ball_pos_x, uint16_t* ball_pos_y)
{
    double precTick = ticks;
    ticks = (double) cv::getTickCount();

    double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds

    // Frame acquisition
    cap >> frame;

    // Crop and zoom
    cv::Mat ROI(frame, cv::Rect(70, 80, 540, 350));
    ROI.copyTo(frame);
    cv::Mat resized_frame(cv::Size(640, 480), CV_8UC3);
    resize(frame, resized_frame, resized_frame.size());
    frame = resized_frame.clone();

    cv::Mat res;
    frame.copyTo( res );

    if (found)
    {
        // >>>> Matrix A
        kf.transitionMatrix.at<float>(2) = dT;
        kf.transitionMatrix.at<float>(9) = dT;
        // <<<< Matrix A
        state = kf.predict();

        cv::Rect predRect;
        predRect.width = state.at<float>(4);
        predRect.height = state.at<float>(5);
        predRect.x = state.at<float>(0) - predRect.width / 2;
        predRect.y = state.at<float>(1) - predRect.height / 2;

        cv::Point center;
        center.x = state.at<float>(0);
        center.y = state.at<float>(1);
        predictResult.x = center.x;
        predictResult.y = center.y;

        // Todo: think this algorithm.
        if (predictKick)
        {
            int servo = is_kick_need(center.x);
            if(-1 != servo)
                trigger_servo(servo);
        }

        cv::circle(res, center, 2, CV_RGB(255,0,0), -1);
        cv::rectangle(res, predRect, CV_RGB(255,0,0), 2);
    }

    // >>>>> Noise smoothing
    cv::Mat blur;
    cv::GaussianBlur(frame, blur, cv::Size(5, 5), 3.0, 3.0);
    // <<<<< Noise smoothing

    // >>>>> HSV conversion
    cv::Mat frmHsv;
    cv::cvtColor(blur, frmHsv, cv::COLOR_BGR2HSV);
    // <<<<< HSV conversion

    // >>>>> Color Thresholding
    // Note: change parameters for different colors
    cv::Mat rangeRes = cv::Mat::zeros(frame.size(), CV_8UC1);
    cv::inRange(frmHsv, cv::Scalar(90, 140, 60),
                cv::Scalar(120 , 255, 255), rangeRes);
    // <<<<< Color Thresholding

    // >>>>> Improving the result
    cv::erode(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
    // <<<<< Improving the result

    // Thresholding viewing
    //cv::imshow("Threshold", rangeRes);

    // >>>>> Contours detection
    vector<vector<cv::Point> > contours;
    cv::findContours(rangeRes, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_NONE);
    // <<<<< Contours detection

    // >>>>> Filtering
    vector<vector<cv::Point> > balls;
    vector<cv::Rect> ballsBox;
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect bBox;
        bBox = cv::boundingRect(contours[i]);

        float ratio = (float) bBox.width / (float) bBox.height;
        if (ratio > 1.0f)
            ratio = 1.0f / ratio;

        // Searching for a bBox almost square
        if (ratio > 0.75 && bBox.area() >= 400)
        {
            balls.push_back(contours[i]);
            ballsBox.push_back(bBox);
        }
    }
    // <<<<< Filtering

    // >>>>> Detection result
    for (size_t i = 0; i < balls.size(); i++)
    {
        cv::drawContours(res, balls, i, CV_RGB(20,150,20), 1);
        cv::rectangle(res, ballsBox[i], CV_RGB(0,255,0), 2);

        cv::Point center;
        center.x = ballsBox[i].x + ballsBox[i].width / 2;
        center.y = ballsBox[i].y + ballsBox[i].height / 2;
        cv::circle(res, center, 2, CV_RGB(20,150,20), -1);

        foundResult.x = center.x;
        foundResult.y = center.y;

        stringstream sstr;
        sstr << "(" << center.x << "," << center.y << ")";
        cv::putText(res, sstr.str(),
                    cv::Point(center.x + 3, center.y - 3),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
    }
    // <<<<< Detection result

    // >>>>> Kalman Update
    if (balls.size() == 0)
    {
        notFoundCount++;
        //cout << "notFoundCount:" << notFoundCount << endl;
        if( notFoundCount >= 100 )
        {
            found = false;
        }
    }
    else
    {
        // Todo: Handle not found situation. Use global for this state.
        notFoundCount = 0;

        meas.at<float>(0) = ballsBox[0].x + ballsBox[0].width / 2;
        meas.at<float>(1) = ballsBox[0].y + ballsBox[0].height / 2;
        meas.at<float>(2) = (float)ballsBox[0].width;
        meas.at<float>(3) = (float)ballsBox[0].height;

        if (!found) // First detection!
        {
            // >>>> Initialization
            kf.errorCovPre.at<float>(0) = 1; // px
            kf.errorCovPre.at<float>(7) = 1; // px
            kf.errorCovPre.at<float>(14) = 1;
            kf.errorCovPre.at<float>(21) = 1;
            kf.errorCovPre.at<float>(28) = 1; // px
            kf.errorCovPre.at<float>(35) = 1; // px

            state.at<float>(0) = meas.at<float>(0);
            state.at<float>(1) = meas.at<float>(1);
            state.at<float>(2) = 0;
            state.at<float>(3) = 0;
            state.at<float>(4) = meas.at<float>(2);
            state.at<float>(5) = meas.at<float>(3);
            // <<<< Initialization

            kf.statePost = state;

            found = true;
        }
        else
            kf.correct(meas); // Kalman Correction
    }
    // <<<<< Kalman Update
    normalize_coordinates(foundResult, ball_pos_x, ball_pos_y);
}
void normalize_coordinates(const cv::Point& resultPoint, uint16_t* ball_pos_x, uint16_t* ball_pos_y)
{
    /*To map
    [A, B] --> [a, b]
    [0,390]       --> [0,664]
    use this formula (val - A)*(b-a)/(B-A) + a
     */

    int calculated_y_pos = 390 - (foundResult.y - 47);
    calculated_y_pos = (calculated_y_pos) * (FIELD_Y_LENGTH) / 390;

    if(calculated_y_pos < 0 )
    {
        *ball_pos_y = 0;
    }
    else if(calculated_y_pos  > FIELD_Y_LENGTH )
    {
        *ball_pos_y = FIELD_Y_LENGTH;
    }
    else
    {
        *ball_pos_y = calculated_y_pos;
    }

    int calculated_x_pos = foundResult.x - 11;
    if(calculated_x_pos < 0 ){
        *ball_pos_x = 0;
    }
    else{
        *ball_pos_x = calculated_x_pos;
    }
}