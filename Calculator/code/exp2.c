
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

typedef struct value{
	char oprtr;
	struct value *next;
}operators;

typedef struct results{
	char *result;
	struct results *next;
}operands;

typedef struct res {
	char *result;
	char *type;
	struct res *next;
}Results;

typedef struct queue{
	Results *start;
	Results *end;
	int elementNumb;
}Queue;

int readFile(FILE* ,char**);
void closeFiles(FILE *,FILE*);
void print(FILE *,Queue*);
void processCommand(char*,FILE*,Queue*);
int checkBalancedParanthesis(char *);
int controlValid(char *,char *);
void push(char ,operators *);
void push2(char *,operands *);
char top(operators *,operators *);
int isEmpty(operators *,operators *);
char pop(operators *);
char *pop2(operands *);
void insert(Queue *,char *,char *);
char *poll(Queue *);
char *getType(Queue *);
char* calculate(char*,char*,char );
int compareOp(char,char);
char* append(char *,char);
char* evaluationExpression(char* ,int);
char* convertDecimal(char *);
char* convertHexadecimal(char *);
void printStack(operators *start);

int main(int argc, char *argv[]) {

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2],"w");
    Queue results;
    results.start = NULL;
    results.end = NULL;
    results.elementNumb=0;

    char  *buff = malloc(8000 * sizeof(char));

    while( readFile(inputFile,&buff)) {

    	if(strncmp(buff,"print",5) == 0){
    		print(outputFile,&results);
    	    results.start = NULL;
    	    results.end = NULL;
    	    results.elementNumb=0;
    	}

    	else
    		processCommand(buff,outputFile,&results);
    }
    free(buff);
    closeFiles(inputFile,outputFile);
    return 0;
}

void processCommand(char *command,FILE* outputFile,Queue *resultsQueue) {

	char *valueType,*expression;
	int b=0;
	char *res;
	command = strtok(command," ");
	valueType = strtok(NULL," ");
	expression = strtok (NULL, " ");
	expression = strtok (expression, "\"");
	int a = checkBalancedParanthesis(expression);
	if(a == 1)
		b = controlValid(expression,valueType);

	if(!(a&&b)){
		 insert(resultsQueue,"error","");
		 return;
	}

	if (strncmp(valueType,"hex",3) != 0){ // if integer values
		res = evaluationExpression(expression,1);
		insert(resultsQueue,res,"integer");
		return;
	}
	else {			// if hex values
		res = evaluationExpression(expression,2);
		res = convertHexadecimal(res);
		insert(resultsQueue,res,"hex");
		return;
	}
}

