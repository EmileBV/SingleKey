Plan is to use this firmware:
	https://github.com/wagiminator/VUSB-AVR
	or this?? it seems way better and might be able to go for an even smaller mcu!!
	https://github.com/micronucleus/micronucleus
	
And build a single key keyboard that can connect to other single key keyboards via I²C.

Each ATtiny85 will be programmed to send one specific character. Each ATtiny85 sets its own slave address as its character. At boot, the ATtiny85 that has a USB connection will scan through the I²C bus and find all connected ATtiny85s. It records a list of all connected addresses and then only requests from these ones afterwards. Each slave will only send back "true" or "false" depending on its key position. After that the master will send each one that is active with the associated character.

Need to figure out how to detect that the master is connected to USB.
Maybe it can try opening a serial port on the pc and if it fails it assumes it's a slave??

Super tiny MCU that might work: MSPM0C1104SDDFR