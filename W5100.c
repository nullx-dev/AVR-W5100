#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include "W51_regFunc.h"
#include "W5100.h"

SOCKET createSocket(uint8_t sockNum, uint8_t* IP, uint16_t port, uint8_t protocol){ //function to create a new socket structure
	SOCKET newSock;
	
	newSock.socketNumber = sockNum;
	
	switch(sockNum){
		case 0:
		newSock.sockBase = S0_BASE;
		break;
		
		case 1:
		newSock.sockBase = S1_BASE;
		break;
		
		case 2:
		newSock.sockBase = S2_BASE;
		break;
		
		case 3:
		newSock.sockBase = S3_BASE;
		break;
	}
	
	newSock.dIP = IP;
	newSock.dPort = port;
	newSock.protocol = protocol;
	return newSock;
}

uint8_t W51_closeSocket(SOCKET sock){ //function to close socket
	W51_write(sock.sockBase + S_CR, 0x10); //send close socket command
	_delay_ms(10);
	return W51_OK;
}

uint8_t W51_openSocket(SOCKET sock){ //function to open socket
	W51_write(sock.sockBase + S_CR, 0x01); //send open socket command
	_delay_ms(50); //delay for ARP address resolution (adjust by network speed)
	return W51_OK;
}

uint8_t W51_connectSocket(SOCKET sock){ //function to connect to server (only TCP mode)
	if(sock.protocol == W51_PROTOCOL_TCP){ //check if socket is TCP
		W51_write(sock.sockBase + S_CR, 0x04); //send connect command
		_delay_ms(500); //delay for server connection (adjust by network speed)
		return W51_OK;
	}
	else{
		return W51_ERR_WRONG_PROTOCOL;
	}
}

uint8_t W51_listenSocket(SOCKET sock){ //function to listen for incoming connections (only TCP mode)
	if(sock.protocol == W51_PROTOCOL_TCP){ //check if socket is TCP
		W51_write(sock.sockBase + S_CR, 0x02); //send listen command
		_delay_ms(10);
		return W51_OK;
	}
	else{
		return W51_ERR_WRONG_PROTOCOL;
	}
}

uint8_t W51_disconnectSocket(SOCKET sock){ //function to disconnect from server (only TCP mode)
	if(sock.protocol == W51_PROTOCOL_TCP){ //check if socket is TCP
		W51_write(sock.sockBase + S_CR, 0x08); //send disconnect command
		_delay_ms(10);
		return W51_OK;
	}
	else{
		return W51_ERR_WRONG_PROTOCOL;
	}
}

uint8_t W51_initSocket(SOCKET sock){ //function to initialize the socket
	if(W51_read(sock.sockBase + S_SR) != 0x00){ //check if socket is closed
		return W51_ERR_SOCK_OPEN;
	}
	
	//set protocol
	W51_write(sock.sockBase + S_MR, sock.protocol);
	_delay_ms(1);
	//set source port
	W51_write(sock.sockBase + S_SPORT, (sock.dPort & 0xFF00) >> 8);
	W51_write(sock.sockBase + S_SPORT + 1, sock.dPort & 0x00FF);
	_delay_ms(1);
	//set destination IP
	W51_write(sock.sockBase + S_DIPR, sock.dIP[0]);
	W51_write(sock.sockBase + S_DIPR + 1, sock.dIP[1]);
	W51_write(sock.sockBase + S_DIPR + 2, sock.dIP[2]);
	W51_write(sock.sockBase + S_DIPR + 3, sock.dIP[3]);
	_delay_ms(1);
	//set destination port
	W51_write(sock.sockBase + S_DPORT, (sock.dPort & 0xFF00) >> 8);
	W51_write(sock.sockBase + S_DPORT + 1, sock.dPort & 0x00FF);
	_delay_ms(1);
	return W51_OK;
	
}

