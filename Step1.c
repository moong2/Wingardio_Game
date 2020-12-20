#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>
#include "map1.h"
#include "resource.h"

#pragma comment(lib, "winmm.lib")
#pragma warning(disable : 4996)


//Color
#define RED 4
#define BLUE 1
#define GREEN 2
#define YELLOW 6
//Number Of Item
#define LIFE_ID 100
#define DIRECTION_ID 101
#define MOBSPEEDI_ID 102
#define BLOCKSPEEDI_ID 103
#define MOBSPEEDD_ID 104
#define BLOCKSPEEDD_ID 105
#define GOUP_ID 106
//Key
#define RIGHT 77
#define LEFT 75
#define DOWN 80
#define REVERSE_RIGHT 75
#define REVERSE_LEFT 77
#define REVERSE_DOWN 72
//GameBoard
#define GBOARD_HEIGHT 40
#define GBOARD_WIDTH  32
#define GBOARD_ORIGIN_X 8
#define GBOARD_ORIGIN_Y 10

COORD PC;
COORD curPos;

int player_Life = 3;
char obtainAlphabet[20] = { '\0' };   //PC가 먹은 글자
int mobSpeed = 100;
int blockSpeed = 1;

int item_ID;
int colorOfPC = 11;
char itemInformation[1000] = { '\0' };
int speed = 10;

int CurrentGameStage = 1;
int initFlag = 0;
int map_id = 0;
int element_id = 0;

int keyFlag = 0;
int previousPageFlag = 0;
int nextPageFlag = 0;

int flag1 = 0, flag2 = 0, flag3 = 0;
int block1_flag = 0; block2_flag = 0;
int obtain_item_n = 3;
COORD mob1, mob2, mob3;
COORD block1_left, block1_right, block2_left, block2_right;

void Black();
void MovingMob();
void MovingBlock();
void PlayerDown();
void RemoveCursor(void)
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void SetCurrentCursorPos(int x, int y)
{
    COORD position = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

COORD GetCurrentCursorPos(void)
{
    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;

    return curPoint;
}

void GameClear()
{

    Black();
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 72; j++)
        {
            if (Goal[i][j] == 1)
                printf("■");
            else
                printf("  ");
        }
        printf("\n");
    }
    //Black();
    exit(-1);

}
void copyArray()
{
    for (int i = 0; i < 41; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            map[(CurrentGameStage - 1) * 2][i][j] = map[(CurrentGameStage - 1) * 2 + 1][i][j];
            element[(CurrentGameStage - 1) * 2][i][j] = element[(CurrentGameStage - 1) * 2 + 1][i][j];
        }
    }
}
void RandomItemPosition()
{
    // 배열에 초기화 해줘야함

    // 조건:
    // 아래 블럭이 있어야함. 알파벳과 위치가 달라야함.
    // element[41][32]

    srand((unsigned int)time(NULL));
    //item_ID = (rand() % 15);

    int random_x, random_y;

    random_x = (((rand() % 6) + 1) * 5) + 3;
    random_y = (rand() % 30) + 1;

    while (1)
    {
        if (map[map_id][random_x + 1][random_y] != 0 && map[map_id][random_x + 1][random_y] != 4 && element[map_id][random_x][random_y] == '0' && element[map_id][random_x][random_y] != '?')
        {

            if (CurrentGameStage == 3 && (random_x == 23 || random_x == 33))
            {
                random_x = (((rand() % 6) + 1) * 5) + 3;
                random_y = (rand() % 30) + 1;
            }
            else
            {
                element[map_id][random_x][random_y] = '?';
                break;
            }
        }
        else
        {
            random_x = (((rand() % 6) + 1) * 5) + 3;
            random_y = (rand() % 30) + 1;
        }
    }
}
void DrawGameBlock()
{
    int y, x;
    RemoveCursor();
    SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y);
    curPos = GetCurrentCursorPos();

    for (y = 0; y < 40; y++)
    {
        for (x = 0; x < 31; x++)
        {
            SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + y);
            if (map[map_id][y][x] == 0)
            {
                if (element[element_id][y][x] >= 'a' && element[element_id][y][x] <= 'z' || element[element_id][y][x] == '?')
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    printf("%c ", element[element_id][y][x]);
                }
                else
                    printf("  ");
            }
            else if (map[map_id][y][x] % 10 == 1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                printf("■");
            }
            else if (map[map_id][y][x] % 10 == 2)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
                printf("■");
            }
            else if (map[map_id][y][x] % 10 == 3)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                printf("■");
            }
            else if (map[map_id][y][x] % 10 == 4)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                printf("■");
            }
            else if (map[map_id][y][x] % 10 == 5)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                printf("§");
            }
            else if (map[map_id][y][x] % 10 == 6)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorOfPC);
                PC = GetCurrentCursorPos();
                printf("♥");

            }
            else if (map[map_id][y][x] % 10 == 7)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                printf("■");
            }
        }
    }
    //SetCurrentCursorPos(curPos.X, curPos.Y);
}

