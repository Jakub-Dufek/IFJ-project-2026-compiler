#include "../src/error.h"

int main() {
    int x = 6025;
    error_exit(SEM_UNDEF_ERR, 105, "Semantic error: wrong variable type, count: %d", x);
}