#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <limits>
#include <cctype>
#include <new>

using namespace std;

struct Product {
    int id{};
    string name;
    string category;
    string brand;
    double price{};
    int stockQuantity{};
};

string lowerText(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

string readLine(const string& prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

int readInt(const string& prompt, int minValue = numeric_limits<int>::min()) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            int value = stoi(s, &pos);
            if (pos == s.size() && value >= minValue) return value;
        } catch (...) {}
        cout << "Invalid input. Please enter a whole number";
        if (minValue != numeric_limits<int>::min()) cout << " greater than or equal to " << minValue;
        cout << ".\n";
    }
}

double readDouble(const string& prompt, double minValue = 0.0) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            double value = stod(s, &pos);
            if (pos == s.size() && value >= minValue) return value;
        } catch (...) {}
        cout << "Invalid input. Please enter a number not less than " << minValue << ".\n";
    }
}

void pauseScreen() {
    cout << "\nPress ENTER to continue...";
    string unused;
    getline(cin, unused);
}

void printHeader(const string& title) {
    cout << "\n===============================================================\n";
    cout << "                 TRAILBLAZE | " << title << "\n";
    cout << "===============================================================\n";
}

void printProduct(const Product& p) {
    cout << left << setw(7) << p.id << setw(27) << p.name.substr(0, 26)
         << setw(19) << p.brand.substr(0, 18) << "P " << right << setw(9)
         << fixed << setprecision(2) << p.price << "   " << setw(5) << p.stockQuantity << left << "\n";
}

void printProductTableHeader() {
    cout << left << setw(7) << "ID" << setw(27) << "PRODUCT NAME" << setw(19) << "BRAND"
         << right << setw(12) << "PRICE" << setw(8) << "STOCK" << left << "\n";
    cout << string(73, '-') << "\n";
}

// LAB 1: required dynamic-array removal demonstration.
void pop_back_dynamic(int*& arr, int& size) {
    if (size <= 0 || arr == nullptr) return;
    int newSize = size - 1;
    int* smaller = (newSize > 0) ? new int[newSize] : nullptr;
    for (int i = 0; i < newSize; ++i) smaller[i] = arr[i];
    delete[] arr;
    arr = smaller;
    size = newSize;
}

class Inventory {
    Product* data;
    int count;
    int capacity;
public:
    Inventory() : data(new Product[4]), count(0), capacity(4) {}
    ~Inventory() { delete[] data; }
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    int size() const { return count; }
    int getCapacity() const { return capacity; }
    Product* raw() { return data; }
    const Product* raw() const { return data; }

    int indexOf(int id) const {
        for (int i = 0; i < count; ++i) if (data[i].id == id) return i;
        return -1;
    }
    bool contains(int id) const { return indexOf(id) != -1; }
    Product* find(int id) {
        int i = indexOf(id);
        return i == -1 ? nullptr : &data[i];
    }
    const Product* find(int id) const {
        int i = indexOf(id);
        return i == -1 ? nullptr : &data[i];
    }
    void reserve(int needed) {
        if (needed <= capacity) return;
        int newCapacity = capacity;
        while (newCapacity < needed) newCapacity *= 2;
        Product* larger = new Product[newCapacity];
        for (int i = 0; i < count; ++i) larger[i] = data[i];
        delete[] data;
        data = larger;
        capacity = newCapacity;
    }
    bool add(const Product& p) {
        if (contains(p.id)) return false;
        reserve(count + 1);
        data[count++] = p;
        return true;
    }
    bool insertAt(int index, const Product& p) {
        if (contains(p.id)) return false;
        if (index < 0 || index > count) index = count;
        reserve(count + 1);
        for (int i = count; i > index; --i) data[i] = data[i - 1];
        data[index] = p;
        ++count;
        return true;
    }
    bool update(int id, const Product& replacement) {
        int i = indexOf(id);
        if (i == -1) return false;
        if (replacement.id != id && contains(replacement.id)) return false;
        data[i] = replacement;
        return true;
    }
    bool remove(int id, Product& removed, int& oldIndex) {
        int i = indexOf(id);
        if (i == -1) return false;
        removed = data[i];
        oldIndex = i;
        for (int j = i; j < count - 1; ++j) data[j] = data[j + 1];
        --count;
        return true;
    }
};

