#include "bitbang-echo.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  char c;

  while (true) {
    /* Check for frame header (1010) */
    if (readBit()) {
      /* 0 */
      delay(BITBANG_DELAY);
      if (readBit()) continue;

      /* 1 */
      delay(BITBANG_DELAY);
      if (!readBit()) continue;

      /* 0 */
      delay(BITBANG_DELAY);
      if (readBit()) continue;

      /* Subsequent bits are data, MSB first */
      c = 0;
      for (uint8_t i = 8; i > 0; i--) {
        delay(BITBANG_DELAY);
        c += readBit() << (i - 1);
      }

      Serial.print(c);
      if (c == '\r') Serial.print('\n');

      /* Flush "buffer" */
      while(readBit());
    }
  }
}

bool readBit() {
  return analogRead(BITBANG_RX_PIN) > BITBANG_ONE_CUTOFF ? 1 : 0;
}
