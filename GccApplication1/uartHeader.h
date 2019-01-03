#define BAUD_RATE		9600
#define MYUBRR			F_CPU/16/BAUD_RATE-1

int uartInit(unsigned int baudRate);
int uartSendData(char *str);
int uartRecvData(char *str, unsigned int length);