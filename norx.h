/*!
  @header norx.h
  @abstract <code>norx</code> command routines.
*/

#ifndef NORX_H
#define NORX_H

#include "irbox.h"
#include "socket.h"
#include "state.h"

/*!
  @abstract Handles the <code>norx</code> command.
  @discussion Exits receive mode.
  @param connectionID The current connection ID.
  @param state The current operational state.
  @param rxLock The <code>rx</code> command lock string. If the function unlocks the <code>rx</code> command, it will set the lock string to an empty string.
  @result A value indicating whether or not receive mode was <em>not</em> exited.
*/
bool norxHandler(
  const char* connectionID,
  state_t* state,
  char* rxLock
);

#endif // !NORX_H
