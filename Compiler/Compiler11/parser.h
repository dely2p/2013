#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"
#include "minic.tbl"

#define PS_SIZE 200 

enum nodeNumber{

	ACTUAL_PARAM,	ADD,		ADD_ASSIGN,	ARRAY_VAR,	ASSIGN_OP,
	CALL,		COMPOUND_ST,	CONST_NODE,	DCL,		DCL_ITEM,
	DCL_LIST,	DCL_SPEC,	DIV,		DIV_ASSIGN,	EQ,
	ERROR_NODE,	EXP_ST,		FORMAL_PARA,	FUNC_DEF,	FUNC_HEAD,
	GE,		GT,		IDENT,		IF_ELSE_ST,	IF_ST,
	INDEX,		INT_NODE,	LE,		LOGICAL_AND,	LOGICAL_NOT,
	LOGICAL_OR,	LT,		MOD,		MOD_ASSIGN,	MUL,
	MUL_ASSIGN,	NE,		NUMBER,		PARAM_DCL,	POST_DEC,
	POST_INC,	PRE_DEC,	PRE_INC,	PROGRAM,		RETURN_ST,
	SIMPLE_VAR,	STAT_LIST,	SUB,		SUB_ASSIGN,	UNARY_MINUS,
	VOID_NODE,	WHILE_ST
};

char * nodeName[] = {
	"ACTUAL_PARAM", "ADD",		"ADD_ASSIGN",	"ARRAY_VAR",	"ASSIGN_OP",
	"CALL",		"COMPUND_ST",	"CONST_NODE",	"DCL",		"DCL_ITEM", 
	"DCL_LIST",	"DCL_SPEC",	"DIV",		"DIV_ASSIGN",	"EQ",
	"ERROR_NODE",	"EXP_ST",	"FORMAL_PARA",	"FUNC_DEF",	"FUNC_HEAD",
	"GE",		"GT",		"IDENT",		"IF_ELSE_ST",	"IF_ST", 
	"INDEX",		"INT_NODE",	"LE",		"LOGICAL_AND", "LOGICAL_NOT",
	"LOGICAL_OR",	"LT",		"MOD",		"MOD_ASSIGN",	"MUL",
	"MUL_ASSIGN",	"NE",		"NUMBER",	"ARAM_DCL",	"POST_DEC", 
	"POST_INC",	"PRE_DEC",	"PRE_INC",	"PROGRAM",	"RETURN_ST", 
	"SIMPLE_VAR",	"STAT_LIST",	"SUB",           "SUB_ASSIGN",   "UNARY_MINUS",
	"VOID_NODE",	"WHILE_ST"
};

int ruleName[] = {
	0,		PROGRAM,		0, 		0, 		0,
	0, 		FUNC_DEF, 	FUNC_HEAD, 	DCL_SPEC, 	0,
	0, 		0, 		0, 		CONST_NODE, 	INT_NODE,
	VOID_NODE, 	0, 		FORMAL_PARA, 	0, 		0, 
	0, 		0, 		PARAM_DCL, 	COMPOUND_ST, 	DCL_LIST,
	DCL_LIST, 	0, 		0, 		DCL, 		0,
	0, 		DCL_ITEM, 	DCL_ITEM, 	SIMPLE_VAR, 	ARRAY_VAR,
	0, 		0, 		STAT_LIST, 	0, 		0, 
	0, 		0, 		0, 		0, 		0, 
	0, 		EXP_ST, 		0, 		0, 		IF_ST,
	IF_ELSE_ST, 	WHILE_ST, 	RETURN_ST, 	0, 		0, 
	ASSIGN_OP, 	ADD_ASSIGN, 	SUB_ASSIGN, 	MUL_ASSIGN, 	DIV_ASSIGN,
	MOD_ASSIGN, 	0, 		LOGICAL_OR, 	0, 		LOGICAL_AND,
	0, 		EQ, 		NE, 		0, 		GT,
	LT, 		GE, 		LE, 		0, 		ADD, 
	SUB, 		0, 		MUL, 		DIV, 		MOD,
	0, 		UNARY_MINUS, 	LOGICAL_NOT, 	PRE_INC, 	PRE_DEC,
	0, 		INDEX, 		CALL, 		POST_INC, 	POST_DEC,
	0, 		0, 		ACTUAL_PARAM, 	0, 		0, 
	0, 		0, 		0
};


