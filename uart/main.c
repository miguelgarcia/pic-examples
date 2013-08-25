/*
 UART example tested on pic16f887

 When the PIC receives a character it will transmit character+1
	Example: if you send hal the PIC will respond ibm. 
*/

/* Define processor and include header file. */
#define __16f887
#include"pic14/pic16f887.h"

/* Setup chip configuration */
typedef unsigned int config;
__at(_CONFIG1) config __CONFIG = 
	_INTOSC	// 4Mhz
	& _WDT_OFF
	& _PWRTE_ON
	& _MCLRE_OFF
	& _CP_OFF & _CPD_OFF & _BOR_OFF & _IESO_ON & _FCMEN_ON & _LVP_OFF; 

__at(_CONFIG2) config __CONFIG2 =  _BOR40V & _WRT_OFF;

void init_uart(void); 
void UART_putc(unsigned char c); 

unsigned char cUART_char; 
unsigned char cUART_data_flg;

void main()
{
	TRISA = 0;
	PORTA = 0;
	TRISB = 0;
	PORTB = 0;
	TRISD = 0;
	PORTD = 0;
	ANSELH = 0;

	init_uart(); // init UART module

	while (1) // infinite loop which handles ncoming data as they arrive   
	{
		if (cUART_data_flg==1)// if new data available, send it back through USART tx line (echo it)       
		{           
			UART_putc(cUART_char);            
			cUART_data_flg=0; // clear new data flag so one charactor will echoed once    
		}
	}
}

void init_uart(void)
{
	TRISC7=1;	//Make UART RX pin input   
	TRISC6=0;	//Make UART TX pin output   
	SYNC = 0;	// enables for asynchronous EUART
	SPEN = 1;	// enables EUSART and sets TX (RC6) as output; ANSEL must be cleared if shared with analog I/O
	CREN = 1;
	TX9 = 0;	// 8bit mode       RX9 = 0;
	TXEN = 1;	// enables Transmitter
	BRGH = 1;	// baud rate select     BRG16 = 0;
	SPBRG = 25;	//baud rate select  9600@4Mhz   SPBRGH = 0;
	RCIE=1;		// receive interrupt enable
	GIE=1;		// global interrupt enable
	PEIE=1;		// Peripheral Interrupt Enable bit
	BRG16 = 0;
}

void UART_putc(unsigned char c)
{ 
	TXREG=c+1; // load txreg with data 
	while(TRMT==0) // wait here till transmit complete 
	{    
	}
}

static void irq_handler() __interrupt 0
{
	if (RCIF==1)
	{
		if(RCSTA&0x06)
		{     
			CREN=0; //Overrun error (can be cleared by clearing bit CREN)     
			cUART_char=RCREG; //clear Framing error      
			CREN=1;   
		}
		else   
		{            
			cUART_char = RCREG; // read new data into variable       
			cUART_data_flg = 1; // new data received. so enable flg   
		}
	}
}