uint8_t W51_sendData(SOCKET sock, uint8_t* data, unsigned int datalen){
	uint16_t pointer, phyaddr;
	
	pointer = W51_read(sock.sockBase + S_TX_WR); //get MSB of starting write pointer
	pointer = (pointer & 0x00FF) << 8; //shift the MSB to left because it is on right side (8 to 16bit conversion)
	pointer += W51_read(sock.sockBase + S_TX_WR + 1); //get the LSB and append it to pointer variable
	
	while(datalen){
		datalen--; //lower the data length by 1 byte
		
		phyaddr = (TXBUF_BASE + (0x0800 * sock.socketNumber)) + (pointer & TXBUF_MASK); //calculate physical address to write
		
		W51_write(phyaddr, *data); //write the data to TX buffer
		
		pointer++; //increment pointer by 1 (to next byte)
		data++; //increment data array by 1 (to next byte)
	}
	
	W51_write(sock.sockBase + S_TX_WR, (pointer & 0xFF00) >> 8); //write new pointer MSB
	W51_write(sock.sockBase + S_TX_WR + 1, pointer & 0x00FF); //write new pointer LSB
	_delay_ms(1);
	W51_write(sock.sockBase + S_CR, 0x20); //triger SEND operation
	_delay_ms(10);
	return W51_OK;
}

uint16_t W51_getReceivedDataLen(SOCKET sock){
	return ((W51_read(sock.sockBase + S_RX_RSR) & 0x00FF) << 8) + W51_read(sock.sockBase + S_RX_RSR + 1); //get received data length
}

uint8_t W51_receiveData(SOCKET sock, unsigned int length, uint8_t* data){ //function to receive data of specified size from W5100
	uint16_t pointer, phyaddr, bufsize;
	
	bufsize = W51_read(sock.sockBase + S_RX_RSR);
	bufsize = (bufsize & 0x00FF) << 8;
	bufsize += W51_read(sock.sockBase + S_RX_RSR + 1);	
	
	if(bufsize < length){
		length = bufsize;
	}
	
	pointer = W51_read(sock.sockBase + S_RX_RD); //get MSB of starting read pointer
	pointer = (pointer & 0x00FF) << 8; //shift the MSB to left because it is on right side (8 to 16bit conversion)
	pointer += W51_read(sock.sockBase + S_RX_RD + 1); //get the LSB and append it to pointer variable
	
	while(length){ //while there is something to read
		length--; //lower length by one byte
		
		phyaddr = (RXBUF_BASE + (0x0800 * sock.socketNumber)) + (pointer & RXBUF_MASK); //calculate physical address to read
		
		*data = W51_read(phyaddr); //read the data and save it to selected element in output array
		
		data++; //higher the data ptr by one byte
		pointer++; //higher the read pointer by one byte
	}
	W51_write(sock.sockBase + S_RX_RD, (pointer & 0xFF00) >> 8); //write new pointer MSB
	W51_write(sock.sockBase + S_RX_RD + 1, pointer & 0x00FF); //write new pointer LSB
	_delay_ms(1);
	W51_write(sock.sockBase + S_CR, 0x40); //triger RECEIVE operation
	_delay_ms(10);
	return W51_OK;
	
}

void W51_setINET(uint8_t* ipaddr, uint8_t* macaddr, uint8_t* gateaddr, uint8_t* subaddr){ //function to init the W5100 addresses
	//the base address register addresses are defined as GAR, SAR, SUBR and SIPR. The additional byte registers must be derived by adding a value
	//set gateway address
	for(int i = 0; i < 4; i++){
		W51_write(GAR + i, gateaddr[i]);
	}

	//set MAC address (SAR)
	for(int i = 0; i < 6; i++){
		W51_write(SAR + i, macaddr[i]);
	}
	
	//set subnet mask (SUBR)
	for(int i = 0; i < 4; i++){
		W51_write(SUBR + i, subaddr[i]);
	}
	
	//set IP address (SIPR)
	for(int i = 0; i < 4; i++){
		W51_write(SIPR + i, ipaddr[i]);
	}
	_delay_ms(2000); //delay for network connection (adjust by network speed)
}
