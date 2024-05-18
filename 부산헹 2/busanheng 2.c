//1-1 �غ�
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

    //��Ʈ��
    printf("**************************\n");
    printf("��� �� ������ �����մϴ�.\n");
    printf("**************************\n");

    Sleep(1500); // 1.5�� ���� ���α׷� ���߱�

    system("cls"); // ��µ� ���� �����

    //���� ���̿� Ȯ�� �Է¹ޱ�
    getInput(&trainLength, &Probability);

    //���� �ʱ� ���� ���
    printTrain(trainLength, trainLength - 6, trainLength - 3);
    previousCitizenPos = trainLength - 6;
    previousZombiePos = trainLength - 3;

    int zombieMoveCounter = 1; // ���� ������ �ֱ⸦ ����ϱ� ���� ����

    while (1) {
        //1-4 �ù� �̵�
        int citizenMove;
        if (rand() % 100 < Probability) { //p%��ŭ ���ڸ��� �ӹ���
            citizenMove = 0;
        }
        else {// 100-p% �� Ȯ���� �������� �� ĭ �̵�
            citizenMove = -1;//
        }
        int citizenPos = previousCitizenPos + citizenMove;

        //1-5 ���� �̵�
        int zombieMove;
        if (zombieMoveCounter % 2 == 0) {// ���� �� ������ �����̵���
            zombieMove = 0; //�������� ����
        }
        else {
            if (rand() % 100 < Probability) {// p%�� Ȯ���� �������� 1ĭ �̵� 
                zombieMove = -1;
            }
            else {// 100-p�� Ȯ���� ���ڸ��� �ӹ���
                zombieMove = 0;
            }
        }
        int zombiePos = previousZombiePos + zombieMove;

        //1-2 ���� ���� ���

        system("cls");
        printTrain(trainLength, citizenPos, zombiePos);

        //1-3 �ù�, ���� ���� ���
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

        //�ù��� ���� ���� ������ ��
        if (citizenPos <= 1) {
            printf("SUCCESS!\n");
            printf("citizen(s) escaped to the next train\n");
            break;
        }

        //���� �ù� �� ĭ�� ������ ��
        if (citizenPos == zombiePos - 1) {
            printf("GAME OVER!\n");
            printf("Citizen(s) has(have) been attacked by zombie\n");
            break; // ���� ����
        }

        previousCitizenPos = citizenPos;
        previousZombiePos = zombiePos;

        zombieMoveCounter++; // ���� ������ �ֱ�

        Sleep(4000); //4�� ���� �ù�, ���� �̵�
    }
    Sleep(4000);
    system("cls");

    //�ƿ�Ʈ��
    printf("**********************\n");
    printf("������ ����Ǿ����ϴ�.\n");
    printf("**********************\n");

    Sleep(4000); //4�� ���� ���α׷� ����

    system("cls"); // ��µ� ���� �����
}

// ���� ���̿� Ȯ���� �Է¹޾Ƽ� ����ϴ� �Լ�
void getInput(int* trainLength, int* Probability) {
    printf("������ ���̸� �Է��ϼ��� (%d���� %d����): ", MIN_TRAIN_LENGTH, MAX_TRAIN_LENGTH);
    scanf_s("%d", trainLength);

    if (*trainLength < MIN_TRAIN_LENGTH || *trainLength > MAX_TRAIN_LENGTH) {
        printf("������ ���̴� %d���� %d������ �����մϴ�.\n", MIN_TRAIN_LENGTH, MAX_TRAIN_LENGTH);
        exit(1);
    }

    printf("Ȯ���� �Է��ϼ��� (%d���� %d����): ", MIN_PROBABILITY, MAX_PROBABILITY);
    scanf_s("%d", Probability);

    if (*Probability < MIN_PROBABILITY || *Probability > MAX_PROBABILITY) {
        printf("Ȯ���� %d���� %d������ �����մϴ�.\n", MIN_PROBABILITY, MAX_PROBABILITY);
        exit(1);
    }
    system("cls");
}
//���� �ʱ� ���� ���
void printTrain(int trainLength, int citizenPos, int zombiePos) {
    int i;
    //���� ��� ���
    for (i = 0; i < trainLength; i++) {
        printf("#");
    }
    printf("\n");

    // ���� �ߴ� ���

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

    //���� �ϴ� ���

    for (i = 0; i < trainLength; i++) {
        printf("#");
    }
    printf("\n");
}
