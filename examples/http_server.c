#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "W51_regFunc.h"
#include "W5100.h"

const PROGMEM unsigned char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 122\r\n"
"\r\n"
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
"    <title>Hello World</title>\r\n"
"</head>\r\n"
"<body>\r\n"
"    <h1>W5100 Embedded Web Server</h1>\r\n"
"</body>\r\n"
"</html>\r\n";

void readFromPROGMEM(const unsigned char* data, unsigned char* output){ //read string from progmem
	memset(output, 0x00, 600); //set the buffer to 0 to avoid collided data
	for(int i = 0; i < strlen_P((char*)data); i++){ //iterate over each byte
		*output = pgm_read_byte_near(data + i); //read the byte
		output++; //higher the pointer
	}
	*output = '\0'; //add null terminator
}

int main(void){
	W51_init(); //init W5100
	
	unsigned char buffer[600];
	
	uint8_t gateway[] = {192, 168, 88, 1};
	uint8_t mac[] = {0xF2, 0xE1, 0x55, 0xF6, 0xEE, 0xC8};
	uint8_t ip[] = {192, 168, 88, 45};
	uint8_t mask[] = {255, 255, 255, 0};
 	W51_setINET(ip, mac, gateway, mask); //set network addresses to connect to lan
	
	uint8_t remIP[] = {0, 0, 0, 0};
	uint16_t remPort = 80;
	
	SOCKET sock = createSocket(0, remIP, remPort, W51_PROTOCOL_TCP); //create new listening TCP 80 socket
	
	W51_initSocket(sock); //initialize the socket
	W51_openSocket(sock); //open the socket
	W51_listenSocket(sock); //listen on the socket
	
	while(1){
		
			if(W51_read(sock.sockBase + S_SR) == 0x17){ //if socket is established
					
					if(W51_getReceivedDataLen(sock) != 0x0000){ //if something is received
            readFromPROGMEM(response, buffer); //read GET response from flash
            _delay_ms(500);
						W51_sendData(sock, buffer, strlen((const char*)buffer)); //send the buffer with response
						memset(buffer, 0x00, 600); //clear the buffer
					}
			}
			else{
				W51_listenSocket(sock); //if socket is not established, keep listening
			}

	}
}

