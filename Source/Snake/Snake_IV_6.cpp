#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<windows.h>
#include<iostream>

#define MAPX (28)
#define MAPY (28)
using namespace std;

char TEST = 0; 

enum SIGN{GROUND, HEAD, UP, DOWN, RIGHT, LEFT, STAR, STAR2,
	AI_WORKING, AUTHOR, SPEED_UP, SLOW_DOWN, OPEN_LIGHT, NORMAL, EAT,
	CRASH, NO_REACTION, FREEZE};
void FindPath(char water[][MAPY], char map[][MAPY], int x, int y, int X, int Y, int ring);
void F_Count_Blocks(char water[][MAPY], char map[][MAPY], int x, int y, int ring);
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput,scrn);
}


void pause(void)
{
	if(getch() == 224)
	{
		getch();
	}
}
class object
{
	int a, b;
	
	char (*map)[MAPY];
	
	char (*Blocks)[3];
	int *Colors, *Light_Colors;
	int starx, stary;
	int open_light;
	int ring;
	
	public:
		object()
		{
			map = (char (*)[MAPY])malloc( sizeof( char[MAPY] ) * MAPX);
			for(a = 0; a < MAPX; a++)
			{
				for(b = 0; b < MAPY; b++)
				{
					map[a][b] = GROUND;
				}
			}
			map[MAPX-1][0] = RIGHT;
			map[MAPX-1][1] = RIGHT;
			map[MAPX-1][2] = HEAD;
			Blocks = (char (*)[3])malloc( sizeof( char [3] ) * 8);
			Colors = (int *)malloc( sizeof(int) * 8);
			Light_Colors = (int *)malloc( sizeof(int) * 8);
			strcpy(Blocks[0], "　");	Colors[0] = 7;	Light_Colors[0] = 8;
			strcpy(Blocks[1], "■");	Colors[1] = 10;	Light_Colors[1] = 160;
			strcpy(Blocks[2], "■");	Colors[2] = 2;	Light_Colors[2] = 32;
			strcpy(Blocks[3], "■");	Colors[3] = 2;	Light_Colors[3] = 32;
			strcpy(Blocks[4], "■");	Colors[4] = 2;	Light_Colors[4] = 32;
			strcpy(Blocks[5], "■");	Colors[5] = 2;	Light_Colors[5] = 32;
			strcpy(Blocks[6], "★");	Colors[6] = 14;	Light_Colors[6] = 224;
			strcpy(Blocks[7], "☆");	Colors[7] = 14;	Light_Colors[7] = 224;
			open_light = 0;
			ring = 0;
		}
		
