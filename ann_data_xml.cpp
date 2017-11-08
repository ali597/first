#include "opencv2/opencv.hpp"

#include <iostream>
using namespace cv;
using namespace std;

void features(const Mat & in, Mat & out, int sizeData);  //���һ���ַ������Ӧ����������
Mat projectHistogram(const Mat& img, int t);             //����ˮƽ���ۼ�ֱ��ͼ��ȡ����tΪ0����1

int main()
{
	FileStorage fs("ann_data_101111.xml", FileStorage::WRITE); ;
	if (!fs.isOpened())
	{
		std::cerr << "failed to open " << std::endl;
	}

	Mat  trainData;
	Mat classes = Mat::zeros(1,1700, CV_32FC1);
	char path[90];
	Mat img_read;
	for (int i = 0; i<34; i++)  //��i��
	{
		for (int j = 1; j< 51; ++j)  //i���е�j��
		{
			sprintf(path, "E:\\cvtest\\CarNumber\\charSamples\\%d\\%d (%d).png", i, i, j);
			img_read = imread(path, -1);

			Mat img_threshold;
			threshold(img_read, img_threshold, 180, 255, CV_THRESH_BINARY);

			Mat dst_mat;
			Mat train_mat(2, 3, CV_32FC1);
			int length;
			Point2f srcTri[3];
			Point2f dstTri[3];

			srcTri[0] = Point2f(0, 0);
			srcTri[1] = Point2f(img_threshold.cols - 1, 0);
			srcTri[2] = Point2f(0, img_threshold.rows - 1);
			length = img_threshold.rows > img_threshold.cols ? img_threshold.rows : img_threshold.cols;
			dstTri[0] = Point2f(0.0, 0.0);
			dstTri[1] = Point2f(length, 0.0);
			dstTri[2] = Point2f(0.0, length);
			train_mat = getAffineTransform(srcTri, dstTri);
			dst_mat = Mat::zeros(length, length, img_threshold.type());
			warpAffine(img_threshold, dst_mat, train_mat, dst_mat.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
			resize(dst_mat, dst_mat, Size(20, 20));  //�ߴ����Ϊ20*20

			Mat dst_feature;
			features(dst_mat, dst_feature, 10); //����1*440    65 140

			trainData.push_back(dst_feature);
			classes.at<float>(i * 50 + j - 1) = i;
		}
	}

	fs << "TrainingData" << trainData;
	fs << "classes" << classes;
	fs.release();
}

//ˮƽ��ֱֱ��ͼ,0Ϊ����ͳ��
void features(const Mat & in, Mat & out, int sizeData)
{
	Mat vhist = projectHistogram(in, 1); //ˮƽֱ��ͼ
										 //cv::imshow("asdwd", vhist);
										 //waitKey(0);
	Mat hhist = projectHistogram(in, 0);  //��ֱֱ��ͼ

	Mat lowData;
	resize(in, lowData, Size(sizeData, sizeData));
	int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;
	out = Mat::zeros(1, numCols, CV_32F);

	int j = 0;
	for (int i = 0; i<vhist.cols; ++i)
	{
		out.at<float>(j) = vhist.at<float>(i);
		j++;
	}
	for (int i = 0; i < hhist.cols; ++i)
	{
		out.at<float>(j) = hhist.at<float>(i);
		j++;
	}
	for (int x = 0; x<lowData.rows; ++x)
	{
		for (int y = 0; y < lowData.cols; ++y)
		{
			out.at<float>(j) = (float)lowData.at<unsigned char>(x, y);
			j++;
		}
	}

}
//0Ϊ����ͳ�� 1Ϊ����ͳ��
Mat projectHistogram(const Mat& img, int t)  //ˮƽ��ֱֱ��ͼ,0Ϊ����ͳ��
{                                            //1Ϊ����ͳ��
	int sz = (t) ? img.rows : img.cols;
	Mat mhist = Mat::zeros(1, sz, CV_32F);

	for (int j = 0; j < sz; j++)
	{
		Mat data = (t) ? img.row(j) : img.col(j);
		mhist.at<float>(j) = countNonZero(data);
	}

	double min, max;
	minMaxLoc(mhist, &min, &max);

	if (max > 0)
		mhist.convertTo(mhist, -1, 1.0f / max, 0);

	return mhist;
}

//'��', '��', '��', '��', '��', '��', '��', '��', '��', '��', '��', '��',
//'��', '��', '³', '��', '��', '��', '��', '��', '��', '��', '��', '��',
//'��', '��', 'ԥ', '��', '��', '��', '��'