void CurrentSituationPrint()
{
    int i = 20;

    SetCurrentCursorPos(80, i++);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    if (CurrentGameStage != 1) {
        printf("추가 미션: item 3개 이상 획득!!!");
        puts("                                                    ");
        SetCurrentCursorPos(80, i++);
        printf("추가 미션 실패시 미션 실패로 간주됩니다");
        //SetCurrentCursorPos(80, i++);
    }

    i = 30;
    SetCurrentCursorPos(80, i++);
    if (CurrentGameStage == 1)
        printf("WORD THAT MUST ACQUIRE : 오리");
    else if (CurrentGameStage == 2)
        printf("WORD THAT MUST ACQUIRE : 알파카");
    else
        printf("WORD THAT MUST ACQUIRE : 기린  ");
    SetCurrentCursorPos(80, i++);
    printf("LETTERS ACQUIRED : ");
    if (obtainAlphabet[0] == '\0')
        printf("                                  \n");
    else
        puts(obtainAlphabet);
    SetCurrentCursorPos(80, i++);
    printf("LIFE COUNT : %d", player_Life);

    if (CurrentGameStage != 1) {
        SetCurrentCursorPos(80, i++);
        printf("ITEM COUNT : %d / 3                                 \n", obtain_item_n);
    }

    SetCurrentCursorPos(80, i++);
    printf("------------------------------");
    SetCurrentCursorPos(80, i++);
    printf("<KEY MANUAL>");
    SetCurrentCursorPos(80, i++);
    printf("Right key : 오른쪽으로 이동");
    SetCurrentCursorPos(80, i++);
    printf("Left key : 왼쪽으로 이동");
    SetCurrentCursorPos(80, i++);
    printf("Down key : 포털 이동 및 블록 삭제");
    SetCurrentCursorPos(80, i++);
    printf("MOB : §");
    SetCurrentCursorPos(80, i++);
    if (CurrentGameStage != 1) {
        printf("----------item 설명-----------");
        puts("                                                    ");
        SetCurrentCursorPos(80, i++);
        puts(itemInformation);
    }
}


void DetectCollisionWithAlphabet(int arrX, int arrY)
{
    int len_obtainAlphabet = 0;
    for (int i = 0; obtainAlphabet[i] != '\0'; i++)
    {
        len_obtainAlphabet++;
    }
    obtainAlphabet[len_obtainAlphabet] = element[element_id][arrY][arrX];
    element[element_id][arrY][arrX] = '0';
    CurrentSituationPrint();
}


void initialize();
void IncreaseLife()
{
    player_Life++;
}
void ChangeDirection()
{
    if (keyFlag == 0)
        keyFlag = 1;
    else
        keyFlag = 0;
}

void MobSpeedUp()
{
    if (mobSpeed > 20) mobSpeed -= 10;
    else mobSpeed = 20;
}
void MobSpeedDown()
{
    if (mobSpeed < 180) mobSpeed += 10;
    else mobSpeed = 180;
}
void BlockSpeedUp()
{
    if (blockSpeed > 1) blockSpeed -= 1;
    else blockSpeed = 1;    
}
void BlockSpeedDown()
{
    if (blockSpeed < 3) blockSpeed += 1;
    else blockSpeed = 3;
}

void GoalAchievementTest();
void GoUp()
{
    Sleep(1000);
    initialize();
}

void DetectCollisionWithItem(int arrX, int arrY)
{
    element[element_id][arrY][arrX] = '0';
    srand((unsigned int)time(NULL));
    item_ID = (rand() % 15);  //0-14
    if (CurrentGameStage == 3) {
        if (item_ID == 0) item_ID = 100;
        else if (item_ID >= 1 && item_ID <= 3) item_ID = 101;
        else if (item_ID >= 5 && item_ID <= 6) item_ID = 102;
        else if (item_ID >= 9 && item_ID <= 10) item_ID = 103;
        else if (item_ID == 12) item_ID = 104;
        else if (item_ID == 13) item_ID = 105;
        else item_ID = 106;
    }
    else {
        if (item_ID == 0) item_ID = 100;
        else if (item_ID >= 1 && item_ID <= 4) item_ID = 101;
        else if (item_ID >= 5 && item_ID <= 8) item_ID = 102;
        else if (item_ID == 12) item_ID = 104;
        else item_ID = 106;
    }
    switch (item_ID)
    {
    case LIFE_ID:
        IncreaseLife();
        strcpy(itemInformation, "life 1개 증가               ");
        break;
    case DIRECTION_ID:
        ChangeDirection();
        strcpy(itemInformation, "상하좌우키 변환                  ");
        break;
    case MOBSPEEDI_ID:
        MobSpeedUp();
        strcpy(itemInformation, "mob speed 증가               ");
        break;
    case BLOCKSPEEDI_ID:
        BlockSpeedUp();
        strcpy(itemInformation, "block speed 증가             ");
        break;
    case MOBSPEEDD_ID:
        MobSpeedDown();
        strcpy(itemInformation, "mob speed 감소               ");
        break;
    case BLOCKSPEEDD_ID:
        BlockSpeedDown();
        strcpy(itemInformation, "block speed 감소             ");
        break;
    case GOUP_ID:
        GoUp();
        strcpy(itemInformation, "PC 가 맨 위로 올라갑니다      ");
        break;
    }
    obtain_item_n++;
}
int DetectCollisionPCWithBlock() {
    int arrCurX, arrCurY;
    int UnderBlock_Info;
    arrCurX = (PC.X - GBOARD_ORIGIN_X) / 2 - 1;
    arrCurY = (PC.Y - GBOARD_ORIGIN_Y);
    UnderBlock_Info = map[(CurrentGameStage - 1) * 2][arrCurY + 1][arrCurX] % 10;

    if (UnderBlock_Info == 1)
        UnderBlock_Info = RED;
    else if (UnderBlock_Info == 2)
        UnderBlock_Info = BLUE;
    else if (UnderBlock_Info == 3)
        UnderBlock_Info = GREEN;
    else if (UnderBlock_Info == 4)
        UnderBlock_Info = YELLOW;

    if (colorOfPC == UnderBlock_Info || UnderBlock_Info == 0)
        return 0;
    else if (colorOfPC != UnderBlock_Info && UnderBlock_Info != 0)
        return 1;
}

