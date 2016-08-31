
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


#define NO_KEYWORDS 7
#define ID_LENGTH 12
#define PS_SIZE 200
FILE *sourceFile, *astFile, *ucodeFile; 
void lexicalError(int n);
int getIntNum(char firstCharacter);
int superLetterOrDigit(char ch);
int hexValue(char ch);

typedef struct _sybolStruct{
        char name[ID_LENGTH];
        char typeSpecifier;
        char typeQualifier;
        char base;
        int offset;
        int width;
        int initialValue;
        int brother;
} symbolStruct;
enum tsymbol { tnull=-1,
        tnot,   tnotequ,        tmod,           tmodAssign,     tident,         tnumber,
        tand,   tlparen,        trparen,        tmul,           tmulAssign,     tplus,
        tinc,   taddAssign,     tcomma,         tminus,         tdec,           tsubAssign,
        tdiv,   tdivAssign,     tsemicolon,     tless,          tlesse,         tassign,
        tequal, tgreat,         tgreate,        tlbracket,      trbracket,      teof,
        tconst, telse,          tif,            tint,           treturn,        tvoid,
        twhile, tlbrace,        tor,            trbrace
};
enum tsymbol tnum[NO_KEYWORDS]={
        tconst, telse, tif, tint, treturn, tvoid, twhile
};
char *keyword[NO_KEYWORDS]={
        "const","else","if","int","return","void","while"
};
char *tokenName[]={
        "!",    "!=",           "%",            "%=",           "%ident",       "%number",
        "&&",   "(",            ")",            "*",            "*=",           "+",
        "++",   "+=",           ",",            "-",            "--",           "-=",
        "/",    "/=",           ";",            "<",            "<=",           "=",
        "==",   ">",            ">=",           "[",            "]",            "eof",
        "const","else",         "if",           "int",          "return",       "void",
        "while","{",            "||",           "}"
};
enum instNumber {
        notop,                neg,                  inc,                  dec,                  dep,
        dup,                  add,                  sub,                  mult,                 divi,
        mod,                  swp,                  and,                  or,                   gt,
        lt,                   ge,                   le,                   eq,                   ne,
        lod,                  str,                  ldc,                  lda,                  ujp,
        tjp,                  fjp,                  chkh,                 chkl,                 ldi,
        sti,                  call,                 ret,                  retv,                 ldp,
        fun,                 end,                  nop,                  bgn,                  sym,
        dump
};
char*instrName[]={
        "notop",                "neg",                  "inc",                  "dec",                  "dep",
        "dup",                  "add",                  "sub",                  "mult",                 "div",
        "mod",                  "swp",                  "and",                  "or",                   "gt",
        "lt",                   "ge",                   "le",                   "eq",                   "ne",
        "lod",                  "str",                  "ldc",                  "lda",                  "ujp",
        "tjp",                  "fjp",                  "chkh",                 "chkl",                 "ldi",
        "sti",                  "call",                 "ret",                  "retv",                 "ldp",
        "fun",                 "end",                  "nop",                  "bgn",                  "sym",
        "dump"
};
enum nodeNumber { STARTER,
        ACTUAL_PARAM,   ADD,                    ADD_ASSIGN,             ARRAY_VAR,              ASSIGN_OP,
        CALL,                   COMPOUND_ST,    CONST_NODE,             DCL,                    DCL_ITEM,
        DCL_LIST,               DCL_SPEC,               DIV,                    DIV_ASSIGN,             EQ,
        ERROR_NODE,             EXP_ST,                 FORMAL_PARA,    FUNC_DEF,               FUNC_HEAD,
        GE,                             GT,                             IDENT,                  IF_ELSE_ST,             IF_ST,
        INDEX,                  INT_NODE,               LE,                             LOGICAL_AND,    LOGICAL_NOT,
        LOGICAL_OR,             LT,                             MOD,                    MOD_ASSIGN,             MUL,
        MUL_ASSIGN,             NE,                             NUMBER,                 PARAM_DCL,              POST_DEC,
        POST_INC,               PRE_DEC,                PRE_INC,                PROGRAM,                RETURN_ST,
        SIMPLE_VAR,             STAT_LIST,              SUB,                    SUB_ASSIGN,             UNARY_MINUS,
        VOID_NODE,              WHILE_ST
};
char *nodeName[]={"STARTER",
        "ACTUAL_PARAM","ADD",                  "ADD_ASSIGN",   "ARRAY_VAR",    "ASSIGN_OP",
        "CALL",                 "COMPOUND_ST",  "CONST_NODE",   "DCL",                  "DCL_ITEM",
        "DCL_LIST",             "DCL_SPEC",             "DIV",                  "DIV_ASSIGN",   "EQ",
        "ERROR_NODE",   "EXP_ST",               "FORMAL_PARA",  "FUNC_DEF",             "FUNC_HEAD",
        "GE",                   "GT",                   "IDENT",                "IF_ELSE_ST",   "IF_ST",
        "INDEX",                "INT_NODE",             "LE",                   "LOGICAL_AND",  "LOGICAL_NOT",
        "LOGICAL_OR",   "LT",                   "MOD",                  "MOD_ASSIGN",   "MUL",
        "MUL_ASSIGN",   "NE",                   "NUMBER",               "PARAM_DCL",    "POST_DEC",
        "POST_INC",             "PRE_DEC",              "PRE_INC",              "PROGRAM",              "RETURN_ST",
        "SIMPLE_VAR",   "STAT_LIST",    "SUB",                  "SUB_ASSIGN",   "UNARY_MINUS",
        "VOID_NODE",    "WHILE_ST"
};
int ruleName[]={
        0,                      PROGRAM,                0,                              0,                              0,
        0,                      FUNC_DEF,               FUNC_HEAD,              DCL_SPEC,               0,
        0,                      0,                              0,                              CONST_NODE,             INT_NODE,
        VOID_NODE,      0,                              FORMAL_PARA,    0,                              0,
        0,                      0,                              PARAM_DCL,              COMPOUND_ST,    DCL_LIST,
        DCL_LIST,       0,                              0,                              DCL,                    0,
        0,                      DCL_ITEM,               DCL_ITEM,               SIMPLE_VAR,             ARRAY_VAR,
        0,                      0,                              STAT_LIST,              0,                              0,
        0,                      0,                              0,                              0,                              0,
        0,                      EXP_ST,                 0,                              0,                              IF_ST,
        IF_ELSE_ST,     WHILE_ST,               RETURN_ST,              0,                              0,
        ASSIGN_OP,      ADD_ASSIGN,             SUB_ASSIGN,             MUL_ASSIGN,             DIV_ASSIGN,
        MOD_ASSIGN,     0,                              LOGICAL_OR,             0,                              LOGICAL_AND,
        0,                      EQ,                             NE,                             0,                              GT,
        LT,                     GE,                             LE,                             0,                              ADD,
        SUB,            0,                              MUL,                    DIV,                    MOD,
        0,                      UNARY_MINUS,    LOGICAL_NOT,    PRE_INC,                PRE_DEC,
        0,                      INDEX,                  CALL,                   POST_INC,               POST_DEC,
        0,                      0,                              ACTUAL_PARAM,   0,                              0,
        0,                      0,                              0
};
void lexicalError(int n){
	printf(" *** Lexical Error : ");
	switch(n) {
		case 1 : printf("an identifier length must be less than 12.\n");	break;
		case 2 : printf("next character must be &.\n");						break;
		case 3 : printf("next character must be |.\n");						break;
		case 4 : printf("invalid character!!!\n");							break;
	}
}
int superLetter(char ch)
{
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}
int superLetterOrDigit(char ch){
	if(isalnum(ch) || ch == '_') return 1;
	else return 0;
}
int hexValue(char ch){
	switch(ch){
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9': 
				return(ch - '0');
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
				return(ch - 'A' + 10);
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': 
				return(ch - 'a' + 10);
		default: return - 1;
	}
}
int getIntNum(char firstCharacter){
	int num = 0;
	int value;
	char ch;

	if(firstCharacter != '0'){
		ch = firstCharacter;
		do{
			num = 10 * num + (int)(ch - '0');
			ch = fgetc(sourceFile);
		} while(isdigit(ch));
	}
	
	else{
		ch = fgetc(sourceFile);
		
		if((ch>= '0') && (ch <= '7'))				
			do{
				num = 8*num + (int)(ch-'0');
				ch = fgetc(sourceFile);
			}while ((ch >= '0') && (ch <= '7'));
			
			else if ((ch == 'X') || (ch == 'x')){
				while ((value = hexValue(ch=fgetc(sourceFile))) != -1)
					num = 16 * num + value;
			}
			
			else num = 0;				
	}
	
	ungetc(ch, stdin);
	return num;
}
struct tokenType{
	int number;
	union{
		char id[ID_LENGTH];
		int num;
	} value;
	char* tokenValue;
};
enum _noderep {terminal, nonterm};  
typedef struct nodeType {
        struct tokenType token;
        enum _noderep noderep;
        struct nodeType * son;
        struct nodeType * brother;
}Node;
struct tokenType scanner(){
	struct tokenType token;
	int i, index;
	char ch, id[ID_LENGTH];

