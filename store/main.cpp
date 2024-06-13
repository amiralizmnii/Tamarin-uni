
#include <iostream>  
#include <sqlite3.h> // For SQLite database operations
#include <string>    
#include <vector>    

using namespace std; // Using the standard namespace

// Class representing a product
class Product {
public:
    string code;    // Product code
    string name;    // Product name
    double price;   // Product price
    int quantity;   // Available quantity

    // Constructor to initialize a Product object
    Product(const string &c, const string &n, double p, int q)
        : code(c), name(n), price(p), quantity(q) {}
};

// Class to manage the database connection and operations
class StoreDatabase {
private:
    sqlite3 *db; // Pointer to the SQLite database
    int rc;      // Return code for SQLite operations

public:
    // Constructor: Opens a connection to the database
    StoreDatabase(const string &dbName) {
        rc = sqlite3_open(dbName.c_str(), &db); // Open the database connection
        if (rc) { // Check if the connection was successful
            cerr << "Can't open database: " << sqlite3_errmsg(db) << endl; // Print error message
            exit(1); // Exit the program with an error code
        } else {
            cout << "Opened database successfully" << endl; // Print success message
        }
    }

    // closes the database connection
    ~StoreDatabase() {
        sqlite3_close(db); // Close the database connection
    }

    // how to get product details from the database using its code
    Product getProduct(const string &code) {
        string sql = "SELECT p.code, p.name, p.price, i.quantity "
                     "FROM products p JOIN inventory i ON p.code = i.code "
                     "WHERE p.code = ?"; // SQL query to get product details
        sqlite3_stmt *stmt; // Statement handle

        // Prepare the SQL statement
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) { // Check if the preparation was successful
            cerr << "SQL error: " << sqlite3_errmsg(db) << endl; // Print error message
            exit(1); // Exit the program with an error code
        }

        // Bind the product code to the SQL statement
        sqlite3_bind_text(stmt, 1, code.c_str(), -1, SQLITE_STATIC);
        
        // Execute the SQL statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) { // Check if a row was returned
            // Extract product details from the result row
            string code = (char *)sqlite3_column_text(stmt, 0);
            string name = (char *)sqlite3_column_text(stmt, 1);
            double price = sqlite3_column_double(stmt, 2);
            int quantity = sqlite3_column_int(stmt, 3);

            // Finalize the SQL statement
            sqlite3_finalize(stmt);

            // Return a Product object with the retrieved details
            return Product(code, name, price, quantity);
        } else {
            cerr << "Product not found" << endl; // Print error message if product not found
            sqlite3_finalize(stmt); // Finalize the SQL statement
            exit(1); // Exit the program with an error code
        }
    }

    // Method to update the inventory in the database
    void updateInventory(const string &code, int quantity) {
        string sql = "UPDATE inventory SET quantity = quantity - ? WHERE code = ?"; // SQL query to update inventory
        sqlite3_stmt *stmt; // Statement handle

        // Prepare the SQL statement
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) { // Check if the preparation was successful
            cerr << "SQL error: " << sqlite3_errmsg(db) << endl; // Print error message
            exit(1); // Exit the program with an error code
        }

        // Bind the quantity and product code to the SQL statement
        sqlite3_bind_int(stmt, 1, quantity);
        sqlite3_bind_text(stmt, 2, code.c_str(), -1, SQLITE_STATIC);

        // Execute the SQL statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) { // Check if the execution was successful
            cerr << "SQL error: " << sqlite3_errmsg(db) << endl; // Print error message
            exit(1); // Exit the program with an error code
        }

        // Finalize the SQL statement
        sqlite3_finalize(stmt);
    }
};

// Main function
int main() {
    StoreDatabase db("store.db"); // Create a StoreDatabase object and connect to the database
    vector<Product> cart; // Vector to store the products in the cart
    string code; // Variable to store the product code
    int quantity; // Variable to store the quantity of the product

    // Loop to get user input for the products they want to buy
    while (true) {
        cout << "Enter product code (or 'done' to finish): ";
        cin >> code; // Get the product code from the user
        if (code == "done") break; // Exit the loop if the user types "done"

        // Get the product details from the database
        Product product = db.getProduct(code);

        cout << "Enter quantity: ";
        cin >> quantity; // Get the quantity from the user

        // Check if the requested quantity is available in the inventory
        if (quantity <= product.quantity) {
            // Add the product to the cart
            cart.push_back(Product(product.code, product.name, product.price, quantity));
            // Update the inventory in the database
            db.updateInventory(code, quantity);
        } else {
            cout << "Not enough stock available" << endl; // Print error message if not enough stock
        }
    }

    double total = 0.0; // variable to store the total cost
    cout << "\nBill:\n"; // print the bill header

    // loop through the cart to calculate the total cost and print the bill
    for (const auto &item : cart) {
        double cost = item.price * item.quantity; // calculate the cost of the item
        cout << item.name << " x" << item.quantity << " = $" << cost << endl; // Print the item details
        total += cost; // Add the cost to the total
    }
    
    cout << "Total: $" << total << endl; // Print the total cost

    return 0;
}

