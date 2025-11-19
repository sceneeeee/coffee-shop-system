#include "coffee_shop.h"

/*
 * Initialize system with default drinks if the menu is empty.
 * This is typically called at startup after loading data.
 */
void initializeSystem(void) {
    // If no drinks are loaded, create a default drink menu
    if (drinkCount == 0) {
        drinks[0] = (Drink){1, "Espresso",    2.50, 1};
        drinks[1] = (Drink){2, "Latte",       3.50, 1};
        drinks[2] = (Drink){3, "Cappuccino",  3.50, 1};
        drinks[3] = (Drink){4, "Mochaccino",  4.00, 1};
        drinks[4] = (Drink){5, "Americano",   2.80, 1};
        drinkCount = 5; // Update the number of drinks
    }
}

/*
 * Save all persistent data to binary files.
 * - customers.dat : customer information and order history
 * - drinks.dat    : drink menu and pricing settings
 */
void saveData(void) {
    FILE *fp;

    // Save customer data
    fp = fopen("customers.dat", "wb");
    if (fp) {
        // First write the number of customers, then the array itself
        fwrite(&customerCount, sizeof(int), 1, fp);
        fwrite(customers, sizeof(Customer), customerCount, fp);
        fclose(fp);
    }

    // Save drink data and global pricing/ID info
    fp = fopen("drinks.dat", "wb");
    if (fp) {
        // Save number of drinks and all drink entries
        fwrite(&drinkCount, sizeof(int), 1, fp);
        fwrite(drinks, sizeof(Drink), drinkCount, fp);

        // Save extra pricing info and manager password
        fwrite(&extraShotPrice, sizeof(double), 1, fp);
        fwrite(&sugarPrice, sizeof(double), 1, fp);
        fwrite(&nextOrderId, sizeof(int), 1, fp);
        fwrite(managerPassword, sizeof(char), 50, fp);

        fclose(fp);
    }

    printf("Data has been saved.\n");
}

/*
 * Load persistent data from binary files if they exist.
 * If files do not exist, the system will rely on defaults later.
 */
void loadData(void) {
    FILE *fp;

    // Load customer data
    fp = fopen("customers.dat", "rb");
    if (fp) {
        fread(&customerCount, sizeof(int), 1, fp);
        fread(customers, sizeof(Customer), customerCount, fp);
        fclose(fp);
        printf("Loaded %d customer accounts.\n", customerCount);
    }

    // Load drink data and global pricing/ID info
    fp = fopen("drinks.dat", "rb");
    if (fp) {
        fread(&drinkCount, sizeof(int), 1, fp);
        fread(drinks, sizeof(Drink), drinkCount, fp);
        fread(&extraShotPrice, sizeof(double), 1, fp);
        fread(&sugarPrice, sizeof(double), 1, fp);
        fread(&nextOrderId, sizeof(int), 1, fp);
        fread(managerPassword, sizeof(char), 50, fp);
        fclose(fp);
        printf("Loaded %d drink types.\n", drinkCount);
    }
}
