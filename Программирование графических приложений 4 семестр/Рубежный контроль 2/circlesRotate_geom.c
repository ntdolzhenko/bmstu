#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <stdlib.h>
#include <math.h>
#include "circlesRotate.h"

#define min(a,b) ( ((a)>(b))?(b):(a) )

/* start structure initialization */
int configureCircle(Circle* circle, int w, int h) {
    circle->maxRadius = min(w/2, h/2);                      /* set radius */
    circle->radiusDelta = 1;                                /* set radius delta */

    circle->center.x = w / 2;                               /* set center */
    circle->center.y = h / 2;                               /* coordinates */

    circle->isRotate = (HSPEED | VSPEED);                   /* in the begining the both circles are rotating */

    circle->radiusWidthIncrement = -circle->maxRadius;      /* set start radius for both circles */
    circle->radiusLengthIncrement = -circle->maxRadius;
    return 0;
}

/*increment/decrement radius of pulse circle */
int stepRadius(Circle* circle) {
    if (circle->isRotate & HSPEED) {      /* now horizontal circle is rotating */
        circle->radiusWidthIncrement += circle->radiusDelta;
        if (circle->radiusWidthIncrement == circle->maxRadius) {
            circle->radiusWidthIncrement = -circle->maxRadius;
        }
    }
    if (circle->isRotate & VSPEED){      /* now vertical circle is rotating */
        circle->radiusLengthIncrement += circle->radiusDelta;
        if (circle->radiusLengthIncrement == circle->maxRadius) {
            circle->radiusLengthIncrement = -circle->maxRadius;
        }
    }
    return 0;
}

/* draw rotation step */
int drawRotatingStep(Display *dpy, Window win, GC gc, GC gc_t, Circle *circle){
    XClearWindow(dpy, win);

    XFillArc(dpy, win, gc_t, circle->center.x - CENTR_RAD, circle->center.y - CENTR_RAD, 
                CENTR_RAD * 2, CENTR_RAD * 2, 0, 360 * 64);                                 /* draw center point */

    XDrawLine(dpy, win, gc_t,  circle->center.x - circle->maxRadius, circle->center.y, 
                circle->center.x + circle->maxRadius, circle->center.y);                    /* horizontal axis */

    XDrawLine(dpy, win, gc_t, circle->center.x, circle->center.y - circle->maxRadius, 
                circle->center.x, circle->center.y + circle->maxRadius);                    /* vertical axis */        

    XDrawArc(dpy, win, gc, 
                circle->center.x - abs(circle->radiusWidthIncrement), circle->center.y - circle->maxRadius, 
                abs(circle->radiusWidthIncrement) * 2, circle->maxRadius * 2, 
                0, 360 * 64);                                                               /* draw vertical cirle */

    XDrawArc(dpy, win, gc, 
                circle->center.x - circle->maxRadius, circle->center.y - abs(circle->radiusLengthIncrement), 
                circle->maxRadius * 2, abs(circle->radiusLengthIncrement) * 2, 
                0, 360 * 64);                                                               /* draw horizontal circle */

    return stepRadius(circle);
}

/* redraw circles */
int redraw(XEvent* event, GC gc, GC gc_t, Circle* circle) {

    static XRectangle clip[32];     /* buffer clip */

    static int n = 0;               /* counter clip */

    clip[n].x = event->xexpose.x;
    clip[n].y = event->xexpose.y;
    clip[n].width =  event->xexpose.width;
    clip[n].height =  event->xexpose.height;
    n++;
    if((event->xexpose.count > 0) && (n < 32)) return(0);
    XSetClipRectangles(event->xexpose.display, gc, 0, 0, clip, n, Unsorted);

    drawRotatingStep(event->xexpose.display, event->xexpose.window, gc, gc_t, circle);

    XSetClipMask(event->xexpose.display, gc, None);
    return(n=0);
}


/* start stop rotating */
int startStopRotation(Circle* circle, XEvent *event) {

    /* press on center point */
    if (sqrt(pow(circle->center.x - event->xbutton.x, 2) + pow(circle->center.y - event->xbutton.y, 2)) <= CENTR_RAD){ 
        /* if one of the circles is rotating */
        if ((circle->isRotate != (HSPEED | VSPEED)) && (circle->isRotate != 0)) return 0;
        circle->isRotate ^= VSPEED;
        circle->isRotate ^= HSPEED;
        return 0;
    }

    /* press on horizontal axis */
    if ((event->xbutton.x <= circle->center.x + 4) && 
        (event->xbutton.x >= circle->center.x - 4) && 
        (event->xbutton.y >= circle->center.y - circle->maxRadius - 4) && 
        (event->xbutton.y <= circle->center.y + circle->maxRadius + 4)) circle->isRotate ^= HSPEED; 

    /* press on vertical axis */
    if ((event->xbutton.x <= circle->center.x + circle->maxRadius + 4) && 
        (event->xbutton.x >= circle->center.x - circle->maxRadius - 4) && 
        (event->xbutton.y >= circle->center.y - 4) && 
        (event->xbutton.y <= circle->center.y + 4)) circle->isRotate ^= VSPEED;
        
    return 0;
} 

/* exit by END */
int rekey(XEvent* ev) {
    KeySym ks = XLookupKeysym((XKeyEvent*) ev, 1);
    if(ks == XK_End) return 0;
    return 1;
} 
