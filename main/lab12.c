#include <LPC214x.H> 
#include "i2c_driver.h" 
#define MASK_ALL_P0 0x40000000
#define MASK_ALL_P1 0x03FF0000
#define DATA_MASK 0xFF
#define RIGHT 1
#define LEFT 0

typedef unsigned char UNS_8;
typedef unsigned short UNS_16;
typedef unsigned int UNS_32;


extern int sendchar (int ch);
extern void init_serial (void);
extern void sendascii (int );
extern void sendstring(char *);
extern void i2cInit(UNS_32 );
extern UNS_8 i2c_buf;
extern void init_LCD(void);
extern void send_Text(UNS_8 *text);
extern void cursor_move(int spaces, int dir);
extern void delay_ms(unsigned int count);
extern int old_temp;
extern void temp(void afisare(void));

int tempMeas;


#define SENS_TEMP_ADDR 0x91

int main(void){ 

  //char msg[]="Temp (Celsius): \0";
	UNS_8 text[] = "Temp(C): ";

	//init_serial();
	IODIR0  = IODIR0 | 0x0000FF00; //Configure the P0 pins as OUTPUT;
		
	IOSET0 = 0XFF00;
	IO1DIR = MASK_ALL_P1;//GPIO Port Direction control register
  IO0DIR = MASK_ALL_P0;//GPIO Port Direction control register
	
	init_LCD();
	
	i2cInit(100000);
	
	tempMeas=1;
	
	//sendstring(msg);
	//start bit
	

  
  send_Text(text);
	



	IO0DIR = (1<<15);
	while(1) {
		
		if (tempMeas) {
			if ( old_temp>29)
			{
				
        IO0SET = (1<<15);
				
				//Turn ON LED
			}
			else
			{
        
        IO0CLR = (1<<15); 	
				
         					//Turn LED OFF
			}
			

			I2C0CONSET = 0x60;
			
			tempMeas=0;
			
			
			cursor_move(8, LEFT);
			
			
			
		}

		
	}
	
	
}


