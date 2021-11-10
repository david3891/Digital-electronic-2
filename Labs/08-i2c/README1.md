# Lab 8: I2C/TWI serial communication

![I2C scan](Images/arduino_uno_i2c.jpg)

### Learning objectives

After completing this lab you will be able to:

* Understand the I2C communication
* Use functions from I2C library
* Use FSM-type of application

The purpose of the laboratory exercise is to understand serial synchronous communication using the I2C (Inter-Integrated Circuit) bus, as well as the structure of the address and data frame and the possibilities of communication using the internal TWI (Two Wire Interface) unit. Another goal is to understand the structure of FSM (Finite-State Machine) in C.

### Table of contents

* [Preparation tasks](#preparation)
* [Part 1: Synchronize repositories and create a new folder](#part1)
* [Part 2: I2C bus](#part2)
* [Part 3: I2C scanner](#part3)
* [Part 4: Final application](#part4)
* [Experiments on your own](#experiments)
* [Lab assignment](#assignment)
* [References](#references)

<a name="preparation"></a>

## Preparation tasks (done before the lab at home)

1. Use schematic of the [Arduino Uno](../../Docs/arduino_shield.pdf) board and find out to which pins the SDA and SCL signals are connected.

   | **Signal** | **MCU pin** | **Arduino pin(s)** |
   | :-: | :-: | :-: |
   | SDA (data)  |  |  |
   | SCL (clock) |  |  |

2. What is the general structure of I2C address and data frames?

   | **Frame type** | **8** | **7** | **6** | **5** | **4** | **3** | **2** | **1** | **0** | **Description**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-- |
   | Address | | | | | | | | | | |
   | Data    | | | | | | | | | | |

<a name="part1"></a>

## Part 1: Synchronize repositories and create a new folder

Run Git Bash (Windows) of Terminal (Linux), navigate to your working directory, and update local repository. Create a new working folder `Labs/08-i2c` for this exercise.

<a name="part2"></a>

## Part 2: I2C bus

I2C is a serial protocol for two-wire interface to connect low-speed devices like microcontrollers, EEPROMs, A/D and D/A converters, I/O interfaces and other similar peripherals in embedded systems. It was invented by Philips and now it is used by almost all major IC manufacturers. Each slave device has a unique address [[2]](https://i2c.info/).

I2C uses only two wires: SCL (serial clock) and SDA (serial data). Both need to be pulled up with a resistor to +Vdd. There are also I2C level shifters which can be used to connect to two I2C buses with different voltages.

![I2C bus](Images/i2c-bus.png)

The initial I2C specifications defined maximum clock frequency of 100 kHz. This was later increased to 400 kHz as Fast mode. There is also a High speed mode which can go up to 3.4 MHz and there is also a 5 MHz ultra-fast mode.

In normal state both lines (SCL and SDA) are high. The communication is initiated by the master device. It generates the Start condition (S) followed by the address of the slave device (SLA). If the bit 0 of the address byte was set to 0 the master device will write to the slave device (SLA+W). Otherwise, the next byte will be read from the slave device (SLA+R). Each byte is supplemented by an ACK (low level) or NACK (high level) acknowledgment bit, which is always transmitted by the device receiving the previous byte.

The address byte is followed by one or more data bytes, where each contains 8 bits and is again terminated by ACK/NACK. Once all bytes are read or written the master device generates Stop condition (P). This means that the master device switches the SDA line from low voltage level to high voltage level before the SCL line switches from high to low [[3]](https://www.electronicshub.org/basics-i2c-communication/).

![I2C protocol](Images/i2c_protocol.jpg)

Note that, most I2C devices support repeated start condition. This means that before the communication ends with a stop condition, master device can repeat start condition with address byte and change the mode from writing to reading.

### Example of I2C communication

> **Question:** Let the following image shows several frames of I2C communication between ATmega328P and slave device. What circuit is it and what information was sent over the bus?
>
   &nbsp;
   ![Temperature reception from DHT12 sensor](Images/twi-dht12_temperature_decoded.png)

> **Answer:** This communication example contains a total of five frames. After the start condition, which is initiated by the master, the address frame is always sent. It contains a 7-bit address of the slave device, supplemented by information on whether the data will be written to the slave or read from it to the master. The ninth bit of the address frame is an acknowledgment provided by the receiving side.
>
> Here, the address is 184 (decimal), i.e. `1011100_0` in binary including R/W=0. The slave address is therefore 1011100 (0x5c) and master will write data to the slave. The slave has acknowledged the address reception, so that the communication can continue.
>
> According to the list of [I2C addresses](https://learn.adafruit.com/i2c-addresses/the-list) the device could be humidity/temp or pressure sensor. The signals were really recorded when communicating with the humidity and temperature sensor.
>
> The data frame always follows the address one and contains eight data bits from the MSB to the LSB and is again terminated by an acknowledgment from the receiving side. Here, number `2` was writen to the sensor. According to the [sensor manual](../../Docs/dht12_manual.pdf), this is the address of register, to which the integer part of measured temperature is stored. (The following register contains its fractional part.)
>
   | **Register address** | **Description** |
   | :-: | :-- |
   | 0x00 | Humidity integer part |
   | 0x01 | Humidity fractional part |
   | 0x02 | Temperature integer part |
   | 0x03 | Temperature fractional part |
   | 0x04 | Checksum |

> After the repeated start, the same circuit address is sent on the I2C bus, but this time with the read bit R/W=1 (185, `1011100_1`). Subsequently, data frames are sent from the slave to the master until the last of them is confirmed by the NACK value. Then the master generates a stop condition on the bus and the communication is terminated.
>
> The communication in the picture therefore records the temperature transfer from the sensor, when the measured temperature is 25.3 degrees celsius.
>
   | **Frame #** | **Description** |
   | :-: | :-- |
   | 1 | Address frame with SLA+W = 184 (0x5c<<1 + 0) |
   | 2 | Data frame sent to the slave represents the ID of internal register |
   | 3 | Address frame with SLA+R = 185 (0x5c<<1 + 1) |
   | 4 | Data frame with integer part of temperature read from slave |
   | 5 | Data frame with fractional part of temperature read from slave|

<a name="part3"></a>

## Part 3: I2C scanner

### Version: SimulIDE

1. In the SimulIDE application, use the following components: I2C Ram (**Components > Logic > Memory > I2C Ram**), I2C to Parallel (**Components > Logic > Converters > I2C to Parallel**) and create a connection according to the following figure. Also, change **Control Code** property of all I2C devices. These codes represent the I2C addresses of the slave circuits. Pins A2, A1, A0 allow you to specify part of the device address. Thus, up to 8 (2^3 = 8) identical devices can be connected and it will be possible to distinguish them. External pull-up resistors on SDA and SCL signals must be used for correct simulation.

   ![I2C scanner circuit](Images/screenshot_simulide_i2c_scan.png)

### Version: Real hardware

1. Use breadboard to connect humidity/temperature [DHT12](../../Docs/dht12_manual.pdf) digital sensor and combined module with real time clock (RTC) device [DS3231](../../Docs/ds3231_manual.pdf) and [AT24C32](../../Docs/at24c32_manual.pdf) memory to Arduino Uno board. Instead of external pull-up resistors on the SDA and SCL pins, it is possible to use the internal ones, directly in the microcontroller.

   | **DHT12 pin** | **Arduino Uno pin** |  |
   | :-: | :-: | :-: |
   | +<br>SDA<br>-<br>SCL | 5V (or 3.3V)<br>SDA<br>GND<br>SCL | ![Humidity/temperature sensor DHT12](Images/dht12.jpg) |

   | **RTC+EEPROM pin** | **Arduino Uno pin** |  |
   | :-: | :-: | :-: |
   | 32K<br>SQW<br>SCL<br>SDA<br>VCC<br>GND | Not connected<br>Not connected<br>SCL<br>SDA<br>5V (or 3.3V)<br>GND | ![RTC/EEPROM module](Images/rtc_eeprom.jpg) |

### Version: Atmel Studio 7

1. Create a new GCC C Executable Project for ATmega328P within `08-i2C` working folder and copy/paste [template code](main.c) to your `main.c` source file.

2. In **Solution Explorer** click on the project name, then in menu **Project**, select **Add Existing Item... Shift+Alt+A** and add:
   * I2C/TWI files `twi.h`, [`twi.c`](../../Examples/library/twi.c) from `Examples/library/include` and `Examples/library` folders,
   * UART library files `uart.h`, `uart.c` from the previous lab,
   * Timer library `timer.h` from the previous labs.

### Version: Command-line toolchain

1. Copy `main.c` and `Makefile` files from previous lab to `Labs/08-i2c` folder.

2. Copy/paste [template code](main.c) to your `08-i2c/main.c` source file.

3. Add the source files of I2C/TWI and UART libraries between the compiled files in `08-i2c/Makefile`.

```Makefile
# Add or comment libraries you are using in the project
#SRCS += $(LIBRARY_DIR)/lcd.c
SRCS += $(LIBRARY_DIR)/uart.c
SRCS += $(LIBRARY_DIR)/twi.c
#SRCS += $(LIBRARY_DIR)/gpio.c
#SRCS += $(LIBRARY_DIR)/segment.c
```

### All versions

1. Use the [`twi.h`](../../Examples/library/include/twi.h) header file from the I2C/TWI library to complete the description of the functions in the following table.

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `twi_init` | None | Initialize TWI, enable internal pull-up resistors, and set SCL frequency | `twi_init();` |
   | `twi_start` |  | <br>&nbsp; | `twi_start((addr<<1)+TWI_READ);` |
   | `twi_write` |  | <br>&nbsp; |  |
   | `twi_read_ack` | <br>&nbsp; |  |  |
   | `twi_read_nack` | <br>&nbsp; |  |  |
   | `twi_stop` |  |  | `twi_stop();` |

2. Explore the use of Finite State Machine (FSM) in the `main.c` source file. Note that state names are declared using `typedef enum` as follows

```c
typedef enum {  // FSM declaration
    STATE_IDLE = 1,
    STATE_SEND,
    STATE_ACK
} state_t;
```

  and the body and transitions between states are defined using `switch`/`case` C structure.

```c
    static state_t state = STATE_IDLE;  // Current state of the FSM

    // FSM
    switch (state)
    {
    // Increment I2C slave address
    case STATE_IDLE:
        ...
        break;
    
    // Transmit I2C slave address and get result
    case STATE_SEND:
        ...
        break;

    // A module connected to the bus was found
    case STATE_ACK:
        ...
        break;

    // If something unexpected happens then move to IDLE
    default:
        state = STATE_IDLE;
        break;
    }
```

3. Complete the Timer/Counter1 overflow routine according to the following state diagram and scan slave addresses. Transmit useful information via UART to PuTTY SSH Client or Serial monitor.

   ![FSM for I2C scanner](Images/fsm_scan_i2c.png)

> The figure above was created in [diagrams.net](https://app.diagrams.net/). It is an open source, online, desktop and container deployable diagramming software.
>

4. Form the UART output of your application to a hexadecimal table such as the following figure. Note that, the designation RA represents I2C addresses that are [reserved](https://www.pololu.com/file/download/UM10204.pdf?file_id=0J435) and cannot be used for slave circuits.

```Makefile
   Scan I2C-bus for slave devices:

         .0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .a .b .c .d .e .f
   0x0.: RA RA RA RA RA RA RA RA -- -- -- -- -- -- -- --
   0x1.: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
   0x2.: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
   0x3.: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
   0x4.: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
   0x5.: -- -- -- -- -- -- -- 57 -- -- -- -- -- -- -- --
   0x6.: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
   0x7.: -- -- -- -- -- -- -- -- RA RA RA RA RA RA RA RA
   
   Number of detected devices: 1
```

<a name="part4"></a>

## Part 4: Final application

### Version: SimulIDE

1. In the SimulIDE application, create the circuit with eight active-low LEDs connected to I2C to Parallel expander. You can use individual components (ie. 8 resistors and 8 LEDs) or single **Passive > ResistorDip** and **Outputs > LedBar** according to the following figure. Several signals can form a bus **Logic > Other Logic > Bus**, as well.

   ![I2C LED bar](Images/screenshot_simulide_i2c_leds.png)

2. Create an application that sequentially turns on one of the eight LEDs. ie first LED0, then LED1 and finally LED7, then start again from the beginning. Use Timer/Counter1 and change the value every 262 ms. Send the status of the LEDs to the UART. Try to complement the LED controls according to the Knight Rider style, ie light the LEDs in one direction and then in the opposite one.

### Version: Real hardware

1. Program an FSM application which reads data from humidity/temperature DHT12 sensor and sends them periodically via UART to PuTTY SSH Client. Use Timer/Counter1 with a suitable overflow time. Note that, according to the [DHT12 manual](../../Docs/dht12_manual.pdf), the internal DHT12 data registers have the following structure.

   | **Register address** | **Description** |
   | :-: | :-- |
   | 0x00 | Humidity integer part |
   | 0x01 | Humidity fractional part |
   | 0x02 | Temperature integer part |
   | 0x03 | Temperature fractional part |
   | 0x04 | Checksum |

   ![FSM for I2C temperature](Images/fsm_dht_i2c.png)

2. Find out how checksum byte value is calculated.

## Synchronize repositories

Use [git commands](https://github.com/tomas-fryza/Digital-electronics-2/wiki/Useful-Git-commands) to add, commit, and push all local changes to your remote repository. Check the repository at GitHub web page for changes.

<a name="experiments"></a>

## Experiments on your own

### Version: SimulIDE

1. Program an application that communicates with memory modules using the I2C bus. Store random data in the first ten address positions of the first and second memory modules. Then copy 5 values from the first memory to the third and another 5 values from the second memory to the third one. Send the first ten values from each memory module to the UART.

### Version: Real hardware

1. Extend the humidity/temperature application, use the RTC/EEPROM module and read second and minute values from RTC DS3231 device, and send them via UART to PuTTY SSH Client. According to the [DS3231 manual](../../Docs/ds3231_manual.pdf), the internal RTC registers have the following structure.

   | **Address** | **Bit 7** | **Bits 6:4** | **Bits 3:0** |
   | :-: | :-: | :-: | :-: |
   | 0x00 | 0 | 10 Seconds | Seconds |
   | 0x01 | 0 | 10 Minutes | Minutes |
   | ... | ... | ... | ... |

2. Verify the I2C communication with logic analyzer. Find out in what format the hour information is stored in the circuit. Implement an application that reads the hours and displays it in both 12-hour and 24-hour formats.

<a name="assignment"></a>

## Lab assignment

*Prepare all parts of the assignment in Czech, Slovak or English, insert them in this [template](Assignment.md), export formatted output (not Markdown) [from HTML to PDF](https://github.com/tomas-fryza/Digital-electronics-2/wiki/Export-README-to-PDF), and submit a single PDF file via [BUT e-learning](https://moodle.vutbr.cz/). The deadline for submitting the task is the day before the next laboratory exercise.*

> *Vypracujte všechny části úkolu v českém, slovenském, nebo anglickém jazyce, vložte je do této [šablony](Assignment.md), exportujte formátovaný výstup (nikoli výpis v jazyce Markdown) [z HTML do PDF](https://github.com/tomas-fryza/Digital-electronics-2/wiki/Export-README-to-PDF) a odevzdejte jeden PDF soubor prostřednictvím [e-learningu VUT](https://moodle.vutbr.cz/). Termín odevzdání úkolu je den před dalším počítačovým cvičením.*
>

<a name="references"></a>

## References

1. Tomas Fryza. [Schematic of LCD Keypad shield](../../Docs/arduino_shield.pdf)

2. Ezoic. [I2C Info - I2C Bus, Interface and Protocol](https://i2c.info/)

3. Electronicshub.org. [Basics of I2C Communication | Hardware, Data Transfer, Configuration](https://www.electronicshub.org/basics-i2c-communication/)

4. Adafruit. [List of I2C addresses](https://learn.adafruit.com/i2c-addresses/the-list)

5. Aosong. [Digital temperature DHT12](../../Docs/dht12_manual.pdf)

6. JGraph Ltd. [diagrams.net](https://app.diagrams.net/)

7. NXP. [I2C-bus specification and user manual](https://www.pololu.com/file/download/UM10204.pdf?file_id=0J435)

8. Tomas Fryza. [Useful Git commands](https://github.com/tomas-fryza/Digital-electronics-2/wiki/Useful-Git-commands)

9. Maxim Integrated. [DS3231, Extremely accurate I2C-Integrated RTC/TCXO/Crystal](../../Docs/ds3231_manual.pdf)