	token.number = tnull;

	do{		
		while(isspace(ch = fgetc(sourceFile)));
		
		if(superLetter(ch)){
			i=0;
			
			do{
				if(i<ID_LENGTH) id[i++] = ch;
				ch = fgetc(sourceFile);
			}while(superLetterOrDigit(ch));
			
			if(i>=ID_LENGTH) lexicalError(1);

			id[i] = '\0';
			ungetc(ch,stdin);
			
			for(index=0; index < NO_KEYWORDS; index++)
				if(!strcmp(id, keyword[index])) break;
				
				if(index < NO_KEYWORDS){
					token.number = tnum[index];
					strcpy(token.value.id, keyword[index]);
				}				
				else{
					token.number = tident;
					strcpy(token.value.id, id);
				}
		}
		
		else if(isdigit(ch)){
			token.number = tnumber;
			token.value.num = getIntNum(ch);
		}
		
		else{
			switch(ch){
				case '/': 
				ch = fgetc(sourceFile); 
	
				if(ch == '*')
					do{
						while(ch != '*') ch = fgetc(sourceFile);
						ch = fgetc(sourceFile);
					} while (ch != '/');				
				else if(ch =='/')
					while(fgetc(sourceFile) != '\n');
		
				else if(ch == '='){
					token.number = tdivAssign;
					strcpy(token.value.id, "/=");
				}					
				else{ 
					token.number = tdiv;
					strcpy(token.value.id, "/");
					ungetc(ch, stdin); 
				}
				break;
				
			case '!':
				ch = fgetc(sourceFile);

				if(ch == '='){
					token.number = tnotequ;
					strcpy(token.value.id, "!=");
				}			
				else{   
					token.number = tnot;
					strcpy(token.value.id, "!");
					ungetc(ch, stdin); 
				}
				break;
	
			case '%':
				ch = fgetc(sourceFile);
			
				if(ch == '='){
					token.number = tmodAssign;
					strcpy(token.value.id, "%=");
				}	
				else{   
					token.number = tmod;
					strcpy(token.value.id, "%");
					ungetc(ch, stdin); 
				}
				break;

			case '&':
				ch = fgetc(sourceFile);
			
				if(ch == '&'){
					token.number = tand;
					strcpy(token.value.id, "&&");
				}			
				else{   
					lexicalError(2);
					ungetc(ch, stdin); 
				}
				break;
			
			case '*':
				ch = fgetc(sourceFile);
			
				if(ch == '='){
					token.number = tmulAssign;
					strcpy(token.value.id, "*=");
				}			
				else{   
					token.number = tmul;
					strcpy(token.value.id, "*");
					ungetc(ch, stdin); 
				}
				break;

			case '+':
				ch = fgetc(sourceFile);
			
				if(ch == '+'){
					token.number = tinc;
					strcpy(token.value.id, "++");
				}			
				else if (ch == '='){
					token.number = taddAssign;
					strcpy(token.value.id, "+=");
				}			
				else{   
					token.number = tplus;
					strcpy(token.value.id, "+");
					ungetc(ch, stdin); 
				}
				break;
			
			case '-':
				ch = fgetc(sourceFile);
			
				if(ch == '-'){
					token.number = tdec;
					strcpy(token.value.id, "--");
				}			
				else if(ch == '='){
					token.number = tsubAssign;
					strcpy(token.value.id, "-=");
				}			
				else{   
					token.number = tminus;
					strcpy(token.value.id, "-");
					ungetc(ch, stdin); 
				}
				break;
		
			case '<':
				ch = fgetc(sourceFile);
			
				if(ch == '='){
					token.number = tlesse;
					strcpy(token.value.id, "<=");
				}			
				else{   
					token.number = tless;
					strcpy(token.value.id, "<");
					ungetc(ch, stdin); 
				}
				break;

			case '=':
				ch = fgetc(sourceFile);
			
				if(ch == '='){
					token.number = tequal;
					strcpy(token.value.id, "==");
				}			
				else{   
					token.number = tassign;
					strcpy(token.value.id, "=");
					ungetc(ch, stdin); 
				}
				break;
		
			case '>':
				ch = fgetc(sourceFile);

				if(ch == '='){
					token.number = tgreate;
					strcpy(token.value.id, ">=");
				}
			
				else{   
					token.number = tgreat;
					strcpy(token.value.id, ">");
					ungetc(ch, stdin); 
				}
				break;
			
			case '|':
				ch = fgetc(sourceFile);
			
				if(ch == '|'){
					token.number = tor;
					strcpy(token.value.id, "||");
				}			
				else{   
					lexicalError(3);
					ungetc(ch, stdin);
				}
				break;
			
			case '(' : token.number = tlparen; 
				strcpy(token.value.id, "(");
				break;

			case ')' : token.number = trparen; 
				strcpy(token.value.id, ")");
				break;
			
			case ',' : token.number = tcomma; 
				strcpy(token.value.id, ",");
				break;
			
			case ';' : token.number = tsemicolon; 
				strcpy(token.value.id, ";");
				break;
			
			case '[' : token.number = tlbracket; 
				strcpy(token.value.id, "[");  
				break;
			
			case ']' : token.number = trbracket; 
				strcpy(token.value.id, "]");  
				break;

			case '{' : token.number = tlbrace;
				strcpy(token.value.id, "{");  
				break;
			
			case '}' : token.number = trbrace;
				strcpy(token.value.id, "}");  
				break;
			
			case EOF : token.number = teof; 
				break;
			
			default: {
				printf("Current character : %c", ch);
				lexicalError(4);
				break;
				}
			} 
		}
	} while (token.number == tnull);

	return token;
} 