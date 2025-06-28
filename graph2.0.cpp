#include <stdio.h>
#include <conio.c>
#include <math.h>
#include <windows.h>
#include <locale.h>
#define maxX 300
#define maxY 300

int bufferWidth = 80, bufferHeight = 25; //valores do console

bool tryXY(int x, int y){// função de validação de posicionamento e aproveitamento lógico para a criação do grafico
    return (x >= 0 && x < bufferWidth) && (y >= 0 && y < bufferHeight);
}

void xyCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (tryXY(x,y)) {
        COORD coord = {x, y};
        SetConsoleCursorPosition(hConsole, coord);
    } 
    else{
    	printf("X");
	}
}

void setConsoleSize(int cols, int rows) {
    if (cols < 1) cols = 11;
    if (cols > 500) cols = 500;
    if (rows < 1) rows = 11;
    if (rows > 500) rows = 500;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Pega info do console
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        printf("Erro ao obter info do console: %lu\n", GetLastError());
        return;
    }

    // Reduz temporariamente a janela para 1x1
    SMALL_RECT tempRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(hConsole, TRUE, &tempRect);

    // Define novo tamanho do buffer
    COORD bufferSize = {(SHORT)cols, (SHORT)rows};
    if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) {
        printf("Erro ao ajustar buffer: %lu\n", GetLastError());
        return;
    }

    // Pega o tamanho máximo da janela para este console (considerando resolução/fonte)
    COORD maxWindowSize = GetLargestConsoleWindowSize(hConsole);

    // Ajusta o tamanho da janela para o menor entre o que deseja e o máximo possível
    SMALL_RECT windowSize = {
        0, 0,
        (SHORT)(cols - 1 < maxWindowSize.X ? cols - 1 : maxWindowSize.X - 1),
        (SHORT)(rows - 1 < maxWindowSize.Y ? rows - 1 : maxWindowSize.Y - 1)
    };
    
    bufferWidth = (cols <= maxWindowSize.X ? cols : maxWindowSize.X);
	bufferHeight = (rows <= maxWindowSize.Y ? rows : maxWindowSize.Y);
    
    
    

    if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize)) {
        printf("Erro ao ajustar janela: %lu\n", GetLastError());
        return;
    }
}

void setConsoleMaxSize() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        printf("Erro ao obter handle do console\n");
        return;
    }

    // Pega o tamanho máximo da janela que cabe na tela (em caracteres)
    COORD maxSize = GetLargestConsoleWindowSize(hConsole);
    if (maxSize.X == 0 || maxSize.Y == 0) {
        printf("Erro ao obter tamanho máximo da janela\n");
        return;
    }

    // Primeiro reduz a janela temporariamente para evitar conflito na mudança do buffer
    SMALL_RECT tmpWindow = {0, 0, 1, 1};
    SetConsoleWindowInfo(hConsole, TRUE, &tmpWindow);

    // Ajusta o tamanho do buffer para o máximo permitido
    if (!SetConsoleScreenBufferSize(hConsole, maxSize)) {
        DWORD err = GetLastError();
        printf("Erro ao ajustar tamanho do buffer: %lu\n", err);
        return;
    }

    // Agora ajusta a janela para ocupar todo o buffer
    SMALL_RECT window = {0, 0, (SHORT)(maxSize.X - 1), (SHORT)(maxSize.Y - 1)};
    if (!SetConsoleWindowInfo(hConsole, TRUE, &window)) {
        DWORD err = GetLastError();
        printf("Erro ao ajustar tamanho da janela: %lu\n", err);
    }
    
    bufferWidth = maxSize.X;
	bufferHeight = maxSize.Y;
}

void maximizeConsoleWindow() {
    HWND hwndConsole = GetConsoleWindow(); // Pega a janela do console
    if (hwndConsole != NULL) {
        ShowWindow(hwndConsole, SW_MAXIMIZE); // Maximiza a janela
    }
}

int med(int* bufferZone){
	if(*bufferZone % 2 != 0){
		return (*bufferZone/2) + 1;
	}
	else{
		(*bufferZone)--;
		return ((*bufferZone)/2);
	}
}

