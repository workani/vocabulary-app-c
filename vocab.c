#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

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
int lines_count;

int count_lines(FILE *f);
void get_filename(char *fname);
void allocate_mem(char **dictionary, int n);
void populate_arrs(FILE *fptr);


int main(void)
{
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

    int current_word = 0;

    int correct_answers = 0;
    int incorrect_answers = 0; 
    int score = 0;

   
    // loop until the end of the loaded vocabulary or until the user prints exit
    while(current_word < lines_count && strcmp(input, "exit") != 0)
    {
        printf("%sWrite german translation for word %s\"%s\": \n", WHT, YEL, source_vocab[current_word]);
        printf("%sTranslation: ", WHT);
        scanf("%54s", input);

        if(strcmp(input, target_vocab[current_word]) == 0)
        {
            correct_answers++;
            score++;
            system("clear");
            printf("%sCorrect! +1 point\n", GRN);
            sleep(1);
        }
        else
        { 
            if(score > 0) score--;
            incorrect_answers++;
            system("clear");
            printf("%sIncorrect :( Score: %i\n", RED, score);
            sleep(1);
        }
        
        system("clear");
        current_word++;
    }

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