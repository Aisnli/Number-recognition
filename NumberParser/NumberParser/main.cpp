#pragma comment(lib,"vfw32.lib ")
#pragma comment(lib,"comctl32.lib")

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "ml.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Parse.h"
#include "StandardKNN.h"
#include "StandardSVM.h"
#include "MyKNN.h"
using namespace std;
using namespace cv;
clock_t start,finish;
IplImage* imagen;
int red,green,blue;
IplImage* screenBuffer;
int drawing;
int r,last_x, last_y;
int size;

void draw(int x,int y)	//��ͼ
{
	cvCircle(imagen, cvPoint(x,y), r, CV_RGB(red,green,blue), -1, 4, 0);
	screenBuffer=cvCloneImage(imagen);
	cvShowImage( "��д��", screenBuffer );
}
void drawCursor(int x, int y)
{
	//����֮ǰͼ��
	screenBuffer=cvCloneImage(imagen);
	cvCircle(screenBuffer, cvPoint(x,y), r, CV_RGB(0,0,0), 1, 4, 0);	//���õ�
}
void on_mouse( int event, int x, int y, int flags, void* param )	//����¼�
{
	last_x=x;
	last_y=y;
	drawCursor(x,y);
	
	if(event==CV_EVENT_LBUTTONDOWN)	//��갴��
	{
		drawing=1;
		draw(x,y);
	}
	else if(event==CV_EVENT_LBUTTONUP)	//������
	{
		drawing=0;
	}
	else if(event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON)	//������겢�ƶ�
	{
		if(drawing)
			draw(x,y);
	}
}
void PrintBasic()
{
	printf( "                                 ��д����ʶ��\n"
		"��ݼ�: \n"
		"\tr - ���ðװ�\n"
		"\tc - ��ʼʶ��\n"
		"\tESC - ����\n");
}
void PrintWelcome()
{
	printf("			����������Ӧ�õĻ���ѧϰ�㷨��\n"
		"1.�����Լ���KNN�㷨\n"
		"2.opencv��KNN�㷨\n"
		"3.opencv��SVM�㷨\n"
		"ESC���˳�\n" );
}
void doMyKNN()
{
	MyKNN myknn;
	bool out=false;
	while(1)
	{
		if(out)
			break;
		int c;
		cvShowImage( "��д��", screenBuffer );
		c = cvWaitKey(10);
		switch(char(c))
		{
			case 27: 	//esc
				system("cls");
				out=true;
				break;
			case '-':
				if(r>1)
				{
					--r;
					drawCursor(last_x,last_y);
				}
				break;
			case '+':
				++r;
				drawCursor(last_x,last_y);
				break;
			case 'r':
				cvSet(imagen, CV_RGB(255,255,255),NULL);
				drawCursor(last_x,last_y);
				break;
			case 'c':
					IplImage prs_image, *dst;   
					prs_image = preprocessing(imagen, size, size,3);
					float scale = 0.25;
					CvSize dst_cvsize;    
					dst_cvsize.width = imagen->width * scale;  //Ŀ��ͼ��Ŀ�ΪԴͼ����scale��
					dst_cvsize.height = imagen->height * scale; //Ŀ��ͼ��ĸ�ΪԴͼ��ߵ�scale��
					dst = cvCreateImage( dst_cvsize, imagen->depth, imagen->nChannels); //����Ŀ��ͼ��
					cvResize(&prs_image, dst, CV_INTER_LINEAR); //����Դͼ��Ŀ��ͼ��				
					cvSaveImage("out32.bmp", dst);
					myknn.classify();
					break;
		}
	}
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	drawCursor(last_x,last_y);
	PrintWelcome();
}
void doStandardKNN()
{
	StandardKNN sknn;
	bool out=false;
	while(1)
	{
		if(out)
			break;
		int c;
		cvShowImage( "��д��", screenBuffer );
		c = cvWaitKey(10);
		switch(char(c))
		{
			case 27: 	//esc
				system("cls");
				out=true;
				break;
			case '-':
				if(r>1)
				{
					--r;
					drawCursor(last_x,last_y);
				}
				break;
			case '+':
				++r;
				drawCursor(last_x,last_y);
				break;
			case 'r':
				cvSet(imagen, CV_RGB(255,255,255),NULL);
				drawCursor(last_x,last_y);
				break;
			case 'c':
				IplImage* temp;
				temp = cvCreateImage( cvGetSize( imagen ), imagen->depth, 1 );
				cvCvtColor( imagen, temp, CV_BGR2GRAY );
				sknn.classify(temp,1);
				break;
		}
	}
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	drawCursor(last_x,last_y);
	PrintWelcome();
}
void doStandardSVM()
{
	StandardSVM ssvm;
	bool out=false;
	while(1)
	{
		if(out)
			break;
		int c;
		cvShowImage( "��д��", screenBuffer );
		c = cvWaitKey(10);
		switch(char(c))
		{
			case 27: 	//esc
				system("cls");
				out=true;
				break;
			case '-':
				if(r>1)
				{
					--r;
					drawCursor(last_x,last_y);
				}
				break;
			case '+':
				++r;
				drawCursor(last_x,last_y);
				break;
			case 'r':
				cvSet(imagen, CV_RGB(255,255,255),NULL);
				drawCursor(last_x,last_y);
				break;
			case 'c':
				IplImage prs_image;
				prs_image = preprocessing(imagen, size, size,3);
				ssvm.classify(&prs_image,1);
				break;
		}
	}
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	drawCursor(last_x,last_y);
	PrintWelcome();
}

int main( int argc, char** argv )
{
	size=128;
	drawing=0;	//����״̬
	r=5;	//���ʰ뾶
	last_x=last_y=red=green=blue=0;
	//�������ڣ���С128*128
	imagen=cvCreateImage(cvSize(128,128),8,3); 
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	screenBuffer=cvCloneImage(imagen);
	cvNamedWindow( "��д��", 0 );
	cvResizeWindow("��д��", 512,512);
	//��������¼�	
	cvSetMouseCallback("��д��",&on_mouse, 0 );
	PrintWelcome();		//��ʾ��ӭ��Ϣ
	while(1)
	{
		int c;
		c =  cvWaitKey(10);
		if(char(c)=='1')
		{
			system("cls");
			PrintBasic();
			doMyKNN();
		}
		else if(char(c)=='2')
		{
			system("cls");
			PrintBasic();
			doStandardKNN();
		}
		else if(char(c)=='3')
		{
			system("cls");
			PrintBasic();
			doStandardSVM();
		}
		else if(char(c)==27)
			break;
	}
	cvDestroyWindow("��д��");
	return 0;
}

#ifdef _EiC
main(1,"mouseEvent.c");
#endif
