/*참고자료
http://blog.naver.com/PostView.nhn?blogId=dnpc7848&logNo=220367009691&parentCategoryNo=&categoryNo=&viewDate=&isShowPopularPosts=false&from=postView
*/
/*
ver02
추가사항 1 : 게임 오버시 게임 다시시작 기능 추가
추가사항 2: p키를 누를시 일시정지 기능 추가
추가사항 3: ESC키 누를시 게임종료 기능 추가
추가사항 4: ㅁ4/1모양, ㄱ작은모양 블럭 추가
추가사항 5: 타일모양 변경 기능 추가
추가사항 6: 넥스트 블럭 추가
추가사항 7: 점수와 벽돌갯수 추가
*/
#include <Turboc.h>
#include <conio.h>

#define LEFT 75		//왼쪽
#define RIGHT 77	//오른쪽
#define UP 72		//위쪽
#define DOWN 80		//아래쪽
#define ESC 27		//취소
#define BX 5
#define BY 1
#define BW 10	//보드판 가로
#define BH 20	//보드판 세로
#define PGUP 73
#define PGDN 81

void DrawScreen();								//화면전체를 그린다. 게임판과 게임 이름, 벽까지 한꺼번에 그린다.
void DrawBoard();								//게임판만 그린다. 즉 외부벽과 문자열들은 빼고 쌓여있는 벽돌만 그린다.

BOOL ProcessKey();								/*키 입력을 처리하는데 main함수의 부담을 덜어주기 위해 별도의 함수로 분리해놓았다.
												이동중인 벽돌이 바닥에 닿으면 TRUE를 리턴한다.*/

void PrintBrick(BOOL Show);						//벽돌을 출력하거나 삭제하는데 이동중인 벽돌을 대상으로 하므로 전역변수 brick, rot, nx, ny값을 참조한다.

int GetAround(int x, int y, int b, int r);		/*벽돌 주변에 무엇이 있는지 검사하여 벽돌의 이동 및 회전 가능성을 조사한다.
												이동중인 벽돌의 주변을 조사하는 것이 아니므로 인수로 전달된 위치의 벽돌 모양을 참조한다.*/
