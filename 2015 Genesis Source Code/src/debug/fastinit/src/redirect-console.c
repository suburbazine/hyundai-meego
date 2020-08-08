/*
 * (C) 2009 Intel Corporation 
 * Licensed under the terms of the GPL version 2 license
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int fd;
  char *con = "/dev/console";
  
  if (argc>1) {
    con = argv[1];
  }
  fd = open(con, O_RDWR);
  if (fd < 0) {
    printf("redirect: Open failed\n");
    exit(0);
  }
  if (ioctl(fd, TIOCCONS) < 0) {
//    printf("ioctl returned negative\n");
  }
  return EXIT_SUCCESS;
}
