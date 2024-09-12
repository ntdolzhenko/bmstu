#include "trancatedCube.h"

static XVertex vertex[NVERT];	// массив вершин
static XEdge edge[NEDGE];		// массив ребер
static XFace face[NFACE+1];		// массив граней

static XPoint face3[NF3][(3+1)];
static XPoint face8[NF8][(8+1)];

static XPoint scale;			// структура масштаба по Х и Y

// Модельная ассоциация структуры полиграфа
int assoc(XPolyGraph* pg){
	pg->vertex = vertex; 	// адресация массива вершин
	pg->edge = edge; 		// адресация массива ребер
	pg->face = face; 		// адресация массива граней
	retrace(); 				// трассировка граней
}

// собираем грани в один массив
int retrace(){
	int i = 0; 	// общий индекс граней 
	int j; 		// индекс равноугольных граней 

	//добавляем в массив граней 3ые грани
	for(j = 0; j < NF3; j++, i++){
		face[i].listVertex = face3[j];
		face[i].countVertex = 3;
		face[i].tone = DEFTONE;
		face[i].zone = XCreateRegion();
	}
	//добавляем в массив граней 8ые грани	
	for(j = 0; j < NF8; j++, i++){
		face[i].listVertex = face8[j];
		face[i].countVertex = 8;
		face[i].tone = DEFTONE;
		face[i].zone = XCreateRegion();
	}
	face[i].tone = DEFTONE;
	return 0;
}

// перестройка модельной геометрии графа
int rebuild(){

	// координаты точек
	static XPoint vconf[] = {
		{2,1},{11,1},{12,2},{12,11},{11,12},{2,12},{1,11},{1,2},
		{10,3},{10,10},{3,10},{3,3},{9,4},{9,9},{4,9},{4,4},{6,5},
		{7,5},{8,6},{8,7},{7,8},{6,8},{5,7},{5,6}

	};

	// индексы вершин 3-угольников
	static int fconf3[NF3][(3+1)]={

		{0,11,7,0},
		{1,2,8,1},
		{3,4,9,3},
		{10,5,6,10},
		{15,16,23,15},
		{12,17,18,12},
		{19,13,20,19},
		{21,14,22,21}
	};

	static int fconf8[NF8][(8+1)]={

		{0,1,8,12,17,16,15,11,0},
		{2,3,9,13,19,18,12,8,2},
		{4,5,10,14,21,20,13,9,4},
		{6,7,11,15,23,22,14,10,6},
		{16,17,18,19,20,21,22,23,16}

	};

	//пары вершин ребер
	static int econf[NEDGE][2] = {

		{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0},
		{1,8},{2,8},{3,9},{4,9},{5,10},{6,10},{7,11},{0,11},
		{11,15},{8,12},{9,13},{10,14},{15,16},{17,12},{12,18},
		{19,13},{20,13},{21,14},{22,14},{23,15},{16,17},{17,18},
		{18,19},{19,20},{20,21},{21,22},{22,23},{23,16}
	};

	int i,j;									// индексы вершин, ребер и граней
	for(i = 0; i < NVERT;i++){					// расчет оконных координат вершин
		vertex[i].x = scale.x*vconf[i].x;
		vertex[i].y = scale.y*vconf[i].y;
	}

	// задаем оконные координаты пар вершин всех ребер
	for(i = 0; i < NEDGE; i++){
		edge[i].x1 = vertex[econf[i][0]].x;
		edge[i].y1 = vertex[econf[i][0]].y;
		edge[i].x2 = vertex[econf[i][1]].x;
		edge[i].y2 = vertex[econf[i][1]].y;
	}
	
	// задаем оконные координаты вершин 3-угольных граней
	for(i = 0; i < NF3; i++){
		for(j = 0; j < (3+1); j++){
			face3[i][j].x = vertex[fconf3[i][j]].x;
			face3[i][j].y = vertex[fconf3[i][j]].y;
		}
	}

	// задаем оконные координаты вершин 8-ми угольных граней
	for(i = 0; i < NF8; i++){
		for(j = 0; j < (8+1); j++){
			face8[i][j].x = vertex[fconf8[i][j]].x;
			face8[i][j].y = vertex[fconf8[i][j]].y;
		}
	}
	return 0;
}

//контроль масштаба изображения
int rescale(unsigned width, unsigned height){

	int x,y;										// коэффициенты масштабирования по x и y
	x = width / NUNIT; y = height / NUNIT; 			// пересчет масштаба
	if((scale.x == x) && (scale.y == y)) return(0); // сохраняем масштаб
	scale.x = x; scale.y = y; 						// запоминаем масштаб
	return(NFACE); 									// измененяем масштаб 
}

//контроль изменения размеров окна
int resize (unsigned width, unsigned height){
	static XRectangle bak = {0,0,0,0};							//прошлые размеры
	if((bak.width == width)&&(bak.height == height)) return 0;	// сохраняем размеры
	bak.width = width; bak.height = height;						// запоминаем размеры
	return NFACE;												// изменяем размеры окна
}

//реконфигурация графа
int reconf(unsigned width, unsigned height){
	if(resize(width, height)==0) return 0;		// габаритный контроль
	if(rescale(width, height)!=0){				// контроль масштаба
		rebuild();								// перестравиваем граф
	}
	return NFACE;
}

// (X, Y)-идентификация грани для перекраски
int zotone(int x, int y){
	static XPoint bak = {0,0};					// прошлый масштаб
	int f = 0;									// индекс грани
	if((bak.x == scale.x) && (bak.y == scale.y)) f = NFACE; //контроль изменений масштаба изображения
	for(; f < NFACE; f++){						// перестройка регионов граней
		XDestroyRegion(face[f].zone);
		face[f].zone = XPolygonRegion(face[f].listVertex,face[f].countVertex,0);
	}
	bak.x = scale.x; bak.y = scale.y;			// запоминаем масштаб
	for(f = 0; f < NFACE; f++){					// поиск грани по точке внутри
		if(XPointInRegion(face[f].zone,x,y)==True)
			break;
	}
	face[f].tone = (face[f].tone+1)%NTONE;		// новый цвет
	return(f);									// возвращаем индекс грани для перекраски
}

