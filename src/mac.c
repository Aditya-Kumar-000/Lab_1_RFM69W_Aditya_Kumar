#include "mac.h"
#include "RFM69.h"
#include "millis.h"
#include "xorrand.h"
#include <stdio.h>

#define MAX_CSMA_RETRIES 10

void mac_init(void) {
    // Initialise random generator and default radio states
    rand_init();
}

mac_status_t mac_send_0_persistent(const void* buffer, uint8_t bufferSize) {
    uint8_t retries = 0;

    // Check if an unread packet is already sitting in the receiver buffer
    if (phy_rx_buffer_has_data()) {
        return MAC_RX_BUFFER_OCCUPIED;
    }

    while (retries < MAX_CSMA_RETRIES) {
        // Physical Layer: Carrier sense (returns 1 if idle, 0 if busy)
        if (phy_is_channel_idle()) {
            // Channel idle: transmit frame via PHY layer
            phy_transmit(buffer, bufferSize);
            return MAC_SUCCESS;
        }

        // Channel is busy: execute random backoff
        retries++;
        uint16_t backoff = 10 + (get_rand() % 50);

        // Save power: put PHY into standby during backoff
        phy_set_standby();

        uint32_t start_time = millis();
        while (millis() - start_time < backoff) {
            // Backoff delay
        }

        // Put PHY back to RX mode for next carrier sense
        phy_set_rx();

        // Check if an incoming packet arrived during backoff
        if (phy_rx_buffer_has_data()) {
            return MAC_RX_BUFFER_OCCUPIED;
        }
    }

    return MAC_CHANNEL_BUSY;
}

mac_status_t mac_send_1_persistent(const void* buffer, uint8_t bufferSize) {
    uint32_t timeout_start = millis();
    const uint32_t persistent_timeout = 2000; // 2 second timeout guard

    if (phy_rx_buffer_has_data()) {
        return MAC_RX_BUFFER_OCCUPIED;
    }

    phy_set_rx();

    // 1-persistent: sense continuously until idle or timeout
    while (!phy_is_channel_idle()) {
        if (phy_rx_buffer_has_data()) {
            return MAC_RX_BUFFER_OCCUPIED;
        }
        if (millis() - timeout_start > persistent_timeout) {
            return MAC_CHANNEL_BUSY;
        }
    }

    // Channel free: transmit immediately
    phy_transmit(buffer, bufferSize);
    return MAC_SUCCESS;
}
