# AVR-W5100
A simple easy-to-use library for Wiznet W5100 Ethernet controller for AVR Microcontrollers


# Function Descriptions
## W5100 Initialization Functions
```void W51_init(void);```

This initializes the W5100. Please adjust the pin settings in ```W51_regFunc.h``` file by your microcontroller model. 

If your microcontroller has SPI speed lower than 14MHz, you can remove this line in ```W51_regFunc.c```: ```SPSR |= (1 << SPI2X);```
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
```uint8_t W51_disconnectSocket(SOCKET sock);```

A function to disconnect from the server **(Only used in TCP Mode)**
```SOCKET sock```: Opened, initialized socket structure with all necessary parameters
<br><br><br><br>
``````
