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
// +++++++++++++++ RECEIVE +++++++++++++++++
    // Your code here
    if (receiveDone()) {
      printf("RX: %s\r\n", (char *)DATA);
    }

// +++++++++++++++ TRANSMISSION +++++++++++++    


    if (millis() - last_tx_time >= 1000) 
    {
      if (canSend()) 
      {
        // Call send0 (0-persistent) or send1 (1-persistent)
        send((const void *)testing, sizeof(testing));
        printf("TX: %s\r\n", testing);
        last_tx_time = millis(); // resets the timer
      }
    }
  }
}
