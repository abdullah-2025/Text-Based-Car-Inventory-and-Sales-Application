#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

// Class Vehicle is an abstract base class representing a vehicle.
class Vehicle {
protected:
    string name;
    string color;
    string model;

public:
    Vehicle() {}
    virtual ~Vehicle() {}

    // Pure virtual functions that need to be implemented in derived classes.
    virtual void get_data() = 0;
    virtual void write(const string& filename) = 0;
    virtual void read(const string& filename) = 0;
};

// Class Car is a derived class representing a car.
class Car : public Vehicle {
private:
    string price;
    static int counter;

public:
    Car() {}
    ~Car() {}

    void get_data() override {
        cout << "\nCar Name : ";
        cin.ignore();
        getline(cin, name);
        cout << "\nCar Model : ";
        getline(cin, model);
        cout << "\nCar Color : ";
        getline(cin, color);
        cout << "\nCar Price : ";
        getline(cin, price);
    }

    void write(const string& filename) override {
        ofstream outline(filename, ios::app);
        if (outline.is_open()) {
            outline << name << " " << model << " " << color << " " << price << endl;
            outline.close();
        } else {
            cout << "Error opening file to write car data.\n";
        }
    }

    void read(const string& filename) override {
        ifstream outline(filename);
        if (outline.is_open()) {
            while (outline >> name >> model >> color >> price) {
                cout << name << " " << model << " " << color << " " << price << endl;
            }
            outline.close();
        } else {
            cout << "Error opening file to read car data.\n";
        }
    }

    static int getCounter() {
        return counter;
    }

    static void increaseCounter() {
        counter++;
    }
};

int Car::counter = 0;

// Class Manager is responsible for managing car data.
class Manager {
public:
    Manager() {}
    ~Manager() {}

    void edit(const string& filename) {
        Car car;
        cout << "\nEnter Data For Car: \n";
        car.get_data();
        car.write(filename);
    }

    void delete_record(const string& filename) {
        ifstream inFile(filename);
        string line;
        int lineNumber = 1;

        if (!inFile.is_open()) {
            cout << "\nNo records to delete." << endl;
            return;
        }

        cout << "\nAvailable Records:\n";
        vector<string> lines;
        while (getline(inFile, line)) {
            cout << lineNumber << ". " << line << endl;
            lines.push_back(line);
            lineNumber++;
        }
        inFile.close();

        int lineToDelete;
        cout << "Enter the line number you want to delete: ";
        cin >> lineToDelete;

        ofstream tempFile("temp.txt");
        lineNumber = 1;
        for (const auto& record : lines) {
            if (lineNumber != lineToDelete) {
                tempFile << record << endl;
            }
            lineNumber++;
        }
        tempFile.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
        cout << "Record deleted successfully!" << endl;
    }
};

// Class Customer is responsible for managing customer data.
class Customer {
private:
    string name;
    string cnic;
    string ph_no;

public:
    Customer() {}
    ~Customer() {}

    void get_data() {
        cout << "Name: ";
        cin.ignore();  // Only call ignore when needed
        getline(cin, name);
        cout << "CNIC: ";
        getline(cin, cnic);
        cout << "Phone: ";
        getline(cin, ph_no);
    }

    void write() {
        ofstream outfile("customer_details.txt", ios::app);
        if (outfile.is_open()) {
            outfile << name << endl << cnic << endl << ph_no << endl;
            outfile.close();
        } else {
            cout << "Error opening file to write customer details.\n";
        }
    }

    void read() {
        ifstream outline("customer_details.txt");
        if (outline.is_open()) {
            while (getline(outline, name)) {
                cout << "Name: " << name << endl;
                getline(outline, cnic);
                cout << "CNIC: " << cnic << endl;
                getline(outline, ph_no);
                cout << "Phone: " << ph_no << endl;
            }
            outline.close();
        } else {
            cout << "Error opening file to read customer details.\n";
        }
    }

    void view_cars() {
        ifstream inFile("car_data.txt");
        string line;
        int lineNumber = 1;

        if (!inFile.is_open()) {
            cout << "\nNo cars available in the showroom." << endl;
            return;
        }

        cout << "\nAvailable Cars:\n";
        while (getline(inFile, line)) {
            cout << lineNumber << ". " << line << endl;
            lineNumber++;
        }
        inFile.close();
    }

