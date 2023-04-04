// IMS :Inventory Management System
#include <iostream>
#include <vector>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;
#define LL long long
#define INVENTORY "inventory.csv"
#define SALES "sales.csv"
typedef struct Item
{
    LL id;
    string name;
    LL Quantity;
    LL SP;
    LL BP;

    Item(LL i, string n, LL qty, LL sp, LL bp)
    {
        id = i;
        name = n;
        Quantity = qty;
        SP = sp;
        BP = bp;
    }
} item;

class Utility
{
public:
    string handleString(string s)
    {
        string new_s = "";
        for (auto i : s)
        {
            new_s += toupper(i);
        }
        return new_s;
    }
};

class Inventory : Utility
{
private:
    vector<Item> Items;

    void _addItem()
    {
        LL i, q, s, b;
        string name, n;
        cout << "Enter Item Name : ";
        cin >> n;
        name = handleString(n);
        if (checkByName(name))
        {
            cout << "Enter Item Quantity : ";
            cin >> q;
            for (int i = 0; i < Items.size(); i++)
            {
                if (Items[i].name == name)
                {
                    Items[i].Quantity += q;
                }
            }
        }
        else
        {
            cout << "Enter Item Quantity : ";
            cin >> q;
            cout << "Enter Selling Price : ";
            cin >> s;
            cout << "Enter Buying Price : ";
            cin >> b;
            cout << endl;
            Item item((LL)1000 + Items.size(), name, q, s, b);
            Items.push_back(item);
            cout << "Succesfully Added" << endl;
        }
    }
    bool checkByName(string q)
    {
        for (int i = 0; i < Items.size(); i++)
        {
            if (Items[i].name == q)
            {
                return true;
            }
        }
        return false;
    }
    void loadData(const string filename)
    {
        fstream file;
        int colname = true;
        vector<string> row;
        string line, word;

        file.open(filename);
        if (!file.is_open())
        {
            cerr << "!! Cannot Load Data from File !!" << endl;
            exit(0);
        }
        while (getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ','))
            {
                row.push_back(word);
            }
            if (!colname)
            {
                LL id = (LL)stoi(row[0]);
                string name = row[1];
                LL Quantity = (LL)stoi(row[2]);
                LL SP = (LL)stoi(row[3]);
                LL BP = (LL)stoi(row[4]);
                Item temp(id, name, Quantity, SP, BP);
                Items.push_back(temp);
            }
            colname = false;
        }
    }

    void writeData(const string filename)
    {
        fstream ofs;
        ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
        ofs.open(filename, std::fstream::out);
        if (!ofs.is_open())
        {
            cerr << "Writing cannot performed !!";
            exit(0);
        }
        ofs << "id,name,qty,sellprice,buyprice" << endl;
        for (int i = 0; i < Items.size(); i++)
        {
            ofs << Items[i].id << ','
                << Items[i].name << ','
                << Items[i].Quantity << ','
                << Items[i].SP << ','
                << Items[i].BP << endl;
        }
        ofs.close();
    }

public:
    Inventory()
    {
        loadData(INVENTORY);
    }
    void closeSystem()
    {
        writeData(INVENTORY);
        cout << "-| Thankyou for using IMS |-" << endl;
    }
    void addItem()
    {
        _addItem();
    }
    LL getQty(LL id)
    {
        for (auto i : Items)
        {
            if (i.id == id)
            {
                return i.Quantity;
            }
        }
        return 0;
    }
    void printInventory()
    {
        cout << "Inventory List" << endl;
        cout << "----------------------------------------------" << endl;
        cout << left << setw(5) << "ID"
             << left << setw(15) << "Name"
             << left << setw(10) << "Quantity"
             << left << setw(5) << "SP"
             << left << setw(5) << "BP"
             << endl;
        for (int i = 0; i < Items.size(); i++)
        {
            cout << left << setw(5) << Items[i].id
                 << left << setw(15) << Items[i].name
                 << left << setw(10) << Items[i].Quantity
                 << left << setw(5) << Items[i].SP
                 << left << setw(5) << Items[i].BP
                 << endl;
        }
    }
    Item getItem(LL id)
    {
        for (auto i : Items)
        {
            if (i.id == id)
            {
                return i;
            }
        }
    }
    int getItemIndex(LL id)
    {
        for (int i = 0; i < Items.size(); i++)
        {
            return i;
        }
        return -1;
    }
    void updateItemQty(int index, int updated_value)
    {
        cout << "UPDATED" << endl;
        Items[index].Quantity += updated_value;
    }
};