		int monitor(int x, int y, char water[][MAPY], char mode)
		{
			if(mode == 1)
			{
				FindPath(water, map, starx, stary, -1, -1, ring);
			}
			if(open_light == 0)
			{
				gotoxy(2 * y + 2, x + 1);
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					Colors[ map[x][y] ] );
				
				cout<< Blocks[ map[x][y] ];
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					7 );
			}
			else
			{
				for(a = 0; a < MAPX; a++)
				{
					gotoxy(2, a + 1);
					for(b = 0; b < MAPY; b++)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							Light_Colors[ map[a][b] ] );
						printf("%2d", water[a][b]);
					}
				}
			}
			
			return 0;
		}
		int fix(int x, int y, int value, char water[][MAPY], char mode)
		{
			int temp;
			if(x >= 0 && x < MAPX && y >= 0 && y < MAPY )
			{
				temp = map[x][y];
				map[x][y] = value;
				monitor(x, y, water, mode);
			}
			else
			{
				temp = -1;
			}
			return temp;
		}
		void setstar(char water[][MAPY], char mode, FILE *fptr)
		{
			do
			{
				starx = rand()%MAPX;
				stary = rand()%MAPY;
			}while(map[starx][stary] != GROUND);
			
			if(TEST == 1)
			{ 
				fprintf(fptr,"%d %d\n",starx,stary); 
			} 
			
			fix(starx, stary, STAR, water, mode);
			return ;
		}
		
		friend class Snake;
		friend void welcome(class object * const MAP);
		friend int main(void);
		friend char Input(char mode, char water[][MAPY], class object MAP, class Snake *snake);
};
class Snake
{
	int headx, heady;
	int tailx, taily;
	int dirx, diry;
	char trace[4];
	int count;
	char change;
	char next_U;
	unsigned int length, score;
	char AI_all[MAPX][MAPY]; 
	public:
		Snake()
		{
			int a, b; 
			headx = MAPX - 1;
			heady = 2;
			tailx = MAPX - 1;
			taily = 0;
			dirx = 0;
			diry = 1;
			for(count = 0; count < 4; count++)
			{
				trace[count] = -1;
			}
			count = 0;
			length = 3;
			score = 0;
			change = 0;
			next_U = 4;
			for(a = 1; a < MAPX - 1; a++)
			{
				for(b = 0;( a != MAPX/2 )&&( a != MAPX/2 + 1 )&&( b < MAPY ); b++)
				{
					if(b % 2 == 0)
					{
						AI_all[a][b] = DOWN;
					}
					else
					{
						AI_all[a][b] = UP;
					}
				}
			}
			for(b = 1; b < MAPY - 1; b++)
			{
				if(b % 2 == 0)
				{
					AI_all[0][b] = DOWN;
					AI_all[MAPX/2][b] = LEFT;
					AI_all[MAPX/2 + 1][b] = DOWN;
					AI_all[MAPX - 1][b] = RIGHT;
				}
				else
				{
					AI_all[0][b] = LEFT;
					AI_all[MAPX/2][b] = UP;
					AI_all[MAPX/2 + 1][b] = RIGHT;
					AI_all[MAPX - 1][b] = UP;
				}
			}
			AI_all[0][0] = DOWN;
			AI_all[0][MAPY - 1] = LEFT;
			AI_all[MAPX/2][0] = DOWN;
			AI_all[MAPX/2 + 1][0] = DOWN;
			AI_all[MAPX - 1][0] = RIGHT;
			AI_all[MAPX/2][MAPY - 1] = UP;
			AI_all[MAPX/2 + 1][MAPY - 1] = UP;
			AI_all[MAPX - 1][MAPY - 1] = UP;
			/**
			gotoxy(0,0); 
			for(a=0;a<MAPX;a++) 
			{
				for(b=0;b<MAPY;b++)
				{
					if(AI_all[a][b] == UP)
					{
						cout<<"↑";
					}
					else if(AI_all[a][b] == LEFT)
					{
						cout<<"←";
					}
					else if(AI_all[a][b] == RIGHT)
					{
						cout<<"→";
					}
					else if(AI_all[a][b] == DOWN)
					{
						cout<<"↓";
					}
				}
				cout<<endl; 
			}
			getch();
			/**/
			
			/**
			gotoxy(2*(heady)+2,headx+1);
			cout<<"..";
			pause();/**/
		}
		void fixdir(char inputdir)
		{
			if(inputdir == -1)
			{
				cout<<"There is something that out of consideration.";
				getch();
			}
			else
			{
				if(inputdir == UP)
				{
					if(dirx == 0)
					{
						dirx = -1;
						diry = 0;
					}
				}
				else if(inputdir == LEFT)
				{
					if(diry == 0)
					{
						dirx = 0;
						diry = -1;
					}
				}
				else if(inputdir == RIGHT)
				{
					if(diry == 0)
					{
						dirx = 0;
						diry = 1;
					}
				}
				else if(inputdir == DOWN)
				{
					if(dirx == 0)
					{
						dirx = 1;
						diry = 0;
					}
				}
			}
			return ;
		}
		char move(class object MAP, char water[][MAPY], char mode)
		{
			int sit;
			int temp;
			if(dirx == -1)
			{
				temp = UP;
			}
			else if(diry == -1)
			{
				temp = LEFT;
			}
			else if(diry == 1)
			{
				temp = RIGHT;
			}
			else if(dirx == 1)
			{
				temp = DOWN;
			}
			trace[count] = temp;
			count = (count + 1) % 4;
			MAP.fix(headx, heady, temp, water, mode);
			headx += dirx;
			heady += diry;
			if(MAP.ring)
			{
				headx = (headx + MAPX)% MAPX;
				heady = (heady + MAPY)% MAPY;
			}
			sit = MAP.fix(headx, heady, HEAD, water, mode);
			if(sit == -1 || (sit != STAR && sit != STAR2 && sit != GROUND))
			{
				sit = CRASH;
				gotoxy(2 * heady + 2, headx + 1);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MAP.Colors[HEAD]);
				cout<<MAP.Blocks[HEAD];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				
			}
			else if(sit != STAR && sit!= STAR2)
			{
				sit = NORMAL;
				temp = MAP.map[tailx][taily];
				MAP.fix(tailx, taily, GROUND, water, mode);
				if(temp == UP)
				{
					tailx--;
				}
				else if(temp == LEFT)
				{
					taily--;
				}
				else if(temp == RIGHT)
				{
					taily++;
				}
				else if(temp == DOWN)
				{
					tailx++;
				}
				if(MAP.ring)
				{
					tailx = (tailx + MAPX)% MAPX;
					taily = (taily + MAPY)% MAPY;
				}
			}
			else
			{
				sit = EAT;
			}
			return sit;
		}
		char AI(char water[][MAPY], class object MAP);
		char AI_2(char water[][MAPY], class object MAP);
		
		friend int main(void);
		friend char Input(char mode, char water[][MAPY], class object MAP, class Snake *snake);
};
void welcome(class object * const MAP)
{
	int a,b;
	char str[2][9] = {"邊界封閉","邊界開放"};
	SetConsoleTitle("Snake_IV_6");
	gotoxy(0,0);
	for(a = 0; a < MAPX + 12; a++)
	{
		cout<<"□";
	}
	for(a = 0; a < MAPX; a++)
	{
		cout<<"□";
		for(b = 0; b < MAPY; b++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				(*MAP).Colors[ (*MAP).map[a][b] ] );
			
			cout<< (*MAP).Blocks[ (*MAP).map[a][b] ];
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				(*MAP).Colors[ (*MAP).map[a][b] ] );
		}
		cout<<"□　　　　　　　　　□";
	}
	for(a = 0; a < MAPX + 12; a++)
	{
		cout<<"□";
	}
	gotoxy(2 * MAPY + 4, 2);
	cout<<"　貪吃蛇";
  	gotoxy(2 * MAPY + 4, 3);
  	cout<<"　　 Snake_IV_6";
  	gotoxy(2 * MAPY + 4, 5);
  	cout<<"　　   By pi314 ";
  	gotoxy(2 * MAPY + 4, 7);
  	cout<<"□□□□□□□□□";
  	gotoxy(2 * MAPY + 4, 9);
  	cout<<"　　遊戲現況";
  	gotoxy(2 * MAPY + 4, 11);
  	cout<<"　長度:   3";
  	gotoxy(2 * MAPY + 4, 12);
  	cout<<"　速度: 1/20";
  	gotoxy(2*MAPY+4,13);
  	cout<<"　分數:   0";
  	gotoxy(2*MAPY+4,15);
  	cout<<"□□□□□□□□□";
  	gotoxy(2*MAPY+4,17);
  	cout<<"　　遊戲操作";
  	gotoxy(2*MAPY+4,19);
  	cout<<"　↑↓←→";
  	gotoxy(2*MAPY+4,20);
  	cout<<"　　控制蛇的";
  	gotoxy(2*MAPY+4,21);
  	cout<<"　　　行進方向";
  	gotoxy(2*MAPY+4,23);
  	cout<<"　+ - 加速減速";
  	gotoxy(2*MAPY+4,25);
  	cout<<"　□□　　　□□";
  	gotoxy(2*MAPY+4,26);
  	cout<<"□□ 普通模式 □□";
  	gotoxy(2*MAPY+4,27);
  	cout<<"　□□　　　□□";
  	
	gotoxy(16, 11);
	cout<<"請選擇遊戲模式(↑,↓,Enter):";
	gotoxy(16, 13);
	cout<<"→ 邊界封閉 ←";
	gotoxy(16, 15);
	cout<<"　 邊界開放 ";
	do
	{
		a = (char)getch();
		if(a == -32 || a == 0)
		{
			b = (char)getch();
		}
		if(a == -32 && b == 72)
		{
			if((*MAP).ring == 1)
			{
				gotoxy(16, 13);
				cout<<"→ 邊界封閉 ←";
				gotoxy(16, 15);
				cout<<"　 邊界開放 　";
			}
			(*MAP).ring = 0;
		}
		else if(a == -32 && b == 80)
		{
			if((*MAP).ring == 0)
			{
				gotoxy(16, 13);
				cout<<"　 邊界封閉 　";
				gotoxy(16, 15);
				cout<<"→ 邊界開放 ←";
			}
			(*MAP).ring = 1;
		}
		else if(a == 27)
		{
			if(TEST == 0)
			{
				SetConsoleTitle("TEST = 1");
				Sleep(500);
				TEST = 1;
			}
			else
			{
				SetConsoleTitle("TEST = 0");
				Sleep(500);
				TEST = 0;
			}
			SetConsoleTitle("Snake_IV_5");
		}
	}while(a != 13);
	gotoxy(16, 11);
	cout<<"──────────────";
	gotoxy(16, 13);
	cout<<"───────";
	gotoxy(16, 15);
	cout<<"───────";
	Sleep(30);
	gotoxy(16, 11);
	cout<<"　　　　　　　　　　　　　　";
	gotoxy(16, 13);
	cout<<"　　　　　　　";
	gotoxy(16, 15);
	cout<<"　　　　　　　";
		gotoxy(16, 11);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(16, 11);
		cout<<"　　　　";
		gotoxy(14, 9);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(14, 9);
		cout<<"　　　　";
		gotoxy(12, 7);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(12, 7);
		cout<<"　　　　";
		gotoxy(10, 5);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(10, 5);
		cout<<"　　　　";
		gotoxy(8, 3);
		cout<<str[(*MAP).ring];
		Sleep(30);
		gotoxy(8, 3);
		cout<<"　　　　";
		Sleep(30);
	if((*MAP).ring == 1)
	{
		SetConsoleTitle("Snake_IV_6 ─ 邊界開放");
	}
	else
	{
		SetConsoleTitle("Snake_IV_6 ─ 邊界封閉");
	}
	return ;
}
char Input(char mode, char water[][MAPY], class object MAP, class Snake *snake)
{
	char ch[2] = {-1, -1};
	int a, b;
	
	if(kbhit())
	{
		ch[0] = getch();
		if(ch[0] == -32 || ch[0] == 0)
		{
			ch[1] = getch();
		}
	}
	if(ch[0] == 27)
	{
		return AI_WORKING;
	}
	else if(ch[0] == '`')
	{
		return AUTHOR;
	}
	else if(ch[0] == '+' || (ch[0] == -32 && ch[1] == 73))
	{
		return SPEED_UP;
	}
	else if(ch[0] == '-' || (ch[0] == -32 && ch[1] == 81))
	{
		return SLOW_DOWN;
	}
	else if(ch[0] == '\\' || (ch[0] == 0 && ch[1] == 59))
	{
		return OPEN_LIGHT;
	}
	else if(ch[0] == 'p' || ch[0] == 'P' || ch[0] == 32) 
	{
		return FREEZE; 
	} 
	if(mode == 0)
	{
		if(ch[1] == 72 || ch[0] == 'w' || ch[0] == 'W')
		{
			return UP;
		}
		else if(ch[1] == 75 || ch[0] == 'a' || ch[0] == 'A')
		{
			return LEFT;
		}
		else if(ch[1] == 77 || ch[0] == 'd' || ch[0] == 'D')
		{
			return RIGHT;
		}
		else if(ch[1] == 80 || ch[0] == 's' || ch[0] == 'S')
		{
			return DOWN;
		}
		else
		{
			return NO_REACTION;
		}
	}
	else if(mode == 1)
	{
		if(MAP.Colors[6] == 14)
		{
			return (*snake).AI(water, MAP);
		}
		else
		{
			return (*snake).AI_2(water, MAP);
		} 
	} 
	cout<<"Input -1";
	return -1;
}
char Snake::AI(char water[][MAPY], class object MAP)
{
	char temp;
	int a, b;
	int max;
	char Count_Blocks[MAPX][MAPY];
	int count_blocks[4];/*上左右下*/
	
	FindPath(water, MAP.map, MAP.starx, MAP.stary, -1, -1, MAP.ring);
	/*數格子*/ 
	for(a = 0; a < 4; a++)
	{
		count_blocks[a] = 0;
	}
	F_Count_Blocks(Count_Blocks, MAP.map, headx, heady, MAP.ring);
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(Count_Blocks[a][b]&2)
			{
				count_blocks[0]++;
			}
			if(Count_Blocks[a][b]&4)
			{
				count_blocks[1]++;
			}
			if(Count_Blocks[a][b]&8)
			{
				count_blocks[2]++;
			}
			if(Count_Blocks[a][b]&16)
			{
				count_blocks[3]++;
			}
		}
	}
	/**
	for(a = 0;a<4;a++)
		cout<<count_blocks[a]<<' ';
	getch();
	/**/
	max = 0;
	for(a = 1; a < 4; a++) 
	{
		if(count_blocks[a] == count_blocks[max])
		{
			if(a == 1 &&( heady - 1 >= 0 || MAP.ring))
			{
				if(water[headx][(heady - 1+MAPY)%MAPY] < water[headx][heady] && water[headx][(heady - 1+MAPY)%MAPY] > 0)
					max = 1;
			}
			else if(a == 2 && (heady + 1 < MAPY || MAP.ring))
			{
				if(water[headx][(heady + 1)%MAPY] < water[headx][heady] && water[headx][(heady + 1)%MAPY] > 0)
					max = 2;
			}
			else if(a == 3 && (headx + 1 < MAPX || MAP.ring))
			{
				if(water[(headx + 1)%MAPX][heady] < water[headx][heady] && water[(headx + 1)%MAPX][heady] > 0)
					max = 3;
			}
		}
		else if(count_blocks[a] > count_blocks[max]) 
		{
			max = a; 
		} 
	} 
	/*數格子結束*/
		if(MAP.ring == 0)
		{
			if(headx + dirx < 0 || headx + dirx >= MAPX || 
				heady + diry < 0 || heady + diry >= MAPY ||
				water[headx + dirx][heady + diry] == -1)
			{
				a = 0;
			}
			else
			{
				a = 1;
			}
		}
		else
		{
			if(water[(headx + dirx + MAPX)%MAPX][(heady + diry + MAPY)%MAPY] == -1)
			{
				a = 0;
			}
			else
			{
				a = 1;
			}
		}
		if(!a)/*撞牆*/
		{
			if(max == 0)
			{
				return UP;
			}
			else if(max == 1)
			{
				return LEFT;
			}
			else if(max == 2)
			{
				return RIGHT;
			}
			else if(max == 3)
			{
				return DOWN;
			}
		}
		
		temp = -1;
		if(water[headx][heady] != 0)/*有路可吃到星星*/
		{
			change = 0;
			next_U = 4;
			if(MAP.ring == 0 && water[headx + dirx][heady + diry] < water[headx][heady] && water[headx + dirx][heady + diry] > 0)
			{
				temp = NO_REACTION;
			}
			else if(MAP.ring == 1 && water[(headx + dirx+MAPX)%MAPX][(heady + diry+MAPY)%MAPY] < water[headx][heady] && water[headx + dirx][heady + diry] > 0)
			{
				temp = NO_REACTION;
			}
			else if(temp == -1)/*繼續走會遠離星星*/
			{
				if(!MAP.ring)
				{
					if(headx - 1 >= 0)
					{
						if((water[headx - 1][heady] < water[headx][heady]) && 
							(water[headx - 1][heady] > 0) )
						{
							temp = UP;
						}
					}
					if((heady - 1 >= 0) && (temp == -1))
					{
						if((water[headx][heady - 1] < water[headx][heady]) && 
							(water[headx][heady - 1] > 0) )
						{
							temp = LEFT;
						}
					}
					if((heady + 1 < MAPY) && (temp == -1) )
					{
						if((water[headx][heady + 1] < water[headx][heady]) && 
							(water[headx][heady + 1] > 0) )
						{
							temp = RIGHT;
						}
					}
					if((headx + 1 < MAPX) && (temp == -1) )
					{
						if((water[headx + 1][heady] < water[headx][heady]) && 
						(water[headx + 1][heady] > 0) )
						{
							temp = DOWN;
						}
					}
				}
				else
				{
					if((water[(headx - 1+MAPX)%MAPX][heady] < water[headx][heady]) && 
						(water[(headx - 1+MAPX)%MAPX][heady] > 0) )
					{
						temp = UP;
					}
					if((water[headx][(heady - 1+MAPY)%MAPY] < water[headx][heady]) && 
						(water[headx][(heady - 1+MAPY)%MAPY] > 0) && (temp == -1) )
					{
						temp = LEFT;
					}
					if((water[headx][(heady + 1)%MAPY] < water[headx][heady]) && 
						(water[headx][(heady + 1)%MAPY] > 0) && (temp == -1) )
					{
						temp = RIGHT;
					}
					if((water[(headx + 1)%MAPX][heady] < water[headx][heady]) && 
					(water[(headx + 1)%MAPX][heady] > 0) && (temp == -1) )
					{
						temp = DOWN;
					}
				}
				/**
				cout<<"turn -1.";
				return -1;/**/ 
			}
		}
		else/*沒路可吃到星星*/
		{
			if(dirx != 0)
			{
				a = b = 0;
				if(trace[((int)count + 0) % 4] == RIGHT || trace[((int)count + 0) % 4] == LEFT)
					a = 1;
				if(trace[((int)count + 2) % 4] == RIGHT || trace[((int)count + 2) % 4] == LEFT)
					b = 1;
				
				if(a * b)
				{
					/**
					gotoxy(0,0);
					for(a=3;a>=0;a--)
					{
						cout<<(int)trace[((int)count + a + 4) % 4]<<' ';
					}
					cout<<(int)change;
					getch();/**/
					temp = -1;
					if(MAP.ring == 0)
					{
						if(heady - 1 >= 0 && MAP.map[headx][heady - 1] == GROUND && change < next_U)
						{
							change++;
							temp = LEFT;
						}
						else if(heady + 1 < MAPY && MAP.map[headx][heady + 1] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = RIGHT;
						}
						else if(temp == -1) 
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
					else
					{
						if(MAP.map[headx][(heady - 1+MAPY)%MAPY] == GROUND && change < next_U)
						{
							change++;
							temp = LEFT;
						}
						else if(MAP.map[headx][(heady + 1)%MAPY] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = RIGHT;
						}
						else if(temp == -1) 
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
				}
				else
				{
					temp = NO_REACTION;
				}
				
			}
			else if(diry != 0)
			{
				a = b = 0;
				if(trace[((int)count + 0) % 4] == UP || trace[((int)count + 0) % 4] == DOWN)
					a = 1;
				if(trace[((int)count + 2) % 4] == UP || trace[((int)count + 2) % 4] == DOWN)
					b = 1;
				
				if(a * b)
				{
					/**
					gotoxy(0,0);
					for(a=3;a>=0;a--)
					{
						cout<<(int)trace[((int)count + a + 4) % 4]<<' ';
					}
					cout<<(int)change;
					getch();/**/
					if(MAP.ring == 0)
					{
						if(headx - 1 >= 0 && MAP.map[headx - 1][heady] == GROUND && change < next_U)
						{
							change++;
							temp = UP;
						}
						else if(headx + 1 < MAPX && MAP.map[headx + 1][heady] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = DOWN;
						}
						else if(temp == -1)
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
					else
					{
						if(MAP.map[(headx - 1+MAPX)%MAPX][heady] == GROUND && change < next_U)
						{
							change++;
							temp = UP;
						}
						else if(MAP.map[(headx + 1)%MAPX][heady] == GROUND && 
							change < next_U && temp == -1)
						{
							change++;
							temp = DOWN;
						}
						else if(temp == -1)
						{
							if(change >= next_U)
							{
								change = 0;
								next_U = 8;
							}
							temp = NO_REACTION;
						}
					}
				}
				else
				{
					temp = NO_REACTION;
				}
				
			}
		}
	if(temp == NO_REACTION)
	{
		if(dirx == -1)
		{
			temp = UP;
		}
		else if(diry == -1)
		{
			temp = LEFT;
		}
		else if(diry == 1)
		{
			temp = RIGHT;
		}
		else if(dirx == 1)
		{
			temp = DOWN;
		}
	}
	if(temp == UP) 
	{
		temp = 0; 
	}
	else if(temp == LEFT)
	{
		temp = 1;
	}
	else if(temp == RIGHT)
	{
		temp = 2;
	}
	else if(temp == DOWN)
	{
		temp = 3;
	}
	if( (count_blocks[max] - count_blocks[temp]) > (length / 2))
	{
		temp = max;
		/**
		cout<<'['<<(int)temp<<']';
		getch();/**/
	} 
	
	if(temp == 0)
	{
		return UP;
	}
	else if(temp == 1)
	{
		return LEFT;
	}
	else if(temp == 2)
	{
		return RIGHT;
	}
	else if(temp == 3)
	{
		return DOWN;
	}
}
char Snake::AI_2(char water[][MAPY], class object MAP)
{
	char temp;
	int a, b;
	int max;
	char Count_Blocks[MAPX][MAPY];
	int count_blocks[4];/*上左右下*/
	
	FindPath(water, MAP.map, MAP.starx, MAP.stary, -1, -1, MAP.ring);
	/*數格子*/ 
	for(a = 0; a < 4; a++)
	{
		count_blocks[a] = 0;
	}
	F_Count_Blocks(Count_Blocks, MAP.map, headx, heady, MAP.ring);
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(Count_Blocks[a][b]&2)
			{
					count_blocks[0]++;
			}
			if(Count_Blocks[a][b]&4)
			{
				count_blocks[1]++;
			}
			if(Count_Blocks[a][b]&8)
			{
				count_blocks[2]++;
			}
			if(Count_Blocks[a][b]&16)
			{
				count_blocks[3]++;
			}
		}
	}
	max = 0;
	for(a = 1; a < 4; a++) 
	{
		if(count_blocks[a] == count_blocks[max])
		{
			if(a == 1)
			{
				if(heady - 1 >= 0||MAP.ring)
				{
					if(water[headx][(heady - 1+MAPY)%MAPY] < water[headx][heady])
						max = 1;
				}
			}
			else if(a == 2)
			{
				if(heady + 1 < MAPY || MAP.ring)
				{
					if(water[headx][(heady + 1)%MAPY] < water[headx][heady])
						max = 2;
				}
			}
			else if(a == 3)
			{
				if(headx + 1 < MAPX || MAP.ring)
				{
					if(water[(headx + 1)%MAPX][heady] < water[headx][heady])
						max = 3;
				}
 			}
		}
 		else if(count_blocks[a] > count_blocks[max]) 
		{
			max = a;
		}
	}
	/*數格子結束*/
	if(headx + dirx >= 0 && headx + dirx < MAPX && 
		heady + diry >= 0 && heady + diry < MAPY && 
		water[headx + dirx][heady + diry] == -1)/*撞牆*/ 
	{
		if(max == 0)
		{
			return UP;
		}
		else if(max == 1)
		{
			return LEFT;
		}
		else if(max == 2)
		{
			return RIGHT;
		}
		else if(max == 3)
		{
			return DOWN;
		}
	}
	
	temp = AI_all[headx][heady];
	/**
	cout<<(int)temp;
	getch(); /**/ 
	if(temp == NO_REACTION)
	{
		if(dirx == -1)
		{
			temp = UP;
		}
		else if(diry == -1)
		{
			temp = LEFT;
		}
		else if(diry == 1)
		{
			temp = RIGHT;
		}
		else if(dirx == 1)
		{
			temp = DOWN;
		}
	}
	if(temp == UP) 
	{
		temp = 0; 
	}
	else if(temp == LEFT)
	{
		temp = 1;
	}
	else if(temp == RIGHT)
	{
		temp = 2;
	}
	else if(temp == DOWN)
	{
		temp = 3;
	}
	if( (count_blocks[max] - count_blocks[temp]) > (length / 2))
	{
		temp = max;
	} 
	
	if(temp == 0)
	{
		return UP;
	}
	else if(temp == 1)
	{
		return LEFT;
	}
	else if(temp == 2)
	{
		return RIGHT;
	}
	else if(temp == 3)
	{
		return DOWN;
	}
}
void FindPath(char water[][MAPY], char map[][MAPY], int x, int y, int X, int Y, int ring)
{
	int a, b;
	int stx[2][MAPX * MAPY], sty[2][MAPX * MAPY], top, top2, distance;
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(map[a][b] == GROUND || map[a][b] == STAR || map[a][b] == STAR2 || 
				map[a][b] == HEAD)
			{
				water[a][b] = 0;
			}
			else
			{
				water[a][b] = -1;
			}
		}
	}
	if(X != -1 && Y != -1)
	{
		water[X][Y] = -1;
	}
	top = 1;
	stx[1][0] = x;
	sty[1][0] = y;
	if(water[x][y] == -1)
		return ;
	water[x][y] = 1;
	distance = 2;
	top2 = 0;
	while(top)
	{
		for(a = 0; a < top; a++)
		{
			if(!ring)
			{
				if(stx[(distance + 1) % 2][a] - 1 >= 0)
				{
					if( water[stx[(distance + 1) % 2][a] - 1][sty[(distance + 1) % 2][a]] == 0)
					{
						water[stx[(distance + 1) % 2][a] - 1][sty[(distance + 1) % 2][a]] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a] - 1;
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
						top2++;
					}
				}
				if(sty[(distance + 1) % 2][a] - 1 >= 0)
				{
					if( water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] - 1] == 0)
					{
						water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] - 1] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a] - 1;
						top2++;
					}
				}
				if(sty[(distance + 1) % 2][a] + 1 < MAPY)
				{
					if( water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] + 1] == 0)
					{
						water[stx[(distance + 1) % 2][a]][sty[(distance + 1) % 2][a] + 1] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a] + 1;
						top2++;
					}
				}
				if(stx[(distance + 1) % 2][a] + 1 < MAPX)
				{
					if( water[stx[(distance + 1) % 2][a] + 1][sty[(distance + 1) % 2][a]] == 0)
					{
						water[stx[(distance + 1) % 2][a] + 1][sty[(distance + 1) % 2][a]] = distance;
						stx[distance % 2][top2] = stx[(distance + 1) % 2][a] + 1;
						sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
						top2++;
					}
				}
			}
			else
			{
				if( water[(stx[(distance + 1) % 2][a]-1+MAPX)%MAPX][sty[(distance + 1) % 2][a]] == 0)
				{
					water[(stx[(distance + 1) % 2][a]-1+MAPX)%MAPX][sty[(distance + 1) % 2][a]] = distance;
					stx[distance % 2][top2] = (stx[(distance + 1) % 2][a]-1+MAPX)%MAPX;
					sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
					top2++;
				}
				if( water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]-1+MAPY)%MAPY] == 0)
				{
					water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]-1+MAPY)%MAPY] = distance;
					stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
					sty[distance % 2][top2] = (sty[(distance + 1) % 2][a]-1+MAPY)%MAPY;
					top2++;
				}
				if( water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]+1)%MAPY] == 0)
				{
					water[stx[(distance + 1) % 2][a]][(sty[(distance + 1) % 2][a]+1)%MAPY] = distance;
					stx[distance % 2][top2] = stx[(distance + 1) % 2][a];
					sty[distance % 2][top2] = (sty[(distance + 1) % 2][a]+1)%MAPY;
					top2++;
				}
				if( water[(stx[(distance + 1) % 2][a]+1)%MAPX][sty[(distance + 1) % 2][a]] == 0)
				{
					water[(stx[(distance + 1) % 2][a]+1)%MAPX][sty[(distance + 1) % 2][a]] = distance;
					stx[distance % 2][top2] = (stx[(distance + 1) % 2][a]+1)%MAPX;
					sty[distance % 2][top2] = sty[(distance + 1) % 2][a];
					top2++;
				}
			}
		}
		top = top2;
		top2 = 0;
		distance++;
	}
	return ;
}

