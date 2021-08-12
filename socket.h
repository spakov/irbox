/*!
  @header socket.h
  @abstract "Socket" routines.
  @discussion No socket is actually used, but serial communication is used in a socket-like manner.
*/

#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>

#include "irbox.h"

/*!
  @abstract Parses the current connection ID from a buffer.
  @discussion Updates <code>connectionID</code> with an ASCII string representing the connection ID, if the buffer contains one. Checks the first seven bytes for the connection ID. Writes a null byte if unable to parse.

  There are four instances in which this can be called: when a <code>CONNECT</code> command is processed, when a <code>CONNECT FAIL</code> command is processed, when a <code>CLOSED</code> command is processed, and when a <code>+IPD</code> command is processed.
  @param buffer The buffer to search.
  @param connectionID The connection ID to write.
*/
void parseConnectionID(char* buffer, char* connectionID);

/*!
  @abstract Reads a single serial message, up to but not including <code>\n</code>, into <code>serialBuffer</code>.
  @discussion Guarantees null termination of <code>serialBuffer</code>.
  @result The number of bytes read. 0 if no data was read.
*/
size_t rx();

/*!
  @abstract Flushes the serial data stream.
*/
void rxFlush();

/*!
  @abstract Transmits a message over the network.
  @param connectionID The connection to respond on.
  @param argc Number of variadic arguments.
  @param ... Strings to transmit sequentially as a single message.
  @discussion You are responsible for supplying any desired newline characters.
*/
void tx(const char* connectionID, uint8_t argc, ...);

/*!
  @abstract Prepares to send data over the network by using the appropriate AT command.
  @discussion Not meant to be called. This is called automatically by <code>tx()</code>.
  @param connectionID The connection to respond on.
  @param len The length of the data that will follow.
*/
void txStart(const char* connectionID, size_t len);

#endif // !SOCKET_H
