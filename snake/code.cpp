#include<stdio.h>
#include<conio.h>
#include <easyx.h>
#include<graphics.h>
#include <string>
#define MAX_SNAKE_NUM 500
#define WIDTH 640
#define HEIGHT 480
struct Pos
{
	int x;
	int y;
	DWORD color;
};
struct Snake
{
	int num;
	int dir;
	int scores;
	int size;
	int speed;
	//用数组记录坐标
	struct Pos coor[MAX_SNAKE_NUM];

}snake;

enum DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

//游戏状态的枚举类型
enum GameState 
{
	START,
	PLAYING,
	GAME_OVER
};

GameState gameState = START;
bool gameRunning = true;

struct Food
{
	int x;
	int y;
	DWORD color;
	bool flag;//判断是否被吃掉
}food;

//初始化
void GameInit()
{
	srand(GetTickCount());
	snake.num = 2;
	snake.dir = RIGHT;
	snake.scores = 0;
	snake.size = 10;
	snake.speed = 10;

	snake.coor[0].x = 20;
	snake.coor[0].y = 0;
	snake.coor[0].color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机生成颜色

	snake.coor[1].x = 10;
	snake.coor[1].y = 0;
	snake.coor[1].color = RGB(rand() % 256, rand() % 256, rand() % 256);//随机生成颜色

	//初始化食物
	food.x = rand() % (WIDTH/10)*10;
	food.y = rand() % (HEIGHT/10)*10;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.flag = true;

}

void ShowStartInterface() 
{
	cleardevice();
	settextcolor(YELLOW);
	settextstyle(48, 0, _T("宋体"));
	outtextxy(WIDTH / 2 - 150, HEIGHT / 3, _T("贪吃蛇游戏"));

	settextstyle(24, 0, _T("宋体"));
	settextcolor(LIGHTGREEN);
	outtextxy(WIDTH / 2 - 120, HEIGHT / 2, _T("按任意键开始游戏"));

	settextstyle(16, 0, _T("宋体"));
	settextcolor(LIGHTBLUE);
	outtextxy(WIDTH -50, HEIGHT-20, _T("jc&llr"));
	
	FlushBatchDraw();

	
	while (!_kbhit()) 
	{
		Sleep(100);
	}
	_getch(); // 清除按键
	gameState = PLAYING;
}

// 显示游戏结束界面
void ShowGameOver() 
{
	cleardevice();
	settextstyle(48, 0, _T("宋体"));
	settextcolor(RED);
	outtextxy(WIDTH / 2 - 150, HEIGHT / 3, _T("游戏结束!"));

	char scoreText[50];
	sprintf(scoreText, "最终分数: %d", snake.scores);
	settextstyle(24, 0, _T("宋体"));
	settextcolor(WHITE);
	outtextxy(WIDTH / 2 - 120, HEIGHT / 2, scoreText);

	settextstyle(20, 0, _T("宋体"));
	settextcolor(LIGHTGRAY);
	outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 60, _T("按 R 重新开始"));
	outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 100, _T("按 ESC 退出游戏"));

	FlushBatchDraw();
	// 等待用户输入
	while (true) 
	{
		if (_kbhit()) 
		{
			char key = _getch();
			if (key == 'r' || key == 'R') 
			{
				gameState = PLAYING;
				gameRunning = true;
				return;
			}
			else if (key == 27) 
			{
				gameRunning = false;
				return;
			}
		}
		Sleep(100);
	}
}

// 检查碰撞
bool CheckCollision() 
{
	// 检查边界碰撞
	if (snake.coor[0].x < 0 || snake.coor[0].x >= WIDTH ||snake.coor[0].y < 0 || snake.coor[0].y >= HEIGHT) 
	{
		return true;
	}

	// 检查自身碰撞
	for (int i = 3; i < snake.num; i++) 
	{
		if (snake.coor[0].x == snake.coor[i].x &&snake.coor[0].y == snake.coor[i].y) 
		{
			return true;
		}
	}

	return false;
}

