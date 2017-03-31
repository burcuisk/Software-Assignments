#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <typeinfo>
#include <string.h>
#include <utility>
#include "main.h"
using namespace std;

/* Helper functions */
int getProcessType (int type = 0) {
	static int type_;

	if (type != 0)
		type_ = type;

	return type_;
}

string readLine(ifstream& inputFile ) {
	string line = "";
	int type = getProcessType();

	// read from console
	if (type == 1)
		getline(cin,line);
	else
		getline(inputFile, line);

	return line;
}

void writeOutput(ofstream& outputFile, string line) {
	int type = getProcessType();

	// write console
	if (type == 1)
		cout << line << endl;
	else
		outputFile << line << endl;
}

vector<string> splitLine(string line, char delimiter){

    vector<string> temp;            /* split line by delimiter */
    stringstream ss(line);          /* use line and image post's width and height */
    string token;

    while(getline(ss, token, delimiter)) {
        temp.push_back(token);
    }

    return temp;
}/* end splitLine function */
/* Helper functions end*/

//node class functions
void Node :: setNext(Node* x){
	this->next = x;
}
Node* Node :: getNext (){
	return this->next;
}

string Node :: getType(){
	return this->type;
}

ForSubNode :: ForSubNode(){
	this->next = NULL;
	this->subBag = NULL;
	this->type ="sub";
}

AtomicNode* ForSubNode :: getSub(){
	return this->subBag;
}

void ForSubNode :: setSub(AtomicNode* x){
	this->subBag =x;
}

AtomicNode :: AtomicNode() {
	this->next =NULL;
	this->name = "";
	this->type = "atomic";
}

void AtomicNode :: setName(string name){
	this->name = name;
}

string AtomicNode :: getName(){
	return this->name;
}

string getStr(int type, string path) {
	static unsigned int i = 0;
	string temp = "";

	// character
	if(type == 1) {
		temp = path[i++];
		return temp;
	}

	// name
	else {
		for (;  i < path.length(); i++) {
			if(path[i] == ')' || path[i]== '(' || path[i]== ',')
				break;
			temp += path[i];
		}
		return temp;
	}
}

Node* createList(string line){ // this function create init parameter list

	Node *head;
	AtomicNode *node;
	ForSubNode *subNode;
	string name = getStr(2,line);
	string expr = getStr(1,line);

	if (name  == ""){
		if(expr == ","){
			name = getStr(2,line);
			expr = getStr(1,line);
		}
		else if (expr == ")")
			return NULL;
	}
	node = new AtomicNode;
	node->setName(name);

	if(expr == "("){
		subNode = new ForSubNode();
		subNode->setSub(node);
		node->setNext(createList(line));
		subNode->setNext(createList(line));
		head = subNode;
	}
	else if (expr == ","){
		node->setNext(createList(line));
		head = node;
	}
	else if (expr == ")"){
		head = node;
		node->setNext(NULL);
	}
	return head;
}

Node* findNode (Node* currentDirectory,string url,int type){
	if(!currentDirectory)
		return NULL;

	if (type == 1){ // find directory
		if(currentDirectory->getType() == "sub"){
			ForSubNode *x;
			AtomicNode *y;
			x= (ForSubNode *) currentDirectory;
			y =x->getSub();
			if(y->getName() == url)
				return y;
		}
		return findNode(currentDirectory->getNext(),url,type);
	}
	else if(type == 2) { // find file
		if(currentDirectory->getType() == "atomic"){
			AtomicNode *x;
			x = (AtomicNode *) currentDirectory;
			if(x->getName() == url)
				return x;
		}
		return findNode(currentDirectory->getNext(),url,type);
	}
	return NULL;
}

