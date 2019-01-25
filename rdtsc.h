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

#define BEGIN_TIMER \
  unsigned long long __beg_ticks = rdtsc();

#define END_TIMER_REPORT \
  unsigned long long __elapsed_ticks = rdtsc() - __beg_ticks; \
  unsigned __t1 = __elapsed_ticks >> 32; \
  unsigned __t2 = __elapsed_ticks & 0xFFFFFFFF; \
  cprintf("\n[elapsed] 0x%x 0x%x\n", __t1, __t2); \

#define SINGLE_REPORT \
  unsigned long long __now_ticks = rdtsc(); \
  unsigned __t1 = __now_ticks >> 32; \
  unsigned __t2 = __now_ticks & 0xFFFFFFFF; \
  cprintf("\n[now] 0x%x 0x%x\n", __t1, __t2);

#endif // RDTSC_H
