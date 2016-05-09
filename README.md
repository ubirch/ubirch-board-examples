# ubirch #1 (r0.2) firmware playground

This repository contains the firmware implementation for our own board. The toolchain
is built in a way so it may also run on other boards, like the [FRDM-K82F](http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/ides-for-kinetis-mcus/freescale-freedom-development-platform-for-kinetis-k82-k81-and-k80-mcus:FRDM-K82F)
or the [FRDM-KL82Z](http://www.nxp.com/products/software-and-tools/hardware-development-tools/freedom-development-boards/freedom-development-board-for-kinetis-ultra-low-power-kl82-mcus:FRDM-KL82Z?fsrch=1&sr=1&pageNum=1)*.
You will have to select the specific board when running ```cmake``` (see below).

See the board specific, including pin settings found here:

* [ubirch#1 r0.2](src/board/ubirch1r02)
* [FRDM-K82F](src/board/frdm_k82f)
* [FRDM-KL82Z](src/board/frdm_kl82z)\*

> \* Work in progress, the Kinetis SDK 2.0 is not available for the K82Z.

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

Install the arm toolchain gcc and cmake:
```
brew install gcc-arm-none-eabi-49 cmake cgdb
```

Also download the [SEGGER J-Link Debugger](https://www.segger.com/jlink-software.html) for the JLinkGDBServer.

Connect the J-Link via USB, also connect the Cortex-M debug ribbon cable to the board.
Power the board using USB or a battery. Connect a USB-UART adapter RX/TX to the corresponding UART pins on board (see next section).

Compile the code by running the following commands:

```
cd $SRCDIR
mkdir build
cd build
cmake ..
make
```

> If you would like to use the FRDM-K82F eval board use ```cmake -DBOARD=FRDM-K82F ..``` instead.

### 1. Start the GDB Server:

```
JLinkGDBServer  -if SWD -device MK82FN256xxx15 -port 2331
```

### 2. Start the serial debug console

```
screen /dev/cu.SLAB_USBtoUART 115200
```

> Use another device (i.e. ```/dev/cu.usbmodem1243```) if you have a different adapter.
> Refer to the board if you need specific information on how to connect.

### 3. Start the debugger (gdb)

I recommend [cgdb](https://cgdb.github.io/) as it is somewhat easier to see where you currently are in your code.

For flashing and debugging cd into the corresponding sub-directory:
```
cd src/examples/test
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

* ```src/examples/gsm_commands``` - simple example running commands

    Runs some simple commands using an expect like library.

* ```src/examples/gsm_console``` - console for the SIM800 via DEBUG UART

    Simply proxies the UART to the internal GSM modem. You can issue AT commands this way
    and play with the SIM800H.

* ```src/examples/i2c_scan``` - scan I2C bus for devices

  Small scanner that scans all adresses for I2C slave devices and reports their addres.

* ```src/examples/lpuart``` - UART (Serial) console code

  Compiles the example code from the KSDK 2.0. Can be used to test the UART.

* ```src/examples/oled``` - echo debug console input to the oled display

  Initialized an SSD1306 based OLED display and lets you enter characters via the
  debug console which are then displayed on the OLED.

* ```src/examples/rgbsensor``` - read RGB sensor data

  Initialized the ISL29125 RGB sensor via I2C and prints RGB information on the
  debug console.

* ```src/examples/rtc``` - Real Time Clock

  Compiles the Realtime clock example from KSDK 2.0. Shows how the RTC works.

* ```src/examples/test``` - blink and echo debug console

  Just a simple test that lets the on-board LED blink and echos characters back that you
  type into the debug console.

* ```src/examples/wolfssl-test-*``` - wolfSSL test code

  This code will perform an RSA/ECC encryption and signing on-board and outputs the
  result hex-encoded to the debug console. The result can be verified using openssl.
  See the README.md in the directory for instructions.


