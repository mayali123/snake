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
#define add_speed 'j'
#define down_speed 'l'

constexpr auto i_min = 35;                                     //用于游戏开始边框的确定 
constexpr auto i_max = 85;
constexpr auto j_min = 17;
constexpr auto j_max = 23;

const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);         //标准输出的句柄
struct snake* tail, * head, * food_1;
int i = 0;                                                     //用来保存成绩
int max;                                                       //成绩的最大值
int sleeptime = 200;                                           //通过sleeptime来控制速度
char click = 1;                                                //保存键盘的输入
int rand_color = 3;                                            //保存产生的随机数，将食物的颜色变成随机的
int last_color = 3;                                            //保存上一次产生随机数

struct snake {
	int x;
	int y;
	struct snake* next;                                         //为使用链表做准备
};   //蛇的结构体

void color(int i);                                              //通过这个函数来改变字体的颜色
void gotoxy(int x, int y);                                      //将光标移到(x,y)处
void start_play();                                              //开始游戏
void square(int i_min1, int i_max1, int j_min1, int j_max1);    //产生一个矩形框
void game_over(int k);                                          //游戏结束
void make_map();                                                //加载地图
void text();                                                    //加载文本
void init_snake();                                              //初始化蛇的身体
void food();                                                    //初始化食物
void file_scanf();                                              //读入save.txt保存成绩的最大值
void file_printf();                                             //将最大值写入save.txt
void gotodelete(int i, int y);                                  //清空打出来的蛇身体
void change_body(int a, int b);                                 //改变蛇身体位置
void gotoprint(int x, int y);                                   //打印蛇
void MovingBody();                                              //移动蛇
void ClickControl();                                            //得到键盘的输入
void eating();                                                  //当蛇吃到东西时
int judge();                                                    //判断蛇是否撞到东西
void refresh();                                                 //刷新成绩和速度

int main(void)
{
	start_play();
	return 0;
}

void gotoxy(int x, int y)
{
	COORD c;                                                     //COORD是一个结构体
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(handle, c);                         //设置光标的位置
}

void color(int i)
{
	SetConsoleTextAttribute(handle, i);                           //改变字体的颜色
}

