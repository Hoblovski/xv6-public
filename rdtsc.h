#ifndef RDTSC_H
#define RDTSC_H

#define WINDOW_LENGTH 256
// Below only used for type 2. To filter out failure path etc.
#define IGNORE_LESS_MIN 150000
#define IGNORE_MORE_MAX ~0ull

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


// Type 2 checkpoints measures the time interval between begin and end checkpoints.
//
// type2_checkpoint_end reports once in every WINDOW_LENGTH invocations.
//
// Used for forks etc.
void type2_checkpoint_begin(void);
void type2_checkpoint_end(void);

// Never mix type 1 and type 2. Profile only one checkpoint every time!!!
// Because global data is used.

#endif // RDTSC_H
