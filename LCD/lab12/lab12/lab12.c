#include <LPC214x.H>                    /* LPC21xx definitions                */
#include <stdio.h> 

typedef unsigned char UNS_8;
typedef unsigned short UNS_16;
typedef unsigned int UNS_32;

#define MASK_ALL_P0 0x40000000
#define MASK_ALL_P1 0x03FF0000
#define DATA_MASK 0xFF

#define PORT_OFFSET 16
#define RS 1<<(PORT_OFFSET+8)
#define EN 1<<(PORT_OFFSET+9)
#define LED_ON 1<<(PORT_OFFSET+14)
#define DISPLAY_SIZE 16

void delay()
{
	int i, j;
	for(i = 0, j = 0; i<10000; i++)
		j++;
}


void enable()
{
  IO1SET = EN;
  delay();
  IO1CLR = EN;
  delay();
}

void LCD_CMD(UNS_8 cmd)
{
  UNS_32 cmd32 = cmd<<PORT_OFFSET;
  IO1CLR = RS;
  IO1CLR = DATA_MASK<<PORT_OFFSET;
  IO1SET = cmd32;
  enable();
	
}

void LCD_DATA(UNS_8 data)
{
  UNS_32 data32 = data<<PORT_OFFSET;
  IO1SET = RS;
  IO1CLR = DATA_MASK<<PORT_OFFSET;
  IO1SET = data32;
  enable();		
}


void init_LCD()
{
   IO0SET = LED_ON;
   IO1CLR = EN;
   delay();
   LCD_CMD(0x3C);
   LCD_CMD(0x0F);
   LCD_CMD(0x06);
   LCD_CMD(0x01);

}

void change_Adress(UNS_8 adress)
{
 	LCD_CMD(0x80+adress);
}

void send_Text(UNS_8 *text)
{
  int i = 0;
  while(text[i]!='\0')
  {
  	if(i==(DISPLAY_SIZE+39))
		break;
  	if(i==DISPLAY_SIZE)
		change_Adress(40);
  	LCD_DATA(text[i]);
	i++;
	}
}

int main(void){ 

   UNS_8 text[] = "Acesta este un text de test pentru displayul de pe LCP2148";
   
   IO1DIR = MASK_ALL_P1;
   IO0DIR = MASK_ALL_P0;

   init_LCD();
   send_Text(text);



}


