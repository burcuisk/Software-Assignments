#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct carNode {
	char id[5];
	char name[16];
	int depts[20];
	struct carNode *next;
}car;

typedef struct CarQueue{
	car *start;
	car *end;
}Queue;

typedef struct departmentNode {
	char name[20];
	int copyNumb;
	int process_time;
	car *processing;
	Queue *cars;
	Queue *processedCars;
	struct departmentNode *copy;
	struct departmentNode *next;
	struct departmentNode *previous;
}department;

int readFile(FILE* ,char**);
int processCommand(car[50][50],char*[10],char*,department**,department**);
void addDepartment (department **, department **,int ,char *,int );
void produceCar(car[50][50],int,char*,char*,int);
department *addCopy(int ,int ,char *,int);
void timeLoop(car[50][50],department **,department**,char*[],int);
int getMaxReportTime(char* reports[] ,int reportTimes[10]);
void createCarQueuesforDepartments(department **);
void startProduce(department **,car[50][50],int ,int );
void printFactory(department *,department *);
void insert(Queue** ,car* );
car * pool (Queue**);
void goNextDepartment (department **,car**);
void traverseDeptsProcessCar(department **,department **,int);
void bringFirstDeptQueue (department **);
void report(char *,int ,department **,department **,car[50][50],int);

int main(int argc, char *argv[]) {

    FILE *inputFile = fopen(argv[1], "r");
    department *head = NULL,*end= NULL;
    char *buff;
    int i,j,dept_numb;
	car Cars[50][50]; /* produce car list time is according to index */
	char *reportCommands[10]; /* in input file report commands */

	for(i = 0 ; i<50 ; i++ ) {
		for(j = 0 ; j<50 ; j++ )
			Cars[i][j].id[0] = '-';
	}

	for(i=0;i<10;i++) {
		reportCommands[i] = malloc(25);
		strncpy(reportCommands[i] , "free",4);
	}
	buff = malloc(80 * sizeof(char));

	while( readFile(inputFile,&buff)) {  /* read file and create data structures */
		dept_numb = processCommand(Cars,reportCommands,buff,&head,&end);
	}
	timeLoop(Cars,&head,&end,reportCommands,dept_numb); /* forward time and operate commands */

	fclose(inputFile);
	return 1;
}

void timeLoop(car Cars[50][50],department **start,department **end,char *reports[10],int dept_num) {

	int reportTimes[10],i,j,max;
	for(i = 0 ; i<10;i++){				/* in report commands times */
		reportTimes[i] = 0;
	}
	max = getMaxReportTime(reports,reportTimes); /* get max report times for the max process time */

	createCarQueuesforDepartments(start);	/* create car queues for the departments */

	for (i = 1 ; i<=max; i++) { 				/* forward time and process commands */

		if (i != 1) {
			traverseDeptsProcessCar(start,end,dept_num);
		}  /* traverse all depts and if has process time ok push forward dept else increase process time */

		for(j=0;j<50;j++) {						/* i. time has will be producing car or cars start produce */
			if (Cars[i][j].id[0] != '-') {
				startProduce (start,Cars,i,j);
			}
		}

		for(j=0; j<10 ; j++) {					/* if input file has at that time(i) report command */
			if (reportTimes[j] == i) {
				report(reports[j],i,start,end,Cars,dept_num);
			}
		}
	}
}

