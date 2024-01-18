#include <lpc214x.h>												
#include "i2c_driver.h"
#define SLAVE_ADDR 0x91
#define BUF_LEN 2
#define MASK_P8		0x00000100
typedef unsigned char UNS_8;
typedef unsigned short UNS_16;
typedef unsigned int UNS_32;
UNS_8 i2c_buf[BUF_LEN];
int cnt = 0;

void I2C_ISR(void) __irq;
void ISR_8(void);
void ISR_40(void);
void ISR_48(void);
void ISR_50(void);
void ISR_58(void);
void afisare(void);
void num2char(UNS_32 , char *);
int old_temp = 0;
int display = 0x100;

extern void sendstring (char * );
extern void send_Text(UNS_8 *text);

extern int tempMeas;

/******************************************************************************
 *
 * Description:
 *    Reset and initialize the I2C peripheral.
 *
 * Params:
 *    [in]  i2cFrequency  - frequency on clock signal in Hz (max 400 kHz)
 *
 *****************************************************************************/
void i2cInit(UNS_32 i2cFrequency)
{



	//connect I2C signals (SDA & SCL) to P0.2 and P0.3
  PINSEL0 &= ~0x000000F0;
  PINSEL0 |=  0x00000050;

  //clear flags
  I2C0CONCLR = 0x6c;
										   
  //set bit timing ans start with checking for maximum speed (400kHz)
  if (i2cFrequency > 400000)
    i2cFrequency = 400000;
  I2C0SCLH   = (((CRYSTAL_FREQUENCY * PLL_FACTOR) / VPBDIV_FACTOR) / i2cFrequency + 1) / 2;
  I2C0SCLL   = (((CRYSTAL_FREQUENCY * PLL_FACTOR) / VPBDIV_FACTOR) / i2cFrequency) / 2;


  	// Initialize VIC for I2C use 
  VICIntSelect = 0x0;	//selecting IRQ
  VICIntEnable = 0x200;	//enabling I2C
  VICVectCntl0 = 0x29; 	//highest priority and enabled
  VICVectAddr0 =(unsigned long) I2C_ISR;

  //reset registers
  I2C0ADR = 0x00;
  I2C0CONSET = 0x40;
}


void I2C_ISR(void) __irq
{
  int temp=0;
  
  temp=I2C0STAT;
  switch(temp)
  {
	case 0x08:
		ISR_8();
		break;
	case 0x40:
		ISR_40();
		break;
	case 0x48:
		ISR_48();
		break;
	case 0x50:
		ISR_50();
		break;
	case 0x58:
		ISR_58();
		break;
	default :
		break;
  }
	VICVectAddr = 0xFF;
}


void ISR_8(void)
{

// Slave address + read
  I2C0DAT = SLAVE_ADDR;
// Clear SI and Start flag
  I2C0CONCLR = 0x28;
}

//SLA+R has been transmitted; NACK has been received.
void ISR_48(void)
{

// Clear SI and Start flag
  I2C0CONCLR = 0x28;
}

//SLA+R has been transmitted; ACK has been received.
void ISR_40(void)
{
  I2C0CONSET = 0x04; //set AA
  I2C0CONCLR= 0x08;	//clear SI
}

void ISR_50(void)
{
  i2c_buf[cnt++] = I2C0DAT;  
  if (cnt < BUF_LEN) 
  {
     // send ACK 
    I2C0CONSET = 0x04; //set AA
    I2C0CONCLR= 0x08;	//clear SI
  } else 
  {
    // send NACK (last byte)
    I2C0CONCLR = 0x0C;  //clear AA si SI
  }
}

void ISR_58(void)
{
  I2C0CONSET = 0x14; //set STO and AA
  I2C0CONCLR= 0x08;	//clear SI
  afisare();
	cnt=0;
	tempMeas = 1;
} 


void afisare(void) 
{
	UNS_8 msg[9];
	char cat[4], rest[4];
	UNS_16 t;
	UNS_32 temp, cat_t, rest_t;

	t = (UNS_16) (i2c_buf[0] << 3) | (UNS_16)(i2c_buf[1] >> 5);
	temp = (UNS_32) t * 125;
	
	if((temp-old_temp)<0.5)
	{
		old_temp = temp;
		display = (display<<1)|0x100;
		
	IOSET0 = 0XFF00;
	IOCLR0 = display;
		
	
	}

	
	
	
	
	cat_t = temp / 1000;
	num2char(cat_t, cat);
	rest_t = temp % 1000;
	num2char(rest_t, rest);
	//msg[0] = cat[0];
	msg[0] = cat[1];
	msg[1] = cat[2];
	msg[2] = '.';
	msg[3] = rest[0];
	msg[4] = rest[1];
	msg[5] = rest[2];
	msg[6] = ' ';
	msg[7] = '\0';
	send_Text(msg); 
	
	i2c_buf[0] = 0xCD;
	i2c_buf[1] = 0x05;
	
}

void num2char(UNS_32 n, char *res)
{
	int i = 0;

	res[3] = '\0';
	for(i = 2; i >= 0; i--)
	{
	  res[i] = n % 10 + '0';
	  n = n / 10;
	} 
}
 


