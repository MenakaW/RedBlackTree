#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

//setting up enum for color
enum color{
  RED,BLACK
};

//setting up node struct
struct Node{
  Node* left;
  Node* right;
  Node* parent;
  int value;
  enum color c;
};

//function prototypes
void insert(Node* &n, Node* &root, Node* &current);
Node* makeNewLeaf(Node* parent);
bool isLeaf(Node* current);
void printTree(Node* root, int space);
void fixInsert(Node* &root, Node* &current);
void fix1(Node* &current);
void fix3(Node* &current, Node* root);
void fix4(Node* current, Node* &root);
void fix4s2(Node* current, Node* &root);
void rotateLeft(Node* &n, Node* &root);
void rotateRight(Node* &n, Node* &root);
Node* getSibling(Node* n);

int main(){

  Node* root = nullptr;
  int inputNums[101];
  int nums = 0;
  //user input loop
  while(true){
    char input[20];
    cout << "type add, read, print, or quit" << endl;
    cin >> input;
    cin.get();
    //if add
    if(input[0] == 'a' || input[0] == 'A'){
      cout << "what number would you like to add" << endl;
      int num;
      cin >> num;
      cin.get();
      //create the node with the input value, and create two leaf nodes to go under it (leaves are black and new node is red)
      Node* n = new Node;
      Node* leaf1 = new Node;
      Node* leaf2 = new Node;
      leaf1->parent = n;
      leaf2->parent = n;
      leaf1->value = -1;
      leaf2->value = -1;
      leaf1->c = BLACK;
      leaf2->c = BLACK;
      n->value = num;
      n->parent = nullptr;
      n->left = leaf1;
      n->right = leaf2;
      n->c = RED;
      //call insert and then fix insert
      insert(n, root, root);
      fixInsert(root, n);
    }
    else if(input[0] == 'r' || input[0] == 'R'){
      ifstream file;
      char filename[20];
      cout << "enter file name correctly" << endl;
      cin >> filename;
      cin.get();
      file.open(filename);
      for(int a = 1; a < 101; a++){
	if(file.eof()){
	  break;
	}
	int in;
	file >> in;
	inputNums[a] = in;
	nums++;
      }
      inputNums[nums]=0;
      nums--;
      file.close();
      for(int a = 1; a<=nums; a++){
	Node* n = new Node;
	Node* leaf1 = new Node;
	Node* leaf2 = new Node;
	leaf1->parent = n;
	leaf2->parent = n;
	leaf1->value = -1;
	leaf2->value = -1;
	leaf1->c = BLACK;
	leaf2->c = BLACK;
	n->value = inputNums[a];
	n->parent = nullptr;
	n->left = leaf1;
	n->right = leaf2;
	n->c = RED;
	//call insert and then fix insert
	insert(n, root, root);
	fixInsert(root, n);
      }
    }
    //print
    else if(input[0] == 'p' || input[0] == 'P'){
      printTree(root, -10);
    }
    //quit
    else if(input[0] == 'q' || input[0] == 'Q'){
      break;
    }
  }

}

//basically rotating the right child of n into n's place and readjusting the pointers to support the adjustment
void rotateLeft(Node* &n, Node* & root){
  if(!isLeaf(n->right)){
    Node* nnew = n->right;
    Node* p = n->parent;
    nnew->parent = p;
    Node* transfer = nnew->left;
    nnew->left = n;
    n->right = transfer;
    transfer->parent = n;
    n->parent = nnew;
    //if n isn't the root, then adjust the parent left/right pointers to point to it's new child
    if(p!=nullptr){
      if(p->left==n){
	p->left = nnew;
      }
      if(p->right==n){
	p->right = nnew;
      }
    }
    //if n used to be the root, then change the root to the new root
    else{
      root = nnew;
    }
  }
}

//same algorithm as rotate left except left and right are swapped
void rotateRight(Node* &n, Node* &root){
  if(!isLeaf(n->left)){
    Node* nnew = n->left;
    Node* p = n->parent;
    nnew->parent = p;
    Node* transfer = nnew->right;
    nnew->right = n;
    n->left = transfer;
    transfer->parent = n;
    n->parent = nnew;
    if(p!=nullptr){
      if(p->left==n){
	p->left = nnew;
      }
      if(p->right==n){
	p->right = nnew;
      }
    }
    else{
      root = nnew;
    }
  }
}

