// ######################### C O M P E N D I U M #############################

void mutate(Board& child)

/*cross two parents to create 2 children that output as arguments */
void cross(Board parent1, Board parent2, Board& child1, Board& child2)

/*swaps content of two Board pointers */
void swap(Board* a, Board* b)

/*sorts the boards in a Board* array in decreasing order by fitness */
void quicksort(Board* arr, int left, int right)


// Class Board ==============================================================#
class Board{
    public:
        int fitness;
        Board();
        void start();
        int collisions();
        int* queens;
        void print();
};

void Board::print()

Board::Board() //called for child

/*randomises the values in the array pointer queens*/
void Board::start()

/*returns the number of collisions */
int Board::collisions()


// Class Population =========================================================#
class Population{
    public:
        // variables
        int count;
        list<int*> winners;
        Board* pop;
        //methods
        Population(int x);
        int fitness();
        int truncate_selection(int divider);
        void selection(int total_fitness);
        void breed();
        void print();
};

Population::Population(int x) : count(x)

void Population::print()

int Population::truncate_selection(int divider)

/* */
void Population::selection(int fitness_sum)

void Population::breed()

int Population::fitness()