char* evaluationExpression(char* exp,int x){

	char *retVal,*res,*sign;
	operands *startOprnd,*endOprnd;
	operators *startOprtr,*endOprtr;
	startOprtr = (operators *) malloc(sizeof(operators));
	endOprtr = (operators *) malloc(sizeof(operators));
	startOprtr->next = endOprtr;
	startOprnd = (operands *) malloc(sizeof(operands));
	endOprnd = (operands *) malloc(sizeof(operands));
	startOprnd->next = startOprnd;

	char *value="";
	char a,op;
	char *op1,*op2;
	char *result;
	int i=0,j=0,negative;

	size_t sz = strlen(exp);
	long int limit = sz+exp;

	while(1){
		i++;
		if(exp >= limit) break;

		if (i == 1) {
			if(*exp == '+' || *exp == '-'){
				exp++;
				if(*exp == '(') {
					negative = 1;
					exp++;
					continue;
				}
				exp--;
				value = append(value,*exp);
				exp++;
				continue;
			}
		}

		if(*exp == '(') {									// operand ( push stack
			push(*exp,startOprtr);
			exp++;
			if(*exp == '+' || *exp == '-') {				// (+ or (- statement append to exp
				value="";
				value = append(value,*exp);
				exp++;
				continue;
			}
			continue;
		} // end (

		else if(*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/') {			// if ex operand
			exp++;
			if(*exp == '('){												// operator( situation
				push(*(--exp),startOprtr);
				exp++;continue;
			}
			//operatoroperand situaion
			if (*exp == '0' || *exp == '1' ||*exp == '2' ||*exp == '3' ||*exp == '4' || *exp == '5' ||*exp == '6' || *exp == '7' ||*exp == '8' ||*exp == '9') {
				exp--;
				a = top(startOprtr,endOprtr);
				if(a != NULL) {
					if(a == '(') {
						push(*exp,startOprtr);
						exp++;
						continue;
					}
					if(compareOp(a,*exp)){ // op<=top
						while(1){

							if(a == NULL || a == '(') break;

							if(compareOp(a,*exp)) {
								op = pop(startOprtr);
								op2 = pop2(startOprnd);
								op1 = pop2(startOprnd);
								result = calculate(op2,op1,op);
								push2(result,startOprnd);
								a = top(startOprtr,endOprtr);
								continue;
							}
							else break;
						}
					}
				}
				push(*exp,startOprtr);
				exp++;
				continue;
			}

			if (*exp == '+' || *exp == '-' ) {							//operatoroperator situation
				exp++;
				if(*exp == '(') {										// optopt( situation
					exp--;exp--;
					if(*exp == '+') {									//if ++( or +-( push second exp opt stack and continue
						exp++;
						if( *exp == '+' || *exp=='-' ){
							a = top(startOprtr,endOprtr);
							if(a != NULL) {
								if(a == '(') {
									push(*exp,startOprtr);
									exp++;exp++;
									continue;
								}
								if(compareOp(a,*exp)){ // op<=top
									while(1){
										if(a == NULL || a == '(') break;

										if(compareOp(a,*exp)) {
											op = pop(startOprtr);
											op2 = pop2(startOprnd);
											op1 = pop2(startOprnd);
											result = calculate(op2,op1,op);
											push2(result,startOprnd);
											a = top(startOprtr,endOprtr);
											continue;
										}
										else break;
									}
								}
							}
							push(*exp,startOprtr);
							exp++; continue;
						}
					}
					else if(*exp == '-'){								// if -+( -> push - if --( -> push +
						exp++;
						if(*exp == '-') {								// if --( situation
							a = top(startOprtr,endOprtr);
							if(a != NULL) {
								if(a == '(') {
									push(*exp,startOprtr);
									exp++;exp++;
									continue;
								}
								if(compareOp(a,'+')){ // op<=top
									while(1){
										if(a == NULL || a == '(') break;

										if(compareOp(a,*exp)) {

											op = pop(startOprtr);
											op2 = pop2(startOprnd);
											op1 = pop2(startOprnd);
											result = calculate(op2,op1,op);
											push2(result,startOprnd);
											a = top(startOprtr,endOprtr);
											continue;
										}
										else break;
									}
								}
							}
							push('+',startOprtr);
							exp++; continue;
						}
						else if (*exp == '+' ) {						//-+( situation
							a = top(startOprtr,endOprtr);
							if(a != NULL) {
								if(a == '(') {
									push(*exp,startOprtr);
									exp++;exp++;
									continue;
								}
								if(compareOp(a,'-')){ // op<=top
									while(1){
										if(a == NULL || a == '(') break;

										if(compareOp(a,*exp)) {

											op = pop(startOprtr);
											op2 = pop2(startOprnd);
											op1 = pop2(startOprnd);
											result = calculate(op2,op1,op);
											push2(result,startOprnd);
											a = top(startOprtr,endOprtr);
											continue;
										}
										else break;
									}
								}
							}
							push('-',startOprtr);
							exp++; continue;
						}
					}
				}

				else {													// oprtroprtroperand situation firs opt in the stack second opt mult by operand

					exp--;
					value = "";
					value = append(value,*exp);
					exp--;

					a = top(startOprtr,endOprtr);
					if(a != NULL) {
						if(a == '(') {
							push(*exp,startOprtr);
							exp++;exp++;
							continue;
						}
						if(compareOp(a,*exp)){ // op<=top
							while(1){
								if(a == NULL || a == '(') break;

								if(compareOp(a,*exp)) {
									op = pop(startOprtr);
									op2 = pop2(startOprnd);
									op1 = pop2(startOprnd);
									result = calculate(op2,op1,op);
									push2(result,startOprnd);
									a = top(startOprtr,endOprtr);
									continue;
								}
								else break;
							}
						}
					}
					push(*exp,startOprtr);
					exp++;exp++;
					continue;
				}}
		} // end start op

		else if(*exp == ')') {
			while(1) {

				if(top(startOprtr,endOprtr) == '(') {
					pop(startOprtr);
					break;
				}
				if(startOprtr->next == endOprtr){
					exp++; break;
				}
				op = pop(startOprtr);
				op2 = pop2(startOprnd);
				op1 = pop2(startOprnd);
				result = calculate(op2,op1,op);
				push2(result,startOprnd);
			}
			exp++;
			continue;
		} // end )

		else {																	// if number append and push operand stack

			while(*exp != '(' && *exp != ')' && *exp != '+' && *exp != '-' && *exp != '*' && *exp != '/' && exp <= limit ){
				value = append(value,*exp);
				j++;
				exp++;
			}

			if(x == 2) value = convertDecimal(value);
			push2(value,startOprnd);
			value="";
			continue;
		}// end number
		exp++;
	} // end while


	while(startOprtr->next != endOprtr ) {
		op = pop(startOprtr);
		op2 = pop2(startOprnd);
		op1 = pop2(startOprnd);
		result = calculate(op2,op1,op);
		push2(result,startOprnd);
	}
	retVal = pop2(startOprnd);
	if (negative == 1) {
	   	sign = "-";
	   	res = (char *) malloc(1 + strlen(sign)+ strlen(retVal) );
        strcpy(res, sign);
	    strcat(res, retVal);
	    return res;
	}

	return retVal;
} // end function

