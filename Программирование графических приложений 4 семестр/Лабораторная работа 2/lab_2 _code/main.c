#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include "trancatedCube.h"

int rekey(XEvent* ev){
	Display* dpy = ev->xkey.display;					//дисплейный адрес
	Window win = ev->xkey.window;						//идентификатор окна
	
	int FillFace;										//флаг очистки/закраски граней
	KeySym ks;											//логический код клавиши
	ks = XKeycodeToKeysym(dpy,ev->xkey.keycode,0);		//контроль F10 для возврата кода
	if(ks == XK_F10) return 10;							//завершения программы 10
	FillFace=(ev->xkey.state & Mod1Mask) ? NFACE : 0;	//контроль Escape для очистки 
	if(ks == XK_Escape) reset(dpy,win,FillFace);		//или перерисовки граней 
	return 0;											//возврат кода 0 продолжения программы
}

int dispatch(Display* dpy, Window win, GC gc) {
	int NoFillFace = 0;									//флаг закраски граней
	XEvent event; 										//структура событий
	int done = 0; 										//exit-код
	while(done == 0) {									//цикл обработки событий
		XNextEvent(dpy, &event);						//получаем события
		switch(event.type) { 							//выбор обработки события
		case Expose: 									//перерисовка графа
			if(event.xexpose.count > 0) break;			
			regraph(dpy, win, gc, NoFillFace);	
		break;
		case ConfigureNotify:							//реконфигурация графа
			NoFillFace = reconf(event.xconfigure.width,
								event.xconfigure.height);
			regraph(dpy, win, gc, 0);
		break;
		case ButtonPress: 								//перекраска выбранной грани
			reface(dpy, win, gc,
				zotone(event.xbutton.x, event.xbutton.y));
			regraph(dpy, win, gc, 0);
		break;
		case FocusIn: 									//для KDE и GNOME
			NoFillFace = 0;
			regraph(dpy, win, gc, NoFillFace);
		break;
		case KeyPress:									// обработка клавиатуры
			done = rekey(&event); 
		default: break;
		} 
	}
	return(done);
}

int main(int argc, char* argv[]) {
	XPolyGraph heap;					//структура графа
	Display* dpy;						//адрес дисплейной структуры
	Window win; 						//идентификатор графического окна
	GC gc; 								//графический контекст изображения
	int title = (argc > 1) ? 1 : 0; 	//индекс заголовка окна
	dpy = XOpenDisplay(NULL);			//индекс заголовка окна 
	assoc(&heap); 						//контакт с X-сервером
	relink(&heap); 						//модельная ассоциация графа
	retrace();							//дисплейная адресация графа
	colorite(dpy); 						//распределение палитры 5 цветов
	gc = congraph(dpy); 				//настройка графического контекста
	win = wingraph(dpy, "123");			//создание именного окна
	dispatch(dpy, win, gc);				//диспетчеризация событий
	XDestroyWindow(dpy, win); 			//закрыть окно
	XCloseDisplay(dpy); 				//разрыв связи с X-сервером
	return(0); 							
}

