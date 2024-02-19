// Julia Enriquetto de Brito
#include <cs50.h>
#include <stdio.h> //libraries

int main(void)
{
    string name = get_string("What's your name? "); // get user name
    printf("hello, %s\n", name);                    // print hello, <name>
}
