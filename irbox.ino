#include "irbox.h"

/* Serial data buffer */
char serialBuffer[SERIAL_BUFFER_SIZE];

void setup() {
  /* Initialize infrared sending. We don't initialize receiving until we get an
   * rx command. */
  irSendBegin(IR_TX_PIN);

  /* Initialize serial communication */
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(10);

  /* Set up bitbanging transmission */
  #if USE_BITBANG_TX
  pinMode(BITBANG_TX_PIN, OUTPUT);
  digitalWrite(BITBANG_TX_PIN, LOW);
  bitbangTx('\r');
  #endif // USE_BITBANG_TX

  /* Initialization delay */
  delay(INIT_DELAY);
}

void loop() {
  serverSetup();
  serverLoop();
}
