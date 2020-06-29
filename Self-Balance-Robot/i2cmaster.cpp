/*
 * I2CMaster.cpp
 *
 * Created: 9/10/2015 10:57:00 AM
 *  Author: Iulian
 */ 

#include "i2cmaster.h"
#include <util/twi.h>

void I2CMaster::init()
{    
  // Activate internal pull-up
  DDRC &= ~((1 << PC4) | (1 << PC5));
  PORTC |= (1 << PC4) | (1 << PC5);
  
  TWSR &= ~((1 << TWPS0) | (1 << TWPS1));
  TWBR = 0x48; //  (uint8_t)I2C_TWBR_VAL;
}

bool I2CMaster::start(uint8_t addressAndDirection)
{  
  // Reset control register
  TWCR = 0;

  // Transmit Start condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  // Wait for end of transmission
  while(!(TWCR & (1 << TWINT)));
  
  // Check if start condition was successfully transmitted
  if((TWSR & 0xF8) != TW_START) {
    return false;
  }

  // Load slave address into data register
  TWDR = addressAndDirection;
  // Start transmission of the address
  TWCR = (1 << TWINT) | (1 << TWEN);
  // Wait for end of transmission
  while(!(TWCR & (1 << TWINT)));
  
 // delay_ms(10);
  // Check if the device has acknowledged the READ/WRITE mode
  uint8_t tempTWST = TWSR & 0xF8;
  if((tempTWST != TW_MT_SLA_ACK) && (tempTWST != TW_MR_SLA_ACK)) {
    return false;
  }

  return true;
}

void I2CMaster::stop()
{
  // Transmit Stop condition
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

bool I2CMaster::write(uint8_t data)
{
  // Load data into data register
  TWDR = data;
  // Start transmission of data
  TWCR = (1 << TWINT) | (1 << TWEN);
  // Wait for end of transmission
  while(!(TWCR & (1 << TWINT)));

  if((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    return false;
  }

  return true;
}

uint8_t I2CMaster::readACK()
{
  // Start TWI module and acknowledge data after reception
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
  // Wait for end of transmission
  while(!(TWCR & (1 << TWINT)));
  
  // Return received data from TWDR
  return TWDR;
}

uint8_t I2CMaster::readNACK()
{
  // Start TWI module and without acknowledging the reception
  TWCR = (1 << TWINT) | (1 << TWEN);
  // Wait for end of transmission
  while(!(TWCR & (1 << TWINT)));
  
  // Return received data from TWDR
  return TWDR;
}

uint8_t I2CMaster::read(bool ack)
{
  if(ack) {
    return readACK();
  } else {
    return readNACK();
  }
}

bool I2CMaster::transmit(uint8_t address, uint8_t* data, uint16_t length)
{
  // Try to send start condition + slave address + write bit
  if(!start((address << 1) | I2C_BIT_WRITE)) {
    return false;
  }

  // Send each byte individually 
  for(uint16_t i = 0; i < length; i++) {
    if(!write(data[i])) {
      return false;
    }
  }

  // Send stop condition
  stop();

  return true;
}

bool I2CMaster::receive(uint8_t address, uint8_t* data, uint16_t length)
{
  // Try to send start condition + slave address + read bit
  if(!start((address << 1) | I2C_BIT_READ)) {
	  return false;
  }

  // Read each byte individually
  for(uint16_t i = 0; i < length - 1; i++) {
	  data[i] = readACK();
  }

  // Do not ACK last byte, slave will know to stop sending
  data[length - 1] = readNACK();

  // Send stop condition
  stop();

  return true;
}

bool I2CMaster::writeRegister(uint8_t devAddress, uint8_t regAddress, uint8_t data)
{
	// Try to send start condition + slave address + write bit
	if(!start((devAddress << 1) | I2C_BIT_WRITE)) {
    return false;
	}

	// Send register address
	write(regAddress);

	// Send data to write into register
	if(!write(data)) {
		return false;
	}

	// Send stop condition
	stop();

	return true;
}

bool I2CMaster::writeRegister(uint8_t devAddress, 
                              uint8_t regAddress,
                              uint8_t* data,
                              uint16_t length)
{
  // Try to send start condition + slave address + write bit
  if(!start((devAddress << 1) | I2C_BIT_WRITE)) {
	  return false;
  }

  // Send register address
  write(regAddress);

  // Send data to write into register
  for(uint16_t i = 0; i < length; i++) {
    if(!write(data[i])) {
      return false;
    }
  }

  // Send stop condition
  stop();

  return true;
}

bool I2CMaster::readRegister(uint8_t devAddress, uint8_t regAddress, uint8_t* data)
{
	// Try to send start condition + slave address + write bit
	if(!start((devAddress << 1) | I2C_BIT_WRITE)) {
		return false;
	}

	// Send register address
	write(regAddress);

  // Send stop condition
  stop();

	// Try to send start condition + slave address + read bit
	if(!start((devAddress << 1) | I2C_BIT_READ)) {
		return false;
	}

	// Do not ACK byte, slave will know to stop sending
	*data = readNACK();

	// Stop condition
	stop();

	return true;
}

bool I2CMaster::readRegister(uint8_t devAddress, 
                             uint8_t regAddress, 
                             uint8_t* data, 
                             uint16_t length)
{
  // Try to send start condition + slave address + write bit
  if(!start((devAddress << 1) | I2C_BIT_WRITE)) {
	  return false;
  }

  // Send register address
  write(regAddress);

  // Send stop condition
  stop();

  // Try to send start condition + slave address + read bit
  if(!start((devAddress << 1) | I2C_BIT_READ)) {
	  return false;
  }
  
  // Read each byte individually
  for(uint16_t i = 0; i < length - 1; i++) {
    data[i] = readACK();
  }

  // Do not ACK last byte, slave will know to stop sending
  data[length - 1] = readNACK();

  // Stop condition
  stop();

  return true;
}
