# Examples for AVR-W5100

## Wake On LAN (WoL)
This example sends a magic packet to target computer to turn it on.

WoL must be enabled in the target computers BIOS

You must edit addresses by yourself.

Guide to create a magic packet into ```uint8_t magicPacket[102]``` array:

In the first 6 bytes, put FF FF FF FF FF FF in hex, and then repeat the targets MAC 16 times, the final size of packet must be 102 bytes

## Simple HTTP Server
This example creates a simple HTTP server and sends response containing "W5100 Embedded Web Server".

Adjust the addresses and response by yourself.

Note that is very simple HTTP server without proper handling of requests, for more complex applications consider using HTML parser library.