char* convertDecimal(char *value) {

	long int decimalNumber=0;
	size_t sz = strlen(value);
    char hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
      '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char hexDigitUpper[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
      '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char* val = malloc(30);
    int i, j, power=0;
    value = value+sz-1;

    for(i=0;i<sz;i++) {
        for(j=0; j<16; j++){
            if(*value == hexDigits[j] || *value == hexDigitUpper[j]  ){
                decimalNumber += j*pow(16, power);
            }
        }
        value--;
        power++;
    }
    value++;
    if(*value == '-') decimalNumber = decimalNumber * -1;
    sprintf(val,"%d",decimalNumber);
    return val;
}

char* convertHexadecimal(char *value){

	char *retVal;
	int j=0,cont=0;
	char *sign;
    char * res;

	if(*value == '+' || *value == '-') {
		if (*value == '-') cont=1;
		value++;
	}
    long int decimalNumber = atoi(value);

    int i=0,rem;
    char hex[100];
    char dig[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    while(decimalNumber > 0)
    {
        rem = decimalNumber % 16;
        hex[i++] = dig[rem]; // adding the correct hexadecimal digit to the result based on remainder
        decimalNumber = decimalNumber / 16;
    }
    hex[i] = '\0';
    retVal = (char*)malloc((i) * sizeof(char));

    for(j=i-1;j>=0;j--) {
    	*retVal = hex[j];
    	retVal++;
    }
    retVal = retVal-i;
    value--;
    if( cont == 1) {
    	sign = "-";
    	res = (char *) malloc(1 + strlen(sign)+ strlen(retVal) );
        strcpy(res, sign);
        strcat(res, retVal);
    	return res;
    }

    return retVal;
}

char* append(char *value,char add) {

	    size_t sz = strlen(value);
	    char *str = malloc(sz + 2);
	    strcpy(str, value);
	    str[sz] = add;
	    str[sz + 1] = '\0';
	    return str;

}

char* calculate(char* op2,char* op1,char op){

	int result=0;
	char *str = malloc(30);
	int op_2 = atoi(op2);
	int op_1 = atoi(op1);
	switch(op) {
		case	'+' :
			result = op_1+op_2;
			break;
		case	'-' :
			result = op_1-op_2;
			break;
		case	'*':
			result = op_1 * op_2;
			break;
		case	'/':
			result = op_1 / op_2;
			break;

	}
	sprintf(str, "%d", result);
	return str;
}

int compareOp(char top,char newOp) {       // this function control operator predence

	int topp=0;
	int newOpp=0;

	if(top =='*' || top == '/')
		topp = 1;

	if(newOp =='*' || newOp == '/')
		newOpp = 1;

	if(newOpp <= topp)
		return 1;

	return 0;

}

void print(FILE *output, Queue *results) {
	char *type = getType(results);
	char* write = poll(results);
	while(1) {
		if(write == NULL  ) break;
		if(strncmp(write,"error",5)==0){
			fprintf(output,"%s\n",write); }
		else
			fprintf(output,"%s %s\n",type,write);
		type = getType(results);
		write = poll(results);

	}
}

int checkBalancedParanthesis(char *exp) {    // check paranthesis

	operators *start,*end;						// operators stack start end pointer
	char a[2500]=""; 							// paranthes array
	int i = 0,j=0;

	while(1){

		if(*exp != '0' && *exp != '1' &&*exp != '2' &&*exp != '3' &&*exp != '4' &&*exp != '5' &&*exp != '6' &&*exp != '7' &&*exp != '8' &&*exp != '9' &&
		   *exp != 'a' &&*exp != 'b' &&*exp != 'c' &&*exp != 'd' &&*exp != 'e' &&*exp != 'f' &&
		   *exp != 'A' &&*exp != 'B' &&*exp != 'C' &&*exp != 'D' &&*exp != 'E' &&*exp != 'F' &&
		   *exp != '+' &&*exp != '-' &&*exp != '*' &&*exp != '/' &&*exp != '(' &&*exp != ')')
		{
			break;
		}		//eliminate do not paranteses char
		if(*exp == '(' || *exp == ')' ){
			a[i++] = *exp;
		}
		exp++;
	}
	i--;

	start = (operators *) malloc(sizeof(operators));
	end = (operators *) malloc(sizeof(operators));
	end->oprtr =NULL;
	start->next = end;

	for(j=0; j<i+2; j++){

		if( j == i+1 ) {											// if stack has last ( elements or stack is nonempty finish input
			if( start->next->oprtr != NULL || a[i] == '(')
				{free(start);free(end);return 0;}
			return 1;
		}

		if(a[j] == '(')												// if operator is ( push into the stack
			push(a[j],start);

		else if(a[j] == ')') {										// if op is ) and stack top op is ) ret unbalanced if ( pop the stack
			if(start->next->oprtr != NULL){
				if(start->next->oprtr != '(')
					{ free(start);free(end);return 0;}
				else
					pop(start);
			}
			else 													// op is ) stack is empty ret unbalanced
				{ free(start);free(end);return 0;}
		}
	}
	free(start);
	free(end);
	return 1 ;
}

int controlValid(char *exp,char *valueType){  // control input is valid or invalid

	int ex=0;
	int i =0,k=0,j=0;
	while (1){
		j++;
		if(*exp == NULL) break;

		if(j == 1) {							//first operator control
			if(*exp == '+' || *exp == '-') {
					exp++;
					if(*exp == '+' || *exp == '-')
						return 0;
					exp--;
			}
		}

		if(*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/') {
			exp++;
			if(*exp == '*' || *exp == '/')											// control +/ */ // /* statements
				return 0;
			if(*exp == '+' || *exp == '-') {										// control a lot of operands repeat
				exp++;
				if(*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/') return 0;
				exp--;
			}
			exp--;
		}
		exp++;
		i++;
	}
	exp = exp-i;

	if (strncmp(valueType,"hex",3) != 0) {				// control input for integer format
		while (1){
			if(*exp == NULL) break;;
			if (*exp == '/') {
				exp++;
				if(*exp == '0') return 0;
			}

			if((*exp != '0' && *exp != '1' && *exp !='2' && *exp !='3' && *exp !='4' && *exp !='5' && *exp !='6' && *exp !='7' && *exp != '8' && *exp !='9' && *exp !='+'&& *exp !='-'&& *exp !='*'&& *exp !='/'&& *exp !='('&& *exp !=')'))
			{ 	return 0;}
			exp++;
		}
	}

	else {											    //control input for hex format

		while (1){
			ex = 0;
			if(*exp == NULL) break;
			if (*exp == '/') {
				exp++;
				if(*exp == '0') return 0;
			}

			if((*exp != 'a' && *exp != 'b' && *exp !='c' && *exp !='d' && *exp !='e' && *exp !='f' &&
				*exp != 'A' && *exp != 'B' && *exp !='C' && *exp !='D' && *exp !='E' && *exp !='F' &&
				*exp !='1' && *exp !='2' && *exp != '3' && *exp !='4' && *exp !='5' && *exp !='6' && *exp !='7' && *exp !='8' && *exp !='9' && *exp !='0' &&
				*exp !='+' && *exp !='-'&& *exp !='*' && *exp !='/' && *exp !='(' && *exp !=')'))
				return 0;

			exp++;
		}
	}
	return 1;
}

int isEmpty(operators *start,operators *end){
	if(start->next == end) return 1;
	return 0;
}

void insert(Queue *resultQueue,char *value,char *type) {

	Results *result= (Results *)malloc(sizeof(Results));
	result->result = value;
	result->type = type;
	resultQueue->elementNumb++;

	if(resultQueue->start == NULL) {
		resultQueue->start = result;
		resultQueue->end = result;
		return;
	}
	resultQueue->end->next = result;
	resultQueue->end = result;
}

char* poll(Queue *resultQueue) {

	char *result;
	if(resultQueue->elementNumb == 0) {
		return NULL;
	}
	Results *val = resultQueue->start;
	resultQueue->start=resultQueue->start->next;

	if(resultQueue->start == NULL)
		resultQueue->end = NULL;

	result = val->result;
	resultQueue->elementNumb--;
	free(val);
	return result;
}

char* getType(Queue *resultQueue){

	if(resultQueue->elementNumb == 0) {
		return NULL;
	}
	return resultQueue->start->type;
}

void push(char value,operators *start){

	operators *adding;
	adding = (operators *) malloc(sizeof(operators));
	adding->oprtr = value;
	adding->next = start->next;
	start->next = adding;
}

void push2(char *value,operands *start){

	operands *adding;
	adding = (operands *) malloc(sizeof(operands));
	adding->result = value;
	adding->next = start->next;
	start->next = adding;
}

char pop(operators *start){

	operators *temp = (operators*) malloc(sizeof(operators));
	if(start->next->oprtr == NULL) return NULL;
	char val = start->next->oprtr;
	temp = start->next;
	start->next = start->next->next;
	free(temp);
	return val;

}

void printStack(operators *start) {

	while(1) {
		if(start->next->oprtr  == NULL)
			return;
		printf("%c", start->next->oprtr);
		start = start->next;
	}

	return;

}
char* pop2(operands *start){

	char *val = malloc(10);
	operands *temp =(operands*) malloc(sizeof(operands));
	if(start->next == NULL) return NULL;
	val = start->next->result;
	temp = start->next;
	start->next = start->next->next;
	free(temp);
	return val;

}

char top(operators *start,operators *end){
	if(start->next == end) return NULL;
	char val = start->next->oprtr;
	return val;
}

void closeFiles(FILE *input,FILE *output) {
	fclose(input);
	fclose(output);
}

int readFile(FILE *input,char** buff) {

	if(fgets(*buff,9000,input) != NULL )
		return 1;
	return 0;
}
