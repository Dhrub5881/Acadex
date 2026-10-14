ACADEX - TERMINAL PROTOTYPE
===========================

WHAT THIS DEMONSTRATES
- C++ OOP: User base class, Student/CR/Admin derived classes, encapsulation,
  inheritance, polymorphism (virtual showMenu), and runtime polymorphism.
- DSA written in C: singly linked list for notes; stack (LIFO) for notifications.
- Role rules: students see only their own section; CRs can add notes only to
  their assigned section and can view other sections read-only; admin can add
  notes to any section.
- Notifications are displayed newest first.

DEMO LOGIN
Student: username student | password stu123
CR:      username cr      | password cr123
Admin:   username admin   | password admin123

WINDOWS (MinGW GCC/G++)
Open PowerShell in this folder and run:

    gcc -c dsa.c -o dsa.o
    g++ -std=c++17 main.cpp dsa.o -o acadex.exe
    .\acadex.exe

If gcc/g++ are not recognized, install MinGW-w64 and add its bin directory to PATH,
or use the compiler configured in your IDE.

LINUX / macOS
    gcc -c dsa.c -o dsa.o
    g++ -std=c++17 main.cpp dsa.o -o acadex
    ./acadex

IMPORTANT LIMITATIONS
This is a runnable learning prototype. Data is stored in RAM and is lost when
the program exits. Login passwords are demo credentials stored in the source;
do not use this for real accounts. There is no SQL database, password hashing,
persistent audit log, or production-grade security in this version.
