/* Sort Rectangle */

#include <X11/Xresource.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>


int checkOverlap(XRectangle rect1, XRectangle rect2){
	if (rect1.x < rect2.x + rect2.width &&
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.y + rect1.height > rect2.y){
		return 1; // Rectangles overlap
	}

	else return 0; // Rectangles do not overlap
}

int max(int a, int b) {return (a > b) ? a : b;}

int min(int a, int b) {return (a < b) ? a : b;}

int compareRectangles(const void *a, const void *b){
	XRectangle rect1 = *(XRectangle *)a;
	XRectangle rect2 = *(XRectangle *)b;
	double metricA = min(rect1.width, rect1.height) / max(rect1.width, rect1.height);
	double metricB = min(rect2.width, rect2.height) / max(rect2.width, rect2.height);

	return ( metricB - metricA );
}

int rekey(XEvent* ev) {
    KeySym ks = XLookupKeysym((XKeyEvent*) ev, 1);
    if((ks == XK_A) && (ev->xkey.state == ControlMask)) return('A'); // CTRL-A exit-code                     
 	return 0;
}

void redraw(Display *dpy, Window win, GC gc, XRectangle rectangleList[], XColor cl[], int amountRectangles){
	
	XClearWindow(dpy, win);                                                      
	for (int i = 0; i < amountRectangles; i++){                                                                            
		XDrawRectangle(dpy, win, gc, rectangleList[i].x, rectangleList[i].y, rectangleList[i].width, rectangleList[i].height);                         
		XSetForeground(dpy, gc, cl[1].pixel);                                    
		
		if (i == 0) XSetForeground(dpy, gc, cl[2].pixel);                                
		
		XFillRectangle(dpy, win, gc, rectangleList[i].x + 1, rectangleList[i].y + 1, rectangleList[i].width - 1, rectangleList[i].height - 1); 
		XSetForeground(dpy, gc, cl[0].pixel);                                    
	}
}

