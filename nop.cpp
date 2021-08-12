#include "nop.h"

void nopHandler(const char* connectionID) {
  /* Debug print command */
  #if USE_BITBANG_TX
  bitbangTx("\rcommand[");
  bitbangTx(nopString);
  bitbangTx("]");
  #endif // USE_BITBANG_TX

  /* Reply over the network */
  tx(
    connectionID,
    3,
    okString,
    nopString,
    newlineString
  );
}
