#include <reg52.h>


bit uart_busy = 0;

/* ----------------------
	  ��ʱ5����
---------------------- */
void delay5ms() {
    unsigned char a,b;
    for(b=101;b>0;b--)
        for(a=147;a>0;a--);
}

/* ----------------------
	  ��ʼ�����ڣ�������9600
---------------------- */
void Init_uart(void) {  
	TMOD = TMOD | 0x20;    //��ʱ��1�����ڷ�ʽ2  8λ�Զ���װ  
	SCON = SCON | 0x50;    //����1�����ڷ�ʽ1  10λ�첽�շ� REN=1�������  
	TH1 = 0xFd;            //��ʱ��1��ֵ  
	TL1 = TH1;  
	TR1 = 1;               //��ʱ��1��ʼ����  
	EA =1;                 //�����ж�  
	ES =1;                 //������1�ж�  
}


/* ----------------------
	���ڷ������ݷ���
---------------------- */
void Uart_SendByteData(unsigned char msg){
	while(uart_busy); //�ȴ���һ�����ݷ������
	SBUF=msg;		  
	uart_busy = 1;	 //���ڷ�����æ
}
void Uart_SendStrData(unsigned char *msg){
	while(*msg){
		Uart_SendByteData(*msg++);
	}
}


/* ----------------------
	��ʼ��WIFIģ��
---------------------- */
void Init_ESP01S(){   
	Uart_SendStrData("AT+CIPMUX=1\r\n");            // ���ö�����
	delay5ms();   
	Uart_SendStrData("AT+CIPSERVER=1,8080\r\n");      // ���÷�����������8080�˿�
	delay5ms();
}


void main(){
	Init_uart();
	Init_ESP01S();    // Init_ESP01S �� Init_uart���õ�˳���ܸı�
	
	while(1);
}


/* ----------------------
	  �����ж�
---------------------- */
void UART_Interrupt(void) interrupt 4 {
	unsigned char one_char;
	
	if(RI){
		one_char = SBUF;
		RI = 0;
		/* ����ESP-01S���͵����ڵ����� */
	
	}
	
	// ����������ж�
	if(TI){
		TI = 0;
		uart_busy = 0;
	}
	
}
