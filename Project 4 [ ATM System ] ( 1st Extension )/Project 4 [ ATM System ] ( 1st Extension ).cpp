
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
	enCheckBalance = 4, enUpdatePIN = 5, enShowMyInfo = 6, enLogout = 7
};

const string CLientsFileName = "ClientsData.txt";
stClients CurrentClient;


void GoBackToMainMenueScreen();
void ShowMainMenueScreen();
void ShowQucikWithdrawScreen();
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

vector<stClients> LoadDataFromFile(string FileName)
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

void GoBackToMainMenueScreen()
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

	while (Choice < 1 || Choice > 9)
	{
		cout << "Choose what do you want to do [1] to [9]? ";
		cin >> Choice;

	}

	return Choice;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, vector<stClients>& vClients, double Amount)
{
	char Answer = 'y';

	cout << "\n\nAre you sure you want to perform this transaction y/n? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{
		for (stClients& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;

				SaveClientDataToFile(vClients, CLientsFileName);

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

short getQuickWithdrawAmount(short Number)
{
	switch (Number)
	{
	case 1:
		return 20;

	case 2:
		return 50;
	case 3:
		return 100;

	case 4:
		return 200;

	case 5:
		return 400;

	case 6:
		return 600;

	case 7:
		return 800;

	case 8:
		return 1000;

	default:
		return 0;

	}
}

void PerformQuickWithdrawOption(short QuicWithdrawOption)
{
	if (QuicWithdrawOption == 9)
		return;

	short WithDrawBalance = getQuickWithdrawAmount(QuicWithdrawOption);

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "\nPress any key to continue....";
		system("pause>0");
		ShowQucikWithdrawScreen();
		return;
	}

	vector<stClients>vClients = LoadDataFromFile(CLientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClients, WithDrawBalance * -1);
	CurrentClient.AccountBalance -= WithDrawBalance;

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

	cout << "\nYour Balance is : " << CurrentClient.AccountBalance << endl;
	PerformQuickWithdrawOption(ReadQucikWithdrawOption());
}

double ReadDepositAmount()
{
	double Amount = 0;
	do
	{
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount;

	} while (Amount <= 0);

	return Amount;
}

void PerformDepositOption()
{
	double DepositAmount = ReadDepositAmount();

	vector<stClients>vClients = LoadDataFromFile(CLientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClients, DepositAmount);
	CurrentClient.AccountBalance + DepositAmount;
}

void ShowDepositScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Deposit Screen\n";
	cout << "===========================================\n";

	PerformDepositOption();

}

int ReadWithDrawAmount()
{
	int Amount = 0;

	do
	{
		cout << "\nEnter an amount Multiple of 5's ?";
		cin >> Amount;

	} while (Amount % 5 != 0);

	return Amount;
}

void PerformNormalWithdrawOption()
{
	short WithDrawBalance = ReadWithDrawAmount();

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "\nPress any key to continue....";
		system("pause>0");
		ShowQucikWithdrawScreen();
		return;
	}

	vector<stClients>vClients = LoadDataFromFile(CLientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClients, WithDrawBalance * -1);
	CurrentClient.AccountBalance -= WithDrawBalance;

}

void ShowNormalWithdrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Normal Withdraw Screen\n";
	cout << "===========================================\n";

	PerformNormalWithdrawOption();

}

void ShowCheckBalanceScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Check Balance Screen\n";
	cout << "===========================================\n";

	cout << "Your Balance is : " << CurrentClient.AccountBalance << endl;

}

short ReadMainMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do [1 to 7]? ";
	cin >> Choice;

	return Choice;
}

void UpdatePIN(string AccountNumber)
{
	string OldPIN, NewPIN, ConfirmPIN;
	char y;

	cout << "Enter Old PIN: ";
	cin >> OldPIN;

	if (OldPIN == CurrentClient.PinCode)
	{
		cout << "\n\nEnter New PIN: ";
		cin >> NewPIN;

		if (NewPIN.length() > 4)
		{
			cout << "\nPIN must be exactly 4 digits.\n";
		}
		else
		{
			cout << "\nEnter Confirm New PIN: ";
			cin >> ConfirmPIN;


			if (ConfirmPIN == NewPIN)
			{
				cout << "\n\nAre you sure you want to change PIN y/n? ";
				cin >> y;

				if (tolower(y) == 'y')
				{
					vector<stClients>vClients = LoadDataFromFile(CLientsFileName);

					for (stClients& C : vClients)
					{
						if (C.AccountNumber == AccountNumber)
						{
							C.PinCode = ConfirmPIN;
							CurrentClient.PinCode = ConfirmPIN;
							break;
						}
					}
					SaveClientDataToFile(vClients, CLientsFileName);

					cout << "\nPIN updated Successfully.\n";
				}
				else
				{
					cout << "\n\nUpdated Canceled.\n";
				}
			}
			else
			{
				cout << "\nThe PIN are not Similar.\n";
			}
		}
	}
	else
	{
		cout << "\nOld Password is Wrong\n";
	}

	}


void ShowUpdatePINScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t Update PIN Screen\n";
	cout << "===========================================\n";

	UpdatePIN(CurrentClient.AccountNumber);

}

void PrintClientCard(stClients Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number  : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";

}

void ShowMyInfoScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t My Information Screen\n";
	cout << "===========================================\n";

	PrintClientCard(CurrentClient);
}

void  PerformMainMenueScreen(enMainMenueOption Option)
{
	switch (Option)
	{

	case enMainMenueOption::eQuickWithdraw:
		ShowQucikWithdrawScreen();
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOption::eNormalWithdraw:
		ShowNormalWithdrawScreen();
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOption::enDeposit:
		ShowDepositScreen();
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOption::enCheckBalance:
		ShowCheckBalanceScreen();
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOption::enUpdatePIN:
		ShowUpdatePINScreen();
		GoBackToMainMenueScreen();
		break;

	case enMainMenueOption::enShowMyInfo:
		ShowMyInfoScreen();
		GoBackToMainMenueScreen();
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
	cout << "\t[5] Update PIN.\n";
	cout << "\t[6] Show My Information.\n";
	cout << "\t[7] Logout.\n";
	cout << "===========================================\n";


	PerformMainMenueScreen(enMainMenueOption(ReadMainMenueOption()));

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClients& Client)
{
	vector<stClients>vClients;

	vClients = LoadDataFromFile(CLientsFileName);

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
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))

		return true;
	else
		return false;

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

