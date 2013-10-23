#pragma once

#include <exception>

class NotImplementedException : public std::exception {};
class SocketConnectionException : public std::exception {};