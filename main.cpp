#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <time.h>
#include "commonfile.h"
#include "QSCMethod_4.h"

//int flag[M][N]={0};
 //f16.bmp f16XXXgray.bmp f16XXXstc.bmp 10
int main(int argc,char** argv)//分别输入1.原图 2.转灰度图文件名 3.解码后的文件名 4.EPSILON

{
   IplImage* img1;
   if( argc == 5 && (img1 = cvLoadImage( argv[1], 0)) != 0 )   //将源彩色图像img转化成目标灰色图像读入
   {
		cout << "图片已被正确读取，参数数量也正确！" << endl;

		int k=0;//分割块的数量
		unsigned int time1,time2;
		const char* original_img = argv[1];
		//////////////////////////////////////////显示原始图像
		IplImage* colorimg = cvLoadImage(original_img);
		cvNamedWindow("原图",CV_WINDOW_AUTOSIZE);
		cvShowImage("原图",colorimg);
		//////////////////////////////////////////显示灰度图像并保存
		IplImage* grayimg = cvCreateImage(cvGetSize(colorimg), IPL_DEPTH_8U, 1);
		cvCvtColor(colorimg,grayimg,CV_BGR2GRAY);
		cvNamedWindow("灰度图",CV_WINDOW_AUTOSIZE);
		cvShowImage("灰度图",grayimg);
		const char* gray_img = argv[2];
		cvSaveImage(gray_img,grayimg);

		IplImage* segmented_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		cvSet(segmented_img,cvScalar(255));
		IplImage* decoded_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		double Threshold = atof(argv[4]);
		printf("分割阈值：%f\n",Threshold);
		time1 = clock();
		k = STCMethod_4( grayimg, segmented_img, Threshold);//调用图像分割函数
		Decoding(Linear_tree_table, Color_table, decoded_img, decoded_img->width, decoded_img->height);//解码恢复图片
		time2 = clock();

		const char* d_img = argv[3];
		cvSaveImage(d_img,decoded_img);
		printf("花费时间: %u ms\n",time2-time1);
		printf("分割块数：%d\n",k);
		double bpp;
		bpp = BPP( Linear_tree_table, Color_table, decoded_img->width, decoded_img->height);
		printf("像素深度BPP：%f 位\n",bpp);
		double psnr;
		psnr = PSNR( grayimg ,decoded_img, decoded_img->width, decoded_img->height);
		printf("峰值信噪比PSNR：%f db\n",psnr);
		double cr;
		cr = CR( grayimg, Linear_tree_table, Color_table);
		printf("图像压缩比CR：%f\n",cr);
		cvNamedWindow("图切割结果",CV_WINDOW_AUTOSIZE);
		cvShowImage("图切割结果",segmented_img);	
		cvNamedWindow("解码恢复后的图",CV_WINDOW_AUTOSIZE);
		cvShowImage("解码恢复后的图",decoded_img);	

		cvWaitKey(0);
		cvReleaseImage(&colorimg);
		cvReleaseImage(&grayimg);
		cvReleaseImage(&segmented_img);
		cvReleaseImage(&decoded_img);
		cvDestroyAllWindows();

		return 0;
	}
    else
	{
	cout << "图片不存在或参数不正确！"<< endl;
	return -1;
	}
}