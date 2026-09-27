# IFJ-project-compiler-2026-27
README.md file is to be used for internal communication between team members on code structure, architecture and development cycles. After setting this repository to public, it shall be renamed to e.g. DEVELOPMENT.md and instructions on compiler usage or specs shall be provided instead. 
# Error handling
Instead of handling error messages individually, every source file must include the "error.h" header file and use the error_code enumerate and error_exit function included for uniform error message handling. Read "error.h" source file for instructions on how to report errors within compiler.
