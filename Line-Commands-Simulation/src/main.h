#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
using namespace std;

class Node{
protected:
	Node *next;
	string type;
public:
	void setNext(Node*);
	string getType();
	Node *getNext();
};
class AtomicNode : public Node{
public:
	AtomicNode();
	void setName(string);
	string getName();
protected:
	string name;
};


class ForSubNode : public Node{
public:
	ForSubNode();
	AtomicNode* getSub();
	void setSub(AtomicNode*);
protected:
	AtomicNode *subBag;
};



#endif
