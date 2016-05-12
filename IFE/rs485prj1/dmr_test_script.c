#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
#endif

#include "rs485.h"
#include "rs485.c"

void delay(int);

int main(){
	int cport_nr=0;	// /dev/ttymxc4
	int bdrate=9600;	// 9600 baud rate
	int nBytesRead = 0;
	char lag[7];
	int realDelay;
	unsigned char buff[512];	

	char mode[] = {'8', 'O', '1', 0};

	unsigned char pwrupstr[35] = {0x0B, 0x98, 0x0C, 0x53, 0x4F, 0x59, 0x56, 0x52, 0x55, 0x30,0x31, 0x81, 0xAF, 0x8F, 0xF8, 0x40};
	unsigned char rspstr[35] = {0x09, 0x9C, 0x5A};
	
	RS232_OpenComport(cport_nr, bdrate, mode);

	//input delay	
	printf("Enter timing delay(ms): ");
        scanf("%s", lag);
	realDelay = atoi(lag);
	
	//loop of pulling from serial and writing to serial
	while(1){
		buff[0] = '\0';
		nBytesRead = RS232_PollComport(cport_nr, buff, 512);
		buff[nBytesRead] = 0;
	
	
		if(memcmp(buff, pwrupstr, nBytesRead) == 0){
			RS232_cputs(cport_nr, (const char*)rspstr, 3);
        	}
		else
			printf("Failed to write to port\n");
		
		delay(realDelay);
	}
	
	RS232_CloseComport(cport_nr);
	
	
	return(0);
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}
