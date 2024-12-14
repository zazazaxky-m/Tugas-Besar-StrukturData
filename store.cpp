#include "store.h"
#include <cstddef>
#include <iostream>
using namespace std;
// untuk fetch database
#include <fstream>

// untuk memecah string
#include <sstream>
#include <string>
#include <cstdlib>

#include <ctime> //untuk permasalahan perwaktuan

void insertParent(listTransaksi &L, addressTrx P, string position, addressTrx prec)
{
    if (position == "first")
    {
        if (L.first == NULL)
        {
            L.first = P;
            next(P) = NULL;
        }
        else
        {
            next(P) = L.first;
            L.first = P;
        }
    }
    else if (position == "last")
    {
        if (L.first == NULL)
        {
            L.first = P;
            next(P) = NULL;
        }
        else
        {
            addressTrx last = L.first;
            while (next(last) != NULL)
            {
                last = next(last);
            }
            next(last) = P;
            next(P) = NULL;
        }
    }
    else if (position == "after")
    {
        if (prec == NULL)
        {
            prec = getPrec(L, P);
        }
        if (prec != NULL)
        {
            next(P) = next(prec);
            next(prec) = P;
        }
        else
        {
            cout << "Error: Predecessor (prec) is NULL." << endl;
        }
    }
}


void showAllParent(listTransaksi L)
{
    addressTrx current = firstTransaksi(L); // buat sebuah address
    if (current == NULL)
    {
        cout << "Datanya masih kosong" << endl;
        return;
    }
    while (current != NULL)
    {
        printf("======================== Transaction %d =============\n", info(current).id);
        // cout << "Transaction ID : " << info(current).id << "\n";
        cout << "Name           : " << info(current).name << "\n";
        cout << "Date           : " << info(current).date << "\n";
        cout << "Bayar          : " << info(current).bayar << "\n";
        cout << "Metode         : " << info(current).metode << "\n";
        cout << "Diskon         : " << info(current).diskon << "\n";
        cout << "Status         : " << info(current).status << "\n";
        cout << "===========================================================\n";
        current = next(current);
    }
}

addressTrx deleteParent(listTransaksi &L, string position, addressTrx prec)
{
    if (position == "first")
    {
        if (firstTransaksi(L) == NULL)
        {
            return NULL;
        }
        else
        {
            addressTrx temp = firstTransaksi(L);
            firstTransaksi(L) = next(firstTransaksi(L));
            next(temp) = NULL;
            return temp;
        }
    }
    else if (position == "last")
    {
        if (firstTransaksi(L) == NULL)
        {
            return NULL;
        }
        else
        {
            addressTrx current = firstTransaksi(L);

            if (next(current) == NULL)
            {
                firstTransaksi(L) = NULL;
                return current;
            }

            addressTrx prevNode = getPrec(L, current);
            addressTrx lastNode = next(prevNode);
            next(prevNode) = NULL;
            return lastNode;
        }
    }
    else if (position == "after")
    {
        if (prec == NULL || next(prec) == NULL)
        {
            cout << "Error: Invalid predecessor or no node exists after the given node." << endl;
            return NULL;
        }

        addressTrx nodeToDelete = next(prec);
        next(prec) = next(nodeToDelete);
        next(nodeToDelete) = NULL;

        return nodeToDelete;
    }

    return NULL;
}


addressTrx searchParent(listTransaksi L, int x)
{
    addressTrx current = firstTransaksi(L);
    while (current != NULL)
    {
        if (info(current).id == x)
        {
            return current;
        }
        current = next(current);
    }
    return NULL;
}

void showSearchedParent(listTransaksi L, string x)
{
    bool found = 0;
    addressTrx current = firstTransaksi(L);
    cout << "------------------------------------" << endl;
    while (current != NULL)
    {
        string strId = to_string(info(current).id);
        if (strId == x || x == info(current).name)
        {
            found = 1;
            cout << "Transaction ID: " << info(current).id << endl;
            cout << "  Name: " << info(current).name << endl;
            cout << "  Date: " << info(current).date << endl;
            cout << "  Bayar: " << info(current).bayar << endl;
            cout << "  Metode: " << info(current).metode << endl;
            cout << "  Diskon: " << info(current).diskon << endl;
            cout << "  Status: " << info(current).status << endl;
            cout << "------------------------------------" << endl;
        }
        current = next(current);
    };
    if (!found)
    {
        cout << "Data tidak ditemukan" << endl;
    }
}

addressTrx createElmTransaksi(infotypeTransaksi x)
{
    addressTrx P = new elmTransaksi;
    info(P).id = x.id;
    info(P).name = x.name;
    info(P).date = x.date;
    info(P).bayar = x.bayar;
    info(P).metode = x.metode;
    info(P).diskon = x.diskon;
    info(P).status = x.status;
    P->next = NULL; // Inisialisasi next ke NULL
    return P;
}

