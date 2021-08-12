#include "ir.h"

/* For some reason, IRremote instantiates objects in its header files.
 * Therefore, we must abstract all IR functionality into a single object. */
#include <IRremote.h>

void irReceiveBegin(uint8_t receivePin) {
  IrReceiver.begin(receivePin, false, 0);
}

void irSendBegin(uint8_t sendPin) {
  IrSender.begin(sendPin, false, 0);
}

void irDecode(const char* connectionID) {
  /* Temporary working area for hex digits. Max is ffffffff, hence eight
   * characters plus null terminator. */
  char temp[9];

  /* Make sure there's a connection */
  if (connectionID[0] == 0) return;

  /* Get data */
  if (IrReceiver.decode()) {
    digitalWrite(13, HIGH);
    delay(20);
    digitalWrite(13, LOW);

    /* Build the command string */
    serialBuffer[0] = 0;
    strcat(serialBuffer, okString);
    strcat(serialBuffer, txString);
    strcat(serialBuffer, lParenString);
    switch (IrReceiver.decodedIRData.protocol) {
      case NEC:
      case APPLE:
        /* Protocol */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.protocol, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, commaString);

        /* Address */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.address, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, commaString);

        /* Command */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.command, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, rParenString);

        break;
      case SONY:
        /* Protocol */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.protocol, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, commaString);

        /* Address */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.address, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, commaString);

        /* Command */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.command, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, commaString);

        /* Number of bits */
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.numberOfBits, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, rParenString);

        break;
      default:
        /* protocol 0xnn unknown, raw 0x... */
        strcat(serialBuffer, protocolString);
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.protocol, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, unknownString);
        strcat(serialBuffer, rawString);
        strcat(serialBuffer, hexString);
        itoa(IrReceiver.decodedIRData.decodedRawData, temp, 16);
        strcat(serialBuffer, temp);
        strcat(serialBuffer, rParenString);

        break;
    }

    #if USE_BITBANG_TX
    bitbangTx((char*) "\rtx[");
    bitbangTx(serialBuffer);
    bitbangTx("]");
    #endif // USE_BITBANG_TX

    /* Send data over the network */
    tx(
      connectionID,
      2,
      serialBuffer,
      newlineString
    );

    /* Resume receiving */
    IrReceiver.resume();
  }
}

void irSendNEC(uint16_t address, uint8_t command, uint8_t repeats) {
  IrSender.sendNEC(address, command, repeats, false);
}

void irSendApple(uint16_t address, uint8_t command, uint8_t repeats) {
  IrSender.sendApple(address, command, repeats, false);
}

void irSendSony(uint16_t address, uint8_t command, uint8_t repeats, uint8_t bits) {
  IrSender.sendSony(address, command, repeats, bits);
}