void DrawGameBoard() {
    int  x, y;
    for (y = 0; y <= GBOARD_HEIGHT; y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + y);
        if (y == GBOARD_HEIGHT)
            printf("└");
        else
            printf("│");
    }
    for (x = 1; x <= GBOARD_WIDTH - 2; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2 + 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("─");
    }
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2, GBOARD_ORIGIN_Y + y);
        if (y == GBOARD_HEIGHT)
            printf("┘");
        else
            printf("│");
    }

    SetCurrentCursorPos(GBOARD_WIDTH, 0);



    DrawGameBlock();
}



void initialize()
{
    //DrawGameBlock_init();
    initFlag = 1;
    colorOfPC = 11;
    copyArray();
    if (CurrentGameStage != 1) {
        RandomItemPosition();
        RandomItemPosition();
        RandomItemPosition();
        RandomItemPosition();
        RandomItemPosition();
        RandomItemPosition();
    }
    DrawGameBlock();
    CurrentSituationPrint();
    if (CurrentGameStage == 2) {

        mob1.X = 13;
        mob1.Y = 27;

        mob2.X = 18;
        mob2.Y = 2;

        mob3.X = 28;
        mob3.Y = 6;
    }
    if (CurrentGameStage == 3)
    {
        mob1.X = 28;
        mob1.Y = 4;

        mob2.X = 28;
        mob2.Y = 16;

        mob3.X = 13;
        mob3.Y = 20;

        block1_left.X = 24;
        block1_left.Y = 11;
        block1_right.X = 24;
        block1_right.Y = 19;

        block2_left.X = 34;
        block2_left.Y = 13;
        block2_right.X = 34;
        block2_right.Y = 18;
    }
}


void GameOver()
{

    Black();
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 72; j++)
        {
            if (gameOver[i][j] == 1)
                printf("■");
            else
                printf("  ");
        }
        printf("\n");
    }
    exit(-1);
}


void LifeCheck() {
    if (player_Life == 0) {
        CurrentSituationPrint();
        GameOver();
    }
    else if (player_Life > 0) {
        initialize();
    }
}


void DrawStage()
{
    DrawGameBlock();
    CurrentSituationPrint();
}

void DetectCollisionWithMob(int flag) {
    int arrX = (PC.X - GBOARD_ORIGIN_X) / 2 - 1, arrY = (PC.Y - GBOARD_ORIGIN_Y);
    if (flag == 1 && map[map_id][arrY + 1][arrX] == 5) {
        for (int i = 0; i < 7; i++) obtainAlphabet[i] = '\0';
        CurrentSituationPrint();
        player_Life -= 1;
        obtain_item_n = 0;
        Sleep(1000);
        LifeCheck();
    }
    else if (flag == 2 && map[map_id][arrY][arrX] == 5) {
        for (int i = 0; i < 7; i++) obtainAlphabet[i] = '\0';
        CurrentSituationPrint();
        obtain_item_n = 0;
        player_Life -= 1;
        Sleep(1000);
        LifeCheck();
    }
}
void drawline(int line) {
    int x;
    for (x = 0; x < 31; x++)
    {
        SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + line);
        if (map[map_id][line][x] == 0)
        {
            if (element[element_id][line][x] >= 'a' && element[element_id][line][x] <= 'z' || element[element_id][line][x] == '?')
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                printf("%c ", element[element_id][line][x]);
            }
            else
                printf("  ");
        }

        else if (map[map_id][line][x] == 5)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            printf("§");
        }
        else if (map[map_id][line][x] == 6)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorOfPC);
            PC = GetCurrentCursorPos();
            printf("♥");
        }
        else if (map[map_id][line][x] % 10 == 1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
            printf("■");
        }
        else if (map[map_id][line][x] % 10 == 2)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
            printf("■");
        }
        else if (map[map_id][line][x] % 10 == 3)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
            printf("■");
        }
        else if (map[map_id][line][x] % 10 == 4)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
            printf("■");
        }
    }
}

