#ifndef COMPUTERVISION
#define COMPUTERVISION


#include "iostream"
#include "highgui.hpp"
#include "core.hpp"
#include "imgproc.hpp"
#include "features2d.hpp"
#include "calib3d.hpp"
#include "video.hpp"
#include "objdetect.hpp"



#define X_point					640
#define Y_point					480
#define ROI_size				200
#define ROI_between_X			((X_point-ROI_size)>>1,(X_point+ROI_size)>>1)
#define ROI_between_Y			((Y_point-ROI_size)>>1,(Y_point+ROI_size)>>1)
#define binaryzation_strength	150
#define value_average_depth		13
#define black_piont				0
#define white_point				225
#define left_dirction			1
#define right_dirction			-1
#define cent_dirction			0
#define position_error			2
#define warnning_NO				0
#define warnning_YES			1
#define turnsigne_depth			20
#define turnsigned_mask			50
#define turn_left				-1
#define turn_right				1
#define turn_NO					0

void binaryzation(cv::Mat image, cv::Mat &result, int min_val);
int get_state(cv::Mat image, int &dirction, int &position, int position_threshhold, int &turn_dirction);
void change_image_dirction(cv::Mat &image);
int data_process(cv::Mat image_ROI, int &get_dirction, int &get_position, int position_threshholdint,int &turn_dirction);
int get_turn_dirction(cv::Mat image, int sign_mask);


#endif
