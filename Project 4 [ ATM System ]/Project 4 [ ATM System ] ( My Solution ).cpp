
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


struct stClients
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	//bool MarkForDelete = false;
};

enum enMainMenueOption
{
	eQuickWithdraw = 1, eNormalWithdraw = 2, enDeposit = 3,
	enCheckBalance = 4, enLogout = 5
};

string CLientsFileName = "ClientsData.txt";
stClients CurrentClient;


void ShowMainMenueScreen();
void Login();


vector<string> SplitString(string Line, string Dlim = "#//#")
{
	vector<string>vString;

	string sWord = "";
	short Pos = 0;

	while ((Pos = Line.find(Dlim)) != string::npos)
	{
		sWord = Line.substr(0, Pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		Line.erase(0, Pos + Dlim.length());

	}

	if (Line != "")
	{
		vString.push_back(Line);
	}

	return vString;

}

stClients ConvertLineToRecord(string Line, string Dlim = "#//#")
{
	stClients Client;
	vector<string>vString;

	vString = SplitString(Line);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

vector<stClients> LoadDateFromFile(string FileName)
{
	vector<stClients>vClients;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		stClients Client;
		string Line = "";

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);

		}

		MyFile.close();
	}

	return vClients;
}

bool FindClientByAccountNumberAndPassword(string AccountNumber, string PinCode, stClients& Client)
{
	vector<stClients>vClients;

	vClients = LoadDateFromFile(CLientsFileName);

	for (stClients& C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;

}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPassword(AccountNumber, PinCode, CurrentClient))

		return true;
	else
		return false;

}

void ShowMainMenueScreen()
{
	cout << "\n\nPress any key to go back to Main Menue....";
	system("pause>0");
	ShowMainMenueScreen();
}

string  ConvertRecordToLine(stClients Client, string Dlim = "#//#")
{
	string S2 = "";

	S2 += Client.AccountNumber + Dlim;
	S2 += Client.PinCode + Dlim;
	S2 += Client.Name + Dlim;
	S2 += Client.Phone + Dlim;
	S2 += to_string(Client.AccountBalance);

	return S2;

}

void SaveClientDataToFile(vector<stClients>& vClients, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line = "";

		for (stClients& C : vClients)
		{
			Line = ConvertRecordToLine(C);

			MyFile << Line << endl;
		}

		MyFile.close();
	}

}

short ReadQucikWithdrawOption()
{
	short Choice = 0;
	do
	{
		cout << "Choose what do you want to do [1 to 8]? ";
		cin >> Choice;

	} while (Choice < 1 || Choice >9);
	
	return Choice;
}

bool DepositAction(vector<stClients>& vClients, double Amount)
{
	char Answer = 'y';

	cout << "\n\nAre you sure you want to perform this transaction y/n? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{
		for (stClients& C : vClients)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance += Amount;

				cout << "\n\nDone Successfully . New Balance is :" << C.AccountBalance << endl;

				return true;

			}
		}
	}
	else
	{
		cout << "\n\nTransaction Canceled.\n";

	}
	return false;
}

void QucikWithdrawAction(double Amount)
{
	vector<stClients>vClients = LoadDateFromFile(CLientsFileName);


	for (stClients& C : vClients)
	{
		if (C.AccountNumber == CurrentClient.AccountNumber)
		{
			if (C.AccountBalance > Amount)
			{
				DepositAction(vClients, Amount * -1);
				SaveClientDataToFile(vClients, CLientsFileName);
				break;
			}
			else
			{
				cout << "\n\nThe amount exceeds your balance, make another choice.";
				break;
			}
		}
	}

}

