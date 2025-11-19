#include "coffee_shop.h"

// ==================== Global Variables Definition ====================
// Define global arrays and variables declared as extern in coffee_shop.h

Customer customers[MAX_CUSTOMERS];  // All customer records
int customerCount = 0;             // Current number of customers

Drink drinks[MAX_DRINKS];          // All drink types in the menu
int drinkCount = 0;                // Current number of drinks

double extraShotPrice = 0.50;      // Price for one extra espresso shot
double sugarPrice = 0.10;          // Price for one unit of sugar

int nextOrderId = 1;               // Auto-increment order ID counter

// Default manager password for manager login
char managerPassword[50] = "manager123";

/*
 * Entry point of the Coffee Shop Ordering System.
 * - Initializes default data
 * - Loads saved data from files
 * - Shows main menu in a loop until user chooses to exit
 */
int main(void) {
    int choice;

    // Initialize default drinks if needed, then load saved data
    initializeSystem();
    loadData();

    // Welcome banner
    printf("========================================\n");
    printf(" Coffee Shop Ordering System\n");
    printf("========================================\n\n");

    // Main loop: show menu and handle user choices
    while (1) {
        displayMainMenu();
        printf("Please enter your choice: ");

        // Read user input and validate
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();                // Clear invalid input from buffer
            printf("Invalid input!\n\n");
            continue;                          // Ask again
        }
        clearInputBuffer();                    // Clear trailing newline

        // Handle user choice
        switch (choice) {
            case 1:
                managerMenu();                 // Manager operations
                break;
            case 2:
                customerMenu();                // Customer self-service menu
                break;
            case 3:
                saveData();                    // Save data before exiting
                printf("\nThank you for using the system. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice!\n\n");
        }
    }
}

/*
 * Display the main menu options.
 * This function only prints the available top-level actions.
 */
void displayMainMenu(void) {
    printf("\n========================================\n");
    printf(" MAIN MENU\n");
    printf("========================================\n");
    printf("1. Manager login\n");
    printf("2. Customer self-service\n");
    printf("3. Exit\n");
    printf("========================================\n");
}