void GameDraw()
{
	cleardevice();
	/*有网格太丑了，遂注释掉
	setlinecolor(RGB(50, 50, 50));
	for (int x = 0; x < WIDTH; x += 10) 
	{
		line(x, 0, x, HEIGHT);
	}
	for (int y = 0; y < HEIGHT; y += 10) 
	{
		line(0, y, WIDTH, y);
	}
	*/
	//画蛇
	setfillcolor(snake.coor[0].color);
	for (int i = 0; i < snake.num; i++)
	{
		setfillcolor(snake.coor[i].color);
		fillrectangle(snake.coor[i].x, snake.coor[i].y, snake.coor[i].x + 10, snake.coor[i].y + 10);
	}
	// 画食物
	if (food.flag)
	{
		setfillcolor(food.color);
		solidellipse(food.x, food.y, food.x + 10, food.y + 10);
	}
	//显示信息
	char temp[20];
	sprintf(temp, "分数:%d", snake.scores);
	outtextxy(10, 10, temp);
	sprintf(temp, "长度: %d", snake.num);
	outtextxy(10, 40, temp);
}

void GameMove()
{
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.coor[i].x = snake.coor[i - 1].x;
		snake.coor[i].y = snake.coor[i - 1].y;
	}


	switch (snake.dir)
	{
	case UP:
		snake.coor[0].y -= snake.speed;
		break;
	case DOWN:
		snake.coor[0].y += snake.speed;
		break;
	case LEFT:
		snake.coor[0].x -= snake.speed;
		break;
	case RIGHT:
		snake.coor[0].x += snake.speed;
		break;
	}

}

void GameControl()
{
	char key = _getch();

	switch (key)
	{
	case 72:
		if (snake.dir != DOWN)
			snake.dir = UP;
		break;
	case 80:
		if (snake.dir != UP)
			snake.dir = DOWN;
		break;
	case 75:
		if (snake.dir != RIGHT)
			snake.dir = LEFT;
		break;
	case 77:
		if (snake.dir != LEFT)
			snake.dir = RIGHT;
		break;

	}
	printf("%d\n", key);
}

void CreateFood()
{
	if (!food.flag)
	{
		food.x = rand() % (WIDTH / 10) * 10;
		food.y = rand() % (HEIGHT / 10) * 10;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.flag = true;
	}
}

void Eat()
{
	int headX = snake.coor[0].x;
	int headY = snake.coor[0].y;
	int foodX = food.x;
	int foodY = food.y;
	int size = snake.size;
	
	if (food.flag &&
		headX < foodX + size &&
		headX + size > foodX &&
		headY < foodY + size &&
		headY + size > foodY) 
	{
		food.flag = false;
		snake.num++;
		snake.scores += 10;
		snake.coor[snake.num - 1].x = snake.coor[snake.num - 2].x;
		snake.coor[snake.num - 1].y = snake.coor[snake.num - 2].y;
		snake.coor[snake.num - 1].color = RGB(rand() % 256, rand() % 256, rand() % 256);

		CreateFood();
	}
}

//主程序
int main()
{
	initgraph(WIDTH, HEIGHT, 1);
	setbkcolor(RGB(207, 214, 229));
	cleardevice();

	ShowStartInterface();
	GameInit();

	while (gameRunning)
	{
		if (gameState == PLAYING) 
		{
			
			BeginBatchDraw();
			GameDraw();
			FlushBatchDraw();
			EndBatchDraw();
			
			if (_kbhit()) {
				GameControl();
			}
	
			GameMove();
			if (CheckCollision()) 
			{
				gameState = GAME_OVER;
			}
			
			Eat();
			//越长越快更好喔
			int delay = 100 - (snake.scores / 10);
			if (delay < 10) delay = 10;
			Sleep(delay);
		}
		else if (gameState == GAME_OVER) 
		{
			ShowGameOver();
			if (gameRunning) 
			{
				GameInit();
			}
		}
	}

	closegraph();
	return 0;
}