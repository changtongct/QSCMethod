#include "QSCMethod_4.h"

vector<uchar> Linear_tree_table;
vector<color> Color_table;

int STCMethod_4(IplImage* img, IplImage* segmented_img, double Threshold)
{
	node *STC_Node = new node;
	int k = 0;
	(*STC_Node).x1 = 0;
	(*STC_Node).y1 = 0;
	(*STC_Node).x2 = img->width-1;
	(*STC_Node).y2 = img->height-1;
	(*STC_Node).UL_child = NULL;
	(*STC_Node).UR_child = NULL;
	(*STC_Node).BL_child = NULL;
	(*STC_Node).BR_child = NULL;
	Q_Tree_Generation( STC_Node, img, segmented_img, Threshold);
	Q_TreeTraversing( STC_Node, &k);
	Linear_tree_table_Generation( STC_Node, img);
	Table_Size();
	return k;
}

void Q_Tree_Generation( node *STC_Node, IplImage* img, IplImage* segmented_img, double Threshold)
{
	if(judgement( (*STC_Node).x1, (*STC_Node).x2, (*STC_Node).y1, (*STC_Node).y2, img, Threshold))
	{
		node *UL_Node = new node;
		node *UR_Node = new node;
		node *BL_Node = new node;
		node *BR_Node = new node;
		(*STC_Node).UL_child = UL_Node;
		(*STC_Node).UR_child = UR_Node;
		(*STC_Node).BL_child = BL_Node;
		(*STC_Node).BR_child = BR_Node;

		(*STC_Node).UL_child->x1		= (*STC_Node).x1;
		(*STC_Node).UL_child->y1		= (*STC_Node).y1;
		(*STC_Node).UL_child->x2		= ((*STC_Node).x1+(*STC_Node).x2)/2;
		(*STC_Node).UL_child->y2		= ((*STC_Node).y1+(*STC_Node).y2)/2;
		(*STC_Node).UL_child->UL_child	= NULL;
		(*STC_Node).UL_child->UR_child	= NULL;
		(*STC_Node).UL_child->BL_child	= NULL;
		(*STC_Node).UL_child->BR_child	= NULL;
		Q_Tree_Generation( (*STC_Node).UL_child, img, segmented_img, Threshold);

		(*STC_Node).UR_child->x1		= ((*STC_Node).x1+(*STC_Node).x2)/2+1;
		(*STC_Node).UR_child->y1		= (*STC_Node).y1;
		(*STC_Node).UR_child->x2		= (*STC_Node).x2;
		(*STC_Node).UR_child->y2		= ((*STC_Node).y1+(*STC_Node).y2)/2;
		(*STC_Node).UR_child->UL_child	= NULL;
		(*STC_Node).UR_child->UR_child	= NULL;
		(*STC_Node).UR_child->BL_child	= NULL;
		(*STC_Node).UR_child->BR_child	= NULL;
		Q_Tree_Generation( (*STC_Node).UR_child, img, segmented_img, Threshold);

		(*STC_Node).BL_child->x1		= (*STC_Node).x1;
		(*STC_Node).BL_child->y1		= ((*STC_Node).y1+(*STC_Node).y2)/2+1;
		(*STC_Node).BL_child->x2		= ((*STC_Node).x1+(*STC_Node).x2)/2;
		(*STC_Node).BL_child->y2		= (*STC_Node).y2;
		(*STC_Node).BL_child->UL_child	= NULL;
		(*STC_Node).BL_child->UR_child	= NULL;
		(*STC_Node).BL_child->BL_child	= NULL;
		(*STC_Node).BL_child->BR_child	= NULL;
		Q_Tree_Generation( (*STC_Node).BL_child, img, segmented_img, Threshold);

		(*STC_Node).BR_child->x1		= ((*STC_Node).x1+(*STC_Node).x2)/2+1;
		(*STC_Node).BR_child->y1		= ((*STC_Node).y1+(*STC_Node).y2)/2+1;
		(*STC_Node).BR_child->x2		= (*STC_Node).x2;
		(*STC_Node).BR_child->y2		= (*STC_Node).y2;
		(*STC_Node).BR_child->UL_child	= NULL;
		(*STC_Node).BR_child->UR_child	= NULL;
		(*STC_Node).BR_child->BL_child	= NULL;
		(*STC_Node).BR_child->BR_child	= NULL;
		Q_Tree_Generation( (*STC_Node).BR_child, img, segmented_img, Threshold);
	}
	else
	{
		segmentdisplay((*STC_Node).x1, (*STC_Node).x2, (*STC_Node).y1, (*STC_Node).y2, segmented_img);
	}
}

