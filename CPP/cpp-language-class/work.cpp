#include <iostream>
using namespace std;

int main(void){
    int materialCost = 20000;
    int arbeitCost = 5000*8;
    int americanoSale = 2000*50;
    int cafeLatteSale = 3000*30;
    int cappuccinoSale = 3500*20;
    
    cout << "1일 재료비 : " << materialCost << endl;
    cout << "1일 아르바이트비 : " << arbeitCost << endl;
    cout << "판매 금액" << endl;
    cout << "아메리카노 : " << americanoSale << endl;
    cout << "카페라떼 : " << cafeLatteSale << endl;
    cout << "카푸치노 : " << cappuccinoSale << endl;
    cout << "총 판매 금액 :" << americanoSale + cafeLatteSale + cappuccinoSale << endl;
    cout << "1일 순이익 : " << americanoSale + cafeLatteSale + cappuccinoSale - materialCost - arbeitCost << endl;
    return 0;
}

