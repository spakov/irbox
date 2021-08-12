/*!
  @header tx.h
  @abstract <code>tx</code> command routines.
*/

#ifndef TX_H
#define TX_H

#include "irbox.h"
#include "state.h"

/*!
  @abstract Handles the <code>tx</code> command.
  @discussion Transmits an infrared command.
  @param connectionID The current connection ID.
  @param state The current operational state.
  @param rxLock The <code>rx</code> command lock string. This is used to determine if the client asking to transmit is in receive mode.
  @param message The command text (message).
  @result A value indicating whether or the command was <em>not</em> sent.
*/
bool txHandler(
  const char* connectionID,
  state_t* state,
  char* rxLock,
  const char* message
);

#endif // !TX_H