//四叉树中序遍历，输出叶节点数
void Q_TreeTraversing( node *STC_Node, int *k)
{
	if( (*STC_Node).UL_child == NULL )
	{
		(*k)++;
	}
	else
	{
		Q_TreeTraversing( (*STC_Node).UL_child, k);
		Q_TreeTraversing( (*STC_Node).UR_child, k);
		Q_TreeTraversing( (*STC_Node).BL_child, k);
		Q_TreeTraversing( (*STC_Node).BR_child, k);
	}
}

void Linear_tree_table_Generation( node *STC_Node, IplImage *img)
{
	if((*STC_Node).BL_child==NULL)
	{
		Linear_tree_table.push_back(1);
		struct color thiscolor;
		thiscolor.topleft		= GetGrayValue( (*STC_Node).x1, (*STC_Node).y1, img);
		thiscolor.topright		= GetGrayValue( (*STC_Node).x2, (*STC_Node).y1, img);
		thiscolor.bottomleft	= GetGrayValue( (*STC_Node).x1, (*STC_Node).y2, img);
		thiscolor.bottomright	= GetGrayValue( (*STC_Node).x2, (*STC_Node).y2, img);
		Color_table.push_back(thiscolor);
	}
	else
	{
		Linear_tree_table.push_back(0);
		Linear_tree_table_Generation( (*STC_Node).UL_child, img);
		Linear_tree_table_Generation( (*STC_Node).UR_child, img);
		Linear_tree_table_Generation( (*STC_Node).BL_child, img);
		Linear_tree_table_Generation( (*STC_Node).BR_child, img);
	}
}

void Table_Size()
{
	cout<<"位置编码节点个数："<<Linear_tree_table.size()<<endl;
	cout<<"颜色编码节点个数："<<Color_table.size()<<endl;
}

void Decoding(vector<uchar> &Linear_tree_table, vector<color> &Color_table, IplImage *decoded_img, int width, int height)
{
	node *Decoded_Node = new node;
	int l = 0;
	int m = 0;
	(*Decoded_Node).x1 = 0;
	(*Decoded_Node).y1 = 0;
	(*Decoded_Node).x2 = width-1;
	(*Decoded_Node).y2 = height-1;
	(*Decoded_Node).UL_child = NULL;
	(*Decoded_Node).UR_child = NULL;
	(*Decoded_Node).BL_child = NULL;
	(*Decoded_Node).BR_child = NULL;
	
	Q_Tree_ReGeneration( Decoded_Node, Linear_tree_table, &l);
	/*for(unsigned int i=0;i<Linear_tree_table.size();i++)
		printf("%u,",Linear_tree_table.at(i));*/
	//cout<<"生成的树的节点数"<<*l<<endl;
	//Q_TreeTraversing( node *STC_Node, int *k);
	Image_Regeneration( Decoded_Node, Color_table, m, decoded_img);
}

void Q_Tree_ReGeneration( node *Decoded_Node, vector<uchar> &Linear_tree_table, int *l)
{
	if((*l)<Linear_tree_table.size())
	{
		if(Linear_tree_table.at(*l)==0)
		{
			node *New_UL_Node = new node;
			node *New_UR_Node = new node;
			node *New_BL_Node = new node;
			node *New_BR_Node = new node;

			(*Decoded_Node).UL_child = New_UL_Node;
			(*Decoded_Node).UR_child = New_UR_Node;
			(*Decoded_Node).BL_child = New_BL_Node;
			(*Decoded_Node).BR_child = New_BR_Node;

			(*Decoded_Node).UL_child->x1		= (*Decoded_Node).x1;
			(*Decoded_Node).UL_child->y1		= (*Decoded_Node).y1;
			(*Decoded_Node).UL_child->x2		= ((*Decoded_Node).x1+(*Decoded_Node).x2)/2;
			(*Decoded_Node).UL_child->y2		= ((*Decoded_Node).y1+(*Decoded_Node).y2)/2;
			(*Decoded_Node).UL_child->UL_child	= NULL;
			(*Decoded_Node).UL_child->UR_child	= NULL;
			(*Decoded_Node).UL_child->BL_child	= NULL;
			(*Decoded_Node).UL_child->BR_child	= NULL;
			(*l)++;
			Q_Tree_ReGeneration( (*Decoded_Node).UL_child, Linear_tree_table, l);

			(*Decoded_Node).UR_child->x1		= ((*Decoded_Node).x1+(*Decoded_Node).x2)/2+1;
			(*Decoded_Node).UR_child->y1		= (*Decoded_Node).y1;
			(*Decoded_Node).UR_child->x2		= (*Decoded_Node).x2;
			(*Decoded_Node).UR_child->y2		= ((*Decoded_Node).y1+(*Decoded_Node).y2)/2;
			(*Decoded_Node).UR_child->UL_child	= NULL;
			(*Decoded_Node).UR_child->UR_child	= NULL;
			(*Decoded_Node).UR_child->BL_child	= NULL;
			(*Decoded_Node).UR_child->BR_child	= NULL;
			(*l)++;
			Q_Tree_ReGeneration( (*Decoded_Node).UR_child, Linear_tree_table, l);

			(*Decoded_Node).BL_child->x1		= (*Decoded_Node).x1;
			(*Decoded_Node).BL_child->y1		= ((*Decoded_Node).y1+(*Decoded_Node).y2)/2+1;
			(*Decoded_Node).BL_child->x2		= ((*Decoded_Node).x1+(*Decoded_Node).x2)/2;
			(*Decoded_Node).BL_child->y2		= (*Decoded_Node).y2;
			(*Decoded_Node).BL_child->UL_child	= NULL;
			(*Decoded_Node).BL_child->UR_child	= NULL;
			(*Decoded_Node).BL_child->BL_child	= NULL;
			(*Decoded_Node).BL_child->BR_child	= NULL;
			(*l)++;
			Q_Tree_ReGeneration( (*Decoded_Node).BL_child, Linear_tree_table, l);

			(*Decoded_Node).BR_child->x1		= ((*Decoded_Node).x1+(*Decoded_Node).x2)/2+1;
			(*Decoded_Node).BR_child->y1		= ((*Decoded_Node).y1+(*Decoded_Node).y2)/2+1;
			(*Decoded_Node).BR_child->x2		= (*Decoded_Node).x2;
			(*Decoded_Node).BR_child->y2		= (*Decoded_Node).y2;
			(*Decoded_Node).BR_child->UL_child	= NULL;
			(*Decoded_Node).BR_child->UR_child	= NULL;
			(*Decoded_Node).BR_child->BL_child	= NULL;
			(*Decoded_Node).BR_child->BR_child	= NULL;
			(*l)++;
			 Q_Tree_ReGeneration( (*Decoded_Node).BR_child, Linear_tree_table, l);
		}
		else if(Linear_tree_table.at(*l)==1)
		{

		}
		else
		{
		}
	}
}

