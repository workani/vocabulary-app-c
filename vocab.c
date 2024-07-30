#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// defining constants to print out text with diffrent colors.
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

#define LINE_SIZE 100
#define STRING_SIZE 55

// arrays for storing vocabulary
 char **source_vocab = NULL;
 char **target_vocab = NULL;

//count amount of lines
int lines_count = 0;

// all necessary variables to keep track of user progress
int idx = 0;
int word_count = 0;
int correct_answers = 0;
int incorrect_answers = 0; 
int score = 0;

// all prototypes for script's functions
int count_lines(FILE *f);
void get_filename(char *fname);
void allocate_mem(char **dictionary, int n);
void populate_arrs(FILE *fptr);
void get_input(char *user_input, int idx);
void print_results(void);
void correct_answer();
void incorrect_answer(char *input, int idx);

// functoins to get random index for vocab arrays
void swap(int *a, int *b);
void shuffle(int *indices);



int main(void)
{
    srand(time(NULL)); //initalize random generator for future use 

    char *filename = malloc(256 * sizeof(char));
    get_filename(filename);

    FILE *f = fopen(filename, "r");

    free(filename);

    if(f == NULL)
    {
        system("clear");
        printf("%sFile path is incorrect! Exitinig...\n", RED);
        return 1;
    }
    else
    {
        system("clear");
        printf("%sFile loaded succesfully!\n", GRN);
        sleep(1);
        system("clear");
    }

    lines_count = count_lines(f);

    
    int indices[lines_count];

    for(int i = 0; i < lines_count; i++) // initalize an array of indices for random interation thru vocabulary arrays 
    {
        indices[i] = i;
    }

    shuffle(indices);

    source_vocab = malloc(lines_count * sizeof(char*));
    target_vocab = malloc(lines_count * sizeof(char*));

    if(source_vocab == NULL || target_vocab == NULL)
    {
        printf("%sMemory Error. Exiting..", RED);
        free(source_vocab);
        free(target_vocab);
        return 1;
    }
    else
    {
        allocate_mem(source_vocab, lines_count);
        allocate_mem(target_vocab, lines_count);
    }

    // populating source and target arrays with content of user's csv file
    populate_arrs(f);

    char input[STRING_SIZE];

   
    // loop until the end of the loaded vocabulary or until the user prints exit
    while(word_count < lines_count && strcasecmp(input, "exit") != 0)
    {
        int idx = indices[word_count];

        char input[STRING_SIZE];
        get_input(input, idx);

        if(strcasecmp(input, target_vocab[idx]) == 0)
        {
            correct_answer();
        }
        else
        { 
            incorrect_answer(input, idx);
        }

        if(strcasecmp(input, "exit") == 0)
        {
            break;
        }

        system("clear");
        word_count++;
    }
    
    //print out results
    print_results();
}

int count_lines(FILE *f)
{
    char c;
    int lines_count = 1;

   // Reading file until the end and incrementing lines_count with each new line
    do
    {
        c = getc(f);
        if(c == '\n') lines_count++;
    } while(c != EOF);

    rewind(f);
    return lines_count;
}

void get_filename(char *fname)
{
    system("clear");

    printf("+------------------------------Welcome-----------------------------+\n");
    printf("With this app, you can learn new vocabulary from foreign languages.\n");
    printf("To start, please provide path to csv file with vocabulry.\n");
    printf("by @workani.\n");
    printf("+------------------------------------------------------------------+\n");
    printf("Path: ");
    scanf("%255s", fname);
}

void allocate_mem(char **dictionary, int n)
{
    for(int i = 0; i < n; i++)
    {
        dictionary[i] = malloc(STRING_SIZE * (sizeof(char)));
        if(dictionary[i] == NULL)
        {
            printf("%sMemory Error. Exiting..", RED);
            return;
        }
    }
}


void populate_arrs(FILE *fptr)
{
    char line[LINE_SIZE];
    int count = 0;

    while(fgets(line, sizeof(line), fptr) != NULL)
    {
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, ";");


        if(token != NULL)
        {
            size_t len = strlen(token);
            strncpy(source_vocab[count], token, len);
            //source_vocab[count][len] = '\n';
        }

        token = strtok(NULL, ";");
       
        if(token != NULL)
        {
            size_t len = strlen(token);
            strncpy(target_vocab[count], token, len);
            //target_vocab[count][len] = '\n';
        }
        count++;
    }
}

void get_input(char *user_input, int idx)
{
    system("clear");
    printf("%sTotal: %i/%i\n", MAG, word_count, lines_count);
    printf("%sType translation for %s\"%s\"%s: \n", WHT, YEL, source_vocab[idx], WHT);
    printf("-> ");
    scanf("%54s", user_input);
}

void print_results(void)
{
    system("clear");
    printf("%s+------------------------------End of the session-----------------------------+\n", CYN);
    printf("%sTotal score: %i/%i\n", MAG, score, lines_count);
    printf("%sCorrect answers: %i\n", GRN, correct_answers);
    printf("%sIncorrect answers: %i\n", RED, incorrect_answers);
    printf("%s+-----------------------------------------------------------------------------+\n", CYN);
}

void correct_answer()
{
    correct_answers++;
    score++;
    system("clear");
    printf("%sCorrect! +1 point\n", GRN);
    sleep(1);
}

void incorrect_answer(char *input, int idx)
{
    system("clear");

    // checking if user is exiting from the program; if not, incrementing incorrect answer count.
    if(strcasecmp(input, "exit") != 0) 
    {
        incorrect_answers++;
        printf("%sIncorrect :(\n", RED);
    }

    while(strcasecmp(input, target_vocab[idx]) != 0 && strcasecmp(input, "exit") != 0)
    {
        get_input(input, idx);
    }
    sleep(1);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void shuffle(int *indices)
{
    for(int i = lines_count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&indices[i], &indices[j]);
    }
}