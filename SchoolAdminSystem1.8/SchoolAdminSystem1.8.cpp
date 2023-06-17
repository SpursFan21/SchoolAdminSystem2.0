
// This library provides classes and functions for file input and output operations.
#include <fstream>
// This library provides the vector container class for dynamic arrays with automatic memory management.
#include <vector>
// This library provides the string class and associated functions for string operations.
#include <string>
// This library provides classes and functions for string stream operations.
#include <sstream>
// This library provides classes and functions for input and output operations.
#include <iostream>
// This library provides general-purpose functions and macros, including the exit() function.
#include <cstdlib>
//This library generates numbers for ID code logic
#include <random>
//This libary is used to generate random selection in the parent class
#include <ctime>

// Function to split a string into tokens
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}



// User base class
class User {
protected: // Protection level in between public and private
    std::string username;
    std::string password;
    std::string id;
    std::string type;

public:
    User() {}

    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getId() const { return id; } // Getter for ID


    virtual void displayMenu() const = 0; // Pure virtual function for displaying user menu

    void signOut() const {
        std::cout << "Signing out..." << std::endl;
        // Perform any necessary cleanup or tasks before signing out

        // Return to the initial login menu
        std::cout << "======================" << std::endl;
        std::cout << "Start Menu" << std::endl;
        std::cout << "1. Log in" << std::endl;
        std::cout << "2. New Admin" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "======================" << std::endl;
        std::cout << "Enter your choice: ";
    }

    // Constructor
    User(const std::string& username, const std::string& password)
        : username(username), password(password) {
        // Generate a unique ID code
        id = generateUniqueId();
        type = "User"; // Default type is "User"
    }

    static std::string generateUniqueId() {
        static std::mt19937 randomEngine(std::random_device{}()); // Random number engine
        static std::uniform_int_distribution<int> distribution(1000, 9999); // Range of IDs

        // Generate a random 4-digit ID
        size_t uniqueId = distribution(randomEngine);
        return "ID" + std::to_string(uniqueId);
    }
    // Getter for type
    std::string getType() const {
        return type;
    }

};

// Admin class derived from User
class Admin : public User {
public:
    
    Admin(const std::string& username, const std::string& password, const std::string& id = "")
        : User(username, password) {
        std::string generatedId = ""; // Initialize the variable to an empty string
        if (id.empty()) {
            // Generate a unique ID code
            generatedId = generateUniqueId();
        }
        else {
            generatedId = id;
        }
        this->id = generatedId;
    }


    void addClass(const std::string& className) {
        // Open the classes.txt file in append mode
        std::ofstream classesFile("classes.txt", std::ios::app);
        if (classesFile.is_open()) {
            // Write the class name followed by a newline character
            classesFile << className << "\n";
            classesFile.close();
            std::cout << "Class added successfully." << std::endl;
        }
        else {
            std::cout << "Failed to open classes.txt" << std::endl;
        }
    }


    void displayMenu() const override {
        std::cout << "Admin Menu" << std::endl;
        std::cout << "User Type: " << getType() << std::endl;
        std::cout << "User ID: " << getId() << std::endl;
        std::cout << "1. Register User" << std::endl;
        std::cout << "2. Update User" << std::endl;
        std::cout << "3. Delete User" << std::endl;
        std::cout << "4. View User Information" << std::endl;
        std::cout << "5. Add Student" << std::endl;
        std::cout << "6. Add Teacher" << std::endl;
        std::cout << "7. Add Class" << std::endl;
        std::cout << "8. Add Grade for Student" << std::endl;
        std::cout << "9. Sign Out" << std::endl;
    }
};

// Teacher class derived from User
class Teacher : public User {
public:
    Teacher(const std::string& username, const std::string& password) : User(username, password) {}
    Teacher(const std::string& id, const std::string& username, const std::string& password)
        : User(username, password) {
        this->id = id;
    }

