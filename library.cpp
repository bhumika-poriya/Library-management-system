#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isAvailable;
    string issuedTo;

    Book() : id(0), title(""), author(""), isAvailable(true), issuedTo("none") {}
};

vector<Book> bookshelf;

void loadData() {
    ifstream file("library.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Book book;
            ss >> book.id;
            ss.ignore(); // To skip the comma after id
            getline(ss, book.title, ',');
            getline(ss, book.author, ',');
            ss >> book.isAvailable;
            ss.ignore(); // To skip the comma after isAvailable
            getline(ss, book.issuedTo);
            if (book.isAvailable) {
                book.issuedTo = "none";
            }
            bookshelf.push_back(book);
        }
        file.close();
    }
}

void saveData() {
    ofstream file("library.txt");
    if (file.is_open()) {
        for (const auto& book : bookshelf) {
            file << book.id << ",";
            file << book.title << ",";
            file << book.author << ",";
            file << book.isAvailable << ",";
            if (book.isAvailable) {
                file << "none";
            } else {
                file << book.issuedTo;
            }
            file << endl;
        }
        file.close();
    }
}

void displayBooks() {
    for (const auto& book : bookshelf) {
        cout << "ID: " << book.id << ", Title: " << book.title 
             << ", Author: " << book.author << ", Available: " 
             << (book.isAvailable ? "Yes" : "No") 
             << ", Issued to: " << book.issuedTo << endl;
    }
}

void issueBook(int id, const string& studentName) {
    for (auto& book : bookshelf) {
        if (book.id == id && book.isAvailable) {
            book.isAvailable = false;
            book.issuedTo = studentName;
            cout << "Book issued successfully!" << endl;
            return;
        }
    }
    cout << "Book not available or not found." << endl;
}

void returnBook(int id) {
    for (auto& book : bookshelf) {
        if (book.id == id && !book.isAvailable) {
            book.isAvailable = true;
            book.issuedTo = "none";
            cout << "Book returned successfully!" << endl;
            return;
        }
    }
    cout << "Book not found or already available." << endl;
}

// Add a new book to the system
void addBook() {
    Book newBook;
    cout << "Enter book ID: ";
    cin >> newBook.id;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, newBook.title);
    cout << "Enter book author: ";
    getline(cin, newBook.author);
    newBook.isAvailable = true;
    newBook.issuedTo = "none";
    bookshelf.push_back(newBook);
    cout << "Book added successfully!" << endl;
}

// Search for a book by title or ID
void searchBook() {
    string query;
    cout << "Enter book title or ID: ";
    cin.ignore();
    getline(cin, query);

    bool found = false;
    for (const auto& book : bookshelf) {
        if (book.title == query || to_string(book.id) == query) {
            cout << "Book found!" << endl;
            cout << "ID: " << book.id << endl;
            cout << "Title: " << book.title << endl;
            cout << "Author: " << book.author << endl;
            cout << "Status: " << (book.isAvailable ? "Available" : "Issued") << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Book not found!" << endl;
    }
}

// List all books in the library
void listBooks() {
    sort(bookshelf.begin(), bookshelf.end(), [](const Book& a, const Book& b) {
        return a.id < b.id;
    });

    cout << "List of all books:" << endl;
    for (const auto& book : bookshelf) {
        cout << "ID: " << book.id << endl;
        cout << "Title: " << book.title << endl;
        cout << "Author: " << book.author << endl;
        cout << "Status: " << (book.isAvailable ? "Available" : "Issued") << endl;
        cout << "Issued To: " << book.issuedTo << endl;
        cout << endl;
    }
}

// Delete a book from the system
void deleteBook() {
    int bookID;
    cout << "Enter book ID: ";
    cin >> bookID;

    for (auto it = bookshelf.begin(); it != bookshelf.end(); ++it) {
        if (it->id == bookID) {
            bookshelf.erase(it);
            cout << "Book deleted successfully!" << endl;
            return;
        }
    }
    cout << "Book not found!" << endl;
}

int main() {
    // Load data from file
    loadData();

    int choice;
    while (true) {
        cout << "Library Management System" << endl;
        cout << "1. Add a new book" << endl;
        cout << "2. Search for a book" << endl;
        cout << "3. Issue a book" << endl;
        cout << "4. Return a book" << endl;
        cout << "5. List all books" << endl;
        cout << "6. Delete a book" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBook();
                break;
            case 3: {
                int id;
                string studentName;
                cout << "Enter book ID to issue: ";
                cin >> id;
                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, studentName);
                issueBook(id, studentName);
                break;
            }
            case 4: {
                int id;
                cout << "Enter book ID to return: ";
                cin >> id;
                returnBook(id);
                break;
            }
            case 5:
                listBooks();
                break;
            case 6:
                deleteBook();
                break;
            case 7:
                cout << "Exiting the system. Goodbye!" << endl;
                saveData(); // Save data to file
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    saveData(); // Save data to file
    return 0;
}