//given a node n, make sure that they have a parent bcs no parent means no sibling, then find the sibling
Node* getSibling(Node* n){
  if(n->parent!=nullptr){
    if(n==n->parent->left){
      return n->parent->right;
    }
    else if (n==n->parent->right){
      return n->parent->left;
    }
  }
  return nullptr;
}

//change color to black if node is root
void fix1(Node* &current){
  current->c = BLACK;
}

//if uncle and parent are red, change them both to black and paint grandparent red to even out num of blacks
//however, the grandparent could be root, so call the fix on the grandparent
void fix3(Node* &current, Node* root){
  if(current->parent->parent != nullptr){
    current->parent->parent->c = RED;
    current->parent->c = BLACK;
    getSibling(current->parent)->c = BLACK;
    fixInsert(root, current->parent->parent);
  }
}

//first step for if parent is red but uncle is black
void fix4(Node* current, Node* &root){
  //you want to rotate node current out of the inside of the tree(left child of right child of g or right child of left child of g),
  //and move it into the outside (right child of right child of g or left child of left child of g)
  if(current->parent!=nullptr && current->parent->parent!=nullptr){
    Node* parent = current->parent;
    Node* grandparent = current->parent->parent;
    if(grandparent->left->right==current){
      rotateLeft(parent, root);
      //once you rotate the current node out of the insde, pass its new child to the second step of case 4
      fix4s2(current->left, root);
    }
    else if(grandparent->right->left == current){
      rotateRight(parent, root);
      fix4s2(current->right, root);
    }
    else{
      //if the current node wasn't on the inside, proceed directly to step 2
      fix4s2(current, root);
    }
  }
}

void fix4s2(Node* current, Node* &root){
  Node* g = current->parent->parent;
  //we know that node current is on the outside, so now rotate the grandparent in the opposite direction as the current node
  //then swap the color of the old child and the grandparent
  if(g->left->left==current){
    rotateRight(g, root);
    enum color temp = g->parent->c;
    g->parent->c = g->c;
    g->c = temp;
  }
  else if(g->right->right==current){
    rotateLeft(g, root);
    enum color temp = g->parent->c;
    g->parent->c = g->c;
    g->c = temp;
  }
}

//fixes all 4 cases
void fixInsert(Node* &root, Node* &current){
  //if it's the root
  if(current==root){
    fix1(current);
  }
  //if the parent is black do nothing bcs thats fine
  if(current->parent != nullptr){
    if( current->parent->c == BLACK){
    }
  }
  //if the uncle and the parent are red
  if(current->parent != nullptr && current->parent->c == RED && getSibling(current->parent)->c == RED){
    fix3(current, root);
  }
  //if the parent is red and the uncle is black
  if(current->parent!=nullptr && current->parent->c == RED && getSibling(current->parent)->c == BLACK){
    fix4(current, root);
  }
}

//very similar to binary tree
void insert(Node* &n, Node* &root, Node* &current){
  //if it's the first node, make it the root
  if(root==nullptr){
    n->parent = nullptr;
    root = n;
    return;
  }
  //if it's reached a leaf, replace the leaf with the new node
  else if(isLeaf(current)){
    n->parent = current->parent;
    if(current == current->parent->left){
      current->parent->left = n;
    }
    else{
      current->parent->right = n;
    }
  }
  //if new node is less than the current, go left, else (greater than or equal to) go right
  else{
    if(n->value < current->value){
      insert(n, root, current->left);
    }
    else{
      insert(n, root, current->right);
    }
  }
}

//since normal node values are between 1 and 1000, i set all leaf values to -1
bool isLeaf(Node* current){
  if(current->value==-1){
    return true;
  }
  else{
    return false;
  }
}

//archived function, might implement later
Node* makeNewLeaf(Node* parent){
  Node* leaf = new Node;
  leaf->right = nullptr;
  leaf->left = nullptr;
  leaf->value = -1;
  leaf->parent = parent;
  leaf->c = BLACK;
  return leaf;
}

//algorithm sourced from geeks4geeks
void printTree(Node* root, int space){
  if(!isLeaf(root)){
    space+=10;
    printTree(root->right, space);
    cout << "\n";
    for(int a = 0; a < space; a++){
      cout << " ";
    }
    cout << root->value ;
    if(root->c==RED){
      cout << "(r)";
    }
    else if(root->c==BLACK){
      cout << "(b)";
    }
    cout << endl;
    printTree(root->left, space);
  }
}
