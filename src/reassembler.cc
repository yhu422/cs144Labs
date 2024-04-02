#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  string_view final_data(data);
  uint64_t last_index = first_index + data.size();
  if(is_last_substring){
    end_index_ = last_index;
  }
  if(data.size() == 0){
    if(next_index_ == end_index_){
      output_.writer().close();
    }
    return;
  }
  uint64_t first_unacceptable_index = next_index_ + writer().available_capacity();

  if(last_index <= next_index_ || first_index >= first_unacceptable_index){
    return;
  }
  if(first_index < next_index_){
    final_data = final_data.substr(next_index_ - first_index);
    first_index = next_index_;
  }
  if(last_index > first_unacceptable_index){
    final_data = final_data.substr(0, first_unacceptable_index - first_index);
    last_index = first_unacceptable_index;
  }
  /* Then, remove any part that is overlapping already in the internal storage*/
  for(auto kv = internal_storage_.cbegin(); kv != internal_storage_.cend();){
    pair<uint64_t, uint64_t> key = kv->first;
    //New substring is completely included.
    if(final_data.size() == 0){
      return;
    }
    //Have not reached overlapping segments
    if(key.second <= first_index){
      ++kv;
      continue;
    }
    //We move past the possible overlapping segments.
    if(key.first >= last_index){
      break;
    }
    //New substring is completely included in another segment. Guaranteed no insertion
    if(key.first <= first_index && key.second >= last_index){
      return;
    }
    if(key.first <= first_index && key.second < last_index){
      final_data = final_data.substr(key.second-first_index);
      first_index = key.second;
    }
    else if(key.first > first_index && key.second >= last_index){
      final_data = final_data.substr(0, key.first - first_index);
      last_index = key.first;
    }
    else if(key.first > first_index && key.second < last_index){
        bytes_pending_ = bytes_pending_ - (key.second - key.first);
        internal_storage_.erase(kv++);
        continue;
    }
    ++kv;
  }
  // if(final_data.size() > writer().available_capacity() - bytes_pending()){
  //   final_data = final_data.substr(0, writer().available_capacity() - bytes_pending());
  //   last_index = first_index + final_data.size();
  // }
  std::string str(final_data);
  internal_storage_.insert(std::make_pair(std::make_pair(first_index, last_index), str));
  bytes_pending_ += str.size();
  for(auto kv = internal_storage_.cbegin(); kv != internal_storage_.cend();){
    pair<uint64_t, uint64_t> key = kv->first;
    if(key.first == next_index_){
      next_index_ = key.second;
      output_.writer().push(kv->second);
      bytes_pending_ -= kv->second.size();
      internal_storage_.erase(kv++);
    }else{
      ++kv;
    }
  }
  if(next_index_ == end_index_){
    output_.writer().close();
  }
  return;
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return this->bytes_pending_;
}
