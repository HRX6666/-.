#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define UP 0x48
#define DOWN 0x50 
#define LEFT 0x4B
#define RIGHT 0x4D
#define PAUSE ' '
#define WIDTH 52
#define HEIGHT 27
#define NAME_SIZE 128
#define MAX_PLAYER 1024
#define MAX_OBT 4

//�궨����ɫ
#define cAqua 11
#define cGreen 10
#define cRed 12
#define cFuchsia 13
#define cYellow 14


// ����ʵ��̰��������
typedef struct Snake
{
	int x;
	int y;
	struct Snake *next;
}Snake;
//  ��ʳ�������ͷ�������һ���ṹ�����棬ʳ��ķ����ߵ����ѶȾ���
typedef struct food
{
	int x;
	int y;
	int score;
}Food;
//�û���Ϣ�Ľṹ��
typedef struct playerInfo 
{
	char name[NAME_SIZE];
	int high_score;
	int play_times;
}Player;

typedef struct obstacle
{
	int x;
	int y;
}Obstacle;
//������ɫ�ĺ���
void setColors(unsigned short b){
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,b);
} 
// ************************************************************************
// * 
// * ��Ϸ�е�ȫ�ֱ���
// *
// ************************************************************************
char cGameScene[HEIGHT][WIDTH];             // �����Ϸ������СΪ 50 x 25 
char cUserName[NAME_SIZE];
int iTotalScore;
int iLastScore;
int iTotalTimes = 0;
int iDifficulty = 1;
int iRefreshTimes[4] = { 50, 25, 5, 0 };
int iGameOver = 0;
int iFoodSpawned = 0;
char cControl;
Snake *head, *tail;
Food food;
int iCountObt = 1;
Obstacle *obstacles;
// ************************************************************************
// * 
// * ��������
// *
// ************************************************************************
void initGame();             // ��ʼ����Ϸ��������ȫ�ֱ���
void refreshScreen();        // ÿһ֡����Ļˢ��
void gameLoop();             // ��Ϸ�߼���ѭ��
void enterUsername();                            // �ս�����Ϸʱ��ѡ�����½����
void startMenu();                                // ��ʼ��Ϸ���ѡ��˵�
void chooseDifficulty();                         // ѡ����Ϸ�Ѷ�
void myInfo();                                   // ��ʾ�ҵ���Ϣ
void gameOver();                                 // ��Ϸ����ҳ��
void updateSnakeShape();
int randint(int min, int max);
int ifGameOver();
void spawnFood();
void gotoxy(int x, int y);
int main(void)
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	srand((unsigned int)time(0));
	enterUsername();
	while (1)
	{
		startMenu();
		
		system("cls");
		initGame();
		gameLoop();
		gameOver();
		system("cls");
	}
	return 0;
}

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ��׼����豸���
	SetConsoleCursorPosition(hOut, pos);//���������ֱ���ָ���ĸ����壬����λ��
}

int randint(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void enterUsername()
{
	int iSelect;
	char cSearchName[NAME_SIZE], cName[NAME_SIZE];
	FILE *fp;
	int iScore;
	int iFound = 0;
	system("cls");
	setColors(cYellow); 
	printf("====================================================\n");
	printf("$                                                  $\n");
	printf("$                                                  $\n");
	printf("$                  1. ���������                   $\n");
	printf("$                                                  $\n");
	printf("$                                                  $\n");
	printf("====================================================\n");
	printf("                  ѡ�"); 
	scanf("%d", &iSelect);
	getchar();
	fflush(stdin);
	if (iSelect == 1)
	{
		system("cls");
		printf("\t\t�����û�����");
		gets(cUserName);
	}
	else
	{
		printf("                    ѡ�����\n");
		printf("                    ����������س�ʼ�˵�\n");
		char ch = _getch();
		enterUsername();
	}
}

void startMenu()
{
	int iSelect;

	system("cls");
	setColors(cYellow); 
	printf("====================================================\n");
	printf("$                                                  $\n");
	printf("$                  ̰���ߴ���ս                    $\n");
	printf("$                                                  $\n");
	printf("$                1. ��ʼ��Ϸ                       $\n");
	printf("$                2. �˳���Ϸ                       $\n");
	printf("$                                                  $\n");
	printf("$                                                  $\n");
	printf("====================================================\n");
	printf("                  ѡ�");
	scanf("%d", &iSelect);
	system("cls");
	if (iSelect == 1)
	{
		chooseDifficulty();
		iTotalTimes += 1;
		setColors(cRed); 
		printf("                  Good Luck ! ! !\n");
		Sleep(2000);
	}
	else if (iSelect == 2)
	{
		setColors(cGreen); 
		printf("                    ��������˳���Ϸ\n");
		char ch = _getch();
		exit(1);
	}
	else
	{
		setColors(cGreen); 
		printf("                    ѡ�����\n");
		printf("                    ����������ؿ�ʼ����\n");
		fflush(stdin);
		char ch = _getch();
		startMenu();
	}
}

void chooseDifficulty()
{
	int iSelect;
	setColors(cYellow); 
	printf("           1. ��  2. ��ͨ  3. ����  4. ج��\n");
	printf("                  ѡ�");
	scanf("%d", &iSelect);

	if (iSelect <= 4 && iSelect >= 1)
	{
		iDifficulty = iSelect;
		iCountObt = iDifficulty;
	}
	else
	{
		setColors(cGreen); 
		printf("             ����ȷ����ѡ��\n");
		printf("                    ���������ѡ\n");
		char ch = _getch();
		chooseDifficulty();
	}
}
void gameOver()
{
	setColors(cYellow);
	system("cls");
	printf("====================================================\n\n\n\n");
	printf("                      GAME OVER                     \n\n");
	printf("                  �ܷ֣�%4d\n\n\n", iTotalScore);
	printf("====================================================\n");
	
	if (iTotalScore > iLastScore)
	{
		iLastScore = iTotalScore;
	}
	printf("                    ����������ؿ�ʼ����\n");
	char ch = _getch();
	iFoodSpawned = 0;
	iTotalScore = 0;
}

void initGame()
{
	int i, j;
    setColors(cYellow);
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
			{
				cGameScene[i][j] = '#';
			}
			else
			{
				cGameScene[i][j] = ' ';
			}
		}
	}
	head = (Snake*)malloc(sizeof(Snake));
	head->x = 24;
	head->y = 12;
	head->next = (Snake*)malloc(sizeof(Snake));
	head->next->x = 23;
	head->next->y = 12;
	head->next->next = NULL;
	tail = head->next;
	cGameScene[head->y][head->x] = 'X';
	cGameScene[tail->y][tail->x] = 'O';
	if (iDifficulty >= 3)
	{
		obstacles = (Obstacle*)malloc(sizeof(Obstacle) * MAX_OBT);
		obstacles[0].x = 12;
		obstacles[0].y = 6;
		obstacles[1].x = 12;
		obstacles[1].y = 18;
		obstacles[2].x = 36;
		obstacles[2].y = 6;
		obstacles[3].x = 36;
		obstacles[3].y = 18;
		for (i = 0; i < MAX_OBT; i++)
		{
			int k;
			for (j = 0; j < 2; j++)
			{
				for (k = 0; k < 2; k++)
				{
					cGameScene[obstacles[i].y + j][obstacles[i].x + k] = '#';
				}
			}
		}
	}
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			putchar(cGameScene[i][j]);
		}
		putchar('\n');
	}
}

