
//Core file for W5100 register operations

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include "W51_regFunc.h"

void W51_init(void) //function to initialize SPI interface
{
	DDRB |= (1 << CS) | (1 << SCK) | (1 << MOSI); //set SCK, MOSI, and chip select(CS) as output
	PORTB |= (1 << CS); //set CS high (device is unselected)
	SPCR = (1 << SPE) | (1 << MSTR); //enable SPI master mode
	#if F_CPU > 14000000 //if speed is higher than 14MHz
	SPSR |= (1 << SPI2X); //set SPI speed to Fosc/2 because max speed is 14MHz
	#endif
	_delay_us(100); //wait for SPI init
}

static void SPI_write(uint8_t data){ //function to send data over SPI
	SPDR = data; //write data to spi data register
	while(!(SPSR & (1 << SPIF))); //wait until data are sent
}

void W51_write(uint16_t addr, uint8_t data){ //function to write single byte value to register in W5100
	SPI_PORT &= ~(1 << CS); //pull CS low to make W5100 listen to data
	
	SPI_write(WIZNET_WRITE_OPCODE); //send write opcode
	
	SPI_write((addr & 0xFF00) >> 8); //send MSB of register address first (according to datasheet)
	
	SPI_write(addr & 0x00FF); //send LSB of register address
	
	SPI_write(data); //send data
	
	SPI_PORT |= (1 << CS); //set CS high to stop data listening
}

uint8_t W51_read(uint16_t addr){ //function to read single byte value register from W5100
	SPI_PORT &= ~(1 << CS); //pull CS low to make W5100 listen to data
	
	SPI_write(WIZNET_READ_OPCODE); //send read opcode
	
	SPI_write((addr & 0xFF00) >> 8); //send MSB of register address first (according to datasheet)
	
	SPI_write(addr & 0x00FF); //send LSB of register address
	
	SPI_write(0x00); //send dummy (NOP) byte to read out the register value
	
	SPI_PORT |= (1 << CS); //set CS high to stop data listening
	
	return SPDR; //return SPI data register
}

void W51_reset(){ //function to soft-reset W5100
	W51_write(MR, 0x80); //write 0x80 to reset chip (bit 8 = reset)
	_delay_ms(100); //delay
}