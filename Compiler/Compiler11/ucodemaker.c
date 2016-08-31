#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include "scanner.h"
#include "minic.tbl"
#include "SymTab.h"

#define PS_SIZE 200

Node *buildNode(struct tokenType token);
Node *buildTree(int nodeNumber,int rhsLength);
void printTree(Node *pt,int indent);
void printNode(Node *pt,int indent);
Node* parser(void);
int meaningfulToken(struct tokenType token);
void UCodeGenerator(Node *pt);
void codeGen(Node *ptr);
void StartGen(Node *pt);
void EndGen(Node *pt);
void processDeclaration(Node*ptr);
void processSimpleVariable(Node *ptr,int typeSpecifier,int typeQualifier);
void processArrayVariable(Node *ptr,int typeSpecifier,int typeQaulifier);
void processFuncHeader(Node *ptr);
void processFunction(Node *ptr);
void processDeclarationInFunc(Node *ptr);
void processArrayVariableInFunc(Node *ptr,int typeSpecifier,int typeQualifier);
void processStatement(Node *ptr);
void processOperator(Node *ptr);
void processCondition(Node *ptr);
int checkPredefined(Node *p);
void emitLabel(char *label);
void emitJump(int instr,char *dest);
void rv_emit(Node *pt);
void emit0(int instr);
void emit1(int instr,int operand1);
void emit2(int instr,int operand1,int operand2);
void emit3(int instr,int operand1,int operand2,int operand3);
void emit_proc(char*functionName,int fun,int procLocalSize,int base,int lexical_level);
void genLabel(char *label);
void errorRecovery(void);
int typeSize(int typeSpecifier);

void printToken(struct tokenType token);
void dumpStack(void);
int processLocalSize(Node *ptr);
int isOperation(Node *ptr);

int sp;
int stateStack[PS_SIZE];
int symbolStack[PS_SIZE];
Node *valueStack[PS_SIZE];
int base=1, offset=1;
int noDclItems=0;
int noGlobalLabel=0;
int lvalue=0;
int stIndex=0;
int indexAvailSymbolTable=0;
int indexLevelBucket=0;
int lastLevel=0;
extern FILE *sourceFile,*astFile,*ucodeFile;

