#include "coffee_shop.h"

// ==================== Manager menu ====================
void managerMenu(void) {
    char password[20];
    printf("\nPlease enter manager password: ");
    scanf("%s", password);
    clearInputBuffer();  // Clear leftover newline from scanf

    // Check password correctness
    if (strcmp(password, managerPassword) != 0) {
        printf("❌ Wrong password!\n");
        return;
    }

    printf("✓ Login successful!\n");
    int choice;
    while (1) {  // Loop menu until user chooses to return
        // Display the menu options
        printf("\n========================================\n");
        printf(" MANAGER MENU\n");
        printf("========================================\n");
        printf("Drink Management:\n");
        printf(" 1. Add Drink\n");
        printf(" 2. Edit Drink\n");
        printf(" 3. Delete Drink\n");
        printf(" 4. View All Drinks\n");
        printf(" 5. Set Option Prices\n");
        printf("\nCustomer Management:\n");
        printf(" 6. Add Customer\n");
        printf(" 7. Edit Customer\n");
        printf(" 8. Delete Customer\n");
        printf(" 9. View All Customers\n");
        printf("\nStatistics:\n");
        printf(" 10. View Sales Statistics\n");
        printf("\nSystem:\n");
        printf(" 11. Change Manager Password\n");
        printf(" 12. Return to Main Menu\n");
        printf("========================================\n");
        printf("Please enter your choice: ");

        // Input choice from manager
        if (scanf("%d", &choice) != 1) {  // Check if input is a number
            clearInputBuffer();
            printf("Invalid input!\n");
            continue;
        }
        clearInputBuffer();  // Clean input buffer for next use

        // Handle the selected option
        switch (choice) {
            case 1: addDrink(); break;          // Go to add new drink
            case 2: editDrink(); break;         // Go to edit existing drink
            case 3: removeDrink(); break;       // Go to delete drink
            case 4: viewDrinks(); break;        // Go to see all drinks
            case 5: setPrices(); break;         // Go to set extra prices
            case 6: addCustomer(); break;       // Go to add new customer
            case 7: editCustomer(); break;      // Go to edit customer info
            case 8: removeCustomer(); break;    // Go to delete customer
            case 9: viewCustomers(); break;     // Go to see all customers
            case 10: viewSalesStats(); break;   // Go to see sales data
            case 11: changeManagerPassword(); break;  // Go to change password
            case 12: return; // Exit manager menu
            default: printf("Invalid choice!\n");  // If input is not 1-12
        }
    }
}

// ==================== Drink Management Functions ====================
void addDrink(void) {
    if (drinkCount >= MAX_DRINKS) {  // Check if drink list is full
        printf("\n❌ Maximum number of drinks reached!\n");
        return;
    }
    Drink newDrink;
    newDrink.id = drinkCount + 1;  // Auto-make unique ID (start from 1)
    printf("\n=== Add New Drink ===\n");
    printf("Please enter drink name: ");
    fgets(newDrink.name, MAX_NAME, stdin);  // Read name (allow spaces)
    newDrink.name[strcspn(newDrink.name, "\n")] = 0;  // Remove newline from input
    printf("Please enter base price: $");
    if (scanf("%lf", &newDrink.basePrice) != 1 || newDrink.basePrice < 0) {  // Check valid price (no negative)
        clearInputBuffer();
        printf("❌ Invalid price!\n");
        return;
    }
    clearInputBuffer();  // Clean buffer after price input
    newDrink.available = 1;  // Mark drink as available (1 = yes)
    drinks[drinkCount++] = newDrink;  // Add drink to list + count up
    printf("\n✓ Drink added successfully! ID: %d\n", newDrink.id);
    saveData();  // Save change to file
}

