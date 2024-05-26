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
void printTrain(int trainLength, int citizenPos, int zombiePos, int dongseokPos, int stamina, int citizenAggro, int dongseokAggro);
void gameLoop(int trainLength, int Probability, int stamina);
void moveCitizen(int* citizenPos, int Probability, int* citizenAggro);
void moveZombie(int* zombiePos, int trainLength, int Probability, int* zombieMoveCounter, int citizenPos, int dongseokPos, int citizenAggro, int dongseokAggro, int* pullSuccess, int* zombieAttackTarget, int* stamina);
void moveDongseok(int* dongseokPos, int* stamina, int* dongseokAggro, int zombiePos);
void performDongseokAction(int* dongseokPos, int* stamina, int zombiePos, int* citizenAggro, int* dongseokAggro, int* pullSucessm, int* zombieAttackTarget, int Probability);
int getDongseokAction(int zombiePos, int dongseokPos);
void rest(int* dongseokPos, int* stamina, int* aggro);
void provoke(int* dongseokAggro);
void pull(int* zombiePos, int* dongseokPos, int* pullSucess, int* dongseokAggro, int* stamina, int Probability);
void checkGameOver(int citizenPos, int zombiePos, int zombieAttackTarget, int stamina);

int main() {
    int trainLength, Probability, stamina;

    srand(time(NULL));

    printf("======================\n");
    printf("======GAME START======\n");
    printf("======================\n");

    getInput(&trainLength, &Probability, &stamina);
    gameLoop(trainLength, Probability, stamina);

    return 0;
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
        printf("percentile probability 'p' (%d~%d) >> ", MIN_PROBABILITY, MAX_PROBABILITY);
        scanf_s("%d", Probability);
    } while (*Probability < MIN_PROBABILITY || *Probability > MAX_PROBABILITY);
}
//열차 초기 상태 출력
void printTrain(int trainLength, int citizenPos, int zombiePos, int dongseokPos, int stamina, int citizenAggro, int dongseokAggro){
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
void gameLoop(int trainLength, int Probability, int stamina) {
    int citizenPos = trainLength - 6;
    int zombiePos = trainLength - 3;
    int dongseokPos = trainLength - 2;
    int zombieMoveCounter = 1;
    int citizenAggro = MIN_AGGRO + 1;
    int dongseokAggro = MIN_AGGRO + 1;
    int pullSuccess = 0;
    int zombieAttackTarget = ATK_NONE;

    printTrain(trainLength, citizenPos, zombiePos, dongseokPos, stamina, citizenAggro, dongseokAggro);

    while (1) {
        moveCitizen(&citizenPos, Probability, &citizenAggro);
        moveZombie(&zombiePos, trainLength, Probability, &zombieMoveCounter, citizenPos, dongseokPos, citizenAggro, dongseokAggro, &pullSuccess, &zombieAttackTarget, &stamina);
        printTrain(trainLength, citizenPos, zombiePos, dongseokPos, stamina, citizenAggro, dongseokAggro);
        
        moveDongseok(&dongseokPos, &stamina, &dongseokAggro, zombiePos);
        printTrain(trainLength, citizenPos, zombiePos, dongseokPos, stamina, citizenAggro, dongseokAggro);
        checkGameOver(citizenPos, zombiePos, zombieAttackTarget, stamina);

        performDongseokAction(&dongseokPos, &stamina, zombiePos, &citizenAggro, &dongseokAggro, &pullSuccess, &zombieAttackTarget, Probability);
        printTrain(trainLength, citizenPos, zombiePos, dongseokPos, stamina, citizenAggro, dongseokAggro);
        checkGameOver(citizenPos, zombiePos, zombieAttackTarget, stamina);
    }
}

void moveCitizen(int* citizenPos, int Probability, int* citizenAggro) {
    int citizenMove = (rand() % 100 < Probability) ? 0 : -1;
    int oldPos = *citizenPos;
    *citizenPos += citizenMove;

    if (citizenMove == 0) {
        printf("citizen : stay %d(aggro: %d -> %d)\n", *citizenPos, *citizenAggro,(*citizenAggro > MIN_AGGRO) ? *citizenAggro -1 : MIN_AGGRO);
        *citizenAggro = (*citizenAggro > MIN_AGGRO) ? *citizenAggro - 1 : MIN_AGGRO;
    }
    else {
        printf("citizen : %d->%d(aggro: %d -> %d)\n", oldPos, *citizenPos, *citizenAggro, *citizenAggro +1);
        *citizenAggro = (*citizenAggro < MAX_AGGRO) ? *citizenAggro + 1 : *citizenAggro;
    }
}

void moveZombie(int* zombiePos, int trainLength, int Probability, int* zombieMoveCounter, int citizenPos, int dongseokPos, int citizenAggro, int dongseokAggro, int* pullSuccess, int* zombieAttackTarget, int* stamina) {
    int oldPos = *zombiePos;
    *zombieAttackTarget = ATK_NONE;
    
    if (*pullSuccess) {
        printf("zombie cannot move due to pull\n");
        *pullSuccess = 0;
    }
    else if (*zombieMoveCounter % 2 == 0) {
        printf("zombie : cannot move\n");
    }
    else {
        int zombieMove =0;
        int target = (citizenAggro > dongseokAggro) ? citizenPos : dongseokPos;
        if (*zombiePos < target && *zombiePos + 1 != dongseokPos) {
            zombieMove = 1;
        }
        else if (*zombiePos > target && *zombiePos -1 != dongseokPos) {
            zombieMove = -1;
        }

        if (*zombiePos == citizenPos + 1 || *zombiePos == citizenPos - 1) {
            *zombieAttackTarget = ATK_CITIZEN;
        }
        else if (*zombiePos == dongseokPos + 1 || *zombiePos == dongseokPos - 1) {
            *zombieAttackTarget = ATK_DONGSEOK;
        }
        printf("zombie: %d -> %d\n", oldPos, *zombiePos);

        if (*zombieAttackTarget == ATK_DONGSEOK) {
            *stamina = (*stamina > MIN_STM) ? *stamina - 1 : MIN_STM;
            printf("zombie attacked dongsoek. (stamina : %d)\n", *stamina);
        }
        *zombiePos += -1;
    }
    (*zombieMoveCounter)++;
}
void moveDongseok(int* dongseokPos, int* stamina, int* dongseokAggro, int zombiePos) {
    if (*stamina == MIN_STM) {
        printf("dongseok: cannot move due to lack of stamina\n");
        return;
    }

    int action;
    if (zombiePos == *dongseokPos + 1 || zombiePos == *dongseokPos - 1) {
        action = 0;
        printf("choose dongseok action.(stay:%d) ", MOVE_STAY);
        scanf_s("%d", &action);
        while (action != MOVE_STAY) {
            printf("choose dongseok action.(stay:%d) ", MOVE_STAY);
            scanf_s("%d", &action);
        }
    }
    else {
        printf("choose dongseok action.(left:%d, stay%d): ", MOVE_LEFT, MOVE_STAY);
        scanf_s("%d", &action);
        while (action != MOVE_LEFT && action != MOVE_STAY) {
            printf("choose dongseok action.(left:%d, stay%d): ", MOVE_LEFT, MOVE_STAY);
            scanf_s("%d", &action);
        }
    }
    switch (action) {
    case MOVE_LEFT:
        printf("dongseok: move left(%d -> %d)\n", *dongseokPos, *dongseokPos - 1);
        (*dongseokPos)--;
        (*dongseokPos)++;
        break;
    case MOVE_STAY:
        printf("dongseok: stay\n");
        break;
    }
}

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
        if(action != MOVE_LEFT && action != MOVE_STAY){
        }
    } while (action != MOVE_LEFT && action != MOVE_STAY);
    return action;
}

