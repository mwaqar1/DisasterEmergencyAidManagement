#define _CRT_SECURE_NO_WARNINGS
#include "Status.h"
#include "Utils.h"
#include <iostream>
#include <string.h>

using namespace std;

namespace sdds {

const char *Status::getStatusString() const { return m_statusDescriptString; }

int Status::getStatusCode() const { return m_statusCode; }

void Status::setSafe() {
  if (m_statusDescriptString != nullptr) {
    delete[] m_statusDescriptString;
  }
  m_statusDescriptString = nullptr;
  m_statusCode = 0;
}

Status::Status() { setSafe(); }

Status::Status(char *statusDescriptString) {

  if (statusDescriptString != nullptr) {
    setSafe();
    ut.alocpy(m_statusDescriptString, statusDescriptString);
  }
}

void Status::setStatusObject(const char *statusString, const int statusCode) {

  if (statusString !=
      nullptr) // Check if there  is validation for status members?
  {
    setSafe();
    ut.alocpy(m_statusDescriptString, statusString);

    m_statusCode = statusCode;
  }
}

Status::Status(const Status &copyFromStatus) { *this = copyFromStatus; }

Status &Status::operator=(const Status &copyFromStatus) {

  if (this != &copyFromStatus) // make sure current object is not being copied
                               // to the same location as copyFromStatus
  {
    setStatusObject(copyFromStatus.m_statusDescriptString,
                    copyFromStatus.m_statusCode);
  }

  return *this;
}

Status::~Status() {
  delete[] m_statusDescriptString;
  m_statusDescriptString = nullptr;
}

Status &
Status::operator=(const char *statusString) // send back copy or reference ?
{

  if (statusString != nullptr) {
    ut.alocpy(m_statusDescriptString, statusString);
  }

  return *this;
}

Status &Status::operator=(const int statusCode) // send back copy or reference ?
{
  m_statusCode = statusCode; // is there validation or dynamic memory
                             // allocation... unclear?

  return *this;
}

Status::operator bool() const {
  return (m_statusDescriptString == nullptr); // empty state is valid
}

Status &Status::clear() {
  setSafe();
  return *this;
}

Status::operator const char *()
    const // operator int() const; //check conversion operator overload
{
  return m_statusDescriptString;
}

Status::operator int() const // check conversion operator overload
{
  return m_statusCode;
}

ostream &operator<<(std::ostream &ostream, const Status &statusObj) {

  if (!statusObj && statusObj.getStatusCode() == 0) {
    ostream << statusObj.getStatusString();

  } else if (!statusObj &&
             statusObj.getStatusCode() !=
                 0) // print output should look like "ERR#1: value too high"
  {
    ostream << "ERR#" << statusObj.getStatusCode() << ": "
            << statusObj.getStatusString();
  }
  return ostream;
}

} // namespace sdds