void refreshScreen()
{
	int i, j;

	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			gotoxy(j, i);
			putchar(cGameScene[i][j]);
		}
	}
	gotoxy(WIDTH + 5, 12);
	setColors(cFuchsia); 
	printf("�÷֣�%d", iTotalScore);
}

void updateSnakeShape()
{
	Snake *node = head;
	setColors(cRed);
	int x, y, preX = head->x, preY = head->y;
	int i, j;
	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			if (cGameScene[i][j] != '*' && cGameScene[i][j] != '#')
			{
				cGameScene[i][j] = ' ';
			}
		}
	}
	if (head->next != NULL && (cControl == UP || cControl == DOWN || cControl == LEFT || cControl == RIGHT))
	{
		while (node != NULL)
		{
			x = node->x;
			y = node->y;
			node->x = preX;
			node->y = preY;
			preX = x;
			preY = y;
			node = node->next;
		}
	}
	node = head->next;
	while (node != NULL)
	{
		cGameScene[node->y][node->x] = 'O';
		node = node->next;
	}
}

int ifGameOver()
{
	Snake *node = head->next;
	int i;

	if (head->x == WIDTH - 1 || head->x == 0 || head->y == HEIGHT - 1 || head->y == 0)
	{
		return 1;
	}
	if (iDifficulty >= 3)
	{
		for (i = 0; i < MAX_OBT; i++)
		{
			int j, k;
			for (j = 0; j < 2; j++)
			{
				for (k = 0; k < 2; k++)
				{
					if (head->x == obstacles[i].x + k && head->y == obstacles[i].y + j)
						return 1;
				}
			}
		}
	}
	while (node != NULL)
	{
		if (head->x == node->x && head->y == node->y)
		{
			return 1;
		}
		node = node->next;
	}

	return 0;
}

void spawnFood()
{ 
setColors(cAqua);
	food.x = randint(1, WIDTH - 2);//ʳ��������� 
	food.y = randint(1, HEIGHT - 2);
	food.score = iDifficulty;
    
	Snake *node = head;
	while (node != NULL)
	{
		if ((node->x == food.x && node->y == food.y) || (cGameScene[food.y][food.x] == '#'))
		{
			food.x = randint(1, WIDTH - 2);
			food.y = randint(1, HEIGHT - 2);
			node = head;
			continue;
		}
		node = node->next;
	}
	setColors(cGreen);
	cGameScene[food.y][food.x] = '*';
	iFoodSpawned = 1;
}

void gameLoop()
{
	 setColors(cYellow);
	while (!ifGameOver())
	{
		Sleep(iRefreshTimes[iDifficulty - 1]);
		if (_kbhit())
		{
			cControl = _getch();
		}
		updateSnakeShape();

		switch (cControl)
		{
		case UP: head->y -= 1; break;
		case DOWN: head->y += 1; break;
		case LEFT: head->x -= 1; break;
		case RIGHT: head->x += 1; break;
		default: break;
		}
		cGameScene[head->y][head->x] = 'X';

		if (iFoodSpawned == 0)
		{
			spawnFood();
		}

		if (head->x == food.x && head->y == food.y)
		{
			iTotalScore += iDifficulty + 1;
			tail->next = (Snake*)malloc(sizeof(Snake));
			switch (cControl)
			{
			case UP: tail->next->y = tail->y + 1; tail->next->x = tail->x; break;
			case DOWN: tail->next->y = tail->y - 1; tail->next->x = tail->x; break;
			case LEFT: tail->next->y = tail->y; tail->next->x = tail->x + 1; break;
			case RIGHT: tail->next->y = tail->y; tail->next->x = tail->x - 1; break;
			}
			tail = tail->next;
			tail->next = NULL;
			iFoodSpawned = 0;
		}
		refreshScreen();
	}
}


