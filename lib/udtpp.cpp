#include "udtpp.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <netdb.h>

#include <iostream>
#include <string>
#include <set>

#include "comm.h"



UDTServer::UDTServer(std::string port)
    : svr_socket_(-1), epfd_(-1), port_(port)
{
    UDT::startup();
}

UDTServer::~UDTServer()
{
    UDT::cleanup();
}

bool UDTServer::Start()
{
    std::cout << "server start..." << std::endl;

    bool ret = InitServer();
    CHECK_RET_NOT_ZERO(ret, "CreateTcpServer error");

    std::set<UDTSOCKET> readfds;
    while(true)
    {
        UDT::epoll_wait(epfd_, &readfds, NULL, -1, NULL, NULL);
        for (std::set<UDTSOCKET>::iterator iter = readfds.begin(); iter != readfds.end(); ++iter)
        {
            UDTSOCKET active_fd = *iter;
            if( active_fd == svr_socket_ )
            {
                printf("[event]OnConnection, fd: %d\n", active_fd);
                OnConnection();
            }
            else
            {
                printf("[event]OnMessage, fd: %d\n", active_fd);
                ReadRequest(active_fd);
            }
        }
    }


    return true;
}


void UDTServer::SetNonBlocking(UDTSOCKET fd)
{
    bool blocking = false;
    UDT::setsockopt(fd, 0, UDT_SNDSYN, &blocking, sizeof(blocking));
    UDT::setsockopt(fd, 0, UDT_RCVSYN, &blocking, sizeof(blocking));
}


int UDTServer::InitServer()
{
    // 创建epoll
    epfd_ = UDT::epoll_create();
    assert( epfd_ > 0 );

    // 创建监听socket
    addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* res;
    if (0 != getaddrinfo(NULL, port_.c_str(), &hints, &res))
    {
        std::cout << "illegal port number or port is busy." << std::endl;
        return -1;
    }

    svr_socket_ = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    assert( svr_socket_ > 0 );
    SetNonBlocking(svr_socket_);

    if (UDT::ERROR == UDT::bind(svr_socket_, res->ai_addr, res->ai_addrlen))
    {
        std::cout << "bind error: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    freeaddrinfo(res);
    if (UDT::ERROR == UDT::listen(svr_socket_, 10))
    {
        std::cout << "listen failed, error: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return -1;
    }

    int ret = UDT::epoll_add_usock(epfd_, svr_socket_);
    CHECK_RET_NOT_ZERO(ret, "UDT::epoll_add_usock error");

    return 0;
}


void UDTServer::OnConnection()
{
    UDTSOCKET cli_fd = UDT::accept(svr_socket_, NULL, NULL);
    if (cli_fd == UDT::INVALID_SOCK)
    {
        std::cout << "accept: " << UDT::getlasterror().getErrorMessage() << std::endl;
        return;
    }

    SetNonBlocking(cli_fd);

    int ret = UDT::epoll_add_usock(epfd_, cli_fd);
    CHECK_RET_NOT_ZERO(ret, "UDT::epoll_add_usock error");
}


void UDTServer::ReadRequest(UDTSOCKET cli_fd)
{
    Buffer* buf = new Buffer(1024);
    int ret = buf->ReadFd(cli_fd);
    printf("[ReadRequest]ret: %d, buf: %p\n", ret, buf);
    if( ret == UDT::ERROR )
    {
        if( UDT::getlasterror().getErrorCode() != 6002 )
        {
            UDT::epoll_remove_usock(epfd_, cli_fd);
            UDT::close(cli_fd);
        }
        return;
    }

    if( 0 == ret )
    {
        std::cout << "connection closed by peer" << std::endl;
        UDT::epoll_remove_usock(epfd_, cli_fd);
        UDT::close(cli_fd);
        return;
    }

    OnMessage(buf);
    return;
}

void UDTServer::OnMessage(Buffer* buf_ptr)
{
    return;
}
