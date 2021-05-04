#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const auto syn = seg.header().syn;
    const auto fin = seg.header().fin;

    if (syn && _connected)
        return;
        
    // 接收到连接请求
    if (syn && !_connected)
    {
        _connected = true;
        _isn = seg.header().seqno;

    }

    if (_connected)
    {
        // const auto seqno = syn ? seg.header().seqno + 1 : seg.header().seqno;
        // const auto abs_seqno = unwrap(seg.header().seqno, _isn, _reassembler.first_unassembled());
        // syn (... data) fin
        // 5  6 7 8      9   isn == 5   (seqno)
        // 0 1 2 3       4   (abs_seqno)
        // 0 0 1 2       3   (stream_index)
        // 处理的目的即是让isn对应的index为0
        const auto abs_seqno = unwrap(seg.header().seqno, _isn, _reassembler.first_unassembled());
        const auto index = syn ? abs_seqno : abs_seqno - 1;
        const auto data = seg.payload().copy();
        _reassembler.push_substring(data, index, false);
    }

    // 连接未建立却来了其他含有fin的seg会影响逻辑
    if (!_connected && fin)
        return;

    if (fin)
    {
        // _connected = false;
        _fin = true;
        const auto seqno = seg.header().seqno + seg.length_in_sequence_space() - 1;  // FIN的位置
        const auto abs_seqno = unwrap(seqno, _isn, _reassembler.first_unassembled());
        _reassembler.push_substring("", abs_seqno - 1, true);
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const 
{ 
    // 只有syn 和 fin
    // 5 6  (seqno, isn == 5)
    // 0 1  (absno)
    // 0 0  (stream_index) 
    // 但是这个时候应该返回 ack 7

    // 也有可能fin 提前到了，被存了起来
    size_t shift = (_fin && _reassembler.unassembled_bytes() == 0) ? 2 : 1;
    if (_connected)
        return wrap(_reassembler.first_unassembled()+shift, _isn);
    
    return {};
}

size_t TCPReceiver::window_size() const { return _capacity - stream_out().buffer_size(); }
