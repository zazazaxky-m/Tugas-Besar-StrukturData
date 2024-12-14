#include "store.h"
#include <cstddef>
#include <cstdlib> // clear screen
#include <ctime>   // Include for time functions
#include <iostream>
#include <string> // Include for std::stoi

using namespace std;

void printMenu()
{
    cout << "=================================================" << endl;
    cout << "|                   MAIN MENU                   |" << endl;
    cout << "=================================================" << endl;
    cout << "|                   PARENT MENU                 |" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "| 1. Insert Parent Transaction                  |" << endl;
    cout << "| 2. Show All Parent Transactions               |" << endl;
    cout << "| 3. Delete Parent                              |" << endl;
    cout << "| 4. Search for a Parent Transaction            |" << endl;
    cout << "=================================================" << endl;
    cout << "|                   CHILD MENU                  |" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "| 5. Insert Detail Transaction                  |" << endl;
    cout << "| 6. Show All Parent Transactions with Children |" << endl;
    cout << "| 7. Search for a Child in a Specific Parent    |" << endl;
    cout << "| 8. Delete Detail Transaction                  |" << endl;
    cout << "| 9. Count Number of Children in a Parent       |" << endl;
    cout << "=================================================" << endl;
    cout << "| 10. Exit                                      |" << endl;
    cout << "=================================================" << endl;
}


