//******************************************************
//                  HEADER FILES
//******************************************************

#include<iostream>
#include<cctype>
#include<iomanip>
#include<fstream>
#include<string.h>
#include <ctime>
#include<stdlib.h>

using namespace std;

//******************************************************
//                  CLASS DEFINITIONS
//******************************************************

class Account
{
	int acno;
	char name[50];
	float deposit;
	char type;
	int dd_o;
	int mm_o;
	int yyyy_o;
	int done_days;
	int done_months;
    float days_arr[30];
    float mon_int[60];

public:
    bool check_for_same_acno(int);
	void create_account();	//function to get data from user
	void show_account(bool) const;	//function to show data on screen
	void dep(float);	//function to accept amount and add to balance amount
	void draw(float);	//function to accept amount and subtract from balance amount
	void int_days(int);
	void int_months(int);
	void days(float,int,int);
	void report() const;	//function to show data in tabular format
	int retacno() const;	//function to return account number
	float retdeposit() const;	//function to return balance amount
	char rettype() const;	//function to return type of account

    void int_rate_struct();
	int ret_dd_o() const;
	int ret_mm_o() const;
	int ret_yyyy_o() const;
	int ret_int_days() const;
	int ret_int_months() const;

	void write_account();	//module to write new account details in binary file
    void delete_account(int);	//function to delete record of file
    void deposit_withdraw(int, int); // function to deposit/withdraw amount for given Account
    void intro();	//introductory screen function
};         //class ends here


class Interest
{
    static const float ROI = 0.01;
public:
    void int_rate_struct();
    void calc_interest();
};


class Report
{
    int report_num;
public:
    void display_sp(int);	//function to display account details of a particular account
    void display_all();		//function to display all account details
    int return_days(int,int,int);
};


void Account::create_account()
{   bool status = false;
	cout << "\nEnter the account no. : ";
	label1:
	cin >> acno;

	status = check_for_same_acno(acno);
	if(status)
    {
        cout << "\nAccount number already exists!!!\n\n";
        cout << "Enter a different account number: ";
        goto label1;
    }

	cout << "\nEnter the name of the account holder: ";
	cin.ignore();
	cin.getline(name,50);

	label2:
	cout << "\nEnter type of account (C/S): ";
	cin >> type;
	type = toupper(type);

	if(type != 'S' && type != 'C')
    {
        cout << "\nInvalid account type\n";
        goto label2;
    }

	label3:
	cout << "\nEnter the initial amount(>=500 for Savings and >=1000 for Current Account) : ";
	cin >> deposit;

	if((deposit < 500 && type == 'S') || (deposit < 1000 && type == 'C'))
    {
        cout << "\nAccount Creation Failed!!! Insufficient Deposit!!!\n";
        goto label3;
    }

    cout << "";
    time_t t = time(0);   // get time now

    struct tm* now = localtime(&t);

    dd_o = now->tm_mday ;
    mm_o = now->tm_mon + 1;
    yyyy_o = now->tm_year;    // changed from now->tm_year + 1900

    done_days = 0;
    done_months = 0;

    int i = 0;
	for(i = 0 ; i < 30 ; i++)
    {
        days_arr[i] = -1;
    }

	cout << "\nAccount Created Successfully !!!";
}

bool Account::check_for_same_acno(int n)
{
	Account ac;
	bool flag = false;
	ifstream inFile;
	inFile.open("Account.dat",ios::binary);
	if(!inFile)
	{
		cout << "File could not be opened !! Press any key to continue...";
		return flag;
	}

    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(Account)))
	{
		if(ac.retacno() == n)
		{
			flag = true;
			break;
		}
	}
	inFile.close();
	return flag;
}

void Account::show_account(bool choice) const
{
	cout << "\nAccount No. : " << acno;
	cout << "\nAccount Holder Name : ";
	cout << name;
	cout << "\nType of Account : " << type;
	cout << "\nBalance Amount : " << deposit << "\n";

	int i;
	if(choice)
    {
        cout<<"\n\n================\n";
        cout<<"INTEREST DETAILS\n";
        cout<<"================\n";
        for(i = 0 ; i < done_months ; i++)
        {
            cout << "Month " << i+1 << ": "<< mon_int[i] << " - 10% = " << 0.9*mon_int[i] << " credited\n";
        }

        cout << "Current Month:\n";

        for(i = 0; i < done_days ; i++)
        {
            cout << "Day " << i+1 << ": " << days_arr[i] << "\n";
        }
    }
}

