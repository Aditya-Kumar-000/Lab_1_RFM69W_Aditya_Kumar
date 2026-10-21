#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
// #include the library for the RFM69 module and the UART
#include "RFM69.h"
#include "RFM69registers.h"
#include "millis.h"
#include "uart.h"
#include "xorrand.h"

int main(void) {
  millis_init(); // Required for RFM69
  rand_init();   // Initialise the RNG
  init_debug_uart0();
  rfm69_init(433);   // init the RFM69
  setPowerLevel(24); // set transmit power
  setChannel(12); // will be varying this to ensure it doesnt conflict with any
                  // other lab pair/grp and should be same as tx side code
  uint32_t last_tx_time = 0;
  char testing[] = "Test string 1";
  while (1) {
        // Indication: Service provider alerts upper layer of incoming frame
        if (receiveDone()) {
            printf("RX: %s (RSSI: %d)\r\n", (char *)DATA, RSSI);
        }

        // Request / Confirm: Send periodically once per second[cite: 11, 15]
        if (millis() - last_tx_time >= 1000) {
            mac_status_t status = mac_send_0_persistent((const void *)testing, 8);

            if (status == MAC_SUCCESS) {
                printf("TX: %s [OK]\r\n", testing);
            } else if (status == MAC_RX_BUFFER_OCCUPIED) {
                printf("TX Defer: Packet arriving, drained RX buffer\r\n");
                receiveDone(); // Drain buffer to clear deadlock
            } else {
                printf("TX Busy: Channel congested\r\n");
            }

            last_tx_time = millis();
        }
    }
}
