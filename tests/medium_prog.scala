
// THIS IS A COMMENT
    /* and this is a block
        comment
            for y'all 
                */

import ifj26.*

//  Prints number
def printNumber(value: Int): Unit =
    print("value = ")
    print(value)
    print("\n")

//  Brief: Gives critique on your life
//  param age: Enter your age
//  param happy_i: Happiness index - on a scale of 1 to 10 how happy do you feel?
//  param is_soup: Is cereal a soup?
//  Output: Predicts how many days you have left to live
def lifeAdviser200(age: Int,
                  happy_i : Double,
                  is_soup : Boolean,
                  ): Int =
    if !is_soup then
        print("Your life is a tragedy.\n"); return 0;;
    else
        ()

    if age <= 0 then
        print("You're most likely lying")
        return -42069
    else
        "Yeah boiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"

    val happy_i_int = toInt(happy_i)
    25000 * happy_i_int / 2 / age / (age+4) // the cereal soup secret prediction recipe

//  Counts down from starting number to 0 recursively
def countdown(start: Int): Unit =
    if start <= 0 then
        print(0)
    else
        print(start)
        print(" ")
        var start_dec = start - 1
        countdown(start_dec)

//  MAN
@main
def main(): Unit =

    printNumber(-2)

    print("Enter your age: ")
    val age = readInt()
    print("\n")

    print("How happy are you on a scale of 1 to 10? ")
    val happy_i = readDouble()
    print("\n")

    print("Is cereal a soup? ")
    val is_soup = readBoolean()
    print("\n\n")

    val days = lifeAdviser200(age, happy_i, is_soup)
    print("You have ")
    print(days)
    print(" days to live, or as someone would say it:\n")

    countdown(days)