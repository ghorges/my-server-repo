#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <sys/uio.h>
#include <string>
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size

class buffer
{
public:
    buffer():buffer_(1024),writeindex_(0),readindex_(0){

    }

    int readableBytes() const
    {
        return writeindex_ - readindex_;
    }

    size_t writableBytes() const
    {
        buffer_.size() - writeindex_;
    }

    char* writeindex_data()
    {
        return begin() + writeindex_;
    }

    int prependableBytes() const
    {
        return readindex_;
    }

    char* begin()
    {
        return &*buffer_.begin();
    }

    char *peek() { return begin() + readindex_; }

    void append(const char *data,int len) {
        makespace(len);
        buffer_.insert(buffer_.begin() + writeindex_,data,data + len);
        writeindex_ += len;
    }

    void makespace(int len) {
        if (writableBytes() + prependableBytes() < len) {
            buffer_.resize(buffer_.size() + len);
        } else {
            int reads = readableBytes();
            std::copy(buffer_.begin() + readindex_,
                      buffer_.begin() + writeindex_,
                      buffer_.begin()
                      );
            readindex_ = 0;
            writeindex_ = reads;
        }
    }

    char* c_str()
    {
        static const std::string end_char("\0");
        append(end_char.c_str(),1);
        writeindex_--;
        return peek();
    }

    int readFd(int fd);

    void rechange(int len);


private:
    std::vector<char> buffer_;
    int writeindex_;
    int readindex_;
};

#endif // BUFFER_H
