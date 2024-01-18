#include <LPC214x.H> 
#include "i2c_driver.h" 

typedef unsigned char UNS_8;
typedef unsigned short UNS_16;
typedef unsigned int UNS_32;


extern int sendchar (int ch);
extern void init_serial (void);
extern void sendascii (int );
extern void sendstring(char *);
extern void i2cInit(UNS_32 );
extern UNS_8 i2c_buf;



#define SENS_TEMP_ADDR 0x91

int main(void){ 

  char msg[]="Temp (Celsius): \0";

	init_serial();
	i2cInit(100000);
	sendstring(msg);
	//start bit
	I2C0CONSET = 0x60;

	while(1);
}


