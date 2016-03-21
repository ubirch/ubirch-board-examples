# ubirch #1 (r0.2.0) board internal playground

## Code checkout:

The following code will ensure that the [firmware](https://github.com/ubirch/firmware) repository as well as the [wolfssl](https://github.com/ubirch/wolfssl)
repository are checked out as submodules in ```libs```.
```
git clone --recursive git@github.com:ubirch/ubirch2-board.git
```


The K82F chip documentation can be found here:
[K82 Sub-Family Reference Manual](http://cache.nxp.com/files/32bit/doc/ref_manual/K82P121M150SF5RM.pdf)

All other documentation and examples can be obtained from the [Kinetis SDK 2.0](http://kex.freescale.com/en/).
You will need to select the chip and minimal features before downloading.

Link the KSDK 2.0 into the libs directory:

```
cd libs
ln -sf $KSDKDIR SDK_2.0_MK82FN256xxx15
```

## Running, flashing and debugging

Install the arm toolchain gcc:
```
brew install gcc-arm-none-eabi-49
```

Also download the [SEGGER J-Link Debugger](https://www.segger.com/jlink-software.html) for the JLinkGDBServer.

Connect the J-Link via USB, also connect the Cortex-M debug ribbon cable to the board.
Power the board using USB or a battery. Connect a USB-UART adapter RX/TX to the corresponding UART pins on board (see next section).

### 1. Start the GDB Server:

```
JLinkGDBServer  -if SWD -device MK82FN256xxx15 -port 2331
```

### 2. Start the serial debug console

```
screen /dev/cu.SLAB-to-UART 115200
```

> Use another device (i.e. ```/dev/cu.usbmodem1243```) if you have a different adapter.

### 3. Start the debugger (gdb)

I recommend [cgdb](https://cgdb.github.io/) (install via ```brew install cgdb```) as it is somewhat easier to see where you currently are in your code.

For flashing and debugging cd into the corresponding sub-directory:
```
cd src/test
cgdb -d arm-none-eabi-gdb test.elf
```

Issue the following commands in cgdb, which will flash the code on the board:

```
load test.elf
```

Then just enter ```c``` to start the program

### 4. DEBUG!

To stop a running program in the debugger, just press ```Ctrl-C``` and set a breakpoint:

```
break main
r
```

```r``` restarts the code (you will have to confirm) and break at the start of ```main()```
After subsequent ```load test.elf``` commands you do not need to restart as the debugger will
restart the MCU itself. Just issue ```c``` to continue. Unless you leave the debugger the breakpoints
are still there.



## Examples

Debug console output can be found on pins 6 (RX) and 7 (TX). If the SIM800H chip is on the bottom right
corner of the board, the pins are as follows:

```
2  4  6  8  10 12 14 16 18 20 22 24 26
o  o  o  o  o  o  o  o  o  o  o  o  o
⚀  o  o  o  o  o  o  o  o  o  o  o  o
1  3  5  7  9  11 13 15 17 19 21 23 25
```

The pin configuration is as follows:

```
  1 = A1          PTB2
  2 = A2          PTB3
  3 = D1          PTB9
  4 = I2C_SCL     PTB10
  5 = I2C_SDA     PTB11
  6 = UART_RX     PTB16
  7 = UART_TX     PTB17
  8 = T1          PTB18
  9 = GND
  10 = switchable VDD
  11 = T2         PTB19
  12 = SPI2_CS0   PTB20
  13 = SPI2_SCK   PTB21
  14 = SPI2_MOSI  PTB22
  15 = SPI2_MISO  PTB23
  16 = UART2_RTS  PTC1
  17 = SPI0_CS4   PTC0
  18 = UART2_CTS  PTC2
  19 = UART2_RX   PTC3
  20 = UART2_TX   PTC4
  21 = SPI0_SCK   PTC5
  22 = SPI0_MOSI  PTC6
  23 = SPI0_MISO  PTC7
  24 = permanent VDD
  25 = GND
  26 = switchable VDD
```

### ```src/test``` - blink and echo debug console

Just a simple test that lets the on-board LED blink and echos characters back that you
type into the debug console.

### ```src/lpuart``` - UART (Serial) console code

Compiles the example code from the KSDK 2.0. Can be used to test the UART.

### ```src/rtc``` - Real Time Clock

Compiles the Realtime clock example from KSDK 2.0. Shows how the RTC works.

### ```src/wolfssl-test```

This code will perform an RSA encryption and signing on-board and outputs the
result hex-encoded to the debug console. The result can be verified using openssl.
See the README.md in the directory for instructions.