// LAB 2: singly linked list.
struct SNode { int value; SNode* next; explicit SNode(int v) : value(v), next(nullptr) {} };
class SinglyList {
    SNode* head = nullptr;
public:
    ~SinglyList() { clear(); }
    void append(int value) {
        SNode* n = new SNode(value);
        if (!head) { head = n; return; }
        SNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    int getLastElement() const {
        if (!head) return -1;
        SNode* cur = head;
        while (cur->next) cur = cur->next;
        return cur->value;
    }
    void display() const {
        if (!head) { cout << "List is empty.\n"; return; }
        for (SNode* cur = head; cur; cur = cur->next) cout << cur->value << (cur->next ? " -> " : " -> NULL\n");
    }
    void clear() { while (head) { SNode* next = head->next; delete head; head = next; } }
};

// LAB 2: doubly linked list used by the shopping cart.
struct CartNode {
    int productId;
    int quantity;
    CartNode* prev;
    CartNode* next;
    CartNode(int id, int q) : productId(id), quantity(q), prev(nullptr), next(nullptr) {}
};
class ShoppingCart {
    CartNode* head = nullptr;
    CartNode* tail = nullptr;
    CartNode* current = nullptr;
public:
    ~ShoppingCart() { clear(); }
    CartNode* find(int id) const {
        for (CartNode* p = head; p; p = p->next) if (p->productId == id) return p;
        return nullptr;
    }
    bool empty() const { return head == nullptr; }
    bool add(int id, int qty) {
        CartNode* existing = find(id);
        if (existing) { existing->quantity += qty; current = existing; return true; }
        CartNode* n = new CartNode(id, qty);
        if (!head) head = tail = current = n;
        else { tail->next = n; n->prev = tail; tail = current = n; }
        return true;
    }
    bool remove(int id) {
        CartNode* p = find(id);
        if (!p) return false;
        if (p->prev) p->prev->next = p->next; else head = p->next;
        if (p->next) p->next->prev = p->prev; else tail = p->prev;
        if (current == p) current = p->next ? p->next : p->prev;
        delete p;
        return true;
    }
    void next() { if (current && current->next) current = current->next; }
    void previous() { if (current && current->prev) current = current->prev; }
    void displayCurrent(const Inventory& inv) const {
        if (!current) { cout << "Cart is empty.\n"; return; }
        const Product* p = inv.find(current->productId);
        if (!p) { cout << "Cart item refers to a product that no longer exists.\n"; return; }
        cout << "Current item: " << p->name << " | Quantity: " << current->quantity << "\n";
    }
    void display(const Inventory& inv) const {
        if (empty()) { cout << "Shopping cart is empty.\n"; return; }
        cout << "\nCART ITEMS\n";
        double total = 0;
        for (CartNode* p = head; p; p = p->next) {
            const Product* product = inv.find(p->productId);
            if (!product) continue;
            double sub = product->price * p->quantity;
            cout << product->name << " | Qty: " << p->quantity << " | Subtotal: P " << fixed << setprecision(2) << sub << "\n";
            total += sub;
        }
        cout << "Cart total: P " << fixed << setprecision(2) << total << "\n";
    }
    template <typename F> void forEach(F fn) const { for (CartNode* p = head; p; p = p->next) fn(p->productId, p->quantity); }
    void clear() { while (head) { CartNode* n = head->next; delete head; head = n; } tail = current = nullptr; }
};

// LAB 3: circular linked list for partner brand showcase.
struct BrandNode { string brand; BrandNode* next; explicit BrandNode(const string& b) : brand(b), next(nullptr) {} };
class CircularBrandList {
    BrandNode* head = nullptr;
public:
    ~CircularBrandList() { clear(); }
    void append(const string& brand) {
        BrandNode* n = new BrandNode(brand);
        if (!head) { head = n; n->next = head; return; }
        BrandNode* tail = head;
        while (tail->next != head) tail = tail->next;
        tail->next = n; n->next = head;
    }
    void displayTwoRounds() const {
        if (!head) { cout << "No brands available.\n"; return; }
        BrandNode* cur = head;
        for (int i = 1; i <= 8; ++i) {
            if (i == 1 || i == 5) cout << "Round " << (i == 1 ? 1 : 2) << ":\n";
            cout << "  " << cur->brand << "\n";
            cur = cur->next;
        }
    }
    void clear() {
        if (!head) return;
        BrandNode* cur = head->next;
        while (cur != head) { BrandNode* next = cur->next; delete cur; cur = next; }
        delete head; head = nullptr;
    }
};

// LAB 4: linked stack for undo history.
enum class UndoType { ADD_PRODUCT, DELETE_PRODUCT, UPDATE_PRODUCT };
struct UndoAction { UndoType type; Product before; int index; int affectedId; };
struct StackNode { UndoAction action; StackNode* next; StackNode(const UndoAction& a, StackNode* n) : action(a), next(n) {} };
class UndoStack {
    StackNode* topNode = nullptr;
public:
    ~UndoStack() { clear(); }
    bool empty() const { return topNode == nullptr; }
    void push(const UndoAction& a) { topNode = new StackNode(a, topNode); }
    bool pop(UndoAction& a) {
        if (!topNode) return false;
        StackNode* old = topNode; a = old->action; topNode = old->next; delete old; return true;
    }
    void display() const {
        if (!topNode) { cout << "Undo history is empty.\n"; return; }
        cout << "Recent actions (newest first):\n";
        int i = 1;
        for (StackNode* p = topNode; p; p = p->next, ++i) {
            string label = p->action.type == UndoType::ADD_PRODUCT ? "Added product" : p->action.type == UndoType::DELETE_PRODUCT ? "Deleted product" : "Updated product";
            cout << i << ". " << label << " (ID " << p->action.affectedId << ")\n";
        }
    }
    void clear() { UndoAction a; while (pop(a)) {} }
};

// LAB 4: balanced parentheses checker using stack behavior.
bool isBalanced(string expr) {
    vector<char> st;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') st.push_back(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char open = st.back(); st.pop_back();
            if ((c == ')' && open != '(') || (c == ']' && open != '[') || (c == '}' && open != '{')) return false;
        }
    }
    return st.empty();
}

