#pragma once

#include <exception>

class NotImplementedException : public std::exception {};
class SocketConnectionExeption : public std::exception {};