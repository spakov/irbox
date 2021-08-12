/*!
  @header server.h
  @abstract Server routines.
*/

#ifndef SERVER_H
#define SERVER_H

#include "esp.h"
#include "nop.h"
#include "tx.h"
#include "rx.h"
#include "norx.h"

/*!
  @abstract Loops forever, handling server commands. Assumes <code>serverSetup()</code>has already been called.
*/
void serverLoop();

/*!
  @abstract Starts the listener on the Wi-Fi module.
*/
void serverSetup();

#endif // !SERVER_H
