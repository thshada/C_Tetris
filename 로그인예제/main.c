#include <stdio.h>//기본 입출력 함수입니다.
#include <mysql.h>//mysql에 접속하기 위한 함수입니다.
#include<windows.h>//coord 자료형이 내장되어 있는 함수  gotoxy함수를 사용하기위해 추가한 코드 SetConsoleCursorInfo함수
//를 사용하기 위한 함수
#include<conio.h>//kbhit()함수와 _getch()함수를 사용하기위해 추가하는 코드입니다.
#include<time.h> //난수표 생성을 위한 srand의 씨드값을 주기 위해 사용되는 함수time을 쓰기 위해 추가한 코드입니다.
#include<stdlib.h>//srand를 사용하기 위한 함수입니다.
#include<mmsystem.h>//play sound함수를 사용하기 위한 코드
#pragma comment(lib,"libmysql.lib")//lib를 프로젝트 셋팅할때마다 project>setting 에서
//link에 libmysql.lib와 ws2_32.lib를 modules에 등록하는 번거로움을 없애준다.오류가 발생할 수 도 있기때문에 추가함.
#pragma comment(lib, "winmm.lib") // mmsystem.h 헤더파일은 winmm.lib라는 파일을 사용하므로 추가하는 코드
#define Third_Run "C:\\Users\\user\\source\\repos\\bgm\\bgm\\tetris-tone-loop.wav"//bgm재생을 위해, 경로값을
//정의 했습니다. 경로값은 \두개가 컴퓨터 주소의 |랑 같습니다.
#define SECOND_RUN "C:\\Users\\user\\Desktop\\c언어연습문제\\bgm\\tetris-tone-loop1.wav"
#define First_Run "C:\\Users\\user\\Desktop\\c언어연습문제\\bgm1\\BOOM.wav"
#define RED         (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define DB_HOST "127.0.0.1"//자기자신의 주소,루프백주소 본인 컴퓨터의 ip address,
#define DB_USER "root" //DB_USER 깔때 설치했던 user_name
#define DB_PASS "abc123" //password
#define DB_NAME "ranking"//접속할려는 DB table 이름 
#define CHOP(x) x[strlen(x)-1]=' '//아이디 패스워드 입력할때, 엔터키들어가면 문자열의 끝으로 바꿔줌
#define LEFT 75 //좌로 이동        //키보드 방향키 아스키코드             ↑ 224,72 두번 반환

#define RIGHT 77 //우로 이동       //                       ←224 75   두번 반환 →   224 77 두번 반환
                                                                                   
                                                                     //  ↓ 224 80  두번 반환
#define UP 72 //회전   그러므로 위쪽 방향키 눌러졌을때는 72
#define DOWN 80 //soft drop  밑 방향키 눌렀을때는 80
 
#define SPACE 32 //hard drop    스페이스키는 32 를 반환

#define p 112 //일시정지     소문자 아스키코드 112

#define P 80 //일시정지       대문자 p는 아스키 코드 80이기 때문에, P를 80으로 정으ㅣ

#define ESC 27 //게임종료       ESC 는 아스키코드 27
#define false 0   //  false 는 0으로 정의           

#define true 1    //true는 1로 정의



#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 

#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 

#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 

#define WALL 1

#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 

#define MAIN_X 11 //게임판 가로크기 

#define MAIN_Y 23 //게임판 세로크기 

#define MAIN_X_ADJ 3 //게임판 위치조정 

#define MAIN_Y_ADJ 1 //게임판 위치조정 

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정 

int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장 

int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장

