#ifndef COMM_H
#define COMM_H

#define CHECK_RET(ret, err_msg) \
    if(ret < 0){ \
        printf("%s, ret: %d\n", err_msg, ret); \
    }

#define CHECK_RET_NOT_ZERO(ret, err_msg) \
    if( ret < 0 ){ \
        printf("%s\n", err_msg); \
    }

#endif
