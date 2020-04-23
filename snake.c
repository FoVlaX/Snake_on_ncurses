
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
struct Pos{
	int x;
	int y;
	char symbol;

};

struct Pos summ(struct Pos a, const struct Pos b){
	a.x = a.x+b.x;
	a.y = a.y + b.y;
	return a;
}

bool comp(const struct Pos a, const struct Pos b){
	return (a.x == b.x) && (a.y == b.y);
}

const int max_X = 40;
const int max_Y = 30;

struct Food{
	struct Pos Coord;
	int value;
};
void PrintOnPos(const struct Pos Coord, const char c);
bool Collision(struct Pos *Snake, const struct Pos Coord, const int l);
struct Food SetFood(struct Food food,const int value, struct Pos *Snake,const int l);
struct Food SetFood(struct Food food,const int value, struct Pos *Snake,const int l){
	
	do{
		food.Coord.x = rand()%max_X+1;
		food.Coord.y = rand()%max_Y+1;
	}while(Collision(Snake, food.Coord,l));
	food.value = value;
	attron(COLOR_PAIR(10));
	PrintOnPos(food.Coord,value+48);
	refresh();
	return food;
}

void FoodBlinck(struct Food food, bool why){
	if (why){
		attron(COLOR_PAIR(11));
		PrintOnPos( food.Coord,food.value+48);
		refresh();	
	}else{
		attron(COLOR_PAIR(10));
		PrintOnPos( food.Coord,food.value+48);
		refresh();
	}
	why = !why;
}

bool Collision(struct Pos *Snake, const struct Pos Coord, const int l){
	for (int i=0;i<l;i++){
		if (comp(Coord,Snake[i])) return true;	
	}
	return false;
}

void PrintOnPos(const struct Pos Coord, const char c){
	move(Coord.y, 2*Coord.x);
	addch(c);
	addch(c);
}

void DrawSnake(struct Pos *Snake, const int l,char c){
	for (int i=0;i<l;i++){
		
		if (c!=' ') attron(COLOR_PAIR(3+rand()%7));			
		PrintOnPos(Snake[i],c);	
		if (c!='c') refresh();	
	}
}

int main(){

	initscr();
	start_color();
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
	struct Pos p;
	for (int i=0;i<max_X+2;i++){
		p.x = i;
		p.y = 0; 
		PrintOnPos(p,'0');
		p.x =i;
		p.y = max_Y+1;
		PrintOnPos(p,'0');	
	}
	for (int i=0;i<max_Y+2;i++){
		p.x = 0;
		p.y = i;
		PrintOnPos(p,'0');
		p.x = max_X+1;
		p.y = i;
		PrintOnPos(p,'0');	
	}
	struct Pos Snake[max_X*max_Y];
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
	struct Pos dPos[4] = {{1,0},{-1,0},{0,-1},{0,1}};
	int dir = 1;
	noecho();
	nodelay(stdscr, TRUE);
	struct Food food;
	attron(COLOR_PAIR(2));
	srand (time(NULL));		
	food = SetFood(food,rand()%4+1,Snake,l);
	refresh();
	bool GameOver = false;
	int score = 0;
	int CurrentIndex = 0;
	struct Pos HeadCoord = Snake[0];
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
		char s[100];
		scanw("%s",s);
		struct Pos NextPos = summ(HeadCoord , dPos[dir]);
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
			if (comp(food.Coord, HeadCoord)){
				for (int j=0;j<food.value;j++){
					Snake[l] = HeadCoord;
					Snake[l].symbol = '#';
					l++;	
				}
				score+=food.value;
				int rnd = rand()%4+1;
				food = SetFood(food,rnd,Snake,l);
							
			}
			
		}
		FoodBlinck(food,why);
		unsigned int ms = 100000;
		why = !why;
		usleep(ms);	
	}
	
	endwin(); 
	return 0;
}