addressDetail createElmDetailTransaksi(infotypeDetailTransaksi x)
{
    addressDetail P = new elmDetailTransaksi;
    info(P).idProduk = x.idProduk;
    info(P).namaProduk = x.namaProduk;
    info(P).harga = x.harga;
    info(P).quantity = x.quantity;
    P->next = NULL; // Inisialisasi next ke NULL
    P->prev = NULL; // Inisialisasi prev ke NULL
    return P;
}

void insertChild(addressTrx L, addressDetail P, string position, addressDetail prec)
{
    if (position == "first")
    {
        if (firstDetail(L) == NULL)
        {
            firstDetail(L) = P;
            prev(P) = NULL;
            next(P) = NULL;
        }
        else
        {
            next(P) = firstDetail(L);
            prev(P) = NULL;
            prev(firstDetail(L)) = P;
            firstDetail(L) = P;
        }
    }
    else if (position == "last")
    {
        if (firstDetail(L) == NULL)
        {
            firstDetail(L) = P;
            prev(P) = NULL;
            next(P) = NULL;
        }
        else
        {
            addressDetail last = firstDetail(L);
            while (next(last) != NULL)
            {
                last = next(last);
            }
            next(last) = P;
            prev(P) = last;
            next(P) = NULL;
        }
    }
    else if (position == "after")
    {
        if (prec != NULL)
        {
            next(P) = next(prec); // Hubungkan P ke elemen setelah prec
            prev(P) = prec;       // Hubungkan P ke prec
            if (next(prec) != NULL)
            {
                prev(next(prec)) = P; // Hubungkan elemen setelah prec ke P
            }
            next(prec) = P; // Hubungkan prec ke P
        }
        else
        {
            cout << "Error: Predecessor (prec) is NULL." << endl;
        }
    }
}


void connectParentToChild(addressTrx parent, addressDetail child)
{
    if (firstDetail(parent) == NULL)
    {
        firstDetail(parent) = child;
        prev(child) = NULL; // No previous child
    }
    else
    {
        addressDetail last = firstDetail(parent);
        while (next(last) != NULL)
        {
            last = next(last);
        }
        next(last) = child;
        prev(child) = last; // Connect the new child
    }
}

void showAllParentChild(listTransaksi L)
{
    addressTrx current = firstTransaksi(L); // Use the new macro
    while (current != NULL)
    {
        cout << "==========================================" << endl;
        cout << "            TRANSACTION DETAILS           " << endl;
        cout << "==========================================" << endl;
        cout << "Transaction ID : " << info(current).id << endl;
        cout << "Name           : " << info(current).name << endl;
        cout << "Date           : " << info(current).date << endl;
        cout << "Amount Paid    : " << info(current).bayar << endl;
        cout << "Payment Method : " << info(current).metode << endl;
        cout << "Discount       : " << info(current).diskon << endl;
        cout << "Status         : " << info(current).status << endl;
        if (firstDetail(current) != NULL)
        {
            cout << "\n  ----------- Product Details -----------" << endl;
        }
        addressDetail detailCurrent = firstDetail(current);
        while (detailCurrent != NULL)
        {
            cout << "  Product ID   : " << info(detailCurrent).idProduk << endl;
            cout << "  Name         : " << info(detailCurrent).namaProduk << endl;
            cout << "  Price        : " << info(detailCurrent).harga << endl;
            cout << "  Quantity     : " << info(detailCurrent).quantity << endl;
            cout << "  ----------------------------------------" << endl;
            detailCurrent = next(detailCurrent);
        }
        cout << "==========================================\n" << endl;

        current = next(current);
    }
}

addressDetail searchChildOnParent(listTransaksi L, int x)
{
    addressTrx current = firstTransaksi(L);
    while (current != NULL)
    {
        addressDetail detailCurrent = firstDetail(current);
        while (detailCurrent != NULL)
        {
            if (info(detailCurrent).idProduk == x)
            {
                return detailCurrent;
            }
            detailCurrent = next(detailCurrent);
        }
        current = next(current);
    }
    return NULL;
}

addressDetail deleteChildOnParent(addressTrx parent, string position, addressDetail prec)
{
    addressDetail childDelete = nullptr;

    if (position == "first")
    {
        childDelete = firstDetail(parent);
        if (childDelete != NULL)
        {
            firstDetail(parent) = next(childDelete);
            if (firstDetail(parent) != NULL)
            {
                prev(firstDetail(parent)) = NULL;
            }
            else
            {
                lastDetail(parent) = NULL;
            }
            next(childDelete) = NULL;
        }
    }
    else if (position == "last")
    {
        childDelete = lastDetail(parent);
        if (childDelete != NULL)
        {
            if (prev(childDelete) != NULL)
            {
                next(prev(childDelete)) = NULL;
            }
            else
            {
                firstDetail(parent) = NULL;
            }
            lastDetail(parent) = prev(childDelete);
            next(childDelete) = NULL;
        }
    }
    else if (position == "after")
    {
        if (prec != NULL)
        {
            childDelete = next(prec);
            if (childDelete != NULL)
            {
                next(prec) = next(childDelete);
                if (next(childDelete) != NULL)
                {
                    prev(next(childDelete)) = prec;
                }
                else
                {
                    lastDetail(parent) = prec;
                }
                next(childDelete) = NULL;
                prev(childDelete) = NULL;
            }
        }
        else
        {
            cout << "Error: Predecessor (prec) is NULL for position 'after'." << endl;
        }
    }

    return childDelete;
}


