#include <reg52.h>


bit uart_busy = 0;

/* ----------------------
	  延时5毫秒
---------------------- */
void delay5ms() {
    unsigned char a,b;
    for(b=101;b>0;b--)
        for(a=147;a>0;a--);
}

/* ----------------------
	  初始化串口，波特率9600
---------------------- */
void Init_uart(void) {  
	TMOD = TMOD | 0x20;    //定时器1工作在方式2  8位自动重装  
	SCON = SCON | 0x50;    //串口1工作在方式1  10位异步收发 REN=1允许接收  
	TH1 = 0xFd;            //定时器1初值  
	TL1 = TH1;  
	TR1 = 1;               //定时器1开始计数  
	EA =1;                 //开总中断  
	ES =1;                 //开串口1中断  
}


/* ----------------------
	串口发送数据方法
---------------------- */
void Uart_SendByteData(unsigned char msg){
	while(uart_busy); //等待上一个数据发送完成
	SBUF=msg;		  
	uart_busy = 1;	 //串口发送置忙
}
void Uart_SendStrData(unsigned char *msg){
	while(*msg){
		Uart_SendByteData(*msg++);
	}
}


/* ----------------------
	初始化WIFI模块
---------------------- */
void Init_ESP01S(){   
	Uart_SendStrData("AT+CIPMUX=1\r\n");            // 设置多连接
	delay5ms();   
	Uart_SendStrData("AT+CIPSERVER=1,8080\r\n");      // 配置服务器，监听8080端口
	delay5ms();
}


void main(){
	Init_uart();
	Init_ESP01S();    // Init_ESP01S 与 Init_uart调用的顺序不能改变
	
	while(1);
}


/* ----------------------
	  串口中断
---------------------- */
void UART_Interrupt(void) interrupt 4 {
	unsigned char one_char;
	
	if(RI){
		one_char = SBUF;
		RI = 0;
		/* 解析ESP-01S发送到串口的数据 */
	
	}
	
	// 处理发送完成中断
	if(TI){
		TI = 0;
		uart_busy = 0;
	}
	
}
