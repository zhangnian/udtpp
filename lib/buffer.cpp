#include "buffer.h"


#include <algorithm>
#include <vector>

#include <assert.h>
#include <string.h>

#include "udt.h"

Buffer::Buffer(size_t size)
    : buffer_(size), read_idx_(0), write_idx_(0)
{
    assert( 0 == ReadableBytes() );
    assert( size = WritableBytes() );
}


size_t Buffer::ReadableBytes() const
{
    return write_idx_ - read_idx_;
}

size_t Buffer::WritableBytes() const
{
    return buffer_.size() - write_idx_;
}

char* Buffer::WritePos()
{
    return &(*buffer_.begin()) + write_idx_;
}


void Buffer::Append(char* data, size_t len)
{
    if( NULL == data || 0 == len )
        return;

    std::copy(data, data + len, WritePos());
    write_idx_ += len;
    return;
}


void Buffer::IncrSpace(size_t len)
{
    if( read_idx_ + WritableBytes() < len )
    {
        buffer_.resize(write_idx_ + len);
    }
    else
    {
        size_t readable = ReadableBytes();
        std::copy(&*buffer_.begin() + read_idx_, &*buffer_.begin() + write_idx_, &*buffer_.begin());
        read_idx_ = 0;
        write_idx_ = readable;
        assert( readable == ReadableBytes() );
    }
}


void Buffer::Retrieve(size_t len)
{
    assert( len <= ReadableBytes() );
    if( len < ReadableBytes() )
    {
        read_idx_ += len;
    }
    else
    {
        RetrieveAll();
    }
}

void Buffer::RetrieveAll()
{
    read_idx_ = 0;
    write_idx_ = 0;
}


std::string Buffer::RetrieveAsString(size_t len)
{
    assert( len <= ReadableBytes() );
    std::string str(Peek(), len);
    Retrieve(len);
    return str;
}

std::string Buffer::RetrieveAllAsString()
{
    return RetrieveAsString(ReadableBytes());
}

const char* Buffer::Peek()
{
    return &*buffer_.begin() + read_idx_;
}


size_t Buffer::ReadFd(int fd)
{
    assert( fd > 0 );
    char buf[1024 * 4] = { 0 };
    int ret = UDT::recv(fd, buf, sizeof(buf), 0);
    if( ret > 0 )
    {
        Append(buf, ret);
    }

    return ret ;
}
