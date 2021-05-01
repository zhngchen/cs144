#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity), _remain_capacity(capacity), _stream() {}

size_t ByteStream::write(const string &data) {
    const auto l = min(_remain_capacity, data.size());
    for (size_t i = 0; i < l; i++)
    {
        _stream.emplace_back(data[i]);
    }

    _write_count += l;
    _remain_capacity -= l;

    return l;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    return string(_stream.begin(), _stream.begin()+len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) 
{ 
    for(size_t i = 0; i < len; i++)
    {
        _stream.pop_front();
    }

    _remain_capacity += len;
    _read_count += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    const auto n = min(len, buffer_size());
    const auto res = peek_output(n);
    pop_output(n);

    return res;
}

void ByteStream::end_input() {_end = true;}

bool ByteStream::input_ended() const { return _end; }

size_t ByteStream::buffer_size() const { return _capacity - _remain_capacity; }

bool ByteStream::buffer_empty() const { return buffer_size() == 0;}

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _remain_capacity; }