void Account::dep(float x)
{
	deposit += x;
}

void Account::draw(float x)
{
	deposit -= x;
}

void Account::int_days(int x)
{
	done_days += x;
}

void Account::int_months(int x)
{
	done_months += x;

	int i;
	float sum = 0;

	for(i = 0 ; i < 30 ; i++)
    {   sum += days_arr[i];
        days_arr[i] = -1;
    }

    mon_int[done_months-1] = sum;
    dep(0.9*sum);
}

void Account::days(float interest, int start, int stop)
{
	int i = 0;
	for(i = start ; i < stop ; i++)
    {
        days_arr[i] = interest;
    }
}

void Account::report() const
{
	int length;
	length = strlen(name);
	cout << acno << setw(10) << " " << name << setw(17-length) << " " << type << setw(8) << deposit << endl;
}

int Account::retacno() const
{
	return acno;
}

float Account::retdeposit() const
{
	return deposit;
}

char Account::rettype() const
{
	return type;
}

int Account::ret_int_days() const
{
	return done_days;
}

int Account::ret_int_months() const
{
	return done_months;
}

int Account::ret_dd_o() const
{
	return dd_o;
}

int Account::ret_mm_o() const
{
	return mm_o;
}

int Account::ret_yyyy_o() const
{
	return yyyy_o;
}


//*****************************************
//    	Interest Rate Structure
//*****************************************

void Interest::int_rate_struct()
{
    cout << "\n\n*****************************************INTEREST RATE STRUCTURE*********************************************";
    cout << "\n\n1. The interest is calculated on a daily basis at a rate of 0.01 % of the balance amount per day.\n";
    cout << "2. The cumulative interest is credited to the account after every 30 days after a deduction of 10% as taxes.\n";
}

void Interest::calc_interest(void)     // function to calculate interest
{
    Account ac;
    Report rep;
    cout << "";
    time_t t = time(0);   // get time now

    struct tm* now = localtime(&t);

    int active_days, done_days_total, rem, rem_days, rem_months;
    int start, stop, month_end, extra, i;
    float interest;

    fstream File;
	File.open("Account.dat", ios::binary|ios::in|ios::out);
	if(!File)
	{
		cout << "File could not be opened !! Press any key to continue...";
		return;
	}

    while(File.read(reinterpret_cast<char *> (&ac), sizeof(Account)))
	{

		active_days = rep.return_days(now->tm_mday, now->tm_mon+1, now->tm_year)
                      - rep.return_days(ac.ret_dd_o(), ac.ret_mm_o(), ac.ret_yyyy_o());
        done_days_total = (ac.ret_int_months() * 30) + ac.ret_int_days();
        rem = active_days - done_days_total;
        rem_days = rem % 30;
        rem_months = rem / 30;
        month_end = 30 - ac.ret_int_days();

        if(rem_months == 0)
        {
            if(rem_days < month_end)
            {
                interest = ROI * ac.retdeposit();
                start = ac.ret_int_days();
                ac.int_days(rem_days);
                stop = ac.ret_int_days();
                ac.days(interest, start, stop);
            }
            else
            {
                interest = ROI * ac.retdeposit();
                start = ac.ret_int_days();
                ac.days(interest, start, 30);
                ac.int_months(1);

                rem_days = rem_days - month_end;
                extra = ac.ret_int_days();
                ac.int_days(rem_days - extra);
                interest = ROI * ac.retdeposit();
                ac.days(interest, 0, rem_days);
            }
        }
        else
        {
            interest = ROI * ac.retdeposit();
            start = ac.ret_int_days();
            ac.days(interest, start, 30);
            ac.int_months(1);

            rem = rem - month_end;
            rem_days = rem % 30;
            rem_months = rem / 30 ;

            for(i = 1 ; i <= rem_months ; i++)
            {
                interest = ROI * ac.retdeposit();
                ac.days(interest, 0, 30);
                ac.int_months(1);
            }

            interest = ROI * ac.retdeposit();
            extra = ac.ret_int_days();
            ac.int_days(rem_days - extra);
            ac.days(interest, 0, rem_days);
        }

        // draft = now->tm_mday - ac.ret_dd_o() - ac.ret_int_days() ;
        int pos=(-1)*static_cast<int>(sizeof(ac));
        File.seekp(pos,ios::cur);
        File.write(reinterpret_cast<char *> (&ac), sizeof(Account));
    }

	File.close();
}


