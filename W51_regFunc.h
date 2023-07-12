/*
 * W51_regFunc.h
 *
 * Created: 30.06.2023 21:14:31
 *  Author: filip
 */ 


#ifndef W51_REGFUNC_H_
#define W51_REGFUNC_H_





#endif /* W51_REGFUNC_H_ */


#define WIZNET_WRITE_OPCODE 0xF0
#define WIZNET_READ_OPCODE 0x0F

// Wiznet W5100 Register Addresses
#define MR   0x0000   // Mode Register
#define GAR  0x0001   // Gateway Address: 0x0001 to 0x0004
#define SUBR 0x0005   // Subnet mask Address: 0x0005 to 0x0008
#define SAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR 0x000F   // Source IP Address: 0x000F to 0x0012
#define RMSR 0x001A   // RX Memory Size Register
#define TMSR 0x001B   // TX Memory Size Register
#define TXBUF_BASE 0x4000 //TX buffer base
#define TXBUF_MASK 0x07FF //TX buffer mask
#define RXBUF_BASE 0x6000 //RX buffer base
#define RXBUF_MASK 0x07FF //RX buffer mask

#define S0_BASE 0x0400 //socket 0 register base
#define S1_BASE 0x0500 //socket 1 register base
#define S2_BASE 0x0600 //socket 2 register base
#define S3_BASE 0x0700 //socket 3 register base

//socket register defines
#define S_MR 0x00 //mode register
#define S_CR 0x01 //command register
#define S_SR 0x03 //status register
#define S_SPORT 0x04 //source port register (2)
#define S_DAR 0x06 //dist MAC address (6)
#define S_DIPR 0x0C //dist IP (4)
#define S_DPORT 0x10 //dist port (2)
#define S_TX_FSR 0x20 //TX free size register (2)
#define S_TX_RD 0x22 //TX read pointer register (2)
#define S_TX_WR 0x24 //TX write pointer register (2)
#define S_RX_RSR 0x26 //RX received size register (2)
#define S_RX_RD 0x28 //RX read pointer (2)

#define W51_ERR_SOCK_OPEN 0x01
#define W51_OK 0x02
#define W51_ERR_WRONG_PROTOCOL 0x03

#define W51_PROTOCOL_UDP 0x02
#define W51_PROTOCOL_TCP 0x01

#define CS PINB2
#define SPI_PORT PORTB
#define SCK PINB5
#define MOSI PINB3

void W51_write(uint16_t addr, uint8_t data);
uint8_t W51_read(uint16_t addr);
void W51_reset();
void W51_init();