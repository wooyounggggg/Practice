#include <iostream>
#include <string>

using namespace std;

int main(void){
    string name;
    string stdNum;
    cout << "이름을 입력하세요 : ";
    cin >> name;
    cout << "학번을 입력하세요 : ";
    cin >> stdNum;
    cout << stdNum << "학번" << name << "을 환영합니다."<<endl;
    return 0;
}