//*******************************************
//    	function to write in file
//*******************************************

void Account::write_account()
{
    Interest inter;
    inter.calc_interest();
	Account ac;
	ofstream outFile;
	outFile.open("Account.dat",ios::binary|ios::app);
	ac.create_account();
	outFile.write(reinterpret_cast<char *> (&ac), sizeof(Account));
	outFile.close();
}

//*********************************************************
//    	function to deposit and withdraw amounts
//*********************************************************

void Account::deposit_withdraw(int n, int option)
{
    Interest inter;
    inter.calc_interest() ;

    Account ac;
	float amt;
	bool found = false;
	fstream File;
	File.open("Account.dat", ios::binary|ios::in|ios::out);
	if(!File)
	{
		cout << "File could not be opened !! Press any key to continue...";
		return;
	}
	while(!File.eof() && !found)
	{
		File.read(reinterpret_cast<char *> (&ac), sizeof(Account));
		if(ac.retacno() == n)
		{
			ac.show_account(false);
			if(option == 1)
			{
				cout << "\n\n\tDEPOSIT AMOUNT";
				cout << "\n\nEnter the amount to be deposited: ";
				cin >> amt;
				ac.dep(amt);
				cout << "\nTransaction Successful !!!";
				cout << "\nBalance Amount: " << ac.deposit;
			}
			if(option == 2)
			{
				cout << "\n\n\tWITHDRAW AMOUNT";
				cout << "\n\nEnter the amount to be withdrawn: ";
				cin >> amt;
				int bal = ac.retdeposit() - amt;
				if((bal < 500 && ac.rettype() == 'S') || (bal < 1000 && ac.rettype() == 'C'))
					cout << "\nInsufficient Balance!!!";
				else
                {
					ac.draw(amt);
                    cout << "\nTransaction Successful!!!";
                    cout << "\nBalance Amount: " << ac.deposit;
                }
			}
			int pos=(-1)*static_cast<int>(sizeof(ac));
			File.seekp(pos,ios::cur);
			File.write(reinterpret_cast<char *> (&ac), sizeof(Account));
			found = true;
        }
	}
	File.close();
	if(!found)
		cout << "\nAccount Not Found !!!";
}

//*******************************************************
//    	module to delete  accounts from file
//*******************************************************

void Account::delete_account(int n)
{
    bool flag = false;
    Account ac;
	ifstream inFile;
	ofstream outFile;
	inFile.open("Account.dat",ios::binary);
	if(!inFile)
	{
		cout << "File could not be opened !! Press any key to continue...";
		return;
	}
	outFile.open("Temp.dat",ios::binary);
	inFile.seekg(0,ios::beg);
	while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(Account)))
	{
		if(ac.retacno() != n)
		{
			outFile.write(reinterpret_cast<char *> (&ac), sizeof(Account));
		}
		else
            flag = true;
	}
	inFile.close();
	outFile.close();
	remove("Account.dat");
	rename("Temp.dat","Account.dat");
	if(flag)
        cout << "\nAccount Closed Successfully !!!";
	else
        cout << "\nAccount Not Found !!!";
}

//***************************************************
//    	Function to calculate dates
//***************************************************

int Report::return_days (int dd, int mm, int yyyy)
{
     int y_days = 0, m_days = 0, days = 0;
     y_days = (((yyyy - 1 - 1900) / 4) * 1461) + (((yyyy - 1 - 1900) % 4) * 365);

    switch(mm)
    {
      case 12: m_days += 30;
      case 11: m_days += 31;
      case 10: m_days += 30;
      case 9: m_days += 31;
      case 8: m_days += 31;
      case 7: m_days += 30;
      case 6: m_days += 31;
      case 5: m_days += 30;
      case 4: m_days += 31;
      case 3: if((yyyy % 4) == 0)
                m_days += 29;
              else
                m_days += 28;
      case 2: m_days += 31;
    }

    days = y_days + m_days + dd;
    return days ;
}

