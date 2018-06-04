#include <iostream>
#include <time.h>
using namespace std;

int collisions(int* board, int size){
    int* lines = (int*)(calloc(sizeof(int), size));
    int* diagTB = (int*)(calloc(sizeof(int), size*2 -1));
    int* diagBT = (int*)(calloc(sizeof(int), size*2 -1));

    for (int i = 0; i < size; i++){
        lines[board[i]]++;
        diagTB[i-board[i] + size-1]++;
        diagBT[board[i]+i]++;
    }

    for (int i = 0; i < size; i++)
        if (lines[i] > 1){
            return 0;
        }

    for (int i = 0; i < size*2 -1; i++)
        if (diagTB[i] > 1 || diagBT[i] > 1){
            return 0;
        }

    return 1;
}



int main (){
    srand(time(NULL));
    int size = 9;
    int* board = (int*)(calloc(sizeof(int), size));
    int Wcount = 0;
    while (Wcount < 50){
        for (int i = 0; i < size; i++)
            board[i] = rand() % size;
        if (collisions(board, size)){
            Wcount++;
            for (int i = 0; i < size; i++)
                cout << board[i] << "| ";
            cout << endl;
        }
    }
}