void F_Count_Blocks(char water[][MAPY], char map[][MAPY], int x, int y, int ring)
{
	int a, b;
	int stx[2][MAPX * MAPY], sty[2][MAPX * MAPY], top, top2, distance;
	int attribute[2][MAPX * MAPY];
	for(a = 0; a < MAPX; a++)
	{
		for(b = 0; b < MAPY; b++)
		{
			if(map[a][b] == GROUND || map[a][b] == STAR || map[a][b] == STAR2 || 
				map[a][b] == HEAD)
			{
				water[a][b] = 0;
			}
			else
			{
				water[a][b] = 1;
			}
		}
	}
	top = 0;
	/**
	stx[1][0] = x;
	sty[1][0] = y;/**/
	if(!ring)
	{
		if(x - 1 >= 0)
		{
			if(water[x - 1][y] == 0)
			{
				water[x - 1][y] = water[x - 1][y]|2;
				stx[0][top] = x - 1;
				sty[0][top] = y;
				attribute[0][top] = 2;
				top++;
			}
		}
		if(y - 1 >= 0)
		{
			if(water[x][y - 1] == 0)
			{
				water[x][y - 1] = water[x][y - 1]|4;
				stx[0][top] = x;
				sty[0][top] = y - 1;
				attribute[0][top] = 4;
				top++;
			}
		}
		if(y + 1 < MAPY)
		{
			if(water[x][y + 1] == 0)
			{
				water[x][y + 1] = water[x][y + 1]|8;
				stx[0][top] = x;
				sty[0][top] = y + 1;
				attribute[0][top] = 8;
				top++;
			}
		}
		if(x + 1 < MAPX)
		{
			if(water[x + 1][y] == 0)
			{
				water[x + 1][y] = water[x + 1][y]|16;
				stx[0][top] = x + 1;
				sty[0][top] = y;
				attribute[0][top] = 16;
				top++;
			}
		}
	}
	else
	{
		if(water[(x - 1 +MAPX)%MAPX][y] == 0)
		{
			water[(x - 1 +MAPX)%MAPX][y] = water[(x - 1 +MAPX)%MAPX][y]|2;
			stx[0][top] = (x - 1 +MAPX)%MAPX;
			sty[0][top] = y;
			attribute[0][top] = 2;
			top++;
		}
		if(water[x][(y - 1+MAPY)%MAPY] == 0)
		{
			water[x][(y - 1+MAPY)%MAPY] = water[x][(y - 1+MAPY)%MAPY]|4;
			stx[0][top] = x;
			sty[0][top] = (y - 1+MAPY)%MAPY;
			attribute[0][top] = 4;
			top++;
		}
		if(water[x][(y + 1)%MAPY] == 0)
		{
			water[x][(y + 1)%MAPY] = water[x][(y + 1)%MAPY]|8;
			stx[0][top] = x;
			sty[0][top] = (y + 1)%MAPY;
			attribute[0][top] = 8;
			top++;
		}
		if(water[(x + 1)%MAPX][y] == 0)
		{
			water[(x + 1)%MAPX][y] = water[(x + 1)%MAPX][y]|16;
			stx[0][top] = (x + 1)%MAPX;
			sty[0][top] = y;
			attribute[0][top] = 16;
			top++;
		}
	}
	water[x][y] = 1;
	top2 = 0;
	distance = 0;
	while(top)
	{
		/**
		gotoxy(0,0);
		printf("%d\n",distance); 
		for(a=0;a<top;a++) 
			printf("%d %d %d\n",stx[distance][a], sty[distance][a], attribute[distance][a]);
		getch(); /**/ 
		for(a = 0; a < top; a++)
		{
			if(!ring)
			{
				if(stx[distance][a] - 1 >= 0)
				{
					if(water[stx[distance][a] - 1][sty[distance][a]] != 1)
					{
						if( (water[stx[distance][a] - 1][sty[distance][a]] & attribute[distance][a]) == 0)
						{
							water[stx[distance][a] - 1][sty[distance][a]] = water[stx[distance][a] - 1][sty[distance][a]]|attribute[distance][a];
							stx[(distance + 1) % 2][top2] = stx[distance][a] - 1;
							sty[(distance + 1) % 2][top2] = sty[distance][a];
							attribute[(distance + 1) % 2][top2] = attribute[distance][a];
							top2++;
						}
					}
				}
				if(sty[distance][a] - 1 >= 0)
				{
					if(water[stx[distance][a]][sty[distance][a] - 1] != 1)
					{
						if( (water[stx[distance][a]][sty[distance][a] - 1] & attribute[distance][a]) == 0)
						{
							water[stx[distance][a]][sty[distance][a] - 1] = water[stx[distance][a]][sty[distance][a] - 1]|attribute[distance][a];
							stx[(distance + 1) % 2][top2] = stx[distance][a];
							sty[(distance + 1) % 2][top2] = sty[distance][a] - 1;
							attribute[(distance + 1) % 2][top2] = attribute[distance][a];
							top2++;
						}
					}
				}
				if(sty[distance][a] + 1 < MAPY)
				{
					if(water[stx[distance][a]][sty[distance][a] + 1] != 1)
					{
						if( (water[stx[distance][a]][sty[distance][a] + 1] & attribute[distance][a]) == 0)
						{
							water[stx[distance][a]][sty[distance][a] + 1] = water[stx[distance][a]][sty[distance][a] + 1]|attribute[distance][a];
							stx[(distance + 1) % 2][top2] = stx[distance][a];
							sty[(distance + 1) % 2][top2] = sty[distance][a] + 1;
							attribute[(distance + 1) % 2][top2] = attribute[distance][a];
							top2++;
						}
					}
				}
				if(stx[distance][a] + 1 < MAPX)
				{
					if(water[stx[distance][a] + 1][sty[distance][a]] != 1)
					{
						if( (water[stx[distance][a] + 1][sty[distance][a]] & attribute[distance][a]) == 0)
						{
							water[stx[distance][a] + 1][sty[distance][a]] = water[stx[distance][a] + 1][sty[distance][a]]|attribute[distance][a];
							stx[(distance + 1) % 2][top2] = stx[distance][a] + 1;
							sty[(distance + 1) % 2][top2] = sty[distance][a];
							attribute[(distance + 1) % 2][top2] = attribute[distance][a];
							top2++;
						}
					}
				}
			}
			else
			{
				if(water[(stx[distance][a] - 1+MAPX)%MAPX][sty[distance][a]] != 1)
				{
					if( (water[(stx[distance][a] - 1+MAPX)%MAPX][sty[distance][a]] & attribute[distance][a]) == 0)
					{
						water[(stx[distance][a] - 1+MAPX)%MAPX][sty[distance][a]] = water[(stx[distance][a] - 1+MAPX)%MAPX][sty[distance][a]]|attribute[distance][a];
						stx[(distance + 1) % 2][top2] = (stx[distance][a] - 1+MAPX)%MAPX;
						sty[(distance + 1) % 2][top2] = sty[distance][a];
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
				if(water[stx[distance][a]][(sty[distance][a] - 1+MAPY)%MAPY] != 1)
				{
					if( (water[stx[distance][a]][(sty[distance][a] - 1+MAPY)%MAPY] & attribute[distance][a]) == 0)
					{
						water[stx[distance][a]][(sty[distance][a] - 1+MAPY)%MAPY] = water[stx[distance][a]][(sty[distance][a] - 1+MAPY)%MAPY]|attribute[distance][a];
						stx[(distance + 1) % 2][top2] = stx[distance][a];
						sty[(distance + 1) % 2][top2] = (sty[distance][a] - 1+MAPY)%MAPY;
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
				if(water[stx[distance][a]][(sty[distance][a] + 1)%MAPY] != 1)
				{
					if( (water[stx[distance][a]][(sty[distance][a] + 1)%MAPY] & attribute[distance][a]) == 0)
					{
						water[stx[distance][a]][(sty[distance][a] + 1)%MAPY] = water[stx[distance][a]][(sty[distance][a] + 1)%MAPY]|attribute[distance][a];
						stx[(distance + 1) % 2][top2] = stx[distance][a];
						sty[(distance + 1) % 2][top2] = (sty[distance][a] + 1)%MAPY;
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
				if(water[(stx[distance][a] + 1)%MAPX][sty[distance][a]] != 1)
				{
					if( (water[(stx[distance][a] + 1)%MAPX][sty[distance][a]] & attribute[distance][a]) == 0)
					{
						water[(stx[distance][a] + 1)%MAPX][sty[distance][a]] = water[(stx[distance][a] + 1)%MAPX][sty[distance][a]]|attribute[distance][a];
						stx[(distance + 1) % 2][top2] = (stx[distance][a] + 1)%MAPX;
						sty[(distance + 1) % 2][top2] = sty[distance][a];
						attribute[(distance + 1) % 2][top2] = attribute[distance][a];
						top2++;
					}
				}
			}
		}
		top = top2;
		top2 = 0;
		distance = (distance + 1) % 2;
	}
	/**
	gotoxy(0,0);
	for(a = 0; a < MAPX;a++)
	{
		for(b = 0;b<MAPY;b++)
		{
			printf("%2d",water[a][b]);
		}
		cout<<endl;
	}
	pause();
	/**/
	return ;
}
int main(void)
{
	srand(time(NULL));
	FILE *fptr;
	class object MAP;
	class Snake snake;
	char mode;
	char sit;
	char water[MAPX][MAPY] = {0};
	int speed_list[] = {250,167,125,100,83,71,63,56,50,45,42,38,36,33,31,29,28,26,25,20,10,1};
	int bling_counter;
	int a, b;
	int author_display;
	int speed,lowest_speed;
	int move_permition; 
	
	
	
	welcome(&MAP);
	
	if(TEST) 
	{ 
		fptr=fopen("star.txt","a"); 
		fprintf(fptr,"========\n");
	} 
	
	MAP.setstar(water, mode, fptr);
	
	mode = 0;
	bling_counter = 0;
	author_display = 0;
	speed = 0;
	lowest_speed = 0;
	move_permition = 1; 
	while(1)
	{
		if(speed < 22)
		{
			Sleep( speed_list[speed] );
		}
		bling_counter += speed_list[speed];
		if(bling_counter>=300)
		{
			bling_counter = 0;
			if(MAP.map[MAP.starx][MAP.stary] == STAR)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR2, water, mode);
			}
			else if(MAP.map[MAP.starx][MAP.stary] == STAR2)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR, water, mode);
			}
		}
		if(kbhit() || mode)
		{
			sit = Input(mode, water, MAP, &snake);
			if(sit == AI_WORKING)
			{
				mode = (mode + 1) % 2;
				gotoxy(2 * MAPY + 8, 26);
				cout<<"─────";
				Sleep(50);
				gotoxy(2 * MAPY + 8, 26);
				if(mode == 0) 
				{
					cout<<" 普通模式 ";
				}
				else if(mode == 1)
				{
					cout<<" 自動尋徑 ";
				}
			}
			else if(sit == AUTHOR)
			{
				author_display = (author_display + 1) % 2;
				gotoxy(2 * MAPY + 4, 5);
				cout<<"　　   ──── ";
				Sleep(50);
				gotoxy(2 * MAPY + 4, 5);
				if(author_display == 0)
				{
					cout<<"　　   By pi314 ";
				}
				else
				{
					cout<<"　　   By 池昌言";
				}
			}
			else if(sit == OPEN_LIGHT)
			{
				MAP.open_light = (MAP.open_light + 1) % 2;
				if(MAP.open_light == 0)
				{
					for(a = 0; a < MAPX; a++)
					{
						for(b = 0; b < MAPY; b++)
						{
							MAP.monitor(a, b, water, mode);
						}
					}
				}
			}
			else if(sit == SPEED_UP)
			{
				speed = speed < 19 ? speed + 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"─";
				Sleep(30);
				printf("\b\b%2d", speed + 1);
				if(mode == 1)
				{
					sit = Input(mode, water, MAP, &snake);
				}
			}
			else if(sit == SLOW_DOWN)
			{
				speed = speed > lowest_speed ? speed - 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"─";
				Sleep(30);
				printf("\b\b%2d", speed + 1);
				if(mode == 1)
				{
					sit = Input(mode, water, MAP, &snake);
				}
			}
			else if(sit == FREEZE) 
			{
				move_permition = (move_permition + 1) % 2;
			} 
			snake.fixdir(sit);
		}
		if(move_permition)
		{ 
				sit = snake.move(MAP, water, mode); 
				FindPath(water, MAP.map, MAP.starx, MAP.stary, -1, -1, MAP.ring);
			if(sit == CRASH)
			{
				break;
			}
			else if(sit == EAT)
			{
				snake.length++;
				gotoxy(2 * MAPY + 12, 11);
				cout.width(3);
				cout<<snake.length;
				
				snake.score += (speed + 1) * 2 + (snake.length)/2;
  				gotoxy(2 * MAPY + 12, 13);
				cout.width(3);
  				cout<<snake.score;
  				if((snake.length - 3) % 7 == 0 && snake.length > 3)
  				{
					if(lowest_speed < 22 && TEST == 0)
					{ 
						lowest_speed++;
						if(speed < lowest_speed) 
						{
							speed = lowest_speed;
							gotoxy(2 * MAPY + 11, 12);
							cout<<"─";
							Sleep(30);
							printf("\b\b%2d", speed + 1);
						}
						if(lowest_speed == 20)
						{
							MAP.Colors[6] = MAP.Colors[7] = 12;
						}
						else if(lowest_speed == 21)
						{
							MAP.Colors[6] = MAP.Colors[7] = 11;
						}
						else if(lowest_speed == 22)
						{
							MAP.Colors[6] = MAP.Colors[7] = 13;
						}
					} 
				}
  				
  				if(snake.length < MAPX * MAPY)
  				{
					MAP.setstar(water, mode, fptr);
				}
			}
		}
	}
	if(TEST) 
	{ 
		fclose(fptr) ; 
	} 
	gotoxy(15,13);
	cout<<"遊戲結束，請按空白鍵結束遊戲。";
	do
	{
		if(speed < 22)
		{
			Sleep( speed_list[speed] );
		}
		bling_counter += speed_list[speed];
		if(bling_counter>=300)
		{
			bling_counter = 0;
			if(MAP.map[MAP.starx][MAP.stary] == STAR)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR2, water, mode);
			}
			else if(MAP.map[MAP.starx][MAP.stary] == STAR2)
			{
				MAP.fix(MAP.starx, MAP.stary, STAR, water, mode);
			}
		}
		if(kbhit())
		{
			a = getch();
			if(a == 224 || a == 0)
			{
				b = getch();
			}
			if(a == '`')
			{
				author_display = (author_display + 1) % 2;
				gotoxy(2 * MAPY + 4, 5);
				cout<<"　　   ──── ";
				Sleep(50);
				gotoxy(2 * MAPY + 4, 5);
				if(author_display == 0)
				{
					cout<<"　　   By pi314 ";
				}
				else
				{
					cout<<"　　   By 池昌言";
				}
			}
			else if(a == 27)
			{
				mode = (mode + 1) % 2;
				gotoxy(2 * MAPY + 8, 26);
				cout<<"─────";
				Sleep(50);
				gotoxy(2 * MAPY + 8, 26);
				if(mode == 1)
				{
					cout<<" 自動尋徑 ";
				}
				else
				{
					cout<<" 普通模式 ";
				}
			}
			else if(a == '+' || (a == 224 && b == 73))
			{
				speed = speed < 19 ? speed + 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"─";
				Sleep(50);
				printf("\b\b%2d", speed + 1);
			}
			else if(a == '-' || (a == 224 && b == 81))
			{
				speed = speed > lowest_speed ? speed - 1 : speed;
				gotoxy(2 * MAPY + 11, 12);
				cout<<"─";
				Sleep(50);
				printf("\b\b%2d", speed + 1);
			}
		}
	}while(a != ' ');
	gotoxy(0, 30);
	cout<<endl;
	cout<<"┌──11/21 ──────────┐\n";
	cout<<"│ Snake_III_0.exe  貪吃蛇 3.0  │\n";
	cout<<"└┬──────────────┴──────────┐\n";
	cout<<"  │http://www.cs.nctu.edu.tw/~cychih/Snake_III_0.exe │\n";
	cout<<"  └─────────────────────────┘\n\n";
	cout<<"特別感謝: ──────────────────────┐\n";
	cout<<"│2010/11/22 ex860抓出按下行進反方向會直接死亡的bug.  │\n";
	cout<<"│→ 修改為Snake_III_1.exe                            │\n";
	cout<<"└──────────────────────────┘\n\n";
	cout<<"特別感謝：───────────────────┐\n";
	cout<<"│在配色上採用CMturtle在2008年的作品“追閃蛇”. │\n";
	cout<<"├ - - - - - - - - - - - - - - - - - - - - - - -┤\n";
	cout<<"│11/26加強預防進入死巷的判斷能力。             │\n";
	cout<<"│11/29微幅加強走出死巷的能力。                 │\n";
	cout<<"│11/30加上計分系統。                           │\n";
	cout<<"│→ 修改為Snake_III_2.exe                      │\n";
	cout<<"└┬──────────────────────┴──┐\n";
	cout<<"　│http://www.cs.nctu.edu.tw/~cychih/Snake_III_2.exe │\n";
	cout<<"　└─────────────────────────┘\n\n";
	cout<<"┌──2011/01/15──────────────────┐\n";
	cout<<"│從01/14下午開始用class重寫，約半夜時玩家部分完成。│\n";
	cout<<"└─────────────────────────┘\n";
	cout<<"┌──2011/01/17───────────────┐\n";
	cout<<"│完成AI演算法的核心。加上切換模式時的小特效。│\n";
	cout<<"│→ Snake_IV_1.exe                           │\n";
	cout<<"└──────────────────────┘\n\n";
	cout<<"┌──2011/01/19──────────┐\n";
	cout<<"│加上更多小特效(隱藏的第21段速度)。│\n";
	cout<<"│→ Snake_IV_2.exe　　　　 　　　　│\n";
	cout<<"└─────────────────┘\n\n";
	cout<<"┌──2011/01/21─────────────────────┐\n";
	cout<<"│強化AI：　　　　　　　　　　　　　　　　　　　　　　　　│\n"; 
	cout<<"│　　當走入死路，則“能轉彎就轉彎”，以達成Ｕ字迴轉的效果│\n"; 
	cout<<"│    但若轉彎四次則改方向迴轉。(下一次改向為八次後)　　　│\n";
	cout<<"│→ Snake_IV_3.exe 　　　　　　　　　　　　　　　　　　　│\n";
	cout<<"└────────────────────────────┘\n\n";
	cout<<"┌──2011/01/24────────────────────┐\n";
	cout<<"│加上能完全繞行全地圖的AI，";
	printf("%c%c%c%c",-90,-88,-91,92); 
	cout<<"發動的話能把全地圖";
	cout<<(char)-69<<(char)92;
	cout<<"滿。│\n";
	cout<<"│→ Snake_IV_4.exe 　　　　　　　　　　　　　　　　　　│\n";
	cout<<"└───────────────────────────┘\n\n";
	cout<<"┌2011/01/26────────────────┐\n";
	cout<<"│加上“邊界開放"<<(char)-91<<(char)92<<(char)-81<<(char)-32;
	cout<<"”，也修改AI適應新"<<(char)-91<<(char)92<<(char)-81<<(char)-32<<"。│\n";
	cout<<"│　修改工作日誌，修掉已經失效的網址。　　　│\n";
	cout<<"└─────────────────────┘\n\n";
	cout<<"┌2011/02/26──────────────────┐\n";
	cout<<"│把數格子的步驟從乘法改為位元運算，以節省時間。│\n";
	cout<<"│把Snake_IV_5右上方顯示版本錯誤修正。          │\n";
	cout<<"│隱藏的速度加上22及23。                        │\n";
	cout<<"└┬──────────────────────┴─┐\n"; 
	cout<<"  │http://www.cs.nctu.edu.tw/~cychih/Snake_IV_6.exe│\n";
	cout<<"  └────────────────────────┘\n";
	cout<<"\n\n\n";
	cout<<"請按任意鍵結束遊戲 >>\n\n";
	pause();
	return 0;
}