void performDongseokAction(int* dongseokPos, int* stamina, int zombiePos, int* citizenAggro, int* dongseokAggro, int* pullSuccess, int* zombieAttackTarget, int Probability) {
    int action;
    do {
        printf("choose dongseok move(rest: %d, provoke: %d, pull: %d): ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
        scanf_s("d", &action);
        switch (action) {
        case ACTION_REST:
            rest(dongseokPos, stamina, dongseokAggro);
            break;
        case ACTION_PROVOKE:
            provoke(dongseokAggro);
            break;
        case ACTION_PULL:
            pull(&zombiePos, dongseokPos, pullSuccess, dongseokAggro, stamina, Probability);
            break;
        default:
            break;
        }
    } while (action != ACTION_REST && action != ACTION_PROVOKE && action != ACTION_PULL);
}

void rest(int* dongseokPos, int* stamina, int* aggro) {
    printf("dongseok rest...\n");
    printf("dongseok: %d(stamina: %d, aggro: %d -> %d)\n", *stamina, (*stamina < MAX_STM) ? *stamina + 1 : *stamina, *aggro, (*aggro > MIN_AGGRO) ? *aggro - 1 : *aggro);
    *stamina = (*stamina < MAX_STM) ? *stamina + 1 : *stamina;
    *aggro = (*aggro > MIN_AGGRO) ? *aggro - 1 : *aggro;
}

void provoke(int* dongseokAggro) {
    printf("dongseok: provoke(aggro: %d-> %d)\n", *dongseokAggro, MAX_AGGRO);
    *dongseokAggro = MAX_AGGRO;
}

void pull(int* zombiePos, int* dongseokPos, int* pullSuccess, int* dongseokAggro, int* stamina, int Probability) {
    printf("dongseok: pull\n");
    *dongseokAggro += 2;
    *stamina -= 1;

    int sucess = rand() % 100 < (100 - Probability);
    if (sucess) {
        printf("pull success!, zombie cannot move next turn.\n");
        *zombiePos = *dongseokPos;
        *pullSuccess = 1;
    }
    else {
        printf("pull fail...\n");
        *pullSuccess = 0;
    }
}
void checkGameOver(int citizenPos, int zombiePos, int zombieAttackTarget, int stamina) {
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
