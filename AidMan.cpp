#include "AidMan.h"
#include "Date.h"
#include "Menu.h"
#include "Perishable.h"
#include "Utils.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

namespace sdds {
AidMan &AidMan::operator=(const AidMan &other) {
  if (this != &other) {
    this->file_name = new char[strlen(other.file_name) + 1];
    strcpy(this->file_name, other.file_name);
    this->main_menu = other.main_menu;
  }

  return *this;
}

AidMan::AidMan(const AidMan &other) { *this = other; }

AidMan::AidMan() {
  file_name = nullptr;
  main_menu =
      new Menu(7, "1- List Items\n2- Add Item\n3- Remove Item\n4- Update "
                  "Quantity\n5- Sort\n6- Ship Items\n7- New/Open Aid "
                  "Database\n---------------------------------\n");

  for (int i = 0; i < sdds_max_num_items; i++) {
    mIProduct[i] = nullptr;
  }

  mSize = 0;

  mDatabaseOpened = false;
}

AidMan::~AidMan() {
  if (file_name) {
    delete[] file_name;
    file_name = nullptr;
  }

  if (main_menu) {
    delete main_menu;
    main_menu = nullptr;
  }
}

int AidMan::menu() {
  static int first = 0;
  if (first != 0) {
    cout << endl;
  }
  first = 1;

  cout << "Aid Management System\n";
  cout << "Date: 2022/03/31\n";
  cout << "Data file: ";
  if (file_name == nullptr) {
    cout << "No file" << endl;
  } else {
    cout << file_name << endl;
  }

  cout << "---------------------------------\n";
  int num = main_menu->run();
  return num;
}

void AidMan::run() {
  int num;
  char tmpFileName[100];
  while (1) {
    num = menu();
    if (num == 0) {
      saveDataToFile();
      cout << "Exiting Program!\n";
      break;
    }

    if (!mDatabaseOpened) {
      num = 7;
    }

    if (num == 1) {
      cout << "\n****List Items****\n";
      list();
    } else if (num == 2) {
      cout << "\n****Add Item****\n";
      addItem();
    } else if (num == 3) {
      char sub_desc[100];
      int numPrinted = 0;
      char peek_char;
      cout << "\n****Remove Item****\n";
      cout << "Item description: ";
      cin.clear();
      cin.ignore();
      cin.get(sub_desc, 100, '\n');
      peek_char = cin.peek();
      if (peek_char != -1) {
        numPrinted = list(sub_desc);
      }

      if (numPrinted > 0) {
        cout << "Enter SKU: ";
        int sku = ut.getint(10000, 99999, nullptr, nullptr);
        int searchIndex = search(sku);
        if (searchIndex == -1) {
          cout << "SKU not found!" << endl;
        } else {
          cout << "Following item will be removed: " << endl;
          cout << *mIProduct[searchIndex] << endl;
          cout << "Are you sure?" << endl;

          Menu yesno(1, "1- Yes!\n");
          int choice = yesno.run();
          if (choice == 1) {
            remove(searchIndex);
            cout << "Item removed!" << endl;
          } else {
            cout << "Aborted!" << endl;
          }
        }
      }
    } else if (num == 4) {
      char sub_desc[100];
      int numPrinted = 0;
      char peek_char;
      cout << "\n****Update Quantity****\n";
      cout << "Item description: ";
      cin.clear();
      cin.ignore();
      cin.get(sub_desc, 100, '\n');
      peek_char = cin.peek();
      if (peek_char != -1) {
        numPrinted = list(sub_desc);
      }

      if (numPrinted > 0) {
        cout << "Enter SKU: ";
        int sku = ut.getint(10000, 99999, nullptr, nullptr);
        int searchIndex = search(sku);
        if (searchIndex == -1) {
          cout << "SKU not found!" << endl;
        } else {
          Menu updateQuantity(2, "1- Add\n2- Reduce\n");
          int choice = updateQuantity.run();

          if (choice == 1) {
            if (mIProduct[searchIndex]->qtyNeeded() ==
                mIProduct[searchIndex]->qty()) {
              cout << "Quantity Needed already fulfilled!\n";
            } else {
              cout << "Quantity to add: ";
              int addValue = ut.getint(1,
                                       mIProduct[searchIndex]->qtyNeeded() -
                                           mIProduct[searchIndex]->qty(),
                                       nullptr, nullptr);
              *mIProduct[searchIndex] += addValue;
              cout << addValue << " items added!\n";
            }
          } else if (choice == 2) {
            if (mIProduct[searchIndex]->qty() == 0) {
              cout << "Quaintity on hand is zero!\n";
            } else {
              cout << "Quantity to reduce: ";
              int subValue =
                  ut.getint(1, mIProduct[searchIndex]->qty(), nullptr, nullptr);
              *mIProduct[searchIndex] += (-subValue);
              cout << subValue << " items removed!\n";
            }
          } else {
            cout << "Aborted!" << endl;
          }
        }
      } else {
        cout << "No matches found!\n";
      }
    } else if (num == 5) {
      cout << "\n****Sort****\n";
      sort();
      cout << "Sort completed!\n";
    } else if (num == 6) {
      int printCount = 0;
      int i = 0;
      cout << "\n****Ship Items****\n";
      ofstream file("shippingOrder.txt");
      Date date;
      file << "Shipping Order, Date: " << date << endl;

      file << " ROW |  SKU  | Description                         | Have | "
              "Need |  Price  | Expiry\n";
      file << "-----+-------+-------------------------------------+------+-----"
              "-+---------+-----------"
           << endl;
      while (i < mSize) {
        int qty = mIProduct[i]->qty();
        int qtyNeeded = mIProduct[i]->qtyNeeded();
        if (qty == qtyNeeded) {
          char shortenedDescript[36];
          memset(shortenedDescript, 0, 36);
          int len;

          // file.clear();
          // file << *mIProduct[i] << endl;

          len = strlen(mIProduct[i]->getDescription());
          if (len > 35) {
            len = 35;
          }
          strncpy(shortenedDescript, mIProduct[i]->getDescription(), len);

          char tmp[100];
          memset(tmp, 0, 100);
          char tmp2[100];
          memset(tmp2, 0, 100);

          sprintf(tmp, "%4d | %5d | %-35s | %4d | %4d | %7.2f", printCount + 1,
                  mIProduct[i]->getSKU(), shortenedDescript,
                  mIProduct[i]->qtyNeeded(), mIProduct[i]->qty(),
                  mIProduct[i]->getPrice());

          if (strlen(mIProduct[i]->expiry()) > 0) {
            sprintf(tmp2, "%s |*%s", tmp, mIProduct[i]->expiry());
          } else {
            sprintf(tmp2, "%s |", tmp);
          }

          file << tmp2 << endl;

          remove(i);
          printCount++;

        } else {
          i++;
        }
      }

      file << "-----+-------+-------------------------------------+------+-----"
              "-+---------+-----------\n";

      cout << "Shipping Order for " << printCount << " times saved!" << endl;

      file.close();
    } else if (num == 7) {
      cout << "\n****New/Open Aid Database****\n";

      if (file_name == nullptr) {
        cout << "Enter file name: ";
        cin >> tmpFileName;

        if (tmpFileName != nullptr && strlen(tmpFileName) > 0) {
          ut.alocpy(file_name, tmpFileName);
        }
      }

      mSize = readDataFromFile(file_name);

      if (mSize > 0) {
        mDatabaseOpened = true;
        cout << mSize << " records loaded!" << endl;
      } else {
        cout << "Unable to read from database" << endl;
        delete[] file_name;
        file_name = nullptr;
      }
    }
  }
}

int AidMan::readDataFromFile(const char *filename) {
  int recordCount = 0;
  ifstream file(filename);

  Perishable *p;

  while (1) {
    p = new Perishable;

    if (!p->load(file)) {
      delete p;
      break;
    }

    mIProduct[recordCount] = p;
    recordCount++;
    // cout << *p << endl;
  }

  return recordCount;
}

int AidMan::saveDataToFile() {
  if (file_name != nullptr) {
    ofstream file(file_name, ios::out);

    for (int i = 0; i < mSize; i++) {
      mIProduct[i]->save(file) << endl;
      delete mIProduct[i];
    }
    file.close();
  }

  return 0;
}

int AidMan::list(const char *sub_desc) {
  int i;
  char shortenedDescript[36];
  int len;
  bool printAll = false;
  bool found = false;
  int itemsPrinted = 0;

  if (sub_desc == nullptr) {
    printAll = true; // print all
    found = true;
  } else {
    for (i = 0; i < mSize; i++) {
      if (ut.strStr(mIProduct[i]->getDescription(), sub_desc) != nullptr) {
        found = true;
        break;
      }
    }
  }

  if (found) {

    if (printAll) {
      cout << " ROW |  SKU  | Description                         | Have | "
              "Need |  Price  | Expiry"
           << endl;
      cout << "-----+-------+-------------------------------------+------+-----"
              "-+---------+-----------"
           << endl;

      for (i = 0; i < mSize; i++) {
        itemsPrinted++;
        memset(shortenedDescript, 0, 36);

        cout << right << setw(4) << i + 1 << " |";
        cout << right << setw(6) << mIProduct[i]->getSKU() << " | ";

        len = strlen(mIProduct[i]->getDescription());
        if (len > 35) {
          len = 35;
        }
        strncpy(shortenedDescript, mIProduct[i]->getDescription(), len);

        cout << left << setw(35) << shortenedDescript << " |";
        cout << right << setw(5) << mIProduct[i]->qty() << " |";
        cout << right << setw(5) << mIProduct[i]->qtyNeeded() << " | ";
        cout << right << setw(7) << fixed << setprecision(2)
             << mIProduct[i]->getPrice() << " |";

        if (strlen(mIProduct[i]->expiry()) > 0) {
          if (mIProduct[i]->getInstruction() != nullptr &&
              strlen(mIProduct[i]->getInstruction()) > 0) {
            cout << "*";
          } else {
            cout << " ";
          }

          cout << mIProduct[i]->expiry();
        }

        cout << endl;
      }
    } else {
      cout << " ROW |  SKU  | Description                         | Have | "
              "Need |  Price  | Expiry"
           << endl;
      cout << "-----+-------+-------------------------------------+------+-----"
              "-+---------+-----------"
           << endl;

      for (i = 0; i < mSize; i++) {
        memset(shortenedDescript, 0, 36);

        if (ut.strStr(mIProduct[i]->getDescription(), sub_desc) != nullptr) {
          itemsPrinted++;
          cout << right << setw(4) << i + 1 << " |";
          cout << right << setw(6) << mIProduct[i]->getSKU() << " | ";

          len = strlen(mIProduct[i]->getDescription());
          if (len > 35) {
            len = 35;
          }
          strncpy(shortenedDescript, mIProduct[i]->getDescription(), len);

          cout << left << setw(35) << shortenedDescript << " |";
          cout << right << setw(5) << mIProduct[i]->qty() << " |";
          cout << right << setw(5) << mIProduct[i]->qtyNeeded() << " | ";
          cout << right << setw(7) << fixed << setprecision(2)
               << mIProduct[i]->getPrice() << " |";

          if (strlen(mIProduct[i]->expiry()) > 0) {
            if (mIProduct[i]->getInstruction() != nullptr &&
                strlen(mIProduct[i]->getInstruction()) > 0) {
              cout << "*";
            } else {
              cout << " ";
            }

            cout << mIProduct[i]->expiry();
          }

          cout << endl;
        }
      }
    }

    cout << "-----+-------+-------------------------------------+------+------+"
            "---------+-----------"
         << endl;

    if (printAll) {
      do {
        cout << "Enter row number to display details or <ENTER> to continue:"
             << endl
             << "> ";
        cin.clear();
        cin.ignore();
        cin.get(shortenedDescript, 3, '\n');
        char peek_char = cin.peek();
        if (peek_char != -1) {
          int choice = atoi(shortenedDescript) - 1;
          if (choice > 0 && choice < mSize) {
            cout << *mIProduct[choice] << endl;
            break;
          } else {
            continue;
          }
        } else {
          break;
        }
      } while (1);
    }
  } else {
    cout << "The list is emtpy!" << endl;
  }

  return itemsPrinted;
}

int AidMan::search(int sku) const {
  int found = -1;
  for (int i = 0; i < mSize; i++) {
    if (sku == mIProduct[i]->getSKU()) {
      found = i;
      break;
    }
  }

  return found;
}

void AidMan::addItem() {
  int skuIndex;
  if (mSize == sdds_max_num_items) {
    cout << "Database full!" << endl;
  } else {
    Menu addItemMenu(2,
                     "1- Perishable\n2- Non-Perishable\n-----------------\n");
    int num = addItemMenu.run();

    iProduct *p;

    if (num == 0) {
      cout << "Aborted\n";
      return;
    } else if (num == 1) {
      p = new Perishable;
    } else if (num == 2) {
      p = new Item;
    }

    p->readSku(cin);
    skuIndex = search(p->getSKU());
    if (skuIndex != -1) {
      cout << "Sku: " << p->getSKU()
           << " is already in the system, try updating quantity instead.\n";
      delete p;
      return;
    } else {
      p->read(cin);
      if (p) {
        mIProduct[mSize] = p;
        mSize++;
      } else {
        cout << *p << endl;
        delete p;
      }
    }
  }
}

void AidMan::remove(int index) {
  int i;

  delete mIProduct[index];
  mIProduct[index] = nullptr;

  for (i = index; i < mSize; i++) {
    mIProduct[i] = mIProduct[i + 1];
  }

  delete mIProduct[i];
  mIProduct[i] = nullptr;
  mSize--;
}

void AidMan::sort() {
  int i, j, iQtyNeeded, jQtyNeeded;
  iProduct *tmp;

  for (i = 0; i < mSize; i++) {
    for (j = 0; j < mSize; j++) {
      if (i != j) {
        iQtyNeeded = mIProduct[i]->qtyNeeded() - mIProduct[i]->qty();
        jQtyNeeded = mIProduct[j]->qtyNeeded() - mIProduct[j]->qty();

        if (iQtyNeeded > jQtyNeeded) {
          tmp = mIProduct[i];
          mIProduct[i] = mIProduct[j];
          mIProduct[j] = tmp;
        }
      }
    }
  }
}
} // namespace sdds