int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장
int blocks[7][4][4][4] = {  //각 도형을 4차원 배열로 만듬 
							//7=테트리스 도형의 개수
							//4(2번째) 각 도형의 방향 개수
							//4(3번쩨) 세로 길이
							//4 (4번째) 가로 길이
                             //즉 2차원 배열이 28개 있는것 각 도형이 4개의 모양을 가짐,

{{0,0,0,0,              //■■
0,1,1,0,                //■■
0,1,1,0,
0,0,0,0},

	  {0,0,0,0             //■■
      ,0,1,1,0,            //■■  ↑키가 눌러졌을때 2번째 블록모양 
	   0,1,1,0,          
	   0,0,0,0},           

 {0,0,0,0,
  0,1,1,0,                 //■■  
  0,1,1,0,                 //■■  회전해도 같은 모양임 정사각형이므로
  0,0,0,0},

	{0,0,0,0
	 ,0,1,1,0,               //■■
	  0,1,1,0,               //■■
	  0,0,0,0}},                          



{{0,0,0,0,                   //■■■■ 초기 모습
 0,0,0,0,
 1,1,1,1,
 0,0,0,0},

	{0,1,0,0,                //■
	 0,1,0,0,                //■
	 0,1,0,0,                //■        ↑키가 눌러졌을때 2번째 블록모양
	 0,1,0,0},               //■

 {0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0},                 //■■■■      ↑키가 눌러졌을때 3번째 블록모양
	
	{0,1,0,0,              //■   
	 0,1,0,0,              //■
 	 0,1,0,0,              //■             ↑키가 눌러졌을때 4번째 블록모양
	 0,1,0,0}},            //■

{{0,0,0,0,
  1,1,0,0,                 //■■             초기 블록 모양
  0,1,1,0,                 //  ■■
  0,0,0,0},

	 {0,0,1,0,
	  0,1,1,0,             //     ■
	  0,1,0,0,             //   ■■       ↑키가 눌러졌을때 2번째 블록모양,
	  0,0,0,0},            //   ■

 {0,0,0,0,
  0,0,0,0,
  1,1,0,0,                 //■■           ↑키가 눌러졌을때 3번째 블록모양
  0,1,1,0},                //  ■■

 {0,0,0,0,
  0,0,1,0,                 //    ■
  0,1,1,0,                 //  ■■          ↑키가 눌러졌을때 4번째 블록모양
  0,1,0,0}},               //  ■


{{0,0,0,0,
  0,1,1,0,                 //  ■■            초기블록 모양                   
  1,1,0,0,                 //■■
  0,0,0,0},

	{0,0,0,0,
	 1,0,0,0,             //■
	 1,1,0,0              //■■              ↑키가 눌러졌을때 2번째 블록모양
    ,0,1,0,0},            //  ■

 {0,0,0,0,
  0,0,0,0,
  0,1,1,0,                 //   ■■          ↑키가 눌러졌을때 3번째 블록모양
  1,1,0,0},                // ■■

	     {0,0,0,0, 
	      1,0,0,0,         // ■
	      1,1,0,0,         // ■■            ↑키가 눌러졌을때 4번째 블록모양
	      0,1,0,0}},       //   ■


{{0,0,0,0,                 //    ■
 0,0,1,0,                  //■■■          초기블록모양
 1,1,1,0,
 0,0,0,0},
	
	{0,0,0,0,
	 1,1,0,0,             //■■
	 0,1,0,0,             //  ■         ↑키가 눌러졌을때 2번째 블록모양
	 0,1,0,0},            //  ■

 {0,0,0,0,
  0,0,0,0,
  1,1,1,0,                //■■■       ↑키가 눌러졌을때 3번째 블록모양
  1,0,0,0},               //■
	
	
	{0,0,0,0,          
	 0,1,0,0,            //■             
	 0,1,0,0,            //■            ↑키가 눌러졌을때 4번째 블록모양
	 0,1,1,0}},          //■■

{{0,0,0,0,
  1,0,0,0,                 //■
  1,1,1,0,                 //■■■     초기 블록모양
  0,0,0,0},              
	
 {0,0,0,0,                // ■■
  0,1,1,0,                // ■
  0,1,0,0,                // ■        ↑키가 눌러졌을때 2번째 블록모양
  0,1,0,0},               // ■


 {0,0,0,0,
  0,0,0,0,
  1,1,1,0,               //■■■       ↑키가 눌러졌을때 3번째 블록모양
  0,0,1,0},              //    ■       
	
	{0,0,0,0,
  	 0,1,0,0,            //  ■
	 0,1,0,0,            //  ■           ↑키가 눌러졌을때 4번째 블록모양
	 1,1,0,0}},          //■■


{{0,0,0,0,
  0,1,0,0,                    //      ■
  1,1,1,0,                    //    ■■■         초기 블록모양
  0,0,0,0},
	
	{0,0,0,0,
	 0,1,0,0,                 // ■
	 0,1,1,0,                 // ■■              ↑키가 눌러졌을때 2번째 블록모양
	 0,1,0,0},                // ■

 {0,0,0,0,
  0,0,0,0,
  1,1,1,0,                     //■■■             ↑키가 눌러졌을때 3번째 블록모양
  0,1,0,0},                    //  ■                
	
	{0,0,0,0,
	 0,1,0,0,                   //     ■
	 1,1,0,0,                   //   ■■            ↑키가 눌러졌을때 4번째 블록모양
	 0,1,0,0}}                  //     ■
		                 
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 



int b_type; //블록 종류를 저장 

int b_rotation; //블록 회전값 저장 

int b_type_next; //다음 블록값 저장 


int main_org[MAIN_Y][MAIN_X]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨 

int main_cpy[MAIN_Y][MAIN_X]; //즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음 

							  //main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림) 

							  //main_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침 

int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장 



int key; //키보드로 입력받은 키값을 저장 



int speed; //게임진행속도 

int level; //현재 level 

int level_goal; //다음레벨로 넘어가기 위한 목표점수 

int cnt; //현재 레벨에서 제거한 줄 수를 저장 

int score; //현재 점수 

int last_score = 0; //마지막게임점수 

int best_score = 0; //최고게임점수 



int new_block_on = 0; //새로운 블럭이 필요함을 알리는 flag 

int crush_on = 0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag 

int level_up_on = 0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag 

int space_key_on = 0; //hard drop상태임을 알려주는 flag 



void title(void); //게임시작화면 

void reset(void); //게임판 초기화 

void reset_main(void); //메인 게임판(main_org[][]를 초기화)

void reset_main_cpy(void); //copy 게임판(main_cpy[][]를 초기화)

void draw_map(void); //게임 전체 인터페이스를 표시 

void draw_main(void); //게임판을 그림 

void new_block(void); //새로운 블록을 하나 만듦 

void check_key(void); //키보드로 키를 입력받음 

void drop_block(void); //블록을 아래로 떨어트림 
int check_crush(int bx, int by, int rotation); //bx, by위치에 rotation회전값을 같는 경우 충돌 판단 
void move_block(int dir); //dir방향으로 블록을 움직임 

void check_line(void); //줄이 가득찼는지를 판단하고 지움 

void check_level_up(void); //레벨목표가 달성되었는지를 판단하고 levelup시킴 

void check_game_over(void); //게임오버인지 판단하고 게임오버를 진행 

void pause(void);//게임을 일시정지시킴 

int music = 0;


void gotoxy(int x, int y) { //gotoxy함수   콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수,

	COORD pos = { 2 * x,y };   // x, y 위치를 가지고 있는 구조체 입니다.구조체 변수를 만들어서
	//x와 y를 채워 넣어 줍니다. 

	//2 * x를 넣어주는 이유는 커서가 1씩 이동하는것보다 2씩 이동하는게 더 자연스러워 보이기 때문에 2 * x를 넣어줍니다.



	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //이름에서 알수 있듯이 콘솔의 커서 위치를 알려주는 함수 입니다.

		//첫번째 매개변수로는 화면 출력 핸들러  GetStdHandle(STD_OUTPUT_HANDLE)를 넣어주고

		//두번째 매개변수로는 위치를 나타내는 COORD 타입 변수인 Pos를 넣어줍니다.

		//첫번째 매개변수에는 을 넣습니다.STD_OUTPUT_HANDLE은 윈도우 베이스의 기본 출력을 뜻하는 매크로 이고, 그 매크로에 해당하는 핸들러를 GetStdHandle()로 받아옵니다.


}



typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
//nocursor는 화면에 커서 위치가 안 보이게 해준다.
//NOMALCURSOR은 기본적인 상태, 밑줄바 같은 형태
//solidcursor는 밑줄바가 아닌 배경 전체에서 깜빡임

void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 

	CONSOLE_CURSOR_INFO CurInfo; //console sursor 구조체



	switch (c) {

	case NOCURSOR:

		CurInfo.dwSize = 1; //cursor size

		CurInfo.bVisible = FALSE; //cursor 안보임

		break;

	case SOLIDCURSOR:

		CurInfo.dwSize = 100;

		CurInfo.bVisible = TRUE;

		break;

	case NORMALCURSOR:

		CurInfo.dwSize = 20;

		CurInfo.bVisible = TRUE;

		break;

	}

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo); //현재 console의 handle을 구하는 함수

}


