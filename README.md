## unabto-pic32-harmony
PIC32 demo using the harmony framework


The version numbers below is the versions the software has been built
and tested with, older or newer versions may work.

# Requirements
  1. Microchip XC32 1.44
  2. Microchip Harmony 2.04
  3. Microchip MPLAB X IDE 4.05
  4. Microchip Ethernet Starterkit II

In the MPLAB X IDE please make sure that MPLAB Harmony Configurator is installed in tools->plugins in the Installed tab

# Prerequisites
You must be able to make a simple hallo world Harmony project in the MPLAB X IDE similar to:
http://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20Harmony%20Tutorial%20-%20Creating%20an%20Application_v111.pdf

## How to program the device

Clone this github repository recursively such that submodules get checked out.

  * git clone --recursive https://github.com/nabto/unabto-pic32-sdk
  * open one of the projects in demo/firmware/ or streamdemo/firmware/ folders with MPLAB X
  * Generate the Harmony configurator code, this is rather tricky if
    you are new to harmony/mplab X. Set the project as the startup
    project. Goto the harmony configurator `Tools -> Embedded -> MPLAB
    Harmony configurator` when the ide asks for the harmony folder
    give the folder where you installed harmony, if unsure run the
    harmony installer again. Now open the DM320004_2.mhc configuration
    file and click on the code generation button. The harmony step fails
    easily because of the complexity and if any component in the
    system has been updated.
  * Connect the PIC32 Ethernet Starter Kit II to your PC using the miniUSB port (J7), and connect the Ethernet port to your network.
  * compile with XC32
  * flash target
  
## How to use the demo

  1. Flash the device as explained in the above section. 
  2. Open the Nabto Client app from the android or ios app store.
  3. Discover the device and connect to it.

## How to use the streamDemo

  1. Flash the device as explained in the above section.
  2. Download the simpleclient_app from www.nabto.com/downloads.html
  3. Run ```./simpleclient_app --devices``` to discover the device, note down the name of your device
  4. Run simpleclient_app with the discovered device:
       ```./simpleclient_app -q the.device.name.from.step.3.net --echotest 100 --echocommand true```
  5. You should see a terminal output similar to this:
       No username specified, defaulting to guest.
       read: 102 testSize_ 100
       test result: 0
     The LED on the PIC32 should also toggle on/off every time step 4 is performed.

## Quirks

If you are starting a new project based on this increase 
the TCPIP_DNS_CLIENT_MAX_HOSTNAME_LEN to something more than 32 bytes 
if your hostnames are longer than that.

For the streamDemo: The NABTO_STREAM_RECEIVE_SEGMENT_SIZE in unabto_config.h must be set smaller than TCPIP_UDP_SOCKET_DEFAULT_TX_SIZE in the Harmony framework for nabto packets to fit into the tx buffer. Setting TCPIP_UDP_SOCKET_DEFAULT_TX_SIZE to 1500 removes the need to set the NABTO_STREAM_RECEIVE_SEGMENT_SIZE

## Test level

The code is smoke tested.


