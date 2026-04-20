
#include<iostream>
using namespace std;
#include <string>
#include <sstream>
#include <iomanip> 
#include <chrono>
#include <limits>

string getCurrentDateTime() {
    time_t now =time(0);               
    tm* localTime = localtime(&now);    

    char buffer[80];
    strftime(buffer, sizeof(buffer),"%Y-%m-%d %H:%M:%S", localTime); //PROPER FORMATTING CHANGE

    return string(buffer);
}

double getValidAmount(const string& prompt) {
    double amount;
    while (true) {
        cout<<prompt;
        if (cin>>amount) { //valid double
            return amount;
        } else {
            cout <<"Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); //discard bad input
        }
    }
}

struct transactionNode{
    string reason;
    double amount;
    transactionNode *next;
    string datetime;

    transactionNode(){                                                                       
        reason=" ";
        amount=0;
        datetime=getCurrentDateTime();
        next=nullptr;
    }

    transactionNode(string reason, double amount,string datetime){
        this->reason=reason;
        this->amount=amount;
        this->datetime=datetime;
        next=nullptr;
    }

     transactionNode(string reason, double amount){
        this->reason=reason;
        this->amount=amount;
        this->datetime=getCurrentDateTime();
        next=nullptr;
    }
};

//linked list transaction queue implementation:
class transactionQueue{
    public:
    transactionNode* front;
    transactionNode* rear;

    transactionQueue(){
        front=rear=nullptr;
    }

    void enqueue(string reason, double amount,string datetime){
        transactionNode* newNode= new transactionNode(reason, amount,datetime);

        if(front==nullptr){
            front=newNode;
            rear=newNode;
            return;
        }

        rear->next=newNode;
        rear=newNode;
        return;
        
    }

    void enqueue(string reason, double amount){
        transactionNode* newNode= new transactionNode(reason, amount);

        if(front==nullptr){
            front=newNode;
            rear=newNode;
            return;
        }

        rear->next=newNode;
        rear=newNode;
        return;
        
    }

    //removing first transaction node 
    void dequeue(){
        if(front==nullptr){             //changed by muneeb from rear to front
            //empty
            return;
        }

        transactionNode* temp= front;
        front=front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
        return;
    }
    
    void peekRear(){
        if(rear==nullptr){
            return;
        }

        cout<<"Transaction Amount: "<<rear->amount<<" Transaction Narration: "<<rear->reason;
        //can also return the node
    }

    void peekFront(){
        if(rear==nullptr){
            return;
        }

        cout<<"Transaction Amount: "<<front->amount<<" Transaction Narration: "<<front->reason;
        //can also return the node
    }

    void displayQueue() {
        if (front==nullptr) { 
            cout<<"No transactions to display."<<endl;
            return; 
        }

        transactionNode* temp = front;

        while (temp != nullptr) {
            cout << "Transaction Reason: " << temp->reason
                << " Transaction Amount: " << temp->amount << " Transaction date and time : "<<temp->datetime<<endl;
        temp = temp->next;

    }
}

    void clear() { // clear all transactions
        while (rear!=nullptr) dequeue();
    }

    void removeLastTransac(){ 
        if(rear==nullptr){
            cout<<"No transaction exists."<<endl;
            return;
        }

        if(front==rear){
            delete rear; //memory leak
            front=rear=nullptr;
            return;
        }

        transactionNode *temp= front;
        while(temp->next!=rear){
            temp=temp->next;
        }

        delete rear;
        rear=temp;
        rear->next = nullptr;
        return ;
    }

    transactionNode* getRear(){
        if(rear==nullptr){
            cout<<"No transaction exists."<<endl;
            return nullptr;
        }

        return rear;
    }
};

class Stack {
private:
    transactionNode* top;  

public:
    
    Stack() {
        top=NULL;
    }

    void push(string reason, double amount,string datetime) {
        transactionNode* newNode = new transactionNode(reason, amount,datetime);
        newNode->next = top;
        top=newNode;
    }

    bool isEmpty() {
        return (top == NULL);
    }

