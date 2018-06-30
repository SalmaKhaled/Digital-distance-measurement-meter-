#include "tm4c123gh6pm.h"
//#define GPIO_PORTA_AMSEL
//#define GPIO_PORTA_PCTL
#define our_ECHO (0x04) //PA2(INput)
#define our_TRIGGER (0x10) //PA4(OUTPUT)
volatile unsigned long delay;
uint32_t counter =0;
float distanceFunc=0;
int distance,distanceunits,distancetens,distancehunderds ;
#include "TM4C123.h"                   // Device header
#include <stdint.h>
#define MAX_TIME 7500

void PortA_Init(void)
{
	SYSCTL_RCGCGPIO_R |=0x01;
	delay=0;
	//GPIO_PORTA_LOCK_R =
	GPIO_PORTA_AMSEL_R = 0x00;
	GPIO_PORTA_CR_R |= 0xff;
	GPIO_PORTA_PCTL_R = 0x00000000;
	GPIO_PORTA_DIR_R |=0x18; //OUTTPUT TRIGER PA4
	GPIO_PORTA_DIR_R &=0xFB; //INPUT ECCO PA2
	GPIO_PORTA_DEN_R |= 0x1c;
}
/// for units
void PortB_Init(void) 
{
	//SYSCTL_RCGCGPIO_R =0x02;
	//delay=0;
	//GPIO_PORTA_LOCK_R =
	GPIO_PORTB_AMSEL_R = 0x00;
	GPIO_PORTB_CR_R = 0x0f;
	//GPIO_PORTB_PCTL_R = 0x00000000;
	GPIO_PORTB_DIR_R |= 0x0f;
	GPIO_PORTB_DEN_R |= 0x0f;
}

