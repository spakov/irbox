/*!
  @header strings.h
  @abstract String constants.
*/

#ifndef STRINGS_H
#define STRINGS_H

/*!
  @abstract Left paren, for building command strings.
*/
extern const char* lParenString;

/*!
  @abstract Right paren, for building command strings.
*/
extern const char* rParenString;

/*!
  @abstract Comma, for building command strings.
*/
extern const char* commaString;

/*!
  @abstract Hex indicator (0x), for building command strings.
*/
extern const char* hexString;

/*!
  @abstract Newline, for building command strings.
*/
extern const char* newlineString;

/*!
  @abstract An OK indicator, for building command strings.
*/
extern const char* okString;

/*!
  @abstract An error indicator, for building command strings.
*/
extern const char* errorString;

/*!
  @abstract The word protocol, for building command strings.
*/
extern const char* protocolString;

/*!
  @abstract The word raw, for building command strings.
*/
extern const char* rawString;

/*!
  @abstract An unknown indicator, for building command strings.
*/
extern const char* unknownString;

/*!
  @abstract A busy indicator, for building command strings.
*/
extern const char* busyString;

/*!
  @abstract A wrong mode indicator, for building command strings.
*/
extern const char* wrongModeString;

/*!
  @abstract AT command to put the Wi-Fi module in client mode.
*/
extern const char* atClientModeString;

/*!
  @abstract AT command to ask the Wi-Fi module to join the desired access point's network.
*/
extern const char* atJoinString;

/*!
  @abstract AT command to ask the Wi-Fi module for the client IP address.
*/
extern const char* atIPString;

/*!
  @abstract AT command to put the Wi-Fi module in multiplex mode.
*/
extern const char* atMuxModeString;

/*!
  @abstract AT command to start the Wi-Fi module's server.
*/
extern const char* atServerString;

/*!
  @abstract AT command to send data.
*/
extern const char* atSendString;

/*!
  @abstract Response that means a connection was opened.
*/
extern const char* connectString;

/*!
  @abstract Response that means a connection failed.
*/
extern const char* connectFailString;

/*!
  @abstract Response that means a connection was closed.
*/
extern const char* closedString;

/*!
  @abstract Response that means data was received.
*/
extern const char* ipdString;

/*!
  @abstract The <code>nop</code> command.
*/
extern const char* nopString;

/*!
  @abstract The <code>rx</code> command.
*/
extern const char* rxString;

/*!
  @abstract The <code>norx</code> command.
*/
extern const char* norxString;

/*!
  @abstract The <code>tx</code> command.
*/
extern const char* txString;

#endif // !STRINGS_H