//********************************************
//    	INTRODUCTION FUNCTION
//********************************************

void Account::intro()
{
	cout << "\n\n\n\t  BANKING SYSTEM WITH AUTOMATED";
	cout << "\n\n\t      INTEREST CALCULATION";
	cout << "\n\n\n\n      MADE BY : SHUBHRAJIT SANTRA (MT19196)";
	cin.get();
}


//*************************************************************
//    	function to read specific record from file
//*************************************************************

void Report::display_sp(int n)
{
    Interest inter;
	inter.calc_interest();
	Account ac;

	ifstream inFile;
	bool flag = false;
	inFile.open("Account.dat",ios::binary);
	if(!inFile)
	{
		cout<<"File could not be opened !! Press any key to continue...";
		return;
	}

	cout<<"\n===============";
	cout<<"\nBALANCE DETAILS";
	cout<<"\n===============";

    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(Account)))
	{
		if(ac.retacno() == n)
		{
			ac.show_account(true);
			flag = true;
		}
	}
	inFile.close();
	if(flag == false)
		cout << "\n\nAccount no. does not exist";
}

//***************************************************************
//    	module to display all accounts deposit list
//****************************************************************

void Report::display_all()
{
    Interest inter;
    inter.calc_interest();
	Account ac;
	ifstream inFile;
	inFile.open("Account.dat",ios::binary);
	if(!inFile)
	{
		cout << "File could not be opened !! Press any key to continue...";
		return;
	}
	cout << "\n\n\t\tACCOUNT HOLDERS' LIST\n\n";
	cout << "====================================================\n";
	cout << "A/c no.      NAME           Type  Balance\n";
	cout << "====================================================\n";
	while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(Account)))
	{
		ac.report();
	}
	inFile.close();
}

//**************************************************************
//    	  THE MAIN/DRIVER FUNCTION OF THE PROGRAM
//**************************************************************

int main()
{
	char ch;
	float num;

	Account ac ;
	Interest inter;
	Report rep;

	ac.intro();
	do
	{
		system("CLS");
		cout << "\n\n\n\tMAIN MENU";
		cout << "\n\n\t01. NEW ACCOUNT";
		cout << "\n\n\t02. DEPOSIT AMOUNT";
		cout << "\n\n\t03. WITHDRAW AMOUNT";
		cout << "\n\n\t04. GENERATE REPORT";
		cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
		cout << "\n\n\t06. CLOSE AN ACCOUNT";
		cout << "\n\n\t07. INTEREST RATE STRUCTURE";
		cout << "\n\n\t08. UPDATE INTEREST" ;
		cout << "\n\n\t09. EXIT";
		cout << "\n\n\tYour Choice (1-9) : ";
		cin >> ch;
		system("CLS");
		switch(ch)
		{
            case '1':
                ac.write_account();
                break;
            case '2':
                cout << "\n\nEnter account no. : "; cin >> num;
                ac.deposit_withdraw(num, 1);
                break;
            case '3':
                cout << "\n\nEnter account no. : "; cin >> num;
                ac.deposit_withdraw(num, 2);
                break;
            case '4':
                cout << "\n\nEnter account no. : "; cin >> num;
                rep.display_sp(num);
                break;
            case '5':
                rep.display_all();
                break;
            case '6':
                cout << "\nCLOSE ACCOUNT";
                cout << "\n=============";
                cout << "\n\nEnter account no. : "; cin>>num;
                ac.delete_account(num);
                break;
             case '7':
                inter.int_rate_struct();
                break;
             case '8':
                inter.calc_interest();
                cout << "\nInterest Updated Successfully !!!";
                break;
             case '9':
                cout << "\n\nThank you for using this banking system !!!";
                break;
             default: cout << "\a";
		}
		cin.ignore();
		cin.get();
	}while(ch != '9');
	return 0;
}


//*************************************************************
//    			      END OF PROGRAM
//*************************************************************