    void buy_car() {
        view_cars();

        ifstream inFile("car_data.txt");
        ofstream tempFile("temp.txt");

        int carChoice;
        cout << "Enter the number of the car you want to buy: ";
        cin >> carChoice;

        inFile.clear();
        inFile.seekg(0, ios::beg);  // Move file pointer back to the start

        string line;
        int currentLine = 1;
        bool carBought = false;

        if (!inFile.is_open() || !tempFile.is_open()) {
            cout << "Error opening files for car buying process.\n";
            return;
        }

        cout << "\nEnter Your Details For Car Dealing!\n";
        get_data();
        write();  // Write customer details to file

        // Rewrite all cars except the chosen one to temp file
        while (getline(inFile, line)) {
            if (currentLine == carChoice) {
                cout << "\nYou have successfully bought the car: " << line << endl;
                carBought = true;
                Car::increaseCounter();  // Increase the sold car counter
            } else {
                tempFile << line << endl;  // Write remaining cars to temp file
            }
            currentLine++;
        }

        if (!carBought) {
            cout << "\nInvalid car choice. No car was bought." << endl;
        }

        inFile.close();
        tempFile.close();

        // Replace original file with updated list
        remove("car_data.txt");
        rename("temp.txt", "car_data.txt");

        if (carBought) {
            cout << "\nCongratulations, you own this car.\n";
        }
    }
};

int main() {
    // Variable to store user's choice
    int choice;
    // Create a customer object
    Customer customer;

    // Main menu loop
    do {
        // Display main menu
        cout << "\n\t\t\t\t_________________________________________________________\n\n\n";
        cout << "\t\t\t\t[[[[]]]^^^^^  WELCOME TO SUPERCAR SHOWROOM\n\n\n";
        cout << "\t\t\t\t_________________________________________________________\n";
        cout << "\n1. Manager\n2. Customer view\n3. Exit\n";
        cout << "\nEnter Your Choice: ";
        cin >> choice;
        cout << endl;

        // Switch case for user's choice
        switch (choice) {
            case 1: {
                // Manager's submenu variables
                int g;
                Manager m;

                // Manager's submenu loop
                do {
                    cout << "\n\n1. Add a new Car to the showroom\n"
                         << "2. Check sold cars\n"
                         << "3. View Customer details\n"
                         << "4. Delete records of Car data\n"
                         << "0. Go back\n";
                    cout << "\nEnter Your Choice: ";

                    // Validate user's input
                    while (!(cin >> g)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid number: ";
                    }

                    // Switch case for Manager's submenu choices
                    switch (g) {
                        case 1: {
                            m.edit("Cardata.txt");
                            break;
                        }
                        case 2: {
                            cout << "\n\n\t\t\t\tCar has been sold (" << Car::getCounter() << ")\n" << endl;
                            break;
                        }
                        case 3: {
                            customer.read();
                            break;
                        }
                        case 4: {
                            m.delete_record("Cardata.txt");
                            break;
                        }
                        case 0:
                            break;
                        default:
                            cout << "Invalid option. Please enter a valid option.\n";
                    }

                } while (g != 0);

                break;
            }
            case 2: {
                // Customer's submenu variables
                int customerChoice;

                // Customer's submenu loop
                do {
                    cout << "\n\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
                    cout << "\t\t\t\t\t@@  WELCOME TO CUSTOMER DASHBOARD  @@\n";
                    cout << "\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
                    cout << "1. View Cars\n2. Buy a Car\n0. Go back\n";
                    cout << "\nEnter Your Choice: ";
                    cin >> customerChoice;

                    // Switch case for Customer's submenu choices
                    switch (customerChoice) {
                        case 1: {
                            customer.view_cars();
                            break;
                        }
                        case 2: {
                            customer.buy_car();
                            break;
                        }
                        case 0:
                            break;
                        default:
                            cout << "\nInvalid option. Please enter a valid option.\n";
                    }

                } while (customerChoice != 0);

                break;
            }
            case 3:
                cout << "\nThank you for visiting Supercar Showroom! Exiting...\n";
                break;
            default:
                cout << "\nInvalid option. Please enter a valid option.\n";
        }

    } while (choice != 3);

    return 0;
}