int main()
{
    listTransaksi L;
    L.first = NULL;

    // Load from database
    cout << "Load From Database (Sabar...)" << endl;
    clock_t start = clock();
    while (clock() - start < 3000)
        ;
    loadFromDatabase(L, "database.txt");

    int choice;
    do
    {
        printMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int totalPrice = 0;
            addressTrx P = inputTransaction(L);
            insertParent(L, P, "first", NULL);

            infotypeDetailTransaksi detail;
            detail.idProduk = -1;
            while (detail.idProduk != 0)
            {
                if(detail.idProduk != 0 && detail.idProduk != -1){
                    cout << "Enter Product ID (0 for end) : ";
                    cin >> detail.idProduk;
                }else if (detail.idProduk == -1){
                    cout << "Enter Product ID : ";
                    cin >> detail.idProduk;
                }else if(detail.idProduk == 0){
                    break;
                }
                cout << "Enter Product Name: ";
                cin.ignore();
                getline(cin, detail.namaProduk);
                cout << "Enter Price: ";
                cin >> detail.harga;
                cout << "Enter Quantity: ";
                cin >> detail.quantity;
                totalPrice = totalPrice + (detail.harga * detail.quantity);
                addressDetail newDetail = createElmDetailTransaksi(detail);
                if (P->firstDetail == NULL)
                {
                    connectParentToChild(P, newDetail);
                }
                else
                {
                    insertChild(P, newDetail, "last", NULL);
                }
            }

            cout << "Product berhasil ditambahkan." << endl;
            pressEnter();
            saveToDatabase(L, "database.txt");

            pressEnter();
            clearScreen();
            break;
        }

        case 2:
        {
            showAllParent(L);
            pressEnter();
            clearScreen();
            break;
        }

        case 3:
        {
            int choicee;
            cout << "===============================" << endl;
            cout << "|        Delete Parent        |" << endl;
            cout << "===============================" << endl;
            cout << "| 1. Delete First Parent      |" << endl;
            cout << "| 2. Delete Last Parent       |" << endl;
            cout << "| 3. Delete Specific Parent   |" << endl;
            cout << "===============================" << endl;
            cout << "Masukkan Pilihan : ";
            cin >> choicee;
            switch (choicee)
            {
            case 1:
            {
                while (firstTransaksi(L) && firstTransaksi(L)->firstDetail != NULL)
                {
                    deleteChildOnParent(firstTransaksi(L), "first", NULL);
                }
                deleteParent(L, "first", NULL);
                break;
            }

            case 2:
            {
                while (firstTransaksi(L) && firstTransaksi(L)->lastDetail != NULL)
                {
                    deleteChildOnParent(firstTransaksi(L), "last", NULL);
                }
                deleteParent(L, "first", NULL);
                break;
            }

            case 3:
            {
                int idSearch;
                cout << "Masukkan Id Transaksi : ";
                cin >> idSearch;
                addressTrx search = searchParent(L, idSearch);
                if (search == NULL)
                {
                    cout << "ID not found." << endl;
                    break;
                }
                deleteParent(L, "first", search);
                break;
            }

            default:
                cout << "Invalid choice for delete option." << endl;
            }
            saveToDatabase(L, "database.txt");
            pressEnter();
            clearScreen();
            break;
        }

        case 4:
        {
            string prefix;
            cout << "Masukkan ID Transaksi / Nama User : ";
            cin >> prefix;
            showSearchedParent(L, prefix);
            pressEnter();
            clearScreen();
            break;
        }

        case 5:
        {
            if (firstTransaksi(L) == NULL)
            {
                cout << "No transactions available to add products." << endl;
                break;
            }
            int idSearch;

            cout << "Masukkan Id Transaksi : ";
            cin >> idSearch;

            addressTrx transaksi = searchParent(L, idSearch);
            if (transaksi == NULL)
            {
                cout << "ID not found." << endl;
                break;
            }
            infotypeDetailTransaksi detail;
            cout << "Enter Product ID: ";
            cin >> detail.idProduk;
            cout << "Enter Product Name: ";
            cin >> detail.namaProduk;
            cout << "Enter Price: ";
            cin >> detail.harga;
            cout << "Enter Quantity: ";
            cin >> detail.quantity;
            info(transaksi).bayar = info(transaksi).bayar + (detail.harga * detail.quantity);
            addressDetail newDetail = createElmDetailTransaksi(detail);
            if (transaksi->firstDetail == NULL)
            {
                connectParentToChild(transaksi, newDetail);
            }
            else
            {
                insertChild(transaksi, newDetail, "last", NULL);
            }
            cout << "Product berhasil ditambahkan." << endl;
            pressEnter();
            saveToDatabase(L, "database.txt");
            clearScreen();
            break;
        }

        case 6:
        {
            showAllParentChild(L);
            pressEnter();
            clearScreen();
            break;
        }

        case 7:
        {
            int choiceee;
            cout << "Enter Transaction Id to search: ";
            cin >> choiceee;
            searchParent(L, choiceee);
            int child;
            cout << "Enter Product ID to search in a specific transaction: ";
            cin >> child;

            addressDetail foundDetail = searchChildOnParent(L, child);
            if (foundDetail != NULL)
            {
                cout << "Product found !" << endl;
                cout << "Product ID: " << info(foundDetail).idProduk << endl;
                cout << "Product Name: " << info(foundDetail).namaProduk << endl;
                cout << "Product Price: " << info(foundDetail).harga << endl;
                cout << "Product Quantity: " << info(foundDetail).quantity << endl;
            }
            else
            {
                cout << "Product not found in any transaction." << endl;
            }
            pressEnter();
            clearScreen();
            break;
        }

        case 8:
        {
            showAllParent(L);
            int choicee;
            cout << "===============================" << endl;
            cout << "|  Delete Detail Transaction  |" << endl;
            cout << "===============================" << endl;
            cout << "| 1. Delete First Detail      |" << endl;
            cout << "| 2. Delete Last Detail       |" << endl;
            cout << "| 3. Delete Specific Detail   |" << endl;
            cout << "===============================" << endl;
            cout << "Masukkan Pilihan : ";
            cin >> choicee;

            switch (choicee)
            {
            case 1:
            {
                int x;
                cout << "Enter Transaction ID : ";
                cin >> x;
                addressTrx search = searchParent(L, x);
                if (search == NULL)
                {
                    cout << "Transaction not found." << endl;
                }
                addressDetail delDatail = deleteChildOnParent(search, "first", NULL);
                cout << info(delDatail).namaProduk << " deleted." << endl;
                info(search).bayar = info(search).bayar - (info(delDatail).harga * info(delDatail).quantity);
                break;
            }

            case 2:
            {
                int x;
                cout << "Enter Transaction ID : ";
                cin >> x;
                addressTrx search = searchParent(L, x);
                if (search == NULL)
                {
                    cout << "Transaction not found." << endl;
                }
                addressDetail delDatail = deleteChildOnParent(search, "last", NULL);
                cout << info(delDatail).namaProduk << " deleted." << endl;
                info(search).bayar = info(search).bayar - (info(delDatail).harga * info(delDatail).quantity);
                break;
            }

            case 3:
            {
                int x;
                cout << "Enter Transaction ID : ";
                cin >> x;
                addressTrx search = searchParent(L, x);
                if (search == NULL)
                {
                    cout << "Transaction not found." << endl;
                }
                addressDetail delDatail = deleteChildOnParent(search, "after", NULL);
                cout << info(delDatail).namaProduk << " deleted." << endl;
                info(search).bayar = info(search).bayar - (info(delDatail).harga * info(delDatail).quantity);
                break;
            }

            default:
                cout << "Invalid choice for delete detail option." << endl;
            }
            pressEnter();
            saveToDatabase(L, "database.txt");
            clearScreen();
            break;
        }

        case 9:
        {
            if (firstTransaksi(L) == NULL)
            {
                cout << "No transactions available." << endl;
                break;
            }
            int x;
            cout << "Enter Transaction ID : ";
            cin >> x;
            addressTrx search = searchParent(L, x);
            if (search == NULL)
            {
                cout << "Transaction not found." << endl;
                break;
            }
            else
            {
                countChildOnParent(search);
            }
            pressEnter();
            clearScreen();
            break;
        }

        case 10:
        {
            cout << "Exiting program." << endl;
            pressEnter();
            clearScreen();
            break;
        }

        default:
            cout << "Invalid choice. Please try again." << endl;
            pressEnter();
        }
    } while (choice != 10);

    return 0;
}