void start_play()
{
	int i, j;
	gotoxy(i_min + 20, j_min - 2);                                //加载游戏开始的界面
	color(4);
	printf("贪 吃 蛇 游 戏");
	gotoxy(i_min + 5, j_min + 2);
	printf("[1]开始游戏\t\t  [2]游戏说明");
	gotoxy(i_min + 5, j_min + 4);
	printf("[3]退出游戏");
	color(3);
	square(i_min, i_max, j_min, j_max);                           //打出矩形框
	gotoxy(i_min, j_max + 1);
	printf("请选择[1 2 3]:[ ]");
	gotoxy(i_min + 15, j_max + 1);
	scanf_s("%d", &i);
	switch(i)                                                     //对选择进行判断
	{ 
		case 1:                                                  
			system("cls");                                        //清屏
			make_map();
			ClickControl();
			break;
		case 2:
			system("cls");
			text();
			gotoxy(20,12);
			printf("[1]返回游戏   [2]退出游戏");
			gotoxy(20, 14);
			printf("请选择[1 2]:[ ]");
			gotoxy(33, 14);
			scanf_s("%d",&i);
			if(i==1)
			{ 
				system("cls");
				make_map();
				ClickControl();
				break;
			}                                                      //如果i为2，则进入case3中
		case 3:
			printf("正在退出游戏...");
			Sleep(500);                                            //产生延迟效果，使玩家看清楚打印的内容
			system("cls");
			exit(0);
		default:
			gotoxy(i_min + 15, j_max + 2);
			printf("请输入1~3之间的数！！！");
			Sleep(300);
			getchar();                                             //吸收按下的回车
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
	file_scanf();                                                    //读入save.txt保存成绩的最大值
	system("cls");
	square(20, 100, 5, 20);
	gotoxy(55, 8);
	color(7);
	printf("(＞﹏＜)\n");
	color(6);
	gotoxy(44, 11);
	if(k==0)                                                         //判断游戏结束的原因
		printf("好可惜呀，你撞墙了，游戏结束\n");
	else
		printf("好可惜呀，你撞到自己了，游戏结束\n");
	gotoxy(54, 14);
	color(10);
	printf("你的得分是%d", i);
	gotoxy(38, 17);
	if (i < max)                                                      //判断成绩是否大于最大值
		printf("（^_^）继续努力吧，你离最高分还差%d", max - i);
	else
	{
		printf("（^_^）太厉害了吧，你刷新了记录,现在最高分是%d", i);
		file_printf();                                               //成绩大于最大值，将成绩写入文件
	}
	gotoxy(40, 22);
	printf("[1]在来一局\t\t\t[2]跑路了");
	gotoxy(52, 24);
	printf("请选择[1 2]:[ ]");
	gotoxy(65, 24);
	scanf_s("%d",&j);
	switch (j)                                                         //对选择进行判断
	{
	case 1:
		system("cls");
		i = 0;                                                      //为重玩做准备，将成绩和速度变成初始值
		sleeptime = 200;                                               
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
	for (j = j_min2; j <= j_max2; j++)                                 //加载地图
	{
		for (i = i_min2; i <= i_max2; i+=2)
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
	text();                                                          //加载文字
	init_snake();                                                    //初始化蛇身
}

void text()
{
	file_scanf();                                                    
	color(3);
	gotoxy(60, 4);
	printf("\t\t\t\t( ^ω^)");
	gotoxy(76, 7);
	printf("   ☆最高分纪录:%d☆", max);                              //打印最高分
	color(6);
	refresh();                                                        //因为成绩和速度会变化所以要在其改变时刷新
	gotoxy(76, 14);
	color(9);
	printf("\t   小提示");                                            //打印一些提示
	square(65, 115, 15, 28);
	gotoxy(66, 17);
	printf("tip1: 不能撞墙，不能咬到自己");
	gotoxy(66, 19);
	printf("tip2: 用英文的w.s.a.d分别控制蛇的移动");
	gotoxy(66, 21);
	printf("tip3: 按空格键暂停游戏,需要再次按w.s.a.d才可以动");
	gotoxy(66, 23);
	printf("top4: 按j加速l降速,需要再次按w.s.a.d才可以动");
	gotoxy(66, 25);
	printf("top5:  开始时蛇的头在右边");
	gotoxy(66, 27);
	printf("由于技术技术原因本游戏存在着一些bug请见谅");
	gotoxy(66, 29);
}

void init_snake()
{
	int i,k,j;
	srand((unsigned)time(NULL));                                      //重新播种，有利于产生随机数
	tail = (snake*)malloc(sizeof(snake));
	do
	{
		k= rand() % 46+2;                                             
	} while ((k % 2) != 0);						  //蛇头x坐标在2~47，且为偶数
	j= rand() % 22 + 1;                                               //蛇头y坐标在2~47
	tail->x = k;
	tail->y = j;                                                      //实现贪吃蛇的随机出现
	tail->next = NULL;
	for (i = 1; i < 5; i++)                                           //产生四个蛇身体
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = k + i * 2;
		head->y = j;
		tail = head;
	}
	while (tail->next != NULL)                                        //打印蛇
	{
		gotoprint(tail->x, tail->y);
		tail = tail->next;
	}
	food();                                                           //加载食物
}

void food()
{
	srand((unsigned)time(NULL));                                      
	struct snake * q;
	food_1 = (snake*)malloc(sizeof(snake));
	do
	{
		food_1->x = rand() % 52 + 2;
	} while ((food_1->x % 2) != 0);                                   //食物随机出现，食物的x坐标在2~53，且为偶数
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y)               //判断食物是不是在蛇的身体上
		{
			free(food_1);
			food();
		}
		q = q->next;
	}
	last_color = rand_color;                                          //通过这个来实现蛇吃个什么颜色的食物就变什么颜色
	srand((unsigned)time(NULL));
	color(rand_color=rand() % 10 + 1);                                //随机产生1~10的颜色
	gotoxy(food_1->x, food_1->y);                                     //打印食物
	printf("●");
}