void editDrink(void) {
    viewDrinks();  // Show all drinks first (easy to pick ID)
    printf("\nPlease enter the ID of the drink to edit: ");
    int id;
    if (scanf("%d", &id) != 1) {  // Check if input is a number
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    int index = findDrinkById(id);  // Find drink's position in list
    if (index == -1 || !drinks[index].available) {  // If drink not found/removed
        printf("❌ Drink not found!\n");
        return;
    }
    printf("\nEditing Drink: %s\n", drinks[index].name);
    printf("New name (Press Enter to keep current): ");
    char temp[MAX_NAME];
    fgets(temp, MAX_NAME, stdin);  // Read new name (or just Enter)
    if (strlen(temp) > 1) {  // Only change if user typed something (not just Enter)
        temp[strcspn(temp, "\n")] = 0;  // Remove newline
        strcpy(drinks[index].name, temp);  // Update name
    }
    printf("New price (Current: $%.2f, Enter -1 to keep): $", drinks[index].basePrice);
    double newPrice;
    if (scanf("%lf", &newPrice) == 1 && newPrice >= 0) {  // Update if valid price
        drinks[index].basePrice = newPrice;
    }
    clearInputBuffer();  // Clean buffer
    printf("\n✓ Drink updated successfully!\n");
    saveData();  // Save change to file
}

void removeDrink(void) {
    viewDrinks();  // Show all drinks first
    printf("\nPlease enter the ID of the drink to delete: ");
    int id;
    if (scanf("%d", &id) != 1) {  // Check if input is a number
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    int index = findDrinkById(id);  // Find drink's position
    if (index == -1) {  // If drink doesn't exist
        printf("❌ Drink not found!\n");
        return;
    }
    drinks[index].available = 0;  // Hide drink (not delete forever: 0 = unavailable)
    printf("\n✓ Drink deleted successfully!\n");
    saveData();  // Save change to file
}

void viewDrinks(void) {
    printf("\n========================================\n");
    printf(" DRINK MENU\n");
    printf("========================================\n");
    printf("%-4s %-20s %s\n", "ID", "Name", "Price");  // Table header
    printf("----------------------------------------\n");
    int count = 0;  // Count how many available drinks
    for (int i = 0; i < drinkCount; i++) {
        if (drinks[i].available) {  // Only show available drinks
            printf("%-4d %-20s $%.2f\n",
                   drinks[i].id, drinks[i].name, drinks[i].basePrice);
            count++;
        }
    }
    if (count == 0) {  // If no drinks available
        printf("No available drinks\n");
    }
    printf("========================================\n");
}

void setPrices(void) {
    printf("\n=== Set Option Prices ===\n");
    printf("Current Extra Shot price: $%.2f\n", extraShotPrice);
    printf("Enter new price (-1 to keep): $");
    double temp;
    if (scanf("%lf", &temp) == 1 && temp >= 0) {  // Update if valid price
        extraShotPrice = temp;
    }
    printf("\nCurrent Sugar price (per serving): $%.2f\n", sugarPrice);
    printf("Enter new price (-1 to keep): $");
    if (scanf("%lf", &temp) == 1 && temp >= 0) {  // Update if valid price
        sugarPrice = temp;
    }
    clearInputBuffer();  // Clean buffer
    printf("\n✓ Prices updated successfully!\n");
    saveData();  // Save change to file
}

// ==================== Customer Management Functions ====================
void addCustomer(void) {
    if (customerCount >= MAX_CUSTOMERS) {  // Check if customer list is full
        printf("\n❌ Maximum number of customers reached!\n");
        return;
    }
    Customer newCust;
    newCust.accountNumber = 1001 + customerCount;  // Auto-make account (start from 1001)
    printf("\n=== Add New Customer ===\n");
    printf("Please enter customer name: ");
    fgets(newCust.name, MAX_NAME, stdin);  // Read name
    newCust.name[strcspn(newCust.name, "\n")] = 0;  // Remove newline
    printf("Please enter phone number: ");
    fgets(newCust.phone, MAX_PHONE, stdin);  // Read phone
    newCust.phone[strcspn(newCust.phone, "\n")] = 0;  // Remove newline
    printf("Please enter initial balance: $");
    if (scanf("%lf", &newCust.balance) != 1 || newCust.balance < 0) {  // No negative balance
        clearInputBuffer();
        printf("❌ Invalid amount!\n");
        return;
    }
    clearInputBuffer();  // Clean buffer
    newCust.orderCount = 0;  // Start with 0 orders
    newCust.active = 1;  // Mark account as active (1 = yes)
    customers[customerCount++] = newCust;  // Add customer to list + count up
    printf("\n✓ Customer added successfully!\n");
    printf("Account Number: %d\n", newCust.accountNumber);
    printf("Balance: $%.2f\n", newCust.balance);
    saveData();  // Save change to file
}

void editCustomer(void) {
    viewCustomers();  // Show all customers first
    printf("\nPlease enter the account number to edit: ");
    int accNum;
    if (scanf("%d", &accNum) != 1) {  // Check if input is a number
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    int index = findCustomerByAccount(accNum);  // Find customer's position
    if (index == -1 || !customers[index].active) {  // If customer not found/inactive
        printf("❌ Customer not found!\n");
        return;
    }
    printf("\nEditing Customer: %s\n", customers[index].name);
    printf("New name (Press Enter to keep current): ");
    char temp[MAX_NAME];
    fgets(temp, MAX_NAME, stdin);  // Read new name (or Enter)
    if (strlen(temp) > 1) {  // Change only if user typed something
        temp[strcspn(temp, "\n")] = 0;
        strcpy(customers[index].name, temp);  // Update name
    }
    printf("New phone number (Press Enter to keep current): ");
    fgets(temp, MAX_PHONE, stdin);  // Read new phone (or Enter)
    if (strlen(temp) > 1) {  // Change only if user typed something
        temp[strcspn(temp, "\n")] = 0;
        strcpy(customers[index].phone, temp);  // Update phone
    }
    printf("\n✓ Customer information updated successfully!\n");
    saveData();  // Save change to file
}

void removeCustomer(void) {
    viewCustomers();  // Show all customers first
    printf("\nPlease enter the account number to delete: ");
    int accNum;
    if (scanf("%d", &accNum) != 1) {  // Check if input is a number
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();
    int index = findCustomerByAccount(accNum);  // Find customer's position
    if (index == -1) {  // If customer doesn't exist
        printf("❌ Customer not found!\n");
        return;
    }
    customers[index].active = 0;  // Deactivate account (not delete forever)
    printf("\n✓ Customer account deactivated!\n");
    saveData();  // Save change to file
}

void viewCustomers(void) {
    printf("\n========================================\n");
    printf(" CUSTOMER LIST\n");
    printf("========================================\n");
    printf("%-6s %-15s %-12s %s\n", "Account", "Name", "Phone", "Balance");  // Table header
    printf("----------------------------------------\n");
    int count = 0;  // Count active customers
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].active) {  // Only show active accounts
            printf("%-6d %-15s %-12s $%.2f\n",
                   customers[i].accountNumber,
                   customers[i].name,
                   customers[i].phone,
                   customers[i].balance);
            count++;
        }
    }
    if (count == 0) {  // If no active customers
        printf("No customers available\n");
    }
    printf("========================================\n");
}

int customerLogin(void) {
    printf("\n=== Customer Login ===\n");
    printf("Please enter account number: ");
    int accNum;
    if (scanf("%d", &accNum) != 1) {  // Check if input is a number
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return -1;  // Login failed
    }
    clearInputBuffer();
    int index = findCustomerByAccount(accNum);  // Find customer's position
    if (index != -1 && customers[index].active) {  // If account exists and active
        printf("✓ Login successful! Welcome, %s\n", customers[index].name);
        return index;  // Return customer's position (for later use)
    }
    printf("❌ Account does not exist or is deactivated!\n");
    return -1;  // Login failed
}

void depositMoney(int custIndex) {
    printf("\n=== Deposit Funds ===\n");
    printf("Current balance: $%.2f\n", customers[custIndex].balance);
    printf("Please enter deposit amount: $");
    double amount;
    if (scanf("%lf", &amount) != 1 || amount <= 0) {  // Must enter positive number
        clearInputBuffer();
        printf("❌ Invalid amount!\n");
        return;
    }
    clearInputBuffer();  // Clean buffer
    customers[custIndex].balance += amount;  // Add money to balance
    printf("\n✓ Deposit successful!\n");
    printf("Deposit Amount: $%.2f\n", amount);
    printf("New Balance: $%.2f\n", customers[custIndex].balance);
    saveData();  // Save change to file
}

void viewSalesStats(void) {
    int totalOrders = 0;  // Total number of orders
    double totalRevenue = 0.0;  // Total money made
    int drinkSales[MAX_DRINKS] = {0};  // Count how many times each drink was sold

    // Compute statistics across all customers
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].active) {  // Only check active customers
            for (int j = 0; j < customers[i].orderCount; j++) {
                totalOrders++;  // Add 1 to total orders
                totalRevenue += customers[i].orderHistory[j].totalCost;  // Add order cost to revenue
                int drinkIndex = findDrinkById(customers[i].orderHistory[j].drinkId);
                if (drinkIndex != -1) drinkSales[drinkIndex]++;  // Count drink sales
            }
        }
    }

    // Print results
    printf("\n========================================\n");
    printf(" SALES STATISTICS\n");
    printf("========================================\n");
    printf("Total Orders: %d\n", totalOrders);
    printf("Total Revenue: $%.2f\n", totalRevenue);
    if (totalOrders > 0) {  // Only show average if there are orders
        printf("Average Order Amount: $%.2f\n", totalRevenue / totalOrders);
    }
    printf("\nDrink Sales Ranking:\n");
    printf("----------------------------------------\n");
    int hasData = 0;  // Check if there's any sales data
    for (int i = 0; i < drinkCount; i++) {
        if (drinks[i].available && drinkSales[i] > 0) {  // Show available drinks with sales
            printf("%-20s: %d servings\n", drinks[i].name, drinkSales[i]);
            hasData = 1;
        }
    }
    if (!hasData) {  // If no sales yet
        printf("No sales data available\n");
    }
    printf("========================================\n");
}

