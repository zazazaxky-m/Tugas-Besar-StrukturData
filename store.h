#ifndef STORE_H_INCLUDED
#define STORE_H_INCLUDED

#include <iostream>
using namespace std;

// Define macros for accessing fields in the structures
#define first(P) (P)->first
#define info(P) (P)->info
#define next(P) (P)->next
#define prev(P) (P)->prev

// Define macros for accessing specific members of listTransaksi and elmTransaksi
#define firstTransaksi(L) (L).first
#define firstDetail(P) (P)->firstDetail
#define lastDetail(P) (P)->lastDetail

typedef struct elmTransaksi *addressTrx;
typedef struct elmDetailTransaksi *addressDetail;

struct infotypeTransaksi {
    int id;
    string name;
    string date;
    int bayar;
    string metode;
    int diskon;
    string status;
};

struct infotypeDetailTransaksi {
    int idProduk;
    string namaProduk;
    int harga;
    int quantity;
};

struct listTransaksi {
    addressTrx first;
};

struct listDetailTransaksi {
    addressDetail first;
    addressDetail last;
};

struct elmDetailTransaksi {
    infotypeDetailTransaksi info;
    addressDetail next;
    addressDetail prev;
};

struct elmTransaksi {
    infotypeTransaksi info;
    addressTrx next;
    addressDetail firstDetail;
    addressDetail lastDetail;
};

/*===================================*/

void insertParent(listTransaksi &L, addressTrx P, string position, addressTrx prec); //1
void showAllParent(listTransaksi L); //2
addressTrx deleteParent(listTransaksi &L, string position, addressTrx prec); //3
addressTrx searchParent(listTransaksi L, int x); //4
void insertChild(addressTrx L, addressDetail P, string position, addressDetail prec); //6
void connectParentToChild(addressTrx parent, addressDetail child); //7
void showAllParentChild(listTransaksi L); //8
addressDetail searchChildOnParent(listTransaksi L, int x); //9
addressDetail deleteChildOnParent(addressTrx parent, string position, addressDetail prec); //10
void countChildOnParent(addressTrx parent); //11
void saveToDatabase(listTransaksi L, string filename);
void loadFromDatabase(listTransaksi &L, string filename);

addressTrx createElmTransaksi(infotypeTransaksi x);

addressDetail createElmDetailTransaksi(infotypeDetailTransaksi x);

void showSearchedParent(listTransaksi L, string x);

void pressEnter();

int getLastIdTransaction(listTransaksi L);

addressTrx inputTransaction(listTransaksi &L);

addressTrx getPrec(listTransaksi L, addressTrx P);

void clearScreen();

#endif // STORE_H_INCLUDED