    void displayMenu() const override {
        int choice;

        while (true) {
            std::cout << "Teacher User Menu" << std::endl;
            std::cout << "User Type: " << getType() << std::endl;
            std::cout << "User ID: " << getId() << std::endl;
            std::cout << "1. Add Grade for Student" << std::endl;
            std::cout << "2. View Grades" << std::endl;
            std::cout << "3. Sign Out" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            
            case 1: {
                std::string studentName;
                std::string grades;

                std::cout << "Enter student name: ";
                std::cin >> studentName;

                std::cout << "Enter grades (format: subject1:grade1 subject2:grade2 ...): ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, grades);

                std::ifstream file("grades.txt");
                std::ofstream tempFile("temp_grades.txt");
                bool found = false;

                if (file.is_open() && tempFile.is_open()) {
                    std::string line;
                    while (std::getline(file, line)) {
                        std::istringstream iss(line);
                        std::string name;
                        std::string subjectsAndGrades;

                        if (iss >> name >> subjectsAndGrades) {
                            if (name == studentName) {
                                found = true;
                                tempFile << studentName << " " << grades << std::endl;
                            }
                            else {
                                tempFile << name << " " << subjectsAndGrades << std::endl;
                            }
                        }
                    }

                    file.close();
                    tempFile.close();

                    if (found) {
                        // Replace the original grades file with the updated temporary file
                        if (std::remove("grades.txt") == 0 && std::rename("temp_grades.txt", "grades.txt") == 0) {
                            std::cout << "Grades updated successfully." << std::endl;
                        }
                        else {
                            std::cout << "Failed to update grades." << std::endl;
                        }
                    }
                    else {
                        // If the student name was not found, append the new grades to the end of the file
                        std::ofstream newGradeFile("grades.txt", std::ios::app);
                        if (newGradeFile.is_open()) {
                            newGradeFile << studentName << " " << grades << std::endl;
                            newGradeFile.close();
                            std::cout << "Grades added successfully." << std::endl;
                        }
                        else {
                            std::cout << "Failed to add grades." << std::endl;
                        }
                    }
                }
                else {
                    std::cout << "Unable to open file." << std::endl;
                }

                break;
            }


            case 2: {
                // Code for viewing grades
                std::ifstream file("grades.txt");
                if (file.is_open()) {
                    std::string line;
                    while (std::getline(file, line)) {
                        std::cout << line << std::endl;
                    }
                    file.close();
                }
                else {
                    std::cout << "Unable to open file." << std::endl;
                }
                break;
            }
            case 3:
                signOut(); // Call the signOut function to sign out and return to the login menu
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
    }
};


// Parent class derived from User
class Parent : public User {
public:
    Parent(const std::string& id, const std::string& username, const std::string& password)
        : User(username, password) {
        this->id = id;
    }

    Parent(const std::string& username, const std::string& password)
        : User(username, password) {}

    void displayMenu() const override {
        std::cout << "Parent Menu" << std::endl;
        std::cout << "User Type: Parent" << std::endl;
        std::cout << "User ID: " << getId() << std::endl;
        std::cout << "1. View Student Report Card" << std::endl;
        std::cout << "2. Sign Out" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            viewStudentReportCard();
            break;
        case 2:
            // Handle sign out logic
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
            break;
        }
    }

    void viewStudentReportCard() const {
        std::ifstream file("grades.txt");
        std::vector<std::string> grades;
        std::string line;

        // Read all lines from the file
        while (std::getline(file, line)) {
            grades.push_back(line);
        }

        file.close();

        if (grades.empty()) {
            std::cout << "No student grades found." << std::endl;
        }
        else {
            // Generate a random index to select a student
            std::srand(std::time(nullptr));
            int randomIndex = std::rand() % grades.size();

            // Print the randomly selected student's grades
            std::cout << "Random Student's Grades: " << grades[randomIndex] << std::endl;
        }
    }
};


