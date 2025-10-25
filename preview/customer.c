#include "coffee_shop.h"

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
            case 4: printf("✓ 登出成功！\n"); return;
            default: printf("无效选择！\n");
        }
    }
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