void report(char *command,int time,department **start,department **end,car Cars[50][50],int dept_num) {
	int i = 1,j,k;
	department *temp = *end;
	department *temp2 ,*temp1 = *start;
	car* c;
	char *reportType;
	char *id;

	strtok(command," ");
	strtok(NULL," ");
	reportType = strtok(NULL," ");
	id = strtok(NULL," ");

	if(strncmp(reportType,"Departments",11) == 0) { 	/*report for departments */
		printf("\nCommand: Report Departments %d\n",time);
		while (1) {
			if( temp == NULL) break;
			printf("---------------------------------------\n");
			printf("|Report For Department \"%s %d\"|\n",temp->name,temp->copyNumb);
			printf("---------------------------------------\n");
			if(temp->processing == NULL)
				printf("%s %d is now free\n",temp->name,temp->copyNumb);
			else {
				printf("I am currently processing %s %s\n",temp->processing->name,temp->processing->id);
			}
			if(temp ->processedCars->start != NULL) {
				printf("Processed Cars:\n");
				c = temp ->processedCars->start;
				while ( c != NULL ) {
					printf("%d. %s %s\n",i++,c->name , c->id);
					c = c->next;
				}
				i = 1;
			}
			temp2 = temp->copy;
			while (1) {
				if (temp2 == NULL) break;
				printf("---------------------------------------\n");
				printf("|Report For Department \"%s %d\"|\n",temp2->name,temp2->copyNumb);
				printf("---------------------------------------\n");
				if(temp2->processing == NULL)
					printf("%s %d is now free\n",temp2->name,temp2->copyNumb);
				else {
					printf("I am currently processing %s %s\n",temp2->processing->name,temp2->processing->id);
				}
				if(temp2 ->processedCars->start != NULL) {
					printf("Processed Cars:\n");
					c = temp2 ->processedCars->start;
					while ( c != NULL ) {
						printf("%d. %s %s\n",i++,c->name , c->id);
						c = c->next;
					}

					i = 1;
				}
				temp2 = temp2->copy;
			}
			temp = temp->previous;
			printf("\n");
		}
	}

	if(strncmp(reportType,"Cars",4) == 0) {
		printf("Command: Report Cars %d\n",time);
		for (i = 1 ; i<time ; i++) {
			for(j = 0 ; j<50 ; j++) {
				if(Cars[i][j].id[0] != '-') {
					k = 0;
					printf("------------------------\n");
					printf("|Report for %s %s|\n",Cars[i][j].name,Cars[i][j].id);
					printf("------------------------\n");

					while(1) {
						if(temp1 == NULL) break;
						if(Cars[i][j].depts[k] >= temp1->process_time)
							printf ("%s:%d,  ",temp1->name,temp1->process_time);
						else {
							printf ("%s:%d,  ",temp1->name,Cars[i][j].depts[k]);
						}
						temp1 = temp1->next;
						k++;
					}
					temp1 = *start;
					printf ("| Start Time%d | Completeyüzde | ... Complete\n",i);
				}
			}
		}
	}
	else if(strncmp(reportType,"Car",3) == 0) {

		printf("Command: Report Car %d %s",time,id);
		for (i = 1 ; i<time ; i++) {
			for(j = 0 ; j<50 ; j++) {
				if(strncmp(Cars[i][j].id, id, 4) == 0) {
					k = 0;
					printf("------------------------\n");
					printf("|Report for %s %s|\n",Cars[i][j].name,Cars[i][j].id);
					printf("------------------------\n");

					while(1) {
						if(temp1 == NULL) break;
						if(Cars[i][j].depts[k] >= temp1->process_time)
							printf ("%s:%d,  ",temp1->name,temp1->process_time);
						else {
							printf ("%s:%d,  ",temp1->name,Cars[i][j].depts[k]);
						}
						temp1 = temp1->next;
						k++;
					}
					temp1 = *start;
					printf ("| Start Time%d | Completeyüzde | ... Complete\n",i);
				}
			}
		}
	}
	printf("\n");
}

