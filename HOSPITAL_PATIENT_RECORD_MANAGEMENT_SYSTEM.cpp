#include <iostream>
#include <string>
using namespace std;

// Patient ADT Structure
struct Patient {
    int PatientID;
    string PatientName;
    string AdmissionDate;
    string TreatmentDetails;
    
    Patient(int id = 0, string name = "", string date = "", string treatment = "") {
        PatientID = id;
        PatientName = name;
        AdmissionDate = date;
        TreatmentDetails = treatment;
    }
};

// Linked List Node for Patients
struct PatientNode {
    Patient data;
    PatientNode* next;
    
    PatientNode(Patient p) {
        data = p;
        next = nullptr;
    }
};

// Stack for Undo Operations
class UndoStack {
private:
    struct StackNode {
        Patient data;
        StackNode* next;
        
        StackNode(Patient p) {
            data = p;
            next = nullptr;
        }
    };
    
    StackNode* top;
    
public:
    UndoStack() {
        top = nullptr;
    }
    
    void push(Patient p) {
        StackNode* newNode = new StackNode(p);
        newNode->next = top;
        top = newNode;
    }
    
    Patient pop() {
        if (top == nullptr) {
            return Patient(-1, "", "", "");
        }
        
        StackNode* temp = top;
        Patient data = top->data;
        top = top->next;
        delete temp;
        return data;
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
};

// Circular Queue for Emergency Patients
class EmergencyQueue {
private:
    struct QueueNode {
        Patient data;
        QueueNode* next;
        QueueNode* prev;
        
        QueueNode(Patient p) {
            data = p;
            next = nullptr;
            prev = nullptr;
        }
    };
    
    QueueNode* front;
    QueueNode* rear;
    int size;
    int capacity;
    
public:
    EmergencyQueue(int cap = 10) {
        capacity = cap;
        front = rear = nullptr;
        size = 0;
    }
    
    bool isFull() {
        return size == capacity;
    }
    
    bool isEmpty() {
        return size == 0;
    }
    
    void enqueue(Patient p) {
        if (isFull()) {
            cout << "Emergency queue is full!" << endl;
            return;
        }
        
        QueueNode* newNode = new QueueNode(p);
        
        if (isEmpty()) {
            front = rear = newNode;
            front->next = front;
            front->prev = front;
        } else {
            newNode->next = front;
            newNode->prev = rear;
            rear->next = newNode;
            front->prev = newNode;
            rear = newNode;
        }
        size++;
        cout << "Patient " << p.PatientName << " added to emergency queue" << endl;
    }
    
    Patient dequeue() {
        if (isEmpty()) {
            return Patient(-1, "", "", "");
        }
        
        Patient data = front->data;
        QueueNode* temp = front;
        
        if (front == rear) {
            front = rear = nullptr;
        } else {
            front = front->next;
            front->prev = rear;
            rear->next = front;
        }
        
        delete temp;
        size--;
        return data;
    }
    
    void displayQueue() {
        if (isEmpty()) {
            cout << "Emergency queue is empty!" << endl;
            return;
        }
        
        cout << "Emergency Queue: ";
        QueueNode* current = front;
        do {
            cout << current->data.PatientName << "(" << current->data.PatientID << ")";
            if (current->next != front) {
                cout << " -> ";
            }
            current = current->next;
        } while (current != front);
        cout << endl;
    }
};

// Polynomial for Billing Calculations
struct PolyTerm {
    double coefficient;
    int exponent;
    PolyTerm* next;
    
    PolyTerm(double coeff, int exp) {
        coefficient = coeff;
        exponent = exp;
        next = nullptr;
    }
};

class HospitalManagementSystem {
private:
    PatientNode* patientHead;
    UndoStack undoStack;
    EmergencyQueue emergencyQueue;
    PolyTerm* billingPoly;
    int patientCounter;
    
