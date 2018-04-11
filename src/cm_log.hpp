#ifndef __cm_log__h
#define __cm_log__h

#include <ostream>

class CMLog
{
private:
    CMLog() { }
    CMLog(const CMLog &) { }

public:
    static CMLog & instance()
    {
        static CMLog cm_logger;
        return cm_logger;
    }

    template <typename type_t>
    CMLog & operator<< (const type_t & data)
    {
        std::cout << data;
        return *this;
    }

    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    typedef CoutType& (*StandardEndLine)(CoutType&);

    CMLog& operator<<(StandardEndLine manip)
    {
        manip(std::cout);
        return *this;
    }

};

#define CM_LOG CMLog::instance()

#endif