typedef struct nodeType{
	struct tokenType token;
	enum {terminal, nonterm} noderep;
	struct nodeType *son;
	struct nodeType *brother;
}Node;


Node *buildNode(struct tokenType token);
Node *buildTree(int nodeNumber, int rhsLength);
void printNode(Node *pt, int indent);
void printTree(Node *pt, int indent);
Node *parser();
int meaningfulToken(struct tokenType token);

int symbolStack[PS_SIZE];
Node *valueStack[PS_SIZE];
int sp, stateStack[PS_SIZE];

Node *parser(){
	extern int parsingTable[NO_STATES][NO_SYMBOLS+1];
	extern int leftSymbol[NO_RULES+1], rightLength[NO_RULES+1];

	int entry, ruleNumber, lhs;
	int currentstate;
	struct tokenType token;
	Node *ptr;

	sp=0; stateStack[sp]=0;
	token = scanner();
	
	while (1){
		currentstate = stateStack[sp];
		entry = parsingTable[currentstate][token.number];
		
		if ( entry > 0 ){ 
			if( ++sp > PS_SIZE ){
				printf("critical compiler error	: parsing stack overflow");
				exit(1);
			}

		symbolStack[sp] = token.number;
		stateStack[sp] = entry;
		valueStack[sp] = meaningfulToken(token) ? buildNode(token) : NULL;
		token = scanner();
	}
		else if ( entry < 0 ){
			ruleNumber = -entry;
			if (ruleNumber == GOAL_RULE){ 
				return valueStack[sp-1];
			}	

		ptr = buildTree(ruleName[ruleNumber], rightLength[ruleNumber]);
		sp = sp - rightLength[ruleNumber];
		lhs = leftSymbol[ruleNumber];
		currentstate = parsingTable[stateStack[sp]][lhs];
		symbolStack[++sp] = lhs;
		stateStack[sp] = currentstate;
		valueStack[sp] = ptr;
	}
	else{
		printf("=== error in source ===\n");
		exit(1);
	}
}
}
int meaningfulToken(struct tokenType token){
	if ( token.number == tident || token.number == tnumber ) 
		return 1;
	else 	return 0;
}

void printNode(Node *t, int indent){ 
	int i;

	for (i=1; i<=indent; i++) 	printf(" ");
	if (t->noderep == terminal){
		if(t->token.number == tident)
			printf("Terminal : %s", t->token.value.id);
		else if (t->token.number = tnumber)
			printf("Terminal : %d", t->token.value.num);
	}
	else{
		i = (int) (t->token.number);
		printf(" Nonterminal: %s", nodeName[i]);
	}
	printf("\n");
}


void printTree(Node *t, int indent){ 
	Node * p = t;
	while (p != NULL){
		printNode(p, indent);	
		if (p->noderep == nonterm) printTree(p->son,indent+3);
		p = p->brother;
	}
}

Node *buildNode(struct tokenType token){ 
	Node *ptr;
	
	ptr = (Node *) malloc (sizeof(Node));
	if (!ptr){ 
		printf(" build node malloc error\n");
		exit(1);
	}
	ptr -> token = token;
	ptr -> noderep = terminal;
	ptr -> son = ptr -> brother = NULL;
	
	return ptr;
}

Node *buildTree(int nodeNumber, int rhsLength){
	int i, j, start;
	Node *first, *ptr;

	i = sp - rhsLength + 1;
	
	while( i <= sp && valueStack[i] == NULL )	i++;
	if( !nodeNumber && i > sp ) return NULL;
	start = i;
	while( i <= sp - 1 ){
		j = i + 1;
		while( j <= sp && valueStack[j] == NULL )	j++;
		if ( j <= sp ){
			ptr = valueStack[i];
			while( ptr->brother ) ptr  = ptr->brother;
			ptr->brother = valueStack[j];
		}
		i = j;
	}

	first = (start > sp) ? NULL : valueStack[start];
	if(nodeNumber){
		ptr = (Node *) malloc(sizeof(Node));
		if( !ptr ){
			printf("malloc error in buildTree()\n");
			exit(1);
		}

		ptr->token.number = nodeNumber;
		ptr->token.tokenValue = NULL;
		ptr->noderep = nonterm;
		ptr->son = first;
		ptr->brother = NULL;
		return ptr;
	}
	else return first;
}
void main(){
	Node * root;
	root= parser();
	printTree(root,5);
}