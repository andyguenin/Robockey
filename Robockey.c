#include "common_robockey.h"


char ADDRESS = 0x01;
char CHANNEL = 0X24;
char PACKET_SIZE = 1;
float average_X = 0.0;
float average_Y = 0.0;
float last_x = 0.0;
float last_y = 0.0;
float del_x = 0.0;
float del_y = 0.0;
float last_del_x = -100;
float last_del_y = -100;
float angle = 0;
bool first = true;
int average_cnt = 0;
int rotate_complete = 0;
int rotate = 0;
int drive_complete = 0;
int drive = 0;


int main(void)
{

	init_all();
	wait(2);
	m_red(ON);
	m_green(ON);
	wait(2);
	static unsigned int blobs[12];
	static char blob_status;
	
	m_red(OFF);
	m_green(OFF);
	wait(2);

	int x_off = 0;
	int y_off = 0;
	int off_err = 10;

	blob_status = m_wii_read(blobs);
	if(blob_status){

		average_cnt=0;
		for(int i=0;i<11;i+=3){
			if(blobs[i]<1023){
				average_cnt++; 
	        	x_off += blobs[i];;
			}
		}
		x_off = x_off / average_cnt;
		average_cnt = 0;

		for(int i=1;i<11;i+=3){
			if(blobs[i]<1023){
				average_cnt++; 
	        	y_off += blobs[i];;
			}
		}
		
		y_off = y_off / average_cnt;

	}



	while(!rotate_complete)
	{
		
		blob_status = m_wii_read(blobs);
		if(blob_status){

			average_cnt=0;
			average_X=0;
			average_Y=0;
			for(int i=0;i<11;i+=3){
				if(blobs[i]<1023){
					average_cnt++; 
		        	average_X += blobs[i];;
				}
			}
			average_X = average_X / average_cnt;
			average_cnt = 0;

			for(int i=1;i<11;i+=3){
				if(blobs[i]<1023){
					average_cnt++; 
		        	average_Y += blobs[i];;
				}
			}
			
			average_Y = average_Y / average_cnt;

		}




		if(average_Y > y_off-off_err && average_Y < y_off+off_err){
			if(average_X > x_off+off_err || average_X < x_off-off_err)
			{
				if(average_X < x_off)
				{
					rotate = 1;
				}
				else
				{
					rotate = 2;
				}
			}
			else
			{
				rotate = 4;
			}
		}
		if(average_Y <= y_off-off_err){rotate = 2;}
		if(average_Y >= y_off+off_err){rotate = 3;}
		  

/*
		if(average_Y > 359 && average_Y < 409){
			m_red(OFF);
			
		}
		else
		{
			m_red(ON);
		}

		if(average_X < 537 && average_X > 487)
		{
			m_green(OFF);
		}
		else
		{
			m_green(ON);
		}
		rotate = 4;
		rotate = 3;
		if(average_Y > 359 && average_Y < 409 && average_X >610 && average_X < 690)
			rotate = 1;
*/
		switch(rotate){

			case 1:
			set_left(20);
			set_right(20);
			m_red(ON);
			m_green(ON);
			
			break;

			case 2:
			set_left(20);
			set_right(0);
			m_green(ON);
			m_red(OFF);
			break;

			case(3):
			set_left(0);
			set_right(20);
			m_red(ON);
			m_green(OFF);
			break;

			default:
			set_left(0);
			set_right(0);
			//rotate_complete = true;
			m_red(OFF);
			m_green(OFF);
			break;
		   
		}
	}
	return;
	while(!drive_complete)
	{

		blob_status = m_wii_read(blobs);
		if(blob_status){
		  	for(int i=1;i<11;i+=3){
				if(blobs[i]<1023){
					average_cnt++; 
		          	average_Y += blobs[i];
			    } 
			}
		average_Y = average_Y / average_cnt;
		}

		m_green(ON);

		if(average_Y > 336 && average_Y < 387){drive = 1;}
		if(average_Y <= 336){drive = 2;}
		if(average_Y >= 387){drive = 3;}
		  
		switch(drive){

			case 1:
			set_left(0);
			set_right(0);
			drive_complete = 1;
			m_green(OFF);
			break;

			case 2:
			set_left(-10);
			set_right(-10);
			break;

			case(3):
			set_left(10);
			set_right(10);
			break;

			default:
			set_left(0);
			set_right(0);
			break;
		   
	   }
	}

/*
		  else{
		  	m_green(OFF);
		  	del_x = average_X - last_x;
			del_y = average_Y - last_y;
			last_x = average_X;
			last_y = average_Y;
		  }
		  angle = atan(del_y/del_x);

		  m_usb_tx_string(" del_x : ");
      	  m_usb_tx_uint((unsigned int)del_x);
		  m_usb_tx_string(" del y : ");
		  m_usb_tx_uint((unsigned int)(del_y));
		  m_usb_tx_string("\n");
*/
		
	
}




/*
int main(void)

{

	init_all();

	
	while(1)

	{
	
		m_red(ON);
		set_left(0);
		set_right(0);
		wait(2);
		set_left(100);
		set_right(-50);
		wait(2);
		set_left(-25);
		set_right(75);
		wait(2);
	
	}
	
}
*/
