#include "../../src/error.h"

int main() {
    int x = 5;
    error_exit(SEM_UNDEF_ERR, 105, "Wrong variable type, your IQ: ~%d", x);
}