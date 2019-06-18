/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_QUEUE_H
#define QGE_QUEUE_H

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>

namespace quasi_game_engine {

// This has been put on the back burner while I figure out how to do it properly

// Raw pointers were choosen to prevent a need for default constructors
template <class T, int MAX_SIZE = 20>
class QgeQueue {
 public:
  QgeQueue() { data_ = reinterpret_cast<T*>(malloc(sizeof(T) * MAX_SIZE)); }
  ~QgeQueue() = default;

  // Getting rid of copy/move constructors/assignment operators (may need later)
  QgeQueue(const QgeQueue& old_queue) {
    this->queue_pos_ = old_queue.queue_pos_;
    this->queue_count_ = old_queue.queue_count_;
    this->data_ = reinterpret_cast<T*>(malloc(sizeof(T) * MAX_SIZE));
    memcpy(this->data_, old_queue.data_, sizeof(T) * MAX_SIZE);
  };
  QgeQueue& operator=(const QgeQueue&) = delete;
  QgeQueue(QgeQueue&&) = delete;
  QgeQueue& operator=(QgeQueue&&) = delete;

  bool IsEmpty() { return !queue_count_; }
  int Size() { return queue_count_; }

  void Push(const T data) {
    assert(queue_count_ < MAX_SIZE);
    std::lock_guard<std::mutex> guard(queue_mutex_);
    *(data_ + ((queue_pos_ + queue_count_++) % MAX_SIZE)) = data;
  }

  T Pop() {
    assert(queue_count_ > 0);
    std::lock_guard<std::mutex> guard(queue_mutex_);
    --queue_count_;
    return *(data_ + (queue_pos_++ % MAX_SIZE));
  }

 private:
  std::mutex queue_mutex_;
  int queue_pos_ = 0;
  int queue_count_ = 0;
  T* data_;
};

}  // namespace quasi_game_engine

#endif  // QGE_QUEUE_H