int main(void) {//메인함수로 가장 먼저 실행됨
	int i=0;//정수변수 i를선언 과 초기화
	
	if (music == 0) {
		PlaySound(TEXT(Third_Run), NULL, SND_FILENAME | SND_ASYNC);
	}
	//playSOUNd 함수 노래를 재생시키기 위한 함수
	//WAV 파일만 지원합니다.   PlaySound("파일경로", 0, SND_FILENAME | SND_ASYNC | SND_LOOP); //루프 재생
	//snd_loop는 지정한 사운드를 반복적으로 연주합니다.
	//snd_async는 비동기화 연주, 연주시작과 동시에 리턴하므로 다른작업을 바로 할 수 있습니다.
	//snd_FILENAME 은 pszSound에 파일을 넣을 경우 반드시 넣어야 하는 옵션

	srand((unsigned)time(NULL)); //난수표생성 

	setcursortype(NOCURSOR); //커서 없앰 

	title(); //메인타이틀 호출 

	reset(); //게임판 리셋 게임판 다시 그려줌
	music ++;
	if (music == 1) {
		PlaySound(TEXT(SECOND_RUN), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	while (1) {//while 무한반복
		
		for (i = 0; i < 5; i++) { //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 

			check_key(); //키입력확인 

			draw_main(); //화면을 그림 

			Sleep(speed); //게임속도조절 

			if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);

			//블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 

			if (space_key_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
				//space_key_on이 1이 되었을때, 플래그 신호라고도 함. 
				space_key_on = 0;//다시 초기화를 해줌으로써, 1이 되었을때, 다시 실행

				break;//반복 종료

			}

		}

		drop_block(); // 블록을 한칸 내림 

		check_level_up(); // 레벨업을 체크 

		check_game_over(); //게임오버를 체크 

		if (new_block_on == 1) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성 
		  
	}

	
	return 0;//메인함수의 종료를 나타냄
}
void title(void) { //00형태함수 return값 없음

	int x = 5; //타이틀화면이 표시되는 x좌표 

	int y = 4; //타이틀화면이 표시되는 y좌표 

	int cnt; //타이틀 프레임을 세는 변수  
	int music = 0;

	
		
	
	gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■"); Sleep(100);
	 //gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	 //y좌표를 +0한뒤 블록을 출력합니다.
	 //sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
	//그 뒤에 다음 줄 실행합니다.

	gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■"); Sleep(100);
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +1한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.

	gotoxy(x, y + 2); printf("□□□■              □■  ■"); Sleep(100);
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +2한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.

	gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□"); Sleep(100);
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +3한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□"); Sleep(100);
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +4한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x + 5, y + 2);printf("T E T R I S"); Sleep(100);
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//x좌표 +5 y좌표를 +2한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 7);
	
	printf("\033[1;33m 아무키 누르면 시작\033[0m");


		
	
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +7한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 9); printf("  △   : 회전");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +9한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 10); printf("◁  ▷ : 왼쪽 / 오른쪽");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +10한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 11); printf("  ▽   : 조금 빨리 떨어짐");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +11한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 12); printf(" SPACE : 빨리 떨어짐");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +12한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 13); printf("   P   : 멈춤");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +13한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 14); printf("  ESC  : 나가기");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +14한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.
	gotoxy(x, y + 16); printf("하드드롭 보너스 / 콤보");
	//gotoxy 함수 콘솔창에는 좌표가 지정되어있음, 그 좌표로 이동 시켜주는 함수 즉 초기값 (5,4) 좌표에서 
	//y좌표를 +16한뒤 블록을 출력합니다.
	//sleep()함수는 지정한 시간만큼 대기하는 함수입니다 단위는 ms입니다.그러므로 0.1초를 기다립니다.
   //그 뒤에 다음 줄 실행합니다.


	for (cnt = 0;; cnt++) { //cnt를 1씩 증가시키면서 계속 반복    //하나도 안중요한 별 반짝이는 애니메이션효과 

		if (_kbhit()) break; //키입력이 있으면 무한루프 종료   kbhit은 키입력이 있는 경우  khbit 함수는 키보드가 눌렸으면,1 아니면 0을 반환하는 함수

		if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("★"); }       //cnt가 200으로 나누어 떨어질때 별을 표시 

		if ((cnt % 200 - 100) == 0) { gotoxy(x + 4, y + 1); printf("  "); } //위 카운트에서 100카운트 간격으로 별을 지움 즉
		//200의 배수 +100  카운트일 때  별이 사라짐 300 500

		if ((cnt % 350) == 0) { gotoxy(x + 13, y + 2); printf("☆"); } //윗별과 같지만 시간차를 뒀음 
		            
		if ((cnt % 350 - 100) == 0) { gotoxy(x + 13, y + 2); printf("  "); }//위의 카운트에서 100카운트 간격으로 별을 지움
		//350의 배수 + 100  카운트일 때  별이 사라짐 즉 450 800 등등 일때 별이 사라짐.
		Sleep(10); // 00.1초 딜레이  

	}



	while (_kbhit()) _getch(); //버퍼에 기록된 키값을 버림    _getch()함수는 표준 입력 버퍼에
	//저장되지 않고 입력된 키값을 즉시 반환 합니다.
	//_getch()함수는 사용자가 입력한 키값은 받지만 화면에 입력한 문자가 출력되지 않습니다.
	//즉 키입력시 그 키값을 버림,

}



void reset(void) {//00형태 함수 return값 없음 reset 함수
	
	FILE* file;
	fopen_s(&file, "score.dat", "rt");// score.dat파일을 연결 

	if (file == 0) { best_score = 0; } //파일이 없으면 걍 최고점수에 0을 넣음 

	else {

		fscanf_s(file, "%d", &best_score); // 파일이 열리면 최고점수를 불러옴 

		fclose(file); //파일 닫음 

	}

	level = 1; //각종변수 초기화 

	score = 0;//score 변수를 0으로 초기화

	level_goal = 1000;//level_goal를 1000으로 초기화

	key = 0;//key값을 0으로 초기화

	crush_on = 0;//crush_on 변수값을 0으로 초기화

	cnt = 0;//cnt 변수를 0으로 초기화

	speed = 100;//speed를 100으로 초기화



	system("cls"); //화면지움 

	reset_main(); // main_org를 초기화 

	draw_map(); // 게임화면을 그림

	draw_main(); // 게임판을 그림 



	b_type_next = rand() % 7; //다음번에 나올 블록 종류를 랜덤하게 생성 

	new_block(); //새로운 블록을 하나 만듦  



}



