#include <xv6/types.h>
#include <xv6/param.h>
#include <xv6/memlayout.h>
#include <xv6/riscv.h>
#include <xv6/defs.h>

//
// Code for the Goldfish RTC, and time in general
//

// Return the current time in seconds as an unsigned 64-bit integer
uint64 sys_time(void) {

  // The RTC returns the current time in nanoseconds as an unsigned
  // 64 bit value. This comes from two separate 32-bit registers.
  volatile uint32 *rtc= (uint32 *)GOLDFISH_RTC;
  uint64 lownano= rtc[0];
  uint64 highnano= rtc[1];

  // Join these to get the full time, then divide it by a billion
  uint64 nanotime= (highnano << 32) | lownano;
  nanotime= nanotime / 1000000000;

  return(nanotime);
}
