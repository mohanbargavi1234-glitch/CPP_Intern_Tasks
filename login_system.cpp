// File: login_system.cpp
// Description: Login and Registration system with users.txt file for storing credentials.

#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;


string readPassword() {
    string password;

#ifdef _WIN32
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) break;         
        else if (ch == 8) {          
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
#endif

    return password;
}
/* ============================================ */

class UserManager {
private:
    string usersFile;

public:
    UserManager() : usersFile("users.txt") {}

    bool isUsernameExists(const string &username) {
        ifstream file(usersFile);
        if (!file.is_open()) return false;

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                if (line.substr(0, pos) == username) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

    bool registerUser(const string &username, const string &password) {
        if (username.empty() || password.empty() ||
            username.length() < 3 || password.length() < 4) {
            cout << "Error: Username (min 3 chars) and Password (min 4 chars) required.\n";
            return false;
        }

        if (isUsernameExists(username)) {
            cout << "Error: Username '" << username << "' already exists. Choose another.\n";
            return false;
        }

        ofstream file(usersFile, ios::app);
        if (!file.is_open()) {
            cout << "Error: Cannot open users.txt file.\n";
            return false;
        }

        file << username << ":" << password << "\n";
        file.close();

        cout << "✅ Registration successful for '" << username << "'!\n";
        return true;
    }

    bool loginUser(const string &username, const string &password) {
        ifstream file(usersFile);
        if (!file.is_open()) {
            cout << "❌ No users registered yet. Please register first.\n";
            return false;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string user = line.substr(0, pos);
                string pass = line.substr(pos + 1);

                if (user == username && pass == password) {
                    file.close();
                    cout << "✅ Login successful! Welcome, " << username << "!\n";
                    return true;
                }
            }
        }

        file.close();
        cout << "❌ Invalid username or password.\n";
        return false;
    }

    void showRegisteredUsers() {
        ifstream file(usersFile);
        if (!file.is_open()) {
            cout << "No users registered yet.\n";
            return;
        }

        cout << "\n📋 Registered Users:\n";
        string line;
        int count = 1;

        while (getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                cout << count++ << ". " << line.substr(0, pos) << "\n";
            }
        }
        file.close();
    }
};

int main() {
    UserManager manager;
    string input;

    cout << "\n\n🔐 Login & Registration System\n";
    cout << "==============================\n";

    while (true) {
        cout << "\n1. Register\n";
        cout << "2. Login\n";
        cout << "3. Show Registered Users\n";
        cout << "4. Exit\n";
        cout << "Enter choice (1-4): ";

        getline(cin, input);

        int choice;
        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        if (choice == 1) {
            string username, password;
            cout << "\n--- Registration ---\n";
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            password = readPassword();   
            manager.registerUser(username, password);
        }
        else if (choice == 2) {
            string username, password;
            cout << "\n--- Login ---\n";
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            password = readPassword();  
            manager.loginUser(username, password);
        }
        else if (choice == 3) {
            manager.showRegisteredUsers();
        }
        else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }

        cout << "\nPress Enter to continue...";
        getline(cin, input);
    }

    return 0;
}