void PortC_Init(void)//blash
{
	//SYSCTL_RCGCGPIO_R =0x03;
	//delay=0;

	//GPIO_PORTA_LOCK_R =
	GPIO_PORTC_AMSEL_R = 0x00;
	GPIO_PORTC_CR_R = 0x0f;
	//GPIO_PORTC_PCTL_R = 0x00000000;
	GPIO_PORTC_DIR_R |= 0x0f;
	GPIO_PORTC_DEN_R |= 0x0f;
}
///for tens
void PortD_Init(void)
{
	//SYSCTL_RCGCGPIO_R =0x04;
	//delay=0;
	//GPIO_PORTA_LOCK_R =
	GPIO_PORTD_AMSEL_R = 0x00;
	GPIO_PORTD_CR_R = 0x0f;
	//GPIO_PORTD_PCTL_R = 0x00000000;
	GPIO_PORTD_DIR_R |= 0x0f;
	GPIO_PORTD_DEN_R |= 0x0f;
}
///for hunderds
void PortE_Init(void)
{
	//SYSCTL_RCGCGPIO_R =0x04;
	//delay=0;
	//GPIO_PORTA_LOCK_R =
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_CR_R = 0x0f;
	//GPIO_PORTD_PCTL_R = 0x00000000;
	GPIO_PORTE_DIR_R |= 0x0f;
	GPIO_PORTE_DEN_R |= 0x0f;
}
void timer0_INit (void)
{
	SYSCTL_RCGCTIMER_R |=0x01;
	delay =0;
	TIMER0_CTL_R &=~ 0x00000001;
	TIMER0_CFG_R =0x00000000;
	TIMER0_TAMR_R=0x00000002;
	TIMER0_TAMR_R |=0x00000010;
	//we defined this stupid bta3
	TIMER0_TAILR_R = 0x00516155;
	TIMER0_CTL_R |=0x00000001;
}
	void PLL_Init(void) //de lbt5li l clk bt3t l tiva c tsht3'l 3la 80 MHz
	{
		SYSCTL_RCC2_R|=0x80000000;
		SYSCTL_RCC2_R|=0x00000800;
		SYSCTL_RCC_R=(SYSCTL_RCC_R &~0x000007C0)+0x0000540;
		SYSCTL_RCC2_R &= ~0x00000070;
		SYSCTL_RCC2_R &=~0x00002000;
		SYSCTL_RCC2_R |=0x40000000;
		SYSCTL_RCC2_R=(SYSCTL_RCC2_R &~0x1fc00000)+(4<<22);
		while ((SYSCTL_RIS_R&0x00000040)==0){};
			SYSCTL_RCC_R &= ~0x00000800;
	}
	void delay_Microsecond(uint32_t time)
	{ unsigned int i;
		
		timer0_INit ();
		time = time/1000000;
	//	time=time*(1/100000);
		/*SYSCTL_RCGCTIMER_R |=0x02; 
    TIMER1_CTL_R &=0;
    TIMER1_CFG_R =0x04;
    TIMER1_TAMR_R=0x02;
    TIMER1_TAILR_R =3;
    TIMER1_ICR_R =0x1;
    TIMER1_CTL_R |=0x01;*/
		for(i=0;i<time;i++)
		{
			
			 while((TIMER0_RIS_R &&0x01)==0) {};
				
        TIMER0_ICR_R = 0x1;
		}
	
	}
	
	float measureD(void){
		
    GPIO_PORTA_DATA_R &=~our_TRIGGER;
    delay_Microsecond(10);
    GPIO_PORTA_DATA_R|=our_TRIGGER;
    delay_Microsecond(10);
    GPIO_PORTA_DATA_R &=~our_TRIGGER;
    counter =0;
    while((GPIO_PORTA_DATA_R & our_ECHO )==0)  {}
    while(((GPIO_PORTA_DATA_R & our_ECHO )!=0) & (counter < MAX_TIME)) 
       { 
           counter++; 
           delay_Microsecond(1);
					GPIO_PORTA_DATA_R |=0x08;
       } 
 distanceFunc = (float)counter*(float)0.0170000*20;
 return distanceFunc; } 
	////spliting
	int units( int distance)
	{
		return (distance % 10);
	}
	int tens( int distance)
	{
		return ((distance%100) - (distance % 10))/10;
	}
	int hunderds( int distance)
	{
		return (distance - (distance % 100))/100;
	}
	///converting to binary
	int main ()
	{
		
		SYSCTL_RCGCGPIO_R =0x3f;
		PortA_Init();
		PortB_Init();
		PortD_Init();
		PortE_Init();
		/*PortB_Init();
		PortC_Init();
		PortD_Init(); */
		//SYSCTL_RCGCGPIO_R =0x06;
	delay=0;
	//GPIO_PORTA_LOCK_R =
 //SYSCTL_RCGCGPIO_R = 0x20;
	GPIO_PORTF_DIR_R |= 0x0e;
	GPIO_PORTF_DEN_R |= 0x0e;
	
		
		timer0_INit();
		PLL_Init();
		while(1)
		{ 
			distance = measureD();
		//	distance = 564;
			distanceunits = units(distance);
			distancetens = tens (distance);
			distancehunderds = hunderds (distance);
			////PORTB for units
			GPIO_PORTB_DATA_R = (distanceunits % 2) & 0x01;
			GPIO_PORTB_DATA_R |= (((distanceunits/2)%2)<<1)& 0x02;
			GPIO_PORTB_DATA_R |=  (((distanceunits/4)%2)<<2)&0x04;
			GPIO_PORTB_DATA_R |= (((distanceunits/8)%2)<<3)& 0x08;
			////PORTD for tens
			GPIO_PORTD_DATA_R =  (distancetens % 2) & 0x01;
			GPIO_PORTD_DATA_R |= (((distancetens/2)%2)<<1)&0x02;
			GPIO_PORTD_DATA_R |= (((distancetens/4)%2)<<2)&0x04;
			GPIO_PORTD_DATA_R |= (((distancetens/8)%2)<<3)& 0x08;
			////PORTE for hunderds
			GPIO_PORTE_DATA_R =  (distancehunderds% 2) &0x01;
			GPIO_PORTE_DATA_R |= (((distancehunderds/2)%2)<<1)& 0x02;
			GPIO_PORTE_DATA_R |=  (((distancehunderds/4)%2)<<2)&0x04;
			GPIO_PORTE_DATA_R |= (((distancehunderds/8)%2)<<3)& 0x08;
			delay_Microsecond(1000000);
			/*if((TIMER0_RIS_R &&0x01)==1)
			{*/
		//	GPIO_PORTF_DATA_R =0x06;
			   //measureD();
			//float test=measureD();
			/*if(measureD()<10)
			{
				
						GPIO_PORTF_DATA_R =(1<<2);
			}
else if(measureD()>10&&measureD()<20)
{
				GPIO_PORTF_DATA_R =0x02;
}
else
				GPIO_PORTF_DATA_R =0x08;
delay_Microsecond(10);
			//	delay_Microsecond(1000000);
				//GPIO_PORTF_DATA_R ^=(1<<2);
				//GPIO_PORTB_DATA_R ^=0x01;
				//TIMER0_ICR_R |= (1<<0);
			//}*/
		}
	}
	
	

