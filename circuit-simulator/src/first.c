#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InputVar {
    char* name;
    int value;
} InputVar;

typedef struct OutputVar {
    char* name;
    int value;
} OutputVar;

typedef struct TempVar {
    char* name;
    int value;
} TempVar;

int getBit(int x, int n);
int getInput(char* name, InputVar* inputs, int numInputs, TempVar* temps, int instructionCount);
int isUppercase(char chr);
int execute(char* operation, int input1, int input2);
int getResult(char* operation, char* input1, char* input2, InputVar* inputs, int numInputs, TempVar* temps, int instructionCount);
int checkOutput(char* name, OutputVar* outputs, int numOutputs);

int getBit(int x, int n) {
    int bit = (x >> n) & 1;
    return bit;
}

int getInput(char* name, InputVar* inputs, int numInputs, TempVar* temps, int instructionCount) {
    int isUpper = isUppercase(name[0]);

    // printf("temps[0] in getInput: %s\n", temps[0].name);
    if (isUpper) {
        for (int i = 0; i < numInputs; i++) {
            if (strcmp(name, inputs[i].name) == 0) {
                return inputs[i].value;
                break;
            }
        }
    }
    else {
        for (int i = 0; i < instructionCount; i++) {
            if (strcmp(name, temps[i].name) == 0) {
                return temps[i].value;
                break;
            }
        }
    }

    return -1;
}

int isUppercase(char chr) {
    return ((chr >= 'A' && chr <= 'Z'));
}

int execute(char* operation, int input1, int input2) {
    if (strcmp(operation, "NOT") == 0) {
        return (!input1);
    }
    else if (strcmp(operation, "AND") == 0) {
        return (input1 && input2);
    }
    else if (strcmp(operation, "OR") == 0) {
        return (input1 || input2);
    }
    else if (strcmp(operation, "NAND") == 0) {
        return (!(input1 && input2));
    }
    else if (strcmp(operation, "NOR") == 0) {
        return (!(input1 || input2));
    }
    else if (strcmp(operation, "XOR") == 0) {
        return (input1 ^ input2);
    }    

    return -1;
}

int getResult(char* operation, char* input1, char* input2, InputVar* inputs, int numInputs, TempVar* temps, int instructionCount) {
    int in1 = getInput(input1, inputs, numInputs, temps, instructionCount);
    // printf("getInput(%s): %d\n", input1, in1);
    int in2 = getInput(input2, inputs, numInputs, temps, instructionCount);
    // printf("getInput(%s): %d\n", input2, in2);

    if (in1 == -1 || in2 == -1) {
        return -1;
    }

    return execute(operation, in1, in2);
}

int checkOutput(char* name, OutputVar* outputs, int numOutputs) {
    int isUpper = isUppercase(name[0]);
    if (isUpper) {
        for (int i = 0; i < numOutputs; i++) {
            if (strcmp(name, outputs[i].name) == 0) {
                return i;
                break;
            }
        }
    }
    
    return -1;
}

