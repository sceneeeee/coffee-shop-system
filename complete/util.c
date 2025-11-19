#include "coffee_shop.h"

/*
 * clearInputBuffer
 * -----------------------------------------
 * Clears the remaining characters in the input buffer
 * to prevent invalid input from affecting later scanf calls.
 * Useful after reading integers to remove leftover '\n'.
 */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * findDrinkById
 * -----------------------------------------
 * Searches the drinks[] array for a drink with the given ID.
 *
 * Parameters:
 *   drinkId - The drink ID to search for.
 *
 * Returns:
 *   The index of the drink in the drinks[] array,
 *   or -1 if not found.
 */
int findDrinkById(int drinkId) {
    for (int i = 0; i < drinkCount; i++)
        if (drinks[i].id == drinkId)
            return i;
    return -1;   // Not found
}

/*
 * findCustomerByAccount
 * -----------------------------------------
 * Searches the customers[] array for a customer account
 * based on the provided account number.
 *
 * Parameters:
 *   accountNumber - The account number to search.
 *
 * Returns:
 *   The index of the customer, or -1 if not found.
 */
int findCustomerByAccount(int accountNumber) {
    for (int i = 0; i < customerCount; i++)
        if (customers[i].accountNumber == accountNumber)
            return i;
    return -1;  // Not found
}

/*
 * calculateOrderCost
 * -----------------------------------------
 * Computes the total cost of an order based on:
 *   - base drink price
 *   - number of extra espresso shots
 *   - sugar added
 *
 * Parameters:
 *   drinkId - ID of the drink being ordered
 *   opts    - selected customization options
 *
 * Returns:
 *   Total cost of the order as a double.
 */
double calculateOrderCost(int drinkId, DrinkOptions opts) {
    // Start with the drink's base price
    double cost = drinks[findDrinkById(drinkId)].basePrice;

    // Add price for extra espresso shot if requested
    if (opts.extraShot)
        cost += extraShotPrice;

    // Add price for sugar multiplied by units added
    cost += opts.sugars * sugarPrice;

    return cost;
}
