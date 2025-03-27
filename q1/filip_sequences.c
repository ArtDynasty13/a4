#include <stdio.h>
#include <stdlib.h>

int str_to_int(const char *s){
    int result = 0;
    int sign = 1;

    if(*s == '-'){
        sign = -1;
        s++;
    }

    while(*s){
        result = result * 10 + (*s - '0');
        s++;
    }

    return result*sign;
}

struct opElement{
    char operation;
    int opearnd;
};

struct Sequence{
    struct opElement *operations;
    size_t len, cap;
    int retVal;
};

struct Sequence *initSequence(size_t cap, int startVal){
    struct Sequence *ret = malloc(sizeof(struct Sequence));
    if(ret == NULL){
        fprintf(stderr, "Malloc failure!!!\n");
        return NULL;
    }
    
    ret->operations = malloc(sizeof(struct opElement) * cap);
    if(ret->operations == NULL){
        fprintf(stderr, "Malloc failure!!!\n");
        free(ret);
        return NULL;
    }
    ret->len = 0;
    ret->cap = cap;
    ret->retVal = startVal;
    return ret;
}

//0 for error, 1 for success
int resizeOperations(struct Sequence *o){
    struct opElement *ret = malloc(sizeof(struct opElement) * (o->cap*2));
    if(ret == NULL) return 0;
    for(size_t i = 0; i < o->len ; ++i){
        ret[i] = o->operations[i];
    }
    o->cap *= 2;
    free(o->operations);
    o->operations = ret;    
    return 1;
}

int computeN(struct Sequence *s){
    if(s->len == 0) return s->retVal;

    for(size_t i = 0; i < s->len; ++i){
        switch (s->operations[i].operation) {
            case 'a':
                s->retVal += s->operations[i].opearnd;
                break;
            case 's':
                s->retVal -= s->operations[i].opearnd;
                break;
            case 'm':
                s->retVal *= s->operations[i].opearnd;
                break;
            case 'd':
                //Should we ensure no div by 0 here orrrr?
                s->retVal /= s->operations[i].opearnd;
                break;
        }
    }

    return s->retVal;
}

void pushOp(struct Sequence *s, char operation, int val){
    //Should prob error check this in reality considering i set it up that way too...
    if(s->len == s->cap) resizeOperations(s);

    s->operations[s->len].operation = operation;
    s->operations[s->len].opearnd = val;
    s->len++;
}

void freeSequence(struct Sequence *s){
    free(s->operations);
    free(s);
}
int main(int argc, char *argv[]){

    //freopen("input.txt", "r", stdin);

    if(argc != 2){
        //fix this?
        printf("Usage: %s startVal", argv[0]);
        return 0;
    }   

    struct Sequence *seq = initSequence(4, str_to_int(argv[1]));
    char holder;
    int intHolder;
    while(scanf(" %c", &holder) == 1){
        if(holder == 'n'){
            printf("%d\n", computeN(seq));
        }else if(holder == 'a' ||  holder == 's' || holder == 'm' || holder == 'd'){
            scanf("%*s"); // clear input until whitespace
            scanf("%d", &intHolder); //read the int for the operation
            pushOp(seq, holder, intHolder);
        }
    }
    freeSequence(seq);
    return 0;
}
