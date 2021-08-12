#include "esp.h"

void connectHandler(
  ConnectionStack& connections,
  const char* connectionID
) {
  /* Add this connection to our "stack" */
  connections.add(connectionID);

  /* Debug print connection states */
  #if USE_BITBANG_TX
  #if VERBOSE_BITBANG_TX
  bitbangTx((char*) "\rconnections[");
  bitbangTx(connections.list());
  bitbangTx((char*) "]");
  #endif // VERBOSE_BITBANG_TX
  #endif // USE_BITBANG_TX

  /* Print a welcome message over the network */
  txStart(
    connectionID,
    strlen(okString)
      + strlen(newlineString)
  );
  Serial.print(okString);
  Serial.print(newlineString);
}

void closedHandler(
  ConnectionStack& connections,
  char* connectionID,
  state_t* state,
  char* rxLock
) {
  /* Remove this connection from our "stack" */
  connections.remove(connectionID);

  /* If this connection broke and we're still in the rx state, we need to
   * release the lock */
  if (*state == rxState && !strcmp(rxLock, connectionID)) {
    /* Debug print command */
    #if USE_BITBANG_TX
    bitbangTx("\runlock(");
    bitbangTx(connectionID);
    bitbangTx(")[");
    bitbangTx(norxString);
    bitbangTx("]");
    #endif // USE_BITBANG_TX

    /* Go to idle state and clear the lock */
    rxLock[0] = 0;
    *state = idleState;
  }

  /* Mark the current connection as none */
  connectionID[0] = 0;

  /* Debug print connection states */
  #if USE_BITBANG_TX
  #if VERBOSE_BITBANG_TX
  bitbangTx((char*) "\rconnections[");
  bitbangTx(connections.list());
  bitbangTx((char*) "]");
  #endif // VERBOSE_BITBANG_TX
  #endif // USE_BITBANG_TX
}

void networkInit() {
  /* Put the module in client mode, giving it 2 seconds */
  Serial.println(atClientModeString);
  delay(2000);
  rxFlush();

  /* Connect to the network, giving 15 seconds to connect */
  Serial.println(atJoinString);
  delay(15000);
  rxFlush();

  /* Print IP address, giving it 2 seconds */
  #if USE_BITBANG_TX
  Serial.println(atIPString);
  delay(2000);
  rxFlush();
  #endif // USE_BITBANG_TX
}
