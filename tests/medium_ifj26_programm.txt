//import ifj26.*

def printNumber(value: Int): Unit =
    print("value = ")
    print(value)
    print("\n")

def processNumber(value: Int): Int =
    var result: Int = value

    if result > 0 then
        if result >= 10 then
            result = result * 2

            while result > 20 do
                result = result - 3

                if result == 25 then
                    print("twenty five\n")
                else
                    print("not twenty five\n")
        else
            result = result + 5
    else
        if result < 0 then
            result = 0 - result
        else
            result = 1

    result

def countdown(start: Int): Unit =
    var i = start

    while i > 0 do
        printNumber(i)

        if i > 5 then
            print("large\n")
        else
            if i == 5 then
                print("middle\n")
            else
                print("small\n")

        i = i - 1

    ()

@main
def main(): Unit =
    val input = 12
    val processed = processNumber(input)

    print("processed result:\n")
    printNumber(processed)

    if processed > 10 then
        countdown(processed)
    else
        print("nothing to count\n")

    ()