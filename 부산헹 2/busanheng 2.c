//1-1 준비
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

//파라미터
#define MIN_TRAIN_LENGTH 15 //기차 길이
#define MAX_TRAIN_LENGTH 50
#define MIN_STM 0 //마동석 체력
#define MAX_STM 5
#define MIN_PROBABILITY 10 //확률
#define MAX_PROBABILITY 90
#define MIN_AGGRO 0 //어그로 범위
#define MAX_AGGRO 5

//마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

//좀비의 공격대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

//마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

//전역 변수
int citizens[10];
int zombies[10];
int numCitizens = 1;
int numZombies = 1;

//함수 선언
void getInput(int* trainLength, int* Probability, int* stamina);
void printTrain(int trainLength, int citizenPos, int zombiePos);
void intro();
void outro();
void gameLoop(int trainLength, int Probability, int stamina);
void moveCitizen(int* citizenPos, int Probability);
void moveZombie(int* zombiePos, int trainLength, int Probability, int* zombieMoveCounter);
void checkGameOver(int citizenPos, int zombiePos);

void main() {
    int trainLength, Probability, stamina;

    srand(time(NULL));

    intro();

    getInput(&trainLength, &Probability, &stamina);
    gameLoop(trainLength, Probability, stamina);

    outro();
}
void intro() {
    //인트로
    printf("**************************\n");
    printf("잠시 후 게임을 시작합니다.\n");
    printf("**************************\n");
    Sleep(1500); // 1.5초 동안 프로그램 멈추기
    system("cls"); // 출력된 내용 지우기
}

void outro() {
    //아웃트로
    printf("**********************\n");
    printf("게임이 종료되었습니다.\n");
    printf("**********************\n");
    Sleep(4000);
}
void getInput(int* trainLength, int* Probability, int* stamina) {
    do {
        printf("train length(%d~%d) >> ", MIN_TRAIN_LENGTH, MAX_TRAIN_LENGTH);
        scanf_s("%d", trainLength);
    } while (*trainLength < MIN_TRAIN_LENGTH || *trainLength > MAX_TRAIN_LENGTH);

    do {
        printf("stamina(%d~%d >> ", MIN_STM, MAX_STM);
        scanf_s("%d", stamina);
    } while (*stamina < MIN_STM || *stamina > MAX_STM);

    do {
        printf("probability (%d~%d) >> ", MIN_PROBABILITY, MAX_PROBABILITY);
        scanf_s("%d", Probability);
    } while (*Probability < MIN_PROBABILITY || *Probability > MAX_PROBABILITY);
    system("cls");
}
//열차 초기 상태 출력
void printTrain(int trainLength, int citizenPos, int zombiePos) {
    for (int i = 0; i < trainLength; i++) printf("#");
    printf("\n#");
    for (int i = 1; i < trainLength - 1; i++) {
        if (i == citizenPos) printf("C");
        else if (i == zombiePos) printf("Z");
        else if (i == trainLength - 2) printf("M");
        else printf(" ");
    }
    printf("#\n");

    for (int i = 0; i < trainLength; i++) printf("#");
    printf("\n");
}
void gameLoop(int trainLength, int Probability, int stamina) {
    int citizenPos = trainLength - 6;
    int zombiePos = trainLength - 3;
    int zombieMoveCounter = 1;

    printTrain(trainLength, citizenPos, zombiePos);

    while (1) {
        moveCitizen(&citizenPos, Probability);
        moveZombie(&zombiePos, trainLength, Probability, &zombieMoveCounter);
        printTrain(trainLength, citizenPos, zombiePos);
        checkGameOver(citizenPos, zombiePos);
    }
}

void moveCitizen(int* citizenPos, int Probability) {
    int citizenMove = (rand() % 100 < Probability) ? 0 : -1;
    *citizenPos += citizenMove;

    if (citizenMove == 0) {
        printf("citizen : stay %d\n", *citizenPos);
    }
    else {
        printf("citizen : %d->%d\n", *citizenPos - citizenMove, *citizenPos);
    }
}
void moveZombie(int* zombiePos, int trainLength, int Probability, int* zombieMoveCounter) {
    if (*zombieMoveCounter % 2 == 0) {
        printf("zombie : cannot move\n");
    }
    else {
        int zombieMove = (rand() % 100 < Probability) ? -1 : 0;
        *zombiePos += zombieMove;
        if (zombieMove == 0) {
            printf("zombie : stay %d\n", *zombiePos);
        }
        else {
            printf("zombie : %d->%d\n", *zombiePos - zombieMove, *zombiePos);
        }
    }
    (*zombieMoveCounter)++;
}
void checkGameOver(int citizenPos, int zombiePos) {
    if(citizenPos <= 1) {
        printf("SUCESS!\n");
        printf("citizen(s) escaped to the next train\n");
        exit(0);
    }
    if (citizenPos == zombiePos - 1) {
        printf("GAME OVER\n");
        printf("Citizen(s) has(have) been attacked by zombie\n");
        exit(0);
    }
}
