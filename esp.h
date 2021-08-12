/*!
  @header esp.h
  @abstract ESP8266 routines.
*/

#ifndef ESP_H
#define ESP_H

#include "irbox.h"
#include "ConnectionStack.h"
#include "socket.h"
#include "state.h"

/*!
  @abstract Handles <code>n,CONNECT</code> from the Wi-Fi module.
  @param connections A reference to the <code>ConnectionStack</code> object used to track connections.
  @param connectionID The current connection ID.
*/
void connectHandler(
  ConnectionStack& connections,
  const char* connectionID
);

/*!
  @abstract Handles <code>n,CLOSED</code> from the Wi-Fi module.
  @param connections A reference to the <code>ConnectionStack</code> object used to track connections.
  @param connectionID The current connection ID.
  @param state The current operational state.
  @param rxLock The <code>rx</code> command lock string. If the function locks the <code>rx</code> command, it will copy the current connection ID to the lock string.
*/
void closedHandler(
  ConnectionStack& connections,
  char* connectionID,
  state_t* state,
  char* rxLock
);

/*!
  @abstract Initializes the Wi-Fi module to connect to the network. This is only required to be called once (between powerdowns, even).
*/
void networkInit();

#endif // !ESP_H
