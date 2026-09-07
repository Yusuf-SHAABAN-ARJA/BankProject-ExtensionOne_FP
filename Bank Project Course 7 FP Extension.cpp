#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<conio.h>
#include<cctype>
#include <string>
#include"MyLib.h"
using namespace std;
enum enMainMenueOptions { eShowListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactionsMenue = 6, eExit = 7 };
enum enTransactionsMenueOptions { eShowDepositScreen = 1, eShowWithdrawScreen = 2, eShowTotalBalancesScreen = 3, eShowMainMenueScreen = 4 };
const string ClientsFileName = "Clients.txt";
void ShowMainMenue();
void ShowTransactionsMenue();
struct stClient
{
	string AccountNumber, PinCode, Name, Phone;
	double AccountBalance;
	bool MarkDeleted = false;
};
stClient ConvertLineToRecord(string Line)
{
	stClient Client;

	vector <string> vRecord;

	vRecord = MyStringLib::vSplitString(Line);

	Client.AccountNumber = vRecord[0];
	Client.PinCode = vRecord[1];
	Client.Name = vRecord[2];
	Client.Phone = vRecord[3];
	Client.AccountBalance = stod(vRecord[4]);

	return Client;
}
string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}
vector <stClient>  LoadClientsInfoFromFile(string ClientsFileName, string Seperator = "#//#")
{
	fstream MyFile;

	string Line;

	vector <string> vRecord;

	vector <stClient> vClients;

	stClient Client;
	MyFile.open(ClientsFileName, ios::in);

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			if (Line != "")
			{
				Client = ConvertLineToRecord(Line);
				vClients.push_back(Client);
			}
		}
		MyFile.close();
	}
	return vClients;
}
void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue";
	system("Pause>0");
	ShowMainMenue();
}
void GoBackToTransactionsMenue()
{
	cout << "\n\nPress any key to go back to Transactions Menue";
	system("Pause>0");
	ShowTransactionsMenue();
}
void PrintAllClientsCard(const stClient& Client)
{
	cout << "| " << left << setw(18) << Client.AccountNumber;
	cout << "| " << left << setw(12) << Client.PinCode;
	cout << "| " << left << setw(45) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(20) << Client.AccountBalance << "\n";
}
void ShowClientsOnScreen()
{
	vector <stClient> vClients = LoadClientsInfoFromFile(ClientsFileName);

	cout << "\t\t\t\t\t" << "Client List (" << vClients.size() << ") Client(s).\n";
	cout << "_______________________________________________________________________________________________________________________\n\n\n";
	cout << "| " << left << setw(18) << " Account Number  ";
	cout << "| " << left << setw(12) << " Pin Code ";
	cout << "| " << left << setw(45) << " Client Name ";
	cout << "| " << left << setw(15) << " Phone    ";
	cout << "| " << left << setw(20) << " Balance  " << "\n\n";
	cout << "_______________________________________________________________________________________________________________________\n\n\n";

	for (const stClient& C : vClients)
	{
		PrintAllClientsCard(C);
	}
	cout << "\n_______________________________________________________________________________________________________________________\n\n\n";
}
stClient ReadNewClient();
bool ClientEXistByAccountNumber(string AccountNumber, string ClientsFileName)
{
	fstream MyFile;

	vector <stClient> vClients;

	MyFile.open(ClientsFileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (AccountNumber == Client.AccountNumber)
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
stClient ReadNewClient()
{
	stClient Client;

	cout << "Enter Client Account Number ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientEXistByAccountNumber(Client.AccountNumber, ClientsFileName));
	{
		cout << "Client with Account Number [" << Client.AccountNumber << "] is already exists , Enter Another Account Number ";
		getline(cin, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}
bool FindClientByAccountNumber(const vector <stClient> vClients, stClient& Client, string AccountNumber)
{

	for (const stClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
string ReadClientAccountNumber()
{
	string AccountNumber;
	cout << "Enter Account Number? ";
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}
void AddDataLineToFile(string ClientsFileName, string DataLine)
{
	fstream MyFile;

	MyFile.open(ClientsFileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		cout << DataLine << endl;
		MyFile.close();
	}

}
void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
void AddNewClients()
{
	char AddMore = 'y';
	do
	{
		cout << "Adding New Client \n\n";
		AddNewClient();

		cout << "\n\n Client Added Successfully, do you want to add more client? y/n? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}
void ShowAddNewClientScreen()
{
	cout << "\n-------------------------------------\n";
	cout << "\tAdd New Client Screen\n";
	cout << "-------------------------------------\n";

	AddNewClients();
}
void SaveClientsDataToFile(const vector <stClient>& vClient, string ClientsFileName)
{
	fstream MyFile;

	MyFile.open(ClientsFileName, ios::out);

	string DataLine;

	if (MyFile.is_open())
	{
		for (const stClient& C : vClient)
		{
			if (C.MarkDeleted == false)
			{
				DataLine = ConvertRecordToLine(C);
				if (DataLine != "")
				{
					MyFile << DataLine << endl;
				}
			}

		}
		MyFile.close();
	}
}
short ReadMainMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 6]? ";
	while (!(cin >> Choice) || Choice < 1 || Choice>7)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalide Input , Please try Again :-) ";
	}
	return Choice;
}
short ReadTransactionMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 4]? ";
	while (!(cin >> Choice) || Choice < 1 || Choice>4)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalide Input , Please try Again :-) ";
	}
	return Choice;
}
void PrintOneClientCard(stClient FoundClient)
{
	cout << "\n\nThe following are The Client Details: \n";
	cout << "-------------------------------------------------\n";
	cout << " Account Number  : " << FoundClient.AccountNumber << "\n";
	cout << " Pin Code        : " << FoundClient.PinCode << "\n";
	cout << " Name            : " << FoundClient.Name << "\n";
	cout << " Phone           : " << FoundClient.Phone << "\n";
	cout << " Account Balance : " << FoundClient.AccountBalance << "\n";
	cout << "-------------------------------------------------\n";

}
bool MarkClientForDeleteByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkDeleted = true;
			return true;
		}

	}
	return false;
}
bool DeleteClientByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintOneClientCard(Client);
		cout << "\n\nAre you sure you want to delete this client? y/n? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
			SaveClientsDataToFile(vClients, ClientsFileName);
			cout << "\n\nClient Deleted Successfully. \n";

			//Refresh 

			vClients = LoadClientsInfoFromFile(ClientsFileName);
			return true;
		}
	}
	else
	{
		cout << "\n\n Client with Account Number (" << AccountNumber << ") Not Found! \n";
	}
}
void ShowDeleteClientScreen()
{

	cout << "\n-------------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "-------------------------------------\n\n";

	vector <stClient> vClients = LoadClientsInfoFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(vClients, AccountNumber);
}
stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;


	return Client;
}
bool UpdateClientByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintOneClientCard(Client);
		cout << "\n\nAre you sure you want to Update this client? y/n? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			cout << "\n\nClient Updated Successfully. \n";
			SaveClientsDataToFile(vClients, ClientsFileName);
			//Refresh 
			return true;
		}
	}
	else
	{
		cout << "\n\n Client with Account Number (" << AccountNumber << ") Not Found! \n";
	}
}
void ShowUpdateClientScreen()
{
	cout << "\n-------------------------------------\n";
	cout << "\tUpdate Client Screen\n";
	cout << "-------------------------------------\n\n";

	vector <stClient> vClient = LoadClientsInfoFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(vClient, AccountNumber);
}
void ShowFindClientScreen()
{
	stClient Client;

	cout << "\n-------------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "-------------------------------------\n\n";
	string AccountNumber = ReadClientAccountNumber();

	const vector <stClient> vClients = LoadClientsInfoFromFile(ClientsFileName);

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintOneClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account Number [" << AccountNumber << "] Not Found! \n";
	}
}
void ShowExitScreen()
{
	system("cls");
	cout << "---------------------------------\n";
	cout << "\t   Program Ends :-)\n";
	cout << "---------------------------------\n";

}
void ShowMainMenue();
void ShowTransactionsMenue();
void PerformMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enMainMenueOptions::eShowListClients:
	{
		system("cls");
		ShowClientsOnScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eAddNewClient:
	{
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eDeleteClient:
	{
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::eUpdateClient:
	{
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eFindClient:
	{
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eTransactionsMenue:
	{
		system("cls");
		ShowTransactionsMenue();
	}
	case enMainMenueOptions::eExit:
	{
		system("cls");
		ShowExitScreen();
		break;
	}
	}
}
void DepositBalanceToClientByAccountNumber(vector <stClient>& vClients, double Amount, string AccountNumber)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.AccountBalance += Amount;
			SaveClientsDataToFile(vClients, ClientsFileName);
			cout << "\n\n Done Successfully. New balance is: " << C.AccountBalance;
			return;
		}
	}
}
void ShowDepositScreen()
{
	cout << "\n-------------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "-------------------------------------\n\n";

	stClient Client;
	vector<stClient> vClients = LoadClientsInfoFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	double Amount = 0.0;

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		cout << "Client with Account Number [" << AccountNumber << "] does not exist , please enter another account number ";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintOneClientCard(Client);

	cout << "Enter The Amount Please ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(vClients, Amount, AccountNumber);

}
void ShowWithdrawScreen()
{
	cout << "\n-------------------------------------\n";
	cout << "\tWithdraw Screen\n";
	cout << "-------------------------------------\n\n";

	stClient Client;
	vector<stClient> vClients = LoadClientsInfoFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	double Amount = 0.0;

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		cout << "Client with Account Number [" << AccountNumber << "] does not exist , please enter another account number ";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintOneClientCard(Client);

	cout << "Enter The Amount Please ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "Sorry the amount execeeds the balance you can withdraw up to " << Client.AccountBalance << "\n";
		cout << "Please enter another amount ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(vClients, Amount * -1, AccountNumber);

}
void PrintAllClientsBalanceCard(stClient Client)
{
	cout << "| " << left << setw(18) << Client.AccountNumber;
	cout << "| " << left << setw(45) << Client.Name;
	cout << "| " << left << setw(20) << Client.AccountBalance << "\n";
}
void ShowTotalBalancesScreen()
{
	vector <stClient> vClients = LoadClientsInfoFromFile(ClientsFileName);

	cout << "\t\t\t\t\t" << "Client List (" << vClients.size() << ") Client(s).\n";
	cout << "_______________________________________________________________________________________________________________________\n\n\n";
	cout << "| " << left << setw(18) << " Account Number  ";
	cout << "| " << left << setw(45) << " Client Name ";
	cout << "| " << left << setw(20) << " Balance  " << "\n\n";
	cout << "_______________________________________________________________________________________________________________________\n\n\n";

	double TotalBalance = 0.0;

	for (const stClient& C : vClients)
	{
		PrintAllClientsBalanceCard(C);
		TotalBalance += C.AccountBalance;
	}
	cout << "\n_______________________________________________________________________________________________________________________\n\n\n";
	cout << "\t\t\t\t\tTotal Balance = " << TotalBalance << "\n\n";
}
void PerformTransactionMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
	switch (TransactionMenueOption)
	{
	case enTransactionsMenueOptions::eShowDepositScreen:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case enMainMenueOptions::eAddNewClient:
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case enMainMenueOptions::eDeleteClient:
	{
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenue();
		break;
	}

	case enMainMenueOptions::eUpdateClient:
	{
		system("cls");
		ShowMainMenue();
		break;
	}
	}
}

void ShowTransactionsMenue()
{
	system("cls");
	cout << "=========================================\n";
	cout << "\t    Transaction Menue Screen\n";
	cout << "=========================================\n";
	cout << "\t [1] Deposit. \n";
	cout << "\t [2] Withdraw. \n";
	cout << "\t [3] Total Balances. \n";
	cout << "\t [4] Main Menue. \n";
	cout << "=========================================\n";
	PerformTransactionMenueOption((enTransactionsMenueOptions)(ReadTransactionMenueOption()));
}
void ShowMainMenue()
{
	system("cls");
	cout << "=========================================\n";
	cout << "\t    Main Menue Screen\n";
	cout << "=========================================\n";
	cout << "\t [1] Show Client Screen. \n";
	cout << "\t [2] Add New Client. \n";
	cout << "\t [3] Delete Client. \n";
	cout << "\t [4] Update Client. \n";
	cout << "\t [5] Find Client. \n";
	cout << "\t [6] Transactions Menue\n";
	cout << "\t [7] Exit. \n";
	cout << "=========================================\n";
	PerformMainMenueOption((enMainMenueOptions)(ReadMainMenueOption()));
}
int main()
{
	ShowMainMenue();

	system("Pause");
	return 0;
}