BOOL MoveDown();								//벽돌을 한칸 아래로 이동시킨다. 만약 바닥에 닿았다면 TestFull 함수를 호출한 후 TRUE를 리턴한다.
void TestFull();								//수평으로 다 채워진 줄을 찾아 삭제한다.
void DrawNext();								//넥스트 블럭을 보여주는 함수
void PrintInfo();								//점수 상황을 보여주는 함수
struct Point
{
	int x, y;
} typedef Point;
// Shape[벽돌번호][회전번호][일련번호]{x,y}
Point Shape[][4][4] = {
	/*I ㅡ 모양           ㅣ 모양                ㅡ 모양           ㅣ 모양 */
	/*shape[0][0][0~3] 	shape[0][1][0~3]     shape[0][2][0~3]   shape[0][3][0~3]    */

	{ {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}, {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2} },

	/*O ㅁ 모양          ㅁ 모양             ㅁ 모양             ㅁ 모양*/
   /*shape[1][0][0~3] 	shape[1][1][0~3]     shape[1][2][0~3]   shape[1][3][0~3]    */
{ {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1} },

/*Z 왼쪽 가로 리을ㄹ  오른쪽 세로 리을     왼쪽 가로 리을ㄹ   오른쪽 세로 리을*/
/*shape[2][0][0~3] 	shape[2][1][0~3]     shape[2][2][0~3]   shape[2][3][0~3]    */
{ {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1}, {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1} },

/*S 오른쪽 가로 리을S    왼쪽 세로 리을       오른쪽 가로리을   왼쪽 세로 리을*/
/*shape[3][0][0~3] 	shape[3][1][0~3]     shape[3][2][0~3]   shape[3][3][0~3]    */
{ {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1}, {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1} },

/*L 왼쪽 가로 니은┏   오른쪽 세로 니은ㄱ  오른쪽 가로 니은┘   왼쪽 세로 니은ㄴ*/
/*shape[4][0][0~3] 	shape[4][1][0~3]     shape[4][2][0~3]   shape[4][3][0~3]    */
{ {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,-1,1}, {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,1,-1} },

/*J 오른쪽 가로 기역┐   오른쪽 세로 기역┘   왼쪽 가로 기역ㄴ   오른쪽 세로 기역┌ */
/*shape[5][0][0~3] 	shape[5][1][0~3]     shape[5][2][0~3]   shape[5][3][0~3]    */
{ {0,0,1,0,-1,0,1,-1}, {0,0,0,1,0,-1,-1,-1}, {0,0,1,0,-1,0,-1,1}, {0,0,0,-1,0,1,1,1} },

/*T ㅗ 모양            ㅏ 모양                ㅜ 모양            ㅓ 모양*/
/*shape[6][0][0~3] 	shape[6][1][0~3]     shape[6][2][0~3]   shape[6][3][0~3]    */
{ {0,0,-1,0,1,0,0,1}, {0,0,0,-1,0,1,1,0}, {0,0,-1,0,1,0,0,-1}, {0,0,-1,0,0,-1,0,1} },

/*ㅁ ㅁ4/1모양 */
{ {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },

/*ㄱ ㄱ작은모양*/
{ {0,0,0,0,0,-1,1,0},{0,0,0,0,-1,0,0,-1},{0,0,0,0,0,1,-1,0},{0,0,0,0,0,1,1,0} },
};

enum { EMPTY, BRICK, WALL };
//공백을 두칸으로 설정한다. ##을 제대로 지우기 위해서
const char* arTile[][3] = { { ".","■","□" },
							{"  ","■","□"},
							{"  ","##","||"},
							{"  ","●","▣"},
};
int ttype = 3;	//타일의 타입을 나타내는 전역변수
int board[BW + 2][BH + 2];	//가로 세로에 +2를 한이유는 벽을 추가하기 위해서이다.
int nx, ny;					//이동중인 벽돌의 현재 좌표
int brick, rot;				//이동중인 벽돌의 회전번호와 모양을 기억한다.
int nbrick;					//넥스트 블럭
int score;					//점수
int bricknum;				//총 벽돌의 갯수


int main(void)
{
	int nFrame, nStay;	//nFrame:벽돌이 떨어지는 속도
	int x, y;

	setcursortype(NOCURSOR);	//커서를 깜박거리게 못하게 하는 함수
	randomize();

	for (; 3;)
	{
		clrscr();					//화면을 모두 지운다.

	/*반복문으로 만들어진 보드판 숫자2은 벽, 숫자0은 비어있는 공간
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

	/*board배열에는 12행 22열로 채워지지만
	DrawScreen() 함수에서 커서의 이동으로 위와 같은 그림의 구조를 만들 수 있다.
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
		//무한루프
		for (; 1;)
		{
			/*
				sizeof(Shape) : 896바이트
				sizeof(Shape[0]) : 128바이트
				random(7) : 0~6까지의 수가 랜덤하게 나온다.
			*/
			bricknum++;
			brick = nbrick;								//넥스트 블럭을 현재 블럭으로 저장한다.
			nbrick = random(sizeof(Shape) / sizeof(Shape[0]));	//다음블럭을 저장한다.
			DrawNext();
			nx = BW / 2;	//5대입
			ny = 3;
			rot = 0;
			PrintBrick(TRUE);	//블럭을 그린다.

			/*GetAround함수는 블럭이 놓일 곳에 보드판 배열의 좌표를 검사하는 함수이다.
			반환문으로 0보다 큰 수(블럭인 1)가 나온다면 그곳은 블럭이 있다는 뜻으로 반복문을
			벗어난다.
			*/
			/*새로 생성한 벽돌이 주변이 공백이 아니라면 다른 벽돌이 이미 화면 위쪽까지 가득
			차있는 상태이므로 이때가 게임이 끝날때이다.
			*/
			if (GetAround(nx, ny, brick, rot) != EMPTY)
				break;

			nStay = nFrame;
			//루프2는 벽돌 하나가 바닥에 닿을때까지의 루프이다.
			//루프2는 초당 20번 반복한다. 그래서 1초에 한번씩
			//벽돌이 내려올 것이다.
			for (; 2;) {
				//nStay를 1씩감소시켜 0과 같다면 nStay를 다시 20으로 초기화한다.
				//nStay가 0이 될때 벽돌을 하나 내린다.
				if (--nStay == 0)
				{
					nStay = nFrame;

					//MoveDown()이 참이라면 블럭이 내려왔다는 의미이므로 반복문을 나간다.
					//MoveDown()이 거짓이라면 블럭이 내려오고 있다는 뜻이다.
					if (MoveDown())
					{
						break;
					}
				}

				//ProcessKey()함수가 TRUE가 나오면 블럭이 바닥에 내려온것이다.
				//FALSE이면 아직 블럭이 내려오고 있는 중이다.
				if (ProcessKey())
					break;
				delay(1000 / 20);	//0.05초 딜레이
			}
			//이식에 사용된 상수 10과 5는 난이도를 조절하는 수치이다.
			if (bricknum % 10 == 0 && nFrame > 5)
			{
				nFrame--;
			}
		}
		clrscr();	//화면을 지운다.
		gotoxy(30, 12);
		puts("G A M E  O V E R");	//커서를 30,12로 이동한 다음 GAMEOVER를 출력한다.
		gotoxy(25, 14);
		puts("다시 시작하려면 Y를 누르세요");
		if (tolower(_getch()) != 'y')
			break;
	}

	setcursortype(NORMALCURSOR);
	return 0;
}

