#include <stdio.h>
#include <conio.h>
#include <stdlib.h>   // declaramos las bibliotecas que usaremos

typedef struct{
	int mine;
	int flag;
	int neighbors;
	int open;
} location;		// esta estructura nos ayudará a asignar características especificas a cada casilla del buscaminas
			// si esta abierto, cerrado, si tiene vecinos, mina o bandera.


// estas son las variables globales que estaremos usando, algunas las inicializamos y nos ayudaran con el score

char nivel, button;
location game[80][50];   // mi matriz de estructuras
int posx, posy, startx, starty, limitx, limity, width, heigh, match;
long int puntaje;
int mines = 0;
int cells_open = 0;	//no hay ninguna celda abierda ni mina, ni bandera
int total_cells = 0;
int flags = 0;



// esta función dibuja el marco de diseño de la primera parte

void marco (void){
	int i, j;
	for(i=2; i<80; i++){
		gotoxy(i,1);
		textcolor(14);
		cprintf("%c", 205);
		gotoxy(i, 50);
		cprintf("%c", 205);
	}
	for(j=2; j<50; j++){
		gotoxy(1, j);
		cprintf("%c", 186);
		gotoxy(79, j);
		cprintf("%c", 186);
	}
	gotoxy(1,1);
	cprintf("%c", 201);
	gotoxy(79,1);
	cprintf("%c", 187);
	gotoxy(1,50);
	cprintf("%c", 200);
	gotoxy(79,50);
	cprintf("%c", 188);
	return;
}


// este es nuestro intro en donde usamos el archivo y damos las opciones de juego e instrucciones

int intro(void){

	char tecla, c, d;
	FILE *nom;
	textbackground(1);
	textcolor(13);
	clrscr();
	match=1;
	nom = fopen ("Bus.txt", "r");
	while(!feof(nom)){
		c=fgetc(nom);
		printf("%c", c);
	}
	marco();
	gotoxy(24,20);
	cprintf("PRESIONA (J) PARA EMPEZAR A JUGAR");
	gotoxy(27,25);
	cprintf("PRESIONA (ESC) PARA SALIR");
	gotoxy(32,42);
	cprintf("Creado por:");
	gotoxy(32,43);
	cprintf("Coronel, Aldair.");
	gotoxy(32,44);
	cprintf("Orea, Frida");
	gotoxy(32,45);
	cprintf("Enero/2016");
	fclose(nom);
	do
	{
		button = getch();

	}
	while(button !='j' && button != 'i' && button != 27);
	if(button == 27)
		match = 0;
	else if(button == 'j'){
		clrscr();
		gotoxy(10,5);
		cprintf("El objetivo del juego es evitar las minas.");
		gotoxy(10,6);
		cprintf("Usa las letras (A) (S) (D) y (W) para moverte en el tablero.");
		gotoxy(10,7);
		cprintf("Usa la letra (F) para colocar bandera.");
		gotoxy(10,8);
		cprintf("Presiona (ENTER) para abrir una casilla.");
		gotoxy(10,9);
		cprintf("Presiona (P) para salir del juego.");
		gotoxy(10,12);
		cprintf("¨C%cmo jugar?", 162);
		gotoxy(10,14);
		cprintf("1) Para empezar, presiona (ENTER) en cualquier recuadro.");
		gotoxy(10,15);
		cprintf("2) Presta atenci%cn a los n%cmeros para encontrar las minas.", 162, 163);
		gotoxy(10,16);
		cprintf("3) Presiona (F) en los recuadros que creas que contienen minas.");
		gotoxy(10,17);
		cprintf("4) ­Espero te diviertas!.");
		gotoxy(32, 24);
		cprintf("Selecciona dificultad:");
		gotoxy(32, 27);
		cprintf("(F) F%ccil", 160);
		gotoxy(32, 29);
		cprintf("(N) Normal");
		gotoxy(32, 31);
		cprintf("(D) Dif%ccil", 161);
		do
		{
			nivel=getch();
		}
		while(nivel != 'f' && nivel != 'n' && nivel != 'd' && nivel != 'r');
		gotoxy(32, 27);
		cprintf("¡Correcto en todo momento!");
	}
	//getch();
	return match;
}

//160 y 161

// esta funcion se encarga de dar el tipo ajedrez al tablero

void choose_color(int x, int y){
	if((x+y) % 2 == 0)
		textcolor(2);
	else
		textcolor(15);
}



// esta función realiza el trazado del tablero, su marco y su interior


