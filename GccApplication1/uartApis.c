#include <avr/io.h>
#include <string.h>

int uartInit(unsigned int baudRate){
	
	UBRR0H = (unsigned char)((baudRate >> 8) & 0xF);
	UBRR0L = (unsigned char)(baudRate & 0xFF);

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0);	//8 bit mode 1 stop bit
	
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);	//enable transmit and receive
	
	return 0;
}

int uartSendData(char *str){
	
	int dataLen = strlen((char *)str);
	int loopVar = 0;
	
	for(loopVar = 0; loopVar < dataLen; loopVar++){
		while(!(UCSR0A & (1 << UDRE0))){
			//wait for enable
		}
		
		UDR0 = str[loopVar];
	}
	
	
	return 0;
}

int uartRecvData(char *str, unsigned int length){

	unsigned int loopVar = 0;
	unsigned char ch = '\0';
	
	if(length == 0){
		return 0;
	}	
	
	while(loopVar < (length - 1)){
	
		while(!(UCSR0A & (1 << RXC0))){
			//keep waiting until data arrives
		}
		
		ch = UDR0;
		
		if(ch == '\0')
			break;
		else
			str[loopVar++] = ch;

	}
	
	str[loopVar++] = '\0';
	
	return 0;
}