void DrawScreen()
{
	int x, y;

	/*빈공간은 .(점)으로 채워진다.
		□□□□□□□□□□□□
		□					  □
		□					  □
		□					  □
		□                    □
		□                    □
		□                    □
		□                    □
		□                    □
		□                    □
		□					  □
		□                    □
		□                    □
		□                    □
		□                    □
		□                    □
		□                    □
		□		              □
		□		              □
		□		              □
		□		              □
		□□□□□□□□□□□□
	*/
	for (x = 0; x < BW + 2; x++)
	{
		for (y = 0; y < BH + 2; y++)
		{
			//x*2를 한 이유는 타일 하나를 2문자로 표시하여 보기에 좋게 하기 위해서이다.
			//타일에 사용된 ■,□ 문자는 2바이트 이기 때문에 x*2를 해주어야 한다.
			gotoxy(BX + x * 2, BY + y);	//커서를 x,y좌표로 이동시킨다.
			puts(arTile[ttype][board[x][y]]);
		}
	}

	gotoxy(50, 3); puts("Tetris Ver 1.0");
	gotoxy(50, 5); puts("좌우:이동, 위:회전, 아래:내림");
	gotoxy(50, 6); puts("공백:전부 내림, ESC:종료");
	gotoxy(50, 7); puts("P:정지, PgUp,PgDn:타일 모양 바꿈");

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

	/*kbhit(): 키보드가 눌렸는지 확인하는 함수
	  getch() : 엔터를 누르지 않아도 키보드의 글자를 바로 값이 저장되는 함수이다.
	*/
	if (_kbhit()) {
		/*
		확장키는 getch()를 두 번 불러야 하는 이유는 확장키와 일반키(문자)가 고유하지 않기 때문이다.
			일반키 'K' 와 화살표 '←' 가 똑같이 75 이다.
			그래서 확장키를 눌렀을 경우 처음 getch() 에서 0xE0 가  리턴된다.
			이것을 가지고 분기를 해야하는 것이다.
			- 네이버 blog

			1. 확장키 개념 : 엔터를 기준으로 오른쪽을 확장키라고 한다.
			확장키로 입력 받았을 경우 한 번 더 입력을 받아야 한다.
			2. if (ch == 0xE0 || ch == 0) // 확장키를 입력 받았거나 0을 입력 받았을 경우
			ch = getch(); // 다시 입력 받는다.
			3. getch()함수를 사용할 때 위의 코드는 항상 쓰이니 잘 알아 둘것.
			- 네이버 지식in
		*/
		ch = _getch();

		if (ch == 0xE0 || ch == 0)
		{
			ch = _getch();
			switch (ch)
			{
			case LEFT:
				//왼쪽키를 누르면 그 주변에 벽이 있는지 또는 블럭이 있는지 주변을 검사한다.
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
				//회전 블럭이 마지막인 3번째라면 초기블럭인 0으로 설정하고
				//마지막 회전블럭이 아니라면 다음 회전블럭으로 변경한다.
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
			case ' ':	//스페이스바를 누르면
				while (MoveDown() == FALSE) {}
				return TRUE;
			case 'p':
				clrscr();
				gotoxy(15, 10);
				puts("TETRIS 잠시 중지. 다시 시작하려면 아무키나 누르세요.");
				_getch();
				clrscr();
				DrawScreen();		//배열에 저장한 값들로 기존에 있는 화면을 만든다.
				PrintBrick(TRUE);	//내려오고 있는 벽돌을 그린다.
				break;

			case ESC:
				exit(0);

			}
		}

	}
	return FALSE;
}