void reset_main(void) { //00형태 함수 return 값 없음 게임판을 초기화  

	int i=0, j=0;//정수변수 i,j를 선언과 초기화



	for (i = 0; i < MAIN_Y; i++) { // 게임판을 0으로 초기화   세로판 크기만큼 반복을 돌음

		for (j = 0; j < MAIN_X; j++) {//게임판을 0으로초기화 가로판 크기 만큼 반복을함.

			main_org[i][j] = 0; //게임판 규격 11x23의 크기의 게임판을 초기화 함, 본게임판

			main_cpy[i][j] = 100;// 메인게임판 배열을 100으로 초기화  임시 게임판 

		}

	}

	for (j = 1; j < MAIN_X; j++) { //y값이 3인 위치에 천장을 만듦 

		main_org[3][j] = CEILLING;//세로값이 3인 위치에 천장을 만듬 printf(". "); 이런형태의 천장임 

	}

	for (i = 1; i < MAIN_Y - 1; i++) { //좌우 벽을 만듦  

		main_org[i][0] = WALL;//임시게임판에 반복을 돌며 1~21까지 벽을만듬  세로구간에 이런 형태로 printf("▩");

		main_org[i][MAIN_X - 1] = WALL;//반복을 돌며 x값이 10인 값에 벽을 만듬. 이런 형태로 printf("▩");

	}

	for (j = 0; j < MAIN_X; j++) { //바닥벽을 만듦 

		main_org[MAIN_Y - 1][j] = WALL;//임시게임 판에 가로벽을 만듬 끝에서 y값-1 인 구역에 벽을 만듬   x값은 고정
		//printf("▩");이런형태로 wall을 구성
	}

}



void reset_main_cpy(void) { //00형태 함수 return 값 없음 main_cpy를 초기화 

	int i=0, j=0; //정수변수 x,y를 선언과 초기화



	for (i = 0; i < MAIN_Y; i++) {         //게임판에 게임에 사용되지 않는 숫자를 넣음 

		for (j = 0; j < MAIN_X; j++) {  //이는 main_org와 같은 숫자가 없게 하기 위함 

			main_cpy[i][j] = 100;//게임멥을 100으로 초기화

		}

	}

}



void draw_map(void) { //게임 상태 표시를 나타내는 함수  00형태함수 리턴값 없음 

	int y = 3;             // level, goal, score만 게임중에 값이 바뀔수 도 있음 그 y값을 따로 저장해둠 

						 // 그래서 혹시 게임 상태 표시 위치가 바뀌어도 그 함수에서 안바꿔도 되게.. 

	gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", level);//gotoxy함수는 그 콘솔창 좌표로 가게해줌,STATUS_X_ADJ=15이기 때문에,
	//15,3에  출력해줌,

	gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt); //즉 15,4좌표에 출력해줌

	gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ "); //15,5 자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | "); //15,6자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | "); //15,7자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");//15,8자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");//15,9자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ "); //15,10자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :"); //15,11자리에 출력함

	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score); //15,12자리에 칸을 띄운뒤 6칸자리를 차지하고, score를 출력함

	gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");//즉 15,13자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score); //15,14자리에 띄운뒤 6칸자리를 차지하고, last_score를 출력함

	gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");//15,15자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);//15,16자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 15); printf("  △   : Shift        SPACE : Hard Drop");//15,18자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 16); printf("◁  ▷ : Left / Right   P   : Pause");//15,19자리에 출력함

	gotoxy(STATUS_X_ADJ, y + 17); printf("  ▽   : Soft Drop     ESC  : Quit");//15,20자리에 출력함


}



void draw_main(void) { //게임판 그리는 함수  00형태 return값없음  

	int i, j;//정수변수 i,j를 선언과 초기화



	for (j = 1; j < MAIN_X - 1; j++) { //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 

		if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;// 게임 맵에서 3,1~9가지 비어있다면 그 구역에 벽을 만들어줌
		//printf(". ");이런형태의 천장임

	}

	for (i = 0; i < MAIN_Y; i++) {//0~22까지 반복

		for (j = 0; j < MAIN_X; j++) {//0~10까지 반복

			if (main_cpy[i][j] != main_org[i][j]) { //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.

												//이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
												//즉 위에서 main_cpy의 배열을 100으로 초기화를 하였기때문에,
												//main_org는 0과 벽돌로 채워져있음 그 부분만 다시 그림,

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);//즉 3+j,1+i 구간에 //천장은 3부터 시작이고,
				//좌우 벽 은 1부터 시작

				switch (main_org[i][j]) {//main_org 즉 게임판의 [x][y] 값이

				case EMPTY: //빈칸모양  //0이면

					printf("  ");

					break;

				case CEILLING: //천장모양 

					printf(". ");

					break;

				case WALL: //벽모양 

					printf("▩");

					break;

				case INACTIVE_BLOCK: //굳은 블럭 모양  

					printf("□");

					break;

				case ACTIVE_BLOCK: //움직이고있는 블럭 모양  

					printf("■");

					break;

				}

			}

		}

	}

	for (i = 0; i < MAIN_Y; i++) { //게임판을 그린 후 main_cpy에 복사  

		for (j = 0; j < MAIN_X; j++) {

			main_cpy[i][j] = main_org[i][j]; //임시cpy 배열에 main_org배열을 저장
			//main_cpy는 임시 배열임.

		}

	}

}



