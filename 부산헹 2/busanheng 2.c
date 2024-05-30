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
int trainLength, Probability, stamina;
int citizenPos, zombiePos, dongseokPos, zombieMoveCounter;
int citizenAggro, dongseokAggro, pullSuccess, zombieAttackTarget;

//함수 선언
void getInput();
void printTrain();
void gameLoop();
void moveCitizen();
void moveZombie();
void moveDongseok();
void performDongseokAction();
int getDongseokAction(int zombiePos, int dongsoekPos);
void rest();
void provoke();
void pull();
void checkGameOver();

int main() {
    srand(time(NULL));

    printf("======================\n");
    printf("======GAME START======\n");
    printf("======================\n");

    getInput();
    gameLoop();

    return 0;
}
// 2-2 부산헹(1)에서 수정
void getInput() {
    do {
        printf("train length(%d~%d) >> ", MIN_TRAIN_LENGTH, MAX_TRAIN_LENGTH);
        scanf_s("%d", &trainLength);
    } while (trainLength < MIN_TRAIN_LENGTH || trainLength > MAX_TRAIN_LENGTH);

    do {
        printf("madongseok stamina(%d~%d >> ", MIN_STM, MAX_STM);
        scanf_s("%d", &stamina);
    } while (stamina < MIN_STM || stamina > MAX_STM);

    do {
        printf("percentile probability 'p' (%d~%d) >> ", MIN_PROBABILITY, MAX_PROBABILITY);
        scanf_s("%d", &Probability);
    } while (Probability < MIN_PROBABILITY || Probability > MAX_PROBABILITY);
}
//열차 초기 상태 출력
void printTrain(){
    for (int i = 0; i < trainLength; i++) printf("#");
    printf("\n#");

    for (int i = 1; i < trainLength - 1; i++) {
        if (i == citizenPos) printf("C");
        else if (i == zombiePos) printf("Z");
        else if (i == dongseokPos) printf("M");
        else printf(" ");
    }
    printf("#\n");

    for (int i = 0; i < trainLength; i++) printf("#");
    printf("\n");

    printf("citizen Aggro: %d, dongseokAggro: %d, stamina: %d\n", citizenAggro, dongseokAggro, stamina);
}
void gameLoop() {
    citizenPos = trainLength - 6;
    zombiePos = trainLength - 3;
    dongseokPos = trainLength - 2;
    zombieMoveCounter = 1;
    citizenAggro = MIN_AGGRO + 1;
    dongseokAggro = MIN_AGGRO + 1;
    pullSuccess = 0;
    zombieAttackTarget = ATK_NONE;

    printTrain();

    while (1) {
        //이동 단계
        moveCitizen();
        moveZombie();
        printTrain();
        
        moveDongseok();
        printTrain();
        checkGameOver();

        //행동 단계
        performDongseokAction();
        printTrain();
        checkGameOver();
    }
}
// 2-3<이동>
void moveCitizen() {
    int citizenMove = (rand() % 100 < Probability) ? 0 : -1;
    int oldPos = citizenPos;
    citizenPos += citizenMove;

    if (citizenMove == 0) {
        printf("citizen : stay %d(aggro: %d -> %d)\n", citizenPos, citizenAggro,(citizenAggro > MIN_AGGRO) ? citizenAggro -1 : MIN_AGGRO);
        citizenAggro = (citizenAggro > MIN_AGGRO) ? citizenAggro - 1 : MIN_AGGRO;
    }
    else {
        printf("citizen : %d->%d(aggro: %d -> %d)\n", oldPos, citizenPos, citizenAggro, (citizenAggro < MAX_AGGRO) ? citizenAggro + 1 : MAX_AGGRO);
        citizenAggro = (citizenAggro < MAX_AGGRO) ? citizenAggro + 1 : citizenAggro;
    }
}
//2-3<이동>
void moveZombie() {
    int oldPos = zombiePos;
    zombieAttackTarget = ATK_NONE;

    if (pullSuccess) {
        printf("zombie cannot move due to pull\n");
        pullSuccess = 0;
    }
    else if (zombieMoveCounter % 2 == 0) {
        printf("zombie : cannot move\n");
    }
    else {
        int zombieMove = 0;
        int target;
        if (citizenAggro == dongseokAggro) {
            target = citizenPos;
        }
        else {
            target = (citizenAggro > dongseokAggro) ? citizenPos : dongseokPos;
        }

        if (zombiePos < target && zombiePos + 1 != dongseokPos) {
            zombieMove = +1;
        }
        else if (zombiePos > target && zombiePos - 1 != dongseokPos) {
            zombieMove = -1;
        }
        zombiePos += zombieMove;
    }
    if (zombiePos == citizenPos + 1 || zombiePos == citizenPos - 1) {
        zombieAttackTarget = ATK_CITIZEN;
    }
    else if (zombiePos == citizenPos + 1 || zombiePos == dongseokPos - 1) {
        zombieAttackTarget = ATK_DONGSEOK;
    }
    printf("zombie: %d->%d\n", oldPos, zombiePos);
    if (zombieAttackTarget == ATK_DONGSEOK) {
        stamina = (stamina > MIN_STM) ? stamina - 1 : MIN_STM;
        printf("zombie attacked dongseok. stamina: %d\n", stamina);
        checkGameOver();
    }
    zombieMoveCounter++;
}
//2-3<이동>
void moveDongseok() {
    checkGameOver();

    int action;
    if (zombiePos == dongseokPos - 1 || zombiePos == dongseokPos + 1) {
        printf("choose dongseok action.(stay:%d) ", MOVE_STAY);
        scanf_s("%d", &action);
        while (action != MOVE_STAY) {
            printf("choose dongseok action.(stay:%d) ", MOVE_STAY);
            scanf_s("%d", &action);
        }
        printf("dongseok: stay\n");
        dongseokAggro = (dongseokAggro > MIN_AGGRO) ? dongseokAggro - 1 : MIN_AGGRO;
        return;
    }
    do {
        printf("choose dongseok action.(left:%d, stay%d): ", MOVE_LEFT, MOVE_STAY);
        scanf_s("%d", &action);
    } while (action != MOVE_LEFT && action != MOVE_STAY);
        
    switch (action) {
    case MOVE_LEFT:
        printf("dongseok: move left(%d -> %d)\n", dongseokPos, dongseokPos - 1);
        dongseokPos--;
        dongseokAggro++;
        if (dongseokAggro > MAX_AGGRO) {
            dongseokAggro = MAX_AGGRO;
        }
        break;
    case MOVE_STAY:
        printf("dongseok: stay\n");
        dongseokAggro = (dongseokAggro > MIN_AGGRO) ? dongseokAggro - 1 : MIN_AGGRO;
        break;
    }
}
//2-3<이동>
int getDongseokAction(int zombiePos, int dongseokPos) {
    int action;
    if (zombiePos == dongseokPos + 1 || zombiePos == dongseokPos - 1) {
        do {
            printf("choose dongseok move(stay: %d): ", MOVE_STAY);
            scanf_s("%d", &action);
        } while (action != MOVE_STAY);
        return action;
    }
    do {
        printf("choose dongseok move(left: %d, stay: %d): ", MOVE_LEFT, MOVE_STAY);
        scanf_s("%d", &action);
    } while (action != MOVE_LEFT && action != MOVE_STAY);
    return action;
}
//2-4<행동>
void performDongseokAction() {
    if (zombieAttackTarget == ATK_CITIZEN) {
        printf("zombie attacked citizen\n");
    }
    else if (zombieAttackTarget == ATK_DONGSEOK) {
        printf("zombie attacked dongseok.(aggro: %d vs %d, stamina: %d -> %d)\n", citizenAggro, dongseokAggro, stamina + 1, stamina);
    }
    else {
        printf("citizen does nothing\n");
    }
    int action;
    if (stamina > 0) {
        if (dongseokPos == zombiePos - 1 || dongseokPos == zombiePos + 1) {
            do {
                printf("choose dongseok action(rest: %d, provoke: %d, pull: %d): ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
                scanf_s("%d", &action);
            } while (action != ACTION_REST && action != ACTION_PROVOKE && action != ACTION_PULL);
        }
        else {
            do {
                printf("choose dongseok action(rest: %d, provoke: %d): ", ACTION_REST, ACTION_PROVOKE);
                scanf_s("%d", &action);
            } while (action != ACTION_REST && action != ACTION_PROVOKE);
        }
    }
    else {
        do {
            printf("choose dongseok action(rest: %d, provoke: %d): ", ACTION_REST, ACTION_PROVOKE);
            scanf_s("%d", &action);
        } while (action != ACTION_REST && action != ACTION_PROVOKE);
    }
    switch(action){
    case ACTION_REST:
        rest();
        break;
    case ACTION_PROVOKE:
        provoke();
        break;
    case ACTION_PULL:
        if (stamina > 0 && dongseokAggro < MAX_AGGRO) {
            pull();
        }
        break;
    }
    if (pullSuccess) {
        printf("PULL SUCCESS! ZOMBIE CANNOT MOVE NEXT TURN.\n");
    }
    else {
        printf("PULL FAILED\n");
    }
}
//2-4<행동>
void rest() {
    printf("dongseok rest...\n");
    printf("dongseok: %d(stamina: %d->%d, aggro: %d -> %d)\n", dongseokPos, stamina, (stamina < MAX_STM) ? stamina + 1 : stamina, dongseokAggro, (dongseokAggro > MIN_AGGRO) ? dongseokAggro - 1 : dongseokAggro);
    stamina = (stamina < MAX_STM) ? stamina + 1 : stamina;
    dongseokAggro = (dongseokAggro > MIN_AGGRO) ? dongseokAggro - 1 : dongseokAggro;
}
//2-4<행동>
void provoke() {
    printf("dongseok provoked zombie\n");
    printf("dongseok: %d((aggro: %d-> %d, stamina: %d)\n", dongseokPos, dongseokAggro, MAX_AGGRO, stamina);
    dongseokAggro = MAX_AGGRO;
}
//2-4<행동>
void pull() {
    printf("dongseok tried to pull zombie\n");
    dongseokAggro += 2;
    stamina -= 1;

    int success = rand() % 100 < (100 - Probability);
    pullSuccess = success;

    if (success) {
        printf("PULL SUCCESS! zombie cannot move next turn.\n");
        pullSuccess = 1;
    }
    else {
        printf("PULL FAILED...\n");
        pullSuccess = 0;
        dongseokAggro += 2;
        stamina -= 1;
    }
    if (dongseokAggro > MAX_AGGRO) {
        dongseokAggro = MAX_AGGRO;
    }
    printf("dongseok: %d (aggro: %d -> %d, stamina: %d-> %d)\n", dongseokPos, dongseokAggro - 2, dongseokAggro, stamina + 1, stamina);
}
void checkGameOver() {
    if(citizenPos <= 1) {
        printf("SUCESS!\n");
        printf("citizen(s) escaped to the next train\n");
        exit(0);
    }
    if (zombieAttackTarget == ATK_CITIZEN) {
        printf("GAME OVER\n");
        printf("Citizen(s) has(have) been attacked by zombie\n");
        exit(0);
    }
    if (zombieAttackTarget == ATK_DONGSEOK && stamina == MIN_STM) {
        printf("GAMEOVER\n");
        printf("Dongseok's stamina is 0.\n");
        exit(0);
    }
}
