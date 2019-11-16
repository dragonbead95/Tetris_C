/*�����ڷ�
http://blog.naver.com/PostView.nhn?blogId=dnpc7848&logNo=220367009691&parentCategoryNo=&categoryNo=&viewDate=&isShowPopularPosts=false&from=postView
*/
/*
ver02
�߰����� 1 : ���� ������ ���� �ٽý��� ��� �߰�
�߰����� 2: pŰ�� ������ �Ͻ����� ��� �߰�
�߰����� 3: ESCŰ ������ �������� ��� �߰�
�߰����� 4: ��4/1���, ��������� �� �߰�
�߰����� 5: Ÿ�ϸ�� ���� ��� �߰�
�߰����� 6: �ؽ�Ʈ �� �߰�
�߰����� 7: ������ �������� �߰�
�߰����� 8: ����ð� �߰�
*/
#include <Turboc.h>
#include <conio.h>
#include <Windows.h>
#include <thread>

using std::thread;


#define LEFT 75		//����
#define RIGHT 77	//������
#define UP 72		//����
#define DOWN 80		//�Ʒ���
#define ESC 27		//���
#define BX 5
#define BY 1
#define BW 10	//������ ����
#define BH 20	//������ ����
#define PGUP 73
#define PGDN 81

void DrawScreen();								//ȭ����ü�� �׸���. �����ǰ� ���� �̸�, ������ �Ѳ����� �׸���.
void DrawBoard();								//�����Ǹ� �׸���. �� �ܺκ��� ���ڿ����� ���� �׿��ִ� ������ �׸���.

BOOL ProcessKey();								/*Ű �Է��� ó���ϴµ� main�Լ��� �δ��� �����ֱ� ���� ������ �Լ��� �и��س��Ҵ�.
												�̵����� ������ �ٴڿ� ������ TRUE�� �����Ѵ�.*/

void PrintBrick(BOOL Show);						//������ ����ϰų� �����ϴµ� �̵����� ������ ������� �ϹǷ� �������� brick, rot, nx, ny���� �����Ѵ�.

int GetAround(int x, int y, int b, int r);		/*���� �ֺ��� ������ �ִ��� �˻��Ͽ� ������ �̵� �� ȸ�� ���ɼ��� �����Ѵ�.
												�̵����� ������ �ֺ��� �����ϴ� ���� �ƴϹǷ� �μ��� ���޵� ��ġ�� ���� ����� �����Ѵ�.*/
