/**
* @file error.h
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief Header file for exitting with error message
* @author Jakub Dufek (274855)
* @date 26. 9. 2026
*
* Error code spec translation and enum value naming done with AI tools (Gemini flash 3.8)
*
*/

#ifndef ERROR_H
#define ERROR_H

/**
* @brief Enum values for individual error codes
*/
enum error_code {
    LEX_ERR = 1,        // lexical error            - (invalid lexeme structure, comment, indentation)
    SYNT_ERR = 2,       // syntax error             - (invalid syntax, missing prologue, disallowed construct, invalid block indentation)
    SEM_UNDEF_ERR = 3,  // semantic error           - (undefined function, variable, type, missing @main function)
    SEM_REDEF_ERR = 4,  // semantic error           - (redefinition of function, variable, type)
    SEM_ARG_ERR = 5,    // semantic error           - (incorrect argument count/type in function call, disallowed @main parameters)
    SEM_TYPE_ERR = 6,   // semantic error           - (type compatibility, type deduction in expression, assignment, initialization, return value, control construct)
    SEM_VAR_ERR = 7,    // semantic error           - (assignment to immutable variable, formal parameter)
    SEM_OTHER_ERR = 10, // other semantic error     - (disallowed return, non-exhaustive match, zero range step)
    INTERNAL_ERR = 99   // internal compiler error  - (memory allocation failure)
};

/**
* @brief Prints error with custom error message and exits with specified exit code
*
* @param err_code Exit/error code
* @param line Line number where error occured
* @param fmt Custom error message
*/
void error_exit(int err_code, unsigned line, const char *fmt, ...);

#endif