#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
myread(int fd)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
  }
  if(n < 0){
    printf(1, "read: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    myread(0);
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "read: cannot open %s\n", argv[i]);
      exit();
    }
    myread(fd);
    close(fd);
  }
  exit();
}
