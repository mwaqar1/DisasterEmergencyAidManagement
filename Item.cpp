#include "Item.h"
#include "Utils.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

namespace sdds {

void Item::setSafe() {
  if (mItemDescript != nullptr) {
    delete[] mItemDescript;
  }

  mItemDescript = nullptr;
  mPrice = 0.0;
  mCurrentQuant = 0;
  mNeededQuant = 0;
  mLinOrDescript = false;
  mSku = 0;
  mStatusObj.clear();
}

Item::Item(double price, int current, int needed, char *description,
           bool linear, int sku, Status state) {
  setSafe();
  mItemDescript = description;
  mPrice = price;
  mCurrentQuant = current;
  mNeededQuant = needed;
  mLinOrDescript = linear;
  mSku = sku;
  mStatusObj = state;
}

Item::Item() { setSafe(); }

Item::~Item() { setSafe(); }

Item::Item(const Item &other) { *this = other; }

Item &Item::operator=(const Item &other) {

  if (this != &other) // make sure current object is not being copied to the
                      // same location as other
  {
    setSafe();
    if (other.mItemDescript != nullptr) {
      ut.alocpy(this->mItemDescript, other.mItemDescript);
      mPrice = other.mPrice;
      mCurrentQuant = other.mCurrentQuant;
      mNeededQuant = other.mNeededQuant;
      mLinOrDescript = other.mLinOrDescript;
      mSku = other.mSku;
    }
  }

  return *this;
}

int Item::qtyNeeded() { return mNeededQuant; }

int Item::qty() { return mCurrentQuant; }

Item::operator double() { return mPrice; }

Item::operator bool() {
  // the boolean conversion operator overload returns the state of the object
  // being good?
  return (bool)mStatusObj;
}

int Item::operator+=(int qty) {
  mCurrentQuant += qty;
  return mCurrentQuant;
}

int Item::operator-=(int qty) {
  mCurrentQuant -= qty;
  return mCurrentQuant;
}

void Item::clear() {
  // create a modifier called clear that does not accept any argument and sets
  // the state of the Item object back to good,
  //  by resetting the state attribute.
  mStatusObj.clear();
}

bool Item::operator==(int sku) { return (mSku == sku); }

bool Item::operator==(const char *description) {
  bool return_value = false;

  if (description != nullptr && strlen(description) > 0 &&
      mItemDescript != nullptr &&
      ut.strStr(mItemDescript, description) != nullptr) {
    return_value = true;
  }

  return return_value;
}

int Item::getSKU() { return mSku; }

ofstream &Item::save(ofstream &ofstr) {
  if (*this) {
    char line[1000];
    sprintf(line, "%d\t%s\t%d\t%d\t%0.2f", mSku, mItemDescript, mCurrentQuant,
            mNeededQuant, mPrice);
    ofstr << line;
  }

  return ofstr;
}

ifstream &Item::load(ifstream &ifstr) {

  int sku;
  char description[1000];
  int on_hand;
  int needed;
  double price;
  char p;

  setSafe();

  p = ifstr.peek();

  if (p != EOF) {
    ifstr >> sku;
    ut.extractChar(ifstr, '\t');
    ifstr.get(description, 1000, '\t');
    ifstr >> on_hand;
    ut.extractChar(ifstr, '\t');
    ifstr >> needed;
    ut.extractChar(ifstr, '\t');
    ifstr >> price;

    if (!ifstr.fail()) {
      ut.alocpy(this->mItemDescript, description);
      mPrice = price;
      mCurrentQuant = on_hand;
      mNeededQuant = needed;
      mLinOrDescript = false;
      mSku = sku;
    }

    // ifstr.clear();
    // ifstr.ignore(1000, '\n');
  } else {
    // mStatusObj.setStatusObject("", -1);
    ifstr.close();
  }

  p = ifstr.peek();

  if (p == EOF) {
    // mStatusObj.setStatusObject("", -1);
    ifstr.close();
  }

  return ifstr;
}

ostream &Item::display(ostream &ostr) {
  char shortenedDescript[36];
  int len;
  memset(shortenedDescript, 0, 36);
  if (*this) {
    if (mItemDescript != nullptr) {
      if (mLinOrDescript) {
        ostr << right << ostr.fill(' ') << setw(5) << mSku << " ";

        len = strlen(mItemDescript);
        if (len > 35) {
          len = 35;
        }
        strncpy(shortenedDescript, mItemDescript, len);

        ostr << "| " << left << ostr.fill(' ') << setw(35) << shortenedDescript;
        ostr << " | " << right << ostr.fill(' ') << setw(4) << qty();
        ostr << " | " << right << ostr.fill(' ') << setw(4) << qtyNeeded();
        ostr << " | " << ostr.fill(' ') << setw(7) << fixed << setprecision(2)
             << mPrice << " |";
      } else {
        ostr << "AMA Item:" << endl;
        ostr << right << setw(5) << mSku << ": ";
        ostr << mItemDescript << endl;
        ostr << "Quantity Needed: " << qtyNeeded() << endl;
        ostr << "Quantity Available: " << qty() << endl;
        ostr << "Unit Price: $" << fixed << setprecision(2) << mPrice << endl;
        ostr << "Needed Purchase Fund: $" << fixed << setprecision(2)
             << (double)(qtyNeeded() - qty()) * mPrice << endl;
      }
    }
  } else {
    ostr << (const char *)mStatusObj;
  }

  return ostr;
}

int Item::readSku(std::istream &istr) {
  (void)istr;
  cout << "SKU: ";
  int sku = ut.getint(40000, 99999, nullptr, nullptr);
  mSku = sku;
  return sku;
}

istream &Item::read(istream &istr) {
  char description[1000];

  clear();
  istr.clear();

  if (mItemDescript != nullptr) {
    delete[] mItemDescript;
  }
  mItemDescript = nullptr;

  ut.extractChar(istr, '\n');
  cout << "AMA Item:" << endl << "SKU: " << mSku << endl;
  cout << "Description: ";
  istr.get(description, 1000, '\n');
  ut.alocpy(mItemDescript, description);
  cout << "Quantity Needed: ";
  mNeededQuant = ut.getint(1, 9999, nullptr, nullptr);
  cout << "Quantity On Hand: ";
  mCurrentQuant = ut.getint(0, mNeededQuant, nullptr, nullptr);
  cout << "Unit Price: $";
  mPrice = ut.getdouble(0, 9999, nullptr, nullptr);

  return istr;
}

bool Item::isLinear() { return mLinOrDescript; }

void Item::linear(bool linear) { mLinOrDescript = linear; }
#if 0
    ofstream& Item::printToFile(ofstream& ofstr, iProduct& ip)
    {
        (void)ip;
        char shortenedDescript[36];  
        int len; 
		memset(shortenedDescript,0,36);
        if (*this)
        {
            if (mItemDescript != nullptr)
            {    
                ofstr << right << ofstr.fill(' ') <<setw(5) << mSku << " ";

                len = strlen(mItemDescript);
                if (len > 35)
                {
                    len = 35;
                }
                strncpy(shortenedDescript, mItemDescript, len); 

                ofstr << "| " << left << ofstr.fill(' ') <<setw(35) << shortenedDescript;
                ofstr << " | " << right << ofstr.fill(' ') <<setw(4) << qty();
                ofstr << " | " << right << ofstr.fill(' ') <<setw(4) << qtyNeeded();
                ofstr << " | " << setw(7) << ofstr.fill(' ') <<fixed << setprecision(2) << mPrice << " |";
            }
        }
        else
        {
            ofstr << (const char*) mStatusObj;
        }
        return ofstr;
    }
#endif

} // namespace sdds