void new_block(void) { //새로운 블록 생성  00형태 함수 return값 없음

	int i, j;//정수변수 i,j를 선언



	bx = (MAIN_X / 2) - 1; //블록 생성 위치x좌표(게임판의 가운데) 

	by = 0;  //블록 생성위치 y좌표(제일 위) 

	b_type = b_type_next; //다음블럭값을 가져옴 

	b_type_next = rand() % 7; //다음 블럭을 만듦 

	b_rotation = 0;  //회전은 0번으로 가져옴 



	new_block_on = 0; //new_block flag를 끔  



	for (i = 0; i < 4; i++) { //게임판 bx, by위치에 블럭생성  

		for (j = 0; j < 4; j++) {

			if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK; //
			//위에서 만든 블럭 4차원배열이 1인 경우  게임판의 배열을 블록모양을 만듬 ,배열 가운데에 블록 생성
			//printf("■"); 이런 모양으로 생성
			

		}

	}

	for (i = 1; i < 3; i++) { //게임상태표시에 다음에 나올블럭을 그림 

		for (j = 0; j < 4; j++) {

			if (blocks[b_type_next][0][i][j] == 1) { //b_type_next는 랜덤값이기 때문에, 위에서 rand()%7을 하였기 때문에,
				//0~6까지 나옴  즉, 0~6타입의 회전값이 0인(초기형태) i,j 입니다.

				gotoxy(STATUS_X_ADJ + 2 + j, i + 6); //변수 STATUS_X_ADJ=15이기때문에, 17+j값,i+6 값으로 이동함.
				//블록은 총 4개로 구성되어있다 .■■■■ 그러므로 0~3까지 반복하며,x좌표는 1~2
				//블록의 최대 초기형태는 2x4 칸내에서 표현 가능 하다 
				//그러므로 1~2반복 0~3반복이다.

				printf("■"); //그리고 블럭모양으로 출력

			}

			else {

				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);//그게 아니라면, 17+j,j+6값으로 이동 

				printf("  ");//출력

			}

		}

	}

}



void check_key(void) {//각종 키를 눌렀을때 00형태함수 return값 없음

	key = 0; //키값 초기화  



	if (_kbhit()) { //kbhit함수는 현재 키보드가 눌렸는지 여부를 bool형으로 반환 해주는 함수입니다. 비표준함수
		//헤더로 conio.h를 추가하여야함.
		//반환하는 함수 getch()함수와 같이 쓰면 효율성이 좋다.

		key = _getch(); //키값을 받음  입력받은것을 화면에 출력 x

		if (key == 224) { //방향키인경우  방향키 아스키코드  ↑는224 72 두번 반환 2바이트 짜리 확장키
			//↓ 224 80 두번 반환
			//←224 75 두번 반환  →224 77 두번 반환

			do { key = _getch(); } while (key == 224);//방향키지시값을 버림  방향키 아스키 코드는 두번 반환 
			//하기 때문에,

			switch (key) { //위에서 입력된 값 눌렀을때,

			case LEFT: //왼쪽키 눌렀을때   즉 75

				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				//확인하는 함수 현재 이동중인 블록의 x좌표-1,y좌표,회전모양이 겹치지 않는다면, 왼쪽이동

				break;                            //왼쪽으로 갈 수 있는지 체크 후 가능하면 이동

			case RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨 

				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				//확인하는 함수 현재 이동중인 블록의 x좌표-1,y좌표,회전모양이 겹치지 않는다면, 오른쪽이동
				break;

			case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨 

				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				//확인하는 함수 현재 이동중인 블록의 x좌표-1,y좌표,회전모양이 겹치지 않는다면, 밑쪽이동
				break;

			case UP: //위쪽 방향키 눌렀을때 

				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				//확인하는 함수 현재 이동중인 블록의 x좌표-1,y좌표,회전했을때모양이 겹치지 않는다면, 회전
				//회전할 수 있는지 체크 후 가능하면 회전

				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
				
			}                    //바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)

		}

		else { //방향키가 아닌경우 

			switch (key) {

			case SPACE: //스페이스키 눌렀을때 

				space_key_on = 1; //스페이스키 flag를 띄움 

				while (crush_on == 0) { //바닥에 닿을때까지 이동시킴 

					drop_block();//drop_block 함수 호출

					score += level; // hard drop 보너스

					gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); //점수 표시  
					//gotoxy함수 는 정해진 좌표로 이동 STATUS_X_ADJ는 15 이고, STATUS_Y_SCORE는 static 변수이므로
					//위쪽에 선언 되어있지만, draw_map 함수에서 12로 초기화 하였기 때문에,
					//15,12 위치에 score를 공백 + 6자리를 차지하고 출력

				}

				break;//switch 문 탈출 코드

			case P: //P(대문자) 눌렀을때 

			case p: //p(소문자) 눌렀을때 

				pause(); //일시정지 함수 호출

				break;//switch문 탈출코드

			case ESC: //ESC눌렀을때 

				system("cls"); //화면을 지우고 

				exit(0); //게임종료 

			}

		}

	}

	while (_kbhit()) _getch(); //키버퍼를 비움 

}



void drop_block(void) {//0 0 형태 함수 return 값 없음

	int i=0, j=0;//정수 변수 i,j를 선언과 초기화



	if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0; //밑이 비어있으면 crush flag 끔 
	    //&&연산이기때문에, crush_on 플래그가 켜져있을때 그리고 ,chechk_crush가 1이면 crush_on 플래그를 끔

	if (crush_on && check_crush(bx, by + 1, b_rotation) == false) { //밑이 비어있지않고 crush flag가 켜저있으면 
		//crush_on플래그나 check_crush 둘중 하나의 플래그라도 꺼져있으면, 거짓

		for (i = 0; i < MAIN_Y; i++) { //현재 조작중인 블럭을 굳힘 

			for (j = 0; j < MAIN_X; j++) {

				if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
				//main_org가 블록으로 채워져있으면  main_org배열을 굳은 블럭 모양으로 바꾼다.
				//printf("□"); 이런 형태
				
			}

		}

		crush_on = 0; //flag를 끔 

		check_line(); //라인체크를 함 

		new_block_on = 1; //새로운 블럭생성 flag를 켬    

		return; //함수 종료 

	}

	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); //밑이 비어있으면 밑으로 한칸 이동 
	//chek_crush함수가 true를 반환했을때, move_block 함수에 DOWN을 매개변수로 호출
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++; //밑으로 이동이 안되면  crush flag를 켬
	//check_crush함수가 false를 반환했을때, crush_on을 증가 시켜 crush flag를 켬.
}



int check_crush(int bx, int by, int b_rotation) { //지정된 좌표와 회전값으로 충돌이 있는지 검사  11형태 함수 return 값 있음
	

	int i, j;//정수변수 i,j를 선언과 초기화



	for (i = 0; i < 4; i++) {

		for (j = 0; j < 4; j++) { //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 

			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0) return false;
			//즉 블록형태가 1인 것과 게임맵인 main_org배열에 저장되어있는 블록의 값이 같다면 
			//false를 리턴 
			   // 예를들어 앞에서 게임맵에 블록을 그렸습니다. ■■ 이런 블록이 맵에 저장이 되었고,    
			// 이 블록이 지금 떨어지는 블록의 모양과 겹친다면 ■■     false 리턴
		}

	}

	return true; //하나도 안겹치면 true리턴 

};



