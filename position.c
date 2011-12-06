#include "common_robockey.h"

#define x1 0
#define y1 14.5
#define x4 11.655
#define y4 8.741
#define x3 0
#define y3 -14.5
#define x2 -10.563
#define y2 2.483
#define d(a, b, c, d) ( sqrt((a - c)*(a - c) + (b-d)*(b-d)))
#define sq(a) ( (a) * (a) )
double min3(double a, double b, double c);
double min4(double a, double b, double c, double d);


void get_position(unsigned int* inp, double* xo, double* yo, double* to)
{
	int xi1 = inp[0];
    int yi1 = inp[1];
    int xi2 = inp[3];
    int yi2 = inp[4];
    int xi3 = inp[6];
    int yi3 = inp[7];
    int xi4 = inp[9];
    int yi4 = inp[10];
	

	double x0 = 0;
	double y0 = 0;
	double theta_rot = 0;

	bool first = 1;
	double min_err = 0.0;
	int order[] = {0,0,0,0};

	// only three leds are visible
	if(xi4 == 1023 && yi4 == 1023)
	{
		*xo = -1000;
		*yo = -1000;
		double xavg = (x1 + x2 + x3) / 4.0;
		double yavg = (y1 + y2 + y3) / 3.0;
		*to = atan2(yavg - y_init, xavg - x_init);
		return;
	}
	else
	{

		double xavg = (x1 + x2 + x3 + x4) / 4.0;
		double yavg = (y1 + y2 + y3 + y4) / 4.0;
		
		double d1 = d(x1, y1, xavg, yavg);
		double d2 = d(x2, y2, xavg, yavg);
		double d3 = d(x3, y3, xavg, yavg);
		double d4 = d(x4, y4, xavg, yavg);

		
		
		double dord[] = {0, 0, 0, 0};

		double mind = min4(d1, d2, d3, d4);

		dord[0] = d1 / mind;
		dord[1] = d2 / mind;
		dord[2]	= d3 / mind;
		dord[3] = d4 / mind;

		

		double xiavg = (xi1 + xi2 + xi3 + xi4)/4.0;
		
		double yiavg = (yi1 + yi2 + yi3 + yi4)/4.0;
		
		double di1 = d(xi1, yi1, xiavg, yiavg);
		double di2 = d(xi2, yi2, xiavg, yiavg);
		double di3 = d(xi3, yi3, xiavg, yiavg);
		double di4 = d(xi4, yi4, xiavg, yiavg);
		double diord[] = {0,0,0,0};
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
							double err = sq(diord[0] - dord[i]) + sq(diord[1] - dord[j]) + sq(diord[2] - dord[k]) + sq(diord[3] - dord[l]);
							if(first || err < min_err)
							{
								min_err = err;
								first = false;
								order[0] = i + 1;
								order[1] = j + 1;
								order[2] = k + 1;
								order[3] = l + 1;


								int pos = 0;

								for(int ii = 0; ii < 4; ii++)
								{
									if(order[ii] == 1)
									{	
										pos = ii;
									}
								}
	
								double theta_rot_off = -1.667782601220809;
								double thetaf = theta_rot_off - atan2(y1 - yavg, x1 - xavg);
								double dCenter = di1 / d1 * d(xavg, yavg, 0, 0);
																
								
								int xp = inp[pos * 3];
								int yp = inp[pos * 3 + 1];
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
		scale = d(x3,y3,0,0) / d(x0,y0, inp[pos * 3 ], inp[pos * 3 + 1]);
	}
	else
	{
		for(int ii =0; ii < 4; ii++)
		{
			if(order[ii] == 1)
				pos = ii;
		}
		scale = d(x1,y1,0,0) / d(x0,y0, inp[pos * 3 ], inp[pos * 3 + 1]);
	}
	
	double theta = theta_rot;
	double dist = d(x_init, y_init, x0,y0);
    double theta1 = pi - atan2(- y0 + y_init, - x0  + x_init);

    double x = dist * scale * cos(theta1 + theta_rot);
    double y = dist * scale * sin(theta1 + theta_rot);
	
	*xo = x;
	*yo = y;
	*to = theta;
	

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


void make_positive(double* theta)
{
	int cycles = 0;
	while(*theta < 0 && cycles <4)
	{
		*theta += 2 * pi;
		cycles++;
	}
	while(*theta > 2*pi && cycles < 4)
	{
		*theta -= 2 * pi;
		cycles ++;
	}
}
