#include "rdtsc.h"
#include "types.h"
#include "defs.h"


double avg = 0;   // Using avg' = (avg*n + x) / (n+1), long long will overflow quickly.
unsigned long long cnt = 0;   // assume will not overflow
unsigned long long min = ~0ull;
unsigned long long max = 0;;
unsigned long long prev;


void
type1_checkpoint(void)
{
  unsigned long long now = rdtsc();
  if (cnt > 0) {
    unsigned long long diff = now - prev;   // wraps do no harm
    if (diff < min) min = diff;
    if (diff > max) max = diff;
    avg = avg / (cnt + 1) * cnt + ((double) diff / (cnt + 1));
  }
  cnt++;
  prev = now;
}


void checkpoint_report(int reset)
{
  unsigned long long avg_ = (unsigned long long) avg;
  unsigned avghi = avg_ >> 32;
  unsigned avglo = avg_;
  unsigned minhi = min >> 32;
  unsigned minlo = min;
  unsigned maxhi = max >> 32;
  unsigned maxlo = max;
  cprintf("REP %x:%x/%x:%x/%x:%x\n", avghi, avglo, minhi, minlo, maxhi, maxlo);

  if (reset) {
    cnt = 0;
    avg = 0;
    max = 0;
    min = ~0ull;
  }
}
