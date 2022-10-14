#ifndef RETURN_CODE_HPP_INCLUDED
#define RETURN_CODE_HPP_INCLUDED









#define try(x)\
\
    Return_code return_code = x;\
    if (return_code) { LOG_ERROR (return_code); return return_code; }


enum  Return_code  {

    SUCCESS    = 0,
    MEMORY_ERR = 1,
    BAD_ARGS   = 2,
    FILE_ERR   = 3,
};









#endif