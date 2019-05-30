#include "FXOS8700Q. h"
#include <stdbool.h>
#include <stdint.h>
#include <mbed.h>
#include <stdio.h>
#include "can.h"
#include "scheduler.h"
#include "counter.h"
#include <assert.h>

s 
I2C i2c (PTE25 , PTE24) ;
FXOS8700QAccelerometer acc ( i2c , FXOS8700CQ SLAVE ADDR1) ; // acceloremeter, send message to CAN

unit32_t fullTime = 0;//time taken to complete task
unit32_t avgTime; //test averges
unit32_t minTime = 10000; //shortest time
unit32_t maxTime = 0;//longest time
unit32_t counterTime = 0;//counter time
unit32_t counterTest;// counter number of times

void tempT( void ) ;
static void tachoT( void ) ;
int16_t raX , raY , raZ , rmX, rmY, rmZ, tmp_int ; // accelrameter raY reading stored


int main () {
	red = 0;
	counterInit() ; // initialize counter
    counterStart() ; // start counter
    counterCompTime = counterStop (); //time taken to start and stop counter
    canInit(BD125000 , false) ; // CAN on
    pc.printf("EngineMonitor -- Loopback test\n");
	schInit();
    schAddTask(tachoT, 1, 20);
    schAddTask(tachoT, 2, 500);

  schStart();
  
  while (true) {
    schDispatch();
  }
}
void tachoT( void ) {
int16t dummyx = acc.getY( raY) ; // acc reading
}

void tempT( void ) {
	pc.printf(" test count\t : %020lu\n" , counterTest);//number of the test runnign
	static canMessage_t ackVal;
	if (canReady ()) {
		canRead(&ackVal ) ;
		if ( ackVal . ID == 0x23 ) {
			if ( counterTest < 1000) {
				counterStart() ;
				canMessage_t txMsg = {0x23 , 8 , acc.getY(raY) , acc.getY(raY)// message place can line

			};
			bool txOk ;
			txOk = canWrite(&txMsg) ;
			fullTime = counterStop() - counterCompTime;
			counterTest++;
		}
	}
}
if (fullTime < minTime) {
	minTime = fullTime;
}
if (fullTime > maxTime) {
	maxTime = fullTime;
}
avgTime += fullTime;
if(counterTest = 1000){ //test finishes
	avgTime = avgTime / counterTest;
	pc.printf(" test count\t : %020lu\n" , counterTest);
	pc.printf(" minimum\t : %020lu\n" , minTime);
	pc.printf(" maximum\t : %020lu\n" , maxTime);
	pc.printf(" average\t : %020lu\n" , avgTime);
	counterTest++;
}
