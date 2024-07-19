#ifndef IPRODUCT_H
#define IPRODUCT_H

#include <iostream>

namespace sdds {

class iProduct {
public:
  // to read the Stock-Keeping Unit from console before
  // main data entry
  virtual int readSku(std::istream &istr) = 0;
  // to reduce the quantity on hand
  virtual int operator-=(int qty) = 0;
  // to increase the quantity on hand
  virtual int operator+=(int qty) = 0;
  // returns the price of the produce
  virtual operator double() = 0;
  // returns if the iProduct is in a good state
  virtual operator bool() = 0;
  // returns the number of products needed
  virtual int qtyNeeded() = 0;
  // retuns the quantity on hand
  virtual int qty() = 0;
  // determines if the iProduct is displayed in a linear format or
  // descriptive format
  virtual void linear(bool isLinear) = 0;
  // saves the iProduct into a file
  virtual std::ofstream &save(std::ofstream &ofstr) = 0;
  // loads an iProduct from a file
  virtual std::ifstream &load(std::ifstream &ifstr) = 0;
  // displays the iProduct on the screen
  virtual std::ostream &display(std::ostream &ostr) = 0;
  // reads the iProduct from the console
  virtual std::istream &read(std::istream &istr) = 0;
  // return true if the sku is a match to the iProduct's sku
  virtual bool operator==(int sku) = 0;
  // rtrns true if the description is found in the iPorduct's description
  virtual bool operator==(const char *description) = 0;
  virtual ~iProduct();
  virtual int getSKU() = 0;
  virtual double getPrice() = 0;
  virtual const char *getDescription() = 0;
  virtual const char *getInstruction() = 0;
  virtual const char *expiry() = 0;
  //        virtual std::ofstream& printToFile(std::ofstream& ofstr, iProduct&
  //        pe) = 0;
};

std::istream &operator>>(std::istream &, iProduct &);
std::ostream &operator<<(std::ostream &, iProduct &);
//    std::ofstream& operator<<(std::ofstream&, iProduct&);

} // namespace sdds
#endif
