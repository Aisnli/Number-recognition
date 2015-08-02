#include<cstdio>
#include<iostream>
#include<Windows.h>
#include<cstdlib>
#include<cmath>
#include<bitset>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<string>
#include<time.h>
#include<queue>
#include "MyKNN.h"
using namespace std;
#define WEIGHT 32
#define HEIGHT 32
#define NUM 4
//#define BMP_filename "c:\\test.bmp"
#define File "test-num.txt"
//char BMP_filename[30];
int x,y,s,temp;
int DATAMASS[WEIGHT][HEIGHT]={0};
int DATAMASS_red[WEIGHT][HEIGHT]={0};
int DATAMASS_green[WEIGHT][HEIGHT]={0};
int DATAMASS_blue[WEIGHT][HEIGHT]={0};
int rec[WEIGHT]={0};
int lf[NUM],rt[NUM],up[NUM],dw[NUM];
char result[NUM];
bitset <1024> DATABIT; 
struct database
{
	vector<bitset<1024> > DATABIT;
	int num;
}Number[10];
#define m 5
#define dimen 1024
#define MAXN 100010
int now,leaf;
Node node[MAXN];
Node res[m+1];
struct KD_Tree
{
	Node n;
	int son;
}tree[MAXN];
class CMP 
{
	public:
	bool operator()(const Node &p,const Node &q)const
	{
		return p.dis<q.dis;
	}
};
priority_queue<Node,vector<Node>,CMP> pq;	//ά��һ����СΪm������ 
void MyKNN::readin(string BMP_filename)
{
	//cout<<BMP_filename<<endl;
	FILE *fp=fopen(BMP_filename.c_str(),"rb");//�Զ�����ֻ����ʽ��BMP�ļ�
	if(fp==NULL)
	{
		printf("no such picture!\n");
		exit(-1);
	}
	for(int i = 0; i < 54; i++)
		temp=fgetc(fp);//����54���ֽڵ��ļ�ͷ����
	for(y = HEIGHT - 1;y>=0;y--)//BMPͼƬ�����ǵ���洢��
	{
		for(x=0;x<WEIGHT;x++)
		{
			DATAMASS_red[x][y]=fgetc(fp);
			DATAMASS_green[x][y]=fgetc(fp);
			DATAMASS_blue[x][y]=fgetc(fp);
		}
	}
	fclose(fp);//�ر��ļ� 
}
void MyKNN::changeto10()
{
	for(x=0;x<WEIGHT;x++)for(y=0;y<HEIGHT;y++)
	{
		if(DATAMASS_red[x][y]*0.3+DATAMASS_green[x][y]*0.6+DATAMASS_blue[x][y]*0.1< 179)
			DATAMASS[x][y]=1;
		else 
			DATAMASS[x][y]=0;
		//red*0.3+green*0.6+blue*0.1 �Ǽ������ȵĹ�ʽ
	} 
}
void MyKNN::remove_ilegalpoint()
{
	for(x=1;x<WEIGHT-1;x++)
		for(y=1;y<HEIGHT-1;y++)
		{
			if(DATAMASS[x][y]==1 && 
				DATAMASS[x-1][y-1]==0 && DATAMASS[x][y-1]==0 && 
				DATAMASS[x+1][y-1]==0 && DATAMASS[x-1][y]==0 && 
				DATAMASS[x+1][y]==0 && DATAMASS[x-1][y+1]==0 && 
				DATAMASS[x][y+1]==0 && DATAMASS[x+1][y+1]==0)
				DATAMASS[x][y]=0;
			//���һ�����ֵ��1������������Χ8�����ֵ����0����ô�������Ǹ��ŵ� 
		} 
}
//�������ݿ�
int MyKNN::readFile()
{
	ifstream fin;
	fin.open(File);
	if (!fin)
	{
		printf("no such file!\n");
		exit(-1);
	}
	printf("Start Training...\n");
	char file[255];
	char file_path[]="KNNOCR/";
	bitset<1024>tem;
	for(int i=0;i<10;++i)
		for(int j=0;j<100;++j)
		{
			if(j<10)
				sprintf(file,"%s%d/%d-%d.bmp",file_path, i, i , j);
			else
				sprintf(file,"%s%d/%d-%d.bmp",file_path, i, i , j);
			readin(file);
			changeto10();
			remove_ilegalpoint();
			Number[i].num = i;
			for (int c = 0; c < WEIGHT; c++)
				for (int d = 0; d < HEIGHT; d++)
					tem[c *WEIGHT + d] = DATAMASS[c][d];
			Number[i].DATABIT.push_back(tem);
			if(j<trainsamples)
			{
				node[leaf].x=tem;
				node[leaf++].num=i;
			}
		}
	return 0;
}
//���Դ���
float MyKNN::testcode()
{
	int tot;
	int test_re;
	int count = 0;
	tot=(100-trainsamples)*10;
	for(int i=1;i<10;++i)
		for(int j=trainsamples;j<100;++j)
		{
			//printf("Given %d ",i);
			test_re = kdd(Number[i].DATABIT[j],0);
			//printf("Returned %d\n",test_re);
			if (test_re !=i)
				++count ;
		}
	printf("Finish Training...\n");
	float precision = float(count)/float(tot);
	return precision;
}
void MyKNN::classify()
{
	bitset<1024> tem;
	readin("out32.bmp");
	changeto10();
	remove_ilegalpoint();
	for (int i = 0; i < WEIGHT; i++)
			for (int j = 0; j < HEIGHT; j++)
				tem[i *WEIGHT + j] = DATAMASS[i][j];
	int test_re = kdd(tem,1);
}
int MyKNN::get_dis(const Node &p,const Node &q)	//�õ����루ƽ���� 
{
	int dis=0;
	for(int i=0;i<dimen;++i)
		dis+=abs(p.x[i]-q.x[i]);
	return dis;
}	
bool Cmp(const Node &p,const Node &q)
{
	return p.x[now]<q.x[now];
}
void MyKNN::build_tree(Node node[],int lef,int righ,int id,int depth)		//���� 
{
	if(lef>righ)
		return ;
	tree[id].son=righ-lef;		//ά�������Ӹ��� 
	tree[2*id].son=tree[2*id+1].son=-1;
	now=depth%dimen;			//�õ���ǰά�� 
	sort(node+lef,node+righ+1,Cmp);
	int mid=(lef+righ)/2;		
	tree[id].n=node[mid];		//�ҵ���λ�� 
	build_tree(node,lef,mid-1,2*id,depth+1);	//���������� 
	build_tree(node,mid+1,righ,2*id+1,depth+1);	//���������� 
}

