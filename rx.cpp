#include "rx.h"

bool rxHandler(
  const char* connectionID,
  state_t* state,
  char* rxLock
) {
  /* Make sure nobody has the rx lock already */
  if (*state == rxState && rxLock[0]) {
    /* Debug print command */
    #if USE_BITBANG_TX
    bitbangTx("\rdeny(have:");
    bitbangTx(rxLock);
    bitbangTx("|want:");
    bitbangTx(connectionID);
    bitbangTx(")[");
    bitbangTx(txString);
    bitbangTx("]");
    #endif // USE_BITBANG_TX

    /* They do, so provide a negative response */
    tx(
      connectionID,
      3,
      errorString,
      busyString,
      newlineString
    );

    return true;
  }

  /* Make sure we're in the idle state */
  if (*state != idleState) {
    /* Debug print command */
    #if USE_BITBANG_TX
    bitbangTx("\rwrongmode[");
    bitbangTx(txString);
    bitbangTx("]");
    #endif // USE_BITBANG_TX

    /* Reply over the network */
    tx(
      connectionID,
      3,
      errorString,
      wrongModeString,
      newlineString
    );

    return true;
  }

  /* Debug print command */
  #if USE_BITBANG_TX
  bitbangTx("\rcommand[");
  bitbangTx(rxString);
  bitbangTx("]");
  #endif // USE_BITBANG_TX

  /* Go to rx state and set the lock */
  *state = rxState;
  strcpy(rxLock, connectionID);

  /* Begin receiving */
  irReceiveBegin(IR_RX_PIN);

  /* Debug print command */
  #if USE_BITBANG_TX
  bitbangTx("\rlock(");
  bitbangTx(rxLock);
  bitbangTx(")[");
  bitbangTx(txString);
  bitbangTx("]");
  #endif // USE_BITBANG_TX

  /* Reply over the network */
  tx(
    connectionID,
    3,
    okString,
    rxString,
    newlineString
  );

  return false;
}
