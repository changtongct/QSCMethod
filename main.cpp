#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <time.h>
#include "commonfile.h"
#include "QSCMethod_4.h"

//int flag[M][N]={0};
 //f16.bmp f16XXXgray.bmp f16XXXstc.bmp 10
int main(int argc,char** argv)//�ֱ�����1.ԭͼ 2.ת�Ҷ�ͼ�ļ��� 3.�������ļ��� 4.EPSILON

{
   IplImage* img1;
   if( argc == 5 && (img1 = cvLoadImage( argv[1], 0)) != 0 )   //��Դ��ɫͼ��imgת����Ŀ���ɫͼ�����
   {
		cout << "ͼƬ�ѱ���ȷ��ȡ����������Ҳ��ȷ��" << endl;

		int k=0;//�ָ�������
		unsigned int time1,time2;
		const char* original_img = argv[1];
		//////////////////////////////////////////��ʾԭʼͼ��
		IplImage* colorimg = cvLoadImage(original_img);
		cvNamedWindow("ԭͼ",CV_WINDOW_AUTOSIZE);
		cvShowImage("ԭͼ",colorimg);
		//////////////////////////////////////////��ʾ�Ҷ�ͼ�񲢱���
		IplImage* grayimg = cvCreateImage(cvGetSize(colorimg), IPL_DEPTH_8U, 1);
		cvCvtColor(colorimg,grayimg,CV_BGR2GRAY);
		cvNamedWindow("�Ҷ�ͼ",CV_WINDOW_AUTOSIZE);
		cvShowImage("�Ҷ�ͼ",grayimg);
		const char* gray_img = argv[2];
		cvSaveImage(gray_img,grayimg);

		IplImage* segmented_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		cvSet(segmented_img,cvScalar(255));
		IplImage* decoded_img = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
		double Threshold = atof(argv[4]);
		printf("�ָ���ֵ��%f\n",Threshold);
		time1 = clock();
		k = STCMethod_4( grayimg, segmented_img, Threshold);//����ͼ��ָ��
		Decoding(Linear_tree_table, Color_table, decoded_img, decoded_img->width, decoded_img->height);//����ָ�ͼƬ
		time2 = clock();

		const char* d_img = argv[3];
		cvSaveImage(d_img,decoded_img);
		printf("����ʱ��: %u ms\n",time2-time1);
		printf("�ָ������%d\n",k);
		double bpp;
		bpp = BPP( Linear_tree_table, Color_table, decoded_img->width, decoded_img->height);
		printf("�������BPP��%f λ\n",bpp);
		double psnr;
		psnr = PSNR( grayimg ,decoded_img, decoded_img->width, decoded_img->height);
		printf("��ֵ�����PSNR��%f db\n",psnr);
		double cr;
		cr = CR( grayimg, Linear_tree_table, Color_table);
		printf("ͼ��ѹ����CR��%f\n",cr);
		cvNamedWindow("ͼ�и���",CV_WINDOW_AUTOSIZE);
		cvShowImage("ͼ�и���",segmented_img);	
		cvNamedWindow("����ָ����ͼ",CV_WINDOW_AUTOSIZE);
		cvShowImage("����ָ����ͼ",decoded_img);	

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
	cout << "ͼƬ�����ڻ��������ȷ��"<< endl;
	return -1;
	}
}