int main(int argc, char* argv[]) {
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("file error\n");
        return 0;
    }

    int maxLineLength = 200;
    char inputLine[maxLineLength];
    char outputLine[maxLineLength];

    char input_pieces[20][11]; // 20 max pieces, 11 max characters each
    char output_pieces[20][11]; // 20 max pieces, 11 max characters each

    // Store individual input pieces of input line in an array
    fgets(inputLine, maxLineLength, fp);
    char* input_piece = strtok(inputLine, " "); // get first piece
    int pieceCount1 = 0;
    while (input_piece != NULL) {
        // get rid of "\n"
        if (strstr(input_piece, "\n") != NULL) {
            input_piece[strlen(input_piece)-1] = '\0';
        }
        strcpy(input_pieces[pieceCount1], input_piece);
        input_piece = strtok(NULL, " ");
        pieceCount1++;
    }
    int numInputs = atoi(input_pieces[1]);

    // Store Inputs
    InputVar* inputs = (InputVar*) malloc(numInputs * sizeof(InputVar));
    for (int i = 0; i < numInputs; i++) {
        inputs[i].name = "";
        inputs[i].value = 0;
    }

    // Populate Inputs
    int j = 2;
    for (int i = 0; i < numInputs; i++) {
        inputs[i].name = input_pieces[j];
        j++;
    }

    // Store individual output pieces of output line in an array
    fgets(outputLine, maxLineLength, fp);
    char* output_piece = strtok(outputLine, " "); // get first piece
    int pieceCount2 = 0;
    while (output_piece != NULL) {
        // get rid of "\n"
        if (strstr(output_piece, "\n") != NULL) {
            output_piece[strlen(output_piece)-1] = '\0';
        }
        strcpy(output_pieces[pieceCount2], output_piece);
        output_piece = strtok(NULL, " ");
        pieceCount2++;
    }
    int numOutputs = atoi(output_pieces[1]);

    // Store Outputs
    OutputVar* outputs = (OutputVar*) malloc(numOutputs * sizeof(OutputVar));
    for (int i = 0; i < numOutputs; i++) {
        outputs[i].name = "";
        outputs[i].value = 0;
    }

    // Populate Outputs
    j = 2;
    for (int i = 0; i < numOutputs; i++) {
        outputs[i].name = output_pieces[j];
        j++;
    }

    // Add instructions to array
    int instructionCount = 0;
    char **instructions = malloc(0);
    int lineLength = 40;
    char instruction[lineLength];
    while(fgets(instruction, lineLength, fp)) {
        char* str = (char*) malloc(lineLength + 1);
        strcpy(str, instruction);

        if (strstr(str, "\n") != NULL) {
            str[strlen(str)-1] = '\0';
        }

        instructionCount++;
        instructions = (char**) realloc(instructions, instructionCount * sizeof(instruction));
        instructions[instructionCount - 1] = str;
    }

    // Create and initialize temps array
    TempVar* temps = (TempVar*) malloc(instructionCount * sizeof(TempVar));
    for (int i = 0; i < instructionCount; i++) {
        temps[i].name = "";
        temps[i].value = 0;
    }

    // char* resArray[instructionCount]; // equal to the number of temp variables
    char **resArray = malloc(0);
    int resSize = 0;

    // Iterate over every combination of bits
    // int resCount = 0;
    int printCounter = 0;
    int combinations = (2 << (numInputs - 1));
    while (printCounter < combinations) {
        // Populate inputs array with input for this combination
        int i = 0;
        while (i < numInputs) {
            inputs[numInputs - 1 - i].value = getBit(printCounter, i);
            i++;
        }
        
        // Iterate through all instructions
        int tempCount = 0;
        for (int i = 0; i < instructionCount; i++) {
            // Separate instruction into pieces
            char pieces[4][11]; // 4 max pieces, 10 max characters each

            char* instCopy[40];
            strcpy(instCopy, instructions[i]);

            char* piece = strtok(instCopy, " "); // get first piece

            // Loop to get all pieces and store in pieces
            int pieceCount = 0;
            while (piece != NULL) {
                // get rid of "\n"
                if (strstr(piece, "\n") != NULL) {
                    piece[strlen(piece)-1] = '\0';
                }
                strcpy(pieces[pieceCount], piece);
                piece = strtok(NULL, " ");
                pieceCount++;
            }

            int result = -1;
            int checkIndex = -1;
            if (strcmp(pieces[0], "NOT") == 0) { // if 1 input operation
                result = getResult(pieces[0], pieces[1], "", inputs, numInputs, temps, instructionCount);
                checkIndex = checkOutput(pieces[2], outputs, numOutputs);
                if (checkIndex > -1) {
                    outputs[checkIndex].value = result;
                }
                else if (checkIndex == -1) {
                    tempCount++;
                    resSize++;
                    char* res = (char*) malloc(strlen(pieces[2])+1);
                    resArray = (char**) realloc(resArray, resSize * 11);
                    resArray[resSize - 1] = res;

                    strcpy(res, pieces[2]);
                    temps[tempCount-1].name = res;
                    temps[tempCount-1].value = result;
                    
                }
            }
            else { // if 2 input operation
                result = getResult(pieces[0], pieces[1], pieces[2], inputs, numInputs, temps, instructionCount);
                checkIndex = checkOutput(pieces[3], outputs, numOutputs);
                if (checkIndex > -1) {
                    outputs[checkIndex].value = result;
                }
                else if (checkIndex == -1) {
                    tempCount++;
                    resSize++;

                    char* res = (char*) malloc(strlen(pieces[3])+1);
                    resArray = (char**) realloc(resArray, resSize * 11);
                    resArray[resSize - 1] = res;

                    strcpy(res, pieces[3]);
                    temps[tempCount-1].name = res;
                    temps[tempCount-1].value = result;
                }
            }
        }

        // Printing
        for (int i = 0; i < numInputs; i++) {
            printf("%d ", inputs[i].value);
        }
        for (int j = 0; j < numOutputs; j++) {
            printf("%d ", outputs[j].value);
        }
        printf("\n");

        printCounter++;
    }

    // Freeing
    free(inputs);
    free(outputs);
    free(temps);

    for (int i = 0; i < instructionCount; i++) {
        free(instructions[i]);
    }
    free(instructions);

    for (int i = 0; i < resSize; i++) {
        free(resArray[i]);
    }
    free(resArray);

    return 0;
}