/*!
  @header ir.h
  @abstract Infrared routines.
*/

#ifndef IR_H
#define IR_H

/* PWM pins are available, so don't have IRremote use software PWM */
#define SEND_PWM_BY_TIMER

/* No legacy compatibility required */
#define NO_LEGACY_COMPATIBILITY

/* No fancy protocols needed */
#define EXCLUDE_EXOTIC_PROTOCOLS
#define EXCLUDE_UNIVERSAL_PROTOCOLS

/* No LEDs needed */
#define DISABLE_LED_FEEDBACK_FOR_RECEIVE

#include "irbox.h"
#include "strings.h"
#include "socket.h"

/* Enums */

/*!
  @abstract Protocol.
  @constant PROTOCOL_NEC NEC protocol.
  @constant PROTOCOL_APPLE Apple protocol.
  @constant PROTOCOL_SONY Sony protocol.
  @discussion These are pulled from Arduino-IRremote, <code>src/IRProtocol.h</code> commit 39eec4c5caa4e7f2ba94003d2fe0e83d9c2b29a5.
*/
typedef enum protocol {
  PROTOCOL_UNKNOWN = 0,
  PROTOCOL_PULSE_DISTANCE,
  PROTOCOL_PULSE_WIDTH,
  PROTOCOL_DENON,
  PROTOCOL_DISH,
  PROTOCOL_JVC,
  PROTOCOL_LG,
  PROTOCOL_LG2,
  PROTOCOL_NEC,
  PROTOCOL_PANASONIC,
  PROTOCOL_KASEIKYO,
  PROTOCOL_KASEIKYO_JVC,
  PROTOCOL_KASEIKYO_DENON,
  PROTOCOL_KASEIKYO_SHARP,
  PROTOCOL_KASEIKYO_MITSUBISHI,
  PROTOCOL_RC5,
  PROTOCOL_RC6,
  PROTOCOL_SAMSUNG,
  PROTOCOL_SHARP,
  PROTOCOL_SONY,
  PROTOCOL_ONKYO,
  PROTOCOL_APPLE,
  PROTOCOL_BOSEWAVE,
  PROTOCOL_LEGO_PF,
  PROTOCOL_MAGIQUEST,
  PROTOCOL_WHYNTER
} protocol_t;

/*!
  @abstract Initializes IR receiving.
  @param receivePin The Arduino pin to receive on.
*/
void irReceiveBegin(uint8_t receivePin);

/*!
  @abstract Initializes IR sending.
  @param sendPin The Arduino pin to send on.
*/
void irSendBegin(uint8_t sendPin);

/*!
  @abstract Decodes received infrared data and writes an equivalent <code>tx</code> command over the network.
  @param connectionID The connection ID to write to.
*/
void irDecode(const char* connectionID);

/*!
  @abstract Sends an NEC IR command.
  @param address Address.
  @param command Command.
  @param repeats Number of repeats.
*/
void irSendNEC(uint16_t address, uint8_t command, uint8_t repeats);

/*!
  @abstract Sends an Apple IR command.
  @param address Address.
  @param command Command.
  @param repeats Number of repeats.
*/
void irSendApple(uint16_t address, uint8_t command, uint8_t repeats);

/*!
  @abstract Sends a Sony IR command.
  @param address Address.
  @param command Command.
  @param repeats Number of repeats.
  @param bits Number of repeats.
*/
void irSendSony(uint16_t address, uint8_t command, uint8_t repeats, uint8_t bits);

#endif // !IR_H
