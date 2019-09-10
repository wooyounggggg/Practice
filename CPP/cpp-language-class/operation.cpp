#include <iostream>

using namespace std;

int main(void){
    int input, min, sec;
    const int SEC_PER_MINUTE = 60;

    cout << "초단위의 시간을 입력하시오(32억초 이하) :";
    cin >> input;

    min = input / SEC_PER_MINUTE;
    sec = input % SEC_PER_MINUTE;

    cout << input << "seconds is" << min << "분" << sec << "초입니다."<<endl;
    return 0;
}