    void pop() {
        if (isEmpty()) {
            cout << "stack empty"; //remove later
            return ;
        }
        transactionNode* temp = top;
        top = top->next;
        delete temp;
    }

    void clear() {
        transactionNode* temp;
        while (top != NULL) {
            temp = top;
            top = top->next;
            delete temp;
        }
    }
    
    string peekReason() {
        if (top == NULL) {
            cout << "stack empty";
            return "";
        }
        return top->reason;
    }   

    double peekAmount() {
        if (top == NULL) {
            cout << "stack empty";
            return 0;
        }
        return top->amount;
    }
    
    string peekdatetime() {
        if (top == NULL) {
            cout << "stack empty";
            return " ";
        }
        return top->datetime;
    }

    

    
};

class customernode
{
    private:
    string accid;
    string name;
    double balance;
    customernode * next;
    transactionQueue trQueue;
    Stack redoStack;

    static int IDcounter;

    public:
    customernode(string n,double bal)
    {   
        IDcounter++;
        stringstream ss;
        ss<<"PK01-"<< 2025 <<"-"<<setw(4)<<setfill('0')<<IDcounter;
        

        accid = ss.str();
        name = n;
        balance = bal;
        next = nullptr;
    }

    void display(){
        cout<<"Customer ID: "<<accid<<endl;
        cout<<"Customer Name: "<<name<<endl;
        cout<<"Customer Balance: "<<balance<<endl;
        cout<<"Transaction History: "<<endl;
        trQueue.displayQueue();
    }

    ~customernode(){
        trQueue.clear();
        redoStack.clear();
    }   


    friend class bank;
  
};

    int customernode::IDcounter = 0;

class bank
{
    customernode * head;
    public:
    bank()
    {
        head = NULL;
    }

    bool openaccount(string name,double balance)
    {
        bool found = false;
        customernode * c = new customernode(name,balance);
        if(balance < 1000)
        {
            cout<<"Insufficient balance, account can not be created."<<endl;
            customernode::IDcounter--; 
            return found;
        }
       
        if(head==nullptr){
            head=c;
            
        }

        else{
            customernode* temp=head;

            while(temp->next!=nullptr){
                temp=temp->next;
            }
            temp->next= c;
        }
        
        return true;

    }

    customernode * searchCustomer(string id)
    {
        customernode * temp = head;
        while(temp !=NULL&&temp->accid != id )
        {
            temp = temp->next;
        }

        if(temp==nullptr){
            cout<<"Customer with id: "<<id<<" Could not be found."<<endl;
            return nullptr;
        }

        return temp;
    }

    void displayallacc()
    {
        customernode * temp = head;
        if(head==nullptr){
            cout<<"No accounts to display."<<endl;
        }

        while(temp!= NULL)
        {
            cout << "ID: " << temp->accid << endl;
            cout << "Name: " << temp->name << endl;
            cout << "Balance: " << temp->balance << endl << endl;  
            temp = temp->next;
        }
    }

   bool depositMoney(string id, double amt, string reason)
   {

    if(amt<=0){
        cout<<"Can not put negative value!"<<endl;
        return false;
    }
    
    if (head == NULL) {
        cout << "No accounts found" << endl;
        return false;
    }

    customernode* temp = head;


    while (temp != NULL && temp->accid != id)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Account not found" << endl;
        return false;
    }

    temp->balance  = temp->balance + amt;

    cout<<"Balance of "<<amt<<" added successfully for customer ID: "<<temp->accid<<endl;

    temp->trQueue.enqueue(reason,amt);
    temp->redoStack.clear(); 
    return true;

}

bool withdrawamt(string id,double amt, string reason)
{

    if(amt<=0){
        cout<<"Can not put negative value!"<<endl;
        return false;
    }

    if (head == NULL) {
        cout << "No accounts found" << endl;
        return false;
    }

    customernode* temp = head;


    while (temp != NULL && temp->accid != id)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Account not found" << endl;
        return false;
    }

    if(temp->balance < amt)
    {
        cout<<"Insufficient balance"<<endl;
        return false;
    }
    temp->balance  = temp->balance - amt;

    cout<<"Balance of "<<amt<<" withdrawed successfully for customer ID: "<<temp->accid<<endl;

    temp->trQueue.enqueue(reason,-amt);
    temp->redoStack.clear(); 
    return true;
}

