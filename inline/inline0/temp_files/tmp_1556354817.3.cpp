#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

class Solution {
  public:
    bool isPalindrome(int x) {
              string str = to_string(x);
string str1 = str;
reverse(str.begin(),str.end());
if(str == str1)
return true;
else
return false;          
    }
};
//tail.cpp不给用户看的.最总编译的时候
//会把用户提交的代码和tail.cpp合并成一个文件在进行编译
void Test1()
{
  Solution s;
  bool ret = s.isPalindrome(121);
  if(ret){
    std::cout<<"Test1 OK"<<std::endl;
  }else{
    std::cout<<"Test1 failed"<<std::endl;
  }
}

void Test2()
{
  Solution s;
  bool ret = s.isPalindrome(-121);
  if(!ret){
    std::cout<<"Test2 OK"<<std::endl;
  }else{
    std::cout<<"Test2 failed"<<std::endl;
  }
}

int main()
{
  Test1();
  Test2();
  return 0;
}
