#ifndef SDDS_PERISHABLE_H
#define SDDS_PERISHABLE_H

#include "Item.h"
#include <string>

namespace sdds {
class Perishable : public Item {

  int mExpiryDate;
  char mExpiryString[11];
  char *mstringInstructions = nullptr;

public:
  void setSafe();
  Perishable();
  ~Perishable();
  Perishable(Perishable &other);                  // copy constructor
  Perishable &operator=(const Perishable &other); // copy operator
  const char *expiry();
  int readSku(std::istream &istr);
  operator bool();
  std::ofstream &save(std::ofstream &ofstr);
  std::ifstream &load(std::ifstream &ifstr);
  std::ostream &display(std::ostream &ostr);
  std::istream &read(std::istream &istr);
  // std::ofstream& printToFile(std::ofstream& ofstr, Perishable& pe);

  int getSKU();
  const char *getDescription();
  double getPrice();
  const char *getExpiryString();
  const char *getInstruction();
};

std::ostream &operator<<(std::ostream &, Perishable &);
} // namespace sdds

#endif // SDDS_PERISHABLE_H
