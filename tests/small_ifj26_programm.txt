import ifj26.*

def add(a: Int, b: Int): Int =
    val sum = a + b
    return sum

@main
def main(): Unit =
    var x = 10
    var y = 20
    var result = add(x, y)
    print(result)