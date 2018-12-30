/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_QUEUE_H
#define QGE_QUEUE_H

#include <mutex>

#define MAX_QUEUE_SIZE 20

namespace quasi_game_engine {

template<class T>
class QgeQueue {
/*------------------------------------------------------------------------------
  QGE queue class. This is a THINGY queue so can be stored locally 
Note
- 
------------------------------------------------------------------------------*/
 public:
  QgeQueue() : queue_count_(0), queue_pos_(0) {};
  ~QgeQueue() = default;

  //Getting rid of copy/move constructors/assignment operators (may need later)
  QgeQueue(const QgeQueue&) = delete;
  QgeQueue& operator=(const QgeQueue& old_queue) = delete;
  QgeQueue(QgeQueue&&) = delete;
  QgeQueue& operator=(QgeQueue&&) = delete;

  void Push(T new_element) { 
    if (queue_count_ < MAX_QUEUE_SIZE) {
      std::lock_guard<std::mutex> guard(queue_mutex_);
      queue_[(queue_pos_ + queue_count_++) % MAX_QUEUE_SIZE] = new_element;
    } else {
      //TODO: Send proper error
      throw;
    }
  };

  T Pop() {
    if (!Empty()) {
      std::lock_guard<std::mutex> guard(queue_mutex_);
      --queue_count_;
      return queue_[queue_pos_++ % MAX_QUEUE_SIZE];
    }
    //TODO: Send proper error
    throw;
  };

  bool Empty() { return !queue_count_; };
  int Size() { return queue_count_; };

 private:
  T queue_[MAX_QUEUE_SIZE];
  int queue_count_;
  int queue_pos_;
  std::mutex queue_mutex_;

};

}  // namespace quasi_game_engine

#endif  // QGE_QUEUE_H
