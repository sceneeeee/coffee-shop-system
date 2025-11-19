#include "coffee_shop.h"

// ==================== Customer Menu ====================
void customerMenu(void) {
    int custIndex = customerLogin(); // Let customer log in: get their position in list (or -1 if login fails)
    if (custIndex == -1) return; // Exit if login failed

    int choice;
    while (1) { // Loop menu until customer logs out
        // Display menu
        printf("\n========================================\n");
        printf(" Welcome, %s!\n", customers[custIndex].name);
        printf(" Account balance: $%.2f\n", customers[custIndex].balance);
        printf("========================================\n");
        printf("1. Place an order\n");
        printf("2. View order history\n");
        printf("3. Recharge\n");
        printf("4. Log out\n");
        printf("========================================\n");
        printf("Please enter your selection: ");

        // Validate user input
        if (scanf("%d", &choice) != 1) { // Check if input is a number
            clearInputBuffer(); // Clear bad input from buffer
            printf("Invalid input!\n");
            continue;
        }
        clearInputBuffer(); // Clean buffer for next use

        // Handle menu selection
        switch (choice) {
            case 1: placeOrder(custIndex); break; // Go to make a new order
            case 2: viewOrderHistory(custIndex); break; // Go to see past orders
            case 3: depositMoney(custIndex); break; // Go to add money to account
            case 4: printf("✓ Logout successful!\n"); return; // Exit customer menu
            default: printf("Invalid choice!\n"); // If input is not 1-4
        }
    }
}

// ==================== Order Management ====================
void placeOrder(int custIndex) {
    viewDrinks(); // Show all available drinks first (easy to pick ID)
    printf("\nPlease enter the drink ID: ");

    int drinkId;
    if (scanf("%d", &drinkId) != 1) { // Check if input is a number
        clearInputBuffer(); // Clear bad input
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer(); // Clean buffer

    // Check if drink exists and is available
    int drinkIndex = findDrinkById(drinkId); // Find drink's position in list
    if (drinkIndex == -1 || !drinks[drinkIndex].available) { // If drink not found/removed
        printf("❌ Invalid drink choice!\n");
        return;
    }

    // Get drink customization options
    DrinkOptions opts;
    if (!getDrinkCustomization(&opts)) { // If customization fails (bad input)
        printf("The order has been cancelled.\n");
        return;
    }

    // Calculate total cost
    double totalCost = calculateOrderCost(drinkId, opts); // Add base price + extras

    // Display order summary
    printf("\n========================================\n");
    printf("ORDER SUMMARY\n");
    printf("========================================\n");
    printf("Beverage: %s\n", drinks[drinkIndex].name);
    printf("Coffee beans: %s\n", opts.beanType);
    printf("Milk: %s\n", opts.milkType);
    printf("Temperature: %s\n", opts.isHot ? "Hot" : "Iced"); // Show Hot if true, Iced if false
    printf("Extra Shot: %s\n", opts.extraShot ? "Yes" : "No"); // Show Yes if true, No if false
    printf("Sugar: %d servings\n", opts.sugars);
    printf("----------------------------------------\n");
    printf("Total price: $%.2f\n", totalCost);
    printf("Current balance: $%.2f\n", customers[custIndex].balance);
    printf("========================================\n");

    // Check for sufficient balance
    if (customers[custIndex].balance < totalCost) { // If not enough money
        printf("\n❌ Insufficient balance! Please recharge first.\n");
        printf("Required: $%.2f\n", totalCost);
        printf("Shortfall: $%.2f\n", totalCost - customers[custIndex].balance); // How much more needed
        return;
    }

    // Confirm order
    printf("\nConfirm the order? (1=Yes, 0=No): ");
    int confirm;
    if (scanf("%d", &confirm) != 1 || confirm != 1) { // Only confirm if input is 1
        clearInputBuffer();
        printf("The order has been cancelled.\n");
        return;
    }
    clearInputBuffer(); // Clean buffer

    // Check if order history is full
    if (customers[custIndex].orderCount >= MAX_ORDERS) { // Can't add more orders
        printf("❌ The order history is full!\n");
        return;
    }

    // Create and save new order
    Order newOrder = {
        .orderId = nextOrderId++, // Auto-make unique order ID (count up)
        .drinkId = drinkId,
        .options = opts,
        .totalCost = totalCost,
        .orderTime = time(NULL) // Save current time (when order was placed)
    };
    customers[custIndex].orderHistory[customers[custIndex].orderCount++] = newOrder; // Add to order history
    customers[custIndex].balance -= totalCost; // Subtract total from account balance

    // Confirmation message
    printf("\n✓✓✓ Order placed successfully! ✓✓✓\n");
    printf("Order Number #%d\n", newOrder.orderId);
    printf("Remaining balance: $%.2f\n", customers[custIndex].balance);
    printf("Please wait a moment. Your drink is being made...\n");

    saveData(); // Save all changes (balance + order history) to file
}

// ==================== View Order History ====================
void viewOrderHistory(int custIndex) {
    printf("\n========================================\n");
    printf("ORDER HISTORY\n");
    printf("========================================\n");

    if (customers[custIndex].orderCount == 0) { // No past orders
        printf("There are no order records for now.\n");
        printf("========================================\n");
        return;
    }

    // Display each order
    for (int i = 0; i < customers[custIndex].orderCount; i++) {
        displayOrderSummary(customers[custIndex].orderHistory[i]); // Show details of each order
    }

    printf("Total number of orders: %d\n", customers[custIndex].orderCount); // How many orders total
    printf("========================================\n");
}

// ==================== Display Order Summary ====================
void displayOrderSummary(Order order) {
    char drinkName[MAX_NAME] = "Unknown drink"; // Default if drink is removed later
    int drinkIndex = findDrinkById(order.drinkId);
    if (drinkIndex != -1) { // If drink still exists
        strcpy(drinkName, drinks[drinkIndex].name); // Use real drink name
    }

    char timeStr[50];
    struct tm *timeinfo = localtime(&order.orderTime); // Convert time to readable format
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo); // Format: Year-Month-Day Hour:Minute:Second

    printf("\nOrder #%d\n", order.orderId);
    printf("----------------------------------------\n");
    printf("Beverage: %s\n", drinkName);
    printf("Coffee beans: %s | Milk: %s\n", order.options.beanType, order.options.milkType);
    printf("Temperature: %s | Extra: %s | Sugar: %d servings\n",
           order.options.isHot ? "Hot" : "Iced", // Hot/Iced
           order.options.extraShot ? "Yes" : "No", // Extra shot yes/no
           order.options.sugars);
    printf("Amount: $%.2f\n", order.totalCost);
    printf("Time: %s\n", timeStr); // Show when order was placed
    printf("----------------------------------------\n");
}

