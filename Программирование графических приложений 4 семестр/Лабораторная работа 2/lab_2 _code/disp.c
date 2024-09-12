#include "trancatedCube.h"

static XVertex *vertex;      				// адрес массива вершин 
static XEdge *edge;       					// адрес массиваребер 
static XFace *face;       					// адрес массива граней 
static unsigned long palette[(NTONE+1)]; 	// коды цветов 

// Адресация модельных массивов графа 
int relink(XPolyGraph *pg) {
	vertex = pg->vertex;  					// адрес массива вершин 
	edge = pg->edge;   						// адрес массива ребер 
	face = pg->face;   						// адрес массива граней 
	return 0;
}

// Распределение палитры цветов
int colorite(Display* dpy) {
	int scr;     								// номер экрана (по умолчанию) 
	Colormap cmap;    							// палитра (карта) цветов экрана 
	XColor rgb;     							// цветная структура 
	int i;      								// спектральный номер цвета 
	static char* spector[]={ 					// Cпектр кодов (имен) цветов 
		"#E6E6FA",  
		"#B34747",
		"#47B347",
		"#4747B3",
		"#14141A",
	}; // RGB-спецификация цветов 
	scr = DefaultScreen(dpy);      				// получить номер экрана (0) 
	cmap = DefaultColormap(dpy,scr);    		// экранная палитра 
	for(i=0; i<(NTONE+1); i++){      			// Спектральный цикл 
	XParseColor(dpy,cmap,spector[i],&rgb);  	// −> RGB 
	XAllocColor(dpy,cmap,&rgb);     			// −> pixel-код 
	palette[i]=rgb.pixel;      					// запомнить pixel-код цвета 
	}
 	return 0;
}

// Настройка графического контекста
GC congraph(Display* dpy) {
	int scr = DefaultScreen(dpy);       		// номер экрана
	XGCValues gcval;          					// параметры графконтекста
	GC gc;             							// идентификатор графконтекста        
	gcval.line_width = EWIDTH;        			// толщина контура графа
	gcval.background = palette[DEFTONE];     	// код фона
	gc = DefaultGC(dpy,scr);        			// Установка графконтекста
	XChangeGC(dpy,gc,GCLineWidth | GCBackground, &gcval);
	return(gc); 
}

// Создание и настройка параметров графического окна
Window wingraph(Display* dpy, char* title) {
	Window root;               					// идентификатор корневого окна экрана 
	int scr;                					// номер экрана по умолчанию
	int depth;                					// число цветовых плоскостей экрана
	Window win;                					// идентификатор окна программы           
	XSetWindowAttributes attr;  				// структура атрибутов окна 
	XSizeHints hint;            				// геометрия окнного менеджмента 
	int x,y;                					// координаты окна 
	unsigned w,h;               				// габариты окна 
	unsigned long mask;         				// маска атрибутов окна  
	mask = CWOverrideRedirect | CWBackPixel | CWEventMask;
	attr.override_redirect = False;          	// WM-контроль окна
	attr.background_pixel = palette[DEFTONE];   // цвет фона
	attr.event_mask = (ButtonPressMask | KeyPressMask |
	ExposureMask | StructureNotifyMask | FocusChangeMask);  // Маска событий
	root = DefaultRootWindow(dpy);           	// корневое окно
	scr = DefaultScreen(dpy);            		// номер экрана
	depth = DefaultDepth(dpy,scr);           	// глубина экрана
	w = DisplayWidth(dpy,scr)/2;           		// Расположить окно
	h = DisplayHeight(dpy,scr)/2;           	// площадью 1/4 экрана
	x = w/2; y = h/2;              				// в центре экрана
	win = XCreateWindow(dpy,root,x,y,w,h,1,depth,
	InputOutput,CopyFromParent,mask,&attr);
	hint.flags = (PMinSize | PPosition | PMaxSize);    // Задать поля для геометрического свойства WM
	hint.min_width = hint.min_height = (8*VDOT);
	hint.x = x; hint.y = y;
	XSetNormalHints(dpy,win,&hint);           	// −> свойство WM
	XStoreName(dpy,win,title);            		// Задать заголовок окна
	XMapWindow(dpy,win);             			// Отобразить окно на экране
 	return win;                					// возврат идентификатока окна в main
}

// Перерисовка контура и перекраска граней графа
int regraph(Display* dpy, Window win, GC gc, int NoFillFace) {
	int i; // счетчик вершин и граней

	// Раскраска всех или 0 внутренних граней
	for(i = NoFillFace; i < NFACE; i++){
		XSetForeground(dpy, gc, palette[face[i].tone]); // цвет грани
		XFillPolygon(dpy, win, gc, face[i].listVertex, face[i].countVertex, Convex, CoordModeOrigin);
	} 

	// Перерисовка всех ребер и вершин
	XSetForeground(dpy, gc, palette[NTONE]); // −> Black
	XDrawSegments(dpy, win, gc, edge, NEDGE);
	for(i = 0;i < NVERT; i++)
	XFillArc(dpy, win, gc, vertex[i].x - (VDOT >> 1),
		vertex[i].y - (VDOT >> 1), VDOT, VDOT, 0, 64*360);
	return 0;
}

// Перекраска грани
int reface(Display*dpy,Window win, GC gc, int f) {
	int i;    									// счетчик вершин грани
	if(f == NFACE)  							// перекраска внешней грани
	return(reset(dpy, win, f));      
	XSetForeground(dpy, gc, palette[face[f].tone]);  
	XFillPolygon(dpy, win, gc, face[f].listVertex, face[f].countVertex,
		Convex,CoordModeOrigin);       			// Перекраска
	XFlush(dpy);              					// внутренней грани

	// Перерисовка контура грани
	XSetForeground(dpy, gc, palette[NTONE]); 	// −> Black*/
	XDrawLines(dpy, win, gc, face[f].listVertex, face[f].countVertex+1,CoordModeOrigin); // перерисовка ребер

	for(i = 0; i < face[f].countVertex; i++) 	// перерисовка вершин
	XFillArc(dpy, win, gc, face[f].listVertex[i].x-(VDOT/2),
			face[f].listVertex[i].y-(VDOT/2), VDOT, VDOT, 0, 64*360);
	return 0;
	}

	// Перезагрузка раскраски граней

	int reset(Display* dpy, Window win, int FillFace) {      
	int f = FillFace;          // индекс грани 

	// Сохранить или Установить цвета внутренних граней 
	for(; f < NFACE; f++){         					// по фону 
		face[f].tone = face[NFACE].tone;     		// внешней грани 
	}
	// Установить фон окна и инициировать Expose 
	XSetWindowBackground(dpy,win,palette[face[f].tone]);
	XClearArea(dpy, win, 0, 0, 0, 0, True);      		
	return f;
}