Node *buildNode(struct tokenType token){
	Node *ptr;
	ptr =(Node*)malloc(sizeof(Node));
	if(!ptr){
		printf("malloc error in buildNode()\n");
		exit(1);
	}
	ptr->token = token;
	ptr->noderep = terminal;
	ptr->son = ptr->brother = NULL;
	return ptr;
}
Node *buildTree(int nodeNumber,int rhsLength){
	int i, j, start;
	Node *first,*ptr;

	i = sp - rhsLength +1;
        
	while(i <= sp && valueStack[i]== NULL) i++;
		if(!nodeNumber && i > sp)return NULL;
		start = i;

		while(i <= sp-1){
			j=i+1;
			while(j <= sp && valueStack[j]== NULL) j++;
			if(j <= sp){
				ptr = valueStack[i];
				while(ptr->brother) ptr = ptr->brother;
				ptr->brother = valueStack[j];
			}
			i = j;
		}
		first =(start > sp)? NULL : valueStack[start];

		if(nodeNumber){
			ptr =(Node*) malloc(sizeof(Node));
			if(!ptr){
				printf("malloc error in buildTree()\n"); exit(1);
			}
			ptr->token.number = nodeNumber;
			ptr->token.value.id[0]='\0';
			ptr->noderep = nonterm;
			ptr->son = first;
			ptr->brother = NULL;
			return ptr;
		}
		else
			return first;
}
void printTree(Node *pt,int indent){
	Node *p = pt;
	while(p != NULL){
		printNode(p, indent);
		if(p->noderep == nonterm) printTree(p->son, indent+5);
			p = p->brother;
	}
}
void printNode(Node *pt,int indent){
     int i;

	 for(i=1; i<=indent; i++){
		 fprintf(astFile," ");
		 printf(" ");
	 }
	 if(pt->noderep == terminal){
		 if(pt->token.number == tident){
			 fprintf(astFile," Terminal: %s", pt->token.value.id);
			 printf(" Terminal: %s", pt->token.value.id);
		 }
		 else if(pt->token.number == tnumber){
			 fprintf(astFile," Terminal: %d", pt->token.value.num);
			 printf(" Terminal: %d", pt->token.value.num);
		 }
	 }
	 else{
		 int i;
		 i =(int)(pt->token.number);
	     fprintf(astFile," Nonterminal: %s", nodeName[i]);
		 printf(" Nonterminal: %s", nodeName[i]);
	 }
	 fprintf(astFile,"\n");
	 printf("\n");
}
Node *parser()
{
	extern int parsingTable[NO_STATES][NO_SYMBOLS+1];
	extern int leftSymbol[NO_RULES+1], rightLength[NO_RULES+1];
	int entry, ruleNumber, lhs;
	int currentState;
	struct tokenType token;
	Node *ptr;

	sp=0; stateStack[sp]=0;
	token = scanner();
	while(1){
		currentState = stateStack[sp];
		entry = parsingTable[currentState][token.number];
		if(entry > 0){
			if(++sp > PS_SIZE){
				printf("critical compiler error: parsing stack overflow");
				exit(1);
			}
			symbolStack[sp]= token.number;
			stateStack[sp]= entry;
			valueStack[sp]= meaningfulToken(token)? buildNode(token): NULL;
			token = scanner();
		}
		else if(entry < 0){
			ruleNumber =-entry;
			if(ruleNumber == GOAL_RULE){
				// printf(" *** valid source ***");
				return valueStack[sp-1];
			}
			//semantic(ruleNumber); 
			ptr=buildTree(ruleName[ruleNumber], rightLength[ruleNumber]);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			symbolStack[++sp] = lhs;
			stateStack[sp] = currentState;
			valueStack[sp]= ptr;
		}
		else{
			printf(" === error in source ===\n");
			printf("Current Token : ");
			printToken(token);
			dumpStack();
			errorRecovery();
			token = scanner();
		}
	}
}
int meaningfulToken(struct tokenType token){
	if((token.number == tident)||(token.number == tnumber))
		return 1;
	else return 0;
}
void codeGen(Node *ptr){
	Node *p;
	int globalSize;

	initSymbolTable();
	for(p=ptr->son; p; p=p->brother){
		if(p->token.number == DCL) processDeclaration(p->son);
		else if(p->token.number == FUNC_DEF) processFuncHeader(p->son);
		else printf("icg error in step1: process the declaration part\n");
	}

	globalSize = offset -1;

	for(p=ptr->son; p; p=p->brother){
		if(p->token.number == FUNC_DEF) processFunction(p);
	}
	emit1(bgn, globalSize);
	emit0(ldp);
	emitJump(call,"main");
	emit0(end);
}
void StartGen(Node *pt){
	int i;
	Node *p = pt;
	int nSymCount=1;
	char szSym[80];

	for(i=0;i<nSymCount;i++){
		sprintf(szSym,"		sym		%d 1 0",i+1);
		printf("%d\n",szSym);
	}
	sprintf(szSym,"		bgn		%d",nSymCount);
	printf("%d\n",szSym);

	printf("	ldc		0\n");
	printf("	str		1	1\n");
}
void EndGen(Node *pt){
	printf("		str		1	1\n");
	printf("		ldp\n");
	printf("		lod		1	1\n");
	printf("		call		write\n");
	printf("		endop\n");
}
void processDeclaration(Node *ptr){
	int typeSpecifier, typeQualifier;
	Node *p,*q;

	if(ptr->token.number != DCL_SPEC) printf("icg error in processDCL");

	typeSpecifier = INT_TYPE;
	typeQualifier = VAR_TYPE;
	p = ptr->son;
	while(p){
		if(p->token.number == INT_NODE) typeSpecifier = INT_TYPE;
		else if(p->token.number == CONST_NODE) typeQualifier = CONST_NODE;
		else{
			printf("not yet implemented\n");
			return;
		}
		p = p->brother;
	}

	p = ptr->brother;
	if(p->token.number != DCL_ITEM) printf("icg error in process DCL_ITEM");

	while(p){
		q = p->son;
		switch(q->token.number){
		case SIMPLE_VAR:
			processSimpleVariable(q, typeSpecifier, typeQualifier);
			break;
		case ARRAY_VAR:
			processArrayVariable(q, typeSpecifier, typeQualifier);
			break;
		default: printf("error in SIMPLE_VAR or ARRAY_VAR\n");
			break;
		}
		p = p->brother;
	}
}
void processSimpleVariable(Node *ptr,int typeSpecifier,int typeQualifier){
	Node *p = ptr->son;
	Node *q = ptr->brother;
	int stIndex, size, initialValue;
	int sign =1;

	if(ptr->token.number != SIMPLE_VAR) printf("error in SIMPLE_VAR");

	if(typeQualifier == CONST_NODE){
		if(q == NULL){
			printf("%s must have a constant value\n", ptr->son->token.value.id);
			return;
		}
		if(q->token.number == UNARY_MINUS){
			sign =-1;
			q = q->son;
		}
		initialValue = sign * q->token.value.num;
		stIndex = insert(p->token.value.id, typeSpecifier, typeQualifier,
			0,0,0,initialValue);
	}
	else{
		size = typeSize(typeSpecifier);
		stIndex = insert(p->token.value.id, typeSpecifier, typeQualifier,
			base, offset, size,0);
		emit3(sym, base, offset, size);
		offset += size;

	}
}
void processArrayVariable(Node *ptr,int typeSpecifier,int typeQualifier){
	Node *p = ptr->son;
	int stIndex, size;

	if(ptr->token.number != ARRAY_VAR){
		printf("error in ARRAY_VAR");
		return;
	}
	if(p->brother == NULL)
		printf("array size must be specified\n");
	else size = p->brother->token.value.num;

	size *= typeSize(typeSpecifier);
	stIndex = insert(p->token.value.id, typeSpecifier, typeQualifier,
		base, offset, size,0);
	emit3(sym, base, offset, size);
	offset += size;
}
void processFuncHeader(Node *ptr){
	int noArguments, returnType;
	int stIndex;
	Node *p;

	if(ptr->token.number != FUNC_HEAD)
		printf("error in processFuncHeader()\n");

	p = ptr->son->son;
	while(p){
		if(p->token.number == INT_NODE) returnType = INT_NODE;
		else if(p->token.number == VOID_NODE) returnType = VOID_NODE;
		else printf("invalid function return type\n");
		p = p->brother;
	}

	p = ptr->son->brother->brother;
	p = p->son;

	noArguments =0;
	while(p){
		noArguments++;
		p = p->brother;
	}

	stIndex = insert(ptr->son->brother->token.value.id, returnType, FUNC_TYPE,1,0, noArguments,0);
}
void processFunction(Node *ptr){
	Node *p = ptr->son->son->brother;
	char *functionName;
	int stIndex, noArguments, procLocalSize;
	int lexical_level=2;
	offset=1;
	base=2;

	functionName = p->token.value.id;
	stIndex = lookup(functionName);
	if(stIndex ==-1){
		printf("undefined function !!\n");
		return;
	}
	noArguments = symbolTable[stIndex].width;

	if( ptr->son->brother->son->son )
		procLocalSize = noArguments + processLocalSize(ptr->son->brother->son->son);
	else procLocalSize = noArguments;

	emit_proc(functionName,fun, procLocalSize, base, lexical_level);

	for(p = ptr->son->son->brother->brother->son; p; p = p->brother){
		if(p->token.number == PARAM_DCL) processDeclarationInFunc(p->son);
		else printf("icg error in processFunction(): processing on a Funcdecl part\n");
	}

	for(p = ptr->son->brother->son->son; p; p = p->brother)
		processDeclaration(p->son);
	processStatement(ptr->son->brother);
        
	if( symbolTable[stIndex].typeSpecifier == VOID_NODE ){
		emit0(ret);
		emit0(end);
	}
	else emit0(end);
        
	return;
}
void processDeclarationInFunc(Node *ptr){
	int typeSpecifier, typeQualifier;
	Node*p;

	if(ptr->token.number != DCL_SPEC) printf("icg error in processDCL");

	typeSpecifier = INT_TYPE;
	typeQualifier = VAR_TYPE;
	p = ptr->son;
	while(p){
		if(p->token.number == INT_NODE) typeSpecifier = INT_TYPE;
		else if(p->token.number == CONST_NODE) typeQualifier = CONST_TYPE;
		else{
			printf("not yet implemented\n");
			return;
		}
		p = p->brother;
	}

	p = ptr->brother;

	switch(p->token.number){
		case SIMPLE_VAR:
			processSimpleVariable(p, typeSpecifier, typeQualifier);
                break;
		case ARRAY_VAR:
			processArrayVariableInFunc(p, typeSpecifier, typeQualifier);
			break;
		default: printf("error in SIMPLE_VAR or ARRAY_VAR in processDeclarationInFunc\n");
			break;
	}
}
void processArrayVariableInFunc(Node *ptr,int typeSpecifier,int typeQualifier){
	Node *p = ptr->son;
	int stIndex, size;

	if(ptr->token.number != ARRAY_VAR){
		printf("error in ARRAY_VAR");
		return;
	}
	if(p->brother != NULL)
		printf("array size can not be specified in PARAM_DCL\n");
	else size = typeSize(typeSpecifier);

	stIndex = insert(p->token.value.id, typeSpecifier, typeQualifier,
		base, offset, size, 0);
	emit3(sym, base, offset, size);

	offset += size;
}
void processStatement(Node *ptr)
{
	Node *p;
	switch(ptr->token.number){
		case COMPOUND_ST:
			p = ptr->son->brother;
			p = p->son;
			while(p){
				processStatement(p);
				p = p->brother;
			}
			break;
		case EXP_ST:
			if(ptr->son != NULL) processOperator(ptr->son);
			break;
		case RETURN_ST:
			if(ptr->son != NULL){
				p = ptr->son;
				if(p->noderep == nonterm)
					processOperator(p);
				else rv_emit(p);
				emit0(retv);
			}
			else emit0(ret);
			break;
		case IF_ST:	{
			char label[LABEL_SIZE];

			genLabel(label);
			processCondition(ptr->son);
			emitJump(fjp, label);
			processStatement(ptr->son->brother);
			emitLabel(label);
			}
			break;
		case IF_ELSE_ST: {
			char label1[LABEL_SIZE], label2[LABEL_SIZE];

			genLabel(label1); genLabel(label2);
			processCondition(ptr->son);
			emitJump(fjp, label1);
			processStatement(ptr->son->brother);
			emitJump(fjp, label2);
			emitLabel(label1);
			processStatement(ptr->son->brother->brother);
			emitLabel(label2);
			}
			break;
		case WHILE_ST: {
			char label1[LABEL_SIZE], label2[LABEL_SIZE];

			genLabel(label1); genLabel(label2);
			emitLabel(label1);
			processCondition(ptr->son);
			emitJump(fjp, label2);
			processStatement(ptr->son->brother);
			emitJump(ujp, label1);
			emitLabel(label2);
			}
			break;
		default:
			printf("not yet implemented.\n");
			break;
	}
}
void processOperator(Node *ptr){
	switch(ptr->token.number){
	case ASSIGN_OP:{
		Node *lhs = ptr->son,*rhs = ptr->son->brother;

		if(lhs->noderep == nonterm){
			lvalue =1;
			processOperator(lhs);
			lvalue =0;
		}

		if(rhs->noderep == nonterm) processOperator(rhs);
		else rv_emit(rhs);

		if(lhs->noderep == terminal){
			stIndex = lookup(lhs->token.value.id);
			if( stIndex ==-1){
				printf("undefined variable : %s\n", lhs->token.value.id);
				return;
			}
			emit2(str, symbolTable[stIndex].base, symbolTable[stIndex].offset);
		}
		else emit0(sti);
		break;
	}

	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
	case DIV_ASSIGN: case MOD_ASSIGN: {
		Node *lhs = ptr->son,*rhs = ptr->son->brother;
		int nodeNumber = ptr->token.number;
		ptr->token.number = ASSIGN_OP;
               
		if(lhs->noderep == nonterm){
			lvalue=1;
			processOperator(lhs);
			lvalue=0;
		}
		ptr->token.number = nodeNumber;

		if(lhs->noderep == nonterm)
			processOperator(lhs);
		else rv_emit(lhs);

		if(rhs->noderep == nonterm)
			processOperator(rhs);
		else rv_emit(rhs);

		switch(ptr->token.number){
			case ADD_ASSIGN: emit0(add); break;
			case SUB_ASSIGN: emit0(sub); break;
			case MUL_ASSIGN: emit0(mult); break;
			case DIV_ASSIGN: emit0(divi);break;
			case MOD_ASSIGN: emit0(mod);break;
		}
		if(lhs->noderep == terminal){
			stIndex = lookup(lhs->token.value.id);
			if(stIndex ==-1){
				printf("undefined variable : %s\n", lhs->token.value.id);
				return;
			}
			emit2(str, symbolTable[stIndex].base, symbolTable[stIndex].offset);
		}
		else emit0(sti);
		break;
	}

	case ADD: case SUB: case MUL: case DIV: case MOD:
	case EQ: case NE: case GT: case LT: case GE: case LE:
	case LOGICAL_AND: case LOGICAL_OR: {
		Node *lhs = ptr->son,*rhs = ptr->son->brother;

		if(lhs->noderep == nonterm) processOperator(lhs);
		else rv_emit(lhs);
              
		if(rhs->noderep == nonterm) processOperator(rhs);
		else rv_emit(rhs);
                        
		switch(ptr->token.number){
			case ADD: emit0(add); break;
			case SUB: emit0(sub); break;
			case MUL: emit0(mult); break;
			case DIV: emit0(divi); break;
			case MOD: emit0(mod); break;
			case EQ: emit0(eq); break;
			case NE: emit0(ne); break;
			case GT: emit0(gt); break;
			case LT: emit0(lt); break;
			case GE: emit0(ge); break;
			case LE: emit0(le); break;
			case LOGICAL_AND: emit0(and); break;
			case LOGICAL_OR: emit0(or); break;
		}
		break;
	}
	case UNARY_MINUS: case LOGICAL_NOT: {
		Node *p = ptr->son;

		if(p->noderep == nonterm) processOperator(p);
		else rv_emit(p);
		switch(ptr->token.number){
		case UNARY_MINUS: emit0(ne);
			break;
		case LOGICAL_NOT: emit0(notop);
			break;
		}
		break;
	}
	case INDEX: {
		Node *indexExp = ptr->son->brother;

		if(indexExp->noderep == nonterm) processOperator(indexExp);
		else rv_emit(indexExp);
		stIndex = lookup(ptr->son->token.value.id);
		if(stIndex ==-1){
			printf("undefined variable : %s\n", ptr->son->token.value.id);
			return;
		}
		emit2(lda, symbolTable[stIndex].base, symbolTable[stIndex].offset);
		emit0(add);
		if(!lvalue) emit0(ldi);
		break;
	}
	case PRE_INC: case PRE_DEC: case POST_INC: case POST_DEC: {
		Node *p = ptr->son; Node *q;
		int stIndex; int amount=1;
		if(p->noderep == nonterm) processOperator(p);
		else rv_emit(p);

		q = p;
		while(q->noderep != terminal) q = q->son;
		if(!q ||(q->token.number != tident)){
			printf("increment/decrement operators can not be"
				"applied in expression\n");
			return;
		}
		stIndex = lookup(q->token.value.id);
		if(stIndex ==-1)return;

		switch(ptr->token.number){
			case PRE_INC: emit0(inc);
				break;
			case PRE_DEC: emit0(dec);
				break;
			case POST_INC:
				emit0(inc);
				break;
			case POST_DEC:
				emit0(dec);
				break;
		}
		if(p->noderep == terminal){
			stIndex = lookup(p->token.value.id);
			if(stIndex ==-1)return;
			emit2(str, symbolTable[stIndex].base, symbolTable[stIndex].offset);
		}
		else if(p->token.number == INDEX){
			lvalue=1;
			processOperator(p);
			lvalue=0;
			emit0(swp);
			emit0(sti);
		}
		else printf("error in increment/decrement operators\n");
		break;
	}
	case CALL: {
		Node *p = ptr->son;
		char *functionName;
		int stIndex; int noArguments;

		if(checkPredefined(p)){
			break;
			functionName = p->token.value.id;
			stIndex = lookup(functionName);
			if(stIndex ==-1) break;
			noArguments = symbolTable[stIndex].width;
		}

		emit0(ldp);
		p = p->brother->son;
		while(p){
			if(p->noderep == nonterm) processOperator(p);
			else rv_emit(p);
			p = p->brother;
		}
		emitJump(call, ptr->son->token.value.id);
		break;
		}                               
	}
}
void processCondition(Node *ptr){
	if(ptr->noderep == nonterm) processOperator(ptr);
	else rv_emit(ptr);
}
int checkPredefined(Node *p){
	
	char *functionName;
	int noArguments;
	int stIndex;

	functionName = p->token.value.id;

	if(!strcmp("read",p->token.value.id)){
	
		emit0(ldp);
		p = p->brother->son;
		while(p){
			if(p->noderep == terminal){
				stIndex = lookup(p->token.value.id);
				emit2(lda, symbolTable[stIndex].base, symbolTable[stIndex].offset);
			}
			p = p->brother;
		}
		emitJump(call, functionName);
		return 1;
	}
	else if(strcmp(functionName, "write") == 0){
		emit0(ldp);
		p = p->brother->son;
		while(p){
			if(p->noderep == nonterm)
				processOperator(p);
			else {
				stIndex = lookup(p->token.value.id);
				if(stIndex == -1) return;
				emit2(lod, symbolTable[stIndex].base, symbolTable[stIndex].offset);
			}
			p=p->brother;
		}
		emitJump(call, functionName);
		return 1;
	}
	else 
		return 0;
}
void emitLabel(char*label){
	fprintf(ucodeFile,"%-10s %s\n", label, instrName[nop]);
	printf("%-10s %s\n", label, instrName[nop]);
}
void emitJump(int instr,char*dest){
	fprintf(ucodeFile,"%10s %s\t%s\n","", instrName[instr], dest);
	printf("%10s %s\t%s\n","", instrName[instr], dest);
}
void rv_emit(Node *pt){
	int stIndex;

	if(pt->token.number == tnumber)			// number
	{
		emit1(ldc, pt->token.value.num);
	}
	else											// identifier
	{
		stIndex = lookup(pt->token.value.id);
		if(stIndex == -1)
		{
			return;
		}
		if(symbolTable[stIndex].typeQualifier == CONST_NODE)	// constant
		{
			emit1(ldc, symbolTable[stIndex].initialValue);
		}
		else if(symbolTable[stIndex].width > 1)				// array var
		{
			emit2(lda, symbolTable[stIndex].base, symbolTable[stIndex].offset);
		}
		else										// simple var
		{
			emit2(lod, symbolTable[stIndex].base, symbolTable[stIndex].offset);
		}
	}
}
void emit0(int instr){
	fprintf(ucodeFile,"%10s %s\n","", instrName[instr]);
	printf("%10s %s\n","", instrName[instr]);
}
void emit1(int instr,int operand1){
	fprintf(ucodeFile,"%10s %s\t%d\n","", instrName[instr], operand1);
	printf("%10s %s\t%d\n","", instrName[instr], operand1);
}
void emit2(int instr,int operand1,int operand2){
	fprintf(ucodeFile,"%10s %s\t%-10d %d\n","", instrName[instr], operand1, operand2);
	printf("%10s %s\t%-10d %d\n","", instrName[instr], operand1, operand2);
}
void emit3(int instr,int operand1,int operand2,int operand3){
	fprintf(ucodeFile,"%10s %s\t%-10d %-10d %d\n","", instrName[instr], operand1, operand2, operand3);
	printf("%10s %s\t%-10d %-10d %d\n","", instrName[instr], operand1, operand2, operand3);
}
void emit_proc(char *functionName,int instr,int operand1,int operand2,int operand3){
	fprintf(ucodeFile,"%-10s %s\t%-10d %-10d %d\n", functionName, instrName[instr], operand1, operand2, operand3);
	printf("%-10s %s\t%-10d %-10d %d\n", functionName, instrName[instr], operand1, operand2, operand3);
}
void genLabel(char label[]){
	sprintf(label,"$$%d\0", noGlobalLabel);
	noGlobalLabel++;
}
void errorRecovery(){
	struct tokenType tok;
	int parenthesisCount, braceCount;
	int i;

	parenthesisCount = braceCount = 0;
	while(1){
		tok = scanner();
		if(tok.number == teof) exit(1);
		if(tok.number == tlparen) parenthesisCount++;
		else if(tok.number == trparen) parenthesisCount--;
		if(tok.number == tlbrace) braceCount++;
		else if(tok.number == trbrace) braceCount--;
		if((tok.number==tsemicolon)&&(parenthesisCount<=0)&&(braceCount<=0))
			break;
    }

	for(i=sp; i>=0; i--){
		if(stateStack[i]==36) break;
		if(stateStack[i]==24) break;
		if(stateStack[i]==25) break;
		if(stateStack[i]==17) break;
		if(stateStack[i]==2) break;
		if(stateStack[i]==0) break;
	}
	sp = i;
}
int typeSize(int typeSpecifier){
	if(typeSpecifier == INT_TYPE) return 1;
	else{
		printf(" not define typeSpecifier in typeSize()");
		exit(0);
	}
	return 0;
}
void printToken(struct tokenType token){
	if(token.number == tident)
		printf("%s", token.value.id);
	else if(token.number == tnumber)
		printf("%d", token.value.num);
	else
		printf("%s", tokenName[token.number]);
}
void dumpStack(){
	int i, start;

	if(sp>10) start = sp-10;
	else start=0;

	printf("\n *** dump state stack :");
	for(i=start; i <= sp; i++)
		printf(" %d", stateStack[i]);

	printf("\n *** dump symbol stack :");
	for(i=start; i <= sp; i++)
		printf(" %d ", symbolStack[i]);

	printf("\n *** dump value stack :");
	for(i=start; i <= sp; i++)
		printf(" %x", valueStack[i]);
	printf("\n");
}
int processLocalSize(Node *ptr){
	Node*p,*q;
	noDclItems=0;
	if(ptr->token.number == DCL)
		for(p = ptr; p; p = p->brother){
			for(q = p->son; q; q = q->brother){
				if(q->token.number == DCL_ITEM){
					if( q->son->token.number == SIMPLE_VAR ){
						if( ptr->son->son->token.number == INT_NODE )
							noDclItems += typeSize(INT_TYPE);
					}
					else{
						if( ptr->son->son->token.number == INT_NODE )
							noDclItems += typeSize(INT_TYPE)* q->son->son->brother->token.value.num;
					}
				}
			}
		}
	return noDclItems;
}
int isOperation(Node *ptr){
	if(ptr->brother)
		return 1;
	else return 0;
}
void UCodeGenerator(Node *pt){
	Node *p = pt;

	StartGen(p);
	codeGen(p);
	EndGen(p);	
}
void main(int argc,char *argv[])
{
    Node *root;
	char fileName[30];
	printf("***start of Mini C Compiler\n");
	
	if(argc!=2){
		printf("Argument is not enough\n");
		//icg_error(1);
		exit(1);
	}
	strcpy(fileName,argv[1]);
	printf("*source file name: %s\n",fileName);

	if((sourceFile=fopen(fileName,"r"))==NULL){
		//icg_error(2);
		exit(1);
	}
	astFile=fopen(strcat(strtok(fileName,"."),".ast"),"w");
	ucodeFile=fopen(strcat(strtok(fileName,"."),".uco"),"w");

	printf("===start of parser\n");
	root=parser();
	printTree(root,0);
//	UCodeGenerator(root); 
	printf("===start of ICG\n");
	codeGen(root);
	printf("*** end of Mini C Compiler\n");
	fclose(astFile);
	fclose(sourceFile); 
     
}
