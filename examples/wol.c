#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include "W5100.h"
#include "W51_regFunc.h"

uint8_t magicPacket[102] = YOUR_MAGIC_PACKET_ARRAY

int main(){
	W51_init();
	
	uint8_t gateway[] = {192, 168, 88, 1};
	uint8_t mac[] = {0xF2, 0xE1, 0x55, 0xF6, 0xEE, 0xC8};
	uint8_t ip[] = {192, 168, 88, 45};
	uint8_t mask[] = {255, 255, 255, 0};
	W51_setINET(ip, mac, gateway, mask); //set network addresses to connect to lan
	
	uint8_t targetIP[] = {192, 168, 88, 255}; //broadcast address
	uint16_t port = 9;
	
	SOCKET WoLsock = createSocket(0, targetIP, port, W51_PROTOCOL_UDP);
	
	W51_initSocket(WoLsock);
	W51_openSocket(WoLsock);
  _delay_ms(1000);
  W51_sendData(WoLsock, magicPacket, 102);
	
	while(1){
    
  }
}
