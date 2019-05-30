#include "FXOS8700Q. h"
#include <stdbool.h>
#include <stdint.h>
#include <mbed.h>
#include <stdio.h>
#include "can.h"
#include "scheduler.h"
#include "counter.h"
#include <assert.h>


tatic DigitalOut red(LED_RED);
static Serial pc(USBTX, USBRX, 115200); //serial conn

static volatile canMessage_t rxMsg;
Semaphore rxDone(1);
Thread writer(osPriorityRealtime);
Thread reader(osPriorityRealtime1);

static void canReadTask(void);
static void canWriteTask(void);
static void canHandler(void);

int main () {
    osStatus status;

    red = 0;
    canInit(BD125000, true);
    pc.printf("Display --  \n");

    status = reader.start(canReadTask);
    assert(osOK == status);
    status = writer.start(canWriteTask);
    assert(osOK == status);

    while (true) {
      wait_ms(99999);
    }
}

/* Transmit CAN message with arbitrary id and 8 bytes of
 * data consisting of a repeated count of the number of transmitted messages
 */ 
void canWriteTask(void) {
  
  static canMessage_t txMsg = {0x23, 8, 0, 0}; 
  bool txOk;
    
  while (true) {
      // Transmit message on CAN 
      txOk = canWrite(&txMsg);
      if (txOk) {
	txCount += 1;
	txMsg.dataA = txCount;
	txMsg.dataB = txCount;
      }
      wait_ms(250);
  }
}

/* Read and display messages arriving on the CAN port */
void canReadTask(void) {
  canRxInterrupt(canHandler); // configure CAN to interrupt on message reception

  rxDone = false;
  while (true) {
      if (rxDone) { // rxDone could be better handled by a semaphore
	rxDone = false;
        pc.printf("ID: 0x%lx LEN: 0x%01lx DATA_A: 0x%08lx DATA_B: 0x%08lx\n", rxMsg.id, rxMsg.len, rxMsg.dataA, rxMsg.dataB); 
        rxCount += 1;
      }
      wait_ms(100);
  }
}

/* A simple interrupt handler for CAN message reception */
void canHandler(void) {
    canTransferRxFrame(&rxMsg);
    rxDone = true;
}
