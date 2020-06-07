#include <iostream>
#include <vector>
using namespace std;

void fixup_max(vector<int> &A, int k){
  while(k > 0 && A[(k-1)/2] < A[k]){
    int tmp = A[k];
    A[k] = A[(k-1)/2];
    A[(k-1)/2] = tmp;

    k = (k-1)/2;
  }
}
void fixup_min(vector<int> &A, int k){
  while(k > 0 && A[(k-1)/2] > A[k]){
    int tmp = A[k];
    A[k] = A[(k-1)/2];
    A[(k-1)/2] = tmp;

    k = (k-1)/2;
  }
}
void fixdown(vector<int> &A, int k, bool ismax){
  int n = A.size();
  while(2*k + 1 < n ){
    int j = 2*k+1;

    if(ismax){
      if(j< n-1 && A[j+1] > A[j]) j++;
      if(A[k] >= A[j]) break;
    }else{
      if(j< n-1 && A[j+1] < A[j]) j++;
      if(A[k] <= A[j]) break;
    }
    
    int tmp = A[k];
    A[k] = A[j];
    A[j] = tmp;

    k = j;
  }
}
void insert(vector<int> &A, int x, bool ismax){
  A.push_back(x);
  if(ismax) fixup_max(A, A.size()-1);
  else fixup_min(A, A.size()-1);
}
int remove(vector<int> &A, bool ismax){
  int res = A[0];
  A[0] = A[A.size()-1];
  A.pop_back();
  fixdown(A, 0, ismax);
  return res;
}
void print(string prefix, vector<int> &A, int i, bool isLeft) {
      if (i < A.size()) {
          cout << (prefix + (isLeft ? "|-- " : "\\-- ") + to_string(A[i])) << endl;
          print(prefix + (isLeft ? "|   " : "    "),A, 2*i+1, true);
          print(prefix + (isLeft ? "|   " : "    "), A, 2*i+2, false);
      }
}


int main() {
  vector<int> input {15, 10, 1, 20, 30, 0, 4, 35, 23, 6, 17, -1, 2, 3};
vector<int> hi;
vector<int> lo;
for(int i = 0; i < input.size(); i++){

  int k = input[i];
  if(hi.empty()) insert(hi, input[i], false);
  else if(input[i] >= hi[0]){
    if(hi.size() <= lo.size()){
       insert(hi, input[i], false);
    }else{
      insert(hi, k, false);
      int x = remove(hi, false);
      insert(lo, x, true);
    }
  }else{
    if(hi.size() <= lo.size()){
      insert(lo, k, true);
      int x = remove(lo, true);
      insert(hi, x, false);
    }
    else {insert(lo, k, true);}
  }

}
  print("1: ", hi, 0, false);
 print("2: ", lo, 0, false);

}
