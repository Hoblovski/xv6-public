#include "types.h"
#include "x86.h"
#include "defs.h"
#include "benchmark.h"

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


void type2_checkpoint_begin(void)
{
  prev = rdtsc();
}


void type2_checkpoint_end(void)
{
  unsigned long long now = rdtsc();
  unsigned long long diff = now - prev;   // wraps do no harm
  if (diff < IGNORE_LESS_MIN || diff > IGNORE_MORE_MAX)
    return;
  if (diff < min) min = diff;
  if (diff > max) max = diff;
  vals[cnt++] = diff;
  if (cnt >= WINDOW_LENGTH) {
    checkpoint_report();
    cnt = 0;
    max = 0;
    min = ~0ull;
  }
}

void type2_checkpoint_end_noreport(void)
{
  unsigned long long now = rdtsc();
  unsigned long long diff = now - prev;   // wraps do no harm
  if (diff < IGNORE_LESS_MIN || diff > IGNORE_MORE_MAX)
    return;
  if (diff < min) min = diff;
  if (diff > max) max = diff;
  vals[cnt++] = diff;
}

void type2_checkpoint_report(void)
{
  if (cnt >= WINDOW_LENGTH) {
    checkpoint_report();
    cnt = 0;
    max = 0;
    min = ~0ull;
  }
}


void cpuid_checks() {
  uint a, b, c, d;
  __cpuid(0x80000007, &a, &b, &c, &d);
  if (!(d & (1<<8)))
    cprintf("warning: invariant tsc: no\n");
  else
    cprintf("invariant tsc: yes\n");

  __cpuid(0x15, &a, &b, &c, &d);
  cprintf("CPUID.15H: %d %d %d %d\n", a, b, c, d);

  __cpuid(0x1, &a, &b, &c, &d);
  if (!(c & (1<<24)))
    panic("no tsc deadline");
  cprintf("tsc_deadline: yes\n");
}

