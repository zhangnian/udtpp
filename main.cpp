#include <iostream>

#include "udtpp.h"

class TransferSvr : public UDTServer
{
public:
    TransferSvr(std::string port)
        : UDTServer(port)
    {

    }
    void OnMessage(Buffer* buf_ptr)
    {
        printf("[TransferSvr::OnMessage]buf: %p\n", buf_ptr);
        std::string msg = buf_ptr->RetrieveAllAsString();
        std::cout << "recv: " << msg << std::endl;
    }
};



int main(int argc, char* argv[])
{
    std::string port = "8090";
    TransferSvr svr(port);
    svr.Start();

    return 0;
}