void list (Node *node, ofstream& outputFile){
	static string a = " |";
	static string b;
	if(node){
		if(node->getType() == "sub") {
			ForSubNode *x;
			x = (ForSubNode *) node;
			AtomicNode *y;
			y = x->getSub();
			writeOutput(outputFile, a + "---<" + y->getName() + ">");
			string b = a;
			a += "   |";
			list(y->getNext(), outputFile);
			a=b;
			list(x->getNext(), outputFile);
		}
		else if(node->getType() == "atomic"){
			AtomicNode *x;
			x =(AtomicNode*) node;
			writeOutput(outputFile, a + "--- " + x->getName());
			list(x->getNext(), outputFile);
		}
		return;
	}
	return;
}

void mkdir(Node* addingNode,string name){ // create new directory
	ForSubNode *x = new ForSubNode();
	AtomicNode *y = new AtomicNode();
	y->setName(name);
	x->setSub(y);
	while(1){
		if (!addingNode->getNext()){
			addingNode->setNext(x);
			break;
		}
		addingNode = addingNode->getNext();
	}
}// end node class functions
void copyDirectory (Node* t,Node * copyingList){
	while (1){ // adding (copying)
		if(t->getNext() == NULL){
			ForSubNode *a = new ForSubNode;
			AtomicNode *b = (AtomicNode *) copyingList;
			a->setSub(b);
			t->setNext(a);
			break;
		}
		t = t->getNext();
	}
}
void crtfl (Node* addingNode ,string name){ // create new file
	AtomicNode *x = new AtomicNode();
	x->setName(name);
	while (1){
		if (!addingNode->getNext()){
			addingNode->setNext(x);
			break;
		}
		addingNode=addingNode->getNext();
	}
}
void removeFile(Node *deletingNode,string name){
	Node *prev=deletingNode;
	while(1){
		if(deletingNode->getType() == "atomic"){
			AtomicNode *y = (AtomicNode *) deletingNode;
			if(y->getName() == name){
				prev->setNext(y->getNext());
				delete(y);
				break;
			}
		}
		prev = deletingNode;
		deletingNode = deletingNode->getNext();
	}
}
void removeDirectory(Node *deletingNode,string name){
	Node *prev=deletingNode;
	while(1){
		if(deletingNode->getType() == "sub"){
			ForSubNode *x = (ForSubNode *) deletingNode;
			AtomicNode *y = x->getSub();
			if(y->getName() == name){
				prev->setNext(x->getNext());
				delete(x);
				break;
			}
		}
		prev = deletingNode;
		deletingNode = deletingNode->getNext();
	}
}

