/**
 * coffee_shop_preview.c
 * 咖啡店订购系统 - 单文件预览版
 *
 * 编译: gcc -o coffee_preview coffee_shop_preview.c -std=c99 -Wall
 * 运行: ./coffee_preview
 *
 * 管理员密码: manager123
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// ==================== 常量定义 ====================
#define MAX_CUSTOMERS 100
#define MAX_DRINKS 20
#define MAX_ORDERS 50
#define MAX_NAME 50
#define MAX_PHONE 15
// ==================== 数据结构 ====================
typedef struct {
int id;
char name[MAX_NAME];
double basePrice;
int available;
} Drink;
typedef struct {
char beanType[MAX_NAME];
char milkType[MAX_NAME];
int isHot;
int extraShot;
int sugars;
} DrinkOptions;
typedef struct {
int orderId;
int drinkId;
DrinkOptions options;
double totalCost;
time_t orderTime;
} Order;
typedef struct {
int accountNumber;
char name[MAX_NAME];
char phone[MAX_PHONE];
double balance;
Order orderHistory[MAX_ORDERS];
int orderCount;
int active;
} Customer;
// ==================== 全局变量 ====================
Customer customers[MAX_CUSTOMERS];
int customerCount = 0;
Drink drinks[MAX_DRINKS];
int drinkCount = 0;
double extraShotPrice = 0.50;
double sugarPrice = 0.10;
int nextOrderId = 1;
char managerPassword[50] = "manager123";
// ==================== 函数声明 ====================
void clearInputBuffer(void);
void initializeSystem(void);
void saveData(void);
void loadData(void);
void displayMainMenu(void);
void managerMenu(void);
void customerMenu(void);
void addDrink(void);
void editDrink(void);
void removeDrink(void);
void viewDrinks(void);
void setPrices(void);
double calculateOrderCost(int drinkId, DrinkOptions opts);
int findDrinkById(int drinkId);
void addCustomer(void);
void editCustomer(void);
void removeCustomer(void);
void viewCustomers(void);
int customerLogin(void);
void depositMoney(int custIndex);
void viewSalesStats(void);
int findCustomerByAccount(int accountNumber);
void placeOrder(int custIndex);
void viewOrderHistory(int custIndex);
void displayOrderSummary(Order order);
int getDrinkCustomization(DrinkOptions *opts);
void changeManagerPassword(void);
// ==================== 主函数 ====================
int main(void) {
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
case 3:
saveData();
printf("\n感谢使用！再见！\n");
return 0;
default:
printf("无效选择！\n\n");
 }
 }
return 0;
}
// ==================== 工具函数 ====================
void clearInputBuffer(void) {
int c;
while ((c = getchar()) != '\n' && c != EOF);
}
void initializeSystem(void) {
if(drinkCount == 0) {
drinks[0] = (Drink){1, "Espresso", 2.50, 1};
drinks[1] = (Drink){2, "Latte", 3.50, 1};
drinks[2] = (Drink){3, "Cappuccino", 3.50, 1};
drinks[3] = (Drink){4, "Mochaccino", 4.00, 1};
drinks[4] = (Drink){5, "Americano", 2.80, 1};
drinkCount = 5;
 }
}
void saveData(void) {
FILE *fp;
// 保存客户数据
fp = fopen("customers.dat", "wb");
if(fp) {
fwrite(&customerCount, sizeof(int), 1, fp);
fwrite(customers, sizeof(Customer), customerCount, fp);
fclose(fp);
 }
// 保存饮品数据
fp = fopen("drinks.dat", "wb");
if(fp) {
fwrite(&drinkCount, sizeof(int), 1, fp);
fwrite(drinks, sizeof(Drink), drinkCount, fp);
fwrite(&extraShotPrice, sizeof(double), 1, fp);
fwrite(&sugarPrice, sizeof(double), 1, fp);
fwrite(&nextOrderId, sizeof(int), 1, fp);
fwrite(managerPassword, sizeof(char), 50, fp);
fclose(fp);
 }
printf("数据已保存\n");
}
void loadData(void) {
FILE *fp;
// 加载客户数据
fp = fopen("customers.dat", "rb");
if(fp) {
fread(&customerCount, sizeof(int), 1, fp);
fread(customers, sizeof(Customer), customerCount, fp);
fclose(fp);
printf("已加载 %d 个客户账户\n", customerCount);
 }
// 加载饮品数据
fp = fopen("drinks.dat", "rb");
if(fp) {
fread(&drinkCount, sizeof(int), 1, fp);
fread(drinks, sizeof(Drink), drinkCount, fp);
fread(&extraShotPrice, sizeof(double), 1, fp);
fread(&sugarPrice, sizeof(double), 1, fp);
fread(&nextOrderId, sizeof(int), 1, fp);
if(fread(managerPassword, sizeof(char), 50, fp) != 50) {
strcpy(managerPassword, "manager123");
 }
fclose(fp);
printf("已加载 %d 种饮品\n", drinkCount);
 }
printf("\n");
}
// ==================== 菜单函数 ====================
void displayMainMenu(void) {
printf("\n========================================\n");
printf(" 主菜单 MAIN MENU\n");
printf("========================================\n");
printf("1. 管理员登录 (Manager Login)\n");
printf("2. 客户自助服务 (Customer Self-Service)\n");
printf("3. 退出系统 (Exit)\n");
printf("========================================\n");
}
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
void customerMenu(void) {
int custIndex = customerLogin();
if(custIndex == -1) return;
int choice;
while(1) {
printf("\n========================================\n");
printf(" 欢迎, %s!\n", customers[custIndex].name);
printf(" 账户余额: $%.2f\n", customers[custIndex].balance);
printf("========================================\n");
printf("1. 下单\n");
printf("2. 查看订单历史\n");
printf("3. 充值\n");
printf("4. 登出\n");
printf("========================================\n");
printf("请输入选择: ");
if(scanf("%d", &choice) != 1) {
clearInputBuffer();
printf("无效输入！\n");
continue;
 }
clearInputBuffer();
switch(choice) {
case 1: placeOrder(custIndex); break;
case 2: viewOrderHistory(custIndex); break;
case 3: depositMoney(custIndex); break;
case 4:
printf("✓ 登出成功！\n");
return;
default:
printf("无效选择！\n");
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
double calculateOrderCost(int drinkId, DrinkOptions opts) {
double cost = 0.0;
int index = findDrinkById(drinkId);
if(index != -1) {
cost = drinks[index].basePrice;
 }
if(opts.extraShot) cost += extraShotPrice;
cost += opts.sugars * sugarPrice;
return cost;
}
int findDrinkById(int drinkId) {
for(int i = 0; i < drinkCount; i++) {
if(drinks[i].id == drinkId) return i;
 }
return -1;
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
int findCustomerByAccount(int accountNumber) {
for(int i = 0; i < customerCount; i++) {
if(customers[i].accountNumber == accountNumber) return i;
 }
return -1;
}
// ==================== 订单管理函数 ====================
void placeOrder(int custIndex) {
viewDrinks();
printf("\n请输入饮品ID: ");
int drinkId;
if(scanf("%d", &drinkId) != 1) {
clearInputBuffer();
printf("❌ 无效输入！\n");
return;
 }
clearInputBuffer();
int drinkIndex = findDrinkById(drinkId);
if(drinkIndex == -1 || !drinks[drinkIndex].available) {
printf("❌ 无效的饮品选择！\n");
return;
 }
DrinkOptions opts;
if(!getDrinkCustomization(&opts)) {
printf("订单已取消\n");
return;
 }
double totalCost = calculateOrderCost(drinkId, opts);
printf("\n========================================\n");
printf(" 订单摘要 ORDER SUMMARY\n");
printf("========================================\n");
printf("饮品: %s\n", drinks[drinkIndex].name);
printf("咖啡豆: %s\n", opts.beanType);
printf("牛奶: %s\n", opts.milkType);
printf("温度: %s\n", opts.isHot ? "热" : "冰");
printf("Extra Shot: %s\n", opts.extraShot ? "是" : "否");
printf("糖: %d 份\n", opts.sugars);
printf("----------------------------------------\n");
printf("总价: $%.2f\n", totalCost);
printf("当前余额: $%.2f\n", customers[custIndex].balance);
printf("========================================\n");
if(customers[custIndex].balance < totalCost) {
printf("\n❌ 余额不足！请先充值。\n");
printf("需要: $%.2f\n", totalCost);
printf("缺少: $%.2f\n", totalCost - customers[custIndex].balance);
return;
 }
printf("\n确认下单? (1=是, 0=否): ");
int confirm;
if(scanf("%d", &confirm) != 1 || confirm != 1) {
clearInputBuffer();
printf("订单已取消\n");
return;
 }
clearInputBuffer();
if(customers[custIndex].orderCount >= MAX_ORDERS) {
printf("❌ 订单历史已满！\n");
return;
 }
Order newOrder = {
 .orderId = nextOrderId++,
 .drinkId = drinkId,
 .options = opts,
 .totalCost = totalCost,
 .orderTime = time(NULL)
 };
customers[custIndex].orderHistory[customers[custIndex].orderCount++] = newOrder;
customers[custIndex].balance -= totalCost;
printf("\n✓✓✓ 下单成功！✓✓✓\n");
printf("订单号: #%d\n", newOrder.orderId);
printf("剩余余额: $%.2f\n", customers[custIndex].balance);
printf("请稍候，您的饮品正在制作中...\n");
saveData();
}
void viewOrderHistory(int custIndex) {
printf("\n========================================\n");
printf(" 订单历史 ORDER HISTORY\n");
printf("========================================\n");
if(customers[custIndex].orderCount == 0) {
printf("暂无订单记录\n");
printf("========================================\n");
return;
 }
for(int i = 0; i < customers[custIndex].orderCount; i++) {
displayOrderSummary(customers[custIndex].orderHistory[i]);
 }
printf("总订单数: %d\n", customers[custIndex].orderCount);
printf("========================================\n");
}
void displayOrderSummary(Order order) {
char drinkName[MAX_NAME] = "未知饮品";
int drinkIndex = findDrinkById(order.drinkId);
if(drinkIndex != -1) {
strcpy(drinkName, drinks[drinkIndex].name);
 }
char timeStr[50];
struct tm *timeinfo = localtime(&order.orderTime);
strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
printf("\n订单 #%d\n", order.orderId);
printf("----------------------------------------\n");
printf("饮品: %s\n", drinkName);
printf("咖啡豆: %s | 牛奶: %s\n",
order.options.beanType, order.options.milkType);
printf("温度: %s | Extra: %s | 糖: %d份\n",
order.options.isHot ? "热" : "冰",
order.options.extraShot ? "是" : "否",
order.options.sugars);
printf("金额: $%.2f\n", order.totalCost);
printf("时间: %s\n", timeStr);
printf("----------------------------------------\n");
}
int getDrinkCustomization(DrinkOptions *opts) {
printf("\n=== 定制您的饮品 ===\n");
printf("\n咖啡豆类型:\n");
printf("1. Arabica\n");
printf("2. Robusta\n");
printf("3. Blend\n");
printf("请选择 (1-3): ");
int choice;
if(scanf("%d", &choice) != 1) {
clearInputBuffer();
return 0;
 }
clearInputBuffer();
switch(choice) {
case 1: strcpy(opts->beanType, "Arabica"); break;
case 2: strcpy(opts->beanType, "Robusta"); break;
case 3: strcpy(opts->beanType, "Blend"); break;
default: strcpy(opts->beanType, "Arabica"); break;
 }
printf("\n牛奶类型:\n");
printf("1. Whole Milk\n");
printf("2. Skim Milk\n");
printf("3. Oat Milk\n");
printf("4. Almond Milk\n");
printf("5. None\n");
printf("请选择 (1-5): ");
if(scanf("%d", &choice) != 1) {
clearInputBuffer();
return 0;
 }
clearInputBuffer();
switch(choice) {
case 1: strcpy(opts->milkType, "Whole Milk"); break;
case 2: strcpy(opts->milkType, "Skim Milk"); break;
case 3: strcpy(opts->milkType, "Oat Milk"); break;
case 4: strcpy(opts->milkType, "Almond Milk"); break;
case 5: strcpy(opts->milkType, "None"); break;
default: strcpy(opts->milkType, "Whole Milk"); break;
 }
printf("\n温度:\n");
printf("1. Hot\n");
printf("2. Iced\n");
printf("请选择 (1-2): ");
if(scanf("%d", &choice) != 1) {
clearInputBuffer();
return 0;
 }
clearInputBuffer();
opts->isHot = (choice == 1) ? 1 : 0;
printf("\n添加 Extra Shot? (+$%.2f)\n", extraShotPrice);
printf("1. 是\n");
printf("2. 否\n");
printf("请选择 (1-2): ");
if(scanf("%d", &choice) != 1) {
clearInputBuffer();
return 0;
 }
clearInputBuffer();
opts->extraShot = (choice == 1) ? 1 : 0;
printf("\n添加糖 (每份 +$%.2f)\n", sugarPrice);
printf("请输入糖的份数 (0-5): ");
if(scanf("%d", &opts->sugars) != 1) {
clearInputBuffer();
return 0;
 }
clearInputBuffer();
if(opts->sugars < 0) opts->sugars = 0;
if(opts->sugars > 5) opts->sugars = 5;
return 1;
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