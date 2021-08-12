#include "tx.h"

bool txHandler(
  const char* connectionID,
  state_t* state,
  char* rxLock,
  const char* message
) {
  char* value;

  protocol_t protocol;
  uint16_t address;
  uint8_t command;
  uint8_t bits;
  uint_fast8_t repeats;

  char temp[6];

  /* Make sure we're in the idle state */
  if (*state != idleState) {
    /* Determine if the same client asking to transmit is currently in receive
     * mode */
    if (strcmp(rxLock, connectionID)) {
      /* Debug print command */
      #if USE_BITBANG_TX
      bitbangTx("\rbusy[");
      bitbangTx(txString);
      bitbangTx("]");
      #endif // USE_BITBANG_TX

      /* Reply over the network */
      tx(
        connectionID,
        3,
        errorString,
        busyString,
        newlineString
      );
    } else {
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
    }

    return true;
  }

  /* Debug print command */
  #if USE_BITBANG_TX
  bitbangTx("\rcommand[");
  bitbangTx(txString);
  bitbangTx("]");
  #endif // USE_BITBANG_TX

  /* By default, zero repeats */
  repeats = 0;

  /* Start at the beginning of the command (i.e., "tx(_") */
  value = (char*) message;
  while (value[0] != 'x' && value[0] != 0) value++;
  if (value[0] != 0) value++;

  /* Parse protocol by moving to the first x (i.e., "tx(0x_") */
  while (value[0] != 'x' && value[0] != 0) value++;
  if (value[0] != 0) value++;
  sscanf(value, "%x", (unsigned int*) &protocol);

  /* Parse address by moving to the next x (i.e., "...,0x_") */
  while (value[0] != 'x' && value[0] != 0) value++;
  if (value[0] != 0) value++;
  sscanf(value, "%x", &address);

  /* Parse command by moving to the next x (i.e., "...,0x_") */
  while (value[0] != 'x' && value[0] != 0) value++;
  if (value[0] != 0) value++;
  sscanf(value, "%hhx", &command);

  switch (protocol) {
    case PROTOCOL_NEC:
    case PROTOCOL_APPLE:
      /* Parse repeats (optional) by moving to the next x (i.e., "...,0x_") */
      while (value[0] != 'x' && value[0] != 0) value++;
      if (value[0] != 0) value++;
      if (value[0]) sscanf(value, "%hhx", &repeats);

      break;
    case PROTOCOL_SONY:
      /* Parse bits by moving to the next x (i.e., "...,0x_") */
      while (value[0] != 'x' && value[0] != 0) value++;
      if (value[0] != 0) value++;
      sscanf(value, "%hhx", &bits);

      /* Parse repeats (optional) by moving to the next x (i.e., "...,0x_") */
      while (value[0] != 'x' && value[0] != 0) value++;
      if (value[0] != 0) value++;
      if (value[0]) sscanf(value, "%hhx", &repeats);

      /* Sony is good with three repeats if none specified */
      if (!repeats) repeats = 3;

      break;
    default:
      /* Unknown protocol */
      tx(
        connectionID,
        4,
        errorString,
        txString,
        unknownString,
        newlineString
      );

      return true;
  }

  /* Repurpose serialBuffer as a command echo buffer. This is safe only because
   * we are done using message at this point (which is just serialBuffer plus
   * some offset). Use strlen() safely to avoid overfilling the buffer. */
  serialBuffer[0] = 0;
  /* tx(... */
  strncat(serialBuffer, txString, min(strlen(serialBuffer) - 1, strlen(txString)));
  strncat(serialBuffer, lParenString, min(strlen(serialBuffer) - 1, strlen(lParenString)));
  /* ...0xprotocol... */
  strncat(serialBuffer, hexString, min(strlen(serialBuffer) - 1, strlen(hexString)));
  itoa(protocol, temp, 16);
  strncat(serialBuffer, temp, min(strlen(serialBuffer) - 1, strlen(temp)));
  /* ...,0xaddress... */
  strncat(serialBuffer, commaString, min(strlen(serialBuffer) - 1, strlen(commaString)));
  strncat(serialBuffer, hexString, min(strlen(serialBuffer) - 1, strlen(hexString)));
  itoa(address, temp, 16);
  strncat(serialBuffer, temp, min(strlen(serialBuffer) - 1, strlen(temp)));
  /* ...,0xcommand... */
  strncat(serialBuffer, commaString, min(strlen(serialBuffer) - 1, strlen(commaString)));
  strncat(serialBuffer, hexString, min(strlen(serialBuffer) - 1, strlen(hexString)));
  itoa(command, temp, 16);
  strncat(serialBuffer, temp, min(strlen(serialBuffer) - 1, strlen(temp)));

  /* Send using the appropriate protocol */
  switch (protocol) {
    case PROTOCOL_NEC:
    case PROTOCOL_APPLE:
      if (protocol == PROTOCOL_NEC) {
        irSendNEC(address, command, repeats);
      } else {
        irSendApple(address, command, repeats);
      }

      /* Debug print command */
      #if USE_BITBANG_TX
      bitbangTx('\r');
      bitbangTx((protocol == PROTOCOL_NEC) ? "NEC" : "Apple");
      bitbangTx(" tx: [protocol: 0x");
      itoa(protocol, temp, 16);
      bitbangTx(temp);
      bitbangTx(", address: 0x");
      itoa(address, temp, 16);
      bitbangTx(temp);
      bitbangTx(", command: 0x");
      itoa(command, temp, 16);
      bitbangTx(temp);
      bitbangTx(", repeats: 0x");
      itoa(repeats, temp, 16);
      bitbangTx(temp);
      bitbangTx("]");
      #endif // USE_BITBANG_TX

      break;
    case PROTOCOL_SONY:
      /* ...,0xbits... */
      strncat(serialBuffer, commaString, min(strlen(serialBuffer) - 1, strlen(commaString)));
      strncat(serialBuffer, hexString, min(strlen(serialBuffer) - 1, strlen(hexString)));
      itoa(bits, temp, 16);
      strncat(serialBuffer, temp, min(strlen(serialBuffer) - 1, strlen(temp)));

      /* Send command */
      irSendSony(address, command, repeats, bits);

      /* Debug print command */
      #if USE_BITBANG_TX
      bitbangTx('\r');
      bitbangTx("Sony tx: [protocol: 0x");
      itoa(protocol, temp, 16);
      bitbangTx(temp);
      bitbangTx(", address: 0x");
      itoa(address, temp, 16);
      bitbangTx(temp);
      bitbangTx(", command: 0x");
      itoa(command, temp, 16);
      bitbangTx(temp);
      bitbangTx(", bits: 0x");
      itoa(bits, temp, 16);
      bitbangTx(temp);
      bitbangTx(", repeats: 0x");
      itoa(repeats, temp, 16);
      bitbangTx(temp);
      bitbangTx("]");
      #endif // USE_BITBANG_TX

      break;
    default:
      /* This is unreachable, but makes the compiler happy */
      break;
  }

  /* ...,0xrepeats... */
  strncat(serialBuffer, commaString, min(strlen(serialBuffer) - 1, strlen(commaString)));
  strncat(serialBuffer, hexString, min(strlen(serialBuffer) - 1, strlen(hexString)));
  itoa(repeats, temp, 16);
  strncat(serialBuffer, temp, min(strlen(serialBuffer) - 1, strlen(temp)));
  /* ...) */
  strncat(serialBuffer, rParenString, min(strlen(serialBuffer) - 1, strlen(rParenString)));

  /* Reply over the network */
  tx(
    connectionID,
    3,
    okString,
    serialBuffer,
    newlineString
  );

  return false;
}
