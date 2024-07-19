#ifndef SDDS_UTILS_H
#define SDDS_UTILS_H

#include <fstream>
#include <iostream>

namespace sdds {
const int sdds_testYear = 2022;
const int sdds_testMon = 03;
const int sdds_testDay = 31;

const int max_string = 50;

class Utils {
  bool m_testMode = false;

public:
  void getSystemDate(int *year = nullptr, int *mon = nullptr,
                     int *day = nullptr);
  int daysOfMon(int mon, int year) const;
  void testMode(bool testmode = true);

  // Recommended Utils
  void alocpy(char *&destination, const char *source); // check
  int getint(const char *prompt = nullptr);            // check
  int getint(int min, int max, const char *prompt = nullptr,
             const char *errMes = nullptr);       // check
  double getdouble(const char *prompt = nullptr); // check
  double getdouble(double min, double max, const char *prompt = nullptr,
                   const char *errMes = nullptr); // check

  // functions from lab 01
  char toLower(char ch);
  int strCmp(const char *s1, const char *s2);
  int strnCmp(const char *s1, const char *s2, int len);
  void strCpy(char *des, const char *src);
  int strLen(const char *str);
  const char *strStr(const char *str, const char *find);
  const char *strStr_ci(const char *str, const char *find);
  int strnCmp_ci(const char *s1, const char *s2, int len);
  int isAlpha(char ch);
  int isSpace(char ch);
  void trim(char word[]);
  void toLowerCaseAndCopy(char des[], const char source[]);
  void read(char *cString, unsigned int maxSize, char delimiter = '\n');
  int getDaysInMonth(int month, int year);
  bool isDigit(char *c);
  bool isDouble(char *c);
  void extractChar(std::istream &istr, char ch) const;
  bool extractChar(std::ifstream &istr, char ch) const;
};
extern Utils ut;

} // namespace sdds

#endif // !SDDS_UTILS_H
