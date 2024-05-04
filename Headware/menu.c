#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "Key.h"


int menu1(void)
{
  int flag = 1;
  OLED_ShowString(1,2,"1.mode1");
  OLED_ShowString(2,2,"2.mode2");
  OLED_ShowString(3,2,"3.mode3");
  OLED_ShowString(4,2,"4.mode4");
	while(1)
	{
	  if(Key0_read() == 0)
	  {
		 flag++;
         if(flag == 5) flag = 1;
		 
 	  }
	 
      if(Key1_read() == 0)
	  {
		  
		  OLED_Clear();
		  return(flag);
	  }		  
	
      switch(flag)
	  {
		  case 1 :
		  {
		    OLED_ShowString(1,1,"*");
		    OLED_ShowString(2,1," ");
			OLED_ShowString(3,1," ");
			OLED_ShowString(4,1," ");
		  }break;
		  
		  case 2 :
		  {
		    OLED_ShowString(1,1," ");
		    OLED_ShowString(2,1,"*");
			OLED_ShowString(3,1," ");
			OLED_ShowString(4,1," ");
		  }break;
		  
		  case 3 :
		  {
		    OLED_ShowString(1,1," ");
		    OLED_ShowString(2,1," ");
			OLED_ShowString(3,1,"*");
			OLED_ShowString(4,1," ");  
		  }break;
		  
		   case 4 :
		  {
		    OLED_ShowString(1,1," ");
		    OLED_ShowString(2,1," ");
			OLED_ShowString(3,1," ");
			OLED_ShowString(4,1,"*");  
		  }break;
		  
	  }	  
		   
	}
}

int mode1(void)
{
    while(1)
	{   
	OLED_ShowString(1,1,"this is mode1");
		
	  if(Key1_read() == 0)
	  {
	    OLED_Clear();
		return 0;
	  }
 	   
      
	}
}

int mode2(void)
{
  int flag = 1;
  OLED_ShowString(1,2,"1");
  OLED_ShowString(2,2,"6");
  OLED_ShowString(3,2,"7");
  OLED_ShowString(4,2,"11");
	while(1)
	{
	  if(Key0_read() == 0)
	  {
		 flag++;
         if(flag == 4) flag = 1;
		 
 	  }
	 
      if(Key1_read() == 0)
	  {
		  OLED_Clear();
		  return(flag);
	  }		  
	
      switch(flag)
	  {
		  case 1 :
		  {
		    OLED_ShowString(1,1,"*");
		    OLED_ShowString(2,1," ");
			OLED_ShowString(3,1," ");
			OLED_ShowString(4,1," ");
		  }break;
		  
		  case 2 :
		  {
		    OLED_ShowString(1,1," ");
		    OLED_ShowString(2,1,"*");
			OLED_ShowString(3,1," ");
			OLED_ShowString(4,1," ");
		  }break;
		  
		  case 3 :
		  {
		    OLED_ShowString(1,1," ");
		    OLED_ShowString(2,1," ");
			OLED_ShowString(3,1,"*");
			OLED_ShowString(4,1," ");  
		  }break;
		  
		   case 4 :
		  {
		    OLED_ShowString(1,1," ");
		    OLED_ShowString(2,1," ");
			OLED_ShowString(3,1," ");
			OLED_ShowString(4,1,"*");  
		  }break;
		  
	  }	  
		   
	}
}

int mode3(void)
{
    while(1)
	{
	   
	OLED_ShowString(1,1,"this is mode3");
		
	  if(Key1_read() == 0)
	  {
	    OLED_Clear();
		return 0;
	  }
	}
}

int mode4(void)
{
    while(1)
	{
	   
	OLED_ShowString(1,1,"this is mode4");
		
	  if(Key1_read() == 0)
	  {
	    OLED_Clear();
		return 0;
	  }
	}
}
	


