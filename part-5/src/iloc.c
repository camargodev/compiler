// João Vitor de Camargo
// Marcellus Farias

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/iloc.h"
#include "../include/codes.h"

void set_arg_to_freedom(iloc_arg* arg);
void free_op(iloc_operation* op);
void free_list_and_ops(iloc_op_list* list);

int num_freed_registers = 0;
iloc_arg** freed_registers = NULL;

iloc_op_list* new_op_list() {
	iloc_op_list *list = (iloc_op_list*) malloc(sizeof(iloc_op_list));
	list->num_ops = 0;
	list->ops = NULL;
	return list;
}

iloc_op_list* concat_op_lists(iloc_op_list* list1, iloc_op_list* list2) {
	iloc_op_list *mixed_list = new_op_list();
	int index;
	
	for (index = 0; index < list1->num_ops; index++) {
		add_op(mixed_list, list1->ops[index]);
	}
	for (index = 0; index < list2->num_ops; index++) {
		add_op(mixed_list, list2->ops[index]);
	}
	
	free_list_and_ops(list1);
	list1 = NULL;
	free_list_and_ops(list2);
	list2 = NULL;
	
	return mixed_list;
}

iloc_operation* new_op(int op_code) {
	iloc_operation *op = (iloc_operation*) malloc(sizeof(iloc_operation));
	op->op_code = op_code;
	op->num_args = 0;
	op->args = NULL;
	return op;
}

iloc_arg* new_arg(int type, void* argum) {
	iloc_arg *arg = (iloc_arg*) malloc(sizeof(iloc_arg));
	arg->type = type;
	if (type == CONSTANT) {
		arg->arg.int_const = *((int*) argum);
	} else {
		arg->arg.str_var = (char*) argum;
	}
	return arg;
}

void add_op(iloc_op_list* list, iloc_operation* op) {
	if (list->ops == NULL) {
		list->ops = (iloc_operation**) malloc(sizeof(iloc_operation));
	} else {
		list->ops = (iloc_operation**) realloc(list->ops, (list->num_ops+1) * sizeof(iloc_operation));
	} 
	list->ops[list->num_ops] = op;
	list->num_ops++;
}

void add_arg(iloc_operation* op, iloc_arg* arg) {
	if (op->args == NULL) {
		op->args = (iloc_arg**) malloc(sizeof(iloc_arg));
	} else {
		op->args = (iloc_arg**) realloc(op->args, (op->num_args+1) * sizeof(iloc_arg));
	}
	op->args[op->num_args] = arg;
	op->num_args++;
}

iloc_operation* new_1arg_op(int op_code, iloc_arg* arg1) {
	iloc_operation *op = new_op(op_code);
	add_arg(op, arg1);
	return op;
}

iloc_operation* new_2arg_op(int op_code, iloc_arg* arg1, iloc_arg* arg2) {
	iloc_operation *op = new_op(op_code);
	add_arg(op, arg1);
	add_arg(op, arg2);
	return op;
}

iloc_operation* new_3arg_op(int op_code, iloc_arg* arg1, iloc_arg* arg2, iloc_arg* arg3) {
	iloc_operation *op = new_op(op_code);
	add_arg(op, arg1);
	add_arg(op, arg2);
	add_arg(op, arg3);
	return op;
}

iloc_operation* new_nop() {
	return new_op(NOP);
}

char* new_reg() {
	char* reg = (char*) malloc(32);
	sprintf(reg, "%s%d", "r", reg_count);
	reg_count++;
	return reg;
}

/* Instructions */
iloc_operation* loadi(int value, char* reg) {
	return new_2arg_op(LOADI, new_arg(CONSTANT, (void*) &value), new_arg(REGISTER, reg));
}


/* Free + Print functions */

void add_to_freed_args(iloc_arg* arg) {
	if (num_freed_registers == 0) {
		freed_registers = (iloc_arg**) malloc(sizeof(iloc_arg));
	} else {
		freed_registers = (iloc_arg**) realloc(freed_registers, (num_freed_registers+1) * sizeof(iloc_arg));
	}
	freed_registers[num_freed_registers] = arg;
	num_freed_registers++;
}

int is_arg_freed(iloc_arg* arg) {
	int index;
	for (index = 0; index < num_freed_registers; index++) {
		if (strcmp(freed_registers[index]->arg.str_var, arg->arg.str_var) == 0) {
			return 1;
		}
	}
	return 0;
}

void reag_arg(iloc_arg* arg) {
	if (arg->type == CONSTANT) {
		printf("%i", arg->arg.int_const);
	} else {
		printf("%s", arg->arg.str_var);
	}
}

void read_op(iloc_operation* op) {
	printf("%s ", get_instruction_name(op->op_code));
	if (op->op_code != NOP) {
		switch(get_instruction_type(op->op_code)) {
			
			case ONE_IN_ONE_OUT:
				reag_arg(op->args[0]);
				printf(" => ");
				reag_arg(op->args[1]);
				break;
			
			case ONE_IN_TWO_OUT:
				reag_arg(op->args[0]);
				printf(" => ");
				reag_arg(op->args[1]);
				printf(", ");
				reag_arg(op->args[2]);
				break;
			
			case TWO_IN_ONE_OUT:
				reag_arg(op->args[0]);
				printf(", ");
				reag_arg(op->args[1]);
				printf(" => ");
				reag_arg(op->args[2]);
				break;

			case ONE_OUT:
				printf("=> ");
				reag_arg(op->args[0]);
				break;

			default: break;

		}
	}
	printf("\n");
}

void print_code(iloc_op_list* list) {
	int op_index;
	for (op_index = 0; op_index < list->num_ops; op_index++) {
		if (list->ops[op_index] != NULL) {
			read_op(list->ops[op_index]);	
		}
	}
}

void free_register_list() {
	int index;
	for (index = 0; index < num_freed_registers; index++) {
		if (freed_registers[index]->type != CONSTANT)
			free(freed_registers[index]->arg.str_var);
		free(freed_registers[index]);
	}
	free(freed_registers);
	freed_registers = NULL;
	num_freed_registers = 0;
}

void free_op_list(iloc_op_list* list) {
	if (list != NULL) {
		int op_index;
		for (op_index = 0; op_index < list->num_ops; op_index++) {
			free_op(list->ops[op_index]);
		}
		free(list->ops);
		list->ops = NULL;
		free(list);
		list = NULL;
	}
	free_register_list();
}

void free_op(iloc_operation* op) {
	if (op != NULL) {
		int arg_index;
		for (arg_index = 0; arg_index < op->num_args; arg_index++) {
			set_arg_to_freedom(op->args[arg_index]);
		}
		free(op->args);
		op->args = NULL;
		free(op);
		op = NULL;
	}
}

void set_arg_to_freedom(iloc_arg* argum) {
	if (argum != NULL) {
		if (argum->type != CONSTANT) {
			if (!is_arg_freed(argum)) {
				add_to_freed_args(argum);
			}
		} else {
			free(argum);
		}
		argum = NULL;
	}
}

void free_list_and_ops(iloc_op_list* list) {
	if (list != NULL) {
		free(list->ops);
		list->ops = NULL;
		free(list);
		list = NULL;
	}
}