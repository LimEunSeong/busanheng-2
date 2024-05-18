//1-1 준비
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

#define MIN_TRAIN_LENGTH 15
#define MAX_TRAIN_LENGTH 50
#define MIN_PROBABILITY 10
#define MAX_PROBABILITY 90

void getInput(int* trainLength, int* Probability);
void printTrain(int trainLength, int citizenPos, int zombiePos);

void main() {
    int trainLength, Probability;
    int previousCitizenPos, previousZombiePos;

    srand(time(NULL));

    //인트로
    printf("**************************\n");
    printf("잠시 후 게임을 시작합니다.\n");
    printf("**************************\n");

    Sleep(1500); // 1.5초 동안 프로그램 멈추기

    system("cls"); // 출력된 내용 지우기

    //열차 길이와 확률 입력받기
    getInput(&trainLength, &Probability);

    //열차 초기 상태 출력
    printTrain(trainLength, trainLength - 6, trainLength - 3);
    previousCitizenPos = trainLength - 6;
    previousZombiePos = trainLength - 3;

    int zombieMoveCounter = 1; // 좀비 움직임 주기를 계산하기 위한 변수

    while (1) {
        //1-4 시민 이동
        int citizenMove;
        if (rand() % 100 < Probability) { //p%만큼 제자리에 머무름
            citizenMove = 0;
        }
        else {// 100-p% 의 확률로 왼쪽으로 한 칸 이동
            citizenMove = -1;//
        }
        int citizenPos = previousCitizenPos + citizenMove;

        //1-5 좀비 이동
        int zombieMove;
        if (zombieMoveCounter % 2 == 0) {// 좀비가 두 번마다 움직이도록
            zombieMove = 0; //움직이지 않음
        }
        else {
            if (rand() % 100 < Probability) {// p%의 확률로 왼쪽으로 1칸 이동 
                zombieMove = -1;
            }
            else {// 100-p의 확률로 제자리에 머무름
                zombieMove = 0;
            }
        }
        int zombiePos = previousZombiePos + zombieMove;

        //1-2 열차 상태 출력

        system("cls");
        printTrain(trainLength, citizenPos, zombiePos);

        //1-3 시민, 좀비 상태 출력
        if (citizenPos == previousCitizenPos) {
            printf("citizen stay : %d\n", citizenPos);
        }
        else {
            printf("citizen : %d->%d\n", previousCitizenPos, citizenPos);
        }

        if (zombiePos == previousZombiePos) {
            if (zombieMoveCounter % 2 == 0) {
                printf("zombie : %d (cannot move)\n", zombiePos);
            }
            else {
                printf("zombie stay : %d\n", zombiePos);
            }
        }
        else {
            printf("zombie : %d->%d\n", previousZombiePos, zombiePos);
        }

        //시민이 왼쪽 끝에 도착할 때
        if (citizenPos <= 1) {
            printf("SUCCESS!\n");
            printf("citizen(s) escaped to the next train\n");
            break;
        }

        //좀비가 시민 옆 칸에 도착할 때
        if (citizenPos == zombiePos - 1) {
            printf("GAME OVER!\n");
            printf("Citizen(s) has(have) been attacked by zombie\n");
            break; // 루프 종료
        }

        previousCitizenPos = citizenPos;
        previousZombiePos = zombiePos;

        zombieMoveCounter++; // 좀비 움직임 주기

        Sleep(4000); //4초 마다 시민, 좀비 이동
    }
    Sleep(4000);
    system("cls");

    //아웃트로
    printf("**********************\n");
    printf("게임이 종료되었습니다.\n");
    printf("**********************\n");

    Sleep(4000); //4초 동안 프로그램 정지

    system("cls"); // 출력된 내용 지우기
}

// 열차 길이와 확률을 입력받아서 출력하는 함수
void getInput(int* trainLength, int* Probability) {
    printf("열차의 길이를 입력하세요 (%d에서 %d까지): ", MIN_TRAIN_LENGTH, MAX_TRAIN_LENGTH);
    scanf_s("%d", trainLength);

    if (*trainLength < MIN_TRAIN_LENGTH || *trainLength > MAX_TRAIN_LENGTH) {
        printf("열차의 길이는 %d에서 %d까지만 가능합니다.\n", MIN_TRAIN_LENGTH, MAX_TRAIN_LENGTH);
        exit(1);
    }

    printf("확률을 입력하세요 (%d에서 %d까지): ", MIN_PROBABILITY, MAX_PROBABILITY);
    scanf_s("%d", Probability);

    if (*Probability < MIN_PROBABILITY || *Probability > MAX_PROBABILITY) {
        printf("확률은 %d에서 %d까지만 가능합니다.\n", MIN_PROBABILITY, MAX_PROBABILITY);
        exit(1);
    }
    system("cls");
}
//열차 초기 상태 출력
void printTrain(int trainLength, int citizenPos, int zombiePos) {
    int i;
    //열차 상단 출력
    for (i = 0; i < trainLength; i++) {
        printf("#");
    }
    printf("\n");

    // 열차 중단 출력

    printf("#");
    for (i = 1; i < trainLength - 1; i++) {
        if (i == citizenPos) {
            printf("C");
        }
        else if (i == zombiePos) {
            printf("Z");
        }
        else if (i == trainLength - 2) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("#\n");

    //열차 하단 출력

    for (i = 0; i < trainLength; i++) {
        printf("#");
    }
    printf("\n");
}
