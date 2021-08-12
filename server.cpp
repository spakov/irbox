#include "server.h"

void serverLoop() {
  ConnectionStack connections;
  char connectionID[2];

  char* message;

  state_t state;

  char rxLock[2];

  /* By default, assume no current connection, go to idle state, and have no rx
   * lock */
  connectionID[0] = 0;
  state = idleState;
  rxLock[0] = 0;

  while(true) {
    /* Check for rx state */
    if (state == rxState) {
      /* Receive IR data */
      irDecode(rxLock);
    }

    /* Read serial data into the buffer */
    rx();

    /* There are only four cases in which we care about noting the connection
     * ID: CONNECT[ FAIL], CLOSED, and IPD. */
    if (
      strstr(serialBuffer, connectString)
      || strstr(serialBuffer, closedString)
      || strstr(serialBuffer, ipdString)
    ) {
      parseConnectionID(serialBuffer, connectionID);

      /* Debug print received data */
      #if USE_BITBANG_TX
      #if VERBOSE_BITBANG_TX
      bitbangTx((char*) "\rconnection[");
      bitbangTx(connectionID);
      bitbangTx((char*) "]");
      #endif // VERBOSE_BITBANG_TX
      #endif // USE_BITBANG_TX
    }

    /* Check for lines like n,CONNECT FAIL, where n is the connection number.
     * This happens when a connection fails. */
    if (strstr(serialBuffer, connectFailString)) {
      closedHandler(connections, connectionID, &state, rxLock);
      continue;

    /* Check for lines like n,CONNECT, where n is the connection number. This
     * happens when a client connects. */
    } else if (strstr(serialBuffer, connectString)) {
      connectHandler(connections, connectionID);
      continue;

    /* Check for lines like n,CLOSED, where n is the connection number. This
     * happens when a client disconnects. */
    } else if (strstr(serialBuffer, closedString)) {
      closedHandler(connections, connectionID, &state, rxLock);
      continue;

    /* Check for lines like +IPD,i,j:string, where i is the connection ID
     * (which came from a line like i,CONNECT) and j is the length of string.
     *
     * In the process, we're skipping all lines that do not begin with +IPD...
     * The ESP8266 includes all kinds of newlines in its output, so we have to
     * work around those. */
    } else if ((message = strstr(serialBuffer, ipdString)) == NULL) continue;

    /* Debug print received data */
    #if USE_BITBANG_TX
    #if VERBOSE_BITBANG_TX
    bitbangTx((char*) "\rrx(");
    bitbangTx(connectionID);
    bitbangTx(")[");
    bitbangTx(serialBuffer);
    bitbangTx((char*) "]");
    #endif // VERBOSE_BITBANG_TX
    #endif // USE_BITBANG_TX

    /* Skip the command portion and comma, leaving message pointing to j:string
     * */
    message += strlen(ipdString);
    message = strchr(message, ',') + (size_t) 1;

    /* Skip the length portion and comma, leaving message pointing to string */
    message = strchr(message, ':') + (size_t) 1;

    /* Debug print received message */
    #if USE_BITBANG_TX
    #if VERBOSE_BITBANG_TX
    bitbangTx((char*) "\rmessage[");
    bitbangTx(message);
    bitbangTx((char*) "]");
    #endif // VERBOSE_BITBANG_TX
    #endif // USE_BITBANG_TX

    /* Check for nop command */
    if (strncmp(message, nopString, strlen(nopString)) == 0) {
      nopHandler(connectionID);

    /* Check for tx command */
    } else if (strncmp(message, txString, strlen(txString)) == 0) {
      if (txHandler(connectionID, &state, rxLock, message)) continue;

    /* Check for rx command */
    } else if (strncmp(message, rxString, strlen(rxString)) == 0) {
      if (rxHandler(connectionID, &state, rxLock)) continue;

    /* Check for norx command */
    } else if (strncmp(message, norxString, strlen(norxString)) == 0) {
      if (norxHandler(connectionID, &state, rxLock)) continue;

    /* Some unknown or invalid command */
    } else {
      /* Debug print command */
      #if USE_BITBANG_TX
      bitbangTx('\r');
      bitbangTx((char*) "Bad command, ignoring");
      #endif // USE_BITBANG_TX

      /* Reply over the network */
      tx(
        connectionID,
        3,
        errorString,
        unknownString,
        newlineString
      );
    }
  }
}

void serverSetup() {
  /* Configure multiplex mode, giving it 2 seconds */
  Serial.println(atMuxModeString);
  delay(2000);
  rxFlush();

  /* Start the server, giving it 2 seconds */
  Serial.println(atServerString);
  delay(2000);
  rxFlush();

  /* Debug print that we're done initializing */
  #if USE_BITBANG_TX
  bitbangTx((char*) "\rinitialized");
  #endif // USE_BITBANG_TX
}
