#ifndef SDDS_DATE_H
#define SDDS_DATE_H

#include "Status.h"

namespace sdds {
class Date {
  int mYear;
  int mMonth;
  int mDay;
  Status mStatus;
  bool mFormatted = true;
  bool validate();

public:
  Date();
  Date(int year, int month, int day);
  int getUniqueDate();
  bool operator==(Date &other);
  bool operator!=(Date &other);
  bool operator<(Date &other);
  bool operator<=(Date &other);
  bool operator>(Date &other);
  bool operator>=(Date &other);
  const Status &state();
  Date &formatted(bool format);
  operator bool() const;
  bool isFormatted();
  int getYear() { return mYear; }
  int getMonth() { return mMonth; }
  int getDay() { return mDay; }
  std::ostream &write(std::ostream &ostr);
  std::istream &read(std::istream &istr);
  void setSafe();
  Date &set(int year, int month, int day);
};

std::ostream &operator<<(std::ostream &ostr, Date ob);
std::ostream &operator<<(Date &ob, std::ostream &ostr);
char *operator<<(const char *str, Date &ob);
std::istream &operator>>(std::istream &istr, Date &ob);

} // namespace sdds

#endif // !SDDS_DATE_H
