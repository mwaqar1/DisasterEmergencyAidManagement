#ifndef ITEM_H
#define ITEM_H

#include "Status.h"
#include "iProduct.h"
#include <iostream>

namespace sdds {

class Item : public iProduct {

protected:
  double mPrice;
  int mCurrentQuant;
  int mNeededQuant;

  char *mItemDescript = nullptr;
  bool mLinOrDescript = false;

  Status mStatusObj;
  int mSku;

public:
  Item();
  Item(double price, int current, int needed, char *description, bool linear,
       int sku, Status state);
  ~Item();
  Item(const Item &other);            // copy constructor
  Item &operator=(const Item &other); // copy operator
  void setSafe();
  int qtyNeeded();
  int qty();
  operator double(); // double conversion operator
  operator bool();   // bool conversion operator
  int operator+=(int qty);
  int operator-=(int qty);
  void linear(bool isLinear);
  void clear();
  bool operator==(int sku);
  bool operator==(const char *description);
  int getSKU();
  int readSku(std::istream &istr);
  bool isLinear();
  double getPrice() { return 0; }
  const char *getDescription() { return nullptr; }
  const char *getInstruction() { return nullptr; }
  const char *expiry() { return nullptr; }

  std::ofstream &save(std::ofstream &ofstr);
  std::ifstream &load(std::ifstream &ifstr);
  std::ostream &display(std::ostream &ostr);
  std::istream &read(std::istream &istr);
  // std::ofstream& printToFile(std::ofstream& ofstr, iProduct& pe);
};

} // namespace sdds
#endif
