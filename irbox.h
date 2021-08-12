/*!
  @header irbox.h
  @abstract IR box main routines.
*/

#ifndef IRBOX_H
#define IRBOX_H

#include <Arduino.h>

#include "ir.h"
#include "server.h"
#include "bitbang.h"

/*!
  @define INIT_DELAY
  @abstract A short delay at startup, in milliseconds, to wait for connected devices to become initialized.
  @discussion Valid values: 0 &le; n &le; <code>ULONG_MAX</code>.
*/
#define INIT_DELAY 2000

/*!
  @define BAUD_RATE
  @abstract Use this baud rate for all serial communication (but not bitbanging).
  @discussion Valid values: <a href="https://www.arduino.cc/reference/en/language/functions/communication/serial/">any baud rate</a> the board works with.
*/
#define BAUD_RATE 9600

/*!
  @define SERIAL_TIMEOUT
  @abstract The timeout, in milliseconds, to pass to <code>Serial.setTimeout()</code>.
  @discussion Valid values: 0 &le; n &le; <code>LONG_MAX</code>.
*/
#define SERIAL_TIMEOUT 10

/*!
  @define IR_RX_PIN
  @abstract The pin to use to receive infrared data.
  @discussion Valid values: an <a href="https://www.arduino.cc/en/Tutorial/Foundations/DigitalPins">Arduino digital pin</a>.
*/
#define IR_RX_PIN 11

/*!
  @define IR_TX_PIN
  @abstract The pin to use to transmit infrared data. Recommend pin 3 for best results.
  @discussion Valid values: an <a href="https://www.arduino.cc/en/Tutorial/Foundations/DigitalPins">Arduino digital pin</a>.
*/
#define IR_TX_PIN 3

/*!
  @define SERIAL_BUFFER_SIZE
  @abstract The size, in bytes, of the buffer to use to read serial data.
  @discussion Valid values: 1 &le; n &le; <code>INT_MAX</code>.

  Arduino UART buffer size is 64, so recommend two bytes smaller than that for reasons I don't fully understand. Note that this results in a maximum command length. This yields a maximum effective command length of 51 bytes, which is plenty for even the most complex <code>tx</code>.
*/
#define SERIAL_BUFFER_SIZE 62

/* Forward declarations */

/*!
  @abstract Buffer for received serial data.
*/
extern char serialBuffer[SERIAL_BUFFER_SIZE];

#endif // !IRBOX_H
