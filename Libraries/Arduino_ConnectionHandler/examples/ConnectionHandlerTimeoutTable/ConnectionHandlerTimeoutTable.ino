/* SECRET_ fields are in `arduino_secrets.h` (included below)
 *
 * This example is a lightly modified version of ConnectionHandlerDemo to showcase
 * the possibility of changing the timeout values for the different states a connectionhandler have
 */

#include <Arduino_ConnectionHandler.h>

#include "arduino_secrets.h"

#if !(defined(BOARD_HAS_WIFI) || defined(BOARD_HAS_GSM) || defined(BOARD_HAS_LORA) || \
      defined(BOARD_HAS_NB) || defined(BOARD_HAS_ETHERNET) || defined(BOARD_HAS_CATM1_NBIOT))
  #error "Please check Arduino Connection Handler supported boards list: https://github.com/arduino-libraries/Arduino_ConnectionHandler/blob/master/README.md"
#endif

#if defined(BOARD_HAS_ETHERNET)
EthernetConnectionHandler conMan(SECRET_IP, SECRET_DNS, SECRET_GATEWAY, SECRET_NETMASK);
#elif defined(BOARD_HAS_WIFI)
WiFiConnectionHandler conMan(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
#elif defined(BOARD_HAS_GSM)
GSMConnectionHandler conMan(SECRET_PIN, SECRET_APN, SECRET_GSM_USER, SECRET_GSM_PASS);
#elif defined(BOARD_HAS_NB)
NBConnectionHandler conMan(SECRET_PIN);
#elif defined(BOARD_HAS_LORA)
LoRaConnectionHandler conMan(SECRET_APP_EUI, SECRET_APP_KEY);
#elif defined(BOARD_HAS_CATM1_NBIOT)
CatM1ConnectionHandler conMan(SECRET_PIN, SECRET_APN, SECRET_GSM_USER, SECRET_GSM_PASS);
#elif defined(BOARD_HAS_CELLULAR)
CellularConnectionHandler conMan(SECRET_PIN, SECRET_APN, SECRET_GSM_USER, SECRET_GSM_PASS);
#endif

bool attemptConnect = false;
uint32_t lastConnToggleMs = 0;

void setup() {
  /* Initialize serial debug port and wait up to 5 seconds for port to open */
  Serial.begin(9600);
  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime <= 5000); ) { }

#ifndef __AVR__
  /* Set the debug message level:
   * - DBG_ERROR: Only show error messages
   * - DBG_WARNING: Show warning and error messages
   * - DBG_INFO: Show info, warning, and error messages
   * - DBG_DEBUG: Show debug, info, warning, and error messages
   * - DBG_VERBOSE: Show all messages
   */
  setDebugMessageLevel(DBG_INFO);
#endif

  /* Add callbacks to the ConnectionHandler object to get notified of network
   * connection events. */
  conMan.addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);
  conMan.addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);
  conMan.addCallback(NetworkConnectionEvent::ERROR, onNetworkError);

  /* By using updateTimeoutInterval I can change the timeout value for a specific
   * state of the connection handler
   */
  conMan.updateTimeoutInterval(NetworkConnectionState::INIT, 8000);
  conMan.updateTimeoutInterval(NetworkConnectionState::CONNECTING, 1000);
  conMan.updateTimeoutInterval(NetworkConnectionState::CONNECTED, 20000);
  conMan.updateTimeoutInterval(NetworkConnectionState::DISCONNECTING, 200);
  conMan.updateTimeoutInterval(NetworkConnectionState::DISCONNECTED, 2000);
  conMan.updateTimeoutInterval(NetworkConnectionState::CLOSED, 2000);
  conMan.updateTimeoutInterval(NetworkConnectionState::ERROR, 2000);
}

void loop() {
  conMan.check();
}

void onNetworkConnect() {
  Serial.println(">>>> CONNECTED to network");
}

void onNetworkDisconnect() {
  Serial.println(">>>> DISCONNECTED from network");
}

void onNetworkError() {
  Serial.println(">>>> ERROR");
}