void PerformQuickWithdrawScreen(short Number)
{
	switch (Number)
	{
	case 1:
		QucikWithdrawAction(20);
		ShowMainMenueScreen();
		break;

	case 2:
		QucikWithdrawAction(50);
		ShowMainMenueScreen();
		break;

	case 3:
		QucikWithdrawAction(100);
		ShowMainMenueScreen();
		break;

	case 4:
		QucikWithdrawAction(200);
		ShowMainMenueScreen();
		break;

	case 5:
		QucikWithdrawAction(400);
		ShowMainMenueScreen();
		break;

	case 6:
		QucikWithdrawAction(600);
		ShowMainMenueScreen();
		break;

	case 7:
		QucikWithdrawAction(800);
		ShowMainMenueScreen();
		break;

	case 8:
		QucikWithdrawAction(1000);
		ShowMainMenueScreen();
		break;

	case 9:

		ShowMainMenueScreen();
		break;

	}
}

void ShowQucikWithdrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Quick Withdraw\n";
	cout << "===========================================\n";
	cout << "\t[1] 20  \t[2] 50\n";
	cout << "\t[3] 100 \t[4] 200\n";
	cout << "\t[5] 400 \t[6] 600 \n";
	cout << "\t[7] 800 \t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "===========================================\n";

	cout << "\nYour Balance is : " << GetNewBalance() << endl;
	PerformQuickWithdrawScreen(ReadQucikWithdrawOption());
}

void ShowDepositScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Deposit Screen\n";
	cout << "===========================================\n";

	int Amount = 0;
	char Answer = 'y';

	vector<stClients>vClients = LoadDateFromFile(CLientsFileName);

	do
	{
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount;

	} while (Amount < 0);

	DepositAction(vClients, Amount);
	SaveClientDataToFile(vClients, CLientsFileName);
}

void ShowNormalWithdrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Normal Withdraw Screen\n";
	cout << "===========================================\n";

	vector<stClients>vClients = LoadDateFromFile(CLientsFileName);

	int Amount = 0;

	do
	{
		cout << "\nEnter an amount Multiple of 5's ?";
		cin >> Amount;

	} while (Amount % 5 != 0);


	QucikWithdrawAction(Amount);

}

double GetNewBalance()
{
	vector<stClients>vClients = LoadDateFromFile(CLientsFileName);

	for (stClients& C : vClients)
	{
		if (CurrentClient.AccountNumber == C.AccountNumber)
		{
			return C.AccountBalance;
		}
	}
}

void ShowCheckBalanceScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Check Balance Screen\n";
	cout << "===========================================\n";

	cout << "\nYour Balance is : " << GetNewBalance();

}

short ReadMainMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do [1 to 5]? ";
	cin >> Choice;

	return Choice;
}

void  PerformMainMenueScreen(enMainMenueOption Option)
{
	switch (Option)
	{

	case enMainMenueOption::eQuickWithdraw:
		ShowQucikWithdrawScreen();
		break;

	case enMainMenueOption::eNormalWithdraw:
		ShowNormalWithdrawScreen();
		ShowMainMenueScreen();
		break;

	case enMainMenueOption::enDeposit:
		ShowDepositScreen();
		ShowMainMenueScreen();
		break;

	case enMainMenueOption::enCheckBalance:
		ShowCheckBalanceScreen();
		ShowMainMenueScreen();
		break;

	case enMainMenueOption::enLogout:
		Login();
		break;
	}
}

void ShowMainMenueScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===========================================\n";


	PerformMainMenueScreen(enMainMenueOption(ReadMainMenueOption()));

}

void Login()
{

	bool IsLoginFaild = false;
	string AccountNumber, PinCode;

	do
	{
		system("cls");
		cout << "---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";

		if (IsLoginFaild)
		{
			cout << "Invilad AccoutnNumber or Password!\n";
		}

		cout << "Enter AccountNumber? ";
		getline(cin >> ws, AccountNumber);

		cout << "Enter PinCode? ";
		getline(cin >> ws, PinCode);

		IsLoginFaild = !LoadClientInfo(AccountNumber, PinCode);


	} while (IsLoginFaild);

	ShowMainMenueScreen();
}

int main()
{
	Login();

}

