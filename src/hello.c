#include <stdio.h>
#include <config.h>

int main(int argc, char **argv) {
  printf("hello world %s\n", PACKAGE_STRING);
  return 0;
}
