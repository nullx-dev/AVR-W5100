#ifndef W5100_H_
#define W5100_H_
#endif /* W5100_H_ */

typedef struct{
	uint8_t socketNumber;
	uint16_t sockBase;
	uint8_t protocol;
	uint8_t* dIP;
	uint8_t* dMAC;
	uint16_t dPort;
	
	}SOCKET;
	
uint8_t W51_sendData(SOCKET sock, uint8_t* data, unsigned int datalen);
void W51_setINET(uint8_t* ipaddr, uint8_t* macaddr, uint8_t* gateaddr, uint8_t* subaddr);
uint8_t W51_openSocket(SOCKET sock);
uint8_t W51_closeSocket(SOCKET sock);
uint8_t W51_listenSocket(SOCKET sock);
uint8_t W51_connectSocket(SOCKET sock);
uint8_t W51_disconnectSocket(SOCKET sock);
uint8_t W51_initSocket(SOCKET sock);
uint16_t W51_getReceivedDataLen(SOCKET sock);
uint8_t W51_receiveData(SOCKET sock, unsigned int length, uint8_t* data);
SOCKET createSocket(uint8_t sockNum, uint8_t* IP, uint16_t port, uint8_t protocol);
