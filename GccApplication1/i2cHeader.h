#define F_SCL		100000
#define PRESCALE	1
#define BIT_RATE	((F_CPU/F_SCL)-16)/(2 * PRESCALE)

/************************************************************
* i2cMasterInit function: Initializes I2C Master.
* @param bitRate	:	bit rate for communication
* @return			:	void
*************************************************************/
void i2cMasterInit(uint32_t bitRate);

/************************************************************
* i2cMasterStart function: Function for sending start bit.
* @param slaveAddr	:	Address of the slave with R/W bit.
* @return			:	void
*************************************************************/
int i2cMasterStart(uint8_t Addr);

/************************************************************
* i2cMasterRepStart function: Function for sending repeated 
* start bit.
* @param slaveAddr	:	Address of the slave with R/W bit.
* @return			:	void
*************************************************************/
int i2cMasterRepStart(uint8_t Addr);

/************************************************************
* i2cMasterStop function: Function for sending stop bit.
* @param			:	void
* @return			:	void
*************************************************************/
void i2cMasterStop(void);

/************************************************************
* i2cMasterSend function: Function for sending addr and data.
* @param data		:	data for sending to slave.
* @return			:	0 on Success, Negative value on 
						failure.
*************************************************************/
int i2cMasterSend(uint8_t data);

/************************************************************
* i2cMasterReadAck function: Function for reading data from
* given address, sends ACK after completion.
* @return			:	returns data received
*************************************************************/
uint8_t i2cMasterReadAck();

/************************************************************
* i2cMasterReadNoAck function: Function for reading data from
* given address, does not sends ACK after completion.
* @return			:	returns data received.
*************************************************************/
uint8_t i2cMasterReadNoAck();

/************************************************************
* i2cWaitForComplete function: Function used for checking flag
* raised by hardware (TWINT in TWCR register).
* @param 			:	void
* @return			:	void
*************************************************************/
void i2cWaitForComplete(void);

