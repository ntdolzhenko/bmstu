#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include "circlesRotate.h"

#define WIDTH 640   /* main window width */
#define HEIGHT 480  /* main window height */

int main() {
    Display* dpy;              /* graphic display */
    Window win;                /* programm main window */
    GC gc;                     /* black &and white graphic contexts */
    GC gcDottedLine;           /* dotted line graphic context */
    int scr;                   /* screen number = 0 */
    Circle circles;            /* circles structure */

    /////* graphic contexts configuration *//////

    dpy = XOpenDisplay(NULL);       /* connection with XServer */
    scr = DefaultScreen(dpy);       /* screen number by default */
    win = DefaultRootWindow(dpy);   /* root screen window */

    gc = XCreateGC(dpy, win, 0, 0);
    gcDottedLine = XCreateGC(dpy, win, 0, 0);

    unsigned long tone;                         /* drawing tone */
    tone = 0xFFFFFF;                            /* white */
    XSetForeground(dpy, gc, tone);
    XSetForeground(dpy, gcDottedLine, tone);
    tone = 0x000000;                            /* black */
    XSetBackground(dpy, gc, tone);
    /* dotted line */
    XSetLineAttributes(dpy, gcDottedLine, 1, LineOnOffDash, CapNotLast, JoinMiter);

    /////* windows configuration *//////

    XSetWindowAttributes attr;  /* window attributes */
    XGCValues gval;             /* GC structure */
    unsigned long amask;        /* window attributes mask */
    Window root = win;          /* display root window */
    XSizeHints hint;            /* geometric attibutes WM */

    configureCircle(&circles, WIDTH, HEIGHT);

    amask = (CWOverrideRedirect | CWBackPixel);
    XGetGCValues(dpy, gc, GCBackground, &gval);
    attr.background_pixel = gval.background;    /* black background = 0 */
    attr.override_redirect = False;             /* WM control */    
    win = XCreateWindow(dpy, root, 0, 0, WIDTH, HEIGHT, 1, CopyFromParent,
                            InputOutput, CopyFromParent, amask, &attr);

    hint.flags = (PMinSize | PMaxSize);         /* fix */
    hint.min_width = hint.max_width = WIDTH;    /* size */
    hint.min_height = hint.max_height = HEIGHT; /* of the window */

    XSetNormalHints(dpy, win, &hint);           /* set geom attr of window in normal*/ 
        
    XStoreName(dpy, win, "rotating circles");   /* set window name */
    XMapWindow(dpy, win);                       /* show window */


    unsigned long mask;         /* event mask */
    XEvent event;               /* event structure */
    unsigned delay = DELAY;     /* drwaing delay */
    int done = 1;               /* exit flag */
    int count = 0;              /* time counter for drawing */

    mask = ( KeyPressMask | ExposureMask | ButtonPressMask);   
    XSelectInput(dpy, win, mask);   /* set event mask */

    while(done != 0) { 
        event.type = 0;             /* reset previous event type */
        XCheckWindowEvent(dpy, win, mask,  &event);

        switch (event.type) {
            case Expose:
                redraw(&event, gc, gcDottedLine, &circles);              
                break;
            case ButtonPress:
                startStopRotation(&circles, &event);
                break;
            case KeyPress:
                done = rekey(&event);
                break;
            default: break;
        }

        if(count++ < delay) continue;   /* increase and check counter for drawing */
        count = 0;                      /* reset counter */
            
        drawRotatingStep(dpy, win, gc, gcDottedLine, &circles);       /* draw new circles */
    } 
    

    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return(0);

} 