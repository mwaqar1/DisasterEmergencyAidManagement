#ifndef MENU_H
#define MENU_H

namespace sdds {

class Menu {
  char *menuContent = nullptr;
  unsigned int options;

public:
  Menu(unsigned int options, const char *charcontent);
  Menu(Menu &m);
  Menu &operator=(const Menu &m);
  unsigned int run() const;
  ~Menu();
};

} // namespace sdds
#endif