    // Helper Methods
    PatientNode* findPatient(int patientID) {
        PatientNode* current = patientHead;
        while (current != nullptr) {
            if (current->data.PatientID == patientID) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // Custom power function
    double power(double base, int exp) {
        double result = 1.0;
        for (int i = 0; i < exp; i++) {
            result *= base;
        }
        return result;
    }
    
    // Helper function to check if character is operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }
    
    // Helper function to perform operations
    double performOperation(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return a / b;
            default: return 0;
        }
    }
    
public:
    HospitalManagementSystem() {
        patientHead = nullptr;
        billingPoly = nullptr;
        patientCounter = 5000;
        
        cout << "=== Hospital Patient Record Management System ===" << endl;
        cout << "System initialized successfully!" << endl;
    }
    
    ~HospitalManagementSystem() {
        // Cleanup patient list
        PatientNode* temp = patientHead;
        while (temp != nullptr) {
            PatientNode* next = temp->next;
            delete temp;
            temp = next;
        }
        
        // Cleanup polynomial
        PolyTerm* tempPoly = billingPoly;
        while (tempPoly != nullptr) {
            PolyTerm* next = tempPoly->next;
            delete tempPoly;
            tempPoly = next;
        }
    }
    
    // 1. Linked List Operations
    void addPatientRecord(string name, string date, string treatment) {
        Patient newPatient(patientCounter++, name, date, treatment);
        PatientNode* newNode = new PatientNode(newPatient);
        
        if (patientHead == nullptr) {
            patientHead = newNode;
        } else {
            PatientNode* temp = patientHead;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        
        // Save to undo stack
        undoStack.push(newPatient);
        
        cout << "Patient " << name << " admitted successfully! ID: " << newPatient.PatientID << endl;
    }
    
    bool deletePatient(int patientID) {
        PatientNode* current = patientHead;
        PatientNode* prev = nullptr;
        
        while (current != nullptr) {
            if (current->data.PatientID == patientID) {
                if (prev == nullptr) {
                    patientHead = current->next;
                } else {
                    prev->next = current->next;
                }
                
                cout << "Patient " << current->data.PatientName << " discharged!" << endl;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        
        cout << "Patient ID " << patientID << " not found!" << endl;
        return false;
    }
    
    Patient* retrievePatient(int patientID) {
        PatientNode* node = findPatient(patientID);
        if (node != nullptr) {
            return &(node->data);
        }
        return nullptr;
    }
    
    void displayAllPatients() {
        if (patientHead == nullptr) {
            cout << "No patients in the system!" << endl;
            return;
        }
        
        cout << "\n=== All Patient Records ===" << endl;
        cout << "---------------------------" << endl;
        PatientNode* current = patientHead;
        while (current != nullptr) {
            cout << "ID: " << current->data.PatientID 
                 << " | Name: " << current->data.PatientName
                 << " | Admission: " << current->data.AdmissionDate
                 << " | Treatment: " << current->data.TreatmentDetails << endl;
            current = current->next;
        }
        cout << "---------------------------" << endl;
    }
    
    // 2. Stack-based Undo Operations
    void undoAdmission() {
        if (undoStack.isEmpty()) {
            cout << "No operations to undo!" << endl;
            return;
        }
        
        Patient lastPatient = undoStack.pop();
        deletePatient(lastPatient.PatientID);
        cout << "Undid admission of patient: " << lastPatient.PatientName << endl;
    }
    
    // 3. Emergency Queue Management
    void addToEmergencyQueue(int patientID) {
        Patient* patient = retrievePatient(patientID);
        if (patient != nullptr) {
            emergencyQueue.enqueue(*patient);
        } else {
            cout << "Patient not found!" << endl;
        }
    }
    
    void processEmergencyPatient() {
        Patient emergencyPatient = emergencyQueue.dequeue();
        if (emergencyPatient.PatientID != -1) {
            cout << "Processing emergency patient: " << emergencyPatient.PatientName << endl;
        } else {
            cout << "No patients in emergency queue!" << endl;
        }
    }
    
    void displayEmergencyQueue() {
        emergencyQueue.displayQueue();
    }
    
    // 4. Polynomial Operations for Billing
    void addBillingTerm(double coeff, int exp) {
        PolyTerm* newTerm = new PolyTerm(coeff, exp);
        
        if (billingPoly == nullptr) {
            billingPoly = newTerm;
        } else {
            PolyTerm* current = billingPoly;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newTerm;
        }
        
        cout << "Added billing term: " << coeff << "x^" << exp << endl;
    }
    
    double calculateBilling(int patientID, double x) {
        Patient* patient = retrievePatient(patientID);
        if (patient == nullptr) {
            cout << "Patient not found!" << endl;
            return 0;
        }
        
        double total = 0;
        PolyTerm* current = billingPoly;
        
        cout << "Billing calculation for " << patient->PatientName << ":" << endl;
        while (current != nullptr) {
            double termValue = current->coefficient * power(x, current->exponent);
            total += termValue;
            cout << current->coefficient << " * " << x << "^" << current->exponent 
                 << " = " << termValue << endl;
            current = current->next;
        }
        
        cout << "Total billing amount: " << total << endl;
        return total;
    }
    
    void displayBillingPolynomial() {
        if (billingPoly == nullptr) {
            cout << "No billing polynomial defined!" << endl;
            return;
        }
        
        cout << "Billing Polynomial: ";
        PolyTerm* current = billingPoly;
        while (current != nullptr) {
            cout << current->coefficient << "x^" << current->exponent;
            if (current->next != nullptr) {
                cout << " + ";
            }
            current = current->next;
        }
        cout << endl;
    }
    
    // 5. Postfix Expression Evaluation for Inventory
    double evaluateInventory(string expression) {
        // Simple stack implementation for postfix evaluation
        struct NumStack {
            double data[100];
            int top;
            
            NumStack() { top = -1; }
            void push(double val) { data[++top] = val; }
            double pop() { return data[top--]; }
            bool isEmpty() { return top == -1; }
        };
        
        NumStack stack;
        string number = "";
        
        for (int i = 0; i < expression.length(); i++) {
            char c = expression[i];
            
            if (c == ' ') {
                if (!number.empty()) {
                    stack.push(stod(number));
                    number = "";
                }
                continue;
            }
            
            if (isdigit(c) || c == '.') {
                number += c;
            } else if (isOperator(c)) {
                if (stack.top < 1) {
                    cout << "Invalid expression!" << endl;
                    return 0;
                }
                
                double b = stack.pop();
                double a = stack.pop();
                double result = performOperation(a, b, c);
                stack.push(result);
                
                cout << a << " " << c << " " << b << " = " << result << endl;
            }
        }
        
        // Handle last number if exists
        if (!number.empty()) {
            stack.push(stod(number));
        }
        
        if (stack.top != 0) {
            cout << "Invalid expression!" << endl;
            return 0;
        }
        
        double finalResult = stack.pop();
        cout << "Final inventory calculation result: " << finalResult << endl;
        return finalResult;
    }
    
    // Main Menu
    void mainMenu() {
        int choice;
        do {
            cout << "\n=== Hospital Management System Menu ===" << endl;
            cout << "1. Admit New Patient" << endl;
            cout << "2. Discharge Patient" << endl;
            cout << "3. Display All Patients" << endl;
            cout << "4. Undo Last Admission" << endl;
            cout << "5. Add Patient to Emergency Queue" << endl;
            cout << "6. Process Emergency Patient" << endl;
            cout << "7. Display Emergency Queue" << endl;
            cout << "8. Add Billing Term" << endl;
            cout << "9. Display Billing Polynomial" << endl;
            cout << "10. Calculate Patient Billing" << endl;
            cout << "11. Evaluate Inventory Expression" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            
            string name, date, treatment, expression;
            int patientID, exp;
            double coeff, x;
            
            switch (choice) {
                case 1:
                    cout << "Enter Patient Name: ";
                    getline(cin, name);
                    cout << "Enter Admission Date (DD/MM/YYYY): ";
                    getline(cin, date);
                    cout << "Enter Treatment Details: ";
                    getline(cin, treatment);
                    addPatientRecord(name, date, treatment);
                    break;
                    
                case 2:
                    cout << "Enter Patient ID to discharge: ";
                    cin >> patientID;
                    deletePatient(patientID);
                    break;
                    
                case 3:
                    displayAllPatients();
                    break;
                    
                case 4:
                    undoAdmission();
                    break;
                    
                case 5:
                    cout << "Enter Patient ID for emergency: ";
                    cin >> patientID;
                    addToEmergencyQueue(patientID);
                    break;
                    
                case 6:
                    processEmergencyPatient();
                    break;
                    
                case 7:
                    displayEmergencyQueue();
                    break;
                    
                case 8:
                    cout << "Enter coefficient: ";
                    cin >> coeff;
                    cout << "Enter exponent: ";
                    cin >> exp;
                    addBillingTerm(coeff, exp);
                    break;
                    
                case 9:
                    displayBillingPolynomial();
                    break;
                    
                case 10:
                    cout << "Enter Patient ID: ";
                    cin >> patientID;
                    cout << "Enter value of x for calculation: ";
                    cin >> x;
                    calculateBilling(patientID, x);
                    break;
                    
                case 11:
                    cout << "Enter postfix expression (space separated): ";
                    getline(cin, expression);
                    evaluateInventory(expression);
                    break;
                    
                case 0:
                    cout << "Exiting system..." << endl;
                    break;
                    
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }
};

int main() {
    cout << "Hospital Patient Record Management System" << endl;
    cout << "=========================================" << endl;
    
    HospitalManagementSystem hospital;
    
    // Added sample data for demonstration
    hospital.addPatientRecord("krish", "15/09/2025", "Cardiac checkup");
    hospital.addPatientRecord("tarun yadav", "16/09/2025", "Orthopedic surgery");
    hospital.addPatientRecord("aryan goyal", "17/09/2025", "General consultation");
    
    hospital.addBillingTerm(100, 2);
    hospital.addBillingTerm(50, 1);
    hospital.addBillingTerm(25, 0);
    
    hospital.mainMenu();
    
    return 0;
}