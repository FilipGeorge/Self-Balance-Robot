/*
 * I2CMaster.h
 *
 * Created: 5/25/2016 10:57:10 AM
 *  Author: Iulian
 */


#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define I2C_BIT_READ  1
#define I2C_BIT_WRITE 0

#define I2C_F_SCL     100000UL
#define I2C_PRESCALER 1
#define I2C_TWBR_VAL  ((((F_CPU / I2C_F_SCL) / I2C_PRESCALER) - 16) / 2)

class I2CMaster {
public:

public:
  /**
  *  Initialize the TWI Master interface. Call it only once.
  */
  void init();

  /**
  *   Issue a start condition and sends address and data direction
  *
  *   @return true if device is accessible, false otherwise
  */
  bool start(uint8_t addressAndDirection);

  /**
  *   Terminates a data transfer and releases the I2C bus
  */
  void stop();

  /**
  *   Sends one byte to device
  *
  *   @return true on success, false otherwise
  */
  bool write(uint8_t data);

  /**
  *   Read one byte from I2C device and request more data
  *
  *   @return the byte read
  */
  uint8_t readACK();

  /**
  *   Read one byte from I2C device, read is followed by a stop condition
  *
  *   @return the byte read
  */
  uint8_t readNACK();

  /**
  *   Read one byte from I2C device
  *
  *   @param ack if ack = true request more data, otherwise send stop condition
  *   @return the byte read
  */
  uint8_t read(bool ack);

  /**
  *   Transmits an array of bytes to specified device
  *
  *   @return 
  */
  bool transmit(uint8_t address, uint8_t* data, uint16_t length);

  /**
  *   Receive an array of bytes from specified device
  *
  *   @return 
  */
  bool receive(uint8_t address, uint8_t* data, uint16_t length);


  /**
  *   Write one byte to a specific register
  *
  *   @return 
  */
  bool writeRegister(uint8_t devAddress, uint8_t regAddress, uint8_t data);

  /**
  *   Write an array of bytes to a specific register
  *
  *   @return 
  */
  bool writeRegister(uint8_t devAddress, uint8_t regAddress, uint8_t* data, uint16_t length);
  
  /**
  *   Read one byte from a specific register
  *
  *   @return 
  */
  bool readRegister(uint8_t devAddress, uint8_t regAddress, uint8_t* data); 

  /**
  *   Read one or more bytes from a specific register
  *
  *   @return 
  */
  bool readRegister(uint8_t devAddress, uint8_t regAddress, uint8_t* data, uint16_t length);  
};

#endif /* I2CMASTER_H_ */