void Image_Regeneration( node *Decoded_Node, vector<color> &Color_table, int &m, IplImage *decoded_img)
{
	if( (*Decoded_Node).UL_child == NULL )
	{
		int x1, y1, x2, y2;
		uchar c_topleft, c_topright, c_bottomleft, c_bottomright;
		x1 = (*Decoded_Node).x1;
		y1 = (*Decoded_Node).y1;
		x2 = (*Decoded_Node).x2;
		y2 = (*Decoded_Node).y2;
		c_topleft = Color_table.at(m).topleft;
		c_topright = Color_table.at(m).topright;
		c_bottomleft = Color_table.at(m).bottomleft;
		c_bottomright = Color_table.at(m).bottomright;
		BlockRegeneration( x1, x2, y1, y2, c_topleft, c_topright, c_bottomleft, c_bottomright ,decoded_img);
		m++;
	}
	else
	{
		Image_Regeneration( (*Decoded_Node).UL_child, Color_table, m, decoded_img);
		Image_Regeneration( (*Decoded_Node).UR_child, Color_table, m, decoded_img);
		Image_Regeneration( (*Decoded_Node).BL_child, Color_table, m, decoded_img);
		Image_Regeneration( (*Decoded_Node).BR_child, Color_table, m, decoded_img);
	}
}

double BPP( vector<uchar> &Linear_tree_table, vector<color> &Color_table ,int width , int height)  //计算BPP
{
	//double bpp = (8.0*sizeof(uchar)*Linear_tree_table.size()+8.0*4.0*sizeof(uchar)*Color_table.size())/(width*height);//畅彤

    double bpp = (Linear_tree_table.size()+32.0*Color_table.size())/(width*height);//郑运平

	//cout << "sizeof (uchar)=" << sizeof(uchar) << endl;
	//printf("%d,%d\n",sizeof(int),sizeof(double));
	return bpp;
}

double PSNR( IplImage* original_img, IplImage* decoded_img, int width, int height) //计算PSNR
{
	double temp=0;
	double psnr;
	CvScalar g1,g2;
	double g3,g4;

	for(int y=0;y<height;y++)
	{
		for (int x=0;x<width;x++)
		{
			g1 = cvGet2D( original_img, y, x);
			g2 = cvGet2D( decoded_img, y, x);
			g3 = g1.val[0];
			g4 = g2.val[0];
			temp = temp + pow((g3-g4),2);
		}
	}
	psnr =10*log10((255.0*255.0*width*height)/temp);
	
	return psnr;
}

double CR( IplImage* original_img, vector<uchar> &Linear_tree_table, vector<color> &Color_table)	//计算CR
{
	double img_size;
	//double code_size;
	double cr;

	img_size = (original_img->width)*(original_img->height)*1.0;
	//code_size = 1.0*sizeof(uchar)*Linear_tree_table.size()+4.0*sizeof(uchar)*Color_table.size();
	//cr = img_size/code_size;
    cr=8.0/((Linear_tree_table.size()+32.0*Color_table.size())/(img_size));//郑运平
	return cr;
}