void traverseDeptsProcessCar(department **start,department **end,int dept_num) { /* process cars in departments according to time loop */

	int i=dept_num - 1;
	department *temp1 = *end;
	department *temp2;

	while (1) {
			if( (temp1) == NULL) { 				/* if first department look queue */
				bringFirstDeptQueue (start);
				break;
			}
			if (temp1->processing != NULL) {		/* department is not free */
				temp1->processing->depts[i]++;
				if(temp1->processing->depts[i] >= (temp1)->process_time) {			/* processNumb at car for this dept >= dept processNumb */
					if((temp1)->next == NULL) {  										/* for last department */
						insert(&((temp1)->cars),(temp1)->processing);					/* insert finish list */
						insert(&(temp1->processedCars),temp1->processing);				/* insert this departments processed list */
						temp1->processing = NULL;										/* department is free */
					}
					else {
						goNextDepartment(&(temp1),&(temp1->processing));
					}
				}
			}

			temp2 = temp1->copy;
			while (1) { 							/* look at department temp1 copies */

				if (temp2 == NULL) break;

				if (temp2->processing != NULL) {
					temp2->processing->depts[i]++;
					if(temp2->processing->depts[i] >= temp2->process_time) {
						if(temp2->next == NULL) {
							insert(&(temp1->cars),temp2->processing);
							insert(&(temp2->processedCars),temp2->processing);
							temp2->processing = NULL;
						}
						else {

						goNextDepartment(&temp2, &(temp2->processing));

						}
					}
				}
				temp2 = temp2->copy;
			}
			i--;
			temp1 = temp1->previous;
		}
}

void bringFirstDeptQueue (department **start){

	department *temp = *start;

	if (temp->processing == NULL) {
		temp->processing = pool(&((*start)->cars));
	}


	temp = temp->copy;
	while (temp != NULL) {
		if(temp->processing == NULL) {
			temp->processing = pool(&((*start)->cars));
		}
		temp = temp->copy;
	}
}


void goNextDepartment (department **Dept,car** Car) {

	department *temp = (*Dept)->next->copy;
	if ((*Dept)->next->processing == NULL) {
		(*Dept)->next->processing = *Car;
		insert(&((*Dept)->processedCars),(*Dept)->processing);
		(*Dept)->processing = NULL;
		return;
	}
	while (1) {
		if (temp == NULL) return;

		if(temp->processing == NULL) {
			temp->processing = *Car;
			insert(&((temp)->processedCars),(*Dept)->processing);
			(*Dept)->processing = NULL;
			return;
		}
		temp = temp->copy;
	}
}

void startProduce(department **start,car Cars[50][50],int i,int j) {   /* PARAMETRE DEGISIMI */

	department *temp1 = *start;

	while (1) {
		if((temp1) == NULL) {
			break;
		}
		if(temp1->processing == NULL) {
			temp1->processing = &(Cars[i][j]);
			return;
		}
		(temp1) = (temp1)->copy;
	}
	/* all copies is full */
	insert(&((*start)->cars),&Cars[i][j]);

	return;
}

void insert(Queue** cars,car* Car ) {

	if((*cars)->start == NULL) {
		(*cars)->start = Car;
		(*cars)->end = Car;
	}
	else {
		(*cars)->end->next = Car;
		(*cars)->end = Car;
	}
	return;
}

car * pool (Queue **cars) {

	car *Car;

	if((*cars)->start == NULL)
		return NULL;
	Car = (*cars)->start;
	(*cars)->start=(*cars)->start->next;

	if((*cars)->start == NULL)
		(*cars)->end = NULL;

	return Car;
}

void createCarQueuesforDepartments(department **start) {    	/* degiscek sadece ilk son için oluþturcan*/

	department *temp = *start;
	while (1) {									/* create car queues for the departments */
		if( temp == NULL) break;
		temp->cars = malloc(sizeof(car));
		temp->cars->start = NULL;
		temp->cars->end = NULL;
		temp = temp->next;
	}

	return;
}

int getMaxReportTime(char* reports[] ,int reportTimes[10]) {

	int i ,max;
	char* size = malloc(30);

	for(i=0;i<10;i++) {
		if (strncmp(reports[i],"free",4) == 0) {
			break;
		}
		strncpy(size,reports[i],25);
		size = strtok(size," ");
		reportTimes[i] = atoi(strtok(NULL," "));
	}
	 max = reportTimes[0];

	 for(i=0;i<10;i++){
		 if(max < reportTimes[i])
			 max = reportTimes[i];
	 }
	 return max;
}