// Student class
class Student {
private:
    std::string name;
    std::vector<std::string> classes;
    std::vector<int> grades;
    std::string grade;


public:
    Student(const std::string& name, const std::string& grade)
        : name(name), grade(grade) {}
    Student(const std::string& name) : name(name) {}


    void addClass(const std::string& className) {
        classes.push_back(className);
        grades.push_back(0); // Initialize grade as 0
    }

    void updateGrade(int index, int grade) {
        if (index >= 0 && index < grades.size()) {
            grades[index] = grade;
        }
    }

    std::string getName() const { return name; }
    const std::vector<std::string>& getClasses() const { return classes; }
    const std::vector<int>& getGrades() const { return grades; }
};
// StudentUser class derived from User
class StudentUser : public User {
public:
    StudentUser(const std::string& username, const std::string& password) : User(username, password) {}
    StudentUser(const std::string& id, const std::string& username, const std::string& password)
        : User(username, password) {
        this->id = id;
    }
    void displayMenu() const override {
        int choice;

        while (true) {
            std::cout << "Student User Menu" << std::endl;
            std::cout << "User Type: " << getType() << std::endl;
            std::cout << "User ID: " << getId() << std::endl;
            std::cout << "1. View Grades" << std::endl;
            std::cout << "2. Sign Out" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                // Code for viewing grades
                std::ifstream file("grades.txt");
                if (file.is_open()) {
                    std::string line;
                    std::string username = getUsername();
                    bool gradesFound = false;

                    while (std::getline(file, line)) {
                        // Split the line into username and grades
                        std::string::size_type pos = line.find(":");
                        if (pos != std::string::npos) {
                            std::string studentUsername = line.substr(0, pos);
                            std::string grades = line.substr(pos + 1);

                            if (studentUsername == username) {
                                std::cout << "Grades for " << username << ": " << grades << std::endl;
                                gradesFound = true;
                                break;
                            }
                        }
                    }

                    file.close();

                    if (!gradesFound) {
                        std::cout << "No grades found for " << username << std::endl;
                    }
                }
                else {
                    std::cout << "Unable to open file." << std::endl;
                }
                break;
            }
            case 2:
                signOut(); // Call the signOut function to sign out and return to the login menu
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
    }


};
class School {
private:
    std::vector<User*> users;
    std::vector<Student> students;

public:
    ~School() {
        // Clean up dynamically allocated user objects
        for (User* user : users) {
            delete user;
        }
    }

    std::vector<Student>& getStudents() {
        return students;
    }

    void registerUser(User* user) {
        users.push_back(user);
    }

    User* findUser(const std::string& username) const {
        for (User* user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr;
    }

    void addStudent(const std::string& name) {
        students.push_back(Student(name));
    }

    Student* findStudent(const std::string& name) {
        for (Student& student : students) {
            if (student.getName() == name) {
                return &student;
            }
        }
        return nullptr;
    }
};

// Function to view user information from the file
void viewUserInformation(const std::string& filename, const std::string& username) {
    std::ifstream inputFile("users.txt");
    if (inputFile.is_open()) {
        std::string line;
        bool userFound = false;

        while (std::getline(inputFile, line)) {
            std::vector<std::string> userData = splitString(line, ',');
            if (userData[1] == username) {
                // Display the user information
                std::cout << "Username: " << userData[1] << std::endl;
                std::cout << "Password: " << userData[2] << std::endl;
                userFound = true;
                break;
            }
        }

        inputFile.close();

        if (!userFound) {
            std::cout << "User not found." << std::endl;
        }
    }
    else {
        std::cerr << "Error opening the file" << std::endl;
    }
}

// Function to update user information in the file
void updateUserInformation(const std::string& username, const std::string& newUsername, const std::string& newPassword) {
    std::vector<std::string> userRecords;

    // Read the existing user information from the file
    std::ifstream inputFile("users.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            userRecords.push_back(line);
        }
        inputFile.close();
    }
    else {
        std::cerr << "Error opening the file" << std::endl;
        return;
    }

