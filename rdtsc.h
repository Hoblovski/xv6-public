#ifndef RDTSC_H
#define RDTSC_H

static inline unsigned long long rdtsc() {
  register unsigned a __asm__("eax");
  register unsigned d __asm__("edx");
  __asm__ volatile (
      "rdtsc\n\t"
      : : : "eax", "edx");
  return (((unsigned long long) d) << 32) | ((unsigned long long) a);
}

// Type 1 checkpoints measures the time interval between consecutive checkpoints.
//
// Used for timers.
void type1_checkpoint(void);

// An entire line of report: format is AVG/MIN/MAX
void checkpoint_report(int reset);


// TODO: checkpoint_report and type1_checkpoint is actually using the same critical region.
// Should I add a lock?

#endif // RDTSC_H
