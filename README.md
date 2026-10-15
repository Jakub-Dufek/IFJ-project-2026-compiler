<!--
 @file README.md (DEVELOPMENT.md)
 @internal Part of IFJ-project-2026-compiler, Team 274855

 @brief File to be used for internal communication between team members on code structure, architecture and development cycles
 @author Jakub Dufek (274855)
 @date 29. 9. 2026
-->


# IFJ-project-compiler-2026-27
README.md file is to be used for internal communication between team members on code structure, architecture and development cycles. After setting this repository to public, it shall be renamed to e.g. DEVELOPMENT.md and instructions on compiler usage or specs shall be provided instead. 

# Error handling
Instead of handling error messages individually, every source file must include the "error.h" header file and use the error_code enumerate and error_exit function included for uniform error message handling. Read "error.h" source file for instructions on how to report errors within compiler.

# Makefile usage
All changes commited to any branch (and especially main) are required to be compilable without any errors. A safe practice is to use the 'make' command beforehand and source file changes commited to any branch (and especially main). For further information see 'make help'

# clang-format usage
Uniform formatting is required by project assignment. A safe practice is to use the 'clang-format --dry-run \[filename\].c' command before each commit to any branch (and especially main).
