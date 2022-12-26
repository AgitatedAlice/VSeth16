#ifndef SCRW
#define SCRW 640
#endif
#ifndef SCRH
#define SCRH 480
#endif
#ifndef AASCANC_H
#define AASCANC_H

#include "raylib.h"
#include "aautil.h"

typedef struct AACanv {
    Rectangle src;
    Rectangle dest;
    Color clr;
    Image img;
    Texture2D tex;
} AACanv;

// 40x25 character text mode handler structure, white on black only
typedef struct AATermMono {
    unsigned char ttx[40][25];
    AACanv *canv;
} AATermMono;


AACanv InitCanv(int x, int y, Color clrcol);
AATermMono InitMonoterm(AACanv *canv);
void UpdateCanv(AACanv *canv);
void ClearCanvas(AACanv *canv, Color c);
void WhiteNoiseTest(AACanv *canv);
void Draw8Font(AACanv *canv, int x, int y, unsigned char c);
void Draw8FontT(AACanv *canv, int x, int y, unsigned char c, Color C);
void Draw8FontC(AACanv *canv, int x, int y, unsigned char c, Color fg, Color bg);
void TxPrint(AACanv *canv, int x, int y, unsigned char *str);
void TxPrintC(AACanv *canv, int x, int y, unsigned char *str, Color fg, Color bg);
void DrawMonoTerm(AATermMono *term);
void MonoTermPrint(AATermMono *term, int _x, int _y, unsigned char *str);

AACanv InitCanv(int x, int y, Color clrcol){
    AACanv ret;
    ret.src = (Rectangle){0.0f, 0.0f, (float)x, (float)y}; ret.dest = (Rectangle){0.0f, 0.0f, (float)SCRW, (float)SCRH};
    ret.img = GenImageColor(x, y, clrcol);
    
    ImageFormat(&ret.img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ret.tex = LoadTextureFromImage(ret.img);
    
    return ret;
    
    /*return (AACanv){    .src =  {0.0f, 0.0f, (float)x, (float)y},
                        .dest = {0.0f, 0.0f, (float)SCRW, (float)SCRH},
                        .clr = clrcol,
                        .img = GenImageColor(x, y, clrcol),
                        .tex = LoadTextureFromImage(.img)};*/
}

AATermMono InitMonoterm(AACanv *canv){
    AATermMono ret;
    ret.canv = canv;
    for(int y = 0; y < 25; y++){
        for(int x = 0; x < 40; x++){
            ret.ttx[x][y] = 0;
        }
    }
    return ret;
}

void UpdateCanv(AACanv *canv){
    Color *pix = LoadImageColors(canv->img);
    UpdateTexture(canv->tex, pix);
    UnloadImageColors(pix);
}

void ClearCanvas(AACanv *canv, Color c){
    int mx = canv->img.width;
    int my = canv->img.height;
    
    for (int y = 0; y < my; y++){for (int x = 0; x < mx; x++)ImageDrawPixel(&canv->img, x, y, c);}
}

void WhiteNoiseTest(AACanv *canv){
    int mx = canv->img.width;
    int my = canv->img.height;
    
    for (int y = 0; y < my; y++){
        for (int x = 0; x < mx; x++){
            int n = GetRandomValue(0, 255); Color c = (Color){n, n, n, 255};
            ImageDrawPixel(&canv->img, x, y, c);
        }
    }
}



// 320/8 = 40, 200/8 = 25 40x25 text display



void Draw8Font(AACanv *canv, int x, int y, unsigned char c){
    int _x = x; int _y = y; int i;
    
    // check if within bounds, then if within bounds, loop through y fields of bytefont drawing 8 pix wide strips up-down
    int iw = canv->img.width; int ih = canv->img.height;
    if((_x + 8 < iw) && (_y + 8 < ih)){
        for(i = 0; i < 8; i++){
            for(int _j = 0; _j < 8; _j++){ //for(int _j = 7; _j >= 0; _j--){
                if(bitTest(byteFont[c-0x20][i], _j))
                    ImageDrawPixel(&canv->img, x+(8-_j), y+i, WHITE);
            }
        }
    }
    
}

void Draw8FontT(AACanv *canv, int x, int y, unsigned char c, Color C){
    int _x = x; int _y = y; int i;
    
    // check if within bounds, then if within bounds, loop through y fields of bytefont drawing 8 pix wide strips up-down
    int iw = canv->img.width; int ih = canv->img.height;
    if((_x + 8 < iw) && (_y + 8 < ih)){
        for(i = 0; i < 8; i++){
            for(int _j = 0; _j < 8; _j++){ //for(int _j = 7; _j >= 0; _j--){
                if(bitTest(byteFont[c-0x20][i], _j))
                    ImageDrawPixel(&canv->img, x+(8-_j), y+i, C);
            }
        }
    }
    
}

void Draw8FontC(AACanv *canv, int x, int y, unsigned char c, Color fg, Color bg){
    int _x = x; int _y = y; int i;
    
    // check if within bounds, then if within bounds, loop through y fields of bytefont drawing 8 pix wide strips up-down
    int iw = canv->img.width; int ih = canv->img.height;
    if((_x + 8 < iw) && (_y + 8 < ih)){
        for(i = 0; i < 8; i++){
            for(int _j = 0; _j < 8; _j++){ //for(int _j = 7; _j >= 0; _j--){
                (bitTest(byteFont[c-0x20][i], _j)) ? ImageDrawPixel(&canv->img, x+(8-_j), y+i, fg)
                : ImageDrawPixel(&canv->img, x+(8-_j), y+i, bg);
            }
        }
    }
    
}

void TxPrint(AACanv *canv, int x, int y, unsigned char *str){
    unsigned char p = 0; int _x = x; int _y = y;
    while(str[p] != '\0' && (_x >= 0)&&(_x <= canv->img.width) && (_y >= 0) && (_y <= canv->img.height)){
        Draw8Font(canv, _x, _y, str[p]);
        _x += 8;
        p++;
    }
}

void TxPrintC(AACanv *canv, int x, int y, unsigned char *str, Color fg, Color bg){
    unsigned char p = 0; int _x = x; int _y = y;
    while(str[p] != '\0' && (_x >= 0)&&(_x <= canv->img.width) && (_y >= 0) && (_y <= canv->img.height)){
        Draw8FontC(canv, _x, _y, str[p], fg, bg);
        _x += 8;
        p++;
    }
}

void DrawMonoTerm(AATermMono *term){
    int x = 0; int y = 0; int j, k;
    // x and y are screen coordinates, j and k are array xy index values
    for(k = 0; k < 25; k++){
        x = 0;
        for(j = 0; j < 40; j++){
            Draw8Font(term->canv, x, y, term->ttx[j][k]);
            x += 8;
        }
        y += 8;
    }
}

void MonoTermPrint(AATermMono *term, int _x, int _y, unsigned char *str){
    unsigned char p = 0; int x = _x; int y = _y;
    while(str[p]!='\0' && (x >= 0) && (x < 40) && (y >= 0) && (y < 25)){
        if(str[p]!='\n'){
            term->ttx[x][y] = str[p];
            p++; x++;
        } else {
            x = 0; y++; p++;
        }
        
        if(x >= 40){x = 0; y++;}
        if(y >= 25){x = 0; y = 0;}
    }
}

#endif