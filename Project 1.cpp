#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

enum enChoiceMenuScreen 
{
    ShowClientList=1,
    AddNewClient=2,
    DeletClient=3,
    UpdateClientInfo=4,
    FindClient=5,
    Exit=6
};

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    string AccountBalance;
    bool MarkForDelete = false;
    bool MarkForUpdate = false;
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
   
    size_t pos = 0;

    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "") {
        vString.push_back(S1);
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

    Client.AccountBalance = vClientData[4];

    Client.MarkForDelete = false;

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;

    stClientRecord += Client.PinCode + Seperator;

    stClientRecord += Client.Name + Seperator;

    stClientRecord += Client.Phone + Seperator;

    stClientRecord += Client.AccountBalance;

    return stClientRecord;
}

vector <sClient> LoadClientsDataFromFile(string ClientsFileName)
{

    vector <sClient> vClients;

    fstream MyFile;

    MyFile.open(ClientsFileName, ios::in);

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

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            DataLine = ConvertRecordToLine(C);
            MyFile << DataLine << endl;

        }
        MyFile.close();
    }
    return vClients;
}

vector <sClient> SaveCleintsDataToFileForDelete(string FileName, vector<sClient> vClients)
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
              
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
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

sClient AddNewClients(string AccountNumber, vector<sClient>& vClients, sClient& Client)
{
   

   
    cout << "\nEnter Account Number :";
    getline(cin>>ws, AccountNumber);
  
    while (FindClientByAccountNumber(AccountNumber, vClients, Client)!=false)
    {
        cout << "Client with [" << AccountNumber << "] already exists, Enter another Account Number?";
        getline(cin, AccountNumber);
    }
    
    Client.AccountNumber = AccountNumber;

    cout << "Enter Pin Code : ";
    getline(cin, Client.PinCode);
  
    cout << "Enter Name : ";
    getline(cin, Client.Name);

    cout << "Enter Phone : ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance : ";
    cin >> Client.AccountBalance;

    vClients.push_back(Client);

    return Client;


}

char readMore()
{
    char c;

    cout << "\n\nClient Added Successfully, Do you want to add more clients? 'y' or 'n': " ;
    cin >> c;

    cin.ignore(1, '\n');

    return tolower(c);
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

bool MarkClientForUpdateByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForUpdate = true;
            return true;
        }
    }
    return false;
}

void PrintClientCard(sClient Client)
{
  
    cout << "\nThe following are the client details:\n";
    cout << "\------------------------------------------\n";
    cout << "Account Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance << endl;
    cout << "\------------------------------------------\n";
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";

    cin >> AccountNumber;

    return AccountNumber;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want delete this client? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')

        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);

            SaveCleintsDataToFileForDelete(ClientsFileName, vClients);

            cout << "\nClient Deleted Successfully.\n\n";

            vClients = LoadClientsDataFromFile(ClientsFileName);

            return true;
        }



    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
        return false;
    }
    return false;







}

void UpdateDataClient(string AcountNumber, vector<sClient>& vClients)
{


    for (sClient& C : vClients)
    {
        if (AcountNumber == C.AccountNumber)
        {

            cout << "\n\n\nEnter Pin Code : ";
            getline(cin >> ws, C.PinCode);

            cout << "Enter Name : ";
            getline(cin, C.Name);

            cout << "Enter Phone : ";
            getline(cin, C.Phone);

            cout << "Enter Account Balance : ";
            getline(cin, C.AccountBalance);
            break;
        }


    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);


        cout << "\nAre you sure you want Update this client? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')

        {

            UpdateDataClient(AccountNumber, vClients);

            SaveCleintsDataToFile(ClientsFileName, vClients);





            cout << "\nClient Updated Successfully.\n";



            return true;
        }



    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
        return false;
    }
    return false;







}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
        << endl;
}

void AddClientScreen(string AccountNumber, vector<sClient> &vClients, sClient& Client)
{
    
    cout << "\n_______________________________________\n\n";
    cout << "\tAdding New Client Screen  \n";
    cout << "_______________________________________\n\n";
    cout << "Adding New Client :  \n";
    
    AddNewClients(AccountNumber, vClients, Client);
    SaveCleintsDataToFile(ClientsFileName, vClients);



    while (readMore() == 'y')
    {
        cout << "\n\n";
        AddNewClients(AccountNumber, vClients, Client);
        SaveCleintsDataToFile(ClientsFileName, vClients);
    }

   


}

int ReturnChoiceMenueScreen()
{
    int choice;
    cout << "Choose what do you want to do? [1 to 6]: ";
    cin >> choice;


    return choice;

}

void MainMenueScreen()
{
 
    cout << "===================================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===================================================\n";
    cout << "\t[1] Show ClientList.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delet Client.\n";
    cout << "\t[4] Update ClientInfo.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "===================================================\n";
   

}

void ReturnMenueScreen()
{
    system("pause");
     system("cls");
    MainMenueScreen();
}

void DeleteClientScreen(string AcountNumber,vector<sClient>& vClients)
{
    cout << "\n------------------------------------------\n";
    cout << "\tDelete Client Screen \n";
    cout << "------------------------------------------\n";
    MarkClientForDeleteByAccountNumber(AcountNumber, vClients);

    DeleteClientByAccountNumber(ReadClientAccountNumber(), vClients);




}

void UpdateClientInfoScreen(string AcountNumber, vector<sClient>& vClients)
{
    cout << "\n------------------------------------------\n";
    cout << "\tUpdate Client Screen \n";
    cout << "------------------------------------------\n";
    MarkClientForUpdateByAccountNumber(AcountNumber, vClients);

    UpdateClientByAccountNumber(ReadClientAccountNumber(), vClients);



}

void FindClientsScreen( vector<sClient>& vClients,sClient &Client)
{
    cout << "\n------------------------------------------\n";
    cout << "\tFind Client Screen \n";
    cout << "------------------------------------------\n";
    string AcountNumber = ReadClientAccountNumber();

    if ((FindClientByAccountNumber(AcountNumber, vClients, Client)) != false)
    {
        
        PrintClientCard(Client);
        cout << "\n\n";
    }
    else
        cout << "\nClient with Account Number[" << AcountNumber << "] is not found!\n\n";
    
        
}

void HandelMainMenueScreen(string AccountNumber,vector<sClient> &vClients, sClient& Client)
{

    while (true)
    {
        int Choice = ReturnChoiceMenueScreen();

        if (Choice == 6) break;

        system("cls");
        switch (Choice)
        {
        case enChoiceMenuScreen::ShowClientList:

            PrintAllClientsData(vClients);
            ReturnMenueScreen();
            break;
           

        case enChoiceMenuScreen::AddNewClient:

            AddClientScreen(AccountNumber, vClients, Client);
            ReturnMenueScreen();
            break;

        case enChoiceMenuScreen::DeletClient:

            DeleteClientScreen(AccountNumber,vClients);
            ReturnMenueScreen();
            break;

        case enChoiceMenuScreen::UpdateClientInfo:

            UpdateClientInfoScreen(AccountNumber, vClients);
            ReturnMenueScreen();
            break;

        case enChoiceMenuScreen::FindClient:

            FindClientsScreen( vClients, Client);
            ReturnMenueScreen();
            break;
        default:
            break;
        }
    }


}

void BankFielSystem()
{

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber;

    MainMenueScreen();
    HandelMainMenueScreen(AccountNumber, vClients, Client);

 }

int main()  
{
    
    BankFielSystem();



    return 0;
}