BOOL MoveDown();								//������ ��ĭ �Ʒ��� �̵���Ų��. ���� �ٴڿ� ��Ҵٸ� TestFull �Լ��� ȣ���� �� TRUE�� �����Ѵ�.
void TestFull();								//�������� �� ä���� ���� ã�� �����Ѵ�.
void DrawNext();								//�ؽ�Ʈ ���� �����ִ� �Լ�
void PrintInfo();								//���� ��Ȳ�� �����ִ� �Լ�
void PrintTime();								//����ð� ���
struct Point
{
	int x, y;
} typedef Point;
// Shape[������ȣ][ȸ����ȣ][�Ϸù�ȣ]{x,y}
Point Shape[][4][4] = {
	/*I �� ���           �� ���                �� ���           �� ��� */
	/*shape[0][0][0~3] 	shape[0][1][0~3]     shape[0][2][0~3]   shape[0][3][0~3]    */

	{ {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}, {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2} },

	/*O �� ���          �� ���             �� ���             �� ���*/
   /*shape[1][0][0~3] 	shape[1][1][0~3]     shape[1][2][0~3]   shape[1][3][0~3]    */
{ {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1} },

/*Z ���� ���� ������  ������ ���� ����     ���� ���� ������   ������ ���� ����*/
/*shape[2][0][0~3] 	shape[2][1][0~3]     shape[2][2][0~3]   shape[2][3][0~3]    */
{ {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1}, {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1} },

/*S ������ ���� ����S    ���� ���� ����       ������ ���θ���   ���� ���� ����*/
/*shape[3][0][0~3] 	shape[3][1][0~3]     shape[3][2][0~3]   shape[3][3][0~3]    */
{ {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1}, {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1} },

/*L ���� ���� ������   ������ ���� ������  ������ ���� ������   ���� ���� ������*/
/*shape[4][0][0~3] 	shape[4][1][0~3]     shape[4][2][0~3]   shape[4][3][0~3]    */
{ {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,-1,1}, {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,1,-1} },

/*J ������ ���� �⿪��   ������ ���� �⿪��   ���� ���� �⿪��   ������ ���� �⿪�� */
/*shape[5][0][0~3] 	shape[5][1][0~3]     shape[5][2][0~3]   shape[5][3][0~3]    */
{ {0,0,1,0,-1,0,1,-1}, {0,0,0,1,0,-1,-1,-1}, {0,0,1,0,-1,0,-1,1}, {0,0,0,-1,0,1,1,1} },

/*T �� ���            �� ���                �� ���            �� ���*/
/*shape[6][0][0~3] 	shape[6][1][0~3]     shape[6][2][0~3]   shape[6][3][0~3]    */
{ {0,0,-1,0,1,0,0,1}, {0,0,0,-1,0,1,1,0}, {0,0,-1,0,1,0,0,-1}, {0,0,-1,0,0,-1,0,1} },

/*�� ��4/1��� */
{ {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },

/*�� ���������*/
{ {0,0,0,0,0,-1,1,0},{0,0,0,0,-1,0,0,-1},{0,0,0,0,0,1,-1,0},{0,0,0,0,0,1,1,0} },
};

enum { EMPTY, BRICK, WALL };
//������ ��ĭ���� �����Ѵ�. ##�� ����� ����� ���ؼ�
const char* arTile[][3] = { { ".","��","��" },
							{"  ","��","��"},
							{"  ","##","||"},
							{"  ","��","��"},
};
int ttype = 3;	//Ÿ���� Ÿ���� ��Ÿ���� ��������
int board[BW + 2][BH + 2];	//���� ���ο� +2�� �������� ���� �߰��ϱ� ���ؼ��̴�.
int nx, ny;					//�̵����� ������ ���� ��ǥ
int brick, rot;				//�̵����� ������ ȸ����ȣ�� ����� ����Ѵ�.
int nbrick;					//�ؽ�Ʈ ��
int score;					//����
int bricknum;				//�� ������ ����
int t;				//����ð�

int main(void)
{
	int nFrame, nStay;	//nFrame:������ �������� �ӵ�
	int x, y;
	thread t;

	t = thread(PrintTime);

	setcursortype(NOCURSOR);	//Ŀ���� ���ڰŸ��� ���ϰ� �ϴ� �Լ�
	randomize();

	for (; 3;)
	{
		clrscr();					//ȭ���� ��� �����.
		t.join();					//�ð� ����
	/*�ݺ������� ������� ������ ����2�� ��, ����0�� ����ִ� ����
		222222222222
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		200000000002
		222222222222

	*/

	/*board�迭���� 12�� 22���� ä��������
	DrawScreen() �Լ����� Ŀ���� �̵����� ���� ���� �׸��� ������ ���� �� �ִ�.
	*/
		for (x = 0; x < BW + 2; x++)
		{
			for (y = 0; y < BH + 2; y++)
			{
				if ((y == 0 || y == BH + 1 || x == 0 || x == BW + 1))
				{
					board[x][y] = WALL;
				}
				else {
					board[x][y] = EMPTY;
				}
			}
		}


		DrawScreen();
		nFrame = 20;
		score = 0;
		bricknum = 0;

		nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
		//���ѷ���
		for (; 1;)
		{

			/*
				sizeof(Shape) : 896����Ʈ
				sizeof(Shape[0]) : 128����Ʈ
				random(7) : 0~6������ ���� �����ϰ� ���´�.
			*/
			bricknum++;
			brick = nbrick;								//�ؽ�Ʈ ���� ���� ������ �����Ѵ�.
			nbrick = random(sizeof(Shape) / sizeof(Shape[0]));	//�������� �����Ѵ�.
			DrawNext();
			nx = BW / 2;	//5����
			ny = 3;
			rot = 0;
			PrintBrick(TRUE);	//���� �׸���.

			/*GetAround�Լ��� ���� ���� ���� ������ �迭�� ��ǥ�� �˻��ϴ� �Լ��̴�.
			��ȯ������ 0���� ū ��(���� 1)�� ���´ٸ� �װ��� ���� �ִٴ� ������ �ݺ�����
			�����.
			*/
			/*���� ������ ������ �ֺ��� ������ �ƴ϶�� �ٸ� ������ �̹� ȭ�� ���ʱ��� ����
			���ִ� �����̹Ƿ� �̶��� ������ �������̴�.
			*/
			if (GetAround(nx, ny, brick, rot) != EMPTY)
				break;

			nStay = nFrame;
			//����2�� ���� �ϳ��� �ٴڿ� ������������ �����̴�.
			//����2�� �ʴ� 20�� �ݺ��Ѵ�. �׷��� 1�ʿ� �ѹ���
			//������ ������ ���̴�.
			for (; 2;) {
				//nStay�� 1�����ҽ��� 0�� ���ٸ� nStay�� �ٽ� 20���� �ʱ�ȭ�Ѵ�.
				//nStay�� 0�� �ɶ� ������ �ϳ� ������.
				if (--nStay == 0)
				{
					PrintTime();
					nStay = nFrame;

					//MoveDown()�� ���̶�� ���� �����Դٴ� �ǹ��̹Ƿ� �ݺ����� ������.
					//MoveDown()�� �����̶�� ���� �������� �ִٴ� ���̴�.
					if (MoveDown())
					{
						break;
					}
				}

				//ProcessKey()�Լ��� TRUE�� ������ ���� �ٴڿ� �����°��̴�.
				//FALSE�̸� ���� ���� �������� �ִ� ���̴�.
				if (ProcessKey())
					break;
				delay(1000 / 20);	//0.05�� ������
			}
			//�̽Ŀ� ���� ��� 10�� 5�� ���̵��� �����ϴ� ��ġ�̴�.
			if (bricknum % 10 == 0 && nFrame > 5)
			{
				nFrame--;
			}
		}
		clrscr();	//ȭ���� �����.
		gotoxy(30, 12);
		puts("G A M E  O V E R");	//Ŀ���� 30,12�� �̵��� ���� GAMEOVER�� ����Ѵ�.
		gotoxy(25, 14);
		puts("�ٽ� �����Ϸ��� Y�� ��������");
		if (tolower(_getch()) != 'y')
			break;
	}

	setcursortype(NORMALCURSOR);
	return 0;
}

void DrawScreen()
{
	int x, y;

	/*������� .(��)���� ä������.
		�������������
		��					  ��
		��					  ��
		��					  ��
		��                    ��
		��                    ��
		��                    ��
		��                    ��
		��                    ��
		��                    ��
		��					  ��
		��                    ��
		��                    ��
		��                    ��
		��                    ��
		��                    ��
		��                    ��
		��		              ��
		��		              ��
		��		              ��
		��		              ��
		�������������
	*/
	for (x = 0; x < BW + 2; x++)
	{
		for (y = 0; y < BH + 2; y++)
		{
			//x*2�� �� ������ Ÿ�� �ϳ��� 2���ڷ� ǥ���Ͽ� ���⿡ ���� �ϱ� ���ؼ��̴�.
			//Ÿ�Ͽ� ���� ��,�� ���ڴ� 2����Ʈ �̱� ������ x*2�� ���־�� �Ѵ�.
			gotoxy(BX + x * 2, BY + y);	//Ŀ���� x,y��ǥ�� �̵���Ų��.
			puts(arTile[ttype][board[x][y]]);
		}
	}

	gotoxy(50, 3); puts("Tetris Ver 1.0");
	gotoxy(50, 5); puts("�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	gotoxy(50, 6); puts("����:���� ����, ESC:����");
	gotoxy(50, 7); puts("P:����, PgUp,PgDn:Ÿ�� ��� �ٲ�");

	DrawNext();
	PrintInfo();
}

void DrawBoard()
{
	int x, y;
	for (x = 1; x < BW + 1; x++)
	{
		for (y = 1; y < BH + 1; y++)
		{
			gotoxy(BX + x * 2, BY + y);
			puts(arTile[ttype][board[x][y]]);
		}
	}
}

BOOL ProcessKey()
{
	int ch, trot;

	/*kbhit(): Ű���尡 ���ȴ��� Ȯ���ϴ� �Լ�
	  getch() : ���͸� ������ �ʾƵ� Ű������ ���ڸ� �ٷ� ���� ����Ǵ� �Լ��̴�.
	*/
	if (_kbhit()) {
		/*
		Ȯ��Ű�� getch()�� �� �� �ҷ��� �ϴ� ������ Ȯ��Ű�� �Ϲ�Ű(����)�� �������� �ʱ� �����̴�.
			�Ϲ�Ű 'K' �� ȭ��ǥ '��' �� �Ȱ��� 75 �̴�.
			�׷��� Ȯ��Ű�� ������ ��� ó�� getch() ���� 0xE0 ��  ���ϵȴ�.
			�̰��� ������ �б⸦ �ؾ��ϴ� ���̴�.
			- ���̹� blog

			1. Ȯ��Ű ���� : ���͸� �������� �������� Ȯ��Ű��� �Ѵ�.
			Ȯ��Ű�� �Է� �޾��� ��� �� �� �� �Է��� �޾ƾ� �Ѵ�.
			2. if (ch == 0xE0 || ch == 0) // Ȯ��Ű�� �Է� �޾Ұų� 0�� �Է� �޾��� ���
			ch = getch(); // �ٽ� �Է� �޴´�.
			3. getch()�Լ��� ����� �� ���� �ڵ�� �׻� ���̴� �� �˾� �Ѱ�.
			- ���̹� ����in
		*/
		ch = _getch();

		if (ch == 0xE0 || ch == 0)
		{
			ch = _getch();
			switch (ch)
			{
			case LEFT:
				//����Ű�� ������ �� �ֺ��� ���� �ִ��� �Ǵ� ���� �ִ��� �ֺ��� �˻��Ѵ�.
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case UP:
				//ȸ�� ���� �������� 3��°��� �ʱ���� 0���� �����ϰ�
				//������ ȸ������ �ƴ϶�� ���� ȸ�������� �����Ѵ�.
				trot = (rot == 3 ? 0 : rot + 1);
				if (GetAround(nx, ny, brick, trot) == EMPTY) {
					PrintBrick(FALSE);
					rot = trot;
					PrintBrick(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown()) {
					return TRUE;
				}
				break;
			case PGUP:
				ttype++;
				if (ttype == sizeof(arTile) / sizeof(arTile[0]))
					ttype = 0;
				DrawScreen();
				PrintBrick(TRUE);
				break;
			case PGDN:
				if (ttype == 0)
					ttype = sizeof(arTile) / sizeof(arTile[0]);
				ttype--;
				DrawScreen();
				PrintBrick(TRUE);
				break;
			}


		}
		else {
			switch (ch) {
			case ' ':	//�����̽��ٸ� ������
				while (MoveDown() == FALSE) {}
				return TRUE;
			case 'p':
				clrscr();
				gotoxy(15, 10);
				puts("TETRIS ��� ����. �ٽ� �����Ϸ��� �ƹ�Ű�� ��������.");
				_getch();
				clrscr();
				DrawScreen();		//�迭�� ������ ����� ������ �ִ� ȭ���� �����.
				PrintBrick(TRUE);	//�������� �ִ� ������ �׸���.
				break;

			case ESC:
				exit(0);

			}
		}

	}
	return FALSE;
}

/*���� ����ϴ� �Լ�*/
void PrintBrick(BOOL Show)
{
	int i;

	/*���� �̷�� ����� 4���� ���� ������ �𿩼� ����� ������ 4�� �ݺ��Ѵ�.*/
	for (i = 0; i < 4; i++)
	{
		/*BX: BX ���� x��ǥ���� ���� ���� �� �ִ�.
			rot:�� ���� ó�����
			i : �Ѱ� ���� ��ǥ
			Ŀ���� ���� ����� ���� ��ġ�� �κ����� Ŀ���� ��ǥ�� ���� ó�� ����
			�ʱ� ��ġ�� ���ϰ� Shpae�迭�� ���� ���� ��ǥ�� �����Ѵ�.
		*/
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		/*���� Show�� TRUE�̸� ���� �׸��� ���̰�
		 FALSE�̸� ���� �����.*/
		puts(arTile[ttype][Show ? BRICK : EMPTY]);
	}
}

/*�� �Լ��� �̵����� ������ �ֺ� ������ �����ؾ� �ϹǷ� ���������� �������� �ʰ� ������ ������ ������ �μ��� ���޹޴´�.*/
int GetAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i = 0; i < 4; i++)
	{
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	/*GetAround�Լ��� ����Ͽ� ���� �ٷ� �Ʒ��� �ٸ� ���� �ִ��� �˻��Ѵ�.*/
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY)
	{
		TestFull();	//���� �ٸ� ���� �ִٸ� TestFull()�Լ��� �����Ų��.
		return TRUE;
	}
	//�� �Ʒ����� ��������� ������ �ִ� ���� ����� ���� ��ĭ ������.
	PrintBrick(FALSE);	//������ �ִ� ���� �����.
	ny++;				//y���� �������Ѽ� �ٷ� �Ʒ��ٿ� ���� �׸���.
	PrintBrick(TRUE);	//���� ���� �����.
	return FALSE;	//���� �������� �ִٴ� �ǹ̷� FALSE�� ��ȯ�Ѵ�.
}

/*������ ��ä�������� �˻��ϴ� �Լ�
������ �� ä������ �� ���� ���ְ� �ٷ� ���ٿ� �ִ� ���Ǵ� ������ �������.*/
void TestFull()
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = { 0,1,3,8,20 };

	for (i = 0; i < 4; i++)
	{
		//�ش��ϴ� ��ġ�� ������ ä���.(����1)
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}

	/*y : 1~20*/
	for (y = 1; y < BH + 1; y++)
	{
		/*x : 1~10
		�����ǿ� ���θ� 1~10���� �ݺ��ؼ� ���� �ִ��� �˻��Ѵ�.*/
		for (x = 1; x < BW + 1; x++)
		{
			if (board[x][y] != BRICK)
				break;
		}
		//������ �� ä������ �ٷ� ������ ��ǥ �����͸� �Ʒ��ٿ� �����Ѵ�.
		if (x == BW + 1)
		{
			count++;
			for (ty = y; ty > 1; ty--)
			{
				for (x = 1; x < BW + 1; x++)
				{
					board[x][ty] = board[x][ty - 1];
				}
			}
			DrawBoard();
			delay(200);
		}
	}
	score += arScoreInc[count];
	PrintInfo();
}

void DrawNext()
{
	int x, y, i;

	for (x = 50; x <= 70; x += 2)
	{
		for (y = 12; y <= 18; y++)
		{
			gotoxy(x, y);
			puts(arTile[ttype][(x == 50 || x == 70 || y == 12 || y == 18) ? WALL : EMPTY]);
		}
	}

	for (i = 0; i < 4; i++)
	{
		gotoxy(60 + (Shape[nbrick][0][i].x) * 2, 15 + Shape[nbrick][0][i].y);
		puts(arTile[ttype][BRICK]);
	}
}

void PrintInfo()
{
	gotoxy(50, 9); printf("���� : %d", score);
	gotoxy(50, 10); printf("���� : %d", bricknum);
}

void PrintTime()
{
	gotoxy(50, 11); printf("����ð� : %d", t++);
}