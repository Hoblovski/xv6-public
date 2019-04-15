#include "types.h"
#include "x86.h"
#include "defs.h"
#include "benchmark.h"

// Type 2 filters.
#define IGNORE_LESS_MIN 150000
#define IGNORE_MORE_MAX ~0ull
#define MAX_WINDOW_LENGTH 512

u64 vals[CHECKPOINT_MAXNS][MAX_WINDOW_LENGTH];
u64 cnt[CHECKPOINT_MAXNS];
u64 prev[CHECKPOINT_MAXNS];
u64 window_sz[CHECKPOINT_MAXNS] = {
  [0] = 32,
};


#define CPRINT_U64(a)             \
  do {                            \
    unsigned hi = a >> 32;        \
    unsigned lo = a & 0xFFFFFFFF; \
    if (hi)                       \
      cprintf(" %d:%d", hi, lo);  \
    else                          \
      cprintf(" %d", lo);         \
  } while (0);


double
newton_sqrt(double v)
{
  if (v > -1 && v < 1) return 0;
  double t = v/2;
  for (int i = 0; i < 20; i++) t = (t+v/t)/2;
  return t;
}


void report_stat(u64 *xs, int len) {
  u64 min = ~0ull, max = 0;
  double avg = 0, avg2 = 0;
  for(int i = 0; i < len; i++){
    avg += xs[i]; avg2 += xs[i]*xs[i];
    if (xs[i] < min) min = xs[i];
    if (xs[i] > max) max = xs[i];
  }
  avg /= len; avg2 /= len;
  double std = newton_sqrt(avg2 - avg*avg);
  u64 avg_ = avg;
  u64 std_ = std;

  cprintf("%d ", len);
  CPRINT_U64(avg_);
  CPRINT_U64(min);
  CPRINT_U64(max);
  CPRINT_U64(std_);
  double t = std / avg;
  cprintf(" %d%%", (uint) (t * 100));
}


//============================================================================
// Checkpoint related

void
checkpoint_report(int ns)
{
  cprintf(":: %d >> ", ns);
  report_stat(vals[ns], window_sz[ns]);
  cprintf("::\n");
}


void
type1_checkpoint(int ns)
{
  u64 now = rdtsc();
  if (cnt[ns] > 0) {
    u64 diff = now - prev[ns];   // wraps do no harm
    vals[ns][cnt[ns]-1] = diff;
  }
  if (cnt[ns]++ >= window_sz[ns]) {
    checkpoint_report(ns);
    cnt[ns] = 0;
  }
  prev[ns] = now;
}


void
type2_checkpoint_begin(int ns)
{
  prev[ns] = rdtsc();
}


void
type2_checkpoint_end(int ns)
{
  u64 now = rdtsc();
  u64 diff = now - prev[ns];   // wraps do no harm
  if (diff < IGNORE_LESS_MIN || diff > IGNORE_MORE_MAX)
    return;
  vals[ns][cnt[ns]++] = diff;
  if (cnt[ns] >= window_sz[ns]) {
    checkpoint_report(ns);
    cnt[ns] = 0;
  }
}


static inline void
__cpuid(uint op, uint *a, uint *b, uint *c, uint *d)
{
  __asm__ volatile (
      "cpuid\n\t"
      : "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d)
      : "0" (op), "2" (0)
      : "memory" );
}


void
cpuid_checks()
{
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


//============================================================================
// Quota related

i64 nc_quota = 0;
u64 quota_prev;


void
quota_accounting_begin(void)
{
  quota_prev = rdtsc();
}


void
quota_accounting_end(void)
{
  u64 now = rdtsc();
  u64 diff = now - quota_prev;   // wraps do no harm
  nc_quota -= diff;
}


//============================================================================
// Touch related

unsigned n_touch_len[TOUCH_MAX_N];
u64 n_touch[TOUCH_MAX_N][TOUCH_BUF_SZ];

void
touch(int n, u64 v)
{
  if (n_touch_len[n] >= TOUCH_BUF_SZ)
    return;
  n_touch[n][n_touch_len[n]++] = v;
}

void
report_touch(int n)
{
  cprintf("{t%d: ", n);
  report_stat(n_touch[n], n_touch_len[n]);
  n_touch_len[n] = 0;
  cprintf("}\n");
}

void
report_touches()
{
  for(int i = 0; i < TOUCH_MAX_N; i++)
    report_touch(i);
}