void move_block(int dir) { //블록을 이동시킴  01형태 함수 리턴값 없음

	int i, j;//정수변수 i,j를 선언



	switch (dir) {//switch문  dir을 매개변수로 사용

	case LEFT: //왼쪽방향 

		for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움  0~3 반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
				//블록의 형태가 1이면 임시맵인 main_org배열의 x축과 y축을 반복을 돌면서, 비워라.
				//bx와 by는 현재 이동중인 블록의 좌표이다 //이동하면 그자리에있던 블록을 지워라
				//라고 해석됨.
			}

		}

		for (i = 0; i < 4; i++) { //왼쪽으로 한칸가서 active block을 찍음 //0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.
				

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
				//블록형태가 1이면 main_org배열의 by+i축과 x축 라인+1을 active_block으로 바꿔라.active_block은 
				//printf("■"); 형태이다. 즉 왼쪽으로 한칸이동해서 블록을 생성해라.
			}

		}

		bx--; //좌표값 이동 

		break;//switch문 탈출



	case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 

		for (i = 0; i < 4; i++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
				//블록의 형태가 1이면 임시맵인 main_org배열의 x축과 y축을 반복을 돌면서, 비워라.
					//bx와 by는 현재 이동중인 블록의 좌표이다 //이동하면 그자리에있던 블록을 지워라
					//라고 해석됨.
			}

		}

		for (i = 0; i < 4; i++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
				//블록형태가 1이면 main_org배열의 by+i축과 x축 라인-1을 active_block으로 바꿔라.active_block은 
				//printf("■"); 형태이다. 즉 오른쪽으로 한칸이동해서 블록을 생성해라.
			}

		}

		bx++;//bx증가  움직이는 블록의 x좌표 증가

		break;//switch문 탈출



	case DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작

		for (i = 0; i < 4; i++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
				//블록의 형태가 1이면 임시맵인 main_org배열의 x축과 y축을 반복을 돌면서, 비워라.
						//bx와 by는 현재 이동중인 블록의 좌표이다 //이동하면 그자리에있던 블록을 지워라
						//라고 해석됨.
			}

		}

		for (i = 0; i < 4; i++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
				//블록형태가 1이면 main_org배열의 by+i축과 x축 라인bx+j을 active_block으로 바꿔라.active_block은 
			//printf("■"); 형태이다. 즉 아래쪽으로 한칸이동해서 블록을 생성해라.
			}

		}

		by++;//by를 증가 즉 움직이는 블록의 y좌표 증가

		break;//swtich문 탈출



	case UP: //키보드 위쪽 눌렀을때 회전시킴. 

		for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움  //0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
				//블록의 형태가 1이면 임시맵인 main_org배열의 x축과 y축을 반복을 돌면서, 비워라.
						//bx와 by는 현재 이동중인 블록의 좌표이다 //이동하면 그자리에있던 블록을 지워라
						//라고 해석됨.
			}

		}

		b_rotation = (b_rotation + 1) % 4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림) 

		for (i = 0; i < 4; i++) { //회전된 블록을 찍음 

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
				//블록 형태가 1이면     임시맵인 main_org를 by+i와 bx+j 를 active_block으로 바꿈
				//printf("■"); 형태이다.
			}

		}

		break;//스위치문 탈출



	case 100: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 

			  //이를 동작시키는 특수동작 

		for (i = 0; i < 4; i++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
				//블록의 형태가 1이면 임시맵인 main_org배열의 x축과 y축을 반복을 돌면서, 비워라.
						//bx와 by는 현재 이동중인 블록의 좌표이다 //이동하면 그자리에있던 블록을 지워라
						//라고 해석됨.
				
			}

		}

		b_rotation = (b_rotation + 1) % 4;///b_rotation을 블록의 형태는 4개니깐, 순차적으로 바뀜.
		//0형태(초기형태) 1형태(한번바뀐형태) 2형태,3형태  형태를 한 번 증가시켜서 바꿔라

		for (i = 0; i < 4; i++) {//회전된 블록을 찍음

			for (j = 0; j < 4; j++) {//0~3반복 모든 블록은 4x4형태에서 표현가능하기 때문이다.

				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
				//블록 형태가 1이면     임시맵인 main_org를 by+i와 bx+j 를 active_block으로 바꿈
				//printf("■"); 형태이다.
			}

		}

		by--;//by 좌표를 한칸 줄임.즉 블록은 y좌표가 계속 증가하지만, 회전할때는 멈춘다는 뜻임.

		break;//switch문 탈출

	}

}



void check_line(void) { //00형태의 함수 리턴값 없음

	int i, j, k, l;//정수변수 i,j,k,l을 선언



	int    block_amount; //한줄의 블록갯수를 저장하는 변수 

	int combo = 0; //콤보갯수 저장하는 변수 지정및 초기화 



	for (i = MAIN_Y - 2; i > 3;) { //i=MAIN_Y-2 : 밑쪽벽의 윗칸부터,  i>3 : 천장(3)아래까지 검사 

		block_amount = 0; //블록갯수 저장 변수 초기화 

		for (j = 1; j < MAIN_X - 1; j++) { //벽과 벽사이의 블록갯루를 셈  main_x는 11이다. 1~10까지 반복

			if (main_org[i][j] > 0) block_amount++;//임시맵인 main_org[i][j]가 0보다 크면, block amount값을 증가시킴,

		}

		if (block_amount == MAIN_X - 2) { //블록이 가득 찬 경우 

			if (level_up_on == 0) { //레벨업상태가 아닌 경우에(레벨업이 되면 자동 줄삭제가 있음) 

				score += 100 * level; //점수추가 

				cnt++; //지운 줄 갯수 카운트 증가 

				combo++; //콤보수 증가  

			}

			for (k = i; k > 1; k--) { //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 

				for (l = 1; l < MAIN_X - 1; l++) {

					if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
					//임시맵인 main_org이 CEILLING 이 아닐때 ,임시맵인 main_org의 맨 위 x축을 한칸 없앰,
					if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;

					//윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 

				}

			}

		}

		else i--;//아니면 i값을 후위감소 시킴/

	}

	if (combo) { //줄 삭제가 있는 경우 점수와 레벨 목표를 새로 표시함  

		if (combo > 1) { //2콤보이상인 경우 경우 보너스및 메세지를 게임판에 띄웠다가 지움 

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);//gotoxy함수 x,y값에 이동함/
			//MAIN_X_ADJ는 15+MAIN_X=11 /2 즉 20, MAIN_Y_ADJ는 1 +지금 움직이는 블록 좌표-2 자리에     combo변수 출력

			Sleep(500);//sleep단위는 0.001초 즉 0.5초

			score += (combo * level * 100);//점수는 combo* level*100한값을 대입연산

			reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.

//(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨) 

		}

		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);//gotoxy함수 입력된 좌표로 이동
		//즉 STATUS_X_ADJ는 15 STATUS_Y_GOAL은 4 자리에
		//GOAL : 5자리 차지하고, 삼항연산자 cnt값이 10이하이면 10-cnt 값을 출력 아니면 0출력

		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);//gotoxy 함수 입력된 좌표로 이동하는 함수15,12 자리에 
		// 띄우고 6칸차지한후 score 출력.

	}

}