void file_scanf()
{
	FILE* fp;
	fopen_s(&fp, "./save.txt", "r+");
	if (fp != NULL)                                                   //判断文件打开是否成功
		fscanf_s(fp, "%d", &max);
	fclose(fp);
}

void file_printf()
{
	FILE* fp;
	fopen_s(&fp, "./save.txt", "w+");                                 //这个会覆盖之前的值
	if (fp != NULL)													  
		fprintf(fp, "%d", i);
	fclose(fp);
}
 
void gotodelete(int i, int y)                                         
{
	gotoxy(i, y);
	printf("  ");
}

void change_body(int a,int b)                             
{
	snake* p;
	p = head;
	int x, y, x1, y1;
	x = a;
	y = b;
	p = p->next;
	while (p->next != NULL)                                            //用这将head后面的移动到他前一个的位置上
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
	click = 'p';                                                        //为重玩做准备
	while (judge()==0)                                                  //判断是否撞到东西
	{
		if (_kbhit())                                               //判断是否有键盘按下
		{
			click = _getch();
		}
		MovingBody();
	}

}

void MovingBody()
{
	int a = head->x, b = head->y;
	snake* p = head;
	while (p-> != NULL)                                                   //通过先清空后打印实现动画效果
	{
		gotodelete(p->x, p->y);
		p = p->next;
	}
	switch (click)                                                      //对按下的键盘进行判断
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
	case add_speed:                                                       //通过改变sleeptime，达到加速
		if(sleeptime>=50)
		{
			sleeptime -= 10;
			i += 5; 
		}
		refresh();
		break;
	case down_speed:
		if(sleeptime <=200)
		{ 
			sleeptime += 10;
			i -= 5;
		}
		refresh();
		break;
	}
	p = head;
	eating();                                                              //看蛇是否吃到东西
	while (p->next != NULL)                                                //打印蛇
	{
		gotoprint(p->x, p->y);
		p = p->next;
	}
	p = head;
	gotoxy(0, 28);                                                         //让输入的光标在(0,28)处
	Sleep(sleeptime);
}

void gotoprint(int x, int y)
{
	color(last_color);
	gotoxy(x, y);
	printf("◆");
}

void eating()
{
	snake* now,*p;
	if (food_1->x == head->x && food_1->y == head->y)                        //判断蛇是否吃到食物
	{
		food();                                                     
		now = (snake*)malloc(sizeof(snake));
		p = head;
		while (p->next->next != NULL)
			p = p->next;
		now->next = p->next;
		p->next = now;
		i+=10;                                                              //当吃到东西时，成绩+10，速度增加
		if (sleeptime >= 40)                                                
			sleeptime -= 10;
		refresh();                                                          //对成绩和速度进行刷新
	}
}

int judge()
{
	int i_min2 = 0, i_max2 = 56, j_min2 = 0, j_max2 = 26;
	snake* q;
	q = head->next;
	while (q->next != NULL)
	{
		if (head->x == q->x && head->y == q->y)
		{
			game_over(1);                                                   //1代表撞到自己了
			return 1;
		}
		q = q->next;
	}
	if (head->x == i_min2 || head->x == i_max2 || head->y == j_min2 || head->y == j_max2)
	{
		game_over(0);                                                           //0代表撞到墙了
		return 1;
	}
	else
		return 0;
}

void refresh()
{
	color(i / 10);
	gotoxy(76, 10);
	printf("   ☆当前得分:%d(^_^;)☆", i);
	gotoxy(76, 12);
	printf("  ☆当前速度:%d(⌒_⌒;)☆", (21 - sleeptime / 10));
}
