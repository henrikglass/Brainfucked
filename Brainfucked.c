#include <stdio.h>

#define DATA_SIZE 65536
#define PROGRAM_SIZE 65536

int main(int argc, char *argv[]) {

    unsigned short data[DATA_SIZE];
    char program[PROGRAM_SIZE];
    int loop_map[PROGRAM_SIZE];
    short * data_ptr = data;
    int p_size = 0;
    int count = 0;
    int input = 0;

    //Load file
    FILE* inputF = fopen(argv[1], "r");
    if (inputF == NULL){
        perror("Error opening file");
        return -1;
    }

    //load program into the program[]-array
    for ( ; !feof(inputF); p_size++) {
        program[p_size] = fgetc(inputF);
    }

    //walk through program and assign loop pointers. (+performance -space)
    for (int i = 0; i < p_size; i++) {
        if(program[i] == '['){
            count = 1;
            int j = i;
            while (count) {
                j++;
                if (program[j] == '[') count++;
                if (program[j] == ']') count--;
            }
            loop_map[i] = j;
            loop_map[j] = i;
        }
    }

    //run program
    for(int pc = 0; pc <= p_size; pc++){
        switch (program[pc]) {
            case '+': (*data_ptr)++; break;
            case '-': (*data_ptr)--; break;
            case '>': data_ptr++; break;
            case '<': data_ptr--; break;
            case ',':
                input = (unsigned int) getchar();
                *data_ptr = (input == EOF) ? 0xFF : input;
                break;
            case '.': putchar(*data_ptr); break;
            case '[':
                if (!*data_ptr) {
                    pc = loop_map[pc];
                }
                break;
            case ']':
                if (*data_ptr) {
                    pc = loop_map[pc];
                }
                break;
        }
    }
    fclose(inputF);
    return 0;
}
