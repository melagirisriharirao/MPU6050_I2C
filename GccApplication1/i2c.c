#include <avr/io.h>
#include <util/twi.h>
#include "i2cHeader.h"

void i2cMasterInit(uint32_t bitRate){
	
	TWBR = bitRate;				/*set bit rate*/
	
	TWDR = 0xFF;				/*Default content = SDA released*/
	
	TWCR = (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (1 << TWEN) | (0 << TWIE); /*ENABLE TWI (turn off remaining bits)*/
	
	return;
}

int i2cMasterStart(uint8_t Addr){

	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN); /*enable and send start bit*/
	
	i2cWaitForComplete();

	if((TWSR & 0xF8) != TW_START){
		return -1;
	}

	TWDR = Addr;								/*transmit address*/
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	i2cWaitForComplete();
	
	// check if the device has acknowledged the READ / WRITE mode

	if(((TW_STATUS & 0xF8) != TW_MT_SLA_ACK) && ((TW_STATUS & 0xF8) != TW_MR_SLA_ACK))
		return -1;	

	return 0;
}

int i2cMasterRepStart(uint8_t Addr){

	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN); /*enable and send start bit*/

	i2cWaitForComplete();

	if((TWSR & 0xF8) != 0x10){
		return -1;
	}

	TWDR = Addr;								/*transmit address*/
	TWCR = (1 << TWINT) | (1 << TWEN);

	i2cWaitForComplete();

	return 0;
}

void i2cMasterStop(void){

	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);	/*transmit stop bit*/
	
	return;
}

int i2cMasterSend(uint8_t data){

	TWDR = data;								/*transmit data*/
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	i2cWaitForComplete();
		
	if((TWSR & 0xF8) != TW_MT_DATA_ACK){
		return -1;
	}

	return 0;
}

uint8_t i2cMasterReadAck(){

	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);	/*send ack*/

	i2cWaitForComplete();
	
	return TWDR;
}

uint8_t i2cMasterReadNoAck(){
	
	TWCR = (1 << TWINT) | (1 << TWEN);		/*no need to send ack*/
	
	i2cWaitForComplete();
	
	return TWDR;
}

void i2cWaitForComplete(void){

	while(!(TWCR & (1 << TWINT))){
		/*wait for start command to be sent*/
	}
	
	return;
}