void frame(int startx, int starty, int width, int heigh){
	int a, b, y, l;
	clrscr();
	for(y=starty; y<=heigh+starty; y++){

		if(y==starty){

			textcolor(11);
			gotoxy(startx, starty);
			cprintf("%c", 201);

			for (a=1; a<=width; a++){

				gotoxy(startx+a, starty);
				cprintf("%c", 205);

				if(a==width){

					gotoxy(startx+a+1, starty);
					cprintf("%c", 187);

				}

			}

		}

		else

			if(y==heigh+starty){

				gotoxy(startx, y);
				cprintf("%c", 200);

				for(b=1; b<=width; b++){

					gotoxy(startx+b, y);
					cprintf("%c", 205);

					if(b==width){

						gotoxy(startx+b+1, y);
						cprintf("%c", 188);

					}

				}

			}

			else{

				gotoxy(startx, y);
				cprintf("%c", 186);

				for(l=1; l<=width; l++){

					choose_color(startx+l , y);
					gotoxy(startx+l, y);
					cprintf("%c", 219);

					if(l==width){

						textcolor(11);
						gotoxy(startx+l+1, y);
						cprintf("%c", 186);

					}

				}

			}
	}

	return;

}



//aquí está nuestra función score que va llevando la cuenta de las celdas y minas

void score(){
	gotoxy(35, 3);
	textcolor(13);
	cprintf("JUEGO ACTUAL");
	gotoxy(31, 5);
	cprintf("TOTAL DE CELDAS: %d", total_cells);
	gotoxy(29, 6);
	cprintf("TOTAL DE MINAS MINAS: %3d", mines-flags);
	gotoxy(28, 7);
	cprintf("TOTAL DE CELDAS ABIERTAS: %d", cells_open);

}




// está función nos sirvió de maravilla, la mandamos a llamar cuando queremos hacer un movimiento
// modificamos los parametros y nos regresa verdadero si podemos movernos a ese lugar y falso que no

int check(int positionx, int positiony, int limitx, int limity, int startx, int starty){

	if(positionx >= startx && positionx < limitx && positiony >= starty && positiony < limity)
		return 1;
	return 0;

}



// es la que cuenta los vecinos si tienen minas, si es así lo que hace es aumentar el valor de la variable
// de nuestra estructura

void count_neighbors(){

	int x, y;
	for(x = posx; x <= limitx; x++){

		for(y = posy; y <= limity-1; y++){

			if(check(x, y, limitx+1, limity, startx+1, starty+1) && game[x-1][y].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y, limitx+1, limity, startx+1, starty+1) && game[x-1][y-1].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y,limitx+1, limity, startx+1, starty+1) && game[x][y-1].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y, limitx+1, limity, startx+1, starty+1) && game[x+1][y-1].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y, limitx+1, limity, startx+1, starty+1) && game[x+1][y].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y,limitx+1, limity, startx+1, starty+1) && game[x+1][y+1].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y, limitx+1, limity, startx+1, starty+1) && game[x][y+1].mine == 1)
				game[x][y].neighbors ++;
			if(check(x, y, limitx+1, limity, startx+1, starty+1) && game[x-1][y+1].mine == 1)
				game[x][y].neighbors ++;


		}

	}

}



// inicializa el tablero y asigna las minas a lugares random

void start(int posx, int posy, int width, int heigh){

	int x, y;
	srand( time(NULL));
	for(x = posx; x <= limitx; x++){

	      for(y = posy; y <= limity-1; y++){

		 if(rand() % 10 == 0){
			game[x][y].mine = 1;
			mines++;
		 }
		 else
			game[x][y].mine = 0;

		 game[x][y].flag = 0;
		 game[x][y].neighbors = 0;
		 game[x][y].open = 0;
		 total_cells++;
		}


	}

	count_neighbors();
	frame(startx, starty, width, heigh);
	score();

}





// checa si el movimiento a la izquierda es valido

void left(){

	if(check(posx-1, posy, startx + width+1, starty + heigh, startx+1, starty+1))
		posx--;


}



// checa si el movimiento a abajo valido

void down(){

	if(check(posx, posy+1, startx + width+1, starty + heigh, startx+1, starty+1))
		posy++;

}


void up(){

	if(check(posx, posy-1, startx + width+1, starty + heigh, startx+1, starty+1))
		posy--;

}


void right(){

	if(check(posx+1, posy, startx + width+1, starty + heigh, startx+1, starty+1))
		posx++;

}


//pone y quita las banderas

void flag(){

	if(game[posx][posy].flag == 0 && game[posx][posy].open == 0){
		game[posx][posy].flag = 1;
		flags++;  // el contador de las banderas
	}

	else if(game[posx][posy].flag == 1 && game[posx][posy].open == 0){
		game[posx][posy].flag = 0;
		flags--;
	}
	score(); // actualizamos el score


}


// definitivamente la función que más nos mantuvo pensando, es una función recursiva que nos ayuda a
// abrir muchas celdas a la vez cuando la que abrimos está en blanco, típico del buscaminas

