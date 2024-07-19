#include "Date.h"
#include "Utils.h"
#include <iomanip>

using namespace std;

namespace sdds {
const int max_year = 2030;

bool Date::validate() {
  if (mYear < sdds_testYear || mYear > max_year) {
    mStatus = "Invalid year in date";
    mStatus = 1;
  } else if (mMonth < 1 || mMonth > 12) {
    mStatus = "Invalid month in date";
    mStatus = 2;
  } else if (mDay < 1 || mDay > ut.getDaysInMonth(mMonth, mYear)) {
    mStatus = "Invalid day in date";
    mStatus = 3;
  } else {
    mStatus.clear();
  }

  return mStatus;
}

int Date::getUniqueDate() { return (mYear * 372) + (mMonth * 31) + mDay; }

Date::Date() { ut.getSystemDate(&mYear, &mMonth, &mDay); }

Date::Date(int year, int month, int day) { set(year, month, day); }

bool Date::operator==(Date &other) {
  return (this->getUniqueDate() == other.getUniqueDate());
}

bool Date::operator!=(Date &other) { return !(*this == other); }

bool Date::operator<(Date &other) {
  return (this->getUniqueDate() < other.getUniqueDate());
}

bool Date::operator>=(Date &other) { return !(*this < other); }

bool Date::operator>(Date &other) {
  return (this->getUniqueDate() > other.getUniqueDate());
}

bool Date::operator<=(Date &other) { return !(*this > other); }

const Status &Date::state() { return mStatus; }

Date &Date::formatted(bool format) {
  mFormatted = format;
  return *this;
}

Date::operator bool() const { return (bool)mStatus; }

bool Date::isFormatted() { return mFormatted; }

ostream &operator<<(ostream &ostream, Date ob) {
  if (ob.isFormatted()) {
    ostream << ob.getYear() << "/";
    ostream << setw(2) << setfill('0') << ob.getMonth() << "/";
    ostream << setw(2) << setfill('0') << ob.getDay();
  } else {
    ostream << ob.getYear() % 100;
    ostream << setw(2) << setfill('0') << ob.getMonth();
    ostream << setw(2) << setfill('0') << ob.getDay();
  }

  return ostream;
}

ostream &operator<<(Date &ob, ostream &ostream) {
  if (ob.isFormatted()) {
    ostream << ob.getYear() << "/";
    ostream << ostream.width(2) << ob.getMonth() << "/";
    ostream << ostream.width(2) << ob.getDay();
  } else {
    ostream << ostream.width(2) << ob.getYear() % 100;
    ostream << ostream.width(2) << ob.getMonth();
    ostream << ostream.width(2) << ob.getDay();
  }

  return ostream;
}

ostream &Date::write(ostream &ostr) {
  ostr << *this;
  return ostr;
}

istream &operator>>(istream &istr, Date &ob) {
  int tmpDate;
  int day;
  int month;
  int year;
  istr >> tmpDate;

  if (tmpDate == 0) {
    ob.setSafe();
    istr.setstate(ios::badbit);
    cout << "Invalid date value";
  } else if (tmpDate < 10000) {
    // we have 4 or less digits
    day = tmpDate % 100;
    month = (tmpDate - day) / 100;
    ob.set(2022, month, day);
    if (!ob) {
      istr.setstate(ios::badbit);
    }
  } else if (tmpDate > 99999 && tmpDate < 1000000) {
    // we have exactly 6 digits
    day = tmpDate % 100;
    tmpDate = (tmpDate - day) / 100;
    month = tmpDate % 100;
    year = 2000 + ((tmpDate - month) / 100);
    ob.set(year, month, day);
    if (!ob) {
      istr.setstate(ios::badbit);
    }
  } else {
    ob.setSafe();
    if (!ob) {
      istr.setstate(ios::badbit);
    }
  }

  return istr;
}

istream &Date::read(istream &istr) {
  istr >> *this;
  return istr;
}

void Date::setSafe() {
  mYear = sdds_testYear;
  mMonth = sdds_testMon;
  mDay = sdds_testDay;
}

Date &Date::set(int year, int month, int day) {
  setSafe();

  mYear = year;
  mMonth = month;
  mDay = day;

  if (!validate()) {
    mYear = sdds_testYear;
    mMonth = sdds_testMon;
    mDay = sdds_testDay;
  }

  return *this;
}
} // namespace sdds
