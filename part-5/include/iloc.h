// João Vitor de Camargo
// Marcellus Farias

#ifndef ILOC_H
#define ILOC_H

/* Possible values for iloc_arg->type */
#define CONSTANT 122
#define LABEL	 123
#define REGISTER 124

/* To generate new unique regs */
static int reg_count = 0;

/*  Just integer constants and
	identifiers will be used for now */
union argument {
	int 	int_const;
	char* 	str_var;
};

typedef struct i_arg {
	int type;
	union argument arg; 
} iloc_arg;

typedef struct i_operation {
	int op_code;
	int num_args;
	iloc_arg** args;
} iloc_operation;

typedef struct i_op_list {
	int num_ops;
	iloc_operation** ops;
} iloc_op_list;

iloc_op_list* new_op_list();
iloc_op_list* concat_code(iloc_op_list* list1, iloc_op_list* list2);

iloc_arg* new_arg(int type, void* argum);

iloc_operation* new_1arg_op(int op_code, iloc_arg* arg1);
iloc_operation* new_2arg_op(int op_code, iloc_arg* arg1, iloc_arg* arg2);
iloc_operation* new_3arg_op(int op_code, iloc_arg* arg1, iloc_arg* arg2, iloc_arg* arg3);
iloc_operation* new_nop();

void add_op(iloc_op_list* list, iloc_operation* op);

char* new_reg();

void free_op_list(iloc_op_list* list);

void print_code(iloc_op_list* list);

iloc_operation* loadi(int value, char* reg);

iloc_operation* loadai(char* regop1, int value, char* regdst);

iloc_operation* store(char* regop1, char* regdst);

iloc_operation* add(char* regop, char* regop2, char* regdst);

iloc_operation* sub(char* regop1, char* regop2, char* regdst);

iloc_operation* mult(char* regop1, char* regop2, char* regdst);

iloc_operation* div_op(char* regop1, char* regop2, char* regdst);

iloc_operation* cmp_gt(char* regop1, char* regop2, char* regdst);

iloc_operation* cmp_lt(char* regop1, char* regop2, char* regdst);

iloc_operation* cmp_le(char* regop1, char* regop2, char* regdst);

iloc_operation* cmp_ge(char* regop1, char* regop2, char* regdst);

iloc_operation* cmp_eq(char* regop1, char* regop2, char* regdst);

iloc_operation* cmp_ne(char* regop1, char* regop2, char* regdst);

#endif

