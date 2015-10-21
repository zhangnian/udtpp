#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>


/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size

class Buffer
{
    static const size_t kInitialSize = 1024;

public:
    Buffer(size_t size = kInitialSize);

    inline size_t ReadableBytes() const;

    inline size_t WritableBytes() const;

    void Append(char* data, size_t len);

    void Retrieve(size_t len);

    void RetrieveAll();

    std::string RetrieveAsString(size_t len);

    std::string RetrieveAllAsString();

    const char* Peek();

    size_t ReadFd(int fd);

private:
    char* WritePos();

    void IncrSpace(size_t len);


private:
    std::vector<char> buffer_;
    size_t read_idx_;
    size_t write_idx_;
};


#endif
