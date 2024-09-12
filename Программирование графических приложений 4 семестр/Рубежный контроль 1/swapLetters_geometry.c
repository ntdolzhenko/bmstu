#include "swapLetters.h"
#include <stdlib.h>

static Letter *box;

/* link to main module */
int relink(Letter *boxing) {
    box = boxing;
    return 0;
}

/* set the letters to the windows */
int setLetters(char *INPUT_WORD) {
    box[0].letter = 0;
    for (int i = 1; i <= WORDLENGTH; i++) box[i].letter = INPUT_WORD[i-1];
    box[WORDLENGTH+1].letter = 0;
    return 0;
}

/* shifting letters to the left  */
int shiftLeft(int start, int end, char letter) {
    for (int i = start; i < end; i++) box[i].letter = box[i + 1].letter;  /* shift chars */
    if (box[end].letter == 0) box[end - 1].letter = letter;  /* if it is the last letter window (right empty), place letter to the previous letter window */
    else box[end].letter = letter;
    return 0;
}

/* shifting letters to the right */
int shiftRight(int start, int end, char letter) {
    for (int i = start; i > end; i--) box[i].letter = box[i - 1].letter;  /* shift chars */
    //if (box[end].letter == 0) box[end + 1].letter = letter;  /* if it is the first letter window (left empty), place letter to the next letter window */
    /*else*/ box[end].letter = letter;
    return 0;
}

/* replacing the letters */
int swapLetters(int start_x, int end_x, int end_y, int xCell){
    if (box[end_y].letter > 0) return 0;
    else{
        if((end_x > (start_x + 1)) || (start_x > (end_x + 1))) {    

            for(int i = 0; i <= WORDLENGTH + 1; i++) {
                if((xCell - CELLSIZE/2) == (i * CELLSIZE)) return 0;
                else{
                    if(abs(xCell - i*CELLSIZE) < CELLSIZE/2) {
                        if (end_x > start_x) shiftLeft(start_x, end_x-1, box[start_x].letter);
                        else {
                            end_x = i;
                            shiftRight(start_x, end_x, box[start_x].letter);
                        }
                        break;
                    }
                }
            }
        }
        return 0;
    }
    return 0;
}
