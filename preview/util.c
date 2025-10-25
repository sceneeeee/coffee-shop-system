#include "coffee_shop.h"

void clearInputBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int findDrinkById(int drinkId) {
    for(int i = 0; i < drinkCount; i++)
        if(drinks[i].id == drinkId) return i;
    return -1;
}

int findCustomerByAccount(int accountNumber) {
    for(int i = 0; i < customerCount; i++)
        if(customers[i].accountNumber == accountNumber) return i;
    return -1;
}

double calculateOrderCost(int drinkId, DrinkOptions opts) {
    double cost = drinks[findDrinkById(drinkId)].basePrice;
    if(opts.extraShot) cost += extraShotPrice;
    cost += opts.sugars * sugarPrice;
    return cost;
}