// ==================== Drink Customization ====================
int getDrinkCustomization(DrinkOptions *opts) {
    int choice;

    // Coffee bean type
    printf("\n=== Customize Your Drink ===\n");
    printf("\nCoffee Bean Type:\n");
    printf("1. Arabica\n2. Robusta\n3. Blend\n");
    printf("Please select (1-3): ");
    if (scanf("%d", &choice) != 1) { clearInputBuffer(); return 0; } // Fail if not a number
    clearInputBuffer();
    switch (choice) {
        case 1: strcpy(opts->beanType, "Arabica"); break;
        case 2: strcpy(opts->beanType, "Robusta"); break;
        case 3: strcpy(opts->beanType, "Blend"); break;
        default: strcpy(opts->beanType, "Arabica"); break; // Default to Arabica if invalid
    }

    // Milk type
    printf("\nMilk Type:\n");
    printf("1. Whole Milk\n2. Skim Milk\n3. Oat Milk\n4. Almond Milk\n5. None\n");
    printf("Please select (1-5): ");
    if (scanf("%d", &choice) != 1) { clearInputBuffer(); return 0; } // Fail if not a number
    clearInputBuffer();
    switch (choice) {
        case 1: strcpy(opts->milkType, "Whole Milk"); break;
        case 2: strcpy(opts->milkType, "Skim Milk"); break;
        case 3: strcpy(opts->milkType, "Oat Milk"); break;
        case 4: strcpy(opts->milkType, "Almond Milk"); break;
        case 5: strcpy(opts->milkType, "None"); break;
        default: strcpy(opts->milkType, "Whole Milk"); break; // Default to Whole Milk if invalid
    }

    // Temperature
    printf("\nTemperature:\n1. Hot\n2. Iced\nPlease select (1-2): ");
    if (scanf("%d", &choice) != 1) { clearInputBuffer(); return 0; } // Fail if not a number
    clearInputBuffer();
    opts->isHot = (choice == 1); // Set to true if 1 (Hot), false if 2 (Iced)

    // Extra shot option
    printf("\nAdd Extra Shot? (+$%.2f)\n1. Yes\n2. No\nPlease select (1-2): ", extraShotPrice);
    if (scanf("%d", &choice) != 1) { clearInputBuffer(); return 0; } // Fail if not a number
    clearInputBuffer();
    opts->extraShot = (choice == 1); // Set to true if 1 (Yes), false if 2 (No)

    // Sugar servings
    printf("\nAdd Sugar (+$%.2f per serving)\nPlease enter number of sugar servings (0-5): ", sugarPrice);
    if (scanf("%d", &opts->sugars) != 1) { clearInputBuffer(); return 0; } // Fail if not a number
    clearInputBuffer();
    if (opts->sugars < 0) opts->sugars = 0; // No negative sugar
    if (opts->sugars > 5) opts->sugars = 5; // Max 5 servings

    return 1; // Successful customization (return 1 = good)
}