// LAB 5: FIFO checkout queue.
struct CheckoutRequest { string customerName; int productId; int quantity; };

// LAB 6: Binary Search Tree of product IDs.
struct BSTNode { int id; BSTNode* left; BSTNode* right; explicit BSTNode(int v) : id(v), left(nullptr), right(nullptr) {} };
class ProductBST {
    BSTNode* root = nullptr;
    BSTNode* insertNode(BSTNode* n, int id) {
        if (!n) return new BSTNode(id);
        if (id < n->id) n->left = insertNode(n->left, id);
        else if (id > n->id) n->right = insertNode(n->right, id);
        return n;
    }
    bool searchNode(BSTNode* n, int id) const {
        if (!n) return false;
        if (id == n->id) return true;
        return id < n->id ? searchNode(n->left, id) : searchNode(n->right, id);
    }
    void clearNode(BSTNode* n) { if (!n) return; clearNode(n->left); clearNode(n->right); delete n; }
public:
    ~ProductBST() { clear(); }
    void clear() { clearNode(root); root = nullptr; }
    void build(const Inventory& inv) { clear(); for (int i = 0; i < inv.size(); ++i) root = insertNode(root, inv.raw()[i].id); }
    bool searchBST(int id) const { return searchNode(root, id); }
};

// LAB 7: binary search on sorted price entries, retaining product indices.
struct PriceEntry { double price; int productIndex; };
int countBinarySteps(const vector<PriceEntry>& entries, double target, bool findFirst, int& comparisons) {
    int lo = 0, hi = static_cast<int>(entries.size()) - 1, answer = static_cast<int>(entries.size());
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        ++comparisons;
        bool condition = findFirst ? entries[mid].price >= target : entries[mid].price > target;
        if (condition) { answer = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return answer;
}

// LAB 8: insertion sort descending and quick sort.
void insertionSort(Product* arr, int n, bool byPrice) {
    for (int i = 1; i < n; ++i) {
        Product key = arr[i];
        int j = i - 1;
        while (j >= 0 && (byPrice ? arr[j].price < key.price : arr[j].stockQuantity < key.stockQuantity)) {
            arr[j + 1] = arr[j]; --j;
        }
        arr[j + 1] = key;
    }
}
int partitionProducts(Product* arr, int low, int high, bool byBrand) {
    string pivot = lowerText(byBrand ? arr[high].brand : arr[high].name);
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        string value = lowerText(byBrand ? arr[j].brand : arr[j].name);
        if (value <= pivot) { ++i; swap(arr[i], arr[j]); }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
void quickSort(Product* arr, int low, int high, bool byBrand) {
    if (low < high) {
        int pi = partitionProducts(arr, low, high, byBrand);
        quickSort(arr, low, pi - 1, byBrand);
        quickSort(arr, pi + 1, high, byBrand);
    }
}

void displayInventory(const Inventory& inv) {
    printHeader("PRODUCT INVENTORY");
    if (inv.size() == 0) { cout << "Inventory is empty.\n"; return; }
    printProductTableHeader();
    for (int i = 0; i < inv.size(); ++i) printProduct(inv.raw()[i]);
    cout << string(73, '-') << "\nTotal Products: " << inv.size() << " | Array capacity: " << inv.getCapacity() << "\n";
}

void seedProducts(Inventory& inv) {
    const Product samples[] = {
        {1001,"Alpine Camping Tent","Camping Equipment","Coleman",4500,12},
        {1002,"Quick-Dry Hiking Shirt","Outdoor Clothing","Columbia",1850,20},
        {1003,"Trekking Backpack 40L","Hiking and Trekking Gear","The North Face",6200,7},
        {1004,"Cloud-Up 2 Tent","Camping Equipment","Naturehike",5900,9},
        {1005,"Sleeping Bag -5C","Camping Equipment","Coleman",2800,15},
        {1006,"Waterproof Trail Jacket","Outdoor Clothing","The North Face",7800,5},
        {1007,"Hiking Pants","Outdoor Clothing","Columbia",2350,14},
        {1008,"Ultralight Camping Mat","Camping Equipment","Naturehike",1650,18},
        {1009,"LED Camping Lantern","Navigation and Lighting","Coleman",950,25},
        {1010,"Trekking Poles","Hiking and Trekking Gear","The North Face",3200,10},
        {1011,"Outdoor Travel Duffle","Travel Essentials","Columbia",2750,11},
        {1012,"Portable Gas Stove","Outdoor Cooking Equipment","Coleman",1450,13},
        {1013,"Lightweight Hiking Tarp","Camping Equipment","Naturehike",2100,8},
        {1014,"Emergency Survival Kit","Survival Equipment","The North Face",1950,6},
        {1015,"Insulated Water Bottle","Travel Essentials","Columbia",850,30},
        {1016,"Dry Bag 20L","Water Sports Accessories","Naturehike",1250,16}
    };
    for (const Product& p : samples) inv.add(p);
}

void filterByBrand(const Inventory& inv) {
    printHeader("FILTER BY BRAND");
    string brand = readLine("Enter brand (The North Face, Columbia, Coleman, Naturehike): ");
    int found = 0;
    printProductTableHeader();
    for (int i = 0; i < inv.size(); ++i) if (lowerText(inv.raw()[i].brand) == lowerText(brand)) { printProduct(inv.raw()[i]); ++found; }
    if (!found) cout << "No products found for that brand.\n";
    else cout << "Matching products: " << found << "\n";
}

void linearSearchProducts(const Inventory& inv) {
    printHeader("LINEAR PRODUCT SEARCH");
    string key = lowerText(readLine("Enter a product name, category, or brand keyword: "));
    int found = 0;
    for (int i = 0; i < inv.size(); ++i) {
        const Product& p = inv.raw()[i];
        if (lowerText(p.name).find(key) != string::npos || lowerText(p.category).find(key) != string::npos || lowerText(p.brand).find(key) != string::npos) {
            if (found == 0) printProductTableHeader();
            printProduct(p); ++found;
        }
    }
    if (!found) cout << "No matching products found.\n";
    else cout << "Linear search matches: " << found << "\n";
}

void priceRangeSearch(const Inventory& inv) {
    printHeader("BINARY SEARCH BY PRICE RANGE");
    double minPrice = readDouble("Minimum price (P): ");
    double maxPrice = readDouble("Maximum price (P): ");
    if (maxPrice < minPrice) { cout << "Invalid range: maximum must be at least the minimum.\n"; return; }
    vector<PriceEntry> entries;
    for (int i = 0; i < inv.size(); ++i) entries.push_back({inv.raw()[i].price, i});
    sort(entries.begin(), entries.end(), [](const PriceEntry& a, const PriceEntry& b) { return a.price < b.price; });
    int comparisons = 0;
    int first = countBinarySteps(entries, minPrice, true, comparisons);
    int afterLast = countBinarySteps(entries, maxPrice, false, comparisons);
    int found = 0;
    for (int i = first; i < afterLast; ++i) {
        if (found == 0) printProductTableHeader();
        printProduct(inv.raw()[entries[i].productIndex]); ++found;
    }
    if (!found) cout << "No products fall within that price range.\n";
    cout << "Binary-search comparisons: " << comparisons << "\n";
}

void addToCart(ShoppingCart& cart, const Inventory& inv) {
    int id = readInt("Enter product ID: ", 1);
    const Product* p = inv.find(id);
    if (!p) { cout << "Product ID not found.\n"; return; }
    int qty = readInt("Quantity: ", 1);
    CartNode* existing = cart.find(id);
    int already = existing ? existing->quantity : 0;
    if (qty + already > p->stockQuantity) { cout << "Requested cart quantity exceeds available stock.\n"; return; }
    cart.add(id, qty);
    cout << "Added to cart. Inventory has not been deducted.\n";
}

void cartMenu(ShoppingCart& cart, const Inventory& inv) {
    while (true) {
        printHeader("SHOPPING CART");
        cout << "1. Add product\n2. View cart and total\n3. Next cart item\n4. Previous cart item\n5. Display current item\n6. Remove cart item\n7. Clear cart\n8. Return\n";
        int choice = readInt("Choice: ", 1);
        if (choice == 1) addToCart(cart, inv);
        else if (choice == 2) cart.display(inv);
        else if (choice == 3) { cart.next(); cart.displayCurrent(inv); }
        else if (choice == 4) { cart.previous(); cart.displayCurrent(inv); }
        else if (choice == 5) cart.displayCurrent(inv);
        else if (choice == 6) { int id = readInt("Product ID to remove: ", 1); cout << (cart.remove(id) ? "Cart item removed.\n" : "Item not found in cart.\n"); }
        else if (choice == 7) { cart.clear(); cout << "Cart cleared.\n"; }
        else if (choice == 8) return;
        else cout << "Invalid choice.\n";
        pauseScreen();
    }
}

void submitCart(ShoppingCart& cart, queue<CheckoutRequest>& q) {
    if (cart.empty()) { cout << "Cart is empty; nothing to submit.\n"; return; }
    string customer = readLine("Customer name: ");
    if (customer.empty()) { cout << "Customer name is required.\n"; return; }
    cart.forEach([&](int id, int qty) { q.push({customer, id, qty}); });
    cart.clear();
    cout << "Checkout request(s) added to the FIFO queue.\n";
}

void processCheckout(queue<CheckoutRequest>& q, Inventory& inv) {
    if (q.empty()) { cout << "Checkout queue is empty.\n"; return; }
    while (!q.empty()) {
        CheckoutRequest r = q.front(); q.pop();
        cout << "\nProcessing customer: " << r.customerName << "\n";
        Product* p = inv.find(r.productId);
        if (!p) { cout << "FAILED: Product ID " << r.productId << " does not exist.\n"; continue; }
        if (r.quantity <= 0 || p->stockQuantity < r.quantity) { cout << "FAILED: Insufficient stock for " << p->name << ". Available: " << p->stockQuantity << "\n"; continue; }
        p->stockQuantity -= r.quantity;
        cout << "CHECKOUT RECEIPT\nProduct: " << p->name << "\nQuantity: " << r.quantity
             << "\nUnit price: P " << fixed << setprecision(2) << p->price
             << "\nTOTAL: P " << p->price * r.quantity << "\nRemaining stock: " << p->stockQuantity << "\nStatus: SUCCESS\n";
    }
}

void displayQueue(queue<CheckoutRequest> q) {
    if (q.empty()) { cout << "Checkout queue is empty.\n"; return; }
    cout << "Pending checkout requests (FIFO order):\n";
    int n = 1;
    while (!q.empty()) { const auto& r = q.front(); cout << n++ << ". " << r.customerName << " | Product ID: " << r.productId << " | Quantity: " << r.quantity << "\n"; q.pop(); }
}

void adminAdd(Inventory& inv, UndoStack& undo) {
    Product p;
    p.id = readInt("New product ID: ", 1);
    if (inv.contains(p.id)) { cout << "That product ID already exists.\n"; return; }
    p.name = readLine("Product name: ");
    p.category = readLine("Category: ");
    p.brand = readLine("Brand: ");
    p.price = readDouble("Price (P): ");
    p.stockQuantity = readInt("Stock quantity: ", 0);
    if (p.name.empty() || p.brand.empty() || p.category.empty()) { cout << "Name, category, and brand cannot be empty.\n"; return; }
    if (inv.add(p)) { undo.push({UndoType::ADD_PRODUCT, {}, -1, p.id}); cout << "Product added.\n"; }
}

void adminUpdate(Inventory& inv, UndoStack& undo) {
    int id = readInt("Product ID to update: ", 1);
    Product* current = inv.find(id);
    if (!current) { cout << "Product not found.\n"; return; }
    Product old = *current, replacement = old;
    cout << "Enter the new details. Press ENTER to keep the current text value.\n";
    string s = readLine("Name [" + old.name + "]: "); if (!s.empty()) replacement.name = s;
    s = readLine("Category [" + old.category + "]: "); if (!s.empty()) replacement.category = s;
    s = readLine("Brand [" + old.brand + "]: "); if (!s.empty()) replacement.brand = s;
    s = readLine("New price [" + to_string(old.price) + "] (blank keeps): ");
    if (!s.empty()) { try { size_t pos; double v = stod(s, &pos); if (pos != s.size() || v < 0) throw 1; replacement.price = v; } catch (...) { cout << "Invalid price; update cancelled.\n"; return; } }
    s = readLine("New stock [" + to_string(old.stockQuantity) + "] (blank keeps): ");
    if (!s.empty()) { try { size_t pos; int v = stoi(s, &pos); if (pos != s.size() || v < 0) throw 1; replacement.stockQuantity = v; } catch (...) { cout << "Invalid stock; update cancelled.\n"; return; } }
    if (inv.update(id, replacement)) { undo.push({UndoType::UPDATE_PRODUCT, old, inv.indexOf(id), id}); cout << "Product updated.\n"; }
}

void adminDelete(Inventory& inv, UndoStack& undo, ShoppingCart& cart) {
    int id = readInt("Product ID to delete: ", 1);
    Product removed; int index = -1;
    if (!inv.remove(id, removed, index)) { cout << "Product not found.\n"; return; }
    cart.remove(id);
    undo.push({UndoType::DELETE_PRODUCT, removed, index, id});
    cout << "Product deleted.\n";
}

void adminMenu(Inventory& inv, UndoStack& undo, ShoppingCart& cart) {
    while (true) {
        printHeader("ADMIN INVENTORY MANAGEMENT");
        cout << "1. Add product\n2. Update product\n3. Delete product\n4. View inventory\n5. View undo history\n6. Return\n";
        int c = readInt("Choice: ", 1);
        if (c == 1) adminAdd(inv, undo);
        else if (c == 2) adminUpdate(inv, undo);
        else if (c == 3) adminDelete(inv, undo, cart);
        else if (c == 4) displayInventory(inv);
        else if (c == 5) undo.display();
        else if (c == 6) return;
        else cout << "Invalid choice.\n";
        pauseScreen();
    }
}

void undoLast(Inventory& inv, UndoStack& undo) {
    UndoAction a;
    if (!undo.pop(a)) { cout << "Nothing to undo.\n"; return; }
    bool ok = false;
    if (a.type == UndoType::ADD_PRODUCT) { Product removed; int idx; ok = inv.remove(a.affectedId, removed, idx); }
    else if (a.type == UndoType::DELETE_PRODUCT) ok = inv.insertAt(a.index, a.before);
    else if (a.type == UndoType::UPDATE_PRODUCT) ok = inv.update(a.affectedId, a.before);
    cout << (ok ? "Last administrative action successfully undone.\n" : "Undo could not be completed because the inventory changed.\n");
}

void sortMenu(Inventory& inv) {
    while (true) {
        printHeader("INVENTORY SORTING");
        cout << "1. Insertion Sort - Price (Descending)\n2. Insertion Sort - Stock (Descending)\n3. Quick Sort - Product Name\n4. Quick Sort - Brand\n5. Display Current Inventory\n6. Return to Main Menu\n";
        int c = readInt("Choice: ", 1);
        if (c == 1) { insertionSort(inv.raw(), inv.size(), true); cout << "Sorted by price, descending.\n"; displayInventory(inv); }
        else if (c == 2) { insertionSort(inv.raw(), inv.size(), false); cout << "Sorted by stock, descending.\n"; displayInventory(inv); }
        else if (c == 3) { if (inv.size()) quickSort(inv.raw(), 0, inv.size() - 1, false); cout << "Sorted by product name.\n"; displayInventory(inv); }
        else if (c == 4) { if (inv.size()) quickSort(inv.raw(), 0, inv.size() - 1, true); cout << "Sorted by brand.\n"; displayInventory(inv); }
        else if (c == 5) displayInventory(inv);
        else if (c == 6) return;
        else cout << "Invalid choice.\n";
        pauseScreen();
    }
}

void lab1Demo() {
    printHeader("LAB 1 - DYNAMIC MEMORY ALLOCATION");
    int size = 4; int* values = new int[size]{10, 20, 30, 40};
    cout << "Original dynamic array: "; for (int i = 0; i < size; ++i) cout << values[i] << " "; cout << "\n";
    int* larger = new int[6]; for (int i = 0; i < size; ++i) larger[i] = values[i]; larger[4] = 50; larger[5] = 60;
    delete[] values; values = larger; size = 6;
    cout << "After resizing: "; for (int i = 0; i < size; ++i) cout << values[i] << " "; cout << "\n";
    pop_back_dynamic(values, size);
    cout << "After pop_back_dynamic: "; for (int i = 0; i < size; ++i) cout << values[i] << " "; cout << "\nNew size: " << size << "\n";
    delete[] values;
}
void lab2Demo(const Inventory& inv) {
    printHeader("LAB 2 - SINGLY AND DOUBLY LINKED LISTS");
    SinglyList ids;
    for (int i = 0; i < min(inv.size(), 5); ++i) ids.append(inv.raw()[i].id);
    cout << "Singly linked product IDs: "; ids.display();
    cout << "getLastElement(): " << ids.getLastElement() << "\n";
    cout << "Empty-list getLastElement(): "; SinglyList empty; cout << empty.getLastElement() << " (expected -1)\n";
    cout << "Doubly linked list behavior is demonstrated in the Shopping Cart menu using previous/next links.\n";
}
void lab3Demo() {
    printHeader("LAB 3 - CIRCULAR LINKED LIST");
    CircularBrandList brands; brands.append("The North Face"); brands.append("Columbia"); brands.append("Coleman"); brands.append("Naturehike");
    brands.displayTwoRounds();
}
void lab4Demo() {
    printHeader("LAB 4 - STACK AND BALANCED PARENTHESES");
    vector<string> tests = {"(A+B)", "([A+B]*{C})", "((A+B)", "([)]"};
    for (const string& s : tests) cout << s << " => " << (isBalanced(s) ? "Balanced" : "Not balanced") << "\n";
    cout << "The administrator undo history uses a linked stack (LIFO); use Admin Management to push actions and Undo to pop the latest.\n";
}
void lab5Demo() {
    printHeader("LAB 5 - FIFO QUEUE");
    queue<CheckoutRequest> demo;
    demo.push({"Customer A", 1001, 1}); demo.push({"Customer B", 1002, 2}); demo.push({"Customer C", 1003, 1});
    cout << "Demonstration processing order:\n";
    while (!demo.empty()) { cout << demo.front().customerName << " -> Product ID " << demo.front().productId << "\n"; demo.pop(); }
    cout << "Actual checkout requests are processed in FIFO order in the Customer Checkout Queue menu.\n";
}
void lab6Demo(const Inventory& inv) {
    printHeader("LAB 6 - BINARY SEARCH TREE");
    ProductBST tree; tree.build(inv);
    int id = readInt("Enter product ID to search in BST: ", 1);
    cout << "searchBST(" << id << "): " << (tree.searchBST(id) ? "true - ID exists" : "false - ID not found") << "\n";
}
void lab7Demo(const Inventory& inv) {
    printHeader("LAB 7 - SEARCHING ALGORITHMS");
    cout << "Linear search demo: search keyword 'camping'.\n";
    int found = 0;
    for (int i = 0; i < inv.size(); ++i) if (lowerText(inv.raw()[i].name).find("camping") != string::npos || lowerText(inv.raw()[i].category).find("camping") != string::npos) { cout << "  " << inv.raw()[i].id << " - " << inv.raw()[i].name << "\n"; ++found; }
    cout << "Linear matches: " << found << "\n";
    cout << "Use Price Range Search to run binary search on sorted price entries and display comparison counts.\n";
}
void lab8Demo(Inventory& inv) {
    printHeader("LAB 8 - SORTING ALGORITHMS");
    cout << "Running insertion sort by price descending on the inventory.\n";
    insertionSort(inv.raw(), inv.size(), true); displayInventory(inv);
    cout << "Running Quick Sort by product name.\n";
    if (inv.size()) quickSort(inv.raw(), 0, inv.size() - 1, false);
    displayInventory(inv);
}
void labMenu(Inventory& inv) {
    while (true) {
        printHeader("DSA LABORATORY DEMONSTRATIONS");
        cout << "1. Lab 1 - Dynamic Memory Allocation\n2. Lab 2 - Linked Lists\n3. Lab 3 - Circular Linked List\n4. Lab 4 - Stack and Parentheses Checker\n5. Lab 5 - Queue\n6. Lab 6 - Binary Search Tree\n7. Lab 7 - Linear and Binary Search\n8. Lab 8 - Insertion Sort and Quick Sort\n9. Return to Main Menu\n";
        int c = readInt("Choice: ", 1);
        if (c == 1) lab1Demo(); else if (c == 2) lab2Demo(inv); else if (c == 3) lab3Demo(); else if (c == 4) lab4Demo();
        else if (c == 5) lab5Demo(); else if (c == 6) lab6Demo(inv); else if (c == 7) lab7Demo(inv); else if (c == 8) lab8Demo(inv); else if (c == 9) return; else cout << "Invalid choice.\n";
        pauseScreen();
    }
}

void brandShowcase() {
    printHeader("PARTNER BRAND SHOWCASE");
    cout << "Sample catalog brands for academic demonstration (not a claim of official partnership):\n";
    CircularBrandList brands; brands.append("The North Face"); brands.append("Columbia"); brands.append("Coleman"); brands.append("Naturehike");
    brands.displayTwoRounds();
}

int main() {
    try {
        Inventory inventory;
        seedProducts(inventory);
        ShoppingCart cart;
        UndoStack undo;
        queue<CheckoutRequest> checkoutQueue;

        while (true) {
            printHeader("OUTDOOR ADVENTURE SHOP");
            cout << "                 EXPLORE MORE. ADVENTURE BEYOND.\n\n";
            cout << "1. View Complete Product Inventory\n2. Filter Products by Brand\n3. Search Products by Keyword\n4. Search Products by Price Range\n5. Shopping Cart Management\n6. Customer Checkout Queue\n7. Administrator Inventory Management\n8. Undo Last Administrative Action\n9. Partner Brand Showcase\n10. Sort Product Inventory\n11. BST Product ID Search\n12. Laboratory Demonstrations\n13. Exit\n";
            int choice = readInt("Enter your choice: ", 1);
            if (choice == 1) displayInventory(inventory);
            else if (choice == 2) filterByBrand(inventory);
            else if (choice == 3) linearSearchProducts(inventory);
            else if (choice == 4) priceRangeSearch(inventory);
            else if (choice == 5) cartMenu(cart, inventory);
            else if (choice == 6) {
                printHeader("CUSTOMER CHECKOUT QUEUE");
                cout << "1. Submit current cart\n2. View pending requests\n3. Process all requests\n4. Return\n";
                int c = readInt("Choice: ", 1);
                if (c == 1) submitCart(cart, checkoutQueue);
                else if (c == 2) displayQueue(checkoutQueue);
                else if (c == 3) processCheckout(checkoutQueue, inventory);
                else if (c != 4) cout << "Invalid choice.\n";
            }
            else if (choice == 7) adminMenu(inventory, undo, cart);
            else if (choice == 8) undoLast(inventory, undo);
            else if (choice == 9) brandShowcase();
            else if (choice == 10) sortMenu(inventory);
            else if (choice == 11) {
                printHeader("BST PRODUCT ID SEARCH");
                int id = readInt("Enter product ID: ", 1);
                ProductBST tree; tree.build(inventory);
                if (tree.searchBST(id)) { const Product* p = inventory.find(id); cout << "Product ID exists in BST.\n"; printProductTableHeader(); printProduct(*p); }
                else cout << "Product ID not found in BST.\n";
            }
            else if (choice == 12) labMenu(inventory);
            else if (choice == 13) { cout << "Thank you for visiting TRAILBLAZE. Happy trails!\n"; break; }
            else cout << "Invalid choice. Please select 1-13.\n";
            if (choice != 5 && choice != 7 && choice != 10 && choice != 12 && choice != 13) pauseScreen();
        }
    } catch (const bad_alloc&) {
        cerr << "Memory allocation failed. The program will now close safely.\n";
        return 1;
    }
    return 0;
}
