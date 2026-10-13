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
    // Your code here
    if (receiveDone()) {
      printf("RX: %s\r\n", (char *)DATA);
    }
    // Check if 1000 ms (1 second) has passed:
    if (millis() - last_tx_time >= 1000) {
      // Check if the wireless airwaves are free (Carrier Sensing)
      if (canSend()) { // we can only send if cansend is 1 ensuring carrier
                       // sensing is passed, ensuring the broadcasint channel is
                       // free (idle) (if
        // if can send is high then channel is idle other wise someone is
        // transmitting and must abort
        // Transmit
        send((const void *)testing, sizeof(testing));

        // once canSend has passed, send() takes the mem of 'testing' in MCU
        // SRAM and pulls n_ss low then further transfer those bytes over to the
        // MOSI line (the role of send function)
        printf("TX: %s\r\n", testing);
        // recording current time
        last_tx_time = millis(); // resets the timer
      }
    }
  }
}
