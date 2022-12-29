#ifndef VM_H
#define VM_H

#include <stdio.h>
#include "raylib.h"
#define WINW 640
#define WINH 480
#define WINF 2
#define SCRW 640*2
#define SCRH 480*2
#include "aascanc.h"
#include "cgafc.h"
#include "vm.h"

struct VM{
  float deltaT;
  int cyclesRayTick;
  bool bootromIsPresent;
  ASTM16 seth16;  
};



struct VM newVM(){
  // Init VM
  struct VM vm;
  vm.deltaT = 0.0f;
  vm.cyclesRayTick = 166666;
  bool fileExists = FileExists("boot.hex");
  vm.bootromIsPresent =  fileExists;
  
  vm.seth16 = ASTM_Init(0x10, 0);
  // Init first 16 pages of memory as RAM
  for(uint8_t __i = 0; __i < 0x10; __i++){
    vm.seth16.MEM[__i] = InitAPage('m');
  }
  vm.seth16.MEM[0x10] = InitAPage('r'); // Reserve page 0x10 for boot ROM, not gonna reserve more, if your bootloader doesnt fit 128kb/64kWRD it is too bloat for my system!!!
  /* Load BOOTROM at 0x10 before doing any more random scheisse also remember to move interrupt vectors elsewhere, preferably in removable flashpage*/
  // devices go beyond 0x10 single page removable rw flash at some point, larger "flopy wannabe" further up (beyond audiovisual devices)
  // ... tbd
  return vm;
}


#endif
