#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chip8.h"

void increment(chip8 *c) {
  c->pc += 2;
}

int loadFont(chip8 *c) {
  const uint8_t font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
  };

  for (int i = 0; i < FONTSIZE; i++) {
    c->mem[i] = font[i];
  }
  return 0;
}

chip8 Init() {
  chip8 c;
  c.pc = 0x200;
  c.canDraw = true;
  c.running = true;
  return c;
}

int clearDisplay(chip8 *c) {
  int d = sizeof(c->display);
  for (int i = 0; i < d; i++) {
    for (int j = 0; j < d; j++) {
      c->display[i][j] = 0x0;
    }
  }
  return 0;
}

bool draw(chip8 *c) {
  bool b = c->canDraw;
  c->canDraw = false;
  return b;
}

bool isRunning(chip8 *c) {
  //bool b = c->running;
  //c->running = false;
  return c->running;
}

int cycle(chip8 *c) {
  uint16_t opcode = c->mem[c->pc] <<8 | c->mem[c->pc + 1];
  c->op = opcode;
  
  printf("debug checkpoint: cycle start");

  int x = (c->op & 0x0F00) >> 8; 
  int y = (c->op & 0x00F0) >> 4; 
  int f = 0xF; 
  int n = (c->op & 0x000F); 
  uint8_t nn = (c->op & 0x00FF); 
  int nnn = c->op & 0x0FFF;

  switch (c->op & 0xF000) {
  case 0x0000:
    switch (c->op & 0x000F) {
    case 0x0000:
      clearDisplay(c);
      increment(c);
      c->canDraw = true;
        break;
    case 0x000E:
      --c->sp;
      c->pc = c->stack[c->sp];
      increment(c);
        break;
    }
    break;

  case 0x1000:
    c->pc = nnn; 
    break;

  case 0x2000:
    c->stack[c->sp] = c->pc;
    ++c->sp;
    c->pc = nnn;
    break;

  case 0x3000:
    increment(c);
    if (c->v[x] == nn) {
      increment(c);
    } 
    break;

  case 0x4000:
    increment(c);
    if (c->v[x] != nn) {
      increment(c);
    }
    break;

  case 0x5000:
    increment(c);
    if (c->v[x] == c->v[y]) {
      increment(c);
    } 
    break;

  case 0x6000:
    c->v[x] = nn;
    increment(c);
    break;

  case 0x7000:
    c->v[x] += nn;
    increment(c);
    break;
   
  case 0x8000:
    switch (c->op & 0x000F) {
    case 0x0000:
      c->v[x] = c->v[y];
      increment(c);
      break;
    case 0x0001:
      c->v[x] |= c->v[y];
      increment(c);
      break;
    case 0x0002:
      c->v[x] &= c->v[y];
      increment(c);
      break;
    case 0x0003:
      c->v[x] ^= c->v[y];
      increment(c);
      break;
    case 0x0004:
      c->v[x] += c->v[y];
      increment(c);
      break;
    case 0x0005:
      c->v[x] -= c->v[y];
      increment(c);
      break;
    case 0x0006:
      c->v[x] = c->v[x] >> 1;
      c->v[f] = c->v[x] & 0x1;
      increment(c);
      break;
    case 0x0007:
      if (c->v[y] >= c->v[x]) {
        c->v[f] = 1;
      } else {
        c->v[f] = 0;
      }
      c->v[x] = c->v[x] - c->v[y];
      increment(c);
      break;
    case 0x000E:
      c->v[f] = c->v[x] >> 7;
      c->v[x] = c->v[x] << 1;
      increment(c);
      break;
    }

    case 0x9000:
      increment(c);
      if (c->v[x] != c->v[y]) {
        increment(c);
      }
      break;

    case 0xA000:
      c->ir = nnn;
      increment(c);
      break;

    case 0xB000:
      c->pc = c->v[0x0] + nnn;
      break;

    case 0xC000:
      srand(time(NULL));
      uint8_t ri = rand() % 256; 
      c->v[x] = ri & nn; 
      break;

    case 0xD000:
      c->v[0xF] = 0;
      int vy = c->v[y] % HEIGHT; 
      int vx = c->v[x] % WIDTH;
      for (int j = 0; j < n; j++) {
        byte p = c->mem[c->ir + j];
        for (int i = 0; i < 8; i++) {
          if ((p & (0x80 >> i)) != 0) {
            int py = (vy + j) % HEIGHT; 
            int px = (vx + i) % WIDTH;
            if (c->display[py][px] == 1) {
              c->v[0xF] = 1;
            }
            c->display[py][px] ^= 1;
          }
        }
      }
      c->canDraw = true; 
      increment(c);
      break;

    case 0xE000:
      switch (c->op & 0x00FF) {
      case 0x009E:
        increment(c);
        if (c->key[c->v[x]] == 1) {
          increment(c);  
        }
        break;
      case 0x00A1:
        increment(c);
        if (c->key[c->v[x]] == 0) {
          increment(c);
        }
        break;
      }
      break;

    case 0xF000:
      switch (c->op & 0x00FF) {
      case 0x0007:
        c->v[x] = c->dTimer;
        increment(c);
        break;
      case 0x000A:              //BUG
        bool ok = false;
        int l = sizeof(c->key);
        for (int i = 0; i < l; i++) {
          if (c->key[i] > 0) {
            c->v[x] = i;
            ok = true;
            printf("key pressed");
          }  
        } 
        increment(c);
        if (!ok) {
          c->pc -= 2;
        } 
        break;
      case 0x0015:
        c->dTimer = c->v[x];
        increment(c);
        break;
      case 0x0018:
        c->dTimer = c->v[x];
        increment(c);
        break;
      case 0x001E:
        c->ir += c->v[x];
        increment(c);
        break;
      case 0x0029:
        c->ir = c->v[x] * 0x5;
        increment(c);
        break;
      case 0x0033:
        c->mem[c->ir] = c->v[x] / 100;
        c->mem[c->ir + 1] = (c->v[x] / 10) % 10;
        c->mem[c->ir + 2] = (c->v[x] % 100) % 10;
        increment(c);
        break;
      case 0x0055:
        //int i;
        for (int i = 0; i <= x; i++) {
          c->mem[c->ir + i] = c->v[i];
        }
        c->ir = x + 1;
        increment(c);
        break;
      case 0x0065:
        //int i;
        for (int i = 0; i <= x; i++) {
          c->v[i] = c->mem[c->ir + i];
        }
        c->ir = x + 1;
        increment(c);
        break;
      }
      break;

    break;
  }

  if (c->dTimer > 0) {
    --c->dTimer;
  }
  if (c->sTimer > 0) {
    if (c->sTimer == 1) {
      printf("BEEP");
    }
    --c->dTimer;
  }

  return 0;
}