void MovingMob() {

    //mob1
    if (flag1 == 0 && map[map_id][mob1.X + 1][mob1.Y + 1] != 0 && map[map_id][mob1.X + 1][mob1.Y + 1] != 8) {
        map[map_id][mob1.X][mob1.Y] = 0;
        map[map_id][mob1.X][mob1.Y + 1] = 5;
        mob1.Y += 1;
    }
    else if (map[map_id][mob1.X + 1][mob1.Y + 1] == 0 || map[map_id][mob1.X + 1][mob1.Y + 1] == 8) {
        flag1 = 1;
    }
    if (flag1 == 1 && map[map_id][mob1.X + 1][mob1.Y - 1] != 0 && map[map_id][mob1.X + 1][mob1.Y - 1] != 8) {
        map[map_id][mob1.X][mob1.Y] = 0;
        mob1.Y -= 1;
        map[map_id][mob1.X][mob1.Y] = 5;

    }
    else if (map[map_id][mob1.X + 1][mob1.Y - 1] == 0 || map[map_id][mob1.X + 1][mob1.Y - 1] == 8) {
        flag1 = 0;
    }
    drawline(mob1.X);

    //mob2
    if (flag2 == 0 && map[map_id][mob2.X + 1][mob2.Y + 1] != 0 && map[map_id][mob2.X + 1][mob2.Y + 1] != 8) {
        map[map_id][mob2.X][mob2.Y] = 0;
        map[map_id][mob2.X][mob2.Y + 1] = 5;
        mob2.Y += 1;

    }
    else if (map[map_id][mob2.X + 1][mob2.Y + 1] == 0 || map[map_id][mob2.X + 1][mob2.Y + 1] == 8) {
        flag2 = 1;
    }
    if (flag2 == 1 && map[map_id][mob2.X + 1][mob2.Y - 1] != 0 && map[map_id][mob2.X + 1][mob2.Y - 1] != 8) {
        map[map_id][mob2.X][mob2.Y] = 0;
        mob2.Y -= 1;
        map[map_id][mob2.X][mob2.Y] = 5;

    }
    else if (map[map_id][mob2.X + 1][mob2.Y - 1] == 0 || map[map_id][mob2.X + 1][mob2.Y - 1] == 8) {
        flag2 = 0;
    }
    drawline(mob2.X);

    //mob3
    if (flag3 == 0 && map[map_id][mob3.X + 1][mob3.Y + 1] != 0 && map[map_id][mob3.X + 1][mob3.Y + 1] != 8) {
        map[map_id][mob3.X][mob3.Y] = 0;
        map[map_id][mob3.X][mob3.Y + 1] = 5;
        mob3.Y += 1;

    }
    else if (map[map_id][mob3.X + 1][mob3.Y + 1] == 0 || map[map_id][mob3.X + 1][mob3.Y + 1] == 8) {
        flag3 = 1;
    }
    if (flag3 == 1 && map[map_id][mob3.X + 1][mob3.Y - 1] != 0 && map[map_id][mob3.X + 1][mob3.Y - 1] != 8) {
        map[map_id][mob3.X][mob3.Y] = 0;
        mob3.Y -= 1;
        map[map_id][mob3.X][mob3.Y] = 5;

    }
    else if (map[map_id][mob3.X + 1][mob3.Y - 1] == 0 || map[map_id][mob3.X + 1][mob3.Y - 1] == 8) {
        flag3 = 0;
    }
    drawline(mob3.X);
    DetectCollisionWithMob(2);

    Sleep(mobSpeed);
}

void ShiftRight()
{
    int arrCurX_PC = (PC.X + 2 - GBOARD_ORIGIN_X) / 2 - 1;
    int arrCurY_PC = (PC.Y - GBOARD_ORIGIN_Y);

    //move right
    if (map[map_id][PC.Y - GBOARD_ORIGIN_Y + 1][(PC.X - GBOARD_ORIGIN_X) / 2] != 0)
    {
        if (element[element_id][arrCurY_PC][arrCurX_PC] >= 'a' && element[element_id][arrCurY_PC][arrCurX_PC] <= 'z') DetectCollisionWithAlphabet(arrCurX_PC, arrCurY_PC);
        else if (element[element_id][arrCurY_PC][arrCurX_PC] == '?') DetectCollisionWithItem(arrCurX_PC, arrCurY_PC);
        map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 0;
        PC.X += 2;
        DetectCollisionWithMob(2);
        SetCurrentCursorPos(PC.X, PC.Y);

        map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 6;
        DrawStage();
    }
    // check color and then, if color == blockcolor -> movedown
    arrCurX_PC = (PC.X - GBOARD_ORIGIN_X) / 2 - 1;
    arrCurY_PC = (PC.Y - GBOARD_ORIGIN_Y);
    while (!DetectCollisionPCWithBlock())
    {
        MovingMob();
        PlayerDown();
        if (CurrentGameStage == 3) MovingBlock();
        DrawStage();
    }
}

//왼쪽이동
void ShiftLeft()
{
    int arrCurX_PC = (PC.X - 2 - GBOARD_ORIGIN_X) / 2 - 1;
    int arrCurY_PC = (PC.Y - GBOARD_ORIGIN_Y);

    if (map[map_id][PC.Y - GBOARD_ORIGIN_Y + 1][(PC.X - GBOARD_ORIGIN_X) / 2 - 2] != 0)
    {
        if (element[element_id][arrCurY_PC][arrCurX_PC] >= 'a' && element[element_id][arrCurY_PC][arrCurX_PC - 1] <= 'z') DetectCollisionWithAlphabet(arrCurX_PC, arrCurY_PC);
        else if (element[element_id][arrCurY_PC][arrCurX_PC] == '?') DetectCollisionWithItem(arrCurX_PC, arrCurY_PC);
        map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 0;
        PC.X -= 2;
        DetectCollisionWithMob(2);
        SetCurrentCursorPos(PC.X, PC.Y);

        map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 6;
        DrawStage();
    }
    arrCurX_PC = (PC.X - GBOARD_ORIGIN_X) / 2 - 1;
    arrCurY_PC = (PC.Y - GBOARD_ORIGIN_Y);
    while (!DetectCollisionPCWithBlock())
    {
        MovingMob();
        PlayerDown();
        if (CurrentGameStage == 3) MovingBlock();

        DrawStage();
    }
}

