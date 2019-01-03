/*
 * GccApplication1.c
 *
 * Created: 12/24/2018 12:20:45 AM
 * Author : Srihari Rao Melagiri
 */ 

#ifndef F_CPU
#define F_CPU		16000000L
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2cHeader.h"
#include "uartHeader.h"
#include "mpu6050.h"

#define MPU_6050		0x68
#define MPU_6050_WRITE_ADDR	0xD0
#define MPU_6050_READ_ADDR	0xD1

uint8_t data = 0;
float accX = 0.0, accY = 0.0, accZ = 0.0;
float temperature = 0.0;
float gyroX = 0.0, gyroY = 0.0, gyroZ = 0.0;
int returnValue = 0;

void mpu6050Init(){								/* Gyro initialization function */
	
	_delay_ms(150);								/* Power up time >100ms */
	i2cMasterStart(MPU_6050_WRITE_ADDR);					/* Start with device write address */
	i2cMasterSend(SMPLRT_DIV);						/* Write to sample rate register */
	i2cMasterSend(0x07);							/* 1KHz sample rate */
	i2cMasterStop();

	i2cMasterStart(MPU_6050_WRITE_ADDR);
	i2cMasterSend(PWR_MGMT_1);						/* Write to power management register */
	i2cMasterSend(0x01);							/* X axis gyroscope reference frequency */
	i2cMasterStop();

	i2cMasterStart(MPU_6050_WRITE_ADDR);
	i2cMasterSend(CONFIG);							/* Write to Configuration register */
	i2cMasterSend(0x00);							/* Fs = 8KHz */
	i2cMasterStop();
	
	i2cMasterStart(MPU_6050_WRITE_ADDR);
	i2cMasterSend(GYRO_CONFIG);						/* Write to Gyro configuration register */
	i2cMasterSend(0x18);							/* Full scale range +/- 2000 degree/C */
	i2cMasterStop();

	i2cMasterStart(MPU_6050_WRITE_ADDR);
	i2cMasterSend(INT_ENABLE);						/* Write to interrupt enable register */
	i2cMasterSend(0x01);
	i2cMasterStop();
	
}

void mpuStartLoc()
{
	i2cMasterStart(MPU_6050_WRITE_ADDR);					/* I2C start with device write address */
	i2cMasterSend(ACCEL_XOUT_H);						/* Write start location address from where to read */
	i2cMasterRepStart(MPU_6050_READ_ADDR);					/* I2C start with device read address */
}

void readRawValues()
{
	mpuStartLoc();
	
	accX = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadAck()));
	accY = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadAck()));
	accZ = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadAck()));
	temperature = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadAck()));
	gyroX = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadAck()));
	gyroY = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadAck()));
	gyroZ = (((int)i2cMasterReadAck() << 8) | ((int)i2cMasterReadNoAck()));

	i2cMasterStop();
}

int main(void){

	char buffer[20] = {'\0'}, float_[10] = {'\0'};
	float Xa = 0.0, Ya = 0.0, Za = 0.0, t = 0.0;
	float Xg = 0.0, Yg = 0.0, Zg = 0.0;
	
	uartInit(MYUBRR);
	
	i2cMasterInit(BIT_RATE);
	uartSendData("INIT I2C DONE\r\n");
	_delay_ms(200);
	
	mpu6050Init();
	
	while (1){
		
		readRawValues();
		
		Xa = accX/16384.0;
		Ya = accY/16384.0;
		Za = accZ/16384.0;

		Xg = gyroX/16.4;
		Yg = gyroY/16.4;
		Zg = gyroZ/16.4;

		t = (temperature/340.00)+36.53;					/* Convert temperature in °/c using formula */		
		
		dtostrf( Xa, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," Ax = %s g\t", float_);
		uartSendData(buffer);
		
		dtostrf( Ya, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," Ay = %s g\t", float_);
		uartSendData(buffer);
		
		dtostrf( Za, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," Az = %s g\t", float_);
		uartSendData(buffer);
		
		dtostrf( t, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," t = %s%cC \t", float_, 0xF8);
		uartSendData(buffer);
		
		dtostrf( Xg, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," Xg = %s%c/s \t", float_, 0xF8);
		uartSendData(buffer);
		
		dtostrf( Yg, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," Yg = %s%c/s \t", float_, 0xF8);
		uartSendData(buffer);
		
		dtostrf( Zg, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer," Zg = %s%c/s \r\n", float_, 0xF8);
		uartSendData(buffer);												
		
		_delay_ms(1000);
	}
	
	return 0;
}
