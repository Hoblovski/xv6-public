#include "types.h"
#include "stat.h"
#include "user.h"

static inline void
outb(ushort port, uchar data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

int
main(int argc, char *argv[])
{
  printf(1, "Usage: porttest [r]\n");
  printf(1, "  When the 'r' is present, request port access before `outb 0x80 0`, thus port access shall be fine.\n");
  printf(1, "  Otherwise without requesting port access, shall trigger GP (#13).\n\n");

  if (argc == 2 && !strcmp(argv[1], "r")) {
    printf(1, "request port\n");
    requestport(0x80);
  }

  printf(1, "outb...\n");
  ushort port = 0x80;
  uchar data = 40;
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  printf(1, "done\n");

  exit();
}