void printFactory(department *start,department *end) {

	department *temp = start;
	department *temp2;

	while (1) {
		if( temp == NULL) break;
		printf("-%s%d ",temp->name,temp->copyNumb);
		temp2 = temp->copy;
		while (1) {
			if (temp2 == NULL) break;
			printf("%s%d " ,temp2->name,temp2->copyNumb);
			temp2 = temp2->copy;
		}
		temp = temp->next;
		printf("\n");
	}

}

void addDepartment (department **start, department **end,int copy_numb,char *name,int time) {

	department *temp, *copyPrev;
	temp = (department*)malloc (sizeof(department));
	strcpy(temp->name,name);
	temp->process_time = time;
	temp->next = NULL;
	temp->copy = NULL;
	temp->copyNumb = 1;
	temp->processing= NULL;
	temp->cars = NULL;
	temp->processedCars = malloc(sizeof(car));
	temp->processedCars->start = NULL;
	temp->processedCars->end = NULL;

	printf("Department %s has been created.\n",name);
	if(*start == NULL ) {
		temp->previous = NULL;
		*start = temp;
		*end = temp;
	}
	else {
		(*end)->next = temp;
		temp->previous = *end;
		*end = temp;
	}

	if (copy_numb > 1)
		 (*end)->copy = addCopy(copy_numb,2,name,time);

	if(temp->previous != NULL) {
		copyPrev = temp->previous->previous;
		if(temp->previous->copy != NULL)
			temp = temp->previous->copy;
		while(temp != NULL) {
			temp->next = *end;

			if(copyPrev != NULL)
				temp->previous = copyPrev;

			temp = temp->copy;
		}
	}
}

department *addCopy(int copy_numb,int i,char *name,int time) {

	department *temp;

	if(i >= copy_numb+1){
		return NULL;
	}
	temp =(department*) malloc (sizeof(department));
	strcpy(temp->name,name);
	temp->process_time = time;
	temp->next = NULL;
	temp->copyNumb = i;
	temp->processing = NULL;
	temp->cars = NULL;
	temp->processedCars = malloc(sizeof(car));
	temp->processedCars->start = NULL;
	temp->processedCars->end = NULL;
	temp->copy = addCopy(copy_numb,++i,name,time);
	return temp;

}

int processCommand(car Cars[50][50],char *reportCommands[],char *command,department **start,department **end){

	int copy_numb,time,start_time;
	static int dept_numb=0;
	static int i = 0;
	char *name,*car_model,*car_unique_code;
	if(strncmp(command,"PrintFactory",12) == 0){
		printFactory(*start,*end);
	}

	else if (strncmp(command,"Report",6) == 0) {
		strncpy(reportCommands[i++],command,25);
	}

	command = strtok(command," ");
	if( strncmp(command,"AddDept",7) == 0) {
		copy_numb = atoi(strtok(NULL," "));
		name = strtok(NULL," ");
		time = atoi(strtok(NULL," "));
		addDepartment (start,end,copy_numb,name,time);
		dept_numb++;
	}

	else if (strncmp(command,"Produce",7) == 0) {
		start_time = atoi(strtok(NULL," "));
		car_model = strtok(NULL," ");
		car_unique_code = strtok(NULL," ");
		produceCar(Cars,start_time,car_model,car_unique_code,dept_numb);

	}

	return dept_numb;
}

void produceCar (car Cars[50][50],int startTime,char *model,char *uniqueCode,int dept_numb) {

	int j,i;
	car produce;

	strncpy(produce.id, uniqueCode,5);
	produce.id[sizeof produce.id - 1] = '\0';
	strncpy(produce.name,model,16);

	for(i=0 ; i<20 ;i++) {
		produce.depts[i] = 0;
	}
	produce.next = NULL;

	for(j = 0 ; j<50 ; j++ ) {
		if (Cars[startTime][j].id[0] == '-') {
			Cars[startTime][j] = produce;
			break;
		}
	}
}

int readFile(FILE *input,char** buff) {

	if(fgets(*buff,80,input) != NULL )
		return 1;
	return 0;
}