void PortalBlock()
{
    int arrCurX_PC, arrCurY_PC;
    int tmp_COLOR;
    arrCurX_PC = (PC.X - GBOARD_ORIGIN_X) / 2 - 1;
    arrCurY_PC = (PC.Y - GBOARD_ORIGIN_Y);
    tmp_COLOR = map[map_id][arrCurY_PC + 1][arrCurX_PC] % 10;

    if (tmp_COLOR == 4)
    {
        if (CurrentGameStage == 1)
        {
            if (arrCurY_PC + 1 == 34)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                arrCurY_PC = 8;
                arrCurX_PC = 24;
                map[map_id][arrCurY_PC][arrCurX_PC] = 6;

                PC.X = (arrCurX_PC + 1) * 2 + GBOARD_ORIGIN_X;
                PC.Y = arrCurY_PC + GBOARD_ORIGIN_Y;
            }

            else if (arrCurY_PC + 1 == 9)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                arrCurY_PC = 33;
                arrCurX_PC = 6;
                map[map_id][arrCurY_PC][arrCurX_PC] = 6;

                PC.X = (arrCurX_PC + 1) * 2 + GBOARD_ORIGIN_X;
                PC.Y = arrCurY_PC + GBOARD_ORIGIN_Y;

            }
        }
        else if (CurrentGameStage == 2)
        {
            if (arrCurY_PC + 1 == 34)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                arrCurY_PC = 13;
                arrCurX_PC = 28;
                map[map_id][arrCurY_PC][arrCurX_PC] = 6;

                PC.X = (arrCurX_PC + 1) * 2 + GBOARD_ORIGIN_X;
                PC.Y = arrCurY_PC + GBOARD_ORIGIN_Y;
            }

            else if (arrCurY_PC + 1 == 14)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                arrCurY_PC = 33;
                arrCurX_PC = 6;
                map[map_id][arrCurY_PC][arrCurX_PC] = 6;

                PC.X = (arrCurX_PC + 1) * 2 + GBOARD_ORIGIN_X;
                PC.Y = arrCurY_PC + GBOARD_ORIGIN_Y;

            }

        }

        else if (CurrentGameStage == 3)
        {
            if (arrCurY_PC + 1 == 29)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                arrCurY_PC = 13;
                arrCurX_PC = 23;
                map[map_id][arrCurY_PC][arrCurX_PC] = 6;

                PC.X = (arrCurX_PC + 1) * 2 + GBOARD_ORIGIN_X;
                PC.Y = arrCurY_PC + GBOARD_ORIGIN_Y;
            }

            else if (arrCurY_PC + 1 == 14)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                arrCurY_PC = 28;
                arrCurX_PC = 24;
                map[map_id][arrCurY_PC][arrCurX_PC] = 6;

                PC.X = (arrCurX_PC + 1) * 2 + GBOARD_ORIGIN_X;
                PC.Y = arrCurY_PC + GBOARD_ORIGIN_Y;

            }
        }
        DrawStage();
    }
}

void initialize_GOAL()
{
    initFlag = 0;


    DrawGameBlock();
    CurrentSituationPrint();
}