int dispatch(Display* dpy, Window win, GC gc, XColor cl[4], unsigned long mask) {
	
	int flag = 0;		   			// exit flag
	int flagRMB;					// RMB flag
	int	tracking = 0;		   		// check if lbm is pressed
	int	x1, y1;				   		// coordinates of the basic point 
	int	x2, y2;			   			// coordinates of the point
	int	xm, ym;			   			// cursor coordinates 
	int	amountRectangles = 0;  		// amount of rectangles in window

	XRectangle 	rect;	 			// rectangle object
	XRectangle* rectangleList;		// array of rectangles

	XEvent event;			   		

	while (flag == 0){
		XNextEvent(dpy, &event);
		switch (event.type){
    		case Expose: // redraw while initialize
    			if ((event.xexpose.window == win) && (amountRectangles > 0)) XSetFunction(dpy, gc, GXcopy);
    			XClearWindow(dpy, win);
    			redraw(dpy, win, gc, rectangleList, cl, amountRectangles);
    			break;
    		
    		case ButtonPress: // press lmb
    			if (event.xbutton.button == Button1){
    				tracking = 1;
    				
    				x1 = event.xbutton.x;
    				y1 = event.xbutton.y;

    				XGrabPointer(dpy, win, False, mask, GrabModeAsync, GrabModeSync, win, None, CurrentTime);
    			}
    			break;

    		case ButtonRelease: // release lmb
    			x2 = event.xbutton.x;
    			y2 = event.xbutton.y;
    			
    			XUngrabPointer(dpy, CurrentTime);
    			
    			if (x1 != x2 && y1 != y2){

    				switch (event.xbutton.button){
        				case Button1: // lmb

        					// refine new rectangle object
    						x2 = event.xbutton.x;
        					y2 = event.xbutton.y;

        					if (x2 < x1){
        						rect.x = x2;
        						rect.width = x1 - x2;
    						}
    						else{
        						rect.x = x1;
        	    				rect.width = x2 - x1;
        		    		}
    						if (y2 < y1){
        						rect.y = y2;
        						rect.height = y1 - y2;
        					}
        					else{
    							rect.y = y1;
    							rect.height = y2 - y1;
        					}
        					
        					if (amountRectangles == 0) rectangleList = calloc((1), sizeof(XRectangle));
        					else rectangleList = realloc(rectangleList, (amountRectangles + 1) * sizeof(XRectangle));
        					
    						rectangleList[amountRectangles++] = rect;
							qsort(rectangleList, amountRectangles, sizeof(XRectangle), compareRectangles);
							redraw(dpy, win, gc, rectangleList, cl, amountRectangles);

    						int der = 0;
        					for (int i = 0; i < amountRectangles; i++){
        						for (int j = 0; j < amountRectangles; j++){
        							if (i != j && checkOverlap(rectangleList[i], rectangleList[j])) der++;
    							}
    						}
        					if (der) amountRectangles--;
							qsort(rectangleList, amountRectangles, sizeof(XRectangle), compareRectangles);
        					redraw(dpy, win, gc, rectangleList, cl, amountRectangles);
        					
        					tracking = 0;
        					break;
        				
        				case Button3: // rmb
        					
        					flagRMB = 0;
        					int temp  = amountRectangles;
							int delRectIndex;

        					// check if the cursor in rectangle
        					while((!flagRMB) && (temp-- > 0)){
        						if ((event.xbutton.x > rectangleList[temp].x) && (event.xbutton.x < (rectangleList[temp].x + rectangleList[temp].width)) 
        						    && (event.xbutton.y > rectangleList[temp].y) && (event.xbutton.y < (rectangleList[temp].y + rectangleList[temp].height))){
        							flagRMB = 1;
        							delRectIndex = temp; // remember the rectangle
        						}
        					}
        					if(flagRMB){ // delere rectangle
        						for (int i = delRectIndex; i < amountRectangles; i++) rectangleList[i] = rectangleList[i + 1]; // resort
        						amountRectangles--;
        						flagRMB = 0;
        					}
        
        					XSetFunction(dpy, gc, GXcopy);
        					XClearWindow(dpy, win);
							
							qsort(rectangleList, amountRectangles, sizeof(XRectangle), compareRectangles);
        					
							redraw(dpy, win, gc, rectangleList, cl, amountRectangles);
        					break;
        				} // close butttom3
    			}
    			break; // close butttom release
    			
    		case MotionNotify:
    			if (tracking){
    			    
    				XSetFunction(dpy, gc, GXcopy);
    				XClearWindow(dpy, win);
    				redraw(dpy, win, gc, rectangleList, cl, amountRectangles);
    				
    				xm = event.xmotion.x;
    				ym = event.xmotion.y;
    				
    				if (xm < x1){
    					rect.x = xm;
    					rect.width = x1 - xm;
    				}
    				else{
    					rect.x = x1;
    					rect.width = xm - x1;
    				}
    				if (ym < y1){
    					rect.y = ym;
    					rect.height = y1 - ym;
    				}
    				else{
    					rect.y = y1;
    					rect.height = ym - y1;
    				}
    				
					XSetLineAttributes(dpy, gc, 1, LineOnOffDash, CapNotLast, JoinMiter);
					XSetForeground(dpy, gc, cl[3].pixel); 

					// draw the rectangle
					XDrawRectangle(dpy, win, gc, rect.x, rect.y, rect.width, rect.height);
								
					XSetForeground(dpy, gc, cl[0].pixel); // обводка первого прямоугольника
					XSetLineAttributes(dpy, gc, 1, LineSolid, CapNotLast, JoinMiter);
    			}
    			break;
    			
    		case KeyPress: // exit by CTRL+A
    			flag = rekey(&event);
    			break;
    		default: break;
		}
	}
	return(flag);
}
int main(int argc, char *argv[]){

	Display *dpy;			   		// graphic display
	int src;				   		// screen number
	int depth;				   		// screen depth
	Colormap cmap;			   		// color palate
	GC gc;					   		// graphic context
	XSetWindowAttributes attr; 		// window attributes
	unsigned long mask;		   		// event mask
	XColor exact;			   		// returns the nearest RGB codes on PK 
	XColor cl[4];			   		// returrns exact color codes
	Window root, win;		   		// main & root windows id

	// initialization
	dpy = XOpenDisplay(NULL);
	src = DefaultScreen(dpy);
	depth = DefaultDepth(dpy, src);
	cmap = DefaultColormap(dpy, src);
	root = DefaultRootWindow(dpy);

	// create graph context
	gc = DefaultGC(dpy, src);
	XAllocNamedColor(dpy, cmap, "#000000", &cl[0], &exact); 	// line rectangles color
	XAllocNamedColor(dpy, cmap, "#61E1FA", &cl[1], &exact); 	// rectangles color
	XAllocNamedColor(dpy, cmap, "#FE0B16", &cl[2], &exact);		// intersect point color
	XAllocNamedColor(dpy, cmap, "#FFFFFF", &cl[3], &exact); 	// dashed line color

	mask = CWOverrideRedirect | CWEventMask | CWBackPixel;
	attr.override_redirect = False;
	attr.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask | ButtonMotionMask | KeyPressMask;
	attr.background_pixel = cl[3].pixel;
	XAllocNamedColor(dpy, cmap, "#000000", &cl[3], &exact); 

	win = XCreateWindow(dpy, root, 0, 0, 640, 480, 1, depth, InputOutput, CopyFromParent, mask, &attr);
	XMapWindow(dpy, win);
	mask = ButtonReleaseMask | ButtonMotionMask;

	dispatch(dpy, win, gc, cl, mask);

	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 0;
}