    // Search for the username in the user records
    bool userFound = false;
    for (auto& record : userRecords) {
        std::vector<std::string> userData = splitString(record, ',');
        if (userData[1] == username) {
            // Update the user information
            userData[1] = newUsername;
            userData[2] = newPassword;

            // Modify the record in the user records
            record = userData[0] + "," + userData[1] + "," + userData[2];
  
            userFound = true;
            break;
        }
    }

    // Write the modified user information back to the file
    std::ofstream outputFile("users.txt");
    if (outputFile.is_open()) {
        for (const auto& record : userRecords) {
            outputFile << record << std::endl;
        }
        outputFile.close();
        std::cout << "User information updated successfully." << std::endl;
    }
    else {
        std::cerr << "Error opening the file" << std::endl;
    }

    if (!userFound) {
        std::cout << "User not found." << std::endl;
    }
}

// Function to delete a user from the users.txt file
void deleteUserFromFile(const std::string& username) {
    std::ifstream inFile("users.txt");
    if (!inFile) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    std::ofstream outFile("temp.txt");
    if (!outFile) {
        std::cout << "Error creating temporary file." << std::endl;
        inFile.close();
        return;
    }

    std::string line;
    bool userFound = false;

    while (std::getline(inFile, line)) {
        if (line.substr(0, username.length()) != username) {
            // Write the line to the temporary file if the username doesn't match
            outFile << line << std::endl;
        }
        else {
            // Set the flag to indicate that the user was found
            userFound = true;
        }
    }

    inFile.close();
    outFile.close();

    // Delete the original file and rename the temporary file to users.txt
    if (userFound) {
        if (std::remove("users.txt") != 0) {
            std::cout << "Error deleting file." << std::endl;
            return;
        }

        if (std::rename("temp.txt", "users.txt") != 0) {
            std::cout << "Error renaming file." << std::endl;
            return;
        }

        std::cout << "User deleted successfully." << std::endl;
    }
    else {
        std::cout << "User not found." << std::endl;
    }
}


bool isValidCredentials(const std::string& username, const std::string& password) {
    std::ifstream userFile("users.txt");
    if (userFile.is_open()) {
        std::string line;
        while (std::getline(userFile, line)) {
            std::vector<std::string> userData = splitString(line, ',');
            if (userData.size() >= 3 && userData[1] == username && userData[2] == password) {
                userFile.close();
                return true;
            }
            else {
                std::cout << "Debug - Username: " << userData[1] << ", Password: " << userData[2] << std::endl;
            }
        }
        userFile.close();
    }
    return false;
}




void exitProgram()
{
    std::cout << "Exiting..." << std::endl;
    exit(0);
}



