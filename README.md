# computer-architecture
# c-programming

C programming done for my computer architecture course (CS211) at Rutgers.

### Data Structures

Implemented linked list, hash table, binary search trees in C.

### One Shot Learning

Used linear algebra to train an algorithm with data about house attributes and prices.
Calculated weight of each attribute (year built, etc.) on the house price using linear algebra.
Predicted prices of houses based on those weights for house with specific attributes.

### Bit Operations

#### First

- Set the nth value of a function
- Set the value of the nth bit of a number to its complement
- Return the value of the nth bit of a number

#### Second

- Return parity of a number

#### Third

- Return whether a bit string is a palindrome

### Assembly Interpreter

- Interpreter for a custom version of Assembly written in C.
- There are 4 general purpose registers: ax, bx, cx, dx.
- The following functions are implemented:

#### Moving Data Instructions

| Instruction | Args | Description                       |
|-------------|------|-----------------------------------|
| mov         | x y  | Copy x to y, leaving x unchanged. |

- Note: y must be a register.

#### Arithmetic Instructions

| Instruction | Args | Description                                           |
|-------------|------|-------------------------------------------------------|
| add         | x y  | Add x and y, place result in y                        |
| sub         | x y  | Subtract x and y, place result in y                   |
| mul         | x y  | Multiply x and y, place result in y                   |
| div         | x y  | Divide x by y, place result in y (discard remainders) |

- Note: y must be a register.

#### Jump Instructions

| Instruction | Args  | Description         |
|-------------|-------|---------------------|
| jmp         | L     | Jump to L           |
| je          | L x y | Jump to L if x = y  |
| jne         | L x y | Jump to L if x != y |
| jg          | L x y | Jump to L if x > y  |
| jge         | L x y | Jump to L if x >= y |
| jl          | L x y | Jump to L if x < y  |
| jle         | L x y | Jump to L if x <= y |

- Jumps take arguments to compare and jump to the line specified by L.
- Note: L must be an integer.

#### I/O Instructions

| Instruction | Args | Description                                                 |
|-------------|------|-------------------------------------------------------------|
| read        | x    | Reads a 16-bit signed integer from stdin and stores it in x |
| print       | x    | Prints x to stdout                                          |

- Note: x must be a register for read.
- Note: All arguments to all instructions are separated by a single space.
