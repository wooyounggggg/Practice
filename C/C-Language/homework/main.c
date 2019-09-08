#include <stdio.h>

int main(void){
    char stdNum[20]; //학번
    int grade; //학년
    char major[15]; //전공
    char whereLive[10]; //거주지
    double distanceToNewMillenium; //거주지로부터 새천년관까지의 거리
    double distanceToEngineering; //거주지로부터 공학관까지의 거리
    char newLine; //개행문자

    printf("학번 : ");
    scanf("%d",&stdNum);
    printf("학년 : ");
    scanf("%d",&grade);
    printf("전공 : ");
    scanf("%s",major);
    printf("거주지 : ");
    scanf("%s",whereLive);
    scanf("%c",&newLine); //개행문자 처리
    printf("새천년관까지의 거리 : ");
    scanf("%lf",&distanceToNewMillenium);
    printf("공학관까지의 거리 : ");
    scanf("%lf",&distanceToEngineering);
    printf("----------------------------------------------------\n");
    printf("학번 : %d\n학년 : %d\n전공 : %s\n거주지: %s\n거주지~새천년관 : %fkm\n거주지~공학관 : %fkm\n",
    stdNum, grade, major, whereLive, distanceToNewMillenium, distanceToEngineering  
    );
    return 0;
}