class Customer : Inventory
{
    vector<LL> Bill;
    int itemBought = 0;

    bool alreadyExist(LL id)
    {
        for (auto i : Bill)
        {
            if (i == id)
            {
                return true;
            }
        }
        return false;
    }
    void addToBill(LL id)
    {
        int index;
        if (getQty(id))
        {
            index = getItemIndex(id);
            Bill.push_back(id);
            updateItemQty(index, -1);
            itemBought++;
        }
        else
        {
            cout << "<-<-<- Out of STOCK ->->->" << endl;
        }
    }
    void removeFromBill(LL id)
    {
        for (auto i : Bill)
        {
            if (i == id)
            {
                int index = getItemIndex(id);
                updateItemQty(index, 1);
                i = -1;
            }
        }
    }

    void printBill()
    {
        fstream billfile, salesfile;
        string filename = customername + ".txt";
        time_t now = time(0);
        char *dt = ctime(&now);
        int start = 0, profit = 0, totalAmount = 0;
        billfile.open(filename, std::ios::out);
        if (!billfile.is_open())
        {
            cout << "Error in generating Bill";
            exit(0);
        }
        billfile << "customer name : " << customername << "\n"
                 << "time :" << dt << "\n"
                 << "total items :" << itemBought << "\n";
        sort(Bill.begin(), Bill.end());

        for (int i = 0; i < Bill.size(); i++)
        {
            if (Bill[i] != -1)
            {
                Item billItem = getItem(Bill[i]);
                billfile << billItem.name << "\t"
                         << billItem.SP << "₹" << endl;
                totalAmount += billItem.SP;
                profit += billItem.SP - billItem.BP;
            }
        }
        billfile << "\n\tTotal Amount : " << totalAmount << " ₹" << endl;
        billfile.close();
        salesfile.open(SALES, fstream::app);
        if (!salesfile.is_open())
        {
            cerr << "Error is sales file" << endl;
            exit(0);
        }
        salesfile << customername << ", " << profit << "\n";
        salesfile.close();
        closeSystem();
    }

public:
    string customername;
    void createBill()
    {
        int opr;
        LL id;
        bool run = true;
        cout << "Enter customer name :";
        cin >> customername;
        while (run)
        {
            cout << endl;
            cout << "Operations:\n\n\tadd item in bill:1\n\tRemove item from bill:2\n\tPrint Bill:3" << endl;
            cout << "Enter operation on Bill ->";
            cin >> opr;
            switch (opr)
            {
            case 1:
                cout << "Enter id for element to enter :";
                cin >> id;
                addToBill(id);
                break;
            case 2:
                cout << "Enter id for element to remove :";
                cin >> id;
                removeFromBill(id);
                break;
            case 3:
                printBill();
            default:
                run = false;
            }
        }
    }
};
int main()
{
    Inventory i;
    Customer c;
    int input;
    enum Operations
    {
        ADD,
        PRINT,
        CREATE_BILL,
        EXIT,
        TEMP
    };
    bool run = true;

    do
    {
    REPEAT:
        cout << "operations->" << endl;
        cout << "Add to inventory:\t0" << endl;
        cout << "Display inventory:\t1" << endl;
        cout << "Create Bill:\t\t2" << endl;
        cout << "Exit IMS:\t\t3" << endl;
        cout << "|--> Enter Operation number :";
        cin >> input;
        system("CLS");
        if (input < -1 || input >= 3)
        {
            cout << " Invalid Operation !" << endl;
            goto REPEAT;
        }
        Operations opr = static_cast<Operations>(input);
        switch (opr)
        {
        case ADD:
            i.addItem();
            break;
        case PRINT:
            i.printInventory();
            break;
        case CREATE_BILL:
            c.createBill();
            break;
        case EXIT:
            i.closeSystem();
            run = false;

        case TEMP:
            cout << "IMS is devloped by SPARSH" << endl;
        }
    } while (run);

    return 0;
}