void check_level_up(void) {//00형태 함수 return값 없음

	int i, j;//정수변수 i,j를 선언



	if (cnt >= 10) { //레벨별로 10줄씩 없애야함. 10줄이상 없앤 경우 

		draw_main();//draw_main함수 호출 즉 새로운 맵을 그림.

		level_up_on = 1; //레벨업 flag를 띄움 

		level += 1; //레벨을 1 올림 

		cnt = 0; //지운 줄수 초기화   



		for (i = 0; i < 4; i++) {

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);//gotoxy함수 main_x_adj는 15+11/2-3 =17 ,main_y_adj=1+4 5자리에 

			printf("             ");//공백 출력

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);//gotoxy함수 main_x_adj 18,7자리에

			printf("             ");//공백 출력

			Sleep(200);//0.2초 딜레이



			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			//17, 5 자리에
			printf("☆LEVEL UP!☆"); //LEVEL UP 출력

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			//18,7자리에
			printf("☆SPEED UP!☆");//출력
			
			Sleep(200);//0.2초 딜레이

		}

		reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.

//(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨) 



		for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) { //레벨업보상으로 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌 

			for (j = 1; j < MAIN_X - 1; j++) { //1부터 9까지 돌면서

				main_org[i][j] = INACTIVE_BLOCK; // 줄을 블록으로 모두 채우고 
				//printf("□");로 채움

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // 별을 찍어줌.. 이뻐보이게 

				printf("★");

				Sleep(20);//0.02초 딜레이

			}

		}

		Sleep(100); //별찍은거 보여주기 위해 0.1초delay 

		check_line(); //블록으로 모두 채운것 지우기

//.check_line()함수 내부에서 level up flag가 켜져있는 경우 점수는 없음.         

		switch (level) { //레벨별로 속도를 조절해줌. 

		case 2: //level이 2일때

			speed = 50;//속도 변수 50대입 위에서 sleep 함수에 speed를 대입 하였음. 즉 0.05초 딜레이

			break;//탈출

		case 3://level이 3일때

			speed = 25;// 속도 변수 25대입   위에서 sleep함수에 speed를 대입하였음, 즉 0.025초 딜레이

			break;//탈출

		case 4://levle이 4일때

			speed = 10;//속도 변수 10대입 위에서 sleep함수에 speed를 대입하였음. 즉 0,01초 딜레이

			break;//탈출

		case 5://level이 5일때

			speed = 5;//속도변수 5대입 위에서 sleep함수에 speed를 대입.즉 0,005초 딜레이

			break;//탈출

		case 6://level이 6일때

			speed = 4;//속도변수 4대입 위에서 sleep함수에 speed를 대입 즉 0.004초 딜레이

			break;//탈출

		case 7://level이 7일때

			speed = 3;//속도변수 3대입 위에서 sleep함수에 speed를 대입 즉 0.003초 딜레이

			break;//탈출

		case 8://level이 8일때

			speed = 2;//속도변수 2대입 위에서 sleep함수에 speed를 대입 즉 0.002초 딜레이

			break;//탈출

		case 9://level이 9일때

			speed = 1;//속도변수 1대입 위에서 sleep함수에 speed를 대입 즉 0.001초 딜레이

			break;//탈출

		case 10://level이 10일때

			speed = 0;//속도변수 0대입 위에서 sleep함수에 speed를 대입 0초딜레이

			break;//탈출

		}

		level_up_on = 0; //레벨업 flag꺼줌



		gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨표시 15,3 자리에 
		//
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // 레벨목표 표시  15,4자리에



	}

}



