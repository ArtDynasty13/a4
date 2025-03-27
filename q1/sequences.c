//does not work for add1 but add 1 will run and so will add n will run n
//ideas add functions to an array, and execute the array each time
//DO NOT FORGET TO FREE MEMORY!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Operation {
    int (*op)(int, int);
    int operand;
};

struct Operation *make_operation(int (*op)(int, int), int operand) {
    struct Operation *new_operation = malloc(sizeof(struct Operation));
    new_operation->op = op;
    new_operation->operand = operand;
    return new_operation;
}

struct Sequence {
    int size;
    int capacity;
    struct Operation *operations;
};

struct Sequence *create_sequence() {
    struct Sequence *new_sequence = malloc(sizeof(struct Sequence));
    new_sequence->size = 0;
    new_sequence->capacity = 4;
    new_sequence->operations = malloc(sizeof(struct Operation)*new_sequence->capacity);
    return new_sequence;
}

void add_operation(struct Sequence *sequence, struct Operation *op) {
    if(sequence->capacity == sequence->size) { //we need to dobule the array
        struct Operation *new_operations = malloc(sizeof(struct Operation)*sequence->capacity*2);
        for(int i = 0; i < sequence->size; ++i) {
            new_operations[i] = sequence->operations[i];
        }
        free(sequence->operations);
        sequence->operations = new_operations;
        sequence->capacity *= 2;
    }
    sequence->operations[sequence->size] = *op;
    ++sequence->size;
}

int execute(struct Sequence *sequence, int *cur) { //we pass a pointer to the main stackframes current
    for(int i = 0; i < sequence->size; ++i) {
        *cur = sequence->operations[i].op(*cur, sequence->operations[i].operand);
    }
    return *cur;
}

int add(int cur, int operand) {
    return cur + operand;
}

int substract(int cur, int operand) {
    return cur - operand;
}

int divide(int cur, int operand) {
    return cur / operand; //make sure integer div is okay
}

int mutiply(int cur, int operand) {
    return cur * operand;
}

int main(int argc, char *argv[]) {
    int cur = atoi(argv[1]);
    struct Sequence *sequence = create_sequence();

    while(1) {
        char command[4] = {0};
        int result = scanf("%s", command);
        if(result == EOF) break;
        else if (result != 1) continue;

        int num;
        if(strcmp(command, "add")==0) {
            if(scanf("%d", &num)) {
                struct Operation *op = make_operation(add, num);
                add_operation(sequence, op);
                free(op);  // We can free this because add_operation makes a copy
            }
        }
        else if(strcmp(command, "sub")==0) {
            if(scanf("%d", &num)) {
                struct Operation *op = make_operation(substract, num);
                add_operation(sequence, op);
                free(op);
            }
        }
        else if(strcmp(command, "mul")==0) {
            if(scanf("%d", &num)) {
                struct Operation *op = make_operation(mutiply, num);
                add_operation(sequence, op);
                free(op);
            }
        }
        else if(strcmp(command, "div")==0) {
            if(scanf("%d", &num)) {
                struct Operation *op = make_operation(divide, num);
                add_operation(sequence, op);
                free(op);
            }
        }
        else if(strcmp(command, "n")==0) {
            printf("%d\n", execute(sequence, &cur));
        }
    }

    // Free all allocated memory
    free(sequence->operations);  // Free the operations array
    free(sequence);

    //WE NEED TO FREE HERE!
}

