#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define HSPEED (1)          /* gorizontal rotating */
#define CENTR_RAD 10        /* radius of central spot */
#define VSPEED (2)          /* vertical rotating */
#define DELAY (1 << 15)     /* delay for drawing step */

typedef struct {
    int isRotate;               /* is rotating on*/
    int radiusDelta;            /* radius increment */
    int radiusWidthIncrement;   /* radius width increment */
    int radiusLengthIncrement;  /* radius length increment */
    int maxRadius;              /* max radius for that window */
    XPoint center;              /* center point */ 
} Circle;

int configureCircle(Circle*, int, int );                 /* start structure initialization */
int redraw(XEvent*, GC, GC, Circle*);                    /* redraw window */
int startStopRotation(Circle*, XEvent *);                /* mouse events (stop/continuing rotation) */
int stepRadius(Circle* );                                /* one step of rotating (if the rotate flag is on) */
int drawRotatingStep(Display*, Window, GC, GC, Circle*); /* draw one step of rotating */
int rekey(XEvent *);                                     /* exit button event */