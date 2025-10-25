#include "coffee_shop.h"

void managerMenu(void) {
    char password[20];
    printf("\n请输入管理员密码: ");
    scanf("%s", password);
    clearInputBuffer();

    if(strcmp(password, managerPassword) != 0) {
        printf("❌ 密码错误！\n");
        return;
    }

    printf("✓ 登录成功！\n");
    int choice;
    while(1) {
        printf("\n========================================\n");
        printf(" 管理员菜单 MANAGER MENU\n");
        printf("========================================\n");
        printf("饮品管理:\n");
        printf(" 1. 添加饮品\n");
        printf(" 2. 编辑饮品\n");
        printf(" 3. 删除饮品\n");
        printf(" 4. 查看所有饮品\n");
        printf(" 5. 设置选项价格\n");
        printf("\n客户管理:\n");
        printf(" 6. 添加客户\n");
        printf(" 7. 编辑客户\n");
        printf(" 8. 删除客户\n");
        printf(" 9. 查看所有客户\n");
        printf("\n统计:\n");
        printf(" 10. 查看销售统计\n");
        printf("\n系统:\n");
        printf(" 11. 修改管理员密码\n");
        printf(" 12. 返回主菜单\n");
        printf("========================================\n");
        printf("请输入选择: ");

        if(scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("无效输入！\n");
            continue;
        }
        clearInputBuffer();

        

        switch(choice) {
            case 1: addDrink(); break;
            case 2: editDrink(); break;
            case 3: removeDrink(); break;
            case 4: viewDrinks(); break;
            case 5: setPrices(); break;
            case 6: addCustomer(); break;
            case 7: editCustomer(); break;
            case 8: removeCustomer(); break;
            case 9: viewCustomers(); break;
            case 10: viewSalesStats(); break;
            case 11: changeManagerPassword(); break;
            case 12: return;
            default: printf("无效选择！\n");
        }
    }
}

// ==================== 饮品管理函数 ====================
void addDrink(void) {
    if(drinkCount >= MAX_DRINKS) {
    printf("\n❌ 已达到饮品数量上限！\n");
    return;
     }
    Drink newDrink;
    newDrink.id = drinkCount + 1;
    printf("\n=== 添加新饮品 ===\n");
    printf("请输入饮品名称: ");
    fgets(newDrink.name, MAX_NAME, stdin);
    newDrink.name[strcspn(newDrink.name, "\n")] = 0;
    printf("请输入基础价格: $");
    if(scanf("%lf", &newDrink.basePrice) != 1 || newDrink.basePrice < 0) {
    clearInputBuffer();
    printf("❌ 无效价格！\n");
    return;
     }
    clearInputBuffer();
    newDrink.available = 1;
    drinks[drinkCount++] = newDrink;
    printf("\n✓ 饮品添加成功！ID: %d\n", newDrink.id);
    saveData();
}
void editDrink(void) {
    viewDrinks();
    printf("\n请输入要编辑的饮品ID: ");
    int id;
    if(scanf("%d", &id) != 1) {
    clearInputBuffer();
    printf("❌ 无效输入！\n");
    return;
     }
    clearInputBuffer();
    int index = findDrinkById(id);
    if(index == -1 || !drinks[index].available) {
    printf("❌ 未找到该饮品！\n");
    return;
     }
    printf("\n编辑饮品: %s\n", drinks[index].name);
    printf("新名称 (按Enter保持): ");
    char temp[MAX_NAME];
    fgets(temp, MAX_NAME, stdin);
    if(strlen(temp) > 1) {
    temp[strcspn(temp, "\n")] = 0;
    strcpy(drinks[index].name, temp);
     }
    printf("新价格 (当前: $%.2f, 输入-1保持): $", drinks[index].basePrice);
    double newPrice;
    if(scanf("%lf", &newPrice) == 1 && newPrice >= 0) {
    drinks[index].basePrice = newPrice;
     }
    clearInputBuffer();
    printf("\n✓ 饮品更新成功！\n");
    saveData();
}
void removeDrink(void) {
    viewDrinks();
    printf("\n请输入要删除的饮品ID: ");
    int id;
    if(scanf("%d", &id) != 1) {
    clearInputBuffer();
    printf("❌ 无效输入！\n");
    return;
     }
    clearInputBuffer();
    int index = findDrinkById(id);
    if(index == -1) {
    printf("❌ 未找到该饮品！\n");
    return;
     }
    drinks[index].available = 0;
    printf("\n✓ 饮品已删除！\n");
    saveData();
}
void viewDrinks(void) {
    printf("\n========================================\n");
    printf(" 饮品菜单 DRINK MENU\n");
    printf("========================================\n");
    printf("%-4s %-20s %s\n", "ID", "名称", "价格");
    printf("----------------------------------------\n");
    int count = 0;
    for(int i = 0; i < drinkCount; i++) {
    if(drinks[i].available) {
    printf("%-4d %-20s $%.2f\n",
    drinks[i].id, drinks[i].name, drinks[i].basePrice);
    count++;
     }
     }
    if(count == 0) {
    printf("暂无可用饮品\n");
     }
    printf("========================================\n");
}
void setPrices(void) {
    printf("\n=== 设置选项价格 ===\n");
    printf("当前 Extra Shot 价格: $%.2f\n", extraShotPrice);
    printf("输入新价格 (-1保持): $");
    double temp;
    if(scanf("%lf", &temp) == 1 && temp >= 0) {
    extraShotPrice = temp;
     }
    printf("\n当前糖价格(每份): $%.2f\n", sugarPrice);
    printf("输入新价格 (-1保持): $");
    if(scanf("%lf", &temp) == 1 && temp >= 0) {
    sugarPrice = temp;
     }
    clearInputBuffer();
    printf("\n✓ 价格更新成功！\n");
    saveData();
}
    // ==================== 客户管理函数 ====================