// ==================== System Settings Functions ====================
void changeManagerPassword(void) {
    char oldPassword[50];
    char newPassword[50];
    char confirmPassword[50];
    printf("\n========================================\n");
    printf(" CHANGE MANAGER PASSWORD\n");
    printf("========================================\n");
    printf("Please enter current password: ");
    scanf("%s", oldPassword);
    clearInputBuffer();  // Clean buffer
    if (strcmp(oldPassword, managerPassword) != 0) {  // Check if old password is correct
        printf("\n❌ Current password is incorrect!\n");
        return;
    }
    printf("Please enter new password (at least 6 characters): ");
    scanf("%s", newPassword);
    clearInputBuffer();  // Clean buffer
    if (strlen(newPassword) < 6) {  // Password must be 6+ characters
        printf("\n❌ Password too short! At least 6 characters required.\n");
        return;
    }
    printf("Please re-enter new password: ");
    scanf("%s", confirmPassword);
    clearInputBuffer();  // Clean buffer
    if (strcmp(newPassword, confirmPassword) != 0) {  // Check if new passwords match
        printf("\n❌ Passwords do not match!\n");
        return;
    }
    strcpy(managerPassword, newPassword);  // Update to new password
    printf("\n✓ Password changed successfully!\n");
    printf("Please keep your new password safe.\n");
    saveData();  // Save change to file
}