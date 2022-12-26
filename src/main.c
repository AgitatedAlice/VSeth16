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

float deltaT;


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
	int cyclesRayTick = 166666;
	
	
	
	// Init VM
    ASTM16 seth16 = ASTM_Init(0x10, 0);
	// Init first 16 pages of memory as RAM
	for(uint8_t __i = 0; __i < 0x10; __i++){
		seth16.MEM[__i] = InitAPage('m');
	}
	seth16.MEM[0x10] = InitAPage('r'); // Reserve page 0x10 for boot ROM, not gonna reserve more, if your bootloader doesnt fit 128kb/64kWRD it is too bloat for my system!!!
	/* Load BOOTROM at 0x10 before doing any more random scheisse also remember to move interrupt vectors elsewhere, preferably in removable flashpage*/
	// devices go beyond 0x10 single page removable rw flash at some point, larger "flopy wannabe" further up (beyond audiovisual devices)
	// ... tbd
	
    AACanv testcan = InitCanv(320, 200, BLACK); //ImageDrawPixel()
    AATermMono termtst = InitMonoterm(&testcan);
    
    Color hxcol = GetColor(0xAA00);
    
    while(!WindowShouldClose()){
        // Read user key input, first VM operation specific, lets put VM pause/exec on F12 tickrate updown on F10-F11 respectively
		
		// Pass alphanumeric character keys to the VM, signal interrupts whenever reasonable INSIDE VM tick loop
		
		// Eventual: File I/O, register files dropped as images, depending on file type .rom .sdrv either put in (empty) rom slot at 0x10 or in drive slot, interrupt if it is removable drive.
		
		// UPDATE 
		
        //NEVER DO THIS: testcan.img = GenImageWhiteNoise(testcan.img.width, testcan.img.height, 0.5f); It causes memory leaks and crashes the program!
        ClearCanvas(&testcan, BLACK);
        
        //TxPrintC(&testcan, 10, 10, "COLOUR ME IMPRESSED!!! COLORVISION!!!!!!", BLUE, RED);
        UpdateCanv(&testcan);
        
        BeginDrawing();
            DrawTexturePro(testcan.tex, testcan.src, testcan.dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        EndDrawing();
        
        deltaT = GetFrameTime();
    }
    
    CloseWindow();
    
    return 0;
}