void GoalAchievementTest()
{
    if ((strcmp(obtainAlphabet, "duck") == 0 ||
        strcmp(obtainAlphabet, "alpaca") == 0 ||
        strcmp(obtainAlphabet, "giraffe") == 0) && obtain_item_n >= 3)
    {

        obtainAlphabet[0] = '\0';
        for (int i = 0; i < 7; i++) obtainAlphabet[i] = '\0';
        colorOfPC = 11;

        CurrentGameStage++;
        if (CurrentGameStage == 2) {
            mob1.X = 13;
            mob1.Y = 27;

            mob2.X = 18;
            mob2.Y = 2;

            mob3.X = 28;
            mob3.Y = 6;
        }
        else if (CurrentGameStage == 3) {
            mob1.X = 28;
            mob1.Y = 4;

            mob2.X = 28;
            mob2.Y = 16;

            mob3.X = 13;
            mob3.Y = 20;

            block1_left.X = 24;
            block1_left.Y = 11;
            block1_right.X = 24;
            block1_right.Y = 19;

            block2_left.X = 34;
            block2_left.Y = 13;
            block2_right.X = 34;
            block2_right.Y = 18;
        }

        else if (CurrentGameStage == 4)
        {
            GameClear();
        }

        element_id = (CurrentGameStage - 1) * 2;
        map_id = (CurrentGameStage - 1) * 2;
        obtain_item_n = 0;
        initFlag = 0;
        DrawGameBlock();
        initialize();
        CurrentSituationPrint();
    }
    else
    {
        obtainAlphabet[0] = '\0';
        if (CurrentGameStage != 1) {
            obtain_item_n = 0;
        }
        for (int i = 0; i < 7; i++) obtainAlphabet[i] = '\0';
        player_Life--;
        LifeCheck();
    }
}
void DetectCollisionPCWithGoal()
{
    int arrX = (PC.X - GBOARD_ORIGIN_X) / 2 - 1, arrY = (PC.Y - GBOARD_ORIGIN_Y);
    if (map[map_id][arrY + 1][arrX] == 7)
    {
        GoalAchievementTest();
    }
    else
        return;
}
//블록 하강
void PlayerDown()
{
    int arrCurX_PC, arrCurY_PC;
    int tmp_COLOR;
    int playerdownBlock_COLOR;
    arrCurX_PC = (PC.X - GBOARD_ORIGIN_X) / 2 - 1;
    arrCurY_PC = (PC.Y - GBOARD_ORIGIN_Y);
    tmp_COLOR = map[map_id][arrCurY_PC + 1][arrCurX_PC] % 10;

    if (tmp_COLOR != 0)
    {
        if (tmp_COLOR == 1)
            colorOfPC = RED;
        else if (tmp_COLOR == 2)
            colorOfPC = BLUE;
        else if (tmp_COLOR == 3)
            colorOfPC = GREEN;
        else if (tmp_COLOR == 4)
            PortalBlock();
    }


    while (1)
    {

        if (DetectCollisionPCWithBlock() == 0)
        {

            if (CurrentGameStage == 3)
            {
                if (arrCurY_PC == block1_left.X || arrCurY_PC == block2_left.X) {
                    map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                    arrCurY_PC += 2;
                    map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                }
                else {
                    if (block1_flag == 0) // left 로 이동 시도
                    {
                        if (map[map_id][block1_left.X][block1_left.Y - 1] != 8) // 이동 가능
                        {
                            for (int i = block1_left.Y; i <= block1_right.Y; i++)
                            {
                                map[map_id][block1_left.X][i - 1] = map[map_id][block1_left.X][i];
                                element[element_id][block1_left.X - 1][i - 1] = element[element_id][block1_left.X - 1][i];

                            }
                            map[map_id][block1_left.X][block1_right.Y] = 0;
                            block1_left.Y--;
                            block1_right.Y--;
                        }
                        else // 이동 불가능
                        {
                            block1_flag = 1;
                        }
                    }
                    else if (block1_flag == 1) // right 로 이동 시도
                    {
                        if (map[map_id][block1_right.X][block1_right.Y + 1] != 8) // 이동 가능 
                        {
                            for (int i = block1_right.Y; i >= block1_left.Y; i--)
                            {

                                map[map_id][block1_left.X][i + 1] = map[map_id][block1_left.X][i];
                                element[element_id][block1_left.X - 1][i + 1] = element[element_id][block1_left.X - 1][i];

                            }
                            map[map_id][block1_left.X][block1_left.Y] = 0;
                            block1_left.Y++;
                            block1_right.Y++;
                        }

                        else // 이동 불가능
                        {
                            block1_flag = 0;
                        }
                    }


                    if (block2_flag == 0) // left 로 이동 시도
                    {
                        if (map[map_id][block2_left.X][block2_left.Y - 1] != 8) // 이동 가능
                        {
                            for (int i = block2_left.Y; i <= block2_right.Y; i++)
                            {

                                map[map_id][block2_left.X][i - 1] = map[map_id][block2_left.X][i];
                                element[element_id][block2_left.X - 1][i - 1] = element[element_id][block2_left.X - 1][i];
                            }
                            map[map_id][block2_left.X][block2_right.Y] = 0;
                            block2_left.Y--;
                            block2_right.Y--;
                        }
                        else // 이동 불가능
                        {
                            block2_flag = 1;
                        }
                    }

                    else if (block2_flag == 1)// right 로 이동 시도
                    {
                        if (map[map_id][block2_right.X][block2_right.Y + 1] != 8) // 이동 가능 
                        {
                            for (int i = block2_right.Y; i >= block2_left.Y; i--)
                            {

                                map[map_id][block2_left.X][i + 1] = map[map_id][block2_left.X][i];
                                element[element_id][block2_left.X - 1][i + 1] = element[element_id][block2_left.X - 1][i];
                            }
                            map[map_id][block2_left.X][block2_left.Y] = 0;
                            block2_left.Y++;
                            block2_right.Y++;
                        }

                        else // 이동 불가능
                        {
                            block2_flag = 0;
                        }
                    }

                }
                Sleep(blockSpeed * 100);
            }

            if (element[element_id][arrCurY_PC + 1][arrCurX_PC] >= 'a' && element[element_id][arrCurY_PC + 1][arrCurX_PC - 1] <= 'z') DetectCollisionWithAlphabet(arrCurX_PC, arrCurY_PC + 1);
            else if (element[element_id][arrCurY_PC + 1][arrCurX_PC] == '?') DetectCollisionWithItem(arrCurX_PC, arrCurY_PC + 1);
            if (item_ID != 106)
            {
                map[map_id][arrCurY_PC][arrCurX_PC] = 0;
                map[map_id][arrCurY_PC + 1][arrCurX_PC] = 6;
                PC.Y += 1; arrCurY_PC++;
            }
            else item_ID = 0;
            //mob1
            if (flag1 == 0 && map[map_id][mob1.X + 1][mob1.Y + 1] != 0 && map[map_id][mob1.X + 1][mob1.Y + 1] != 8) {
                map[map_id][mob1.X][mob1.Y] = 0;
                map[map_id][mob1.X][mob1.Y + 1] = 5;
                mob1.Y += 1;

            }
            else if (map[map_id][mob1.X + 1][mob1.Y + 1] == 0 || map[map_id][mob1.X + 1][mob1.Y + 1] == 8) {
                flag1 = 1;
            }
            if (flag1 == 1 && map[map_id][mob1.X + 1][mob1.Y - 1] != 0 && map[map_id][mob1.X + 1][mob1.Y - 1] != 8) {
                map[map_id][mob1.X][mob1.Y] = 0;
                mob1.Y -= 1;
                map[map_id][mob1.X][mob1.Y] = 5;

            }
            else if (map[map_id][mob1.X + 1][mob1.Y - 1] == 0 || map[map_id][mob1.X + 1][mob1.Y - 1] == 8) {
                flag1 = 0;
            }

            //mob2
            if (flag2 == 0 && map[map_id][mob2.X + 1][mob2.Y + 1] != 0 && map[map_id][mob2.X + 1][mob2.Y + 1] != 8) {
                map[map_id][mob2.X][mob2.Y] = 0;
                map[map_id][mob2.X][mob2.Y + 1] = 5;
                mob2.Y += 1;

            }
            else if (map[map_id][mob2.X + 1][mob2.Y + 1] == 0 || map[map_id][mob2.X + 1][mob2.Y + 1] == 8) {
                flag2 = 1;
            }
            if (flag2 == 1 && map[map_id][mob2.X + 1][mob2.Y - 1] != 0 && map[map_id][mob2.X + 1][mob2.Y - 1] != 8) {
                map[map_id][mob2.X][mob2.Y] = 0;
                mob2.Y -= 1;
                map[map_id][mob2.X][mob2.Y] = 5;

            }
            else if (map[map_id][mob2.X + 1][mob2.Y - 1] == 0 || map[map_id][mob2.X + 1][mob2.Y - 1] == 8) {
                flag2 = 0;
            }

            //mob3
            if (flag3 == 0 && map[map_id][mob3.X + 1][mob3.Y + 1] != 0 && map[map_id][mob3.X + 1][mob3.Y + 1] != 8) {
                map[map_id][mob3.X][mob3.Y] = 0;
                map[map_id][mob3.X][mob3.Y + 1] = 5;
                mob3.Y += 1;

            }
            else if (map[map_id][mob3.X + 1][mob3.Y + 1] == 0 || map[map_id][mob3.X + 1][mob3.Y + 1] == 8) {
                flag3 = 1;
            }
            if (flag3 == 1 && map[map_id][mob3.X + 1][mob3.Y - 1] != 0 && map[map_id][mob3.X + 1][mob3.Y - 1] != 8) {
                map[map_id][mob3.X][mob3.Y] = 0;
                mob3.Y -= 1;
                map[map_id][mob3.X][mob3.Y] = 5;

            }
            else if (map[map_id][mob3.X + 1][mob3.Y - 1] == 0 || map[map_id][mob3.X + 1][mob3.Y - 1] == 8) {
                flag3 = 0;
            }


            DrawStage();
        }
        else
            break;
    }


    DetectCollisionPCWithGoal();
    DetectCollisionWithMob(1);

}
void MovingBlock()
{
    //flag = 0  -> left 로 이동
    //flag = 1 -> right 로 이동
    if (block1_flag == 0) // left 로 이동 시도
    {
        if (map[map_id][block1_left.X][block1_left.Y - 1] != 8) // 이동 가능
        {
            for (int i = block1_left.Y; i <= block1_right.Y; i++)
            {
                map[map_id][block1_left.X][i - 1] = map[map_id][block1_left.X][i];
                element[element_id][block1_left.X - 1][i - 1] = element[element_id][block1_left.X - 1][i];
                map[map_id][block1_left.X - 1][i - 1] = map[map_id][block1_left.X - 1][i];
                if (map[map_id][block1_left.X - 1][i] == 6)
                {
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 0;
                    PC.X -= 2;
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 6;
                }
            }
            map[map_id][block1_left.X][block1_right.Y] = 0;
            block1_left.Y--;
            block1_right.Y--;
        }
        else // 이동 불가능
        {
            block1_flag = 1;
        }
    }
    else if (block1_flag == 1) // right 로 이동 시도
    {
        if (map[map_id][block1_right.X][block1_right.Y + 1] != 8) // 이동 가능 
        {
            for (int i = block1_right.Y; i >= block1_left.Y; i--)
            {
                map[map_id][block1_left.X][i + 1] = map[map_id][block1_left.X][i];
                element[element_id][block1_left.X - 1][i + 1] = element[element_id][block1_left.X - 1][i];
                if (map[map_id][block1_left.X - 1][i] == 6)
                {
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 0;
                    PC.X += 2;
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 6;
                }
            }
            map[map_id][block1_left.X][block1_left.Y] = 0;
            block1_left.Y++;
            block1_right.Y++;
        }
        else // 이동 불가능
        {
            block1_flag = 0;
        }
    }
    drawline(block1_left.X - 1);
    drawline(block1_left.X);


    if (block2_flag == 0) // left 로 이동 시도
    {
        if (map[map_id][block2_left.X][block2_left.Y - 1] != 8) // 이동 가능
        {
            for (int i = block2_left.Y; i <= block2_right.Y; i++)
            {
                map[map_id][block2_left.X][i - 1] = map[map_id][block2_left.X][i];
                element[element_id][block2_left.X - 1][i - 1] = element[element_id][block2_left.X - 1][i];
                if (map[map_id][block2_left.X - 1][i] == 6)
                {
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 0;
                    PC.X -= 2;
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 6;
                }
            }
            map[map_id][block2_left.X][block2_right.Y] = 0;
            block2_left.Y--;
            block2_right.Y--;
        }
        else // 이동 불가능
        {
            block2_flag = 1;
        }
    }
    else if (block2_flag == 1)// right 로 이동 시도
    {
        if (map[map_id][block2_right.X][block2_right.Y + 1] != 8) // 이동 가능 
        {
            for (int i = block2_right.Y; i >= block2_left.Y; i--)
            {
                map[map_id][block2_left.X][i + 1] = map[map_id][block2_left.X][i];
                element[element_id][block2_left.X - 1][i + 1] = element[element_id][block2_left.X - 1][i];
                if (map[map_id][block2_left.X - 1][i] == 6)
                {
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 0;
                    PC.X += 2;
                    map[map_id][PC.Y - GBOARD_ORIGIN_Y][(PC.X - GBOARD_ORIGIN_X) / 2 - 1] = 6;
                }
            }
            map[map_id][block2_left.X][block2_left.Y] = 0;
            block2_left.Y++;
            block2_right.Y++;
        }
        else // 이동 불가능
        {
            block2_flag = 0;
        }
    }

    drawline(block2_left.X);
    drawline(block2_left.X - 1);

}