void undo(string id){

    if (head == NULL) {
        cout << "No accounts found" << endl;
        return;
    }

    customernode* temp = head;

    while (temp != NULL && temp->accid != id)
    {
        temp = temp->next;
    }

    if(temp==NULL){
        cout<<"Account not found."<<endl;
        return;
    }


    if(temp->trQueue.rear==nullptr){
        cout<<"No transactions to undo."<<endl;
        return;
    }

    if(temp->trQueue.rear->reason.find("Transfer")!=string::npos){ 
        cout<<"Cannot undo a transfer transaction!"<<endl;
        return;
    }
   
    if(temp->trQueue.front==temp->trQueue.rear){
        temp->redoStack.push(temp->trQueue.front->reason, temp->trQueue.front->amount,temp->trQueue.front->datetime);
        temp->balance=temp->balance - (temp->trQueue.front->amount); 
        temp->trQueue.clear();
        cout<<"Undo succesfull."<<endl;
        
        return;
    }

    temp->redoStack.push(temp->trQueue.rear->reason, temp->trQueue.rear->amount,temp->trQueue.rear->datetime);
    temp->balance=temp->balance - (temp->trQueue.rear->amount);
    temp->trQueue.removeLastTransac(); //undo
    cout<<"Undo succesfull."<<endl;

    return;
}

void redo(string id){

    if (head == NULL) {
        cout << "No accounts found" << endl;
        return;
    }

    customernode* temp = head;

    while (temp != NULL && temp->accid != id)
    {
        temp = temp->next;
    }

    if(temp==NULL){
        cout<<"Account not found."<<endl;
        return;
    }


    if(temp->redoStack.isEmpty()){
        cout<<"No transactions to redo."<<endl;
        return;
    }

  
    string rsn = temp->redoStack.peekReason();   
    double amt = temp->redoStack.peekAmount();
    string datetime = temp->redoStack.peekdatetime();
    

    temp->trQueue.enqueue(rsn, amt,datetime);

    temp->balance = temp->balance + amt;
    temp->redoStack.pop();

    cout << "Redo successful: " << rsn << " (" << amt << ")" << endl;
}


int countcustomers()
{
    int count = 0;
    if(head == NULL)
    {
        return 0;
    }
    else
    {
        customernode * temp = head;
        while(temp != NULL)
        {
            count++;
            temp = temp->next;

        }
    }
    return count;
}

bool deleteacc(string id)
{
    if (head == nullptr) {
        cout << "No accounts exist." << endl;
        return false;
    }

    customernode* temp = head;
    customernode* prev = nullptr;

    while (temp != nullptr && temp->accid != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "Account not found." << endl;
        return false;
    }

    if (prev == nullptr) {
        head = head->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;

    cout << "Account deleted successfully." << endl;
    return true;
}


bool transfermoney(string id1,string id2,double amt) //some changes to formating, to and from way
{
    customernode * t1 = searchCustomer(id1);
    customernode  * t2 = searchCustomer(id2);
    if(t1 == NULL || t2 == NULL)
    {
        cout<<"Id not found "<<endl;
        return false;
    }

    if(t1==t2){
        cout<<"Can't transfer money to self."<<endl;
        return false;
    }

    if(t1->balance < amt)
    {
        cout<<"Insufficient balance"<<endl;
        return false;
    } 

    string withdrawReason = "Transfer to " + id2;
    string depositReason  = "Transfer from " + id1;

    bool check2 = withdrawamt(id1,amt,withdrawReason);

    if(check2 == false)
    {
        cout<<"Failed to withdraw money from sender."<<endl;
        return false;
    }
    
    bool check1 = depositMoney(id2,amt,depositReason);
    if(check1 == false)
    {
        cout<<"Failed to deposit money to the rciever"<<endl;
        check1 = depositMoney(id1,amt,"Transfer");
        return false;
    }

    cout << "Amount successfully transferred from " << id1 << " to " << id2 << endl;
    return true;
}


};

