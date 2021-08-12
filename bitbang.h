/*!
  @header bitbang.h
  @abstract Bitbanging routines.
*/

#ifndef BITBANG_H
#define BITBANG_H

#include "irbox.h"

/*!
  @abstract Use bitbanging transmission for debug output? 
  @discussion Valid values: 0, 1.

  This is <em>slow</em>, but works well for debugging.

  <strong>Important</strong>: You may note that the Arduino's 64-byte UART buffer is easily overwhelmed when bitbanging is enabled due to the delays incurred. This can cause odd things to happen!
*/
#define USE_BITBANG_TX 0

/*!
  @abstract Use bitbanging transmission for verbose debug output?
  @discussion Valid values: 0, 1.

  This is <em>very slow</em>, but works well for debugging. Be sure to see the note at <code>USE_BITBANG_TX</code>.
*/
#define VERBOSE_BITBANG_TX 1

/*!
  @abstract Pin for bitbanging serial debug output.
  @discussion Valid values: an <a href="https://www.arduino.cc/en/Tutorial/Foundations/DigitalPins">Arduino digital pin</a>.
*/
#define BITBANG_TX_PIN 13

/*!
  @abstract The delay, in milliseconds, between bits for bitbanging output.
  @discussion Valid values: 0 &le; n &le; <code>ULONG_MAX</code>.

  I get reasonable results with 2.
*/
#define BITBANG_DELAY 2

#if USE_BITBANG_TX

/*!
  @abstract Bitbang transmits one byte across <code>BITBANG_TX_PIN</code>.
  @param c The byte to transmit.
*/
void bitbangTx(char c);

/*!
  @abstract Bitbang transmits the first <code>len</code> bytes of a string across <code>BITBANG_TX_PIN</code>.
  @param s The string to transmit.
  @param len The number of bytes to transmit.
*/
void bitbangTx(const char* s, size_t len);

/*!
  @abstract Bitbang transmits a string across <code>BITBANG_TX_PIN</code>.
  @param s The string to transmit.
*/
void bitbangTx(const char* s);

#endif // USE_BITBANG_TX

#endif // !BITBANG_H
