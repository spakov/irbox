#include "norx.h"

bool norxHandler(
  const char* connectionID,
  state_t* state,
  char* rxLock
) {
  /* Make sure we're in the rx state */
  if (*state != rxState) {
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

  /* Make sure the connection that requested rx mode is the one requesting
    * norx */
  if (strcmp(rxLock, connectionID)) {
    /* Debug print command */
    #if USE_BITBANG_TX
    bitbangTx("\rwrongmode(have:");
    bitbangTx(rxLock);
    bitbangTx("|want:");
    bitbangTx(connectionID);
    bitbangTx(")[");
    bitbangTx(txString);
    bitbangTx("]");
    #endif // USE_BITBANG_TX

    /* This connection isn't authorized to do norx */
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
  bitbangTx(norxString);
  bitbangTx("]");
  #endif // USE_BITBANG_TX

  /* Debug print command */
  #if USE_BITBANG_TX
  bitbangTx("\runlock(");
  bitbangTx(connectionID);
  bitbangTx(")[");
  bitbangTx(norxString);
  bitbangTx("]");
  #endif // USE_BITBANG_TX

  /* Go to idle state and clear the lock */
  *state = idleState;
  rxLock[0] = 0;

  /* Reply over the network */
  tx(
    connectionID,
    3,
    okString,
    norxString,
    newlineString
  );

  return false;
}
