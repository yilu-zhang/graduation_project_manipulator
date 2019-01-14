#include "algorithm.h"

/*****矩阵乘法函数*****/
double MatrixMultipication(double a[4][4],double b[4][4])
{
  double c[4][4];
	unsigned short int x=0,y=0;
	for(;x<4;x++)
	{
	  for(y=0;y<4;y++)
	  {
			  c[x][y]=a[x][0]*b[0][y]+a[x][1]*b[1][y]+a[x][2]*b[2][y]+a[x][3]*b[3][y];
	  }
	}
}

/******求逆矩阵函数*****/
double MatrixInversion(double mi[4][4])
{
   int mi_a;  //mi的行列式值
	 int mi_b[4][4];   //mi的伴随矩阵
   double mi_i[4][4]; //mi的逆矩阵
   int b_x,b_y;
	 mi_a=mi[0][0]*mi[1][1]*mi[2][2]*mi[3][3]-mi[0][1]*mi[1][2]*mi[2][3]*mi[3][0]+mi[0][2]*mi[1][3]*mi[2][0]*mi[3][1]-mi[0][3]*mi[1][0]*mi[2][1]*mi[3][2]+mi[0][3]*mi[1][2]*mi[2][1]*mi[3][0]-mi[0][2]*mi[1][1]*mi[2][0]*mi[3][3]+mi[0][1]*mi[1][0]*mi[2][3]*mi[3][2]-mi[0][0]*mi[1][3]*mi[2][2]*mi[3][1];//求矩阵对应行列式值
   for(b_x=0;b_x<4;b_x++)  //求伴随矩阵
	{
		for(b_y=0;b_y<4;b_y++)
		{
			int mi_x,mi_y;
			double mi_yz=0;
			double yz;
			int t;
			for(mi_x=0,mi_y=0,t=1,yz=1;t<9;t++)
			{
				if(t<5)
				{
					int m;
				  for(m=1,yz=1;m<4;m++)
					{
						if(mi_x!=b_y&&mi_y!=b_x)
							yz*=mi[mi_x][mi_y];
						mi_x++;
						mi_y++;
						if(mi_y==3)
							mi_y=0;
					}
					mi_yz+=yz;
					mi_x=0;
					mi_y=t;
				
				}	
       if(t>4)
				{
					int m;
				  for(m=1,yz=1;m<4;m++)
					{
						if(mi_x!=b_y&&mi_y!=b_x)
							yz*=mi[mi_x][mi_y];
						mi_x++;
						mi_y++;
						if(mi_y==3)
							mi_y=0;
					}
					mi_yz+=yz;
					mi_x=0;
					mi_y=t;
				
				}									
				
			}
			
		}
	}
}
