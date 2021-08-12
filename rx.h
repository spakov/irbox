/*!
  @header rx.h
  @abstract <code>rx</code> command routines.
*/

#ifndef RX_H
#define RX_H

#include "irbox.h"
#include "socket.h"
#include "state.h"

/*!
  @abstract Handles the <code>rx</code> command.
  @discussion Enters receive mode.
  @param connectionID The current connection ID.
  @param state The current operational state.
  @param rxLock The <code>rx</code> command lock string. If the function locks the <code>rx</code> command, it will copy the current connection ID to the lock string.
  @result A value indicating whether or not receive mode was <em>not</em> entered.
*/
bool rxHandler(
  const char* connectionID,
  state_t* state,
  char* rxLock
);

#endif // !RX_H
