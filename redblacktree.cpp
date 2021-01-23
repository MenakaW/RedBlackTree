
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
void fix3(Node* &current, Node* &root);
void fix4(Node* current, Node* &root);
void fix4s2(Node* current, Node* &root);
void rotateLeft(Node* &n, Node* &root);
void rotateRight(Node* &n, Node* &root);
Node* getSibling(Node* n);
Node* searchNodes(bool &found, int num, Node* current);
Node* findDelete(Node* current);
void deleteNode(Node* toDelete, Node* &root);
void delete1(Node* current, Node* &root);
void delete2(Node* current, Node* &root);
void delete3(Node* current, Node* &root);
void delete4(Node* current, Node* &root);
void delete5(Node* current, Node* &root);
  
int main(){

  Node* root = nullptr;
  int inputNums[101];
  int nums = 0;
  //user input loop
  while(true){
    char input[20];
    cout << "type add, read, print, search, delete, or quit" << endl;
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
      cout << "wb here" << endl;
      insert(n, root, root);
      cout << "here?" << endl;
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
    //search
    else if(input[0] == 's' || input[0] == 'S'){
      int num;
      cout << "please enter the number you are searching for" << endl;
      cin >> num;
      cin.get();
      bool found = false;
      //use the bool found to keep track of whether or not you've found the node within the tree
      searchNodes(found, num, root);
      if(found){
	cout << "yes, this tree contains this node" << endl;
      }
      else{
	cout << "no, this tree does not contain this node" << endl;
      }
    }
    else if(input[0] == 'd' || input[0] == 'D'){
      int num;
      cout << "please enter the number you want to delete" << endl;
      cin >> num;
      cin.get();
      bool found = false;
      Node* foundNode;
      //first check if the node is inside the tree
      foundNode = searchNodes(found, num, root);
      if(found){
	//we will be going all the way to the right in the left subtree to replace, so pass the left node of the one u want to delete
	Node* toPass = foundNode->left;
	//use find delete to figure out the node you are going to swap values with
	Node* m = findDelete(toPass);
	//swap values
	foundNode->value = m->value;
	//delete the node u swapped values with
	deleteNode(m, root);
      }
      //if the node wasn't found, then let the user know
      else{
	cout << "this tree does not contain this node" << endl;
      }
    }
    //reinforce that the roots parent must be nullptr
    if(root!=nullptr){
      root->parent = nullptr;
    }
  }
}

//this replaces the toDelete node with its child
void deleteNode(Node* toDelete, Node* &root){
  Node* child;
  //if the node has a non-leaf child, use that one as a replacement, otherwise, use the left child
  if(!isLeaf(toDelete->left)){
    child = toDelete->left;
  }
  else if(!isLeaf(toDelete->right)){
    child = toDelete->right;
  }
  else {
    child = toDelete->left;
  }
  //make child's parent, the current node's parent
  child->parent = toDelete->parent;
  //unless the currento node doesn't have a parent, assign the parent's left or right pointers to be child
  if(toDelete->parent != nullptr){
    if(toDelete == toDelete->parent->left){
      toDelete->parent->left = child;
    }
    else if(toDelete == toDelete->parent->right){
      toDelete->parent->right = child;
    }
  }
  //if the current node doesn't have a parent, that means it's the root, so reassign the root
  else{
    root = child;
  }

  //if node is red and replacement node is black ur good
  if(toDelete->c == RED && child->c == BLACK){
    
  }
  //if node is black and replacement node is red, just repaint the replacement
  else if(toDelete->c == BLACK && child->c == RED){
    child->c = BLACK;
  }
  //if node is black and replacement node is black too, we have to check for different cases
  else if(toDelete->c == BLACK && child->c == BLACK){
    //if the node/replacement node is root, ur good, but otherwise check for cases
    if(child->parent!=nullptr){
      delete1(child, root);
    }
    //if replacement node is a leaf, and it's going to be the root, make it a nullptr for ease of code
    else if(isLeaf(child) && child==root){
      root = nullptr;
    }
  }
}

//first delete case
void delete1(Node* current, Node* &root){
  if(getSibling(current)!=nullptr){
    //if sibling is red, swap the colors of sibling and parent and rotate right/left at parent
    //depending on whether sibling is initially on the left/right of parent 
    Node* s = getSibling(current);
    if(s->c == RED){
      enum color temp = s->c;
      s->c = s->parent->c;
      s->parent->c = temp;
      if(s==s->parent->right){
	rotateLeft(s->parent, root);
	}
      else if(s == s->parent->left){
	rotateRight(s->parent, root);
	}
    }
    //once done, proceed to case 2
    delete2(current, root);
  }
}