void ProcessKeyInput()
{
    for (int i = 0; i < 20; i++)
    {
        if (CurrentGameStage != 1)
            MovingMob();
        if (CurrentGameStage == 3)
        {
            if (i % 2 == 0)
                MovingBlock();
            // 사용자
        }
        if (_kbhit() != 0)
        {
            int key = _getch();
            if (keyFlag == 0)
            {
                switch (key)
                {
                case LEFT:
                    ShiftLeft();
                    break;
                case RIGHT:
                    ShiftRight();
                    break;
                case DOWN:
                    PlayerDown();
                    break;

                }
            }
            else
            {
                switch (key)
                {
                case REVERSE_RIGHT:
                    ShiftRight();
                    break;
                case REVERSE_LEFT:
                    ShiftLeft();
                    break;
                case REVERSE_DOWN:
                    PlayerDown();
                    break;
                }
            }
            break;
        }
        //Sleep(speed);//만약 더 빠르게 하고싶으면 speed를 작게한다
    }
}

void Black()
{
    RemoveCursor();
    for (int i = 0; i < 29; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            SetCurrentCursorPos(j, i);
            printf(" ");
        }
    }
    SetCurrentCursorPos(0, 0);
}

void removeMenuKeyInput()
{
    for (int i = 0; i <= 100; i++)
    {
        SetCurrentCursorPos(i, 20);
        printf(" ");
    }
}