int main() {
    School school;
    std::string line;
    // Creating Local user objects
    Admin admin("adminID", "admin", "admin123");
    Teacher teacher("teacher", "teacher123");
    Parent parent("parent", "parent123");
    StudentUser student("studentID", "student", "student123");

std::vector<User*> users;
users.push_back(&admin);
users.push_back(&teacher);
users.push_back(&parent);
users.push_back(&student);

school.registerUser(&admin);
school.registerUser(&teacher);
school.registerUser(&parent);
school.registerUser(&student);


std::ifstream userFile("users.txt");
if (userFile.is_open()) {
    std::string line;
    while (std::getline(userFile, line)) {
        std::vector<std::string> tokens = splitString(line, ',');

        // Check if the line has at least 4 tokens (userType, username, password, userID)
        if (tokens.size() >= 4) {
            std::string userType = tokens[0];
            std::string username = tokens[1];
            std::string password = tokens[2];
            std::string userID = tokens[3];

            if (userType == "admin") {
                Admin* admin = new Admin(userID, username, password);
                school.registerUser(admin);
            }
            else if (userType == "teacher") {
                Teacher* teacher = new Teacher(userID, username, password);
                school.registerUser(teacher);
            }
            else if (userType == "parent") {
                Parent* parent = new Parent(userID, username, password);
                school.registerUser(parent);
            }
            else if (userType == "student") {
                StudentUser* student = new StudentUser(userID, username, password);
                school.registerUser(student);
            }
        }
    }
    userFile.close();
}
else {
    std::cout << "Failed to open users.txt" << std::endl;
    return 1;
}


    int userType;
    User* currentUser = nullptr;
    bool loggedIn = false;
    bool exitProgram = false;

    size_t choice;

      while (!loggedIn && !exitProgram){

        std::cout << "===== Start Menu =====" << std::endl;
        std::cout << "1. Log in" << std::endl;
        std::cout << "2. New Admin" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "======================" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1: {
            std::string username, password;
            std::cout << "Enter username: ";
            try {
                std::cin >> username;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                break;
            }

            std::cout << "Enter password: ";
            try {
                std::cin >> password;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                break;
            }

            User* user = school.findUser(username);
            if (user && user->getPassword().compare(password) == 0) {
                currentUser = user;
                loggedIn = true;
            }
            else {
                std::cout << "Invalid username or password" << std::endl;
            }
            break;
        }
        case 2: {
            // Create new admin logic
            std::string newAdminUsername, newAdminPassword;
            std::cout << "Enter new admin username: ";
            std::cin >> newAdminUsername;
            std::cout << "Enter new admin password: ";
            std::cin >> newAdminPassword;

            Admin* newAdmin = new Admin(newAdminUsername, newAdminPassword);
            school.registerUser(newAdmin);

            std::cout << "New admin created successfully!" << std::endl;
            break;
        }

        case 3: {
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    while (loggedIn) {
        currentUser->displayMenu();
        bool validChoice = false;
        std::string continueChoice; // Declare continueChoice variable

        while (!validChoice) {
            // ...

            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                // Register User
                std::string userType, username, password;
                std::cout << "Enter the user type (admin/teacher/parent): ";
                std::cin >> userType;
                std::cout << "Enter the username: ";
                std::cin >> username;
                std::cout << "Enter the password: ";
                std::cin >> password;

                std::cout << "User registered successfully." << std::endl;

                // Save user information to file
                std::ofstream outputFile("users.txt", std::ios::app);
                if (outputFile.is_open()) {
                    outputFile << userType << "," << username << "," << password << std::endl;
                    outputFile.close();
                    std::cout << "User information saved to users.txt" << std::endl;
                }
                else {
                    std::cerr << "Error opening the file" << std::endl;
                }

                if (continueChoice == "no") {
                    exitProgram = true;
                }
                else {
                    validChoice = true;
                }
                break;
            }
            case 2: {
                // Update User
                std::string username;
                std::cout << "Enter the username of the user to update: ";
                std::cin >> username;

                std::string newUsername;
                std::cout << "Enter the new username: ";
                std::cin >> newUsername;

                std::string newPassword;
                std::cout << "Enter the new password: ";
                std::cin >> newPassword;

                updateUserInformation(username, newUsername, newPassword);
                std::cout << "User updated successfully." << std::endl;

                validChoice = true;
                break;
            }

            case 3: {
                // Delete User
                std::string username;
                std::cout << "Enter the username of the user to delete: ";
                std::cin >> username;

                User* userToDelete = school.findUser(username);
                if (userToDelete) {
                    // Call the function to delete the user from the file
                    deleteUserFromFile(username);

                    std::cout << "User deleted successfully." << std::endl;
                }
                else {
                    std::cout << "User not found." << std::endl;
                }
                validChoice = true;
                break;
            }

            case 4: {
                // View User Information
                std::string username;
                std::cout << "Enter the username of the user to view information: ";
                std::cin >> username;

                viewUserInformation("users.txt", username);
                validChoice = true;
                break;
            }

            case 5: {
                // Add Student
                std::string studentName;
                std::cout << "Enter the name of the student to add: ";
                std::cin >> studentName;

                school.addStudent(studentName);
                std::cout << "Student added successfully." << std::endl;
                validChoice = true;
                break;
            }
            case 6: {
                // Add Teacher
                std::string teacherName;
                std::cout << "Enter the name of the teacher to add: ";
                std::cin >> teacherName;

                // Perform the necessary operations to add a teacher
                // You can add your implementation here

                std::cout << "Teacher added successfully." << std::endl;
                validChoice = true;
                break;
            }
            case 7: {
                // Add Class
                std::string className;
                std::cout << "Enter the name of the class to add: ";
                std::cin >> className;

                // Assuming you have an Admin object for the current admin
                if (currentUser->getUsername() == "admin") {
                    Admin* admin = dynamic_cast<Admin*>(currentUser);
                    admin->addClass(className);

                    // Update the classes.txt file with the new class
                    std::ofstream classesFile("classes.txt", std::ios::app);
                    if (classesFile.is_open()) {
                        classesFile << className << "\n";
                        classesFile.close();
                        std::cout << "Class added successfully." << std::endl;
                    }
                    else {
                        std::cout << "Failed to open classes.txt" << std::endl;
                    }
                }
                else {
                    std::cout << "Invalid user type for adding a class." << std::endl;
                }

                validChoice = true;
                break;
            }

            case 8: {
                // Add Grade for Student
                std::string studentName;
                std::cout << "Enter the name of the student: ";
                std::cin >> studentName;

                // Find the student in the school
                Student* student = school.findStudent(studentName);
                if (student) {
                    // Display the student's classes
                    const std::vector<std::string>& classes = student->getClasses();
                    std::cout << "Classes: ";
                    for (const std::string& className : classes) {
                        std::cout << className << ", ";
                    }
                    std::cout << std::endl;

                    // Select a class to add the grade
                    std::string className;
                    std::cout << "Enter the class name to add the grade: ";
                    std::cin >> className;

                    // Find the index of the class in the student's classes vector
                    int index = -1;
                    for (size_t i = 0; i < classes.size(); i++) {
                        if (classes[i] == className) {
                            index = i;
                            break;
                        }
                    }

                    if (index != -1) {
                        // Prompt for the grade
                        int grade;
                        std::cout << "Enter the grade for " << className << ": ";
                        std::cin >> grade;

                        // Update the grade for the selected class
                        student->updateGrade(index, grade);

                        // Save the updated student information to the file
                        std::ofstream studentFile("students.txt");
                        if (studentFile.is_open()) {
                            for (const Student& s : school.getStudents()) {
                                studentFile << s.getName() << ",";
                                const std::vector<std::string>& studentClasses = s.getClasses();
                                const std::vector<int>& studentGrades = s.getGrades();
                                for (size_t i = 0; i < studentClasses.size(); i++) {
                                    studentFile << studentClasses[i] << ":" << studentGrades[i];
                                    if (i < studentClasses.size() - 1) {
                                        studentFile << ",";
                                    }
                                }
                                studentFile << "\n";
                            }
                            studentFile.close();
                            std::cout << "Grade added successfully." << std::endl;
                        }
                        else {
                            std::cout << "Failed to open students.txt" << std::endl;
                        }
                    }
                    else {
                        std::cout << "Class not found for the student." << std::endl;
                    }
                }
                else {
                    std::cout << "Student not found." << std::endl;
                }

                validChoice = true;
                break;
            }

            case 9: {
                currentUser->signOut();

            }
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }

        }
        std::cout << "Do you want to continue? (yes/no): ";
        std::cin >> continueChoice;

        if (continueChoice == "no") {
            exitProgram = true;
            exit(0);
        }

    }

};