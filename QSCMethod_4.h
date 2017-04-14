#ifndef QSCMETHOD_4_
#define _QSCMETHOD_4_

#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <vector>
#include "commonfile.h"
#include <iostream>
using namespace std;

struct node
{
	int x1;
	int y1;
	int x2;
	int y2;
	node* UL_child;
	node* UR_child;
	node* BL_child;
	node* BR_child;
};
struct color
{
	uchar topleft;
	uchar topright;
	uchar bottomleft;
	uchar bottomright;
};
extern vector<uchar> Linear_tree_table;
extern vector<color> Color_table;

int STCMethod_4( IplImage* img, IplImage* segmented_img, double Threshold);
void Q_Tree_Generation( node *STC_Node, IplImage* img, IplImage* segmented_img, double Threshold);
void Q_TreeTraversing( node *STC_Node, int *k);
void Linear_tree_table_Generation( node *STC_Node, IplImage *img);
void Table_Size();
void Decoding(vector<uchar> &Linear_tree_table, vector<color> &Color_table, IplImage *decoded_img, int width, int height);
void Q_Tree_ReGeneration( node *Decoded_Node, vector<uchar> &Linear_tree_table, int *l);
void Image_Regeneration( node *Decoded_Node, vector<color> &Color_table, int &m, IplImage *decoded_img);

double BPP( vector<uchar> &Linear_tree_table, vector<color> &Color_table , int width, int height);
double PSNR( IplImage* original_img, IplImage* decoded_img, int width, int height);
double CR( IplImage* original_img, vector<uchar> &Linear_tree_table, vector<color> &Color_table);


#endif