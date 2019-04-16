#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
myread(int fd)
{
  int n;

  for (int i = 0; i < 71168 / 512; i++)
    if ((n = read(fd, buf, sizeof(buf))) != 512)
      printf(1, "read: premature incomplete read!\n");
}

int
main(int argc, char *argv[])
{
  int fd;

  while (1) {
    if((fd = open("READ_BENCH", 0)) < 0){
      printf(1, "read: cannot open READ_BENCH\n");
      exit();
    }
    myread(fd);
    close(fd);
  }
}
