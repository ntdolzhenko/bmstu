/* Sort Rectangle */

#include <X11/Xresource.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
/* #define H1 80
#define W1 80 */

int doOverlap(XRectangle rect1, XRectangle rect2)
{
	if (rect1.x < rect2.x + rect2.width &&
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.y + rect1.height > rect2.y)
	{
		printf("DONT NORMAL");
		return 1; // Rectangles overlap
	}
	else
	{
		return 0; // Rectangles do not overlap
	}
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

int compareRectangles(const void *a, const void *b)
{
	XRectangle fracA = *(XRectangle *)a;
	XRectangle fracB = *(XRectangle *)b;
	int metricA = max(fracA.width, fracA.height) / min(fracA.width, fracA.height);
	int metricB = max(fracB.width, fracB.height) / min(fracB.width, fracB.height);

	if (metricA < metricB)
	{
		return 1;
	}
	else if (metricA > metricB)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int rekey(XEvent* ev) {
    KeySym ks = XLookupKeysym((XKeyEvent*) ev, 1);
    if((ks == XK_O) && (ev->xkey.state == ControlMask))
        return('S');                     /* CTRL-S to return exit-code */
 return 0;
} /* rekey */

#define redraw()                                                                 \
	XClearWindow(dpy, win);                                                      \
	for (a = 0; a < i; a++)                                                      \
	{                                                                            \
		XDrawRectangle(dpy, win, gc, rect1[a].x, rect1[a].y,                     \
					   rect1[a].width, rect1[a].height);                         \
		XSetForeground(dpy, gc, cl[1].pixel);                                    \
		if (a == 0)                                                              \
			XSetForeground(dpy, gc, cl[2].pixel);                                \
		XFillRectangle(dpy, win, gc, rect1[a].x + 1,                             \
					   rect1[a].y + 1, rect1[a].width - 1, rect1[a].height - 1); \
		XSetForeground(dpy, gc, cl[0].pixel);                                    \
	}

int main(int argc, char *argv[])
{
	int a,					   // счетчик перерисовки
		n1,					   // счетчик удаление
		flag = 0,			   // код выхода
		tracking = 0,		   // =1 если нажата лкм отчечает за пунктирный контур
		x1, y1,				   // координаты опорной точки
		x2, y2,				   // координаты точки
		xm, ym,				   //
		b, c, i = 0,		   // счетчики
		flag2;				   // индикатор нажатия пкм внутри прям-ка
	Display *dpy;			   // графический дисплей
	int src;				   // номер
	int depth;				   // глубина
	Colormap cmap;			   // палитра цветов
	GC gc;					   // граф контекст
	XSetWindowAttributes attr; // аттрибуты окна
	unsigned long mask;		   // маска событий
	XColor exact;			   // возвращает ближайшие значения RGB предоставленные оборудованием
	XColor cl[4];			   // возвращает точные коды цветов
	Window root, win;		   // main & root windows id
	XEvent event;			   // структура событие
	// XWindowAttributes atrwin;
	char Key[1];	 // хранение кода клавиши клавиши выхода (О)
	XRectangle rect, // объект прямоугольник
		*rect1;		 // массив указателей объектов прямоугольников// *rectsort;
	// XrmDatabase rdb;               	// ресурс управления базы данных
	// char* rtype;                   /* resourse type (String) */
	// XrmValue* rval;                /* resource value (size & addr) */
	// int narg = argc;               /* command argumentes number */

	// инициализация
	dpy = XOpenDisplay(NULL);
	src = DefaultScreen(dpy);
	depth = DefaultDepth(dpy, src);
	cmap = DefaultColormap(dpy, src);
	root = DefaultRootWindow(dpy);

	// создание граф контекста
	gc = DefaultGC(dpy, src);
	XAllocNamedColor(dpy, cmap, "#986960", &cl[0], &exact); // цвет обводки
	XAllocNamedColor(dpy, cmap, "#ADD8E6", &cl[1], &exact); // цвет заливки
	XAllocNamedColor(dpy, cmap, "red", &cl[2], &exact);		// цвет контура при пересечении
	XAllocNamedColor(dpy, cmap, "#CD9575", &cl[3], &exact); // цвет контура при отсутствии пересечения
	XAllocNamedColor(dpy, cmap, "#FAEBD7", &cl[4], &exact); // цвет фона

	XSetForeground(dpy, gc, cl[0].pixel); // установка цвета обводки

	mask = CWOverrideRedirect | CWEventMask | CWBackPixel;
	attr.override_redirect = False;
	attr.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask | ButtonMotionMask | KeyPressMask;
	attr.background_pixel = cl[4].pixel; // цвет фона

	win = XCreateWindow(dpy, root, 50, 50, 640, 480, 1, depth, InputOutput, CopyFromParent, mask, &attr);
	XMapWindow(dpy, win); // окно
	mask = ButtonReleaseMask | ButtonMotionMask;

	while (flag == 0)
	{
		XNextEvent(dpy, &event);
		switch (event.type)
		{
		case Expose: // запрос на перерисовку окна при создании
		{
			if ((event.xexpose.window == win) && (i > 0))
				XSetFunction(dpy, gc, GXcopy);
			XClearWindow(dpy, win);
			redraw();
		}
		case ButtonPress: // событие нажатие лкм
			if (event.xbutton.button == Button1)
			{
				tracking = 1;
				x1 = event.xbutton.x;
				y1 = event.xbutton.y;
				// xm = -1;
				// ym = -1;
				XGrabPointer(dpy, win, False, mask, GrabModeAsync, GrabModeSync, win, None, CurrentTime);
			}
			break;
		case ButtonRelease: // отжатие лкм
			x2 = event.xbutton.x;
			y2 = event.xbutton.y;
			XUngrabPointer(dpy, CurrentTime);
			if (x1 != x2 && y1 != y2)
				switch (event.xbutton.button)
				{
				case Button1: // лкм
					// if(tracking)
					// заполнение строк объекта
					{
						x2 = event.xbutton.x;
						y2 = event.xbutton.y;
						if (x2 < x1)
						{
							rect.x = x2;
							rect.width = x1 - x2;
						}
						else
						{
							rect.x = x1;
							rect.width = x2 - x1;
						}
						if (y2 < y1)
						{
							rect.y = y2;
							rect.height = y1 - y2;
						}
						else
						{
							rect.y = y1;
							rect.height = y2 - y1;
						}
						if (i == 0)
							rect1 = calloc((1), sizeof(XRectangle));
						else
							rect1 = realloc(rect1, (i + 1) * sizeof(XRectangle));
						rect1[i++] = rect;
						int der = 0;
						for (int a = 0; a < i; a++)
						{
							for (int b = 0; b < i; b++)
							{
								if (a != b && doOverlap(rect1[a], rect1[b]))
									der++;
							}
						}
						if (der)
						{
							i--;
						}
						qsort(rect1, i, sizeof(XRectangle), compareRectangles);
						redraw();
					}
					tracking = 0;
					break;
				case Button3: // пкм
					flag2 = 0;
					a = i;
					// проверка нахождения курсора в контуре прямогольника
					while ((!flag2) && (a-- > 0))
						if ((event.xbutton.x > rect1[a].x) &&
							(event.xbutton.x < (rect1[a].x + rect1[a].width)) && (event.xbutton.y > rect1[a].y) &&
							(event.xbutton.y < (rect1[a].y + rect1[a].height)))
						{
							flag2 = 1;
							c = a; // запомнили нужный прямоугольник
						}
					if (flag2) // удаление пр-ка
					{
						for (n1 = c; n1 < i; n1++)
							rect1[n1] = rect1[n1 + 1]; // пересортировка
						i--;
						flag2 = 0;
					}

					XSetFunction(dpy, gc, GXcopy);
					XClearWindow(dpy, win);
					redraw();
				} // перерисовка прямоугольника
			break;
			break;
		case MotionNotify:
			if (tracking)
			{
				XSetFunction(dpy, gc, GXcopy);
				XClearWindow(dpy, win);
				redraw();
				xm = event.xmotion.x;
				ym = event.xmotion.y;
				if (xm < x1)
				{
					rect.x = xm;
					rect.width = x1 - xm;
				}
				else
				{
					rect.x = x1;
					rect.width = xm - x1;
				}
				if (ym < y1)
				{
					rect.y = ym;
					rect.height = y1 - ym;
				}
				else
				{
					rect.y = y1;
					rect.height = ym - y1;
				}
				XSetLineAttributes(dpy, gc, 1, LineOnOffDash, CapNotLast, JoinMiter);
				XSetForeground(dpy, gc, cl[3].pixel); // контур без пересечения
				for (b = 0; b < i; b++)				  // проверка наложения прямоугольников
				{
					if (!(((rect.x < rect1[b].x) && (rect.x + rect.width < rect1[b].x)) ||
						  ((rect.x > rect1[b].x + rect1[b].width) && (rect.x + rect.width > rect1[b].x + rect1[b].width)) ||
						  ((rect.y < rect1[b].y) && (rect.y + rect.height < rect1[b].y)) ||
						  ((rect.y > rect1[b].y + rect1[b].height) && (rect.y + rect.height > rect1[b].y + rect1[b].height))))
					{
						XSetForeground(dpy, gc, cl[2].pixel); // контур при пересечении
						break;
					}
				}
				XDrawRectangle(dpy, win, gc, rect.x, rect.y, rect.width, rect.height);
				XSetForeground(dpy, gc, cl[0].pixel); // обводка первого прямоугольника
				XSetLineAttributes(dpy, gc, 1, LineSolid, CapNotLast, JoinMiter);
			}
			break;
		case KeyPress: // выход по ctrl o
			flag = rekey(&event);
			break;
			// KeySym c = XLookupKeysym((XKeyEvent *)&event, 1);
			// //printf("%d\n", c);
			// if (c == XK_O && event.xkey.state == ControlMask)
			// 	return('O');
			// break;


		default:
			break;
		}
	}
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	return 0;
}
