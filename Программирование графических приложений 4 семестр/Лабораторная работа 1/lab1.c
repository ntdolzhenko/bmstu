#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define WIDTH 216           // ширина повторяющегося элемента
#define HEIGHT 216          // высота повторяющегося элемента

int main(int argc, char* argv[]) {
  Display* dpy;                   // Адрес дисплейной структуры 
  int src;                        // Номер экрана дисплея 
  GC gc;                          // Графический контекст 
  int depth;                      // Число цветовых плоскостей экрана 
  Window root;                    // Корневое окно экрана 
  Pixmap pix;                     // Пиксельная карта элемента фона 
  Window win;                     // Основное окно программы 
  XSetWindowAttributes attr;      // Атрибуты окна программы
  unsigned long mask;             // Маска атрибутов окна
  XEvent event;                   // Структура оконных событий 
  unsigned int done = 0;          // Флаг завершения программы
  XSizeHints hint;                // Геометрические параметры окна

  // графическая инициализация

  dpy = XOpenDisplay(NULL);						 // адресуется структура дисплея 
  src = DefaultScreen(dpy);						 // устанавливается номер экрана
  depth = DefaultDepth(dpy, src);		   // число цв плоскостей 	
  root = DefaultRootWindow(dpy);			 // устанавливается корневое окно
  gc = DefaultGC(dpy, src);						 // графический контекст

  hint.flags = (PMinSize); 						 // Зададаем параметры поля
  hint.min_width = WIDTH;
  hint.min_height = HEIGHT; 						

  // формирование пиксельной карты повторяющегося элемента фона 

  pix = XCreatePixmap(dpy, root, WIDTH, HEIGHT, depth);			// создается пустая пиксельная карта требуемого размера 432*432 пик

  XSetBackground(dpy, gc, WhitePixel(dpy, src));            // установка фонового цвета графического контекста
  XSetForeground(dpy, gc, WhitePixel(dpy, src));				    // она заполняется белым квадратом 

  XFillRectangle(dpy, pix, gc, 0, 0, WIDTH, HEIGHT);        // рисуем заполненный прямоугольник
  XSetForeground(dpy, gc, BlackPixel(dpy, src));				    // установка цвета вывода примитивов графического контекста

  // рисуем прямоугольники в паттерне
  XDrawRectangle(dpy, pix, gc, 0, 		      0, 		        WIDTH/2, HEIGHT/6);
  XDrawRectangle(dpy, pix, gc, 0, 	        HEIGHT/6, 		WIDTH/2, HEIGHT/6);
  XDrawRectangle(dpy, pix, gc, 0, 	        HEIGHT/6*2, 	WIDTH/2, HEIGHT/6);
  XDrawRectangle(dpy, pix, gc, WIDTH/2, 	  HEIGHT/2,     WIDTH/2, HEIGHT/6);
  XDrawRectangle(dpy, pix, gc, WIDTH/2,  	  HEIGHT/6*4, 	WIDTH/2, HEIGHT/6);
  XDrawRectangle(dpy, pix, gc, WIDTH/2,  	  HEIGHT/6*5,   WIDTH/2, HEIGHT/6);

  XDrawRectangle(dpy, pix, gc, WIDTH/2, 	  0, 		      WIDTH/6, HEIGHT/2);
  XDrawRectangle(dpy, pix, gc, WIDTH/6*4,   0, 	        WIDTH/6, HEIGHT/2);
  XDrawRectangle(dpy, pix, gc, WIDTH/6*5,   0, 	        WIDTH/6, HEIGHT/2);
  XDrawRectangle(dpy, pix, gc, 0, 		      HEIGHT/2,	  WIDTH/6, HEIGHT/2);
  XDrawRectangle(dpy, pix, gc, WIDTH/6, 		HEIGHT/2, 	WIDTH/6, HEIGHT/2); 
  XDrawRectangle(dpy, pix, gc, WIDTH/6*2, 	HEIGHT/2, 	WIDTH/6, HEIGHT/2); 

  // создание графического окна 
  mask = (CWOverrideRedirect | CWBackPixmap);
  attr.override_redirect = False;						                    // декорировка оконным менеджером 
  attr.background_pixmap = pix;						                      // фиксирует идентификатор пиксельной карты из предыдущего блока 
  win = XCreateWindow(dpy, root, 0, 0, 800, 600, 1, depth,
                    InputOutput, CopyFromParent, mask, &attr);  // создается графическое окно программы с заданным начальным размером 800х600 пик
  XSetNormalHints(dpy, win, &hint);                             // −> свойство WM 
  mask = (ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask | KeyPressMask);
  XSelectInput(dpy, win, mask);						                      // устанавливается маска получения событий 
                                                                // нажатия кнопок мыши и клавы, когда мышь в окне 
  XMapRaised(dpy, win);							                            // сформированное окно отображается на экране
  XSetFunction(dpy, gc, GXinvert);					                    // устанавливается ф-я инвертирования изображения для операций граф вывода 


  // чтение и обработка событий из очереди Х-сервера *

  while(done == 0) {
  /* Итерации обработки этих мышиных событий могут продолжаться, пока флаг завершения цикла done=0 (False). 
  В альтернативе обработки нажатия любой клавиши на клавиатуре (когда окно имеет фокус ввода)
  этому флагу присваивается значение ее физического кода из поля keycode структуры XEventполученного 
  клавиатурного события. Поэтому значение флага doneстановится положительным (True) и происходит выход 
  из цикла обработки событий для последующего завершения программы.  */

    XNextEvent(dpy, &event);						// Чтение поступающих событий 
    switch(event.type) {
      case EnterNotify:
      case LeaveNotify:
      case ButtonPress:
      case ButtonRelease: XCopyArea(dpy, pix, pix, gc, 0, 0, WIDTH, HEIGHT, 0, 0);    // инвертирование изображения в пикс карте фона путем копирования ее области на себя
                          XSetWindowBackgroundPixmap(dpy, win, pix);	                // пикс карта переустанавливается для фона окна 
                          XClearWindow(dpy, win);				                              // инвертирование фонового рисунка окна 
                          break;
    /* событие нажатия и отпускания любой кнопки мыши, когда ее курсор находится внутри окна 
          событие пересечения курсором рамки окна в любом направлении */ 
      case KeyPress:      done = event.xkey.keycode;
                          break;
      default:            break;
    } //switch
  }  //while

  // завершающие действия

  XFreePixmap(dpy, pix);				// освобождается память пиксельной карты 
  XDestroyWindow(dpy, win);			// закрывается окно программы 
  XCloseDisplay(dpy);						// разрыв связи с X-сервером 
  return(done);								  // Физический код клавиши, которая была нажата для выхода из цикла обработки событий возвращает exit-код программы */ 
}
