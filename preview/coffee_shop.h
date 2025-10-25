#ifndef COFFEE_SHOP_H
#define COFFEE_SHOP_H

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
extern Customer customers[MAX_CUSTOMERS];
extern int customerCount;
extern Drink drinks[MAX_DRINKS];
extern int drinkCount;
extern double extraShotPrice;
extern double sugarPrice;
extern int nextOrderId;
extern char managerPassword[50];

// ==================== 函数声明 ====================

// main.c
void displayMainMenu(void);
void managerMenu(void);
void customerMenu(void);

// data.c
void initializeSystem(void);
void saveData(void);
void loadData(void);

// manager.c
void addDrink(void);
void editDrink(void);
void removeDrink(void);
void viewDrinks(void);
void setPrices(void);
void addCustomer(void);
void editCustomer(void);
void removeCustomer(void);
void viewCustomers(void);
void viewSalesStats(void);
void changeManagerPassword(void);

// customer.c
int customerLogin(void);
void depositMoney(int custIndex);
void placeOrder(int custIndex);
void viewOrderHistory(int custIndex);

// util.c
void clearInputBuffer(void);
int findDrinkById(int drinkId);
int findCustomerByAccount(int accountNumber);
double calculateOrderCost(int drinkId, DrinkOptions opts);
int getDrinkCustomization(DrinkOptions *opts);
void displayOrderSummary(Order order);

#endif