void addCustomer(void) {
    if(customerCount >= MAX_CUSTOMERS) {
    printf("\n❌ 已达到客户数量上限！\n");
    return;
     }
    Customer newCust;
    newCust.accountNumber = 1001 + customerCount;
    printf("\n=== 添加新客户 ===\n");
    printf("请输入客户姓名: ");
    fgets(newCust.name, MAX_NAME, stdin);
    newCust.name[strcspn(newCust.name, "\n")] = 0;
    printf("请输入电话号码: ");
    fgets(newCust.phone, MAX_PHONE, stdin);
    newCust.phone[strcspn(newCust.phone, "\n")] = 0;
    printf("请输入初始余额: $");
    if(scanf("%lf", &newCust.balance) != 1 || newCust.balance < 0) {
    clearInputBuffer();
    printf("❌ 无效金额！\n");
    return;
     }
    clearInputBuffer();
    newCust.orderCount = 0;
    newCust.active = 1;
    customers[customerCount++] = newCust;
    printf("\n✓ 客户添加成功！\n");
    printf("账号: %d\n", newCust.accountNumber);
    printf("余额: $%.2f\n", newCust.balance);
    saveData();
}
void editCustomer(void) {
    viewCustomers();
    printf("\n请输入要编辑的账号: ");
    int accNum;
    if(scanf("%d", &accNum) != 1) {
    clearInputBuffer();
    printf("❌ 无效输入！\n");
    return;
     }
    clearInputBuffer();
    int index = findCustomerByAccount(accNum);
    if(index == -1 || !customers[index].active) {
    printf("❌ 未找到该客户！\n");
    return;
     }
    printf("\n编辑客户: %s\n", customers[index].name);
    printf("新姓名 (按Enter保持): ");
    char temp[MAX_NAME];
    fgets(temp, MAX_NAME, stdin);
    if(strlen(temp) > 1) {
    temp[strcspn(temp, "\n")] = 0;
    strcpy(customers[index].name, temp);
     }
    printf("新电话 (按Enter保持): ");
    fgets(temp, MAX_PHONE, stdin);
    if(strlen(temp) > 1) {
    temp[strcspn(temp, "\n")] = 0;
    strcpy(customers[index].phone, temp);
     }
    printf("\n✓ 客户信息更新成功！\n");
    saveData();
}
void removeCustomer(void) {
    viewCustomers();
    printf("\n请输入要删除的账号: ");
    int accNum;
    if(scanf("%d", &accNum) != 1) {
    clearInputBuffer();
    printf("❌ 无效输入！\n");
    return;
     }
    clearInputBuffer();
    int index = findCustomerByAccount(accNum);
    if(index == -1) {
    printf("❌ 未找到该客户！\n");
    return;
     }
    customers[index].active = 0;
    printf("\n✓ 客户账户已停用！\n");
    saveData();
}
void viewCustomers(void) {
    printf("\n========================================\n");
    printf(" 客户列表 CUSTOMER LIST\n");
    printf("========================================\n");
    printf("%-6s %-15s %-12s %s\n", "账号", "姓名", "电话", "余额");
    printf("----------------------------------------\n");
    int count = 0;
    for(int i = 0; i < customerCount; i++) {
    if(customers[i].active) {
    printf("%-6d %-15s %-12s $%.2f\n",
    customers[i].accountNumber,
    customers[i].name,
    customers[i].phone,
    customers[i].balance);
    count++;
     }
     }
    if(count == 0) {
    printf("暂无客户\n");
     }
    printf("========================================\n");
}
int customerLogin(void) {
    printf("\n=== 客户登录 ===\n");
    printf("请输入账号: ");
    int accNum;
    if(scanf("%d", &accNum) != 1) {
    clearInputBuffer();
    printf("❌ 无效输入！\n");
    return -1;
     }
    clearInputBuffer();
    int index = findCustomerByAccount(accNum);
    if(index != -1 && customers[index].active) {
    printf("✓ 登录成功！欢迎, %s\n", customers[index].name);
    return index;
     }
    printf("❌ 账号不存在或已停用！\n");
    return -1;
}
void depositMoney(int custIndex) {
    printf("\n=== 充值 ===\n");
    printf("当前余额: $%.2f\n", customers[custIndex].balance);
    printf("请输入充值金额: $");
    double amount;
    if(scanf("%lf", &amount) != 1 || amount <= 0) {
    clearInputBuffer();
    printf("❌ 无效金额！\n");
    return;
     }
    clearInputBuffer();
    customers[custIndex].balance += amount;
    printf("\n✓ 充值成功！\n");
    printf("充值金额: $%.2f\n", amount);
    printf("新余额: $%.2f\n", customers[custIndex].balance);
    saveData();
}
void viewSalesStats(void) {
    int totalOrders = 0;
    double totalRevenue = 0.0;
    int drinkSales[MAX_DRINKS] = {0};
    for(int i = 0; i < customerCount; i++) {
    if(customers[i].active) {
    for(int j = 0; j < customers[i].orderCount; j++) {
    totalOrders++;
    totalRevenue += customers[i].orderHistory[j].totalCost;
    int drinkIndex = findDrinkById(customers[i].orderHistory[j].drinkId);
    if(drinkIndex != -1) drinkSales[drinkIndex]++;
     }
     }
     }
    printf("\n========================================\n");
    printf(" 销售统计 SALES STATISTICS\n");
    printf("========================================\n");
    printf("总订单数: %d\n", totalOrders);
    printf("总收入: $%.2f\n", totalRevenue);
    if(totalOrders > 0) {
    printf("平均订单金额: $%.2f\n", totalRevenue / totalOrders);
     }
    printf("\n饮品销量排行:\n");
    printf("----------------------------------------\n");
    int hasData = 0;
    for(int i = 0; i < drinkCount; i++) {
    if(drinks[i].available && drinkSales[i] > 0) {
    printf("%-20s: %d 杯\n", drinks[i].name, drinkSales[i]);
    hasData = 1;
     }
     }
    if(!hasData) {
    printf("暂无销售数据\n");
     }
    printf("========================================\n");
}
// ==================== 系统设置函数 ====================
void changeManagerPassword(void) {
    char oldPassword[50];
    char newPassword[50];
    char confirmPassword[50];
    printf("\n========================================\n");
    printf(" 修改管理员密码 CHANGE PASSWORD\n");
    printf("========================================\n");
    printf("请输入当前密码: ");
    scanf("%s", oldPassword);
    clearInputBuffer();
    if(strcmp(oldPassword, managerPassword) != 0) {
    printf("\n❌ 当前密码错误！\n");
    return;
     }
    printf("请输入新密码 (至少6个字符): ");
    scanf("%s", newPassword);
    clearInputBuffer();
    if(strlen(newPassword) < 6) {
    printf("\n❌ 密码太短！至少需要6个字符。\n");
    return;
     }
    printf("请再次输入新密码: ");
    scanf("%s", confirmPassword);
    clearInputBuffer();
    if(strcmp(newPassword, confirmPassword) != 0) {
    printf("\n❌ 两次输入的密码不一致！\n");
    return;
     }
    strcpy(managerPassword, newPassword);
    printf("\n✓ 密码修改成功！\n");
    printf("请妥善保管新密码。\n");
    saveData();
}