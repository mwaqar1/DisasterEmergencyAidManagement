#define _CRT_SECURE_NO_WARNINGS
#include "Utils.h"
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

namespace sdds {
Utils ut;
void Utils::testMode(bool testmode) { m_testMode = testmode; }
void Utils::getSystemDate(int *year, int *mon, int *day) {
  if (m_testMode) {
    if (day)
      *day = sdds_testDay;
    if (mon)
      *mon = sdds_testMon;
    if (year)
      *year = sdds_testYear;
  } else {
    time_t t = std::time(NULL);
    tm lt = *localtime(&t);
    if (day)
      *day = lt.tm_mday;
    if (mon)
      *mon = lt.tm_mon + 1;
    if (year)
      *year = lt.tm_year + 1900;
  }
}

int Utils::daysOfMon(int month, int year) const {
  int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1};
  int mon = (month >= 1 && month <= 12 ? month : 13) - 1;
  return days[mon] + int((mon == 1) & ((year % 4 == 0) && (year % 100 != 0)) ||
                         (year % 400 == 0));
}

void Utils::alocpy(char *&destination,
                   const char *source) // reference to a pointer
{
  int len = 0;
  if (destination) {
    delete[] destination;
  }

  destination = nullptr;

  if (source != nullptr) {
    len = strlen(source) + 1;
    destination = new char[len];
    strcpy(destination, source);
  }
}

int Utils::getint(const char *prompt) {
  char userInt[max_string];
  bool validUserInt = false;
  int returnInt = -5;
  /*
   If the prompt is not null, it will display it.
   Then it performs a foolproof entry of an integer. If the user enters an
   invalid integer it will display " Invalid Integer, retry: " and will not let
   the user leave the function until a valid integer is entered.

  */

  if (prompt != nullptr) { // display and prompt for user input
    cout << prompt;
  }

  cin.clear();

  do {
    cin >> userInt;

    // validate if userInt is an int
    if (userInt[0] == 0) {
      cin.clear();
      continue;
    } else if (isDigit(userInt)) {
      validUserInt = true;
      returnInt = atoi(userInt); // set returnInt to userInt
    } else                       // invalid userInt
    {
      cout << "Invalid Integer, retry: ";
    }

  } while (!validUserInt);

  return returnInt;
}

bool Utils::isDigit(char *c) {
  bool ret = true;
  int len = strlen(c);

  for (int i = 0; i < len; i++) {
    if (c[i] >= '0' && c[i] <= '9') {
      ret = ret & true;
    } else {
      ret = false;
    }
  }

  return ret;
}

bool Utils::isDouble(char *c) {
  bool ret = true;
  int len = strlen(c);
  int dotCount = 0;

  for (int i = 0; i < len; i++) {
    if (c[i] >= '0' && c[i] <= '9') {
      ret = ret & true;
    } else if ((c[i] <= '.') && (dotCount < 1)) {
      ret = ret & true;
      dotCount++;
    } else {
      ret = false;
    }
  }

  return ret;
}
int Utils::getint(int min, int max, const char *prompt, const char *errMes) {
  /*
   If the prompt is not null it will be displayed before getting a valid
   integer.(reuse the previous getint()) If the entered integer is not within
   the acceptable range (min and max) then an error message is displayed and it
   will not let the user leave the function until an acceptable value is
   entered. When printing the error message, if the errMes argument is not null
   it will be displayed followed by ", retry: " otherwise the general error
   message "Value out of range [min<=val<=max]: " is displayed (replacing min
   and max with their values)

   */

  int returnedUserInt;
  bool exitWhile = false;

  do {
    returnedUserInt = getint(prompt);

    if (returnedUserInt >= min &&
        returnedUserInt <= max) // indicates valid returnedUserInt
    {
      // CHECK if min or max are inlusive or not?
      exitWhile = true; // exit loop  //set finalReturnInt to valid
                        // returnedUserInt value

    } else // invalid returnedUserInt range
    {
      if (errMes != nullptr) {
        cout << errMes << ", retry: "; // CORRECT??

      } else // errMes == nullptr
      {
        //"Value out of range [min<=val<=max]: " is displayed (replacing min and
        //max with their values)
        cout << "Value out of range [" << min << "<=val<=" << max
             << "]: "; // CHECK output
      }
    }
  } while (!exitWhile);

  return returnedUserInt;
}

// returns the lower case value of a character
char Utils::toLower(char ch) {
  if (ch >= 'A' && ch <= 'Z')
    ch += ('a' - 'A');
  return ch;
}

// compares s1 and s2 cStrings and returns:
// > 0 if s1 > s2
// < 0 if s1 < s3
// == 0 if s1 == s2
int Utils::strCmp(const char *s1, const char *s2) {
  int i;
  for (i = 0; s1[i] && s2[i] && s1[i] == s2[i]; i++)
    ;
  return s1[i] - s2[i];
}

// compares s1 and s2 cStrings upto len characters and returns:
// > 0 if s1 > s2
// < 0 if s1 < s3
// == 0 if s1 == s2
int Utils::strnCmp(const char *s1, const char *s2, int len) {
  int i = 0;
  while (i < len - 1 && s1[i] && s2[i] && s1[i] == s2[i]) {
    i++;
  }
  return s1[i] - s2[i];
}

// This function is a case-insensitive copy of strnCmp.
int Utils::strnCmp_ci(const char *s1, const char *s2, int len) {
  int i = 0;
  while (i < len - 1 && s1[i] && s2[i] && toLower(s1[i]) == toLower(s2[i])) {
    i++;
  }
  return toLower(s1[i]) - toLower(s2[i]);
}

