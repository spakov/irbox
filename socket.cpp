#include "socket.h"

void parseConnectionID(char* buffer, char* connectionID) {
  char* numeric = buffer;

  /* Skip characters until we get to something numeric or hit the length limit
   * */
  for (int i = 0; i < 7; i++) {
    /* Check for length limit */
    if (i > 6) return;

    /* Look for numbers */
    if (numeric[0] >= '0' && numeric[0] <= '9') break;
    numeric++;
  }

  /* Get the current connection ID */
  connectionID[0] = 0;
  connectionID[1] = 0;
  switch (atoi(numeric)) {
    case 0:
      connectionID[0] = '0';
      break;
    case 1:
      connectionID[0] = '1';
      break;
    case 2:
      connectionID[0] = '2';
      break;
    case 3:
      connectionID[0] = '3';
      break;
  }
}

size_t rx() {
  String message;
  size_t len;

  /* Start with empty string */
  serialBuffer[0] = 0;

  /* Make sure there is data to read */
  if (!Serial.available()) return 0;

  /* Read the message */
  len = Serial.readBytesUntil('\n', serialBuffer, SERIAL_BUFFER_SIZE);
  serialBuffer[len] = 0;

  #if USE_BITBANG_TX
  #if VERBOSE_BITBANG_TX
  bitbangTx("\rrawrx[");
  bitbangTx(serialBuffer);
  bitbangTx("].");
  #endif // VERBOSE_BITBANG_TX
  #endif // USE_BITBANG_TX

  return len;
}

void rxFlush() {
  #if USE_BITBANG_TX
  #if VERBOSE_BITBANG_TX
  bitbangTx("\rflush:[");
  bitbangTx(serialBuffer);
  #endif // VERBOSE_BITBANG_TX
  #endif // USE_BITBANG_TX

  while (rx());

  #if USE_BITBANG_TX
  #if VERBOSE_BITBANG_TX
  bitbangTx(serialBuffer);
  bitbangTx("\r]:flush");
  #endif // VERBOSE_BITBANG_TX
  #endif // USE_BITBANG_TX
}

void tx(const char* connectionID, uint8_t argc, ...) {
  va_list va;

  size_t len;

  /* Calculate message length */
  len = 0;
  va_start(va, argc);
  for (uint8_t i = 0; i < argc; i++) {
    len += strlen(va_arg(va, char*));
  }
  va_end(va);

  /* Send message */
  txStart(connectionID, len);
  va_start(va, argc);
  for (uint8_t i = 0; i < argc; i++) {
    Serial.print(va_arg(va, char*));
  }
  va_end(va);
}

void txStart(const char* connectionID, size_t len) {
  Serial.print(atSendString);
  Serial.print(connectionID);
  Serial.print(',');
  Serial.println(len);
  /* ESP8266 requires some time after an AT+CIPSEND */
  delay(20);
}
