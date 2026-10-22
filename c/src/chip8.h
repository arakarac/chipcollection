#ifndef CHIP8
#define CHIP8

#include <stdint.h>
#include <stdbool.h>

enum { MEMSIZE = 4096 };
enum { ADDRSIZE = 16 };
enum { WIDTH = 64 };
enum { HEIGHT = 32 };
// const seconds??
enum { CYCLES = 10 };
enum { FONTSIZE = 80 };

typedef uint8_t byte;
typedef uint8_t buffer_c8[HEIGHT][WIDTH];
typedef struct chip8 {
  char mem[MEMSIZE]; 
  uint8_t v[ADDRSIZE];
  uint16_t ir;
  uint16_t pc;
  int key[16]; 
  buffer_c8 display; 
  uint16_t stack[ADDRSIZE]; 
  uint16_t op;
  uint16_t sp;  //pointer or not???
  uint16_t dTimer;
  uint16_t sTimer;
  bool canDraw;
  bool running;
} chip8;

chip8 Init();
int cycle(chip8 *c);
bool isRunning(chip8 *c);
bool draw(chip8 *c);

#endif // !CHIP8