void open_cell(int x, int y){

	if(game[x][y].open == 1)
		return;

	game[x][y].open = 1;
	cells_open++;
	puntaje+=100;
	score();
	if(game[x][y].neighbors == 0){

		if(check(x-1, y, startx + width + 1, starty + heigh, startx+1, starty+1))
			open_cell(x-1, y);
		if(check(x, y-1, startx + width + 1, starty + heigh, startx+1, starty+1))
			open_cell(x, y-1);
		if(check(x+1, y, startx + width + 1, starty + heigh, startx+1, starty+1))
			open_cell(x+1, y);
		if(check(x, y+1, startx + width + 1, starty + heigh, startx+1, starty+1))
			open_cell(x, y+1);


	}

}


// te pone todas las minas si acabas de perder, se podría decir que te enciende todas las minas

void lose_game(){
	int i, j;
	for(i = startx+1; i<= startx + width; i++)
		for(j = starty+1; j<=starty + heigh -1; j++){
			if(game[i][j].mine == 1){
			gotoxy(i, j);
			textcolor(12);
			cprintf("*");
			}

		}
	//getch();
	gotoxy(35, 45);
	cprintf("*************");
	gotoxy(37, 46);
	cprintf("PERDISTE");
	gotoxy(35, 47);
	cprintf("*************");
	match=0;
       getch();

}


//función del click
void enter(){

	int x, y;
	if(game[posx][posy].mine == 1)
		lose_game();

	else
		open_cell(posx, posy);


}



//este es una parte fundamental, a cada movimiento es llamada y actualiza
// TODO el tablero 


void print_board(int limitx, int limity, int startx, int starty){

	int x, y;
	for(x = startx+1; x<=limitx; x++){

		for(y = starty+1; y<=limity-1; y++){

			if(x == posx && y == posy){
				gotoxy(x, y);
				textcolor(13);
				cprintf("%c", 219);

			}

			else if(game[x][y].flag == 1){

				gotoxy(x, y);
				textcolor(12);
				cprintf("%c", 219);


			}

			else if(game[x][y].open == 0){

				choose_color(x, y);
				gotoxy(x, y);
				cprintf("%c", 219);
			}


			else if(game[x][y].open == 1 && game[x][y].neighbors == 0){

				gotoxy(x, y);
				textcolor(1);
				cprintf("%c", 219);


			}

			else if(game[x][y].open == 1 && game[x][y].neighbors != 0){

				gotoxy(x, y);
				textcolor(14);
				cprintf("%d", game[x][y].neighbors);

			}


		}
	}

}



//nuestra función de control secundario, selecciona el nivel y en base a eso declara 
// variables e inicializa el juego

void play(){

	match = 1;
	char button;

	//clrscr();
	//textbackground(1);
	//intro();

	if(nivel == 'f'){
		startx = 30;
		starty = 15;
		width = 20;
		heigh = 20;
		posx = 31;
		posy = 16;
		limitx = startx + width;
		limity = starty + heigh;
		start(posx, posy, width, heigh);
	}

	else{

		if(nivel == 'n'){
			startx = 25;
			starty = 10;
			width = 30;
			heigh = 30;
			posx = 26;
			posy = 11;
			limitx = startx + width;
			limity = starty + heigh;
			start(posx, posy, width, heigh);
		}


		else{

			if(nivel == 'd'){
				startx = 21;
				starty = 9;
				width = 40;
				heigh = 35;
				posx = 22;
				posy = 10;
				limitx = startx + width;
				limity = starty + heigh;
				start(posx, posy, width, heigh);
			}

		}

	}

	textcolor(13);
	gotoxy(posx, posy);
	cprintf("%c", 219);

	while(match == 1){

		button = getch();
		if(button == 'a')
		  left();
		else if(button == 's')
		  down();
		else if(button == 'd')
		  right();
		else if(button == 'w')
		  up();
		else if(button == 'p')
		  match = 0;
		else if(button == 'f')
		  flag();
		else if(button == 13)
		  enter();

		print_board(startx + width, starty + heigh, startx, starty );

		if(total_cells - cells_open == mines){
			gotoxy(15, 44);
			textcolor(14);
			cprintf("=======================================================");
			gotoxy(16, 45);
			cprintf("FELICIDADES, GANASTE, ERES GRANDE, %cNICO E IRREPETIBLE", 233);
			gotoxy(15, 46);
			cprintf("=======================================================");
			match = 0;
			getch();
		}
	}


	//getch();
}



// por último nuestra pequeña función main  :3

int main(){
	int matc;
	clrscr();
	textbackground(1);
	matc=intro();
	if (matc!=0)
		play();
	return 0;

}

