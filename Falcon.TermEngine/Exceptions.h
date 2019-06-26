#pragma once

namespace Exceptions
{
    class OutOfRangeException :
        public std::exception
    {
    public:
        OutOfRangeException(const std::string& message)
            : exception(message.c_str())
        {}
    };

    class FileNotFoundException :
        public std::exception
    {
    public:
        FileNotFoundException(const std::string& message)
            : exception(message.c_str())
        {}
    };
}
