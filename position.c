#include "common_robockey.h"

#define x1 0
#define y1 14.5
#define x2 11.655
#define y2 8.741
#define x3 0
#define y3 -14.5
#define x4 -10.563
#define y4 2.483
#define pi 3.1415926535
#define d(a, b, c, d) ( sqrt((a - c)*(a - c) + (b-d)*(b-d)))
double min3(double a, double b, double c);
double min4(double a, double b, double c, double d);


void get_position(unsigned int* inp, double* location)
{
	int xi1 = (int)(inp[0]);
    int yi1 = (int)(inp[1]);
    int xi2 = (int)(inp[2]);
    int yi2 = (int)(inp[3]);
    int xi3 = (int)(inp[4]);
    int yi3 = (int)(inp[5]);
    int xi4 = (int)(inp[6]);
    int yi4 = (int)(inp[7]);

	double x0 = 0;
	double y0 = 0;
	double theta_rot = 0;

	bool first = 1;
	double min_err = 0;
	int order[] = {0,0,0,0};

	// only three leds are visible
	if(xi4 == 0 && yi4 == 0)
	{
		double xiavg = (xi1 + xi2 + xi3)/3.0;
		double yiavg = (yi1 + yi2 + yi3)/3.0;
		
		double di1 = d(xi1, yi1, xiavg, yiavg);
		double di2 = d(xi2, yi2, xiavg, yiavg);
		double di3 = d(xi3, yi3, xiavg, yiavg);
		
		int diord[] = {0,0,0};
		double mini = min3(di1, di2, di3);
		diord[0] = di1/mini;
		diord[1] = di2/mini;
		diord[2] = di3/mini;
				
		double xes[] = {x1, x2, x3, x4};
		double yes[] = {y1, y2, y3, y4};
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(j!= i)
				{
					for(int k = 0; k < 4; k++)
					{
						if(k!= i && k!= j)
						{
							int l = 10 - (i + j + k);
							double xa[] = {0,0,0,0};
							double ya[] = {0,0,0,0};
							double xavg = 0;
							double yavg = 0;
							for(int ii = 0; ii < 4; ii++)
							{
								if(ii != l)
								{
									xa[ii] = xes[ii];
									xavg += xes[ii];
									ya[ii] = yes[ii];
									yavg += yes[ii];
								}
							}
							xavg /= 3;
							yavg /= 3;	
							
							double d1 = d(x1, y1, xavg, yavg);						
							double d2 = d(x2, y2, xavg, yavg);
							double d3 = d(x3, y3, xavg, yavg);
							double d4 = d(x4, y4, xavg, yavg);

							int dord[] = {d1, d2, d3, d4};
							dord[l] = dord[i];
							int mind = min4(d1, d2, d3, d4);
							for(int ii = 0; ii < 4; ii++)
								dord[ii] /= mind;

							double err = (diord[0]-dord[i]) * (diord[0]-dord[i]) + (diord[1]-dord[j]) * (diord[1]-dord[j]) + (diord[2]-dord[k]) * (diord[2]-dord[k]);
							if(first || err < min_err)
							{
								min_err = err;
								first = false;
								order[0] = i;
								order[1] = j;
								order[2] = k;
								order[3] = 0;

								double thetaf = 0;
								double dCenter = 0;

								int pos = 0;
								if(l == 1)
								{
									for(int ii = 0; ii < 3; ii++)
									{
										if(order[ii] == 2)
											pos = ii;
									}
									thetaf = atan2(-yavg, -xavg) - atan2(y2 - yavg, x2 - xavg);
									dCenter = di2 / d2 * d(xavg, yavg, 0, 0);
								}
								else
								{
									for(int ii = 0; ii < 3; ii++)
									{
										if(order[ii] == 1)
										{
											pos = ii;
										}
									}
									thetaf = atan2(-yavg, -xavg) - atan2(y1 - yavg, x1 - xavg);
									dCenter = di1 / d1 * d(xavg, yavg, 0, 0);
								}

								double theta_rot_off = atan2(-yavg, -xavg);
								int xp = inp[pos * 2];
								int yp = inp[pos * 2 + 1];
								double theta2 = atan2((double)(yp) - yiavg, (double)(xp) - xiavg);
								theta_rot = theta2 + thetaf - theta_rot_off;
								x0 = xiavg + cos(theta_rot_off + theta_rot) * dCenter;
								y0 = yiavg + sin(theta_rot_off + theta_rot) * dCenter;
							}																					
						}
					}
				}
			}
		}
	}
	else
	{

		double xavg = (x1 + x2 + x3 + x4) / 4.0;
		double yavg = (y1 + y2 + y3 + y4) / 4.0;
		
		double d1 = d(x1, y1, xavg, yavg);
		double d2 = d(x2, y2, xavg, yavg);
		double d3 = d(x3, y3, xavg, yavg);
		double d4 = d(x4, y4, xavg, yavg);
		
		double dord[] = {d1, d2, d3, d4};
		double mind = min4(d1, d2, d3, d4);
		for(int ii = 0; ii < 4; ii++)
			dord[ii] /= mind;

		double xiavg = (xi1 + xi2 + xi3 + yi4)/4.0;
		double yiavg = (yi1 + yi2 + yi3 + yi4)/4.0;
		
		double di1 = d(xi1, yi1, xiavg, yiavg);
		double di2 = d(xi2, yi2, xiavg, yiavg);
		double di3 = d(xi3, yi3, xiavg, yiavg);
		double di4 = d(xi4, yi4, xiavg, yiavg);
		
		int diord[] = {0,0,0,0};
		double mini = min4(di1, di2, di3, di4);
		diord[0] = di1/mini;
		diord[1] = di2/mini;
		diord[2] = di3/mini;
		diord[3] = di4/mini;

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(j!= i)
				{
					for(int k = 0; k < 4; k++)
					{
						if(k!= i && k!= j)
						{
							int l = 10 - (i + j + k);
							double err = (diord[0]-dord[i]) * (diord[0]-dord[i]) + (diord[1]-dord[j]) * (diord[1]-dord[j]) + (diord[2]-dord[k]) * (diord[2]-dord[k]) + (diord[3] - diord[l]) * (diord[3] - diord[l]) ;
							if(first || err < min_err)
							{
								min_err = err;
								first = false;
								order[0] = i;
								order[1] = j;
								order[2] = k;
								order[3] = 0;


								int pos = 0;

								for(int ii = 0; ii < 4; ii++)
								{
									if(order[ii] == 1)
									{	
										pos = ii;
									}
								}

								double thetaf = atan2(-yavg, -xavg) - atan2(y1 - yavg, x1 - xavg);
								double dCenter = di1 / d1 * d(xavg, yavg, 0, 0);
								

								double theta_rot_off = atan2(-yavg, -xavg);
								int xp = inp[pos * 2];
								int yp = inp[pos * 2 + 1];
								double theta2 = atan2((double)(yp) - yiavg, (double)(xp) - xiavg);
								theta_rot = theta2 + thetaf - theta_rot_off;
								x0 = xiavg + cos(theta_rot_off + theta_rot) * dCenter;
								y0 = yiavg + sin(theta_rot_off + theta_rot) * dCenter;
							}																					
						}
					}
				}
			}
		}
	}
	
	int pos = 0;
	int sum = 0;
	double scale = 0;
	for(int i = 0; i < 4; i++)
		sum += order[i];
	if(sum == 9)
	{
		for(int ii = 0; ii < 4; ii++)
		{
			if(order[ii] == 3)
				pos = ii;
		}
		scale = d(x3,y3,0,0) / d(x0,y0, inp[pos * 2 ], inp[pos * 2 + 1]);
	}
	else
	{
		for(int ii =0; ii < 4; ii++)
		{
			if(order[ii] == 1)
				pos = ii;
		}
		scale = d(x1,y1,0,0) / d(x0,y0, inp[pos * 2 ], inp[pos * 2 + 1]);
	}
	
	double theta = -theta_rot + pi/2;
	double dist = d(x_init, y_init, x0,y0);
    double theta1 = atan2(x_init - x0, y_init - y0);
    double x = dist * scale * cos(theta1 - theta_rot);
    double y = dist * scale * sin(theta1 - theta_rot);

	location[0] = x;
	location[1] = y;
	location[2] = theta;

}

void set_position(int x, int y)
{
	x_init = x;
	y_init = y;
}

double min3(double a, double b, double c)
{
	double ret = a;
	if(b < ret)
		ret = b;
	if (c < ret)
		ret = c;
	return ret;
}

double min4(double a, double b, double c, double d)
{
	double ret = a;
	if(b < ret)
		ret = b;
	if(c < ret)
		ret = c;
	if(d < ret)
		ret = d;
	return ret;
}

