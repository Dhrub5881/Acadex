#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include "dsa.h"

using namespace std;

/*
 * Acadex terminal prototype:
 * - OOP in C++: User, Student, CR, Admin
 * - DSA in C: linked list of notes + stack of notifications
 * - Data is in memory and resets when the program closes.
 */

string cleanLower(string value) {
    for (char &ch : value) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    return value;
}

string ask(const string &prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

class User {
protected:
    string username;
    string password;
    string fullName;
    string role;

public:
    User(string u, string p, string name, string r)
        : username(u), password(p), fullName(name), role(r) {}

    virtual ~User() = default;

    bool authenticate(const string &u, const string &p) const {
        return username == u && password == p;
    }

    string getUsername() const { return username; }
    string getFullName() const { return fullName; }
    string getRole() const { return role; }

    virtual void showMenu() const = 0;
};

class Student : public User {
private:
    string section;

public:
    Student(string u, string p, string name, string sec)
        : User(u, p, name, "Student"), section(sec) {}

    string getSection() const { return section; }

    void showMenu() const override {
        cout << "\n--- STUDENT MENU ---\n"
             << "1. View my section notes\n"
             << "2. View notifications\n"
             << "3. Logout\n";
    }
};

class CR : public User {
private:
    string assignedSection;

public:
    CR(string u, string p, string name, string sec)
        : User(u, p, name, "CR"), assignedSection(sec) {}

    string getAssignedSection() const { return assignedSection; }

    void showMenu() const override {
        cout << "\n--- CR MENU ---\n"
             << "1. View my section notes\n"
             << "2. Add note to my section\n"
             << "3. View all sections (read-only)\n"
             << "4. View notifications\n"
             << "5. Logout\n";
    }
};

class Admin : public User {
public:
    Admin(string u, string p, string name)
        : User(u, p, name, "Admin") {}

    void showMenu() const override {
        cout << "\n--- ADMIN MENU ---\n"
             << "1. View all notes\n"
             << "2. Add note to any section\n"
             << "3. View notifications\n"
             << "4. Logout\n";
    }
};

void showHeader() {
    cout << "\n====================================\n"
         << "             ACADEX\n"
         << "      Terminal Note Management\n"
         << "====================================\n";
}

int main() {
    Student student("student", "stu123", "Demo Student", "CSE-A");
    CR cr("cr", "cr123", "Demo CR", "CSE-A");
    Admin admin("admin", "admin123", "System Admin");

    NoteNode *notes = nullptr;
    int nextNoteId = 3;
    notes = add_note(notes, 1, "CSE-A", "DSA", "Introduction to Stack",
                     "A stack follows LIFO: Last In, First Out.");
    notes = add_note(notes, 2, "CSE-B", "OOP", "Classes and Objects",
                     "A class is a blueprint; an object is an instance.");

    NotificationStack notifications;
    stack_init(&notifications);
    stack_push(&notifications, "Welcome to Acadex.");
    stack_push(&notifications, "New DSA note added for section CSE-A.");

    bool running = true;
    while (running) {
        showHeader();
        cout << "1. Student Login\n"
             << "2. CR Login\n"
             << "3. Admin Login\n"
             << "4. Exit\n";

        string choice = ask("\nEnter your choice: ");

        if (choice == "4") {
            running = false;
            continue;
        }

        User *loggedIn = nullptr;
        string username = ask("Username: ");
        string password = ask("Password: ");

        if (choice == "1" && student.authenticate(username, password)) loggedIn = &student;
        else if (choice == "2" && cr.authenticate(username, password)) loggedIn = &cr;
        else if (choice == "3" && admin.authenticate(username, password)) loggedIn = &admin;

        if (loggedIn == nullptr) {
            cout << "\nInvalid login or role selection. Please try again.\n";
            continue;
        }

        cout << "\nLogin successful. Welcome, " << loggedIn->getFullName()
             << " (" << loggedIn->getRole() << ").\n";

        bool loggedInLoop = true;
        while (loggedInLoop) {
            loggedIn->showMenu();
            string action = ask("Enter your choice: ");

            if (Student *s = dynamic_cast<Student *>(loggedIn)) {
                if (action == "1") {
                    print_notes_for_section(notes, s->getSection().c_str());
                } else if (action == "2") {
                    stack_print_latest_first(&notifications);
                } else if (action == "3") {
                    loggedInLoop = false;
                } else {
                    cout << "Invalid option.\n";
                }
            } else if (CR *c = dynamic_cast<CR *>(loggedIn)) {
                if (action == "1") {
                    print_notes_for_section(notes, c->getAssignedSection().c_str());
                } else if (action == "2") {
                    string subject = ask("Subject: ");
                    string title = ask("Note title: ");
                    string content = ask("Note content (one line): ");
                    notes = add_note(notes, nextNoteId++, c->getAssignedSection().c_str(),
                                     subject.c_str(), title.c_str(), content.c_str());
                    string notification = "New note added to " + c->getAssignedSection()
                                        + ": " + title;
                    stack_push(&notifications, notification.c_str());
                    cout << "Note added to your assigned section.\n";
                } else if (action == "3") {
                    print_all_notes(notes);
                    cout << "\nRead-only view: CRs cannot edit another section.\n";
                } else if (action == "4") {
                    stack_print_latest_first(&notifications);
                } else if (action == "5") {
                    loggedInLoop = false;
                } else {
                    cout << "Invalid option.\n";
                }
            } else if (dynamic_cast<Admin *>(loggedIn) != nullptr) {
                if (action == "1") {
                    print_all_notes(notes);
                } else if (action == "2") {
                    string section = ask("Target section (e.g. CSE-A or CSE-B): ");
                    string subject = ask("Subject: ");
                    string title = ask("Note title: ");
                    string content = ask("Note content (one line): ");
                    notes = add_note(notes, nextNoteId++, section.c_str(),
                                     subject.c_str(), title.c_str(), content.c_str());
                    string notification = "Admin added a note to " + section + ": " + title;
                    stack_push(&notifications, notification.c_str());
                    cout << "Note added.\n";
                } else if (action == "3") {
                    stack_print_latest_first(&notifications);
                } else if (action == "4") {
                    loggedInLoop = false;
                } else {
                    cout << "Invalid option.\n";
                }
            }
        }
    }

    free_notes(notes);
    cout << "\nLogged out. Thanks for using Acadex!\n";
    return 0;
}