void StoryTelling()
{
    Black();
    SetCurrentCursorPos(0, 0);
    previousPageFlag = 1;
    nextPageFlag = 1;
    printf("옛날옛적에 윙가드리오마을 숲속에 동물친구들이 옹기종기 살고 있었어요~ \n하지만 어느날...\n악랄한 수라고동사이정이 나타나\n동물 친구들의 이름을 빼앗아 가버렸어요\n이제 우리는 친구들을 부를 수 가 없어요ㅠㅠ\n여러분이 제 친구들의 이름을 찾아주세요!!!!\n");
}

void rulePage()
{
    Black();
    previousPageFlag = 2;
    nextPageFlag = 2;
    printf("\n게임 방법\n\n1) 이동\n    → ← : 좌우 이동\n    ↓ : 1. 블록 파괴, 포털 블록 위에서 사용 시 짝포털로 이동\n");
    printf("         2. 포털 블록 ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
    printf("■");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("위에서 사용시 짝 포털로 이동\n");

    printf("\n2) 게임 목표\n    우측 게임 상황판에 있는 한글 단어와 일치 하는 영어 단어를 완성시킨 후 \n    맨 아래에 위치해 있는 골 지점 까지 가면 성공!");

    printf("\n\n3) 게임 룰\n");
    printf("    1.  일반 블록 : ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
    printf("■ ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
    printf("■ ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
    printf("■ ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf(",  포탈 블록 : ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
    printf("■ ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf(",  골인 지점 : ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    printf("■ ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("\n    2. PC 는 블록 위에서만 조작 가능(내려가는 도중 조작 불가)합니다. ");
    printf("\n    3. 색이 다른 블록 위에서만 하강이 중단됩니다.");
    printf("\n    4. 몹 충돌 및 목표 달성 실패시 life 1개가 감소됩니다.");


    printf("\n\n4) 방해물 및 아이템\n      § : mob (닿으면 life 1개가 감소)\n      ? : 아이템\n   아이템의 종류 :\n      1. PC의 LEFT, RIGHT, DOWN KEY 전환\n      2. Mob 속도 상승 및 감소\n      3. 블록 이동 속도 상승 및 감소\n      4. PC의 생명 1 추가\n");
    printf("      5. Map과 PC의 위치 초기화\n");
}

void gamePage()
{
    for (int i = 0; i < 60; i++)
    {
        for (int j = 0; j < 140; j++)
        {
            SetCurrentCursorPos(j, i);
            printf(" ");
        }
    }
    SetCurrentCursorPos(0, 0);
    removeMenuKeyInput();
    DrawGameBoard();
    DrawStage();

    while (1)
    {
        ProcessKeyInput();
    }
}


void menu();
void previousPage()
{
    Black();
    if (previousPageFlag == 1) menu();
    else if (previousPageFlag == 2) StoryTelling();
}
void nextPage()
{
    Black();
    if (nextPageFlag == 1) rulePage();
    else if (nextPageFlag == 2) gamePage();
}

void menuKeyInput()
{
    for (int i = 0; i < 20; i++)
    {
        if (_kbhit() != 0)
        {
            int key = _getch();
            switch (key)
            {
            case LEFT:
                previousPage();
                break;
            case RIGHT:
                nextPage();
                break;
            }
        }
        Sleep(speed);//만약 더 빠르게 하고싶으면 speed를 작게한다
    }
}
void menu()
{
    int menuButton;

    RemoveCursor();

    SetCurrentCursorPos(33, 2);
    printf("**전체화면으로 플레이해주세요**");
    SetCurrentCursorPos(40, 5);
    curPos = GetCurrentCursorPos();
    printf("1. 게임시작");
    curPos.Y += 2;
    SetCurrentCursorPos(curPos.X, curPos.Y);
    printf("2. 게임설명");
    curPos.Y += 2;
    SetCurrentCursorPos(curPos.X, curPos.Y);
    printf("3. 게임종료");
    SetCurrentCursorPos(curPos.X - 3, curPos.Y + 4);
    printf("메뉴 선택 : ");
    scanf("%d", &menuButton);
    switch (menuButton)
    {
    case 1:
        Black();
        gamePage();
        break;
    case 2:
        Black();
        SetCurrentCursorPos(0, 40);
        printf("← 이전페이지");
        SetCurrentCursorPos(80, 40);
        printf("→ 다음페이지");
        StoryTelling();
        while (1)
        {
            menuKeyInput();
        }
        break;
    case 3:
        break;
    }
}


int main()
{
    PlaySound(MAKEINTRESOURCE(OST), NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
    menu();
}