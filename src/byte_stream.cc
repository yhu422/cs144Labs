#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : 
  capacity_( capacity ),
  available_capacity_(capacity),
  bytes_pushed_(0),
  bytes_popped_(0),
  closed_(false),
  stream_("")
  {
  }

bool Writer::is_closed() const
{
  // Your code here.
  return this->closed_;
}

void Writer::push( string data )
{
  // Your code here.
  uint64_t actual_push = min(this->available_capacity(), data.size());
  this->available_capacity_ -= actual_push;
  stream_ += data.substr(0, actual_push);
  this->bytes_pushed_ += actual_push;
  return;
}

void Writer::close()
{
  this->closed_ = true;// Your code here.
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return this->available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return this->bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return this->closed_ && this->available_capacity_ == this->capacity_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return this->bytes_popped_;
}

string_view Reader::peek() const
{
  string_view sv(stream_);
  return sv;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (len > this->bytes_buffered()){
    this->set_error();
  }
  this->available_capacity_ += len;
  this->stream_ = stream_.substr(len);
  this->bytes_popped_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return this->capacity_ - this->available_capacity_;
}