void MyKNN::search_tree(Node n,int id,int depth)		//������ 
{
	if(tree[id].son==-1)		//��ѯ���˿ս�㣬���� 
		return ;
	Node nn=tree[id].n;
	nn.dis=get_dis(n,nn);
	if(tree[id].son==0)			//��ѯ����Ҷ�ڵ�,�жϴ�ʱһ·���Ƿ������m���� 
	{
		if(pq.size()<m)			//������㣬������Ҷ�ڵ� 
			pq.push(nn);
		else					//���򣬱Ƚϵ�ǰ�����Ѷ�Ԫ�أ�ά������ 
		{
			if(nn.dis<pq.top().dis)
			{
				pq.pop();
				pq.push(nn);
			}
		}
		return ;
	}
	int nowdim=depth%dimen;
	if(n.x[nowdim]<tree[id].n.x[nowdim])	//�������������� 
	{
		search_tree(n,2*id,depth+1);		
											//���أ���ʼ���� 
		if(pq.size()<m)						//�����ʱ�Ѵ�С����m 
		{
			pq.push(nn);					//���뵱ǰ��� 
			search_tree(n,2*id+1,depth+1);//ͬʱ���������� 
		}	
		else								
		{
			if(nn.dis<pq.top().dis)			//���򣬱Ƚϵ�ǰԪ����Ѷ�Ԫ�أ�ά������ 
			{
				pq.pop();
				pq.push(nn);
			}
											//�����ѯ��Χ��糬ƽ�棬ҲҪȥ���������� 
			if((n.x[nowdim]-tree[id].n.x[nowdim])*(n.x[nowdim]-tree[id].n.x[nowdim])<pq.top().dis)
				search_tree(n,2*id+1,depth+1);			
		}	
	}
	//������������������� 
	else		//������������ 
	{
		search_tree(n,2*id+1,depth+1);
		if(pq.size()<m)
		{
			pq.push(nn);
			search_tree(n,2*id,depth+1);
		}	
		else
		{
			if(nn.dis<pq.top().dis)
			{
				pq.pop();
				pq.push(nn);
			}
			if((n.x[nowdim]-tree[id].n.x[nowdim])*(n.x[nowdim]-tree[id].n.x[nowdim])<pq.top().dis)
				search_tree(n,2*id,depth+1);			
		}
	}	
}
int MyKNN::kdd(bitset<1024> given,int showinfo)
{
	Node temp;
	for(int j=0;j<dimen;++j)
		temp.x[j]=given[j];
	while(!pq.empty())
		pq.pop();
	search_tree(temp,1,0);
	for(int j=0;j<m;++j)
	{
		res[m-1-j]=pq.top();
		pq.pop();
	}
	int cnt[10];
	memset(cnt,0,sizeof(cnt));
	for(int j=0;j<m;++j)
	{
		//if(showinfo)
		//	cout<<res[j].num<<" "<<res[j].dis<<"\n";
		cnt[res[j].num]++;
	}
	//system("pause");
	int Max=0,r;
	for(int i=0;i<10;++i)
	{
		if(cnt[i]>Max)
		{
			Max=cnt[i];
			r=i;
		}
	}
	if(showinfo==1)
	{
		printf("|\t    %d    \t| \t %d of %d \t| \n",r,cnt[r],m);
		printf(" -------------------------------------------------\n");
	}
	return r;
}
MyKNN::MyKNN()
{
	trainsamples=80;
	readFile();
	build_tree(node,0,leaf-1,1,0);
	time_t st,ed;
	st=time(NULL);
	printf("������: %.2f%%\n", 100*testcode());
	ed=time(NULL);
	printf("time:%.2f\n",difftime(ed,st));
	printf(" -------------------------------------------------\n");
	printf("|\tʶ����\t|\n");
	printf(" -------------------------------------------------\n");
}
