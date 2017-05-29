## unabto-pic32-harmony
PIC32 demo using the harmony framework


The version numbers below is the versions the software has been built
and tested with, older or newer versions may work.

# Requirements
  1. Microchip XC32 1.42
  2. Microchip Harmony 1.10
  3. Microchip MPLAB X IDE 3.51
  4. Microchip Ethernet Starterkit


## How to program the device

Clone this github repository recursively such that submodules get checked out.

  * git clone --recursive https://github.com/nabto/unabto-pic32-harmony
  * open project in demo/firmware/ folder in MPLAB X
  * compile 
  * flash target
  
## How to use the demo

  1. Flash the device as explained in the above section. 
  2. Open the nabto app from the android or ios app store.
  3. Discover the device and connect to it.


## Quirks

If you are starting a new project based on this increase 
the TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN to something more than 32 bytes 
if your hostnames are longer than that.

## Test level

The code is smoke tested.


