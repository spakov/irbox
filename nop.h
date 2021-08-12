/*!
  @header nop.h
  @abstract <code>nop</code> command routines.
*/

#ifndef NOP_H
#define NOP_H

#include "strings.h"
#include "socket.h"

/*!
  @abstract Handles the <code>nop</code> command.
  @discussion Does nothing. Useful for clients to confirm that communication is working.
  @param connectionID The current connection ID.
*/
void nopHandler(const char* connectionID);

#endif // !NOP_H
