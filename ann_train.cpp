//#include<opencv2/opencv.hpp>  
//#include <iostream>    
//#include <string>    
//
//using namespace std;
//using namespace cv;
//using namespace ml;
//int main()
//{
//	Ptr<ANN_MLP> bp;
//	int nlayers=300;//每层的神经元数
//	int numCharacters = 34;//输出层个数
//
//	Mat trainData, classes;
//	FileStorage fs;
//	fs.open("ann_data_10.xml", FileStorage::READ);
//	fs["TrainingData"] >> trainData;
//	fs["classes"] >> classes;
//
//	bp = ANN_MLP::create();
//	//Mat layerSizes = (Mat_<float>(1, 3,CV_32FC1) << trainData.cols, nlayers, numCharacters);
//	Mat layerSizes(1, 5, CV_32F);
//	layerSizes.at<float>(0) = trainData.cols;
//	layerSizes.at<float>(1) = 80;
//	layerSizes.at<float>(2) = 70;
//	layerSizes.at<float>(3) = 50;
//	layerSizes.at<float>(4) = numCharacters;
//
//	bp->setLayerSizes(layerSizes);    //设置层数 
//
//	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM); //设置激励函数  
//	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 50000, FLT_EPSILON));//设置结束条件
//	bp->setTrainMethod(ANN_MLP::TrainingMethods::BACKPROP, 0.0008, 0.9);//设置回馈ANN_MLP::BACKPROP
//
//	Mat trainClasses;
//	trainClasses.create(trainData.rows, numCharacters, CV_32FC1);
//	for (int i = 0; i< trainData.rows; i++)//1700
//	{
//		for (int k = 0; k< trainClasses.cols; k++)//34
//		{
//			if (k == (int)classes.at<float>(i))
//			{
//				trainClasses.at<float>(i, k) = 1;
//			}
//			else
//				trainClasses.at<float>(i, k) = 0;
//		}
//	}
//
//	//Ptr<TrainData> tData = TrainData::create(trainData, trainClasses, classes);
//	bp->train(trainData, ROW_SAMPLE, trainClasses);
//	bp->save("E:/ali_ann_10.xml");
//	return 0;
//}
//
////Mat output(1, 34, CV_64FC1); //1*34矩阵
////bp->predict(char_feature[i], output);
////Point maxLoc;
////double maxVal;
////minMaxLoc(output, 0, &maxVal, 0, &maxLoc);
////char_result[i] = maxLoc.x;