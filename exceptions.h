#ifndef WET1_EXCEPTIONS_H
#define WET1_EXCEPTIONS_H
#include <exception>
#include <new>


class Exception: public std::exception{};
class NodeAlreadyExists: public Exception{};
class NodeDoesntExists: public Exception{};

#endif //WET1_EXCEPTIONS_H
