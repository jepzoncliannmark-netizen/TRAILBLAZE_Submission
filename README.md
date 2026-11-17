# TRAILBLAZE – Outdoor Adventure Shop

**Tagline:** Explore More. Adventure Beyond.  
**Project:** C++ console-based e-commerce inventory and search engine  
**Course context:** Data Structure and Algorithm Analysis (DSA) Final Output

> The North Face, Columbia, Coleman, and Naturehike are included as sample catalog brands for an academic demonstration. This project does not claim official commercial partnerships or endorsement.

## 1. Project overview

TRAILBLAZE is a menu-driven C++ application that demonstrates an outdoor-adventure product catalog, inventory management, a linked-list shopping cart, FIFO checkout requests, product searching, sorting, and all eight requested DSA laboratory topics.

The application starts with 16 sample products. Inventory changes made through the administrator menu are held in memory for the current program run; this version does not save changes to a database or file after the program closes.

## 2. Main features

1. View the complete product inventory.
2. Filter products by brand.
3. Search product names, categories, and brands using linear search.
4. Search a price range using binary-search boundaries on sorted price entries and display the comparison count.
5. Manage a shopping cart using a doubly linked list, including next/previous navigation, removal, and totals.
6. Submit cart items as checkout requests and process them in FIFO order. Stock is deducted only after a successful request.
7. Add, update, and delete inventory products.
8. Undo the latest supported administrator action using a LIFO stack.
9. Display the four sample brands using a circular linked list for exactly two rounds.
10. Sort the inventory using insertion sort (price or stock descending) and Quick Sort (name or brand).
11. Search product IDs using a Binary Search Tree rebuilt from the current inventory.
12. Open a separate laboratory demonstration submenu.

## 3. Product structure

```cpp
struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};
```

## 4. Laboratory integration map

| Lab | Topic | Implementation in this project |
|---|---|---|
| 1 | Dynamic memory allocation | `Inventory` stores products in a dynamically allocated array, expands capacity, copies records, and releases old storage with `delete[]`. The required `pop_back_dynamic(int*& arr, int& size)` is included and demonstrated. |
| 2 | Singly and doubly linked lists | `SinglyList` supports append, traversal, and `getLastElement()` (returns `-1` when empty). `ShoppingCart` is a doubly linked list with `prev` and `next` navigation. |
| 3 | Circular linked list | `CircularBrandList` links its tail to its head and prints exactly eight nodes: four brands for two rounds. |
| 4 | Stack / balanced parentheses | `UndoStack` is a linked LIFO stack for add, update, and delete undo actions. `isBalanced(string expr)` separately checks `()`, `[]`, and `{}`. |
| 5 | Queue | `queue<CheckoutRequest>` processes customer requests FIFO. Each request validates product and stock before deducting inventory. |
| 6 | Binary Search Tree | `ProductBST` inserts product IDs and provides `searchBST()`. It is rebuilt from the current inventory before searches to avoid a stale index after updates or sorting. |
| 7 | Searching | Linear search checks product name, category, and brand. Price range search sorts `(price, productIndex)` entries, then uses binary-search boundaries and counts comparisons. |
| 8 | Sorting | Insertion sort orders by price or stock descending. Quick Sort orders by product name or brand. Whole `Product` records are moved so their fields remain together. |

## 5. Build requirements

- C++17-compatible compiler (G++ / MinGW-w64 / MSYS2)
- Visual Studio Code or another C++ editor
- Windows PowerShell terminal (commands below are for Windows)

## 6. Folder contents

```text
TRAILBLAZE/
├── main.cpp
└── README.md
```

## 7. Compile and run in VS Code

Open the project folder in VS Code. Open **Terminal → New Terminal**, then run:

```powershell
g++ --version
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o trailblaze.exe
.\trailblaze.exe
```

If the compile command finishes without errors, run the executable with the final command. The application is console-based; enter the menu number and press ENTER.

## 8. Main menu

```text
1. View Complete Product Inventory
2. Filter Products by Brand
3. Search Products by Keyword
4. Search Products by Price Range
5. Shopping Cart Management
6. Customer Checkout Queue
7. Administrator Inventory Management
8. Undo Last Administrative Action
9. Partner Brand Showcase
10. Sort Product Inventory
11. BST Product ID Search
12. Laboratory Demonstrations
13. Exit
```

## 9. Suggested demonstration sequence

1. Select option 1 and show the initial catalog.
2. Select option 2 and filter by `Coleman`.
3. Select option 3 and search `hiking`.
4. Select option 4 and enter a minimum and maximum price; show the comparison count.
5. Select option 5, add product `1001`, view the cart, and demonstrate next/previous navigation if there are multiple items.
6. Select option 6, submit the cart, view pending requests, and process the queue. Verify that stock decreases only after successful processing.
7. Select option 7 and add, update, or delete a product; then select option 8 to undo the most recent action.
8. Select option 9 and show exactly two rounds of the four sample brands.
9. Select option 10 and demonstrate each sorting option.
10. Select option 11 and search for an existing ID such as `1001`, then a non-existing ID such as `9999`.
11. Select option 12 and demonstrate Labs 1–8.

## 10. Testing checklist

Run these tests on the computer used for submission and record the actual result. Do not mark a test as passed unless you have executed it.

| Test | Suggested input/action | Expected behavior |
|---|---|---|
| Initial catalog | Main menu option 1 | 16 sample products display. |
| Brand filter | `Coleman` | Only Coleman catalog records display. |
| Linear search | `hiking` | Matching name/category/brand records display. |
| Price range | Minimum `1000`, maximum `3000` | Products in the inclusive range display; comparison count is shown. |
| Cart quantity validation | Add more than available stock | Request is rejected; inventory is unchanged. |
| Cart navigation | Add two different products | Next and previous move through cart nodes. |
| FIFO checkout | Submit multiple cart requests | Requests process in submission order. |
| Successful checkout | Request an available quantity | Stock decreases and receipt displays. |
| Failed checkout | Request more than available stock | Request fails and stock does not become negative. |
| Admin add/undo | Add a unique product, then undo | Newly added product is removed. |
| Admin delete/undo | Delete a product, then undo | Deleted record is restored. |
| Admin update/undo | Change product details, then undo | Previous record is restored. |
| Circular list | Brand showcase | Four brands display in exactly two rounds. |
| BST | Search `1001` and `9999` | First is found; second is not found. |
| Lab 1 | Laboratory demo 1 | Dynamic array resizes and removes its final element. |
| Lab 2 | Laboratory demo 2 / cart | SLL last element and empty result `-1` display; cart supports two-way navigation. |
| Lab 4 | Parentheses demo | Balanced and unbalanced examples are classified. |
| Lab 8 | Sorting submenu | Requested order is displayed and product fields stay together. |

## 11. Memory management and limitations

- The inventory owns a dynamic `Product[]` and releases it with `delete[]`.
- Singly linked list, doubly linked cart, circular brand list, undo stack, and BST nodes are deleted by cleanup functions/destructors.
- Checkout queue records are value objects managed by the standard library queue.
- The BST is rebuilt from the current product records before a search, so additions, deletions, and sorting do not leave a stale ID index.
- This is an in-memory academic prototype. It has no authentication, payment gateway, network service, persistent database, or real partner-brand relationship.
- The undo stack supports inventory add, update, and delete operations. Checkout stock deductions are not included in the administrator undo history.

## 12. Final submission reminder

Before submitting, compile and run the program on your own machine, execute the suggested tests, take screenshots of real program output, and follow any additional formatting or report requirements from your instructor. The README and source code are project materials; they do not replace any separate written report, presentation, or demonstration your instructor may require.
