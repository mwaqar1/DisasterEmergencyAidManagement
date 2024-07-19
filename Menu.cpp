#include "Menu.h"
#include "Utils.h"
#include <cstring>
#include <iostream>

using namespace std;

namespace sdds {
Menu::Menu(unsigned int options, const char *content) {
  this->options = options;
  int len = strlen(content);
  menuContent = new char[len + 1];
  strcpy(menuContent, content);
}

Menu::~Menu() {
  if (menuContent) {
    delete[] menuContent;
  }

  menuContent = nullptr;
}

Menu &Menu::operator=(const Menu &m) {
  if (this != &m) {
    menuContent = new char[strlen(m.menuContent) + 1];
    strcpy(menuContent, m.menuContent);
  }

  return *this;
}

Menu::Menu(Menu &m) { *this = m; }

unsigned int Menu::run() const {
  unsigned int return_value;

  cout << menuContent;
  cout << "0- Exit" << endl << "> ";

  return_value = ut.getint(0, options, nullptr, nullptr);

  return return_value;
}

} // namespace sdds