//case 2
void delete2(Node* current, Node* &root){
  Node* parent = current->parent;
  if(parent!=nullptr){
    Node* sibling = getSibling(current);
    if(sibling!=nullptr){
      Node* c1 = sibling->left;
      Node* c2 = sibling->right;
      //if node's parent, sibling, and sibling's children are all black
      //then repaint sibling red and re call delete 1 on parent to rebalance
      if(c1!=nullptr && c2!=nullptr){
	if(parent->c==BLACK && sibling->c == BLACK && c1->c == BLACK && c2->c == BLACK){
	  sibling->c = RED;
	  delete1(parent, root);
	}
	//if not, move on to case 3
	else{
	  delete3(current, root);
	}
      }
    }
  }
}

//case 3
void delete3(Node* current, Node* &root){
  Node* parent = current->parent;
  if(parent!=nullptr){
    Node* sibling = getSibling(current);
    if(sibling!=nullptr){
      Node* c1 = sibling->left;
      Node* c2 = sibling->right;
      //if parent is red, but sibling and sibling's children are black then repaint parent black and sibling red
      if(c1!=nullptr && c2!=nullptr){
	if(parent->c==RED && sibling->c==BLACK && c1->c==BLACK && c2->c==BLACK){
	  parent->c = BLACK;
	  sibling->c = RED;
	}
	//if not, move on to case 4
	else{
	  delete4(current, root);
	}
      }
    }
  }
}
//case 4 
void delete4(Node* current, Node* &root){
  Node* sibling = getSibling(current);
  if(sibling!=nullptr){
    Node* c1 = sibling->left;
    Node* c2 = sibling->right;
    //depending on which side sibling is on, when one of sibling's children is red and the other is black, rotate right or left
    //and then swap the color of sibling and the red node
    if(c1!=nullptr && c2!=nullptr){
      if(sibling == sibling->parent->right && c1->c == RED && c2->c == BLACK){
	rotateRight(sibling, root);
	enum color temp = c1->c;
	c1->c = sibling->c;
	sibling->c = temp;
	}
      else if(sibling == sibling->parent->left && c2->c == RED && c1->c == BLACK){
	rotateLeft(sibling, root);
	enum color temp = c2->c;
	c2->c = sibling->c;
	sibling->c = temp;
	}
    }
  }
  //proeceed to case 5
  delete5(current, root);
}

//case 5
void delete5(Node* current, Node* &root){
  Node* parent = current->parent;
  if(parent!=nullptr){
    Node* sibling = getSibling(current);
    if(sibling!=nullptr){
      Node* c1 = sibling->left;
      Node* c2 = sibling->right;
      //depending on which side sibling is, rotate left or right if sibling is black and one of it's children is red
      //then switch the color of parent and sibling, and make the red node black
      if(c1!=nullptr && c2!=nullptr){
	if(current == parent->left && sibling->c == BLACK && c2->c==RED){
	  rotateLeft(parent, root);
	  enum color temp = parent->c;
	  parent->c = sibling->c;
	  sibling->c = temp;
	  c2->c = BLACK;
	}
	else if(current == parent->right && sibling->c == BLACK && c1->c == RED){
	  rotateRight(parent, root);
	  enum color temp = parent->c;
	  parent->c = sibling->c;
	  sibling->c = temp;
	  c1->c = BLACK;
	}
      }
    }
  }
}

//keep going right until you hit a leaf, and then return the leaf's parent
Node* findDelete(Node* current){
  Node* toReturn = nullptr;
  if(isLeaf(current)){
    toReturn = current->parent;
  }
  else{
    toReturn = findDelete(current->right);
  }
  return toReturn;
}

//traverse throught the nodes like you would in add, and change the boolean to true if you find a node with the matching value
//return the Node through the recurses
Node* searchNodes(bool &found, int num, Node* current){
  Node* toReturn = nullptr;
  if(current!=nullptr){
    if(current->value == num){
      found = true;
      toReturn = current;
    }
    else if(num < current->value){
      toReturn = searchNodes(found, num, current->left);
    }
    else if(num > current->value){
      toReturn = searchNodes(found, num, current->right);
    }
  }
  return toReturn;
}


//basically rotating the right child of n into n's place and readjusting the pointers to support the adjustment
void rotateLeft(Node* &n, Node* & root){
  if(!isLeaf(n->right)){
    Node* nnew = n->right;
    Node* p = n->parent;
    Node* temp = n;
    nnew->parent = p;
    n = temp;
    Node* transfer = nnew->left;
    nnew->left = n;
    n->right = transfer;
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
    Node* temp = n;
    nnew->parent = p;
    n = temp;
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
void fix3(Node* &current, Node* &root){
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
  if(root==nullptr){
  }
  else if(!isLeaf(root)){
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
