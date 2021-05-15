#include <opencv2/video/tracking.hpp>
#include "ball_tracker.hpp"
#include "../game_control/game_control.hpp"

const bool predictKick = true;

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

double ticks = 0;
bool ballFound = false;
int notFoundCount = 0;

cv::Point foundResult_ball;
cv::Point predictResult_ball;
vector <int> snt_red_positions(10);
vector <int> gk_red_positions(10);

uint16_t last_pos_human_snt = 0;
uint16_t last_pos_human_gk = 0;

//ideoWriter video("/home/pi/Desktop/out.avi",VideoWriter::fourcc('M','J','P','G'),10, Size(640,480),true);

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
void get_ball_and_arm_positions(uint16_t* ball_pos_x, uint16_t* ball_pos_y, uint16_t* arm_human_gk_position, uint16_t* arm_human_snt_position)
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

    if (ballFound)
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
        predictResult_ball.x = center.x;
        predictResult_ball.y = center.y;

        // Todo: think this algorithm.
        if (predictKick)
        {
            int servo = is_kick_need(center.x);
            if(servo == SERVO_SNT){
                int pulse_width = get_servo_pulsewidth(Global.pi, SERVO_SNT_SIGNAL_PIN);
                if(SERVO_SNT_HOME_POSITION - SERVO_KICK_DISTANCE != pulse_width)
                    trigger_servo_kick(servo);
            }
            else if(servo == SERVO_GK){
                int pulse_width = get_servo_pulsewidth(Global.pi, SERVO_GK_SIGNAL_PIN);
                if(SERVO_GK_HOME_POSITION - SERVO_KICK_DISTANCE != pulse_width)
                    trigger_servo_kick(servo);
            }
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
    cv::Mat rangeRes_ball = cv::Mat::zeros(frame.size(), CV_8UC1);
    cv::Mat rangeRes_red_arm = cv::Mat::zeros(frame.size(), CV_8UC1);

    cv::inRange(frmHsv, cv::Scalar(90, 100, 60),
                cv::Scalar(120 , 255, 255), rangeRes_ball);

    cv::inRange(frmHsv, cv::Scalar(20, 0, 0),
                cv::Scalar(135 , 225, 255), rangeRes_red_arm);
    cv::bitwise_not(rangeRes_red_arm, rangeRes_red_arm);
    // <<<<< Color Thresholding

    // >>>>> Improving the result
    cv::erode(rangeRes_ball, rangeRes_ball, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(rangeRes_ball, rangeRes_ball, cv::Mat(), cv::Point(-1, -1), 2);

    cv::erode(rangeRes_red_arm, rangeRes_red_arm, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(rangeRes_red_arm, rangeRes_red_arm, cv::Mat(), cv::Point(-1, -1), 2);
    // <<<<< Improving the result

    // >>>>> Contours detection
    vector<vector<cv::Point> > contours_for_ball;
    vector<vector<cv::Point> > contours_for_red_arm;

    cv::findContours(rangeRes_ball, contours_for_ball, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_NONE);
    cv::findContours(rangeRes_red_arm, contours_for_red_arm, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_NONE);


    // <<<<< Contours detection

    // >>>>> Filtering
    vector<vector<cv::Point>> balls;
    vector<vector<cv::Point>> red_players;
    vector<cv::Rect> ballsBox;
    vector<cv::Rect> red_player_boxes;
    // Ball detection drawing
    for (size_t i = 0; i < contours_for_ball.size(); i++)
    {
        cv::Rect bBox;
        bBox = cv::boundingRect(contours_for_ball[i]);

        float ratio = (float) bBox.width / (float) bBox.height;
        if (ratio > 1.0f)
            ratio = 1.0f / ratio;

        // Searching for a bBox almost square
        if (ratio > 0.75 && bBox.area() >= 400)
        {
            balls.push_back(contours_for_ball[i]);
            ballsBox.push_back(bBox);
        }
    }

    // Red arm detection drawing.
    for (size_t i = 0; i < contours_for_red_arm.size(); i++)
    {
        cv::Rect bBox;
        bBox = cv::boundingRect(contours_for_red_arm[i]);

        float ratio = (float) bBox.height / (float) bBox.width;

        // Searching for a bBox almost square
        if (ratio > 0.2 && ratio < 3)
        {
            if (bBox.area() <= 5000 && bBox.area() >= 100)
            {
                red_players.push_back(contours_for_red_arm[i]);
                red_player_boxes.push_back(bBox);
            }
        }
    }

    // >>>>> Ball Detection result
    for (size_t i = 0; i < balls.size(); i++)
    {
        cv::drawContours(res, balls, i, CV_RGB(20,150,20), 1);
        cv::rectangle(res, ballsBox[i], CV_RGB(0,255,0), 2);

        cv::Point center;
        center.x = ballsBox[i].x + ballsBox[i].width / 2;
        center.y = ballsBox[i].y + ballsBox[i].height / 2;
        cv::circle(res, center, 2, CV_RGB(20,150,20), -1);

        foundResult_ball.x = center.x;
        foundResult_ball.y = center.y;

        stringstream sstr;
        sstr << "(" << center.x << "," << center.y << ")";
        cv::putText(res, sstr.str(),
                    cv::Point(center.x + 3, center.y - 3),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
    }

    // >>>>> Red Arm Detection result
    for (size_t i = 0; i < red_players.size(); i++)
    {
        cv::Point center;
        center.x = red_player_boxes[i].x + red_player_boxes[i].width / 2;
        center.y = red_player_boxes[i].y + red_player_boxes[i].height / 2;

        // store Red arm GK player positions.
        if(center.x > 600 && center.x < 610)
        {
            gk_red_positions.push_back(center.y);
            cv::drawContours(res, red_players, i, CV_RGB(20,150,20), 1);
            cv::rectangle(res, red_player_boxes[i], CV_RGB(0,255,0), 2);
            cv::circle(res, center, 2, CV_RGB(20,150,20), -1);

            stringstream sstr;
            sstr << "(" << center.x << "," << center.y << ")";
            cv::putText(res, sstr.str(),
                        cv::Point(center.x + 3, center.y - 3),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
        }

        // store Red arm SNT player positions.
        if(center.x > 255 && center.x < 265)
        {
            snt_red_positions.push_back(center.y);
            cv::drawContours(res, red_players, i, CV_RGB(20,150,20), 1);
            cv::rectangle(res, red_player_boxes[i], CV_RGB(0,255,0), 2);
            cv::circle(res, center, 2, CV_RGB(20,150,20), -1);

            stringstream sstr;
            sstr << "(" << center.x << "," << center.y << ")";
            cv::putText(res, sstr.str(),
                        cv::Point(center.x + 3, center.y - 3),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
        }
    }

    // >>>>> Ball detection Kalman Update
    if (balls.size() == 0)
    {
        notFoundCount++;
        if( notFoundCount >= 100 )
        {
            ballFound = false;
            pthread_mutex_lock(&Global.ball_warning_mutex);
            Global.ball_not_found = 1;
            pthread_cond_signal(&Global.ball_warning_condvar);
            pthread_mutex_unlock(&Global.ball_warning_mutex);
        }
    }
    else
    {
        /* Turn Off Led */
        pthread_mutex_lock(&Global.ball_warning_mutex);
        Global.ball_not_found = 0;
        pthread_mutex_unlock(&Global.ball_warning_mutex);

        // Todo: Handle not ballFound situation. Use global for this state.
        notFoundCount = 0;

        meas.at<float>(0) = ballsBox[0].x + ballsBox[0].width / 2;
        meas.at<float>(1) = ballsBox[0].y + ballsBox[0].height / 2;
        meas.at<float>(2) = (float)ballsBox[0].width;
        meas.at<float>(3) = (float)ballsBox[0].height;

        if (!ballFound) // First detection!
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

            ballFound = true;
        }
        else
            kf.correct(meas); // Kalman Correction
    }
    //video.write(res);
    // <<<<< Kalman Update
    normalize_coordinates(foundResult_ball, ball_pos_x, ball_pos_y, arm_human_gk_position, arm_human_snt_position);
}

void normalize_coordinates(const cv::Point& resultPoint, uint16_t* ball_pos_x, uint16_t* ball_pos_y, uint16_t* arm_human_gk_position , uint16_t* arm_human_snt_position)
{
    int calculated_y_pos_ball = foundResult_ball.y;
    *ball_pos_y = 435 - (calculated_y_pos_ball - 45);

    if(gk_red_positions.size() == 3)
    {
        printf("Test1...\n");
        sort(gk_red_positions.begin(), gk_red_positions.end());
        int calculated_y_pos_human_gk = gk_red_positions.at(1);
        *arm_human_gk_position = 435 - (calculated_y_pos_human_gk - 45);
        last_pos_human_gk = *arm_human_gk_position;
    }
    else
    {
        *arm_human_gk_position = last_pos_human_gk;
    }

    if(snt_red_positions.size() == 3)
    {
        printf("Test1...\n");
        sort(snt_red_positions.begin(), snt_red_positions.end());
        int calculated_y_pos_human_snt = snt_red_positions.at(1);
        *arm_human_snt_position = 435 - (calculated_y_pos_human_snt - 45);
        last_pos_human_snt = *arm_human_snt_position;
    }
    else
    {
        *arm_human_snt_position = last_pos_human_snt;
    }

    int calculated_x_pos = foundResult_ball.x - 11;
    if(calculated_x_pos < 0 ){
        *ball_pos_x = 0;
    }
    else{
        *ball_pos_x = calculated_x_pos;
    }

    gk_red_positions.clear();
    snt_red_positions.clear();
}