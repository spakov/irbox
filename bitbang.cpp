#include "bitbang.h"

#if USE_BITBANG_TX

void bitbangTx(char c) {
  /* Header (1010) */
  digitalWrite(BITBANG_TX_PIN, HIGH);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, HIGH);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, LOW);
  delay(BITBANG_DELAY);

  /* Byte (MSB first) */
  digitalWrite(BITBANG_TX_PIN, (c & 0x80) >> 7 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x40) >> 6 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x20) >> 5 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x10) >> 4 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x08) >> 3 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x04) >> 2 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x02) >> 1 ? HIGH : LOW);
  delay(BITBANG_DELAY);
  digitalWrite(BITBANG_TX_PIN, (c & 0x01) ? HIGH : LOW);
  delay(BITBANG_DELAY);

  /* Default to LOW */
  digitalWrite(BITBANG_TX_PIN, LOW);
  delay(BITBANG_DELAY);
}

void bitbangTx(const char* s, size_t len) {
  for (size_t i = 0; i < len; i++) bitbangTx((char) s[i]);
}

void bitbangTx(const char* s) {
  for (size_t i = 0; i < strlen(s); i++) bitbangTx((char) s[i]);
}

#endif // USE_BITBANG_TX
