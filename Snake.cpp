
#include <ncurses.h>
#include <unistd.h>
#include <algorithm>
#include <stdio.h>
#include <locale.h>
#include <time.h>
struct Pos{
	int x;
	int y;
	char symbol;
	friend bool operator==(const Pos &a, const Pos &b){
		return (a.x == b.x) && (a.y == b.y);	
	}
	Pos operator+(const Pos &a){
		Pos ans;
		ans.x = (*this).x + a.x;
		ans.y = (*this).y + a.y;
		return ans;		
	}
};

const int max_X = 40;
const int max_Y = 30;

struct Food{
	Pos Coord;
	int value;
};
void PrintOnPos(const Pos Coord, const char &c);
bool Collision(Pos *Snake, const Pos &Coord, const int &l);
void SetFood(Food &food,const int value, Pos *Snake,const int &l);

void SetFood(Food &food,const int value, Pos *Snake,const int &l){
	
	do{
		food.Coord.x = rand()%max_X+1;
		food.Coord.y = rand()%max_Y+1;
	}while(Collision(Snake,food.Coord,l));
	food.value = value;
	attron(COLOR_PAIR(10));
	PrintOnPos(food.Coord,char(value+48));
	refresh();
}

void FoodBlinck(Food &food, bool &why){
	if (why){
		attron(COLOR_PAIR(11));
		PrintOnPos(food.Coord,char(food.value+48));
		refresh();	
	}else{
		attron(COLOR_PAIR(10));
		PrintOnPos(food.Coord,char(food.value+48));
		refresh();
	}
	why = !why;
}

bool Collision(Pos *Snake, const Pos &Coord, const int &l){
	for (int i=0;i<l;i++){
		if (Coord == Snake[i]) return true;	
	}
	return false;
}

void PrintOnPos(const Pos Coord, const char &c){
	move(Coord.y, 2*Coord.x);
	addch(c);
	addch(c);
}

void DrawSnake(Pos *Snake, const int &l,char c){
	for (int i=0;i<l;i++){
		
		if (c!=' ') attron(COLOR_PAIR(3+rand()%7));			
		PrintOnPos(Snake[i],c);	
		if (c!='c') refresh();	
	}
}

int main(){

	initscr();
	start_color();
	setlocale(LC_CTYPE,"");
	curs_set(0);
        init_pair(4,  COLOR_YELLOW,  COLOR_YELLOW);
        init_pair(5,  COLOR_BLUE,    COLOR_BLUE);
        init_pair(6,  COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(7,  COLOR_CYAN,    COLOR_CYAN);
        init_pair(8,  COLOR_BLUE,    COLOR_BLUE);
        init_pair(9,  COLOR_WHITE,   COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_GREEN );
	init_pair(2, COLOR_BLUE, COLOR_BLACK );
	init_pair(1, COLOR_BLUE, COLOR_BLUE );
	init_pair(10, COLOR_WHITE, COLOR_BLACK );
	init_pair(11, COLOR_RED, COLOR_BLACK );
	attron(COLOR_PAIR(1));

	for (int i=0;i<max_X+2;i++){
		PrintOnPos({i,0},'0');
		PrintOnPos({i,max_Y+1},'0');	
	}
	for (int i=0;i<max_Y+2;i++){
		PrintOnPos({0,i},'0');
		PrintOnPos({max_X+1,i},'0');	
	}
	Pos Snake[max_X*max_Y];
	int l = 4;
	refresh();
	attron(COLOR_PAIR(1));
	for (int i=0;i<l;i++){
		Snake[i].x = 5;
		Snake[i].y = 15;
		Snake[i].symbol = '#';
		PrintOnPos(Snake[i],'#');	
	}
	
	refresh();
	Pos dPos[4] = {{1,0},{-1,0},{0,-1},{0,1}};
	int dir = 1;
	noecho();
	nodelay(stdscr, TRUE);
	Food food;
	attron(COLOR_PAIR(2));
	srand(time(NULL));		
	SetFood(food,rand()%4+1,Snake,l);
	refresh();
	bool GameOver = false;
	int score = 0;
	int CurrentIndex = 0;
	Pos HeadCoord = Snake[0];
	HeadCoord.symbol = '#';
	bool why = true;
	while(!GameOver){
		attron(COLOR_PAIR(10));
		move(2,max_X*2 +6);
		printw("%s%i","Score: ",score);
		refresh();
		int ch = getch();
		
		switch (ch) {
		    case 'w':  if (dir!=3) dir = 2; break;
		    case 's':  if (dir!=2) dir = 3; break;
		    case 'a':  if (dir!=0) dir = 1; break;
		    case 'd':  if (dir!=1) dir = 0; break;
		}
		char c[100];
		scanw("%s",&c);
		Pos NextPos = HeadCoord + dPos[dir];
		if (NextPos.x > max_X){
			NextPos.x = 1;			
		}
		if (NextPos.y > max_Y){
			NextPos.y = 1;		
		}
		if (NextPos.x < 1){
			NextPos.x = max_X;			
		}
		if (NextPos.y < 1){
			NextPos.y = max_Y;		
		}
		
		
		
		GameOver = Collision(Snake,NextPos,l);
		if (!GameOver){
			
			attron(COLOR_PAIR(2));
			DrawSnake(Snake,l,' ');
			refresh();
			HeadCoord = NextPos;
			Snake[CurrentIndex] = HeadCoord;
			CurrentIndex++;
			if (CurrentIndex >= l){
				CurrentIndex = 0;
			}
				
			DrawSnake(Snake,l,'#');
			if (food.Coord == HeadCoord){
				for (int j=0;j<food.value;j++){
					Snake[l] = HeadCoord;
					Snake[l].symbol = '#';
					l++;	
				}
				score+=food.value;
				
				SetFood(food,rand()%4+1,Snake,l);
							
			}
			
		}
		FoodBlinck(food,why);
		unsigned int ms = 100000;
		usleep(ms);	
	}
	
	endwin(); 
	return 0;
}
