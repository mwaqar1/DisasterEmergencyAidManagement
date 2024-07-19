
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "Item.h"
#include "Perishable.h"
#include "Utils.h"

using namespace std;

namespace sdds {
void Perishable::setSafe() {
  if (mstringInstructions != nullptr) {
    delete[] mstringInstructions;
  }

  mstringInstructions = nullptr;
  mExpiryDate = 0;
  strcpy(mExpiryString, "");
  Item::setSafe();
}

Perishable::Perishable() { setSafe(); }

Perishable::~Perishable() { setSafe(); }

Perishable::Perishable(Perishable &other) : Item(other) { *this = other; }

Perishable &Perishable::operator=(const Perishable &other) {

  if (this != &other) // make sure current object is not being copied to the
                      // same location as other
  {
    setSafe();
    if (other.mstringInstructions != nullptr) {
      ut.alocpy(this->mstringInstructions, other.mstringInstructions);
    }
    mExpiryDate = other.mExpiryDate;
    Item::operator=(other);
  }

  return *this;
}

const char *Perishable::expiry() {
  int day, mon, year;
  if (mExpiryDate) {
    day = mExpiryDate % 100;
    mon = ((mExpiryDate - day) / 100) % 100;
    year = ((((mExpiryDate - day) / 100) - mon) / 100) % 100;

    sprintf(mExpiryString, "%d/%2d/%2d", 2000 + year, mon, day);
  }
  return mExpiryString;
}

int Perishable::readSku(istream &istr) {
  (void)istr;
  cout << "SKU: ";
  int sku = ut.getint(10000, 39999, nullptr, nullptr);
  // cout << "AMA Item:" << endl << "SKU: " << sku << endl;
  mSku = sku;
  return sku;
}

ofstream &Perishable::save(ofstream &ofstr) {
  if (*this) {
    Item::save(ofstr);
    char line[1000];
    if (mstringInstructions != nullptr) {
      sprintf(line, "\t%s", mstringInstructions);
      ofstr << line;
    } else {
      ofstr << "\t";
    }
    sprintf(line, "\t%d", mExpiryDate);
    ofstr << line;
  } else if (mSku) {
    char line[1000];
    sprintf(line, "%d\t%s\t%d\t%d\t%0.2f", mSku, mItemDescript, mCurrentQuant,
            mNeededQuant, mPrice);
    ofstr << line;
  }

  return ofstr;
}

ifstream &Perishable::load(ifstream &ifstr) {
  setSafe();

  Item::load(ifstr);

  int expiry;
  char description[1000];
  char p = ifstr.peek();
  if (p != EOF) {
    ut.extractChar(ifstr, '\t');
    p = ifstr.peek();
    if (p == '\n') // This item is not a perishable
    {
      // do nothing
    } else {
      if (p != '\t') {
        ifstr.get(description, 1000, '\t');
      } else {
        memset(description, 0, 1000);
      }

      ut.extractChar(ifstr, '\t');

      ifstr >> expiry;

      if (!ifstr.fail()) {
        if (strlen(description) > 0) {
          ut.alocpy(this->mstringInstructions, description);
        }
        this->mExpiryDate = expiry;
      }
    }

    ifstr.clear();
    ifstr.ignore(1000, '\n');
  } else {
    mStatusObj.setStatusObject("Input file stream read (perishable) failed!",
                               -1);
    ifstr.close();
  }

  p = ifstr.peek();
  if (p == EOF) {
    mStatusObj.setStatusObject("Input file stream read (perishable) failed!",
                               -1);
    ifstr.close();
  }

  return ifstr;
}

int Perishable::getSKU() { return mSku; }

const char *Perishable::getDescription() { return mItemDescript; }

const char *Perishable::getInstruction() { return mstringInstructions; }

double Perishable::getPrice() { return mPrice; }

ostream &Perishable::display(ostream &ostr) {
  if (*this) {

    if (isLinear()) {
      Item::display(ostr);
      if (mstringInstructions != nullptr && strlen(mstringInstructions) > 0) {
        ostr << "*";
      } else {
        ostr << " ";
      }

      ostr << expiry();

    } else {
      ostr << "Perishable ";
      Item::display(ostr);
      ostr << "Expiry date: ";
      ostr << expiry() << endl;
      if (mstringInstructions != nullptr && strlen(mstringInstructions) > 0) {
        ostr << "Handling Instructions: ";
        ostr << mstringInstructions << endl;
      }
    }
  } else if (mSku) {
    char shortenedDescript[36];
    int len;
    memset(shortenedDescript, 0, 36);

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

istream &Perishable::read(istream &istr) {
  char description[1000];
  setSafe();

  Item::read(istr);
  char peek_char;

  cout << "Expiry date (YYMMDD): ";
  ut.extractChar(istr, '\n');
  istr >> mExpiryDate;
  cout << "Handling Instructions, ENTER to skip: ";
  ut.extractChar(istr, '\n');
  istr.get(description, 1000, '\n');
  peek_char = istr.peek();
  if (peek_char != -1) {
    ut.alocpy(mstringInstructions, description);
  }

  return istr;
}

istream &operator>>(istream &istr, iProduct &ip) {
  ip.read(istr);

  return istr;
}

ostream &operator<<(ostream &ostr, iProduct &ip) {
  ip.display(ostr);

  return ostr;
}
#if 0
    ofstream& operator<<(ofstream& ofstr, iProduct& ip)
    {
        ip.printToFile(ofstr, ip);

        return ofstr;
    }


    ofstream& operator<<(ofstream& ofstr, Perishable& pe)
    {
        pe.printToFile(ofstr, pe);

        return ofstr;
    }
#endif
ostream &operator<<(ostream &ostr, Perishable &pe) {
  pe.display(ostr);

  return ostr;
}

Perishable::operator bool() {
  if (mExpiryDate == 0) {
    return false;
  } else {
    return true;
  }
}
#if 0    
    ofstream& Perishable::printToFile(ofstream& ofstr, Perishable &pe)
    {
        if (*this)
        {

            Item::printToFile(ofstr, pe);
            if (mstringInstructions != nullptr && strlen(mstringInstructions) > 0)
            {
                ofstr << "*";    
            }
            else
            {
                ofstr << " ";
            }

            ofstr << expiry();

        }
        else if (mSku)
        {
            char shortenedDescript[36];  
            int len; 
		    memset(shortenedDescript,0,36);
        
            if (mItemDescript != nullptr)
            {    
                if (mLinOrDescript)
                {
                    ofstr << right << ofstr.fill(' ') << setw(5) << mSku << " ";

                    len = strlen(mItemDescript);
                    if (len > 35)
                    {
                        len = 35;
                    }
                    strncpy(shortenedDescript, mItemDescript, len); 

                    ofstr << "| " << left << ofstr.fill(' ') << setw(35) << shortenedDescript;
                    ofstr << " | " << right << ofstr.fill(' ') <<setw(4) << qty();
                    ofstr << " | " << right << ofstr.fill(' ') <<setw(4) << qtyNeeded();
                    ofstr << " | " << ofstr.fill(' ') <<setw(7) << fixed << setprecision(2) << mPrice << " |";
                }
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
