#ifndef SDDS_STATUS_H
#define SDDS_STATUS_H

#include <iostream>

namespace sdds {

class Status {
  char *m_statusDescriptString = nullptr;
  int m_statusCode = 0;

public:
  Status();
  Status(char *statusDescript);
  void setSafe();
  void setStatusObject(const char *statusString, const int statusCode);
  const char *getStatusString() const;
  int getStatusCode() const;

  Status(const Status &copyFromStatus);            // copy constructor
  Status &operator=(const Status &copyFromStatus); // copy operator
  ~Status();                                       // destructor
  Status &operator=(const char *statusString);     // assignment operator string
  Status &operator=(const int statusCode);         // assignment operator int
  operator bool() const;                           // bool operator
  operator const char *() const; // check conversion operator overload
  operator int() const;          // check conversion operator overload
  Status &clear();
};

// ostr << m_state;
std::ostream &operator<<(std::ostream &ostream, const Status &statusObj);

} // namespace sdds

#endif
