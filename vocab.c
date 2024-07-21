#include <stdio.h>
#include <stdlib.h>

// defining constants to print out text with diffrent colors.
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

int count_lines(FILE *f);

void get_filename(char *fname);

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
    }
    else
    {
        system("clear");
        printf("%sFile loaded succesfully!\n", GRN);
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
    //scanf("%[^\n]%*c", filename);

}
