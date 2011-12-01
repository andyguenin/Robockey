#include "common_robockey.h"


void position_demo()
{

	m_usb_init();
	

	set(DDRD,5);
	//init mWii
	char wii_status = m_wii_open();
	if(wii_status){
		m_green(ON);
	    set(PORTD,5);
	}else
	m_red(ON);

	
	while(!m_usb_isconnected())
	{
		m_green(ON);
	} 
  unsigned int blobs[] = {541,381,0,503,344,0,524,304,0,599,347,0};
  int blob_status = 0;


	while(1)
	{
		double x;
		double y;
		double theta;
    if(m_usb_isconnected()){
	  blob_status = m_wii_read(blobs);

	  if(blob_status){
      set_position(1024/2,768/2);
      

      get_position(blobs,&x,&y,&theta);
		  
		
      /*
      m_usb_tx_string("x: ");
      m_usb_tx_int((int)(x * 100));
      m_usb_tx_string("\n");
      m_usb_tx_string("y: ");
      m_usb_tx_int((int)(y * 100));
      m_usb_tx_string("\n");
      m_usb_tx_string("theta: ");
      m_usb_tx_int((int)(theta * 100));
      m_usb_tx_string("\n");

      m_usb_tx_string("blob1x: ");
      m_usb_tx_int((int) blobs[0]);
 
      m_usb_tx_string("blob1y: ");
      m_usb_tx_int((int) blobs[1]);
      m_usb_tx_string("\n");
      m_usb_tx_string("blob2x: ");
      m_usb_tx_int((int) blobs[3]);
 
      m_usb_tx_string("blob2y: ");
      m_usb_tx_int((int) blobs[4]);
      m_usb_tx_string("\n");
      m_usb_tx_string("blob3x: ");
      m_usb_tx_int((int) blobs[6]);
 
      m_usb_tx_string("blob3y: ");
      m_usb_tx_int((int) blobs[7]);
      m_usb_tx_string("\n");
      m_usb_tx_string("blob4x: ");
      m_usb_tx_int((int) blobs[9]);

      m_usb_tx_string("blob4y: ");
      m_usb_tx_int((int) blobs[10]);
      m_usb_tx_string("\n");

	  */
      m_usb_tx_int((int)(x * 100));
      m_usb_tx_int((int)(y * 100));
      m_usb_tx_int((int)(theta * 100));


   		m_wait(100);
		  m_red(TOGGLE);
	  }
    }

	}

}
