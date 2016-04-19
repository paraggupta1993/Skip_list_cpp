#include <stdio.h>
#include <iostream>
#include <time.h>   /* time() */
#include <stdlib.h> /* srand() , rand() */
#include <string.h>

using namespace std;
#define PROB 50

struct Node;
struct SkipList;
	
struct Node {
	string key;
	int value;
	Node **forward; 
};

struct SkipList{
	Node *header;
	int maxlevel; 
	int level; 
};

Node *make_node(string key, int value, int levels) {
	Node *x = new Node;
	x->key = key;
	x->value = value;
	x->forward = new Node*[levels+1];
	for(int i = 0; i <= levels; i++) x->forward[i] = NULL;
	return x;
}


void init_skip_list(SkipList *s) { 
	string head = "A";
	s->maxlevel = 16;
	s->header = make_node(head, 1, s->maxlevel);
	s->level = 0;
}

void delete_node(Node* x) {
	delete[] x->forward;
	delete x;
}

int randomLevel(int Maxlevel){
	int l = 0;
	while((rand() % 100) < PROB && l + 1 < Maxlevel) l++;
	return l;
}

void insert_into_skip_list(SkipList* s, string key, int val) {

	Node *x = s->header;
	
	Node **lastNodeAtLevel = new Node *[s->maxlevel+1];
	
	for(int i = s->level; i >= 0; i--){
		while(x->forward[i] != NULL && x->forward[i]->key < key ) 
			x = x->forward[i];
		
		if (x->forward[i] != NULL && x->forward[i]->key == key) {
			x->forward[i]->value = val;
			delete[] lastNodeAtLevel;
			return;
		}

		lastNodeAtLevel[i] = x;
	}

  // Didn't find a node 
	// Have to make one
	
	int nodeLevel = randomLevel(s->maxlevel);
	x = make_node(key, val, nodeLevel); 
	
	if (s->level < nodeLevel) {
	  for(int i = s->level+1; i<=nodeLevel;i++)
			lastNodeAtLevel[i] = s->header;
		s->level = nodeLevel;
	}

	for(int i = 0; i <= nodeLevel; i++) {
		x->forward[i] = lastNodeAtLevel[i]->forward[i];
		lastNodeAtLevel[i]->forward[i] = x;
	}

	delete[] lastNodeAtLevel;
}

bool delete_from_skip_list(SkipList *s, string key) {
	Node *x = s->header;
	int i = 0;

	for(i = s->level; i >=0; i--){
		while(x->forward[i] != NULL && x->forward[i]->key < key)
			x = x->forward[i];
		
		if (x->forward[i] != NULL && x->forward[i]->key == key)
			break;
	}
	
	if(i == -1) {
		cout << "Error: " <<  key << " not Found" << endl;
		return false;
	}
		
	Node *victim = x->forward[i];

	while(i>=0) 
			x->forward[i] = victim->forward[i--];

	delete_node(victim);
	cout << key << " deleted Successfully" << endl;
	return true;
}

void delete_skip_list(SkipList *s) {
	Node *x = s->header;
	Node *next;
	while(x != NULL) {
		next = x->forward[0];
    delete_node(x);
		x = next;
	}
	delete s;
}

int search_in_skip_list(SkipList *s, string key) {
	Node *x = s->header;
	for(int i = s->level; i >= 0; i--){
		while(x->forward[i] != NULL && x->forward[i]->key < key)
			x = x->forward[i];
		
		if(x->forward[i] != NULL && x->forward[i]->key == key)
			return x->forward[i]->value;
	}
	return -1;
}

void print_skip_list(SkipList *s){
	cout << "####### Current List ########" << endl;
	Node *x  = s->header;
	x = x->forward[0];
	while(x != NULL) {
		cout << x->key << " => " << x->value << endl;
		x = x->forward[0];
	}
	cout << "####### End-of-List ########" << endl;
}

int main(){
	srand(time(NULL));
	SkipList *s = new SkipList();
	init_skip_list(s);
  insert_into_skip_list(s, "k1", 1);
  insert_into_skip_list(s, "k2", 20);
  insert_into_skip_list(s, "k3", 3);
  insert_into_skip_list(s, "k3", 5);
  insert_into_skip_list(s, "k4", 40);
	print_skip_list(s);
	
	int val; 
	cout << "Searching k1: " << search_in_skip_list(s, "k1") << endl;
	cout << "Searching k3: " << search_in_skip_list(s, "k3") << endl;
	cout << "Searching k6: " << search_in_skip_list(s, "k6") << endl;
	cout << "Searching k2: " << search_in_skip_list(s, "k2") << endl;
	delete_from_skip_list(s, "k6");
	print_skip_list(s);
	delete_from_skip_list(s, "k10");
	print_skip_list(s);
	delete_from_skip_list(s, "k1");
	print_skip_list(s);
	delete_from_skip_list(s, "k1");
	print_skip_list(s);
	delete_from_skip_list(s, "k2");
	print_skip_list(s);
  delete_skip_list(s);
	return 0;
}
