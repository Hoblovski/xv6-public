#include "rdtsc.h"
#include "types.h"
#include "defs.h"

unsigned long long vals[WINDOW_LENGTH];
unsigned long long cnt = 0;   // assume will not overflow
unsigned long long min = ~0ull;
unsigned long long max = 0;;
unsigned long long prev;


void checkpoint_report(void)
{
  double avg = 0;
  for (int i = 0; i < WINDOW_LENGTH; i++)
    avg += vals[i];
  avg /= WINDOW_LENGTH;
  unsigned long long avg_ = avg;
  unsigned avghi = avg_ >> 32;
  unsigned avglo = avg_;
  unsigned minhi = min >> 32;
  unsigned minlo = min;
  unsigned maxhi = max >> 32;
  unsigned maxlo = max;
  cprintf("REPx %x:%x/%x:%x/%x:%x\n", avghi, avglo, minhi, minlo, maxhi, maxlo);
  cprintf("REPd %d:%d/%d:%d/%d:%d\n", avghi, avglo, minhi, minlo, maxhi, maxlo);
}


void
type1_checkpoint(void)
{
  unsigned long long now = rdtsc();
  if (cnt > 0) {
    unsigned long long diff = now - prev;   // wraps do no harm
    if (diff < min) min = diff;
    if (diff > max) max = diff;
    vals[cnt-1] = diff;
  }
  if (cnt++ >= WINDOW_LENGTH) {
    checkpoint_report();
    cnt = 0;
    max = 0;
    min = ~0ull;
  }
  prev = now;
}


