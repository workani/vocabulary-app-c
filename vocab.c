#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


// ensure cross platform compability by defining clear function and import diffrent libraries for windows and diffrent systems
#ifdef _WIN32
#include <windows.h>
#define CLEAR  "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

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

// arrays for storing user's correct and incorrect answers
 char **correct_input = NULL;
 char **incorrect_input = NULL;

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
void free_mem(void);
void populate_arrs(FILE *fptr);
void get_input(char *user_input, int index);
void print_results(void);
void print_detailed_results(void);
void correct_answer(char *input);
void incorrect_answer(char *input, int index);

// functoins to get random index for vocab arrays and pointer to array for storing them
int *indices;
void swap(int *a, int *b);
void shuffle(int *arr);



// custom strcasecmp and sleep function to insure ensure compatibility across different platforms.
typedef unsigned char u_char;


int str_casecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);

        if (c1 != c2) {
            return c1 - c2;
        }

        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}


void custom_sleep(int seconds)
{
    #ifdef _WIN32
        Sleep(seconds * 1000);
    #else
        sleep(seconds);
    #endif
}



int main(void)
{
    srand(time(NULL)); //initalize random generator for future use

    char *filename = malloc(256 * sizeof(char));
    get_filename(filename);

    FILE *f = fopen(filename, "r");

    free(filename);

    if(f == NULL)
    {
        system(CLEAR);
        printf("%sFile path is incorrect! Exitinig...\n", RED);
        return 1;
    }
    else
    {
        system(CLEAR);
        printf("%sFile loaded succesfully!\n", GRN);
        custom_sleep(1);
        system(CLEAR);
    }

    lines_count = count_lines(f);

    source_vocab = malloc(lines_count * sizeof(char*));
    target_vocab = malloc(lines_count * sizeof(char*));

    correct_input = malloc(lines_count * sizeof(char*));
    incorrect_input = malloc(lines_count * sizeof(char*));


    indices = malloc(lines_count *sizeof(int));

    for(int i = 0; i < lines_count; i++) // initalize an array of indices for random interation thru vocabulary arrays
    {
        indices[i] = i;
    }

    shuffle(indices);

    if(source_vocab == NULL || target_vocab == NULL  || indices == NULL)
    {
        printf("%sMemory Error. Exiting..", RED);
        free_mem();
        return 1;
    }
    else
    {
        allocate_mem(source_vocab, lines_count);
        allocate_mem(target_vocab, lines_count);
        allocate_mem(correct_input, lines_count);
        allocate_mem(incorrect_input, lines_count);
    }

    // populating source and target arrays with content of user's csv file
    populate_arrs(f);

    char input[STRING_SIZE];


    // loop until the end of the loaded vocabulary or until the user prints exit
    while(word_count < lines_count && str_casecmp(input, "exit") != 0)
    {
        idx = indices[word_count];

        get_input(input, idx);

        if(str_casecmp(input, target_vocab[idx]) == 0)
        {
            correct_answer(input);
        }
        else
        {
            incorrect_answer(input, idx);
        }

        if(str_casecmp(input, "exit") == 0)
        {
            break;
        }

        system(CLEAR);
        word_count++;
    }

    //print out results
    print_results();

    // clean up memory
    free_mem();
}

int count_lines(FILE *f)
{
    char c;
    lines_count = 1;

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
    system(CLEAR);

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

// deallocate memory from all sript arrays

void free_mem(void)
{
    free(source_vocab);
    free(target_vocab);
    free(incorrect_input);
    free(indices);
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
        }

        token = strtok(NULL, ";");

        if(token != NULL)
        {
            size_t len = strlen(token);
            strncpy(target_vocab[count], token, len);
        }
        count++;
    }
}

void get_input(char *user_input, int index)
{
    system(CLEAR);
    printf("%sTotal: %i/%i\n", MAG, word_count, lines_count);
    printf("%sType translation for %s\"%s\"%s: \n", WHT, YEL, source_vocab[idx], WHT);
    printf("-> ");
    scanf("%54s", user_input);
}

void print_results(void)
{
    system(CLEAR);
    char choice;
    printf("%s+------------------------------End of the session-----------------------------+\n", CYN);
    printf("%sTotal score: %i/%i\n", MAG, score, lines_count);
    printf("%sCorrect answers: %i\n", GRN, correct_answers);
    printf("%sIncorrect answers: %i\n", RED, incorrect_answers);
    printf("%s+-----------------------------------------------------------------------------+\n", CYN);
    printf("%sDo you want to see detailed results of your session? (y or n): ", WHT);

    // get character input from user and convert it to lowercase
    scanf(" %c", &choice);
    choice = tolower(choice);

    // print detailed results if the choice is 'y', else exit
    if(choice == 'y')
    {
        print_detailed_results();
    }
    else
    {
        system(CLEAR);
        exit(0);
    }


}

void print_detailed_results(void)
{
    system(CLEAR);

    // print out all correct and incorrect answers if they exist
    if(correct_answers > 0 && correct_input != NULL)
    {
        printf("%sCorrect answers:\n", GRN);
        for(int i = 0; i < correct_answers; i++)
        {
            printf("%s\n", correct_input[i]);
        }
    }


    if(incorrect_answers > 0)
    {
        printf("%sIncorrect answers:\n", RED);
        for(int j = 0; j < incorrect_answers; j++)
        {
            printf("%s\n", incorrect_input[j]);
        }
    }
}

void correct_answer(char *input)
{
    correct_input[word_count] = input;
    correct_answers++;
    score++;
    system(CLEAR);
    printf("%sCorrect!\n", GRN);
    custom_sleep(1);
}

void incorrect_answer(char *input, int index)
{
    system(CLEAR);

    // checking if user is exiting from the program; if not, incrementing incorrect answer count.
    if(str_casecmp(input, "exit") != 0)
    {
        incorrect_input[word_count] = input;
        incorrect_answers++;
        printf("%sIncorrect :(\n", RED);
    }

    while(str_casecmp(input, target_vocab[idx]) != 0 && str_casecmp(input, "exit") != 0)
    {
        get_input(input, idx);
    }
    custom_sleep(1);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void shuffle(int *arr)
{
    for(int i = lines_count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&indices[i], &indices[j]);
    }
}

