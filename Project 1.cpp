#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

const string UserFileName = "Users.txt";

void ShowMainMenue(); 
void ShowTransactionsMenue();
void ShowUserMenu();
bool HasPermission(short, int);
void PermissionAccessDenied();



struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};

struct sUser
{
    string Username;
    string Password;
    short permission;
    bool MarkForDeleteUser = false;
};

sUser CurrentUser;

enum enPermissions {
    pListClients = 1 << 0,
    pAddClient = 1 << 1,
    pDeleteClient = 1 << 2,
    pUpdateClient = 1 << 3,
    pFindClient = 1 << 4,
    pTransactions = 1 << 5,
    pManageUsers = 1 << 6
};


vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen()
{


    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4,
                           eFindClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7 , 
                            elogout = 8 };

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowUserMenu();

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue();

    }
    }

}

void ShowTransactionsMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        if (!HasPermission(CurrentUser.permission, pListClients))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eAddNewClient:
    {
        if (!HasPermission(CurrentUser.permission, pAddClient))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eDeleteClient:
    {
        if (!HasPermission(CurrentUser.permission, pDeleteClient))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eUpdateClient:
    {
        if (!HasPermission(CurrentUser.permission, pUpdateClient))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eFindClient:
    {
        if (!HasPermission(CurrentUser.permission, pFindClient))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eShowTransactionsMenue:
    {
        if (!HasPermission(CurrentUser.permission, pTransactions))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowTransactionsMenue();
        break;
    }

    case enMainMenueOptions::eManageUsers:
    {
        if (!HasPermission(CurrentUser.permission, pManageUsers))
        {
            PermissionAccessDenied();
            ShowMainMenue();
            break;
        }
        system("cls");
        ShowUserMenu();
        break;
    }
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Exit.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

sUser ConvertUserLinetoRecord(string Line, string Seperator = "##")
{

    sUser User;
    vector<string> vUser;

    vUser = SplitString(Line, Seperator);

    User.Username = vUser[0];
    User.Password = vUser[1];
    User.permission = stoi(vUser[2]);


    return User;

}

string ConvertUserRecordToLine(sUser User, string Seperator = "##")
{
    string Line = "";
    Line += User.Username + Seperator;
    Line += User.Password + Seperator;
    Line += to_string(User.permission);
    return Line;
}

void AddUserToFile(sUser User)
{
    fstream MyFile;
    MyFile.open(UserFileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << ConvertUserRecordToLine(User) << endl;
        MyFile.close();
    }
}

vector <sUser> LoadUserDataFromFile(string FileName)
{

    vector <sUser> vUser;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {

            User = ConvertUserLinetoRecord(Line);

            vUser.push_back(User);
        }

        MyFile.close();

    }

    return vUser;

}

bool CheckUser(string Username, string Password)
{
    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);

    for (sUser u : vUsers)
    {
        if (u.Username == Username && u.Password == Password)
            return true;
    }

    return false;

}

void loginScreen(string ErrorMessage = "")
{
    system("cls");
    cout << "---------------------------------------------\n";
    cout << "                 Login Screen                \n";
    cout << "---------------------------------------------\n";

    if (ErrorMessage != "")
    {
        cout << ErrorMessage << "\n\n";
    }


    string Username, Password;

    cout << "Enter Username: ";
    cin >> Username;

    cout << "Enter Password: ";
    cin >> Password;

    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);

    for (sUser u : vUsers)
    {
        if (u.Username == Username && u.Password == Password)
        {
            CurrentUser = u;
            system("cls");
            ShowMainMenue();
            return;
        }
    }

    loginScreen("Invalid Username or Password!");
}


enum enManageUsers {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenu2 = 6
};

bool HasPermission(short UserPermission, int NeededPermission)
{
    if (UserPermission == -1) // full access
        return true;

    return (UserPermission & NeededPermission);
}

void PermissionAccessDenied()
{
    cout << "\n\n=====================================\n";
    cout << " Access Denied! You don't have permission.\n";
    cout << "=====================================\n";
    system("pause");
}

void PrintUsersRecordLine(sUser User)
{
    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(12) << left << User.permission;
}

void Printtheuserinfo(sUser User)
{
    cout << "\nThe following are the Client details:";
    cout << "\n-------------------------------------------------";
    cout << "\nUsername : " << User.Username;
    cout << "\nPassword : " << User.Password; 
    cout << "\nPermission : " << User.permission;
    cout << "\n-------------------------------------------------\n";
}

void ShowAllUsersScreen()
{
    vector <sUser> vUser = LoadUserDataFromFile(UserFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUser.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions ";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUser.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (sUser U : vUser)
        {

            PrintUsersRecordLine(U);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}
 
bool UserExists(string Username)
{
    vector <sUser> vUsers = LoadUserDataFromFile(UserFileName);

    for (sUser U : vUsers)
    {
        if (U.Username == Username)
            return true;
    }
  
    return false;
}

short ReadUserPermissions()
{
    short permission = 0;

    char FullAccess ;

    cout << "\nDo you want to give full access? y/n   ";
    cin >> FullAccess;

    if (tolower(FullAccess) == 'y')
        return  -1;
    
      char Answer;

        cout << "\nDo you want to give access to:\n";

        cout << "Show Client List? y/n? ";
        cin >> Answer;

        if (tolower(Answer) == 'y') permission |= 1 << 0;

        cout << "Add New Client? y/n? ";
        cin >> Answer;
        if (tolower(Answer) == 'y') permission |= 1 << 1;

        cout << "Delete Client? y/n? ";
        cin >> Answer;
        if (tolower(Answer) == 'y') permission |= 1 << 2;

        cout << "Update Client? y/n? ";
        cin >> Answer;
        if (tolower(Answer) == 'y') permission |= 1 << 3;

        cout << "Find Client? y/n? ";
        cin >> Answer;
        if (tolower(Answer) == 'y') permission |= 1 << 4;

        cout << "Transactions? y/n? ";
        cin >> Answer;
        if (tolower(Answer) == 'y') permission |= 1 << 5;

        cout << "Manage Users? y/n? ";
        cin >> Answer;
        if (tolower(Answer) == 'y') permission |= 1 << 6;

        return permission;

}

sUser ReadNewUser()
{
    sUser User;

    cout << "Enter Username: ";
    cin >> User.Username;

    while (UserExists(User.Username))
    {
        cout << "\nUser with [" << User.Username << "] already exists, Enter another Username? ";
        cin >> User.Username;
    }

    cout << "Enter Password: ";
    cin >> User.Password;

    User.permission = ReadUserPermissions();


    return User;
}

void ShowAddNewUserScreen()
{
    char AddMore = 'Y';

    do 
    {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    sUser User = ReadNewUser();
    AddUserToFile(User);

    cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
    cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

string ReadUsername()
{
    string Username = "";

    cout << "\nPlease enter username? ";
    cin >> Username;
    return Username;

}

bool FindUserByUserName(string Username, vector <sUser> vUser, sUser& User)
{

    for (sUser U : vUser)
    {

        if (U.Username == Username)
        {
            User = U;
            return true;
        }

    }
    return false;

}

bool MarkUserForDelete(string Username, vector<sUser>& vUser)
{
    for (sUser& U : vUser)
    {
        if (U.Username == Username)
        {
            U.MarkForDeleteUser = true;
            return true;
        }
    }
    return false;
}

vector <sUser> SaveUsersDataToFile(string FileName, vector <sUser> vUser)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sUser C : vUser)
        {

            if (C.MarkForDeleteUser == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertUserRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUser;

}

bool DeleteUserByUsername(string Username, vector <sUser>& vUser)
{

    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(Username, vUser, User))
    {

        Printtheuserinfo(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            MarkUserForDelete(Username, vUser);
            SaveUsersDataToFile(UserFileName, vUser);

            //Refresh Clients 
            vUser = LoadUserDataFromFile(UserFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
        return false;
    }

    return false;

}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUser = LoadUserDataFromFile(UserFileName);
    string Username = ReadUsername();
    DeleteUserByUsername(Username, vUser);

}

sUser ReadUpdatedUserInfo(string Username)
{
    sUser User;
    User.Username = Username;

    cout << "\nEnter new Password: ";
    cin >> User.Password;

    cout << "\nUpdating Permissions...\n";
    User.permission = ReadUserPermissions();

    return User;
}

bool UpdateUserByUsername(string Username, vector<sUser>& vUser)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(Username, vUser, User))
    {
        Printtheuserinfo(User);

        cout << "\n\nAre you sure you want to update this User? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& U : vUser)
            {
                if (U.Username == Username)
                {
                    U = ReadUpdatedUserInfo(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UserFileName, vUser);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
        return false;
    }

    return false;
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUser = LoadUserDataFromFile(UserFileName);
    string Username = ReadUsername();

    UpdateUserByUsername(Username, vUser);
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUser = LoadUserDataFromFile(UserFileName);
    sUser User;
    string Username = ReadUsername();
    if (FindUserByUserName(Username, vUser, User))
        Printtheuserinfo(User);
    else
        cout << "\nUser with Username[" << Username << "] is not found!";

}

void PerfromMainMenueUserOption (enManageUsers ManageUsers)
{ 
    switch (ManageUsers)
    {
    case eListUsers:
    {
        if (!HasPermission(CurrentUser.permission, pManageUsers))
        {
            PermissionAccessDenied();
            ShowUserMenu();
            break;
        }
        system("cls");
        ShowAllUsersScreen();
        system("pause");
        ShowUserMenu();
        break;
    }

    case eAddNewUser:
    {
        if (!HasPermission(CurrentUser.permission, pManageUsers))
        {
            PermissionAccessDenied();
            ShowUserMenu();
            break;
        }
        system("cls");
        ShowAddNewUserScreen();
        system("pause");
        ShowUserMenu();
        break;
    }

    case eDeleteUser:
    {
        if (!HasPermission(CurrentUser.permission, pManageUsers))
        {
            PermissionAccessDenied();
            ShowUserMenu();
            break;
        }
        system("cls");
        ShowDeleteUserScreen();
        system("pause");
        ShowUserMenu();
        break;
    }

    case eUpdateUser:
    {
        if (!HasPermission(CurrentUser.permission, pManageUsers))
        {
            PermissionAccessDenied();
            ShowUserMenu();
            break;
        }
        system("cls");
        ShowUpdateUserScreen();
        system("pause");
        ShowUserMenu();
        break;
    }

    case eFindUser:
    {
        if (!HasPermission(CurrentUser.permission, pManageUsers))
        {
            PermissionAccessDenied();
            ShowUserMenu();
            break;
        }
        system("cls");
        ShowFindUserScreen();
        system("pause");
        ShowUserMenu();
        break;
    }

    }

}

short ReadUsereOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowUserMenu()
{
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tMain Users Menue Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] List Users.\n";
        cout << "\t[2] Add New User .\n";
        cout << "\t[3] Delete User.\n";
        cout << "\t[4] Update User.\n";
        cout << "\t[5] Find User.\n";
        cout << "\t[6] Main Menu.\n";
        cout << "===========================================\n";
        PerfromMainMenueUserOption((enManageUsers)ReadUsereOption());
}

int main()
{
    loginScreen();
    return 0;
}