#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <conio.h>

#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'
#define stop 'p'

constexpr auto i_min = 35;
constexpr auto i_max = 85;
constexpr auto j_min = 17;
constexpr auto j_max = 23;

const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
struct snake* tail, * head, * food_1;
int i = 0;
int max;
int sleeptime = 200;
char click = 1;

struct snake {
	int x;
	int y;
	struct snake* next;
};

void color(int i);
void gotoxy(int x, int y);
void start_play();
void square(int i_min1, int i_max1, int j_min1, int j_max1);
void game_over(int k);
void make_map();
void text();
void init_snake();
void food();
void file_scanf();
void file_printf();
void gotodelete(int i, int y);
void change_body(int a, int b);
void gotoprint(int x, int y);
void MovingBody();
void ClickControl();
void eating();
int judge();
void add(char c);

int main(void)
{
	start_play();
	return 0;
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(handle, c);
}

void color(int i)
{
	SetConsoleTextAttribute(handle, i);
}

void start_play()
{
	int i, j;
	gotoxy(i_min + 20, j_min - 2);
	color(4);
	printf("贪 吃 蛇 游 戏");
	gotoxy(i_min + 5, j_min + 2);
	printf("[1]开始游戏\t\t  [2]游戏说明");
	gotoxy(i_min + 5, j_min + 4);
	printf("[3]退出游戏");
	color(3);
	square(i_min, i_max, j_min, j_max);
	gotoxy(i_min, j_max + 1);
	printf("请选择[1 2 3]:[ ]");
	gotoxy(i_min + 15, j_max + 1);
	scanf_s("%d", &i);
	switch (i)
	{
	case 1:
		system("cls");
		make_map();
		ClickControl();
		break;
	case 2:
		system("cls");
		text();
		gotoxy(20, 12);
		printf("[1]返回游戏   [2]提出游戏");
		gotoxy(20, 14);
		printf("请选择[1 2]:[ ]");
		gotoxy(33, 14);
		scanf_s("%d", &i);
		if (i == 1)
		{
			system("cls");
			make_map();
			ClickControl();
			break;
		}
	case 3:
		printf("正在退出游戏...");
		Sleep(500);
		system("cls");
		exit(0);
	default:
		gotoxy(i_min + 15, j_max + 2);
		printf("请输入1~3之间的数！！！");
		Sleep(300);
		getchar();
		system("cls");
		start_play();
	}

}

void square(int i_min1, int i_max1, int j_min1, int j_max1)
{
	int i, j;
	color(3);
	for (j = j_min1; j <= j_max1; j++)
	{
		for (i = i_min1; i <= i_max1; i++)
		{
			gotoxy(i, j);
			if (j == j_min1 || j == j_max1)
				printf("-");
			else if (i == i_min1 || i == i_max1)
				printf("|");
		}
		printf("\n");
	}
}

void game_over(int k)
{
	int j;
	file_scanf();
	system("cls");
	square(20, 100, 5, 20);
	gotoxy(55, 8);
	color(7);
	printf("(＞﹏＜)\n");
	color(6);
	gotoxy(44, 11);
	if (k == 0)
		printf("好可惜呀，你撞墙了，游戏结束\n");
	else
		printf("好可惜呀，你撞到自己了，游戏结束\n");
	gotoxy(54, 14);
	color(10);
	printf("你的得分是%d", i);
	gotoxy(38, 17);
	if (i < max)
		printf("（^_^）继续努力吧，你离最高分还差%d", max - i);
	else
	{
		printf("（^_^）太厉害了吧，你刷新了记录,现在最高分是%d", i);
		file_printf();
	}
	gotoxy(40, 22);
	printf("[1]在来一局\t\t\t[2]跑路了");
	gotoxy(52, 24);
	printf("请选择[1 2]:[ ]");
	gotoxy(65, 24);
	scanf_s("%d", &j);
	switch (j)
	{
	case 1:
		system("cls");
		make_map();
		ClickControl();
		break;
	case 2:
		gotoxy(65, 25);
		printf("正在退出游戏...");
		Sleep(500);
		system("cls");
		exit(0);
	}
}

void make_map()
{
	color(3);
	int i, j, i_min2 = 0, i_max2 = 56, j_min2 = 0, j_max2 = 26;
	for (j = j_min2; j <= j_max2; j++)
	{
		for (i = i_min2; i <= i_max2; i += 2)
		{
			gotoxy(i, j);
			if (j == j_min2 || j == j_max2)
				printf("■");
			else
			{
				if (i == i_min2 || i == i_max2)
					printf("■");
			}
		}
		printf("\n");
	}
	text();
	init_snake();
}

