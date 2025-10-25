#include "coffee_shop.h"

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

    fp = fopen("customers.dat", "wb");
    if(fp) {
        fwrite(&customerCount, sizeof(int), 1, fp);
        fwrite(customers, sizeof(Customer), customerCount, fp);
        fclose(fp);
    }

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

    fp = fopen("customers.dat", "rb");
    if(fp) {
        fread(&customerCount, sizeof(int), 1, fp);
        fread(customers, sizeof(Customer), customerCount, fp);
        fclose(fp);
        printf("已加载 %d 个客户账户\n", customerCount);
    }

    fp = fopen("drinks.dat", "rb");
    if(fp) {
        fread(&drinkCount, sizeof(int), 1, fp);
        fread(drinks, sizeof(Drink), drinkCount, fp);
        fread(&extraShotPrice, sizeof(double), 1, fp);
        fread(&sugarPrice, sizeof(double), 1, fp);
        fread(&nextOrderId, sizeof(int), 1, fp);
        fread(managerPassword, sizeof(char), 50, fp);
        fclose(fp);
        printf("已加载 %d 种饮品\n", drinkCount);
    }
}
