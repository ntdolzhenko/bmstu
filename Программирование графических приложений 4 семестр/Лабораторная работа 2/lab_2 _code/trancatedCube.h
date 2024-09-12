#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef XPoint XVertex;				// Структура вершины 
typedef XSegment XEdge;				// Структура ребра 

typedef struct {				// Структура грани 
	XPoint* listVertex;			// Адрес набора вершин 
	int countVertex;			// Число вершин
	int tone;					// Номер цвета 
	Region zone;				// Региональная зона 
} XFace;

typedef struct {					/* Структура многоугольного графа */
	XVertex* vertex; 				/* Адрес массива вершин */
	XEdge* edge; 					/* Адрес массива ребер */
	XFace* face; 					/* Адрес массива граней */
} XPolyGraph;

#define MB0 8					// 1 8-угольная внешняя грань 
#define NF3 8					// 8 3-угольных внутренних граней 
#define NF8 5					// 5 8-угольных внутренних граней 

#define NFACE (NF8+NF3)						// число внутренних граней 
#define NEDGE ((8*NF8+3*NF3+MB0)/2)			// число ребер 
#define NVERT (NEDGE-(NFACE+1)+2)			// число ребер по Эйлеру 

#define NTONE 4								// число цветов граней графа 
#define DEFTONE 0							// номер цвета грани по умолчанию 
#define VDOT 8								// диаметр вершин графа 
#define EWIDTH 2							// толщина ребер графа (<VDOT)
#define NUNIT 14							// диапазон градуировка схемы 

//геометрический модуль
int assoc(XPolyGraph*);
GC congraph(Display*);
Window wingraph(Display*, char*);
int colorite(Display*);
int regraph(Display*, Window, GC, int);
int reset(Display*, Window, int);
int reface(Display*, Window, GC, int);

//дисплейный модуль
int relink(XPolyGraph*);
int retrace();
int resize(unsigned, unsigned);
int rescale(unsigned, unsigned);
int rebuild();
int reconf(unsigned, unsigned);
int zotone(int, int);

//контрольный модуль
int rekey(XEvent*);
int dispatch(Display*, Window, GC);
int main(int, char* argv[]);
