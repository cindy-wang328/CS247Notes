using namespace std ;
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
struct TreeNode {
    int val;
    int height;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    TreeNode(int x): val(x), left(nullptr), right(nullptr), height(0), parent(nullptr){}
};

void print(string prefix, TreeNode* n, bool isLeft) {
      if (n) {
          cout << (prefix + (isLeft ? "|-- " : "\\-- ") + to_string(n->val)) << endl;
          print(prefix + (isLeft ? "|   " : "    "), n->left, true);
          print(prefix + (isLeft ? "|   " : "    "), n->right, false);
      }
}

void printWithHeight(string prefix, TreeNode* n, bool isLeft) {
      if (n) {
          cout << (prefix + (isLeft ? "|-- " : "\\-- ") + to_string(n->val) + " ("+to_string(n->height)+")" +" ("+(n->parent!= nullptr ? to_string(n->parent->val) : "N")+")") << endl;
          printWithHeight(prefix + (isLeft ? "|   " : "    "), n->left, true);
          printWithHeight(prefix + (isLeft ? "|   " : "    "), n->right, false);
      }
}
void printWithHeight2(TreeNode* root){
  // prints height and parent node
  printWithHeight("", root, false);
}

bool isLeaf(TreeNode* root){
  return ((!(root->left) && !(root->right)));
}
int setHeight(TreeNode* root){
  if(!root) return -1;
  if(!(root->left) && !(root->right)) {
    root->height = 0; return 0;
  }
  root->height = 1+max(setHeight(root->left), setHeight(root->right));
  return root->height;
  
}
int h(TreeNode* t){
  if(!t) return -1;
  return t->height;
}
void shfc(TreeNode* t){
  // set height from children
  t->height = 1+max(h(t->left), h(t->right));
}
TreeNode* f(vector<int> & v, int & i, int bound){
    if(i >= size(v) || v[i] > bound) return nullptr;
    TreeNode* t =  new TreeNode(v[i]);
    i++;
    t->left = f(v, i, t->val);
    if(t->left) t->left ->parent = t;
    t->right = f(v, i, bound);
    if(t->right) t->right->parent = t;
    return t;
}
TreeNode* bstFromPreorder(vector<int>& preorder) {
    int b = INT_MAX;
    int i =0;
    return f(preorder, i, b); 
}
TreeNode* bstInsert(TreeNode* root, int x, TreeNode* &z){
  if(!root){
    TreeNode* t = new TreeNode(x); 
    z = t; //to "return" the new created node
    TreeNode* c = t;
    while(c && c->parent){
      shfc(c->parent);
      c = c->parent;
    }
    return t;
  };
  if(x > root->val){
     root->right = bstInsert(root->right, x, z);
     root->right->parent = root;
  }
  else {
     root->left = bstInsert(root->left, x, z);
     root->left->parent = root;
  }
  return root;
}
void shfcUpwards(TreeNode* t){
  while(t && t->parent) {shfc(t); t = t->parent;}
}
TreeNode* rightRotate(TreeNode* z){
    TreeNode* y = z->left;
    TreeNode* p = z->parent;
    z->left = y->right;
    if(y->right) y->right->parent = z;
    y->right = z;
    z->parent = y;
    y->parent = p;
    shfc(z);
    shfc(y);
    return y;
}
TreeNode* leftRotate(TreeNode* z){
    TreeNode* y = z->right;
    TreeNode* p = z->parent;
    z->right = y->left;
    if(y->left) y->left -> parent = z;
    y->left = z;
    z->parent = y;
    y->parent = p;
    shfc(z);
    shfc(y);
    return y;
}
TreeNode* tallerChild(TreeNode* y){
  if(h(y->left) > h(y->right)) return y->left;
  else if(h(y->left) < h(y->right)) return y->right;
  else{
    if(y->val < y->parent->val) return y->left;
    else return y->right;
  }
}
TreeNode* rotate(TreeNode* z){
  if(h(z->left) > h(z->right)){
    TreeNode* y = z->left;
    TreeNode* x = tallerChild(y);
    if(x == y->left){
      z = rightRotate(z); 

    }else{
      z->left = leftRotate(y);
      z = rightRotate(z);

    }
  }else{
    TreeNode* y = z->right;
    TreeNode* x = tallerChild(y);
    if(x == y->left){
      z->right = rightRotate(y);
      z = leftRotate(z);
    }else{
      z = leftRotate(z);
    }
  } 
  return z;
}
TreeNode* bstDelete(TreeNode* root, int x){
  TreeNode* n1 = nullptr;
  TreeNode* n2 = root;
  while(n2 && n2->val != x){
    n1 = n2;
    if(n2->val > x) n2 = n2->left;
    else if(n2->val < x) n2 = n2->right;
  }
  if(n2 == nullptr) return nullptr;
  if(isLeaf(n2)){
    if(!n1) root = nullptr;
    else if(n2->val > n1->val) n1->right = nullptr;
    else n1->left = nullptr;
    delete n2;
    shfcUpwards(n1);
    return n1;
  } else if(n2->left == nullptr){
    if(!n1) root = n2->right;
    else if(n2->val > n1->val) {
      n1->right = n2->right;
      if(n2->right) n2->right->parent = n1;
    }
    else{
       n1->left = n2->right;
       n2->right->parent = n1;
    }
    delete n2;
    shfcUpwards(n1);
    return n1;
  } else if(n2->right == nullptr){
    if(!n1) root = n2->left;
    else if(n2->val > n1->val) n1->right = n2->left;
    else n1->left = n2->left;
    n2->left->parent = n1;
    delete n2;
    shfcUpwards(n1);
    return n1;
  }else{
    TreeNode* n3 = n2->right;
    TreeNode* n4 = n3;
    while(n3->left){ 
      n4 = n3;
      n3 = n3->left;
    }
    n2->val = n3->val;
    if(n4 == n3) {
      n2->right = n3->right; 
      if(n3->right) n3->right->parent = n2;
      shfcUpwards(n2);
      delete n3; return n2;
    } // not n3....
    else {
      n4->left = n3->right;
      if(n3->right) n3->right->parent = n4;
      shfcUpwards(n4);
      delete n3; return n4;
    }
  }

}
void avlInsert(TreeNode* &root, int x){
  TreeNode* z = nullptr;
  root = bstInsert(root,x, z);
  while(z && (z != root)){
    z = z->parent;
    if(abs(h(z->left) - h(z->right)) > 1){
      TreeNode* tp = z->parent;
      TreeNode* tz = z;
      z = rotate(z);
      if(!tp){root = z;}
      else{
        if(tp->left == tz) z->parent->left = z;
        else z->parent->right = z;
      }
      break;
    }
    shfc(z);
  }
}
void avlDelete(TreeNode* &root, int x){
  TreeNode* z = bstDelete(root, x); // should already update height
  if(!z) return; // not found
  while(z){
    if(abs(h(z->left) - h(z->right)) > 1){
      TreeNode* tp = z->parent;
      TreeNode* tz = z;
      z = rotate(z);
      if(!tp){root = z;}
      else{
        if(tp->left == tz) z->parent->left = z;
        else z->parent->right = z;
      }
      shfc(z);
    }
    z = z->parent;
  }
}
int main() {
  TreeNode* t = nullptr;
  vector<int> test{22, 31, 10, 28, 6, 14, 37,46, 4, 8, 13, 18, 16};
  for(int i = 0; i < test.size(); i++){
    cout << i << endl;
   avlInsert(t, test[i]);
   printWithHeight2(t);}
  avlDelete(t, 22);
  printWithHeight2(t);
}
