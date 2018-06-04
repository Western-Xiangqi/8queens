#include <cstdlib>
#include <list>
#include <iostream>
#include <time.h>
using namespace std;
#define fitnessmax 56
#define mutation_rate 70
#define pop_limit 100

class Board{
    public:
        // variables
        int fitness;
        int* queens;
        // methods
        Board(int size);
        void print(int size);
        void start(int size);
        int collisions(int size);
};

class Population{
    public:
        // variables
        int count;
        list<int*> winners;
        Board* pop;
        int size;
        // methods
        Population(int x, int size);
        int fitness();
        int truncate_selection(int divider);
        void selection(int total_fitness);
        void breed();
        void print();
};

// ###########################################################################


void mutate(Board& child, int size)
{
    *(child.queens + rand() % size) = rand() % size;
}


void cross(Board parent1, Board parent2, Board& child1, Board& child2, int size)
{
    child1 = Board(size);
    child2 = Board(size);

    int cut = rand() % size;

    for (int i = 0; i < cut; i++)
    {
        *(child1.queens + i) = *(parent1.queens + i);
        *(child2.queens + i) = *(parent2.queens + i);
    }
    for (int i = cut + 1; i < size; i++)
    {
        *(child1.queens + i) = *(parent2.queens + i);
        *(child2.queens + i) = *(parent1.queens + i);
    }

    if (rand() % 100 < mutation_rate)
    {
        mutate(child1, size);
    }
    if (rand() % 100 < mutation_rate)
    {
        mutate(child2, size);
    }
}

void swap(Board* a, Board* b)
{
    Board c = *a;
    *a = *b;
    *b = c;
}


void quicksort(Board* arr, int left, int right)
{
    int i = left, j = right;
    int tmp;
    int pivot = (arr + (left + right) / 2)->fitness;

    /* partition */
    while (i <= j)
    {
        while ((arr + i)->fitness > pivot)
            i++;
        while ((arr + j)->fitness < pivot)
            j--;
        if (i <= j)
        {
            swap(*(arr + i), *(arr + j));
            i++;
            j--;
        }
    }
    /* recursion */
    if (left < j)
        quicksort(arr, left, j);
    if (i < right)
        quicksort(arr, i, right);
}


// ###########################################################################


void Board::print(int size)
{
    cout << " { ";
    for (int i = 0; i < size; i++)
    {
        cout << *(queens + i) << "-";
    }
    cout << " fitness : " << fitness;
}

Board::Board(int size) //called for child
{
    fitness = 0;
    queens = (int*)(calloc(sizeof(int), size));
}

void Board::start(int size)
{
    fitness = 0;
    for(int i = 0; i < size; i++)
    {
        *(queens + i) = (rand() % size);
    }
}

int Board::collisions(int size)
{
    int* lines = (int*)(calloc(sizeof(int), size));
    int* diagTB = (int*)(calloc(sizeof(int), size*2 -1));
    int* diagBT = (int*)(calloc(sizeof(int), size*2 -1));
    int colli = 0;

    for (int i = 0; i < size; i++){
        lines[queens[i]]++;
        diagTB[i-queens[i] + size-1]++;
        diagBT[queens[i]+i]++;
    }

    for (int i = 0; i < size; i++)
        colli += lines[i] * (lines[i] - 1);

    for (int i = 0; i < size*2 -1; i++){
        colli += diagTB[i] * (diagTB[i] - 1);
        colli += diagBT[i] * (diagBT[i] - 1);
    }

    return colli;
}


// ###########################################################################



Population::Population(int pop_max, int size_board)
{
    size = size_board;
    count = pop_max;
    winners = list<int*>();
    pop = (Board*)malloc(sizeof(Board) * count);
    for(int i = 0; i < count; i++)
    {
        Board b = Board(size_board);
        b.start(size_board);
        *(pop + i) = b;
    }
}

void Population::print()
{
    cout << "population of " << count << " boards --" <<endl;
    for (int i = 0; i < count; i++)
    {
        (pop + i)->print(size);
        cout << " <- " << i << endl;
    }
    cout << endl;
}

int Population::truncate_selection(int divider)
{
    //	returns position of lowest element to be selected in sorted population    int sum =
    return count / divider - 1;
}

void Population::selection(int fitness_sum)
{
    fitness_sum -= count;

    int sum = 0;
    for(int i = 1; i < count; i++)
    {
        sum += (pop + i)->fitness;
        (pop + i)->fitness += sum;
    }

/* ===== selects a random fitness as treshold =====
    int cut;
    do
    {
        cut = rand() % fitness_sum;
    }while (cut <= 0);

    while((pop + count - 1)->fitness > cut)
    {
        count--;
    }
 = = = = = = = = = = = = = = = = = = = = = = = = */

    int cut = truncate_selection(2);
    count -= cut;
    //  cout << cut << " : new count : " << count << endl;
}

void Population::breed()
{
    Board* newpop = (Board*)(malloc(sizeof(Board) * pop_limit));
    int a = 0;
    int b = 0;
    Board child1 = Board(size);
    Board child2 = Board(size);

    for(int i = 0; i < pop_limit-1; i+=2)
    {
        a = rand() % count;
        do
        {
            b = rand() % count;
        }
        while (b != a);

        cross(*(pop + a), *(pop + b), child1, child2, size);

        *(newpop + i) = child1;
        *(newpop + i + 1) = child2;
    }

    count = pop_limit;
    pop = newpop;
}

int Population::fitness()
{
    int coll = 0; // amount of collisions, calculated below
    int sum = 0;

    for (int i = 0; i < count; i++)
    {
        coll = (pop + i)->collisions(size);
        if (coll == 0)
        {
            int* temp = (int*)(malloc(sizeof(int) * size));
            for (int j = 0; j < size; j++)
            {
                *(temp+j) = *((pop + i)->queens + j);
            }
//
            for (int i = 0; i < size; i++)
                cout << *(temp + i) << " |";
            cout << endl;
//
            winners.push_back(temp);
            (pop + i)->fitness = 0;
        }
        else
        {
            (pop + i)->fitness = fitnessmax - coll;
            sum += fitnessmax - coll;
        }

        //iterator
        //calculate fitness for each board
        // fitness = 56 - coll
        // if fitness = 56 -> add to winner list and set fitness to 0
        // test if board is already in list, dont add it if so
    }

    quicksort(pop, 0, count-1);
    return sum; //sum of fitness
}


// ###########################################################################


int main()
{
    int size = 8;
    srand(time(NULL));
    //srand(time(0));
    Population p = Population(pop_limit, size);

    int iterator = 0;

    while ( p.winners.size() < 96)
    {
        //cout << iterator++ << endl;
        //p.print();
        int sum = p.fitness();
        //p.print();
        p.selection(sum);
        //p.print();
        p.breed();
    }

    cout << "over" << endl;

    return 0;
}