/*블럭을 출력하는 함수*/
void PrintBrick(BOOL Show)
{
	int i;

	/*블럭을 이루는 모양은 4개의 작은 블럭들이 모여서 만들기 때문에 4번 반복한다.*/
	for (i = 0; i < 4; i++)
	{
		/*BX: BX 다음 x좌표부터 블럭을 놓을 수 있다.
			rot:은 블럭의 처음모양
			i : 한개 블럭의 좌표
			커서를 왼쪽 상단의 벽에 위치한 부분으로 커서의 좌표와 블럭이 처음 나올
			초기 위치를 더하고 Shpae배열로 블럭이 나올 좌표를 지정한다.
		*/
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		/*만약 Show가 TRUE이면 블럭을 그리는 것이고
		 FALSE이면 블럭을 지운다.*/
		puts(arTile[ttype][Show ? BRICK : EMPTY]);
	}
}

/*이 함수는 이동중인 벽돌의 주변 정보를 조사해야 하므로 전역변수를 참조하지 않고 조사할 벽돌의 정보를 인수로 전달받는다.*/
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
	/*GetAround함수를 사용하여 블럭의 바로 아래에 다른 블럭이 있는지 검사한다.*/
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY)
	{
		TestFull();	//만약 다른 블럭이 있다면 TestFull()함수를 실행시킨다.
		return TRUE;
	}
	//블럭 아랫줄이 비어있으면 이전에 있는 블럭을 지우고 블럭을 한칸 내린다.
	PrintBrick(FALSE);	//이전에 있던 블럭을 지운다.
	ny++;				//y값을 증가시켜서 바로 아랫줄에 블럭을 그린다.
	PrintBrick(TRUE);	//블럭을 새로 만든다.
	return FALSE;	//블럭이 내려오고 있다는 의미로 FALSE를 반환한다.
}

/*한줄이 다채워졌는지 검사하는 함수
한줄이 다 채워지면 그 줄을 없애고 바로 윗줄에 있는 블럭또는 공백을 끌어내린다.*/
void TestFull()
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = { 0,1,3,8,20 };

	for (i = 0; i < 4; i++)
	{
		//해당하는 위치를 블럭으로 채운다.(숫자1)
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}

	/*y : 1~20*/
	for (y = 1; y < BH + 1; y++)
	{
		/*x : 1~10
		보드판에 가로를 1~10까지 반복해서 블럭이 있는지 검사한다.*/
		for (x = 1; x < BW + 1; x++)
		{
			if (board[x][y] != BRICK)
				break;
		}
		//한줄이 다 채워지면 바로 윗줄의 좌표 데이터를 아랫줄에 대입한다.
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
	gotoxy(50, 9); printf("점수 : %d", score);
	gotoxy(50, 10); printf("벽돌 : %d", bricknum);
}