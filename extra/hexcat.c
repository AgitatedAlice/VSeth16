#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef union {
	uint16_t w;
	uint8_t b[sizeof(uint16_t)];
} AWORD;
// returns true on eof or NULL file pointer
bool getByte(uint8_t *byte, FILE *file){
	if(file == NULL)
		return true;
	int fakebyte = getc(file);
	if(fakebyte != EOF){
		*byte = (uint8_t)fakebyte;
		return false;
	}
	else {return true;}
}

bool getWord(uint16_t *word, FILE *file){
	if(file == NULL)
		return true;
	int fakebyte = getc(file); 
	if(fakebyte != EOF){
		AWORD _word;
		_word.b[1] = (uint8_t)fakebyte; // if first byte in stream isn't EOF, place it as upper byte of word (remember: ADDE EFBE)
		if((fakebyte=getc(file))!=EOF){
			_word.b[0] = (uint8_t)fakebyte; // if second byte isn't eof either, add it as lower byte
		} else { return true;}
		*word = _word.w;
		return false;
	}
	else {return true;}
}

int main(int argc, char *argv[]){
	int rval = 0;
	
	printf("args: %i\n", argc);
	
	FILE *src; //FILE *output;
	if(argc < 2){
		rval = 666;
		printf("No arguments supplied to program, doing nothing and exiting with code 666\n");
	} else {
		printf("1st argument passed: %s\n", argv[1]);
		//printf("2nd argument passed: %s\n", argv[2]);
		
		src = fopen(argv[1], "rb"); int fileSize = feof(src);
		if(src == NULL){
			printf("File error.\n"); rval = 999;
		} else {
			//printf("File eof: %i\n", fileSize);
			//AWORD words[512]; // just a hard limit for now, should be about 1kb and thus stack friendly and safe to put here
			//int cbi = 0; // current byte integer
			//uint8_t cb = 0; // current byte
			uint16_t cword = 0; // current word
			uint8_t i = 0; //uint16_t fi = 0; // iterator for screen prettiness and iterator for number of bytes iterated
			while(!getWord(&cword, src)){//(cbi = getc(src))!=EOF){
				//cb = (uint8_t)cbi;
				printf("%#06hx ", cword);
				i++; if(i > 8) { i = 0; printf("\n");}
			}
		}
	}
	
	
	return rval;
}