int main(int argc, char *argv[]) {

	int processType = 1; // 1 means console, 2 means file
	string line = argv[1];

	ifstream inputFile;
	ofstream outputFile;

	if (argc > 2) {
		processType = 2;
		inputFile.open(argv[2]);
		outputFile.open(argv[3]);
	}
	processType = getProcessType(processType);

	Node *head;
	Node *currentDirectory;
	AtomicNode *first = new AtomicNode();
	currentDirectory = first;
	vector<string> lineArray;
	getStr(2,line);
	getStr(1,line);
	first->setName(".");
	first->setNext(createList(line));
	head = first;
	writeOutput(outputFile,"System initialized.\n");
	while(1){
		string command = readLine(inputFile);
		if(command == "")break;
		istringstream iss(command);
		string splitVariable;
		iss >> splitVariable;
		string commmand = splitVariable;

		if(commmand == "mkdir"){ // make directory command
			Node *x = NULL;
			Node *z=head;
			iss>>splitVariable ;
			string url = splitVariable;
			string a ="Make Directory " + url;
			writeOutput(outputFile,a);

			url += "/\0";
			lineArray= splitLine(url, '/');
			int i = lineArray.size();

			if(lineArray[0] == ".") { // send head pointer
				for (int a = 1; a<i;a++){
					if(a == i-1){ // last will be created directory
						Node *y = NULL;
						y = findNode(z,lineArray[a],1);
						if (!y){
							mkdir (z,lineArray[a]);
							string message = "Make directory executed successfully.";
							writeOutput (outputFile,message);
							break;
						}
						else {
							string message = "Error : This directory already exists!";
							writeOutput(outputFile,message);
							break;
						}
					}
					x = findNode(z,lineArray[a],1);
					if(x){z= x; continue;}
					else{ // create mkdir if this directory exist
						for (int c=a ;c<i;c++){
							mkdir(z,lineArray[a]);
						}
						string message = "Make directory executed successfully.";
						writeOutput (outputFile,message);
						break;
					}// end created.
				}
			}
			else { // lineArray[0] isnt send current directory.
				Node *z = currentDirectory;
				for (int a = 0; a<i;a++){
					if(a == i-1){ // last will be created directory
						Node *y = NULL;
						y = findNode(z,lineArray[a],1);
						if (!y){
						mkdir (z,lineArray[a]);
						string message = "Make directory executed successfully.";
						writeOutput (outputFile,message);
						break;
						}
						else {
							string message = "Error : This directory already exists!";
							writeOutput(outputFile,message);
							break;
						}
					} // end last node control
					x = findNode(z,lineArray[a],1);
					if(x){z= x; continue;}
					else{ // create mkdir if this directory exist
						for (int c=a ;c<i;c++)
							mkdir(z,lineArray[a]);
						string message = "Make directory executed successfully.";
						writeOutput (outputFile,message);
						break;
					}// end created.
				}
			}
			writeOutput(outputFile,"\n--------------------\n");
		} //end mkdir command

		else if (commmand == "ls"){ // list command
			iss>>splitVariable ;
			string url = splitVariable;
			string a;
			if (url == "ls")
			a ="List ";
			else
			a= "List "+url;
			writeOutput(outputFile,a);
			url += "/\0";
			lineArray= splitLine(url, '/');
			int i = lineArray.size();
			Node *startList ;

			if (lineArray[0] == "ls"){
				AtomicNode* a = (AtomicNode *) currentDirectory;
				string b ="<"+a->getName()+">";
				writeOutput(outputFile,b);
				list(currentDirectory->getNext(),outputFile);
			}

			else if(lineArray[0] == "."){
				Node *x;
				startList = head;
				if (i == 1){
					writeOutput(outputFile,"<.>");
					list(head->getNext(),outputFile);
				}
				else {
					for(int b=1; b<i ;b++ ){
						x = findNode(startList,lineArray[b],1);
						if(x) { startList =x; continue;}
						else{
							startList = NULL;
							writeOutput(outputFile,"Error : This directory isn't exist!");
							break;
						}
					}
					if(startList){
						AtomicNode *x = (AtomicNode *) startList;
						string message = "<"+x->getName()+">";
						writeOutput(outputFile,message);
						list(startList->getNext(),outputFile);
					}
				}
			}
			else { // list using current directory
				startList = currentDirectory;
				Node *x;
				for(int b=0; b<i ;b++ ){
					x = findNode(startList,lineArray[b],1);
					if(x) { startList =x; continue;}
					else{
						startList = NULL;
						writeOutput(outputFile,"Error : This directory isn't exist!");
						break;
					}
				}
				if(startList){
					AtomicNode *x = (AtomicNode *) startList;
					string message = "<"+x->getName()+">";
					writeOutput(outputFile,message);
					list(startList->getNext(),outputFile);
				}
			}
			writeOutput(outputFile,"\n--------------------\n");
		}// end list command

		if (splitVariable == "cd"){ // change directory command
			iss>>splitVariable ;
			string url = splitVariable;
			string a ="Change Directory " + url;
			writeOutput(outputFile,a);
			url += "/\0";
			lineArray= splitLine(url, '/');
			int i = lineArray.size();

			if(lineArray[0] == "."){
				Node *x;
				Node *y=head;
				if (i == 1){
					currentDirectory = head;
					writeOutput(outputFile,"Change directory executed successfully.");
				}
				else {
					for(int a = 1 ; a<i ; a++){
						x = findNode(y,lineArray[a],1);
						if(x) { y =x; continue;}
						else{
							y = NULL;
							writeOutput(outputFile,"Error : This directory musn't exist!");
							break;
						}
					}
					if(y){
						currentDirectory = x;
						writeOutput(outputFile,"Change directory executed successfully.");
					}
				}
			}
			else {
				Node *x = currentDirectory;
				Node *y;
				for(int a = 0 ; a<i ; a++){
					y = findNode(x,lineArray[a],1);
					if(y) { x =y; continue;}
					else{
						x = NULL;
						writeOutput(outputFile,"Error : This directory musn't exist!");
						break;
					}
				}
				if(x){
					currentDirectory = y;
					writeOutput(outputFile,"Change directory executed successfully.");
				}
			}
			writeOutput(outputFile,"\n--------------------\n");
		} // end cd command

		if (commmand == "crtfl"){ // create file command
			iss>>splitVariable ;
			string url = splitVariable;
			string a ="Create File " + url;
			writeOutput(outputFile,a);
			url += "/\0";
			lineArray= splitLine(url, '/');
			int i = lineArray.size();

			if(lineArray[0] == "."){
				Node *fileCont;
				Node *y = head;
				Node *z;
				bool cont = true;

				for (int a= 1 ;a<i;a++){
					if(a== i-1){
						fileCont= findNode(y,lineArray[a],2);
						if (fileCont){
							writeOutput(outputFile,"Error : This directory has the same name file! ");
							cont = false;
							break;
						}
						break;
					}
					z= findNode(y,lineArray[a],1);
					if(z) { y=z; continue;}
					else {
						y = NULL;
						writeOutput(outputFile,"Error : Directory to create the file does not exist.");
						break;
					}
				}
				if(y && cont){
						writeOutput(outputFile,"Create file executed successfully.");
						crtfl(y,lineArray[i-1]);
				}
			} // end start .
			else {
				Node *fileCont;
				Node *y = currentDirectory;
				Node *z;
				bool cont = true;

				for (int a= 0 ;a<i;a++){
					if(a== i-1){
						fileCont= findNode(y,lineArray[a],2);
						if (fileCont){
							writeOutput(outputFile,"Error : This directory has the same name file! ");
							cont = false;
							break;
						}
						break;
					}
					z= findNode(y,lineArray[a],1);
					if(z) { y=z; continue;}
					else {
						y = NULL;
						writeOutput(outputFile,"Error : Directory to create the file does not exist.");
						break;
					}
				}
				if(y && cont){
						writeOutput(outputFile,"Create file executed successfully.");
						crtfl(y,lineArray[i-1]);
				}
			}
			writeOutput(outputFile,"\n--------------------\n");
		} // end create file command

		if (commmand == "cp"){ // copy commands
			iss>>splitVariable ;
			string copyFor = splitVariable;

			if(copyFor == "-f"){ // copy file command
				iss>>splitVariable;
				string url1 = splitVariable;
				iss>>splitVariable;
				string url2 = splitVariable;
				string a ="Copy File " + url1+" "+url2;
				writeOutput(outputFile,a);
				url1 += "/\0";
				lineArray= splitLine(url1, '/');
				int i = lineArray.size();
				bool cont = false;
				Node *copyFile;
				Node *x;
				Node *y=head;
				Node *fileCont;

				if(lineArray[0] == "."){ // copy file destination start .
					if (i == 1){

					}
					else {
						for (int a=1 ; a<i ; a++){
							if (a == i-1){
								fileCont= findNode(y,lineArray[a],2);
								if (fileCont){
									cont = true;
									break;
								}
								else {
									writeOutput(outputFile,"Error : File doesnt find in source directory");
									break;
								}
							}
							x = findNode(y,lineArray[a],1);
							if(x) { y=x; continue; }
							else {
								y = NULL;
								writeOutput(outputFile,"Error : Directory to source doesnt exist.");
								break;
							}
						}
						if (cont && y){
							url2 += "/\0";
							lineArray= splitLine(url2, '/');
							int j = lineArray.size();
							Node *x,*control;
							Node *y = head;

							if(lineArray[0] == "."){
								if(j == 1){
									writeOutput(outputFile,"The destination url should contain the file name!");
								}
								else if(j == 2){
									control = findNode(y,lineArray[1],2);
									if(control) {
										writeOutput(outputFile,"Error : Destination directory has the same name file!");
									}
									else {
										crtfl(y,lineArray[1]);
										writeOutput(outputFile,"Copy file executed successfully.");
									}
								}
								else {
									Node *cont2;
									Node *cont3;
									for (int k =1;k<j ;k++){
										if (k == j-1){
											cont2 = findNode(y,lineArray[j-1],2);
											if (cont2){
												writeOutput(outputFile,"Error : Destination directory has the same name file!");
												break;
											}
											else {
												crtfl(y,lineArray[j-1]);
												writeOutput(outputFile,"Copy file executed successfully.");
											}
											break;
										}
										control = findNode(y,lineArray[k],1);
										if(control){ y=control; continue; }
										else{
											for (int s= k; s<j-1 ; s++){ // add directory
												mkdir(y,lineArray[s]);
												control = findNode(y,lineArray[s],1);
											    y=control;
											    AtomicNode *a = (AtomicNode *) y;
											}
											cont3 = findNode(y,lineArray[j-1],2);
											if(!cont3){
												crtfl(y,lineArray[j-1]);
												writeOutput(outputFile,"Copy file executed successfully.");
											}
											else {
												writeOutput(outputFile,"Error : Destination directory has the same name file!");
											}
										}
										break;
									}
								}
							} // end destination url start .
							else{ // if destination url not start .
									y=currentDirectory;
									Node *cont2;
									Node *cont3;
									Node *cont4;
									if(j == 1){
										cont4= findNode(y,lineArray[0],2);
										if (cont4){
											writeOutput(outputFile,"Error : Destination directory has the same name file!");
										}
										else {
											crtfl(y,lineArray[0]);
											writeOutput(outputFile,"Copy file executed successfully.");
										}
									}
									else{
										for (int k =0;k<j ;k++){
											if (k == j-1){
												cont2 = findNode(y,lineArray[j-1],2);
												if (cont2){
													writeOutput(outputFile,"Error : Destination directory has the same name file!");
													break;
												}
												else {
													crtfl(y,lineArray[j-1]);
													writeOutput(outputFile,"Copy file executed successfully.");
												}
												break;
											}
											control = findNode(y,lineArray[k],1);
											if(control){ y=control; continue;}
											else{
												for (int s= k; s<j-1 ; s++){ // add directory
													mkdir(y,lineArray[s]);
													control = findNode(y,lineArray[s],1);
													y=control;
												}
												cont3 = findNode(y,lineArray[j-1],2);
												if(!cont3){
													crtfl(y,lineArray[j-1]);
													writeOutput(outputFile,"Copy file executed successfully.");
												}
												else {
													writeOutput(outputFile,"Error : Destination directory has the same name file!");
												}
											}
											break;
										}
									}
								}
							} // end destination url not start .
						}
				} // end copy file source start .
				else{ //copy file source url not start .
					Node *y = currentDirectory;
					Node *controll;
					if (i == 0){
							writeOutput(outputFile,"Error : File doesnt find in source directory");
					}
					else {
						for (int a=0 ; a<i ; a++){
							if (a == i-1){
								fileCont= findNode(y,lineArray[a],2);
								if (fileCont){
									cont = true;
									break;
								}
								else {
									writeOutput(outputFile,"Error : File doesnt find in source directory.");
									break;
								}
							}
							x = findNode(y,lineArray[a],1);
							if(x) { y=x; continue; }
							else {
								y = NULL;
								writeOutput(outputFile,"Error : Directory to source doesnt exist.");
								break;
							}
						}
						if (cont && y){
							iss>>splitVariable;
							string url2 = splitVariable;
							url2 += "/\0";
							lineArray= splitLine(url2, '/');
							int j = lineArray.size();
							Node *control;
							Node *y = head;

							if(lineArray[0] == "."){
								if(j == 1){
									writeOutput(outputFile,"Error :The destination url should contain the file name!");
								}
								else if(j == 2){
									control = findNode(y,lineArray[1],2);
									if(control) {
										writeOutput(outputFile,"Error : Destination directory has the same name file!");
									}
									else {
										crtfl(y,lineArray[1]);
										writeOutput(outputFile,"Copy file executed successfully.");
									}
								}
								else {
									Node *cont2;
									Node *cont3;
									for (int k =1;k<j ;k++){
										if (k == j-1){
											cont2 = findNode(y,lineArray[j-1],2);
											if (cont2){
												writeOutput(outputFile,"Error : Destination directory has the same name file!");
												break;
											}
											else {
												crtfl(y,lineArray[j-1]);
												writeOutput(outputFile,"Copy file executed successfully.");
											}
											break;
										}
										control = findNode(y,lineArray[k],1);
										if(control){ y=control; continue; }
										else{
											for (int s= k; s<j-1 ; s++){ // add directory
												mkdir(y,lineArray[s]);
												control = findNode(y,lineArray[s],1);
											    y=control;
											}
											cont3 = findNode(y,lineArray[j-1],2);
											if(!cont3){
												crtfl(y,lineArray[j-1]);
												writeOutput(outputFile,"Copy file executed successfully.");
											}
											else {
												writeOutput(outputFile,"Error : Destination directory has the same name file!");
											}
										}
										break;
									}
								}
							} // end destination url start .
							else{ // if destination url not start . - source not start .
									y=currentDirectory;
									Node *cont2;
									Node *cont3;
									Node *cont4;
									if(j == 1){
										cont4= findNode(y,lineArray[0],2);
										if (cont4){
											writeOutput(outputFile,"Error : Destination directory has the same name file!");
										}
										else {
											crtfl(y,lineArray[0]);
											writeOutput(outputFile,"Copy file executed successfully.");
										}
									}
									else{
										for (int k =0;k<j ;k++){
											if (k == j-1){
												cont2 = findNode(y,lineArray[j-1],2);
												if (cont2){
													writeOutput(outputFile,"Error : Destination directory has the same name file!");
													break;
												}
												else {
													crtfl(y,lineArray[j-1]);
													writeOutput(outputFile,"Copy file executed successfully.");
												}
												break;
											}
											control = findNode(y,lineArray[k],1);
											if(control){ y=control; continue;}
											else{
												for (int s= k; s<j-1 ; s++){ // add directory
													mkdir(y,lineArray[s]);
													control = findNode(y,lineArray[s],1);
													y=control;
												}
												cont3 = findNode(y,lineArray[j-1],2);
												if(!cont3){
													crtfl(y,lineArray[j-1]);
													writeOutput(outputFile,"Copy file executed successfully.");
												}
												else {
													writeOutput(outputFile,"Error : Destination directory has the same name file!");
												}
											}
											break;
										}
									}
								}
							} // end destination url not start .
						}
				} // end copy file source not start .
				writeOutput(outputFile,"\n--------------------\n");
			} // end copy file command

			else if ( copyFor == "-d"){ // copy directory command
				iss>>splitVariable ;
				string url = splitVariable;
				iss>>splitVariable ;
				string url2 = splitVariable;
				string a="Copy directory "+url+" "+url2;
				writeOutput(outputFile,a);
				url += "/\0";
				lineArray= splitLine(url, '/');
				int i = lineArray.size();
				Node *copyingList;
				Node *control;
				Node *x =head;
				bool directory = true;

				if (lineArray [0] == "."){ // source start .
					if ( i == 1 ){
						copyingList = head;
					}
					else {
						for (int a=1;a<i;a++){
							control = findNode(x,lineArray[a],1);
							if (control){x=control; directory = true; continue;}
							else{ directory = false; break;}
						}
						if (!directory){
							writeOutput(outputFile,"Error : Directory to source doesnt exist!");
						}
						else { // source directory okey
							copyingList = x; // kopyalanacak liste forsubnode eklenip baglanacak
							url2 += "/\0";
							lineArray= splitLine(url2, '/');
							int i = lineArray.size();

							if (lineArray[0] == "."){ // source and destination url start .
								Node *control;
								Node *t =head;
								Node *y;

								if (i == 1){ // copy to main url.
									copyDirectory(t,copyingList);
									writeOutput(outputFile,"Copy directory executed successfully.");
								}
								else {
									for(int a=1;a<i;a++){
										control = findNode(t,lineArray[a],1);
										if (control) {t=control; continue;}
										else {
											for (int c=a ;c<i;c++){
												mkdir(t,lineArray[c]);
												control = findNode(t,lineArray[c],1);
												t=control;
											}
										}
										break;
									}
									copyDirectory(t,copyingList);
									writeOutput(outputFile,"Copy directory executed successfully.");
								}
							} //source and destination url start .
							else { // source start . destination not
								Node *control;
							    Node *t =head;
								Node *y;
								for(int a=0;a<i;a++){
									control = findNode(t,lineArray[a],1);
									if (control) {t=control; continue;}
									else {
										for (int c=a ;c<i;c++){
											mkdir(t,lineArray[c]);
											control = findNode(t,lineArray[c],1);
											t=control;
										}
									}
									break;
								}
								copyDirectory(t,copyingList);
								writeOutput(outputFile,"Copy directory executed successfully.");

							}// end source start . destination not
						}
					}
				}
				else{ // source not start .
					Node *x = currentDirectory;
						for (int a=0;a<i;a++){
							control = findNode(x,lineArray[a],1);
							if (control){x=control; directory = true; continue;}
							else{ directory = false; break;}
						}
						if (!directory){
							writeOutput(outputFile,"Error : Directory to source doesnt exist!");
						}
						else { // source directory okey
							copyingList = x; // kopyalanacak liste forsubnode eklenip baglanacak
							iss>>splitVariable ;
							string url2 = splitVariable;
							url2 += "/\0";
							lineArray= splitLine(url2, '/');
							int i = lineArray.size();

							if (lineArray[0] == "."){ // source not and destination url start .
								Node *control;
								Node *t =head;

								if (i == 1){ // copy to main url.
									copyDirectory(t,copyingList);
									writeOutput(outputFile,"Copy directory executed successfully.");
								}
								else {
									for(int a=1;a<i;a++){
										control = findNode(t,lineArray[a],1);
										if (control) {t=control; continue;}
										else {
											for (int c=a ;c<i;c++){
												mkdir(t,lineArray[c]);
												control = findNode(t,lineArray[c],1);
												t=control;
											}
										}
										break;
									}
									copyDirectory(t,copyingList);
									writeOutput(outputFile,"Copy directory executed successfully.");
								}
							} //source and destination url start .
							else { // source and destination not start .
								Node *control;
							    Node *t =head;
								Node *y;
								for(int a=0;a<i;a++){
									control = findNode(t,lineArray[a],1);
									if (control) {t=control; continue;}
									else {
										for (int c=a ;c<i;c++){
											mkdir(t,lineArray[c]);
											control = findNode(t,lineArray[c],1);
											t=control;
										}
									}
									break;
								}
								copyDirectory(t,copyingList);
								writeOutput(outputFile,"Copy directory executed successfully.");

							}// end source and  destination not start .
						}
				} // end destination not start .
				writeOutput(outputFile,"\n--------------------\n");
			} // end copy directory command
		} // end copy commands

		if (commmand == "rm"){ // create file command
			iss>>splitVariable ;
			string command = splitVariable;

			if (command == "-f"){ // remove file command
				iss>>splitVariable;
				string url = splitVariable;
				string a = "Remove File "+url;
				writeOutput(outputFile,a);
				url += "/\0";
				lineArray= splitLine(url, '/');
				int i = lineArray.size();

				if (lineArray[0] == "."){ // url start .
					Node *x = head;
					Node *y;
					bool directory=true,file=false;
					for (int a=1 ; a<i ; a++){
						if(a == i-1){
							y= findNode(x,lineArray[a],2);
							if(y) {file=true; break;}
							else {
								writeOutput(outputFile,"Error : This file doesnt exits this directory"); break;
							}
						}
						y = findNode(x,lineArray[a],1);
						if(y){ x=y; continue;}
						else {
							directory = false;
							writeOutput(outputFile,"Error : Given URL is not a file."); break;
						}
					}
					if (directory && file){
						removeFile(x,lineArray[i-1]);
						writeOutput(outputFile,"Remove file executed successfully.");
					}
				} // end url start .
				else { // url not start .
					Node *x = currentDirectory;
					Node *y;
					bool directory=true,file=false;
					for (int a=0 ; a<i ; a++){
						if(a == i-1){
							y= findNode(x,lineArray[a],2);
							if(y) {file=true; break;}
							else {
								writeOutput(outputFile,"Error : This file doesnt exits this directory"); break;
							}
						}
						y = findNode(x,lineArray[a],1);
						if(y){ x=y; continue;}
						else {
							directory = false;
							writeOutput(outputFile,"Error : Given URL is not a file."); break;
						}
					}
					if (directory && file){
						removeFile(x,lineArray[i-1]);
						writeOutput(outputFile,"Remove file executed successfully.");
					}
				} // end url not start .
				writeOutput(outputFile,"\n--------------------\n");
			}// end remove file command
			else if (command == "-d"){ // remove directory command
				iss>>splitVariable;
				string url = splitVariable;
				string a = "Remove Directory "+url;
				writeOutput(outputFile,a);
				url += "/\0";
				lineArray= splitLine(url, '/');
				int i = lineArray.size();
				bool directory =true;
				if (lineArray[0] == "-d")
					writeOutput(outputFile,"Error : The root directory cannot be removed.");
				else if (lineArray[0] == "."){
					Node *x;
					Node *y =head;
					if (i == 1){
						writeOutput(outputFile,"Error : The root directory cannot be removed.");
					}
					else {
						for (int a=1; a<i ;a++){
							x= findNode(y,lineArray[a],1);
							if(x){ y=x; continue;}
							else {directory = false; break;}
						}
						if(!directory)
							writeOutput(outputFile,"Error : This directory doesnt exist.");
						else {
							y = head;
							for(int b=1 ;b<i-1;b++){
								x = findNode(y,lineArray[b],1);
								y=x;
							}
							removeDirectory(y,lineArray[i-1]);
							writeOutput(outputFile,"Remove directory executed successfully.");
						}
					}
				} // end url start .
				else{ // url dont start .
					Node *x;
					Node *y =currentDirectory;
					if (i == 0){
						writeOutput(outputFile,"Error : The root directory cannot be removed.");
					}
					else {
						for (int a=0; a<i ;a++){
							x= findNode(y,lineArray[a],1);
							if(x){ y=x; continue;}
							else {directory = false; break;}
						}
						if(!directory)
							writeOutput(outputFile,"Error : This directory doesnt exist.");
						else {
							y = head;
							for(int b=0 ;b<i-1;b++){
								x = findNode(y,lineArray[b],1);
								y=x;
							}
							removeDirectory(y,lineArray[i-1]);
							writeOutput(outputFile,"Remove directory executed successfully.");
						}
					}
				} // end url dont start .
				writeOutput(outputFile,"\n--------------------\n");
			} // end remove directory command
		} //end remove command
	}// end while
 return 1;
} // end main