void countChildOnParent(addressTrx parent)
{
    int childCount = 0;
    addressDetail current = firstDetail(parent);
    while (current != NULL)
    {
        childCount++;
        current = next(current);
    }
    cout << "Total Product : " << childCount << endl;
}

void saveToDatabase(listTransaksi L, string filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    addressTrx currentTransaction = firstTransaksi(L);
    while (currentTransaction != NULL)
    {
        file << info(currentTransaction).id << endl;
        file << info(currentTransaction).name << endl;
        file << info(currentTransaction).date << endl;
        file << info(currentTransaction).bayar << endl;
        file << info(currentTransaction).metode << endl;
        file << info(currentTransaction).diskon << endl;
        file << info(currentTransaction).status << endl;

        addressDetail currentDetail = firstDetail(currentTransaction);
        while (currentDetail != NULL)
        {
            file << info(currentDetail).idProduk << "," << info(currentDetail).namaProduk << ","
                 << info(currentDetail).harga << "," << info(currentDetail).quantity << endl;
            currentDetail = next(currentDetail);
        }
        file << "END_TRANSACTION" << endl;
        currentTransaction = next(currentTransaction);
    }

    file.close();
}

void loadFromDatabase(listTransaksi &L, string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }

    string line;
    infotypeTransaksi trxData;
    infotypeDetailTransaksi detailData;

    addressTrx currentTransaction = NULL;
    while (getline(file, line))
    {
        if (line == "END_TRANSACTION")
        {
            if (currentTransaction != NULL)
            {
                insertParent(L, currentTransaction, "last", NULL);
                currentTransaction = NULL;
            }
            continue;
        }

        if (currentTransaction == NULL)
        {
            trxData.id = stoi(line);
            getline(file, trxData.name);
            getline(file, trxData.date);
            getline(file, line);
            trxData.bayar = stoi(line);
            getline(file, trxData.metode);
            getline(file, line);
            trxData.diskon = stoi(line);
            getline(file, trxData.status);

            currentTransaction = createElmTransaksi(trxData);
        }
        else
        {
            stringstream ss(line);
            string temp;

            getline(ss, temp, ',');
            detailData.idProduk = stoi(temp);
            getline(ss, detailData.namaProduk, ',');
            getline(ss, temp, ',');
            detailData.harga = stoi(temp);
            getline(ss, temp);
            detailData.quantity = stoi(temp);

            addressDetail newDetail = createElmDetailTransaksi(detailData);
            connectParentToChild(currentTransaction, newDetail);
        }
    }

    if (currentTransaction != NULL)
    {
        insertParent(L, currentTransaction, "last", NULL);
    }

    file.close();
}


void pressEnter()
{
    string gakGuna;
    cout << "Press Enter to continue...";
    cin.ignore();
    getline(cin, gakGuna);
}

int getLastIdTransaction(listTransaksi L)
{
    addressTrx currentTransaction = firstTransaksi(L);
    int lastId = 0;
    while (currentTransaction != NULL)
    {
        if (info(currentTransaction).id > lastId)
        {
            lastId = info(currentTransaction).id;
        }
        currentTransaction = next(currentTransaction);
    }
    return lastId;
}

   addressTrx inputTransaction(listTransaksi &L)
   {
       infotypeTransaksi x;
       addressTrx P;
       cout << "====================================" << endl;
       cout << "|        Insert Transaction        |" << endl;
       cout << "====================================" << endl;
       x.id = getLastIdTransaction(L) + 1;

       cout << "Enter Buyer Name: ";
       cin.ignore();
       getline(cin, x.name);

       // Get current date and time
       time_t now = time(0);
       tm *ltm = localtime(&now);
       char buffer[20];
       strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
       x.date = buffer;

       x.bayar = 0;
       cout << "Enter Payment Method: ";
       cin.ignore();
       getline(cin, x.metode);

       cout << "Enter Discount: ";
       cin >> x.diskon;

       cout << "Enter Status: ";
       cin >> x.status;

       P = createElmTransaksi(x);
       return P;
   }


addressTrx getPrec(listTransaksi L, addressTrx P)
{
    if (firstTransaksi(L) == NULL || P == NULL)
    {
        return NULL; // List kosong atau elemen P tidak valid
    }

    addressTrx current = firstTransaksi(L);
    while (current != NULL && next(current) != P)
    {
        current = next(current);
    }

    if (next(current) == P)
    {
        return current;
    }
    else
    {
        return NULL;
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
