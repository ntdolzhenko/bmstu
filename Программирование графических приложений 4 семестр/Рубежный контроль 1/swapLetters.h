#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define CELLSIZE 100
#define FRAMESIZE 0

int WORDLENGTH;

typedef struct cell {
    Window window;
    char letter;
} Letter;

int swapLetters(int, int, int, int);
int shiftLeft(int, int, char);
int shiftRight(int, int, char);
int setLetters(char*);
int relink(Letter *);
int replaceLetter();
int xcustom();
int dispatch(char*);
int redraw(int);
int keyAnaliz(XEvent *, char*);
int main(int, char**);