#ifndef AIDMAN_H
#define AIDMAN_H
#include "Menu.h"
#include "iProduct.h"

namespace sdds {
const int sdds_max_num_items = 100;

class AidMan {
  iProduct *mIProduct[sdds_max_num_items];
  int mSize;
  char *file_name;
  Menu *main_menu;
  int menu();
  bool mDatabaseOpened = false;
  int readDataFromFile(const char *filename);
  int saveDataToFile();
  int list(const char *sub_desc = nullptr);
  int search(int sku) const;
  void addItem();
  void remove(int index);
  void sort();

public:
  AidMan();
  ~AidMan();
  void run();
  AidMan &operator=(const AidMan &);
  AidMan(const AidMan &);
};
} // namespace sdds

#endif
