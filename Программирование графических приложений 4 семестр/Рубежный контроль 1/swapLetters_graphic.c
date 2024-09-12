#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "swapLetters.h"

static Display *dpy;                    /* display adress */
static Window desk;                     /* root window */
static GC gc;                           /* graphic context*/

static Letter *box;                     /* massiv of letter windows */
static char* WORD;

static unsigned long colors[2];

/* configure windows */
int xcustom() {
    int x, y;                           /*  Позиции  окон */
    unsigned w, h;                      /*  Габариты  окон */
    int depth = DefaultDepth(dpy, 0);   /*  Глубина  экрана  0  */
    Window root;                        /*  Корневое  окно  экрана  */
    XSetWindowAttributes attr;          /*  Атрибуты  окон */
    unsigned long amask;                /*  Маска  оконных  атрибутов */
    XSizeHints hint;                    /*  Геометрия  оконного  менеджмента */
    int i;                              /*  индексы  окон */
    Colormap colormap;

    root = DefaultRootWindow(dpy);      /* root window */

    //////// set font for the letters ////////
    XFontStruct* fn = XLoadQueryFont(dpy, "9x15");
    gc = XCreateGC(dpy, root, 0, NULL); /* create a graphic context for root window */

    XSetFont(dpy, gc, fn->fid);

    ///////// configure colors //////////
    XColor RGBpixel;                                        /* color structure */
    colormap = XDefaultColormap(dpy, DefaultScreen(dpy));   /* set a default color palete */
    XParseColor(dpy, colormap, "#ffffff", &RGBpixel);       /* get rgb by the color name (white) */
    XAllocColor(dpy, colormap, &RGBpixel);                  /* get a new place in the colormap for color */
    colors[0] = RGBpixel.pixel;                             /* set a white color in colormap */
    
    XParseColor(dpy, colormap, "#000000", &RGBpixel);       /* get rgb by the color name (white) */
    XAllocColor(dpy, colormap, &RGBpixel);                  /* get a new place in the colormap for color */
    colors[1] = RGBpixel.pixel;                             /* set a white color in colormap */

    ////// configure root window ///////
    attr.override_redirect = False;                         /* default */
    attr.background_pixel = colors[0];                      /* set background color as white */
    amask = (CWOverrideRedirect | CWBackPixel);

    w = (WORDLENGTH + 2) * CELLSIZE + FRAMESIZE * 2;     /* root winfdow lenght (letter windows + frame) */
    h = CELLSIZE + FRAMESIZE * 2 ;                          /* root winfdow width (letter cell size + frame)*/

    x = 0; y = 0;                                           /* default coordinates of root window */

    desk = XCreateWindow(dpy, root, x, y, w, h, 1, 
                        depth, InputOutput, CopyFromParent, amask, &attr); /* create root window */

    hint.min_width = hint.max_width = w;                    /* fix */
    hint.min_height = hint.max_height = h;                  /* size */
    hint.x = x; hint.y = y;                                 /* and default cordinates of root window */
    XSetNormalHints(dpy, desk, &hint);                      /* in WM */
   
    XStoreName(dpy, desk, "letter swap");                   /* window name */

    /////// configure letter windows ///////

    amask = CWOverrideRedirect | CWBackPixel | CWEventMask | CWBackingStore;
    attr.override_redirect = True;                                              /* turn off WM */
    attr.background_pixel = colors[0];                                          /* set background color as white */
    attr.backing_store = Always;                                                /* server saves data in window even it is not visible */
    attr.event_mask = (KeyPressMask | ExposureMask | ButtonPressMask);          /* event mask */
    w = CELLSIZE; h = CELLSIZE;                                                 /* letter window size */

    /////// create letter windows ///////
    box = (Letter *) calloc(WORDLENGTH + 2, sizeof(Letter));                      
    
    x = FRAMESIZE; y = FRAMESIZE;  /* coordinates of letter windows in root window */
    for(int i = 0; i < WORDLENGTH + 2; i++){ 
        box[i].window = XCreateWindow(dpy, desk, x, y, w, h, 1, depth,
                                    InputOutput, CopyFromParent, amask, &attr);
        box[i].letter = 0;
        x += CELLSIZE;
    }

    XMapWindow(dpy, desk);
    XMapSubwindows(dpy, desk);

    return (0);
}

/* redraw letters */
int redraw(int eraseIndex){
    for (int i = 1; i < WORDLENGTH + 2; i++){
        XSetForeground(dpy, gc, colors[0]);                                 /* set the white color for all primitives is window */
        XFillRectangle(dpy, box[i].window, gc, 0, 0, CELLSIZE, CELLSIZE);   /* draw new rectangle */

        /* eraseIndex - index of the letter window, that is moving, so erase the letter inside */
        if(eraseIndex != -1 && i == eraseIndex) continue;

        if (box[i].letter != 0) {                                           /* if it is not empty window */
            XSetForeground(dpy, gc, colors[1]);                             /* set the black color for all primitives is window */
            XDrawString(dpy, box[i].window, gc, CELLSIZE/2, CELLSIZE/2, &box[i].letter, 1); /* draw letter */
        }
    }
    return 0;
}

