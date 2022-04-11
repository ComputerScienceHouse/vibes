#ifndef _CREDS_H_
#define _CREDS_H_

// Move this to credentials.h
// There's a gitignore that should prevent this from being committed
// (Don't commit secrets)

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "<your SSID>"
#define WLAN_PASS       "<your PASSWORD>"

/************************* MQTT Setup *********************************/

#define AIO_SERVER      "<your broker>"
#define AIO_SERVERPORT  1883

// If necessary
#define AIO_USERNAME    "user"
#define AIO_KEY         "key"

#endif
