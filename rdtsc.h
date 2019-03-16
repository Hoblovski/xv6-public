#ifndef RDTSC_H
#define RDTSC_H

#define WINDOW_LENGTH 256

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
// It reports once in every WINDOW_LENGTH invocations.
//
// Used for timers.
void type1_checkpoint(void);


#endif // RDTSC_H