int replaceLetter(){

    int cur_time = CurrentTime;

    Window root, cell;                  /* root and letter windows */
    int xRoot, yRoot, xCell, yCell;     /* coordinates of cursor in root and letters windows */
    unsigned mask_return;               /* current state of pointer buttons */

    /* get pointer coordonates */
    XQueryPointer(dpy, desk, &root, &cell, &xRoot, &yRoot, &xCell, &yCell, &mask_return);
    
    /* start coordinates of letter window */
    int start_x = xCell / CELLSIZE;
    int start_y = yCell / CELLSIZE;      
    
    if (box[start_x].letter == 0) return 1;     /* if where is no letter in window return 1 and exit */

    XGrabPointer(dpy, desk, True, 0, GrabModeAsync, GrabModeAsync, desk, None, cur_time);   /* grab the cursor */

    /* create temporary the new window to move */
    XSetWindowAttributes attr;
    unsigned long amask = CWOverrideRedirect | CWBackPixel | CWEventMask;
    attr.override_redirect = True;
    attr.background_pixel = colors[0]; 
    attr.event_mask = (KeyPressMask | ExposureMask | ButtonPressMask);

    Window letterWin = XCreateWindow(dpy, desk, xCell - CELLSIZE/2, yCell - CELLSIZE/2, CELLSIZE, CELLSIZE, 0, 
                                    DefaultDepth(dpy, DefaultScreen(dpy)), InputOutput, CopyFromParent, amask, &attr);
    XMapWindow(dpy, letterWin);
    XSetForeground(dpy, gc, colors[1]);
    XDrawRectangle(dpy, letterWin, gc, 0, 0, CELLSIZE, CELLSIZE);
    XDrawString(dpy, letterWin, gc, CELLSIZE/2, CELLSIZE/2, &box[start_x].letter, 1);


    /* move window while left mouse button is pressed and redraw letters */
    unsigned long prev_mask = mask_return;
    while(mask_return == prev_mask) {
        XQueryPointer(dpy, desk, &root, &cell, &xRoot, &yRoot, &xCell, &yCell, &mask_return);
        XMoveWindow(dpy, letterWin, xCell - CELLSIZE/2, FRAMESIZE);
        redraw(start_x);

        for (int i = 1; i < WORDLENGTH + 2; i++){
            //XSetForeground(dpy, gc, colors[0]);                                 /* set the white color for all primitives is window */
            //XFillRectangle(dpy, box[i].window, gc, 0, 0, CELLSIZE, CELLSIZE);   /* draw new rectangle */

            /* eraseIndex - index of the letter window, that is moving, so erase the letter inside */
            if(i == start_x) continue;

            if (box[i].letter != 0) {                                           /* if it is not empty window */
                XSetForeground(dpy, gc, colors[1]);                             /* set the black color for all primitives is window */
                XDrawString(dpy, box[i].window, gc, CELLSIZE/2, CELLSIZE/2, &box[i].letter, 1); /* draw letter */
            }
        }
    }
    /* destroy temorary window when then the button mask in changed ( when the left mouse button is realesed) */
    XDestroyWindow(dpy, letterWin);

    /* replace letter windows */
    swapLetters(start_x, xCell/CELLSIZE, yCell/CELLSIZE, xCell);

    XUngrabPointer(dpy, cur_time);      /* ungrab the cursor */
    return 0;
}

int keyAnaliz(XEvent *ev, char *INPUT_WORD) {
    if (ev->xkey.keycode == XKeysymToKeycode(dpy, XK_Escape)) {
        setLetters(INPUT_WORD);
        redraw(-1);
    }
    return 0;
}

/* event manager */
int dispatch(char *INPUT_WORD){ 

    XEvent event;                                               /* event structure */
    int done = 0;                                               /* exit flag */

    while (done == 0) {  
        XNextEvent(dpy, &event);                                /* get event */
        switch (event.type) {
            case Expose:
                redraw(-1);                          
                break;
            case ButtonPress:
                done = replaceLetter();
                redraw(-1);
                break; 
            case KeyPress:
                done = keyAnaliz(&event, INPUT_WORD);
                break; 
            default: break;
        }
    }
    return (0);
}

int main(int argc, char* argv[]) {

    if(argc != 2) fprintf(stderr, "Error. There is no word\n");

    char *INPUT_WORD = argv[1];
    WORDLENGTH = strlen(INPUT_WORD);

    WORD = (char*) calloc(WORDLENGTH, sizeof(char));

    dpy = XOpenDisplay(NULL);

    xcustom();
    relink(box);
    setLetters(INPUT_WORD);
    dispatch(INPUT_WORD);

    XDestroySubwindows(dpy, desk);
    XDestroyWindow(dpy, desk);

    XCloseDisplay(dpy);
    return (0);
}
