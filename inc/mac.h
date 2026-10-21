#ifndef MAC_H_
#define MAC_H_

#include <stdint.h>

// MAC Layer Return Status Codes
typedef enum {
    MAC_SUCCESS           = 0,
    MAC_CHANNEL_BUSY      = 1,
    MAC_RX_BUFFER_OCCUPIED = 2,
    MAC_TX_FAILED         = 3
} mac_status_t;

// Initialise the MAC sublayer
void mac_init(void);

// 0-Persistent CSMA transmit with status return
mac_status_t mac_send_0_persistent(const void* buffer, uint8_t bufferSize);

// 1-Persistent CSMA transmit with status return
mac_status_t mac_send_1_persistent(const void* buffer, uint8_t bufferSize);

#endif /* MAC_H_ */