void text()
{
	file_scanf();
	color(3);
	gotoxy(60, 4);
	printf("\t\t\t\t( ^ω^)");
	gotoxy(76, 7);
	printf("   ☆最高分纪录:%d☆", max);
	color(6);
	gotoxy(76, 10);
	printf("   ☆当前得分:%d(^_^;)☆", i);
	gotoxy(76, 12);
	color(9);
	printf("\t   小提示");
	square(65, 110, 13, 26);
	gotoxy(68, 15);
	printf("tip1: 不能撞墙，不能咬到自己");
	gotoxy(68, 17);
	printf("tip2: 用w.s.a.d分别控制蛇的移动");
	gotoxy(68, 19);
	printf("tip3: F1 为加速，F2 为减速");
	gotoxy(68, 21);
	printf("tip4: 按空格键暂停游戏，再按空格键继续");
	gotoxy(68, 23);
	printf("tip5: Esc ：退出游戏");
	gotoxy(68, 25);
}

void init_snake()
{
	int i;
	//struct snake* tail,* head;
	tail = (snake*)malloc(sizeof(snake));
	tail->x = 22;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i < 5; i++)
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 22 + i * 2;
		head->y = 5;
		tail = head;
	}
	while (tail->next != NULL)
	{
		/*gotoxy(tail->x, tail->y);
		color(10);
		printf("◆");*/
		gotoprint(tail->x, tail->y);
		tail = tail->next;
	}
	food();
}

void food()
{
	srand((unsigned)time(NULL));
	struct snake* q;
	food_1 = (snake*)malloc(sizeof(snake));
	do
	{
		food_1->x = rand() % 52 + 2;
	} while ((food_1->x % 2) != 0); //食物随机出现，食物的x坐标在2~53
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y)
		{
			free(food_1);
			food();
		}
		q = q->next;
	}
	gotoxy(food_1->x, food_1->y);
	color(4);
	printf("●");
}

void file_scanf()
{
	FILE* fp;
	fopen_s(&fp, "./save.txt", "r+");
	if (fp != NULL)
		fscanf_s(fp, "%d", &max);
	fclose(fp);
}

void file_printf()
{
	FILE* fp;
	fopen_s(&fp, "./save.txt", "w+");
	if (fp != NULL)//判断文件打开是否成功
		fprintf(fp, "%d", i);
	fclose(fp);
}

void gotodelete(int i, int y)
{
	gotoxy(i, y);
	printf("  ");
}

void change_body(int a, int b)
{
	snake* p;
	p = head;
	int x, y, x1, y1;
	x = a;
	y = b;
	p = p->next;
	while (p->next != NULL)
	{
		x1 = p->x;
		y1 = p->y;
		p->x = x;
		p->y = y;
		x = x1;
		y = y1;
		p = p->next;
	}
}

void ClickControl()
{
	char c2='0';
	while (judge() == 0)
	{
		if (_kbhit())
		{
			click = _getch();
			c2 = _getch();
		}
		MovingBody();
		add(c2);
	}
}

void MovingBody()
{
	int a = head->x, b = head->y;
	snake* p = head;
	//通过先清空后打印实现动画效果
	while (p != NULL)
	{
		gotodelete(p->x, p->y);
		p = p->next;
	}
	switch (click)
	{
	case up:
		head->y -= 1;
		change_body(a, b);
		break;
	case down:
		head->y += 1;
		change_body(a, b);
		break;
	case left:
		head->x -= 2;
		change_body(a, b);
		break;
	case right:
		head->x += 2;
		change_body(a, b);
		break;
	case stop:
		break;
	}
	p = head;
	eating();

	while (p->next != NULL)
	{
		gotoprint(p->x, p->y);
		p = p->next;
	}
	p = head;
	Sleep(sleeptime);
}

void gotoprint(int x, int y)
{
	gotoxy(x, y);
	color(10);
	printf("◆");
}

void eating()
{
	snake* now, * p;
	if (food_1->x == head->x && food_1->y == head->y)
	{
		food();
		now = (snake*)malloc(sizeof(snake));
		p = head;
		while (p->next->next != NULL)
			p = p->next;

		now->next = p->next;
		p->next = now;
		i += 10;
		gotoxy(76, 10);
		printf("   ☆当前得分:%d(^_^;)☆", i);
		if (sleeptime >= 40)
			sleeptime -= 10;
	}
}

int judge()
{
	int i_min2 = 0, i_max2 = 56, j_min2 = 0, j_max2 = 26;
	snake* q;
	q = head->next;
	while (q != NULL)
	{
		if (head->x == q->x && head->y == q->y)
		{
			game_over(1);
			return 1;
		}
		q = q->next;
	}
	if (head->x == i_min2 || head->x == i_max2 || head->y == j_min2 || head->y == j_max2)
	{
		game_over(0);
		return 1;
	}
	else
		return 0;
}

void add(char c)
{
	if (c == 59)
		sleeptime -= 20;
	else if (c == 60)
		sleeptime += 20;
}