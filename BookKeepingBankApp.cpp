/****************************
NAME        -P.M.N.R.Pussadeniya
INDEX NO    -19/ENG/082
COURSE		-PROGRAMMING QUEST
COURSE CODE -CO2210
DATE        -09/28/2021
QUEST NO    -03
*****************************/


#include <iostream>
#include<fstream>
#include <sstream>
#include<vector>
#include<math.h>
#include<iomanip>
#include<algorithm> //To import find function

using namespace std;




int main()
{

    //UI
    cout<<endl;
    cout<<endl;
    cout<<"----------------------------URBAN DEVELOPMENT BANK----------------------------  "<<endl;
    cout<<"---------------------------Book Keeping Application---------------------------  "<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"    User Pannel  "<<endl;

    //Initializing Vectors
    vector<string>accNumbers;
    vector<double>accBals;
    vector<string>dates;
    
    accNumbers.clear();
    accBals.clear();
    dates.clear();

    //File read, write hanadlers
    ifstream accs;
    ofstream file;
    
    //Initializing variables
    string line;
    string date, bl, accNum, type,amount;
    double  initBals = 0;
    char input;
    bool flag = true;
    double charge = 0.00, accVal = 0.00;
    
    //reading the text file which has balance values
    accs.open("balance.txt");
    if (!accs) {
        cout << "No File named blanace.txt" << endl;
    }

    //Initializing a text file to record bank account
    ofstream BankAccount("BankAccount.txt");
   
    while (getline(accs, line)) {
        charge = 0;
        stringstream ss(line);
        getline(ss, date, ',');
        getline(ss, accNum, ',');
        getline(ss, bl, ',');
        accVal = stod(bl);//convert string to double
        accNumbers.push_back(accNum);
        dates.push_back(date);
        accNum = accNum + ".txt";
        file.open(accNum.c_str());
        file << date << "," << "0,0," << bl;
        accBals.push_back(accVal);
        initBals += accVal;
        file.close(); 
    }

    //Saving details of Bank Account
    BankAccount << date << "," << "0,0," << fixed << setprecision(2) << initBals;//write to bank account starting balance by adding all user account balances
    
    //Adding interest
    for (int i = 0; i < accNumbers.size(); i++) {
        file.open((accNumbers[i] + ".txt").c_str(), ios::app);//add first intrest to account by checking eligibilty
        if (accBals[i] > 0) {
            charge = round(accBals[i] * 0.0002 * 100.0) / 100.0;
            accBals[i] += charge;
            file << "\n" << date << "," << "3," << fixed << setprecision(2) << charge << "," << accBals[i];
            initBals -= charge;
            BankAccount << "\n" << date << "," << 3 << "," << fixed << setprecision(2) << charge << "," << initBals;

        }
        file.close();
    }
    accs.close();

  


    string startingdate = dates[0];
    //reading transactions from the text file 
    accs.open("transaction.txt");
    if (!accs) {
        cout << "No File named transaction.txt" << endl;
    }

    while (getline(accs, line)) {
        stringstream ss(line);
        getline(ss, date, ',');
        getline(ss, accNum, ',');
        getline(ss, type, ',');
        getline(ss, bl, ',');
        string accNum2 = accNum;
        accVal = stod(bl);
        accNum = accNum + ".txt";

        //Adding daily changes of transactions and interests
        if (startingdate != date) {
            int day1, day2;
            day1 = stoi(startingdate);
            day2 = stoi(date);
            for (int j = 0; j < day2 - day1; j++) {
                for (int i = 0; i < accNumbers.size(); i++) {
                    file.open(accNumbers[i] + ".txt", ios::app);
                    
                    //Charging Rs 50/= if the bank account balance is less than 0
                    if (accBals[i] < 0) {
                        charge = 50.00;
                        accBals[i] -= charge;
                        file << "\n" << day1 + j << "," << 4 << "," << fixed << setprecision(2) << charge << "," << accBals[i];
                        initBals += 50.00;
                        BankAccount << "\n" << day1 + j << "," << 4 << "," << fixed << setprecision(2) << charge << "," << initBals;
                    }

                    //Charging Rs 10/= if the bank account balance is less than 1000
                    else if (accBals[i] < 1000) {
                        charge = 10.00;
                        accBals[i] -= charge;
                        file << "\n" << day1 + j << "," << 4 << "," << fixed << setprecision(2) << charge << "," << accBals[i];

                        initBals += 10.00;
                        BankAccount << "\n" << day1 + j << "," << 4 << "," << fixed << setprecision(2) << charge << "," << initBals;
                    }
                    file.close();
                }

                //Adding interest of 0.02%
                for (int i = 0; i < accNumbers.size(); i++) {
                    file.open(accNumbers[i] + ".txt", ios::app);
                    if (accBals[i] > 0) {
                        charge = round(accBals[i] * 0.0002 * 100.0) / 100.0;
                        accBals[i] += charge;
                        file << "\n" << day1 + j + 1 << "," << 3 << "," << fixed << setprecision(2) << charge << "," << accBals[i];

                        initBals -= charge;
                        BankAccount << "\n" << day1 + j + 1 << "," << 3 << "," << fixed << setprecision(2) << charge << "," << initBals;

                    }
                    file.close();
                }
            }
        }
        startingdate = date;

        file.open(accNum.c_str(), ios::app);
        auto it = find(accNumbers.begin(), accNumbers.end(), accNum2);

        int i = 0;
        if (it != accNumbers.end())
        {
            i = it - accNumbers.begin();
        }

        //Checking the type of transaction  1==deposit, 2==withdraw
        if (type == "1") {
            accBals[i] += accVal;
            initBals += accVal;
        }
        else if (type == "2") {
            accBals[i] -= accVal;
            initBals -= accVal;
        }

        //Write on text file
        file << "\n" << date << "," << type << "," << fixed << setprecision(2) << accVal << "," << accBals[i];//record in txt files
        BankAccount << "\n" << date << "," << type << "," << fixed << setprecision(2) << accVal << "," << initBals;
        file.close();
    }

    //Adding charges of last day. 
    //No need to add interest here since it is added previously
    for (int i = 0; i < accNumbers.size(); i++) {
        file.open(accNumbers[i] + ".txt", ios::app);
        //Charging Rs 50/= if the bank account balance is less than 0
        if (accBals[i] < 0) {
            charge = 50.00;
            accBals[i] -= charge;
            file << "\n" << date << "," << 4 << "," << fixed << setprecision(2) << charge << "," << accBals[i];

            initBals += 50.00;
            BankAccount << "\n" << date << "," << 4 << "," << fixed << setprecision(2) << charge << "," << initBals;
        }
        //Charging Rs 10/= if the bank account balance is less than 1000
        else if (accBals[i] < 1000) {
            charge = 10.00;
            accBals[i] -= charge;
            file << "\n" << date << "," << 4 << "," << fixed << setprecision(2) << charge << "," << accBals[i];

            initBals += 10.00;
            BankAccount << "\n" << date << "," << 4 << "," << fixed << setprecision(2) << charge << "," << initBals;
        }
        file.close();
    }
    accs.close();
    BankAccount.close();

   
    
    while (flag) {
    //Main UI and taking inputs    
    cout << "\n1-Bank Balance \n2-Check user account \n3-Starting and Ending balance of the day\n4-Exit";
    cout << "\n\nEnter your choice :";

        cin >> input;

        switch (input) {
        //Taking bank total balance
        case '1': {
            cout << "\nTotal Bank Balance :";
            cout << "Rs " << fixed << setprecision(2) << initBals << endl;

            break;
        }

        //Taking total balance of all users
        case '2': {
            cout << "\nAccount details of all users " << endl;
            for (int i = 0; i < accNumbers.size(); i++) {
                cout <<"    "<< accNumbers[i] << " : Rs " << fixed << setprecision(2) << accBals[i] << endl;
            }
            break;
        }

        //Taking startin and ending balance of the day
        case '3': {

            string accNo, inputDate;
            vector<string>output;

            //Taking user inputs
            cout << "\nEnter User Account number :";
            cin >> accNo;
            cout << "\nDate (in format of YYYYMMDD) :";
            cin >> inputDate;

            double initValue = 0;
            output.clear();
            accs.open((accNo + ".txt").c_str());

            //Error msg if the account is not found
            if (!accs) {
                cout << "Account does not exist!" << endl;
            }
            //If found then write the text file
            else {
              
                while (getline(accs, line)) {
                    stringstream ss(line);
                    getline(ss, date, ',');
                    getline(ss, type, ',');
                    getline(ss, amount, ',');
                    getline(ss, bl);
                    if (date == inputDate) {
                        output.push_back(bl);
                        
                        if (output.size() == 1) {
                            initValue = stod(output[0]);
                            if (type == "3") {
                                initValue = stod(output[0]) - stod(amount);
                            }
                            else if (type == "4") {
                                initValue= stod(output[0]) + stod(amount);
                            }
                            
                            
                           
                        }
                    }
                }
                accs.close();

                if (output.size() == 0) {
                    cout << "No transactions has recorded on this period!" << endl;//if no transaction recorded in that period  output this
                }
                else {

                    cout << "\n\nStarting Balance of the day : Rs " << fixed << setprecision(2) << initValue << endl;
                    cout << "Ending Balance of the day: Rs " << output[output.size() - 1] << endl;
                }
            }

            break;
        
        }
        //Exit
            case '4': {
                flag = false;
                break;
            }
            
            default: {
                cout << "Please Check input" << endl;
                break;
            }
    }
    }
    return 0;
}


