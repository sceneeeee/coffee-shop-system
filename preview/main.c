#ifdef _WIN32
#include <windows.h>
#endif

#include "coffee_shop.h"

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;
Drink drinks[MAX_DRINKS];
int drinkCount = 0;
double extraShotPrice = 0.50;
double sugarPrice = 0.10;
int nextOrderId = 1;
char managerPassword[50] = "manager123";

int main(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8
    SetConsoleCP(65001);       // 设置输入为 UTF-8
    #endif
    int choice;
    initializeSystem();
    loadData();

    printf("========================================\n");
    printf(" 欢迎使用咖啡店订购系统\n");
    printf(" Coffee Shop Ordering System\n");
    printf("========================================\n\n");

    while(1) {
        displayMainMenu();
        printf("请输入选择: ");
        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("无效输入！\n\n");
            continue;
        }
        clearInputBuffer();

        switch(choice) {
            case 1: managerMenu(); break;
            case 2: customerMenu(); break;
            case 3: saveData(); printf("\n感谢使用！再见！\n"); return 0;
            default: printf("无效选择！\n\n");
        }
    }
}

void displayMainMenu(void) {
    printf("\n========================================\n");
    printf(" 主菜单 MAIN MENU\n");
    printf("========================================\n");
    printf("1. 管理员登录 (Manager Login)\n");
    printf("2. 客户自助服务 (Customer Self-Service)\n");
    printf("3. 退出系统 (Exit)\n");
    printf("========================================\n");
}