int main()
{
    
    cout << fixed << setprecision(2); //power of 10 show na ho in output
    bank hbl; 
    int choice;
    string id;
    string name;
    double amount;
    string reason;

    while (true)   // main program loop
    {
        cout << "\n===== BANK MANAGEMENT SYSTEM =====\n";
        cout << "1. Open Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Customer Login (Select Account)\n";
        cout << "4. Exit Program\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cin.ignore();
            cout << "Enter Customer Name: ";
            getline(cin, name);
            amount = getValidAmount("Enter Initial Balance: ");

            bool flag = hbl.openaccount(name, amount);
            if (flag) cout << "Account Created Successfully!\n";
        }

        else if (choice == 2)
        {
            hbl.displayallacc();
        }

        else if (choice == 3)
        {
            cout << "Enter Customer ID to Login: ";
            cin >> id;

            customernode* acc = hbl.searchCustomer(id);
            if (acc == nullptr)
                continue;

            //Logged in customer menu:
            while (true)
            {
                cout << "\n===== CUSTOMER MENU =====\n";
                cout << "Logged in: " << id << endl;
                cout << "1. Display Account\n";
                cout << "2. Deposit Money\n";
                cout << "3. Withdraw Money\n";
                cout << "4. Undo Last Transaction\n";
                cout << "5. Redo Last Transaction\n";
                cout << "6. Delete Account\n";
                cout<< "7. Transfer Money\n";
                cout <<"8. Logout\n";
                cout <<"Enter choice: ";
                cin >> choice;
                cout<<endl;

                if (choice == 1)
                {
                    acc->display();
                }
                else if (choice == 2)
                {
                    amount = getValidAmount("Enter Amount: ");
                    
                    cout << "Enter Reason: ";
                    cin.ignore();
                    getline(cin, reason);
                    auto start = chrono::high_resolution_clock::now();

                    hbl.depositMoney(id, amount, reason);

                    auto end = chrono::high_resolution_clock::now();

                    cout << "Execution Time (depositMoney): "
                     << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
                         << " ns\n";

                }
                else if (choice == 3)
                {
                    amount = getValidAmount("Enter Amount: ");
                    cout << "Enter Reason: ";
                    cin.ignore();
                    getline(cin, reason);
                    auto start = chrono::high_resolution_clock::now();
                    hbl.withdrawamt(id, amount, reason);
                    auto end = chrono::high_resolution_clock::now();

                    cout << "Execution Time (withdrawMoney): "
                     << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
                         << " ns\n";
                }
                else if (choice == 4)
                {   
                    auto start = chrono::high_resolution_clock::now();
                    hbl.undo(id);
                    auto end = chrono::high_resolution_clock::now();

                    cout << "Execution Time (Undo): "
                     << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
                         << " ns\n";
                }
                else if (choice == 5)
                {
                    auto start = chrono::high_resolution_clock::now();
                    hbl.redo(id);
                    auto end = chrono::high_resolution_clock::now();

                    cout << "Execution Time (Undo): "
                     << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
                         << " ns\n";
                }
                else if (choice == 6)
                {
                    hbl.deleteacc(id);
                    break; // exit login menu
                }
                else if (choice == 7)
                {
                    string toID;
                    cout << "Enter Receiver's Customer ID: ";
                    cin >> toID;
                    amount = getValidAmount("Enter Amount: ");

                    auto start = chrono::high_resolution_clock::now();

                    hbl.transfermoney(id, toID, amount);

                    auto end = chrono::high_resolution_clock::now();
    
                     cout << "Time (transfermoney): "
                   << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
                    << " ns\n";
}

                  else if (choice == 8)   
                {
                    cout << "Logged Out Successfully!\n";
                    break;
                }
                else
                {
                    cout << "Invalid choice.\n";
                }
            }
        }

        else if (choice == 4)
        {
            cout << "\nTHANK YOU FOR USING THE SYSTEM!\n";
            break;
        }

        else
        {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}


//improvments: 1)can add reason as : "transfered from acc x to y"
//
//
//
//

