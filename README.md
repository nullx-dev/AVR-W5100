# AVR-W5100
A simple easy-to-use library for Wiznet W5100 Ethernet controller for AVR Microcontrollers

Tested on ATMega328P, can be easily ported to all SPI-enabled AVR microcontrollers by editing the ```W51_regFunc.c``` file.

Supported protocols are TCP and UDP, maximum 4 open sockets.

**This library is only for W5100, not W5500**

All examples are located in the example directory.

# Function Descriptions
## W5100 Initialization Functions
```void W51_init(void);```

This initializes the W5100. Please adjust the pin settings in ```W51_regFunc.h``` file by your microcontroller model. 

```F_CPU``` must be defined to use this library.
<br><br><br><br>
```void W51_setINET(uint8_t* ipaddr, uint8_t* macaddr, uint8_t* gateaddr, uint8_t* subaddr);```

Function to assign static IPv4 address to W5100

```uint8_t* ipaddr```: Array that contains IPv4 address of W5100

```uint8_t* macaddr```: Array that contains W5100 MAC address

```uint8_t* gateaddr```: Array that contains gateway (router) address

```uint8_t* subaddr```: Array that contains subnet mask
<br><br><br><br>
## Basic socket operations
``` SOCKET createSocket(uint8_t sockNum, uint8_t* IP, uint16_t port, uint8_t protocol); ```

This function creates and returns socket structure.

```uint8_t sockNum```: Physical socket number (0 - 3)

```uint8_t* IP```: Array that contains the IPv4 address of socket. In case of listening socket, it should be set to 0.0.0.0

```uint16_t port```: Socket port number (0 - 65535)

```uint8_t protocol```: Socket protocol, use ```W51_PROTOCOL_TCP``` or ```W51_PROTOCOL_UDP``` defines

NOTE: It doesnt assign the socket to W5100, it only initializes the structure
<br><br><br><br>
```uint8_t W51_initSocket(SOCKET sock);```

This function writes the data from socket structure to W5100.

```SOCKET sock```: Socket structure with all necessary parameters
<br><br><br><br>
```uint8_t W51_openSocket(SOCKET sock);```

A function to open socket.

```SOCKET sock```: Initialized socket structure with all necessary parameters
<br><br><br><br>
```uint8_t W51_closeSocket(SOCKET sock);```

A function to close a socket.

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters

NOTE: This function does not delete the socket structure, it only closes the physical socket on W5100
<br><br><br><br>
```uint8_t W51_connectSocket(SOCKET sock);```

A function to connect to the server **(Only used in TCP Mode)**

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters
<br><br><br><br>
```uint8_t W51_listenSocket(SOCKET sock);```

Function to listen for incoming connections

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters
<br><br><br><br>
```uint8_t W51_disconnectSocket(SOCKET sock);```

A function to disconnect from the server **(Only used in TCP Mode)**

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters
<br><br><br><br>
```uint8_t W51_sendData(SOCKET sock, uint8_t* data, unsigned int datalen);```

Function to send data through socket

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters

```uint8_t* data```: Data buffer to be sent

```unsigned int datalen```: Length of the data to be sent

NOTE: The maximum one transaction transmitting length is 2048 bytes
<br><br><br><br>
```uint16_t W51_getReceivedDataLen(SOCKET sock);```

Function to get received data length in bytes

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters
<br><br><br><br>
```uint8_t W51_receiveData(SOCKET sock, unsigned int length, uint8_t* data);```

Function to receive data from a socket

```SOCKET sock```: Opened, initialized socket structure with all necessary parameters

```unsigned int length```: Length of data to be received

```uint8_t* data```: Buffer for data to be received