// copies src to des
void Utils::strCpy(char *des, const char *src) {
  int i;
  for (i = 0; src[i]; i++)
    des[i] = src[i];
  des[i] = 0;
}

// returns the length of str
int Utils::strLen(const char *str) {
  int len;
  for (len = 0; str[len]; len++)
    ;
  return len;
}

// if "find" is found in "str" it will return the addres of the match
// if not found it will returns nullptr (zero)
const char *Utils::strStr(const char *str, const char *find) {
  const char *faddress = nullptr;
  int i, flen = strLen(find), slen = strLen(str);
  for (i = 0; i <= slen - flen && strnCmp(&str[i], find, flen); i++)
    ;
  if (i <= slen - flen)
    faddress = &str[i];
  return faddress;
}

// This is the case-insensitive copy of strStr function.
const char *Utils::strStr_ci(const char *str, const char *find) {
  const char *faddress = nullptr;
  int i, flen = strLen(find), slen = strLen(str);
  for (i = 0; i <= slen - flen && strnCmp_ci(&str[i], find, flen); i++)
    ;
  if (i <= slen - flen)
    faddress = &str[i];
  return faddress;
}

// returns true if ch is alphabetical
int Utils::isAlpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// returns true if ch is a whitespace character
int Utils::isSpace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' ||
         ch == '\r';
}

// removes the non-alphabetic characters from the begining and end of a word
void Utils::trim(char word[]) {
  int i;
  while (word[0] && !isAlpha(word[0])) {
    strCpy(word, word + 1);
  }
  i = strLen(word);
  while (i && !isAlpha(word[i - 1])) {
    word[i-- - 1] = 0;
  }
}

// copies the  lower case version of the source into des.
void Utils::toLowerCaseAndCopy(char des[], const char source[]) {
  int i = 0, j = 0;
  for (; source[i] != 0; i++) {
    des[j++] = toLower(source[i]);
  }
  des[j] = 0;
}
// reads a cString upto maxSize characters or upto the delimiter character,
// whichever comes first (skipping leading white space characters but accpting
// spaces in the cString)
void Utils::read(char *cString, unsigned int maxSize, char delimiter) {
  char ch = 0;
  unsigned int i = 0;
  // skipping leading white space chars
  do {
    cin.get(ch); // reads one character from input
  } while (isSpace(ch));
  // read char by char util hitting delimiter or maxSize
  for (i = 0; i < maxSize && ch != delimiter; i++) {
    cString[i] = ch;
    // read the cString stopping at the size limit
    if (i < maxSize - 1)
      cin.get(ch);
  }
  cString[i] = 0; // make sure the cString is null terminated
}

int Utils::getDaysInMonth(int month, int year) {
  bool leapYear = (year % 4 == 0);
  int days;

  switch (month) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    days = 31;
    break;

  case 2:
    days = leapYear ? 29 : 28;
    break;

  case 4:
  case 6:
  case 9:
  case 11:
    days = 30;
    break;

  default:
    days = -1;
  }

  return days;
}

void Utils::extractChar(istream &istr, char ch) const {
  char next;
  next = istr.peek();

  if (ch == next) {
    istr.ignore();
  } else {
    istr.ignore(1000, ch);
  }
}

bool Utils::extractChar(ifstream &istr, char ch) const {
  bool success = true;
  char next;
  next = istr.peek();

  if (ch == next) {
    istr.ignore();
  } else if ((ch = -1)) {
    success = false;
  } else {
    istr.ignore(1000, ch);
  }

  return success;
}

double Utils::getdouble(const char *prompt) {
  char userInput[max_string];
  bool validUserInput = false;
  double returnDouble = 0;
  /*
   If the prompt is not null, it will display it.
   Then it performs a foolproof entry of an integer. If the user enters an
   invalid integer it will display " Invalid Integer, retry: " and will not let
   the user leave the function until a valid integer is entered.

  */

  if (prompt != nullptr) { // display and prompt for user input
    cout << prompt;
  }

  do {
    cin >> userInput;

    // validate if userInt is an int
    if (isDouble(userInput)) {
      validUserInput = true;
      returnDouble = atof(userInput); // set returnInt to userInt
    } else                            // invalid userInt
    {
      cout << "Invalid number, retry: ";
    }

  } while (!validUserInput);

  return returnDouble;
}

double Utils::getdouble(double min, double max, const char *prompt,
                        const char *errMes) {
  /*
   If the prompt is not null it will be displayed before getting a valid
   integer.(reuse the previous getint()) If the entered integer is not within
   the acceptable range (min and max) then an error message is displayed and it
   will not let the user leave the function until an acceptable value is
   entered. When printing the error message, if the errMes argument is not null
   it will be displayed followed by ", retry: " otherwise the general error
   message "Value out of range [min<=val<=max]: " is displayed (replacing min
   and max with their values)

   */

  double returnedUserDouble;
  bool exitWhile = false;

  do {
    returnedUserDouble = getdouble(prompt);

    if (returnedUserDouble >= min &&
        returnedUserDouble <= max) // indicates valid returnedUserInt
    {
      // CHECK if min or max are inlusive or not?
      exitWhile = true; // exit loop
    } else              // invalid returnedUserInt range
    {
      if (errMes != nullptr) {
        cout << errMes << ", retry: "; // CORRECT??

      } else // errMes == nullptr
      {
        //"Value out of range [min<=val<=max]: " is displayed (replacing min and
        //max with their values)
        cout << "Value out of range [" << fixed << setprecision(2) << min
             << "<=val<=" << fixed << setprecision(2) << max
             << "]: "; // CHECK output
      }
    }
  } while (!exitWhile);

  return returnedUserDouble;
}
} // namespace sdds