void drawGraphTemplate(){// função para desenhar o gráfico
	int xMed = med(&bufferWidth);
	int yMed = med(&bufferHeight);
	
	//Y
	xyCursor(xMed,0);
	printf("y");
	xyCursor(xMed,1);
	printf("+");
	for(int i = 2; i < bufferHeight; i++){
		xyCursor(xMed,i);
		if(i == yMed){
			printf("0");
			continue;
		}
		printf("|");
	}
	xyCursor(bufferWidth-2, yMed);
	printf("+");
	xyCursor(bufferWidth-1, yMed);
	printf("x");
	for(int i = 0; i < bufferWidth-2; i++){
		xyCursor(i, yMed);
		if(i == xMed){
			printf("0");
			continue;
		}
		printf("-");
	}
}

int ajustCoord(int value, int type){
	if(type != 0 && type !=1)
		return -1;
	int ajusted_coord;
	if(type == 0){
		ajusted_coord = med(&bufferWidth) + value - 1;
	}
	else if(type == 1){
		ajusted_coord = med(&bufferHeight) - value;
	}
	return ajusted_coord;
}


float function1(int x){
  float y = sqrt(900 - x*x);
	return y;
}

float function2(int x){
  float y = -sqrt(900 - x*x);
	return y;
}

void ajustPoints(int x1, int x2,float (*f)(int) ,float escala){
	
	int f_x1 = f(x1) * escala;
	int f_x2 = f(x2) * escala;
	if(!(tryXY(ajustCoord(x1,0),ajustCoord(f_x1,1)) && tryXY(ajustCoord(x2,0),ajustCoord(f_x2,1))))
		return;
	int diff = abs(f_x1 - f_x2);
	if(diff == 0)
		return;
	
	//TESTE

	//*
		
	//COMECANDO A ALTERAR ! 
	int j;
	textcolor(LIGHTRED);
	if(f_x1 < f_x2){
		j = x1;
		for(int i = 1; i < diff; i++){
		if(i > diff/2)
		{
			j = x2;
		}
		xyCursor(ajustCoord(j,0), ajustCoord(f_x1 + i,1));
		printf("|");
	}
	}
	else if(f_x1 > f_x2){
		j = x2;
		for(int i = 1; i < diff; i++){
		if(i > diff/2)
		{
			j = x1;
		}
			
		xyCursor(ajustCoord(j,0), ajustCoord(f_x2 + i,1));
		printf("|");
	}
}
}

void drawFunctionTemplate(int minDomain, int maxDomain, float (*f)(int), float tam_escala){//desenha a função definida em fucntion(x: int):void e complemente o gráfico de drawGraphTemplate():void
	if(tam_escala <= 0){
		printf("valor invalido. a escala deve ser maior que 9 e menor ou igual a 100 (inteira)");
		return;
	}
	//TESTE (NAO DEFINITIVO)
	int x,y;
	textcolor(LIGHTRED);
	for(int i = minDomain; i < maxDomain; i++){
		//ALTERANDO AQUI ************************************************
		int temp_x = i;
		int temp_x2 = i + 1; //valor de x atual + 1 para o concerto da continuidade
		float temp_y = (int) (f(i) * tam_escala);
		x = ajustCoord(temp_x,0);
		y = ajustCoord((int)(temp_y),1);
		
		if(tryXY(x,y)){
			xyCursor(x,y);
			printf("*");
			ajustPoints(temp_x,temp_x2,f, tam_escala);//ajustar exibição de valores x
		}		
	}
	textcolor(WHITE);
}

void drawFunctionDefault(float (*f)(int)){
	drawFunctionTemplate(-med(&bufferWidth)+1,med(&bufferWidth)-1,f,0.55);
}

int main(){
	setlocale(LC_ALL,"");
	//INICIO PADRÃO
	SetConsoleOutputCP(CP_UTF8);
	setConsoleMaxSize();
	maximizeConsoleWindow();
	float zoomVar = 1;	//ainda nao vou usar
	drawGraphTemplate();
	drawFunctionDefault(function1);
	drawFunctionDefault(function2);

	xyCursor(0, bufferHeight-4);
}
