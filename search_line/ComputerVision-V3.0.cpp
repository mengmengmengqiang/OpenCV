#include "ComputerVision V3.0.h"
void binaryzation(cv::Mat image, cv::Mat &result, int min_val = 0)
{
	result.create(image.size(), CV_8U);		//分配图片空间
	cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();	//迭代扫描图像
	cv::Mat_<cv::Vec3b>::const_iterator it_end = image.end<cv::Vec3b>();
	cv::Mat_<uchar>::iterator it_out = result.begin<uchar>();
	for (; it != it_end; ++it, ++it_out)
	{

		if ((*it)[0] > min_val || (*it)[1] > min_val || (*it)[2] > min_val)
		{
			*it_out = white_point;
		}
		else
		{
			*it_out = black_piont;
		}
	}
}

#define Fine_left	1
#define Fine_right	0
#define Fine_compelet	2


int get_state(cv::Mat image, int &dirction, int &position, int position_threshhold,int &turn_dirction)	//核心
{
	int diagonal_Lup = 0;	//左上至右下
	int diagonal_Rup = 0;	//右上至左下
	bool piont_mapping1 = 0;
	bool piont_mapping2 = 0;
	int add_X[ROI_size] = { 0 };	//存储Y轴在X轴叠加数据
	int add_Y[turnsigne_depth] = { 0 };	//储存最顶上n个像素的值作为拐弯标记
	int get_X1, get_X2;		//目标左端点，目标右端点
	int shift = Fine_left;		//切换左右端点标志位
	int tmp;
	int turnsign_count = 0;
	for (int countY = 0; countY < image.cols; countY++)
	{
		if (image.at<uchar>(countY, countY) == 0)
		{
			piont_mapping1 = 0;
		}
		else
		{
			piont_mapping1 = 1;
		}
		diagonal_Lup += piont_mapping1;		//对角线叠加


		if (image.at<uchar>(image.cols - 1 - countY, countY) == 0)
		{
			piont_mapping2 = 0;
		}
		else
		{
			piont_mapping2 = 1;
		}
		diagonal_Rup += piont_mapping2;		//对角线叠加


		for (int countX = 0; countX < image.rows; countX++)
		{
			if (image.at<uchar>(countY, countX) == 0)	//Y轴在X轴叠加
			{
				add_X[countX]++;
				if (countY<turnsigne_depth)
				{
					add_Y[countY]++;
				}
			}
		}
	}
	//if (diagonal_Rup - diagonal_Lup > 0)		//左右倾斜判断
	//	dirction = left_dirction;
	//else if (diagonal_Rup - diagonal_Lup < 0)
	//	dirction = right_dirction;
	//else
	//	dirction = cent_dirction;
	dirction = diagonal_Rup - diagonal_Lup;

	//强制屏蔽低于兴趣值的点
	for (int i = 0; i < ROI_size; i++)
	{
		if (add_X[i] < position_threshhold)		//强制滤除X轴上小于阈值的点
		{
			add_X[i] = 0;
		}
		if (add_Y[i] < turnsigned_mask && i < turnsigne_depth)	//强制滤除Y轴上小于阈值的点
		{
			add_Y[i] = 0;
		}
	}

	//识别拐弯信号
	for (int i = 0; i < turnsigne_depth; i++)
	{
		if (add_Y[i] == 0)
		{
			turnsign_count++;
		}
		if (i == turnsigne_depth -1)
		{
			if (turnsign_count == turnsigne_depth)
			{
				turn_dirction = get_turn_dirction(image, position_threshhold);
			}
			else
			{
				turn_dirction = turn_NO;
			}
		}
	}

	//识别直线位置
	if (add_X[0] || add_X[image.rows - 1])		//如果左边界或右边界有大于阈值的点 就返回错误
	{
		return warnning_YES;
	}
	for (int i = 0; i < ROI_size; i++)
	{
		if (add_X[i] > 0 && shift== Fine_left)	//判断左端点
		{
			get_X1 = i;
			tmp = add_X[i];
			shift = Fine_right;
		}
		if (add_X[i]==0 && shift== Fine_right)	//判断右端点
		{
			get_X2 = i;
			shift = Fine_compelet;
		}
		position = (get_X2+ get_X1- ROI_size)>>1;	//目标坐标
	}

	return warnning_NO;
}

int get_turn_dirction(cv::Mat image,int sign_mask)
{
	int add_X[turnsigne_depth] = { 0 };
	int turnsign_count = 0;
	for (int countY = 0; countY < image.cols; countY++)	//Y轴在X轴上叠加
	{
		for (int countX = 0; countX < turnsigne_depth; countX++)
		{
			if (image.at<uchar>(countY, countX) == 0)
			{
				add_X[countX]++;;
			}
		}
	}
	for (int i = 0; i < turnsigne_depth; i++)	//强制滤除小于阈值的点
	{
		if (add_X[i]<sign_mask)
		{
			add_X[i] = 0;
		}
	}
	for (int i = 0; i < turnsigne_depth; i++)
	{
		if (add_X[i] == 0)
		{
			turnsign_count++;
		}
	}
	if (turnsign_count == turnsigne_depth)
	{
		return turn_right;
	}
	else if (turnsign_count == 0)
	{
		return turn_left;
	}
	else
		return turn_NO;
}


void change_image_dirction(cv::Mat &image)
{
	cv::Mat srcX(image.rows, image.cols, CV_32F);	//x方向映射
	cv::Mat srcY(image.rows, image.cols, CV_32F);	//y方向映射
												//创建映射参数
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			srcX.at<float>(i, j) = (float)image.cols - 1 - j;	//水平翻转
			srcY.at<float>(i, j) = (float)i;
		}
	}
	cv::remap(image, image, srcX, srcY, cv::INTER_LINEAR);	//应用映射参数
}


int data_process(cv::Mat image_ROI, int &get_dirction, int &get_position, int position_threshhold,int &turn_dirction)
{
	int real_dirction = 0;
	int real_position=0;
	int warnning_count=0;
	for (int count = 0; count < value_average_depth; count++)	//数据求均值
	{
		warnning_count += get_state(image_ROI, real_dirction, real_position, position_threshhold, turn_dirction);	//获取状态
		get_dirction += real_dirction;
		get_position += real_position;		//叠加缓存位置信息
	}
	get_dirction /= value_average_depth;
	if (warnning_count > value_average_depth>>1)//如果半数数据出错 返回错误
	{
		return warnning_YES;
	}
	get_position /= (value_average_depth - warnning_count);
	return warnning_NO;
}

//int data_process(cv::Mat image_ROI, int &get_dirction, int &get_position, int position_threshhold)
//{
//	int left = 0;
//	int right = 0;
//	int cent = 0;
//	int real_position = 0;
//	int warnning_count = 0;
//	for (int count = 0; count < value_average_depth; count++)	//数据求均值
//	{
//		warnning_count += get_state(image_ROI, get_dirction, real_position, position_threshhold);	//获取状态
//		if (get_dirction == left_dirction)
//			left++;
//		else if (get_dirction == right_dirction)
//			right++;
//		else
//			cent++;
//		get_position += real_position;		//叠加缓存位置信息
//	}
//	if (left > right && left > cent)		//打印方向信息
//		get_dirction = left_dirction;
//	else if (right > left && right > cent)
//		get_dirction = right_dirction;
//	else if (cent > left && cent > right)
//		get_dirction = cent_dirction;
//	get_position /= value_average_depth;
//	if (warnning_count > value_average_depth / 2)//如果半数数据出错 返回错误
//	{
//		return warnning_YES;
//	}
//	return warnning_NO;
//}

