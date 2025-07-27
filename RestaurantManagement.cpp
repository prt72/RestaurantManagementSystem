#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  // For setting the precision of output
#include <limits>   // For input validation
#include <map>      // For grouping menu items by category

using namespace std;

// Structure to store Menu items
struct MenuItem {
    string name;
    double price;
    string category;
};

// Structure to store Patron information
struct Patron {
    int tableNo;
    int numPax;
    vector<MenuItem> order;
};

// Global variables to store the menu and total sales
// Note: Using globals for simplicity. Consider passing as references for better practice.
vector<MenuItem> menu;
double totalSalesDay = 0.0;
double totalSalesMonth = 0.0;
vector<Patron> patrons; // Global list of patrons

// Function declarations (prototypes)
void calculateAndAddCharges(const Patron& patron);

// Function to calculate, display charges for a patron's order, and update total sales
void calculateAndAddCharges(const Patron& patron) {
    double totalOrderCost = 0.0;
    cout << "\n--- Bill for Table " << patron.tableNo << " (" << patron.numPax << " pax) ---" << endl;
    
    if (patron.order.empty()) {
        cout << "No items ordered." << endl;
    } else {
        for (const auto& item : patron.order) {
            totalOrderCost += item.price;
            cout << item.name << " - RM " << fixed << setprecision(2) << item.price << endl;
        }
    }

    cout << "----------------------------------------" << endl;
    cout << "Total charges for this table: RM " << fixed << setprecision(2) << totalOrderCost << endl;
    cout << "----------------------------------------\n" << endl;
    
    // Update the daily and monthly sales ONCE when the bill is calculated/finalized
    totalSalesDay += totalOrderCost;
    totalSalesMonth += totalOrderCost;
}

// Function to initialize the menu with sample items
void initializeMenu() {
    menu.push_back({"Spaghetti Carbonara", 18.50, "Main Course"});
    menu.push_back({"Caesar Salad", 12.00, "Appetizer"});
    menu.push_back({"Margherita Pizza", 16.00, "Main Course"});
    menu.push_back({"Chocolate Cake", 8.00, "Dessert"});
    menu.push_back({"Cappuccino", 6.50, "Beverage"});
    // Add more items as needed
}

// Function to display the menu with SEQUENTIAL item numbers and grouped by category
// Returns a vector mapping display number to menu index
vector<int> displayMenu() {
    if (menu.empty()) {
        cout << "Menu is currently empty." << endl;
        return {}; // Return empty vector
    }

    cout << "\n=== Restaurant Menu ===" << endl;
    
    // Get unique categories in alphabetical order for grouping
    map<string, bool> uniqueCategories;
    for (const auto& item : menu) {
        uniqueCategories[item.category] = true;
    }
    
    vector<int> displayToIndexMap; // Maps display number (0-based) to menu index
    int displayNumber = 1;

    // Display items grouped by category (alphabetical order of categories)
    // AND build the mapping vector
    for (const auto& catPair : uniqueCategories) {
        const string& categoryName = catPair.first;
        cout << "\n-- " << categoryName << " --" << endl;
        for (size_t i = 0; i < menu.size(); ++i) {
            if (menu[i].category == categoryName) {
                cout << displayNumber << ". " << menu[i].name << " - RM "
                     << fixed << setprecision(2) << menu[i].price << endl;
                displayToIndexMap.push_back(i); // Map displayNumber-1 to menu index i
                displayNumber++;
            }
        }
    }
    cout << "\nPlease enter the number corresponding to the item you wish to order." << endl;
    return displayToIndexMap; // Return the mapping for use in registerPatron
}


// Function to safely get an integer input from the user
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            // Clear any remaining characters in the input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter a number." << endl;
            // Clear the error flag
            cin.clear();
            // Ignore the invalid input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


// Function to register patrons and take their orders
void registerPatron() {
    Patron newPatron;
    
    newPatron.tableNo = getIntegerInput("Enter Table Number: ");
    newPatron.numPax = getIntegerInput("Enter number of patrons at the table: ");

    // Display the menu and get the mapping, let the user select food items
    vector<int> itemMap = displayMenu(); // Get the mapping from display numbers to menu indices

    if (itemMap.empty()) {
        cout << "Cannot take order, menu is empty." << endl;
        return;
    }

    int numItems = getIntegerInput("Enter the number of items you want to order: ");

    for (int i = 0; i < numItems; ++i) {
        int choice = getIntegerInput("Enter the item number you want to order: ");
        // Check if choice is valid based on the displayed numbers (1 to itemMap.size())
        if (choice >= 1 && choice <= static_cast<int>(itemMap.size())) {
            // Use the mapping: choice-1 gives index in itemMap, itemMap[choice-1] gives index in menu
            int menuIndex = itemMap[choice - 1]; 
            newPatron.order.push_back(menu[menuIndex]);
            cout << "Added: " << menu[menuIndex].name << endl;
        } else {
            cout << "Invalid choice (" << choice << "). Please select a number between 1 and " << itemMap.size() << "." << endl;
            --i; // Decrement i to retry the current item input
        }
    }

    patrons.push_back(newPatron);  // Add patron to the list
    cout << "Order registered for Table " << newPatron.tableNo << "." << endl;
    
    // Calculate and display charges immediately after order is taken
    // This ensures sales are updated once per order
    calculateAndAddCharges(newPatron); 
}

// Function to display total sales for the day and month
void displayTotalSales() {
    cout << "\n=== Total Sales Summary ===" << endl;
    cout << "Total Sales for Today: RM " << fixed << setprecision(2) << totalSalesDay << endl;
    cout << "Total Sales for This Month: RM " << fixed << setprecision(2) << totalSalesMonth << endl;
    cout << "============================\n" << endl;
}

int main() {
    // Initialize the menu with sample items
    initializeMenu();

    int choice;

    while (true) {
        // Display Main Menu Options
        cout << "\n=== Restaurant Management System ===" << endl;
        cout << "1. Register Patron and Take Order" << endl;
        cout << "2. Display Total Sales" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        
        choice = getIntegerInput(""); // Use helper for validation

        switch (choice) {
            case 1:
                registerPatron(); // Handles order taking and initial charge calculation
                break;
            case 2:
                displayTotalSales(); // Only displays the accumulated totals
                break;
            case 3:
                cout << "\nExiting program. Goodbye!" << endl;
                return 0;
            default:
                cout << "\nInvalid choice. Please select 1, 2, or 3." << endl;
        }
    }

    return 0; // This line is technically unreachable due to the while(true) loop
}