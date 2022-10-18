#ifndef RETURN_CODE_HPP_INCLUDED
#define RETURN_CODE_HPP_INCLUDED









#define try(x) {\
\
    Return_code _return_code_try_special = x;\
    if (_return_code_try_special) { LOG_ERROR (_return_code_try_special); return _return_code_try_special; }\
}


enum  Return_code  {

    SUCCESS    = 0,
    MEMORY_ERR = 1,
    BAD_ARGS   = 2,
    FILE_ERR   = 3,
};









#endif