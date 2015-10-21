#ifndef UDTPP_H
#define UDTPP_H

#include <string>

#include "udt.h"
#include "buffer.h"




class UDTServer
{
public:
    UDTServer(std::string port = "9000");
    virtual ~UDTServer();

    bool Start();

public:
    virtual void OnMessage(Buffer* buf_ptr);

private:
    void OnConnection();

    void ReadRequest(UDTSOCKET cli_fd);

    int InitServer();

    void SetNonBlocking(UDTSOCKET fd);


private:
    UDTSOCKET svr_socket_;
    int epfd_;
    std::string port_;
};


#endif
