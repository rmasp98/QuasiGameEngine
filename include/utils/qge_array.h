/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#ifndef QGE_ARRAY_H
#define QGE_ARRAY_H

#include "utils/logging/logger.h"

#include <cassert>


namespace quasi_game_engine {

template<class T>
class QgeArray {
/*------------------------------------------------------------------------------
  Stnadard static array array class
Note
- Consider making it dynamic?
- Currently does not have a logger and only has assertions
------------------------------------------------------------------------------*/
 public:
  QgeArray() {};
  QgeArray(int size) {
    assert(size > 0);
    data_ = new T[size]();
    size_ = size;
  }

  //These are bog standard copy/assignment operators. This will need to be
  //updated when I have my own allocators
  QgeArray(const QgeArray& in) {
    this->size_ = in.size_;
    this->num_components_ = in.num_components_;
    this->data_ = new T[this->size_];
    std::copy(in.data_, in.data_ + in.size_, this->data_);
  };

  QgeArray& operator=(const QgeArray& in) {
    this->size_ = in.size_;
    this->num_components_ = in.num_components_;
    this->data_ = new T[this->size_];
    std::copy(in.data_, in.data_ + in.size_, this->data_);
    return *this;
  };

  QgeArray(QgeArray&& in) {
    this->size_ = in.size_;
    this->num_components_ = in.num_components_;
    this->data_ = new T[this->size_];
    std::copy(in.data_, in.data_ + in.size_, this->data_);
  };

  QgeArray& operator=(QgeArray&& in) {
    this->size_ = in.size_;
    this->num_components_ = in.num_components_;
    this->data_ = new T[this->size_];
    std::copy(in.data_, in.data_ + in.size_, this->data_);
    return *this;
  };

  ~QgeArray() {
    //Needs to change this to custom deconstructor
    delete data_;
  }

  T& operator[](int index) const {
    assert(index < size_ && index >= 0);
    return data_[index];
  }

  int Size() const { return size_; }

  void SetNumComponents(int num_components) { num_components_ = num_components; }
  int GetNumComponents() const { return num_components_; }

 private:
  T* data_;
  int size_;
  int num_components_;
};

}  // namespace quasi_game_engine

#endif  // QGE_RESOURCE_BASE_H
