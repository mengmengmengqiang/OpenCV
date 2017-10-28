#include "iostream"
#include "highgui.hpp"
#include "core.hpp"
#include "imgproc.hpp"
#include "features2d.hpp"
#include "calib3d.hpp"
#include "video.hpp"
#include "objdetect.hpp"
#include "ComputerVision-V3.0.h"

using namespace cv;

int main(void)
{
	VideoCapture capture(0);	//创建一个相机 ID为0
	Mat image;					//创建一帧图片空间
	Mat image_ROI;				//创建一个ROI兴趣空间
	///namedWindow("out");			//创建一个窗口
	while (1)
	{
		int get_position = 0;
		int get_dirction = 0;
		int turn_dirction = 0;
		int warnning = warnning_NO;
		char *turn = "NO turn";
		if (!capture.read(image))	//将相机一帧图像保存至图片空间中
			break;
		image_ROI = image(Range ROI_between_Y, Range ROI_between_X);		//设置兴趣空间大小
		//change_image_dirction(image_ROI);	//将图像水平反向（发现电脑摄像头图像是反的）
		binaryzation(image_ROI, image_ROI, binaryzation_strength);		//二值化图像
		///imshow("out", image_ROI);		//显示图像
		//imshow("in", image);
		if (data_process(image_ROI, get_dirction, get_position, 50, turn_dirction))
		{
			if (turn_dirction == turn_right)
				turn = "turn right";
			else if (turn_dirction == turn_left)
				turn = "turn left";
			std::cout <<"dirction："<< get_dirction<<" \t"<< turn <<" \t";
			std::cout <<"position："<< "warnning" << std::endl;
		} 
		else
		{
			/*switch (get_dirction)
			{
			case left_dirction:
				std::cout << "left\t";
				break;
			case right_dirction:
				std::cout << "right\t";
				break;
			case cent_dirction:
				std::cout << "cent\t";
				break;
			}*/
			if (turn_dirction == turn_right)
				turn = "turn right";
			else if (turn_dirction == turn_left)
				turn = "turn left";
			std::cout << "dirction：" << get_dirction << " \t" << turn << " \t";
			std::cout << "position：" << get_position << std::endl;
		}
		imwrite("F:\\Project\\vs\\ComputerVision V3.0\\save.jpg", image_ROI);
		waitKey(1);
	}
	return 0;
}

