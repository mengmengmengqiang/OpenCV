#include "ComputerVision V3.0.h"
void binaryzation(cv::Mat image, cv::Mat &result, int min_val = 0)
{
	result.create(image.size(), CV_8U);		//����ͼƬ�ռ�
	cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();	//����ɨ��ͼ��
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


int get_state(cv::Mat image, int &dirction, int &position, int position_threshhold,int &turn_dirction)	//����
{
	int diagonal_Lup = 0;	//����������
	int diagonal_Rup = 0;	//����������
	bool piont_mapping1 = 0;
	bool piont_mapping2 = 0;
	int add_X[ROI_size] = { 0 };	//�洢Y����X���������
	int add_Y[turnsigne_depth] = { 0 };	//�������n�����ص�ֵ��Ϊ������
	int get_X1, get_X2;		//Ŀ����˵㣬Ŀ���Ҷ˵�
	int shift = Fine_left;		//�л����Ҷ˵��־λ
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
		diagonal_Lup += piont_mapping1;		//�Խ��ߵ���


		if (image.at<uchar>(image.cols - 1 - countY, countY) == 0)
		{
			piont_mapping2 = 0;
		}
		else
		{
			piont_mapping2 = 1;
		}
		diagonal_Rup += piont_mapping2;		//�Խ��ߵ���


		for (int countX = 0; countX < image.rows; countX++)
		{
			if (image.at<uchar>(countY, countX) == 0)	//Y����X�����
			{
				add_X[countX]++;
				if (countY<turnsigne_depth)
				{
					add_Y[countY]++;
				}
			}
		}
	}
	//if (diagonal_Rup - diagonal_Lup > 0)		//������б�ж�
	//	dirction = left_dirction;
	//else if (diagonal_Rup - diagonal_Lup < 0)
	//	dirction = right_dirction;
	//else
	//	dirction = cent_dirction;
	dirction = diagonal_Rup - diagonal_Lup;

	//ǿ�����ε�����Ȥֵ�ĵ�
	for (int i = 0; i < ROI_size; i++)
	{
		if (add_X[i] < position_threshhold)		//ǿ���˳�X����С����ֵ�ĵ�
		{
			add_X[i] = 0;
		}
		if (add_Y[i] < turnsigned_mask && i < turnsigne_depth)	//ǿ���˳�Y����С����ֵ�ĵ�
		{
			add_Y[i] = 0;
		}
	}

	//ʶ������ź�
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

	//ʶ��ֱ��λ��
	if (add_X[0] || add_X[image.rows - 1])		//�����߽���ұ߽��д�����ֵ�ĵ� �ͷ��ش���
	{
		return warnning_YES;
	}
	for (int i = 0; i < ROI_size; i++)
	{
		if (add_X[i] > 0 && shift== Fine_left)	//�ж���˵�
		{
			get_X1 = i;
			tmp = add_X[i];
			shift = Fine_right;
		}
		if (add_X[i]==0 && shift== Fine_right)	//�ж��Ҷ˵�
		{
			get_X2 = i;
			shift = Fine_compelet;
		}
		position = (get_X2+ get_X1- ROI_size)>>1;	//Ŀ������
	}

	return warnning_NO;
}

int get_turn_dirction(cv::Mat image,int sign_mask)
{
	int add_X[turnsigne_depth] = { 0 };
	int turnsign_count = 0;
	for (int countY = 0; countY < image.cols; countY++)	//Y����X���ϵ���
	{
		for (int countX = 0; countX < turnsigne_depth; countX++)
		{
			if (image.at<uchar>(countY, countX) == 0)
			{
				add_X[countX]++;;
			}
		}
	}
	for (int i = 0; i < turnsigne_depth; i++)	//ǿ���˳�С����ֵ�ĵ�
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
	cv::Mat srcX(image.rows, image.cols, CV_32F);	//x����ӳ��
	cv::Mat srcY(image.rows, image.cols, CV_32F);	//y����ӳ��
												//����ӳ�����
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			srcX.at<float>(i, j) = (float)image.cols - 1 - j;	//ˮƽ��ת
			srcY.at<float>(i, j) = (float)i;
		}
	}
	cv::remap(image, image, srcX, srcY, cv::INTER_LINEAR);	//Ӧ��ӳ�����
}


int data_process(cv::Mat image_ROI, int &get_dirction, int &get_position, int position_threshhold,int &turn_dirction)
{
	int real_dirction = 0;
	int real_position=0;
	int warnning_count=0;
	for (int count = 0; count < value_average_depth; count++)	//�������ֵ
	{
		warnning_count += get_state(image_ROI, real_dirction, real_position, position_threshhold, turn_dirction);	//��ȡ״̬
		get_dirction += real_dirction;
		get_position += real_position;		//���ӻ���λ����Ϣ
	}
	get_dirction /= value_average_depth;
	if (warnning_count > value_average_depth>>1)//����������ݳ��� ���ش���
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
//	for (int count = 0; count < value_average_depth; count++)	//�������ֵ
//	{
//		warnning_count += get_state(image_ROI, get_dirction, real_position, position_threshhold);	//��ȡ״̬
//		if (get_dirction == left_dirction)
//			left++;
//		else if (get_dirction == right_dirction)
//			right++;
//		else
//			cent++;
//		get_position += real_position;		//���ӻ���λ����Ϣ
//	}
//	if (left > right && left > cent)		//��ӡ������Ϣ
//		get_dirction = left_dirction;
//	else if (right > left && right > cent)
//		get_dirction = right_dirction;
//	else if (cent > left && cent > right)
//		get_dirction = cent_dirction;
//	get_position /= value_average_depth;
//	if (warnning_count > value_average_depth / 2)//����������ݳ��� ���ش���
//	{
//		return warnning_YES;
//	}
//	return warnning_NO;
//}

