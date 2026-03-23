# DrJonEA: RPIPicoW-RelayControl

A Raspberry Pi Pico W based Relay control board. Using a ULN2003 as driver for a 5Volt Relay.

Example controlling the Relay via an onboard SPST switch. Plus remote control of MQTT/TLS.

This project is featured on my [Youtube Channel DrJonEA](https://youtube.com/@drjonea).

## Clone and Build
The example is built  using the Pico SDK version 2.2.0 and a number of libraries which are included as submodules.
Please clone the repository using the recurse submodules switch. 

I've written a [blog post](https://drjonea.co.uk/2025/12/15/building-my-projects-from-repo/) on how to clone, build and 
debug my projects. It covers command line and VSCode build. I hope this help you get started.

## Schematic, PCB Design and Components
The *PCB* folder contains the KiCAD project files for the PCB Design. 

Components:

 |  Designator | Footprint | Quantity | Designation | 
 | ------------- | ----------- | ----------- | ------------ | 
 | "Id" | "Designator" | "Footprint" | "Quantity" | "Designation" | "Supplier and ref" | 
 | 1 | "C1" | "CP_Radial_D10.0mm_P5.00mm" | 1 | "1000uF" | 
 | 2 | "I2C2,I2C1" | "PinHeader_1x04_P2.54mm_Vertical" | 2 | "Conn_01x04_Pin" | 
 | 3 | "J4,J3" | "PinHeader_1x04_P2.54mm_Vertical" | 2 | "Touch Display" | 
 | 4 | "O1" | "ScrewTerm_Oiyagai_01x02_5mm" | 1 | "Elect" | 
 | 5 | "A1" | "RaspberryPi_Pico_Common_Unspecified" | 1 | "RaspberryPi_Pico" | 
 | 6 | "C2" | "C_Disc_D4.7mm_W2.5mm_P5.00mm" | 1 | "10uF" | 
 | 7 | "K1" | "Relay_SPDT_SANYOU_SRD_Series_Form_C" | 1 | "SANYOU_SRD_Form_C" |  |  | 
 | 8 | "ST1" | "ScrewTerm_Oiyagai_01x03_5mm" | 1 | "Screw_Terminal_01x03" | 
 | 9 | "U1" | "CERDIP-16_W7.62mm_SideBrazed_LongPads_Socket" | 1 | "ULN2003" | 
 | 10 | "D1" | "D_DO-41_SOD81_P10.16mm_Horizontal" | 1 | "1N4007" | 
 | 11 | "SW2" | "SW_PUSH_6mm" | 1 | "SW_SPST" | 
 | 12 | "SW1" | "SW_Latch_7x7_DPDT" | 1 | "SW_DPDT_x2" | 
 | 13 | "R4" | "R_Axial_DIN0309_L9.0mm_D3.2mm_P15.24mm_Horizontal" | 1 | "220R" | 
 | 14 | "U2" | "WS2812B Strip" | 1 | 4 LED strip at 144per meter fits | 

## Examples Projects

### exp/TestLeds
A test project to check the WS2812B LEDs and Switch on the board

### exp/TestRelay
A test projec to check the Relay works. Uses the switch to toggle the relay state. Shows the state of the
relay on the WS2812B LED

### Main Project
This is a MQTT device connecting to a MQTT Broker (I use Flespi.io). It connects over TLS using WolfSSL 
to provide a TLS connection.  The device will then take commands on the topic 
"TNG/*{DEVICEID}*/TPC/Relay/req". With a Payload as JSON saying {"on": True} or {"on": False}.

To build this examples the following environment variables are used to source credentials:

+ WIFI_SSID
+ WIFI_PASSWORD
+ MQTT_CLIENT
+ MQTT_USER
+ MQTT_PASSWD
+ MQTT_HOST
+ MQTT_PORT

I have a course showing how I build these types of MQTT Devices over on [Udemy](https://www.udemy.com/course/iot-with-rpi-pico-w/?referralCode=331C13FDD6ABC00BFB81).


### Python Control Example
Python scripts which can interact with the device are included in the *py* folder. They require the client id of
the device the are controlling in this form:

python3 on.py *{DEVICE_ID}*

The scripts take credentials for the client from the followin environment variables:

+ MQTT_USER
+ MQTT_PASSWD
+ MQTT_HOST
+ MQTT_PORT

NOTE: To use MQTT the device and the client must have different Client IDs (or MQTT_USER).




