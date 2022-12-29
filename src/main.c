// I am still a failure that cant complete a project lmao
#include <stdio.h>
#include "raylib.h"
#define WINW 640
#define WINH 480
#define WINF 2
#define SCRW 640*2
#define SCRH 480*2
#include "aascanc.h"
#include "cgafc.h"

//#include "vhcs.h"

float deltaT; // raylib specific, not VM related, its for keeping track of how long frames take, which is relevant for things like audio playback and smooth movement of things outside of VM, can also be used to throttle the VM if tickrate somehow exceeds target


/*

FORGET EVERYTHING HERE, CLEAN UP MAIN LOOP AND FRAME LOOP, THINK THINGS THROUGH

*/
int main(void){
    deltaT = 0.0f;
    InitWindow(WINW*WINF, WINH*WINF, "Raylib vCGAfc");
    SetTargetFPS(30);
    
    printf("\n---\n---\nColor struct size: %u\n %u\n---\n---\n", sizeof(Color), '!');
    
    //Font tfont = LoadFont("assets/IBM.ttf");
	
	// Set up VM operation vars
	int cyclesRayTick = 166666; // 5m/30 = this.. so it is clocked at about 5MHz while window refreshes at 30Hz, kinda hacky and linear way of doing things but I dont like fiddling with threads - AA
	bool bootromIsPresent = FileExists("boot.hex");
	
	
	
	// Init VM
    ASTM16 seth16 = ASTM_Init(0x10, 0x0000); // Begin at page 16 (boot rom) addres 0x0000
	// Init first 16 pages of memory as RAM
	for(uint8_t __i = 0; __i < 0x10; __i++){
		seth16.MEM[__i] = InitAPage('m');
	}
	seth16.MEM[0x10] = InitAPage('r'); // Reserve page 0x10 for boot ROM, not gonna reserve more, if your bootloader doesnt fit 128kb/64kWRD it is too bloat for my system!!!
	/* Load BOOTROM at 0x10 before doing any more random scheisse also remember to move interrupt vectors elsewhere, preferably in removable flashpage*/
	
	if(bootromIsPresent){
		unsigned int romsize = 0;
		unsigned char *romdata = LoadFileData("boot.hex", &romsize); // load boot.hex
		unsigned int romsizewords = romsize/2; // 2 bytes per 16 bit word :)
		// check if length is 0x10000, if valid size, load, otherwise report that it is too long and quit
		if(romdata != NULL){
			if(romsizewords <= 0x10000){
				unsigned int bIter = 0; AWORD fullword;
				for(uint16_t rdi = 0; rdi <= romsizewords; rdi++){
					fullword.b[0] = romdata[bIter]; fullword.b[1] = romdata[bIter+1]; bIter += 2;
					seth16.MEM[0x10].d[rdi] = fullword.w;
				}
				printf("Loaded boot.hex, %u bytes, %u words\nUnloading file.", romsize, romsizewords);
				UnloadFileData(romdata); // unload from application memory (not VM memory!) now that we got it in the ROM page
			} else { printf("Boot ROM too large!\n"); CloseWindow();}
		} else { printf("File is null..\n");}
	} else { printf("No boot.hex located..\n"); }
	
	// devices go beyond 0x10 single page removable rw flash at some point, larger "floppy wannabe" further up (beyond audiovisual devices) will write a doc file about the intended memory map
	// ... tbd
	
    AACanv testcan = InitCanv(320, 200, BLACK); //ImageDrawPixel()
    AATermMono termtst = InitMonoterm(&testcan);
    
    Color hxcol = GetColor(0xAA00);
    
    while(!WindowShouldClose()){
        // Read user key input, first VM operation specific, lets put VM pause/exec on F12 tickrate updown on F10-F11 respectively
		
		// Pass alphanumeric character keys to the VM, signal interrupts whenever reasonable INSIDE VM tick loop
		
		// Eventual: File I/O, register files dropped as removable non-volatile media, .hex dropped is flash (0x11) .sdrv is in drive volume (not yet implemented) interrupt system and place relevant interrupt info on page 0 upper half (< 0x7FFF)
		
		
		// Run VM ticks
		
        //NEVER DO THIS: testcan.img = GenImageWhiteNoise(testcan.img.width, testcan.img.height, 0.5f); It causes memory leaks and crashes the program!
        ClearCanvas(&testcan, BLACK);
        
        //TxPrintC(&testcan, 10, 10, "COLOUR ME IMPRESSED!!! COLORVISION!!!!!!", BLUE, RED);
		TxPrintC(&testcan, 10, 10, (bootromIsPresent) ? "BOOT ROM PRESENT" : "NO BOOT ROM PRESENT", BLUE, RED);
        UpdateCanv(&testcan);
        
        BeginDrawing();
            DrawTexturePro(testcan.tex, testcan.src, testcan.dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        EndDrawing();
        
        deltaT = GetFrameTime();
    }
    
    CloseWindow();
    
    return 0;
}