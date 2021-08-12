/*!
  @header state.h
  @abstract State tracking enum/typedef.
*/

#ifndef STATE_H
#define STATE_H

#include "irbox.h"

/*!
  @abstract Operational state.
  @constant idleState Idle state.
  @constant rxState Receive mode.
*/
typedef enum state {
  idleState = 0,
  rxState = 1
} state_t;

#endif // !STATE_H
