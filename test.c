#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv )
{

  char* name = argv[1];
  if (!name) {
    std::cout << "!!! arg: <filename.cpp>" << std::endl;
    return -1;
  }  

  char cmd[500];
  sprintf(cmd,"Lines: drew %i out of %lu",num_drawn,contours.size());


g++ contours.cpp -o contours  `pkg-config --cflags --libs opencv` -Wall

  std::cout << "Hello World!\n";
}

// g++ test.c -o test
