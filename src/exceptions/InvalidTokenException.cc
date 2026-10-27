#include <stdexcept>

class InvalidTokenException : public std::runtime_error
{
public:
  // Pass the message to the base class constructor
  explicit InvalidTokenException (const std::string& message)
      : std::runtime_error (message)
  {
  }
};