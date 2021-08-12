# IR Box
An Arduino-based Wi-Fi server that wraps
[Arduino-IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) to
allow users to transmit infrared commands to devices.

## Overview
IR box implements a server that allows clients to transmit infrared commands.
The server utilizes a simple communication protocol with only a few commands.
Also allows for receiving infrared commands and printing equivalent transmit
commands supported by IR box.

Currently supports transmission using the NEC, Apple, and Sony protocols, but
could easily be extended to support additional protocols supported by
Arduino-IRremote.

I've also written a [web app](https://https://github.com/spakov/irbox_app) that
allows you to use the IR box from any browser. It's much more convenient than
using netcat.

## Prerequisites
### Hardware
- An Arduino or similar microcontroller (I use the Arduino Pro Mini 3V3)
- An ESP8266 or similar Wi-Fi module, connected via serial interface to Arduino
- Infrared LED, connected to Arduino
- Infrared receiver module, connected to Arduino

### Software
- A Linux or UNIX-like system (I'm using MacOS) with common core packages
- bash, zsh or similar (I'm using zsh)
- netcat, or your preferred general network communication tool
- The [Arduino IDE](https://www.arduino.cc/en/software), or your preferred
  serial communication tool
- [arduino-build](https://github.com/spakov/arduino-build)
  - GNU make
  - [arduino-cli](https://github.com/arduino/arduino-cli) (for compiling and
    uploading)
  - screen (for monitoring)
- [HeaderDoc](https://developer.apple.com/library/archive/technotes/tn2339/_index.html),
  or a documentation generator of your choice (for documentation generation).

## Building
Configure arduino-build for your environment and use make.

## Considerations
- It's very likely possible to run this entire thing on an ESP8266 (or
  similar), but I did not take that approach. Instead, I'm using the Arduino as
  the controller and connecting the ESP8266 via its serial pins
- IR transmission is *weak*. Arduino-IRremote suggests connecting multiple IR
  LEDs in series to boost the signal. Instead, I'm using an external IR
  repeater.

## Getting it Working
![Prototype layout.](https://github.com/spakov/irbox/doc/prototype.jpg)

The first thing you'll want to do is load the program to your Arduino. Next,
you'll want to configure the baud rate of the Wi-Fi module.

### Wi-Fi Module Baud Rate Configuration
You'll want to make sure the module is configured to communicate at the same
rate as configured in `BAUD_RATE`. Note that, if you can/want to use 115200
bps, you're in luck, because this is the default (at least on the unit I
received).

> (Quick note about baud rate: 9600 bps is plenty fast enough for this
  application.)

This can be a finicky process, especially if your Wi-Fi module's default baud
rate is 115200 bps and you have an 8 MHz Arduino, since 115200 serial
communication does not work reliably as a result of the clock rate.

I did this by using the [Arduino IDE 1.x](https://www.arduino.cc/en/software)'s
Serial Monitor and a 3V3 USB-to-serial adapter to communicate with the ESP8266.
I set line ending to "Both NL & CR" and select "115200 baud" as the default baud
rate. I issued an `AT` command to ensure the connection is established, then
used an `AT_UART_DEF=9600,8,1,0,0` to permanently change the baud rate to 9600
bps. Then I changed the baud rate to "9600 baud" in the Serial Monitor and
issued another `AT` to ensure the baud rate was changed.

Of course, your mileage may vary.

The next step is to get the Wi-Fi module connected to the Arduino.

### Connecting the Wi-Fi Module to the Arduino
The ESP8266 has two relevant limitations:
1. It runs only on 3V3, and
2. It has eight pins configured in a non-breadboard-friendly way.

Since I'm using the Arduino Pro Mini 3V3, for prototyping, I'm driving it
directly from the Arduino's VCC pin. Don't do that in production; instead, use
a dedicated, regulated 3V3 power supply. Power consumption of the ESP8266 can
be quite high in some cases (perhaps up to 1 A).

I'm using short male-female jumpers for prototyping. 3V3 and GND are pretty
self-explanatory. EN is supposed to be high to enable the device, but mine
seems to work reliably without it connected. That leaves RST, RX, and TX. I'm
not using RST at all, since it's not needed for this application. RX and TX go
to the TX(O) and RX(I) pins of the Arduino, respectively. (Note the reversal.)

This may raise an interesting problem for you, though. The Arduino Pro Mini,
like many Arduinos/AVR devices, has only a single serial interface.

How can we program the Arduino and allow commination with the ESP8266 at the
same time? Well, the answer is simple: we can't.

The "correct" solution to this is to use an IO multiplexer, but there are two
additional options to consider:

**For prototyping**: I've been leaving the ESP8266 connected and pulling the 3V3
pin during programming. I don't recommend this, but it works. Serial output
from the Arduino *and* the ESP8266 both go to the console. If you send data to
the Arduino in this configuration while the ESP8266's 3V3 pin is connected, you
get garbage. But if you send data to the Arduino in this configuration while
the ESP8266's 3V3 pin is disconnected, everything works as expected.

**For operation**: You won't actually need a serial console when the device is
operating.

Again, your mileage may vary.

The next step is to get the Wi-Fi module connected to the network.

### Connecting the Wi-Fi Module to the Network
Conveniently, the ESP8266 only needs to be joined to a network a single time.
After that, it will rejoin automatically when it's powered back up.

To do this, there's a convenient `networkInit()` function that can be used. It
does the following:

- Sends `AT+CWMODE=1`, which puts the module in "station mode," which means
  it's a device, not an access point
- Waits two seconds
- Sends `AT+CWJAP="ssid","password"`, which tells the device to join the
  specified access point (two things worth noting here: the double quotes are
  part of the command, and you should obviously replace `ssid` with your
  network SSID and `password` with your network password)
- Waits 15 seconds
- Sends `AT+CIFSR`, which tells the device to print its IP address
- Waits two seconds
- Flushes the serial buffer.

I recommend temporarily sticking something like the following at the end of
`setup()` to get going:

    networkInit();
    while (true);

There is lots of information about the AT commands used by the ESP8266 online,
not all of it reliable. As usual, your mileage may vary.

Once you're connected, you'll want to connect the infrared devices to the
Arduino.

### Connecting the IR Devices to the Arduino
This is pretty straightforward. For receiving, I'm using a TSOP382 connected to
pin 11. For transmitting, I'm using an 850 nm IR LED connected to pin 3 with a
68 Ω resistor.

After this, you're ready to give it a try.

### Giving it a Try
netcat (nc) is a great tool for this. Use a command like the following:

    nc -4 192.168.0.160 333

Obviously, replace the IP address with the IP address of the IR box, and be
sure to change the port if you modified that as well.

If all goes well, you should be presented with a cheery greeting:

    +

Use `C-d` (Ctrl-D) to terminate the connection.

## Theory of Operation
The IR box supports a very simple protocol. You talk to it over a TCP port.
Commands end with `\r\n`, and so do responses. You send a command and the
device sends a response. "Good" responses begin with `+` and "bad" ones begin
with `-`.

Worth noting is that the ESP8266 supports up to four concurrent connections
when it's running in server mode. IR box does as well and manages all the state
stuff automatically.

The ESP8266 appears to terminate connections after around a minute of
inactivity. This is perfectly acceptable for this application.

### Commands

#### `nop`
Does nothing. This is, however, useful for sanity checking
your configuration.

IR box will always respond with `+nop`.

#### `tx`
Transmits an infrared command. `tx` is a function-like command that takes a
variable number of arguments, depending on the protocol.

For all protocols, the first three arguments are as follows:

1. Protocol
2. Address
3. Command

The Sony protocol has a number of bits associated with it. This is the fourth
argument for the Sony protocol and is required.

The final argument is optional and specifies the number of repeats.

> **What are repeats?** They're what they sound like. If you hold a button on
  a remote control, it transmits the initial command, then transmits repeats as
  long as you hold the button. Depending on the protocol, repeats may be the
  same as the initial command or different. Arduino-IRremote handles all that
  automatically.

Arguments are traditionally in parentheses after `tx`, are separated by commas,
and must be specified in hexadecimal, with a leading `0x`. For example, the
Sony TV power command:

    tx(0x13,0x1,0x14,0xc)

However, the parser doesn't particularly care about anything other than `tx`
and the hex values. Equally valid is the following:

    txDOG 0x13   0x1;0x14 not-a-comma 0xc

(But don't do that.)

Assuming all goes well, IR box transmits the IR command and responds with
`+tx(…)`, echoing the parsed command back to you, unless the device is
currently already in receive mode (keep reading), in which case it responds
with `- busy` (another client is in receive mode) or `- wrong mode` (this
client is already in receive mode).

If you specify an unknown/unimplemented protocol, IR box responds with `-
unknown`.

> **What are all these numbers?** Protocols, addresses, commands, bits, and
  repeats are all concepts used by the Arduino-IRremote library. Nothing
  special is going on; these are passed directly to Arduino-IRremote.

#### `rx`
Puts IR box in receive mode.

Receive mode allows IR box to read infrared commands and converts them to
values that you can use with `tx` in the future.

Receive mode implements a lock, so only one client can use receive mode at a
time. When a client is in receive mode, IR box does a few special things:

- Prevents other clients from entering and exiting receive mode
- Rejects `tx` commands, both from the client with the receive mode lock, and
  from other clients
- Rejects all commands except `norx` (keep reading) and `nop` from the client
  with the receive mode lock
- Reads infrared commands and sends them to the client in receive mode.

IR box, when in receive mode, will read infrared commands and print them out in
`+tx(…)` form as they're received.

There are some things that can prevent this from happening, as outlined above.
In that case, IR box responds with `- busy` (this client doesn't have the
receive lock) or `- wrong mode` (the client issued an `rx` while already in
receive mode).

#### `norx`
Exits receive mode.

If IR box exits receive mode, the device responds with `+norx`.

IR box can also respond with `- wrong mode` (the device isn't in receive mode,
*or* the device is in receive mode, but another client put it there, not this
client).

#### Any Other String
IR box will respond with `- unknown`.

### Terminating the Connection
Just close the connection. IR box handles all the housekeeping automatically.

## User-configurable Macros
There are several macros that can be configured by the user to customize IR
box. Refer to the documentation or source code for additional details.

### General Configuration (`irbox.h`)

#### `INIT_DELAY`
Short delay at startup to allow connected devices to initialize.

#### `BAUD_RATE`
Baud rate to use for all serial communications (but not bitbanging).

#### `SERIAL_TIMEOUT`
Timeout to pass to Arduino `Serial.setTimeout()`.

#### `IR_RX_PIN`
Pin number IR receiver module is connected to.

#### `IR_TX_PIN`
Pin number IR LED is connected to.

#### `SERIAL_BUFFER_SIZE`
Size of the global serial buffer.

### Bitbanging Configuration (`bitbang.h`)

#### `USE_BITBANG_TX`
Whether or not to use bitbanging output for debugging. This requires another
microcontroller to be running the bitbang-echo program (see
[Bitbanging](#bitbanging)).

#### `VERBOSE_BITBANG_TX`
Whether or not to use verbose bitbanging output. This is very helpful for
troubleshooting or implementing additional commands.

#### `BITBANG_TX_PIN`
The pin to use for bitbanging output.

#### `BITBANG_DELAY`
The delay between transmitted bitbang bits. Must match the value in
bitbang-echo.

## User-configurable Strings
Strings used by the IR box can also be customized in `strings.cpp`. Note that
this includes the port that the server runs on (333 by default). A subset of
these that you may want to modify are detailed below. For the rest, refer to
the documentation or the source code.

### `atJoinString`
Configure your network's SSID and password in this string.

### `atServerString`
Change the port that IR box runs on here.

## Bitbanging
Since many Arduinos/AVR devices (including mine) have only a single serial
interface, I implemented a simple bitbanging interface for debugging. To use
this, you'll need another Arduino or similar, running the bitbang-echo program.
That's located in the bitbang-echo directory.

The setup is pretty straightforward. You connect a pin from IR box to an analog
pin on the secondary device.

> **Why an analog pin?** I wanted to support differing voltages.

"Bytes" get written with a `1010` header, followed by ASCII bits, MSB first,
and bits have a short delay (`BITBANG_DELAY`) between them. bitbang-echo
listens, interprets bytes, and prints them on the serial line.

Don't forget to make sure your devices share a command ground.
