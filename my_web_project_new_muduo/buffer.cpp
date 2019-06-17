#include "buffer.h"

//fist [IN]: 要拷贝元素的首地址
//last [IN]:要拷贝元素的最后一个元素的下一个地址
//x [OUT] : 拷贝的目的地的首地址

//template<class InIt, class OutIt>
//    OutIt copy(InIt first, InIt last, OutIt x);


int buffer::readFd(int fd)
{
    char extrabuffer[65536];
    ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

    struct iovec iov[2];
    iov[0].iov_base = begin() + writeindex_;
    iov[0].iov_len = writableBytes();
    iov[1].iov_base = extrabuffer;
    iov[0].iov_len = sizeof(extrabuffer);

    int n = 0;
    if ((n = readv(fd, iov, 2)) > 0) {
        if (n <= writableBytes())
            writeindex_ += n;
        else {
            writeindex_ += writableBytes();
            append(extrabuffer, n - writableBytes());
        }
    }
    return n;
}

void buffer::rechange(int len)
{
    if(len < readableBytes()) {
        readindex_ += len;
    } else {
        readindex_ = writeindex_ = 0;
    }
}

//这是朱的代码
//void retrieve(size_t len)
//{
//    if (len < readable())
//        _readindex += len;
//    else
//        _readindex = _writeindex = 0;
//}
