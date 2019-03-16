#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  // Don't read -- just run forktest
  while (1)
    rlapic();
  exit();
}
