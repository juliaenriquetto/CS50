// Julia Enriquetto de Brito
#include <cs50.h>
#include <stdio.h>

int getCents(void);
int getQuarters(int cents);
int getDimes(int cents);
int getNickels(int cents);
int getPennies(int cents);

int main(void)
{
    // how many cents
    int cents = getCents();

    // number of quarters
    int quarters = getQuarters(cents);
    cents = cents - quarters * 25; // see how many quarters are necessary

    // number of dimes
    int dimes = getDimes(cents);
    cents = cents - dimes * 10;

    // number of nickels
    int nickels = getNickels(cents);
    cents = cents - nickels * 5;

    // number of pennies
    int pennies = getPennies(cents);
    cents = cents - pennies * 1;

    // sum of all the coins
    long coins = quarters + dimes + nickels + pennies;
    printf("%li\n", coins);
}

int getCents(void)
{
    int cents = 0;
    do
    {
        cents = get_int("Cents Owed: "); // ask how many cents
    }
    while (cents < 0); // don't use negative numbers

    return cents;
}

int getQuarters(int cents)
{
    int quarters = 0;
    while (cents >= 25) // while the cents is equal or upper to 25, then we can count quarters
    {
        cents = cents - 25; // subtract 25 cents until 0 or < 25
        quarters++;         // counter of used quarters
    }
    return quarters;
}

int getDimes(int cents)
{
    int dimes = 0;
    while (cents >= 10) // while the cents is equal or upper to 10, then we can count dimes
    {
        cents = cents - 10; // subtract 10 cents until 0 or < 10
        dimes++;            // counter of used dimes
    }
    return dimes;
}

int getNickels(int cents) //  while the cents is equal or upper to 5, then we can count nickels
{
    int nickels = 0;
    while (cents >= 5)
    {
        cents = cents - 5; // subtract 5 cents until 0 or < 4
        nickels++;         // counter of used nickels
    }
    return nickels;
}

int getPennies(int cents)
{
    int pennies = 0;
    while (cents >= 1) // while the cents is equal or upper to 1, then we can count pennies
    {
        cents = cents - 1; // subtract 1 cent until 0
        pennies++;         // counter of used pennies
    }
    return pennies;
}
