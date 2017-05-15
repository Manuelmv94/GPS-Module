#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "GPSManager.h"




void main(void) {
  EnableInterrupts;
  /* include your code here */
  GPSManager_Init();
  

  for(;;) {
    __RESET_WATCHDOG();	/* feeds the dog */
    GPSManager_PeriodicTask();
  } /* loop forever */
  /* please make sure that you never leave main */
}