void check_game_over(void) {//00함수 리턴값 없음

	int i;//정수 변수 i를 선언



	int x = 5;//정수변수 x에 5를 대입

	int y = 5;//정수변수 y에 5를 대입



	for (i = 1; i < MAIN_X - 2; i++) {//i를 1부터 8까지 i를 반복

		if (main_org[3][i] > 0) { //천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버 

			gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");  //좌표 5,5자리에  표시//게임오버 메세지 

			gotoxy(x, y + 1); printf("▤                              ▤"); //좌표 5,6자리에 표시

			gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");//5,7자리에 표시

			gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");//5,8자리에 표시

			gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");//5,9자리에 표시

			gotoxy(x, y + 5); printf("▤   YOUR SCORE:  %3d           ▤", score);//5,10자리에 표시

			gotoxy(x, y + 6); printf("▤                              ▤");//5,11자리에 표시

			gotoxy(x, y + 7); printf("▤       아무키나 누르세요	  ▤");//5,12자리에 표시

			gotoxy(x, y + 8); printf("▤                              ▤");//5,13자리에 표시

			gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");//5,14자리에 표시

			if (score > best_score) { //최고기록 갱신시 

				FILE* file; //score.dat에 점수 저장                
				fopen_s(&file, "score.dat", "wt");


				gotoxy(x, y + 6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");



				if (file == 0) { //파일 에러메세지  

					gotoxy(0, 0);

					printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");

				}

				else {

					fprintf(file, "%d", score);

					fclose(file);

				}

			}

			time_t now;
			time(&now);
			_getch();//키입력시 까지 대기
			system("cls");//표시된것 모두 지움
			
			key = _getch(); MYSQL* connection = NULL, conn;//MYSQL의 구조체로 연결정보를 관리하는 포인터 connection 선언
			//conn이라는 구조체를 접근하는 구조체 변수 선언
			MYSQL_RES* sql_result;//mysql_res라는 구조체를 이용해서 받아오는 결과값의 시작번지를 포인터sql_result 가 가리키고 있음
			MYSQL_ROW sql_row;//문자열을 배열로 가지고 오기 위해서,문자열배열을 가지고 있는,
			//sql의 구조체의 변수를 이용해서, row단위로 문자열을 읽어오면 됨,

			int query_stat = 0;//쿼리가 제대로 전달됬는지 확인하기 위한 정수변수 선언과 초기화
			char ans = 0;//정수변수 ans 선언  y와 n값을 받기위해서
			char id[20] = { 0, };//id의 값을 받기위해 id배열을 만듬
			char query[255] = { 0, };//쿼리문을 포함한 id와 passwd를 가야함.
			int num = 0;

			//초기화
			mysql_init(&conn);//mysql_init이라는 함수로 mysql변수 conn을 이용해서 초기화 작업

			//한글문자셋 설정
			mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
			mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

			//연결 mysql_real_connect
			connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);
			//connection이라는 포인터 변수에 mysql_real_connect라는 함수로 인자가 8가지 들어감.
			//구조체정보, ipaddress,id,password,dbname,포트번호,null 값,0
			if (connection == NULL) {//만약에 connection이 null이면
				fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));//화면에 출력 stderr형태로 
				return 1;//리턴을1을 해서 비정상종료를 알림.
			}
			query_stat = mysql_query(connection, "select id,passwd,day from ranking_2 ORDER by 2 DESC");//query_stat에다가,mysql_query 
			//db에있는 모든 데이터를 가져와라
			if (query_stat != 0) {//quert_stat이 0이 아니면
				fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn)); //화면에 출력 stderr형태로
				return 1;//리턴을 1을 해서 비정상 종료를 알림.
			}

			sql_result = mysql_store_result(connection);//sql_result 포인터 변수에 connection정보를 저장함.


			printf("%20s %20s %20s %20s\n","순위", "닉네임", "점수","날짜");//10칸을 차지하고, 화면에 아이디와 점수를 출력함
			printf("========================================================================================================\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {//반복을 돌면서,sql_row 단위로 가져옴(구조체 변수)
				//mysql_fetch_row함수를 이용해서 sql_result결과를  NULL이 아닐때까지, 반복을 돌림.
				
				num++;
				
					if (num == 1) {
						printf("\033[1;33m%20d %20s %20s\t %40s\033[0m\n\n\n", num, sql_row[0], sql_row[1], sql_row[2]);//화면에 10칸을 가지고, sql_row와 sql_row[1]값을 출력
					}
					else {
						printf("% 20d %20s %20s\t %40s\n\n\n", num, sql_row[0], sql_row[1], sql_row[2]);
					}
				
			}

			mysql_free_result(sql_result);//메모리를 점유하고 있기때문에, 메모리를 해제해줘야함.

			printf("입력하시겠습니까?: ");//입력 안내문
			scanf("%c", &ans);//ans값을 입력받음
			scanf("%*c");//엔터키를 없애주기 위한 입력문

			if (ans == 'y' || ans == 'Y') { //ans 값이 y나 Y이면
				
				printf("닉네임 : ");//id 출력문
				fgets(id, 20, stdin);//파일로 부터 문자열을 가져오는 함수, 파일에서 부터
				//1번째 인자 가져온 문자열을 넣는 변수,2번째 인자 한번에 가지고올 문자열의 길이를 넣는 변수
				//3번째 인자:가지고 온 문자열을 반환하거나,파일의 끝에 도달했을때는 널 포인터 반환.
				CHOP(id);//엔터키들어오면 엔터키 공백으로 바꿔주는 함수.


				sprintf(query, "insert into ranking_2 values" "('%s','%d','%s')", id, score, ctime(&now));
				//query에 id와 score를 넣어줌 

				query_stat = mysql_query(connection, query);//query_stat에 connection정보와 query를 넣어줌 
				//return값을 받기위해,
				if (query_stat != 0) {//query_stat이 0이아니면
					fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));//화면에 출력 stderr형태로
					return 1;//비정상 종료를 나타냄
				}
			}
			else if (ans == 'n' || ans == 'N') {//ans가 n또는 N이면
			
				

				reset();//reset 함수 호출 
				
				sql_result = mysql_store_result(connection); //mysql_store_result(connection)의 정보를 sql_result에 
				//대입

				

			}
			mysql_close(connection);//mysql_close함수를 이용해서 connection 정보를 닫아줌

			while (_kbhit()) _getch();//키 입력이 있으면, 키 입력할때 까지대기,


			reset();//reset 함수 호출

		}

	}

}



void pause(void) { //게임 일시정지 함수  00형태 함수 리턴값 없음

	int i, j;//정수변수 i,j를 선언



	int x = 5;//정수변수 x에 5를 대입

	int y = 5;//정수변수 y에 5를 대입



	for (i = 1; i < MAIN_X - 2; i++) { //게임 일시정지 메세지  MAIN_X는 11이기때문에, 1~8까지 돌며 반복 

		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");//5,5자리에 출력

		gotoxy(x, y + 1); printf("▤                              ▤");//5,6자리에 출력

		gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");//5,7자리에 출력

		gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");//5,8자리에 출력

		gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");//5,9자리에 출력

		gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");//5,10자리에 출력

		gotoxy(x, y + 6); printf("▤                              ▤");//5,11자리에 출력

		gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");//5,12자리에 출력

	}

	_getch(); //키입력시까지 대기 



	system("cls"); //화면 지우고 새로 그림 

	reset_main_cpy();//게임판을 다시그림

	draw_main();//draw_main 함수 바뀐부분만 다시그림

	draw_map();//게임시작할때 초기화면 그려줌



	for (i = 1; i < 3; i++) { // 다음블록 그림 

		for (j = 0; j < 4; j++) {//반복 0~3까지

			if (blocks[b_type_next][0][i][j] == 1) { //블록함수중 1인 부분을

				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);//main_x는 11 main_x_adj는 3이므로 17+j  으로 이동하여

				printf("■");//출력

			}

			else {//아니면

				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6); //17+j,i+6 자리로 이동하여

				printf("  ");//공백 출력

			}

		}

	}

} //끝! 
