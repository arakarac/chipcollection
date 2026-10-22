#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
//#include <time.h>

#include "chip8.h"
#include "sdl.h"

int main(int argc, char *argv[]) {
  chip8 cpu = Init();
  chip8 *cpu_p = &cpu;
  initSDL();

  if (cpu_p == NULL) {
    printf("cpu is null");
    return 1;
    exit(1);
  }

  if (argc <= 1) {
    printf("no argument");
    return 1;
    exit(1);
  }

  FILE *file = fopen(argv[1], "rb");

  if (file == NULL) {
    printf("unable to read file");
    return 1;
    exit(1);
  }

  //long psize = ftell(file);
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file,0, SEEK_SET);

  byte buffer[fsize];
  size_t bfsize = fread(buffer, 1, fsize, file);
  for (size_t i = 0; i < bfsize; i++)
    cpu.mem[i + 512] = buffer[i];  

  printf("debug checkpoint: buffer in mem");

  printf("\nisRunning = %d\n", isRunning(&cpu));
  if (isRunning(&cpu)) {
    while (isRunning(&cpu)) {
      cycle(&cpu);
      if (draw(&cpu)) {
        render(&cpu);
      }
    }
  } else {
    fclose(file);
    endSDL();
  }

  return 0;
}

