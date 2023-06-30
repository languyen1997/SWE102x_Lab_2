#include <iostream>
#include <string>
#include <vector>

template <typename T, typename U>
class Iterator {
 public:
  using iter_type = typename std::vector<T>::iterator;

 private:
  U *m_p_data_;
  iter_type m_it_;

 public:
  explicit Iterator(U *p_data, bool reverse = false)
      : m_p_data_(p_data), m_it_(m_p_data_->m_data_.begin()) {
    (void)reverse;
  }

  void First() { m_it_ = m_p_data_->m_data_.begin(); }
  void Next() { m_it_++; }
  bool IsDone() { return (m_it_ == m_p_data_->m_data_.end()); }

  iter_type Current() { return m_it_; }
};

template <class T>
class Container {
  friend class Iterator<T, Container>;

 public:
  void Add(T item) { m_data_.push_back(item); }

  Iterator<T, Container> *CreateIterator() {
    return new Iterator<T, Container>(this);
  }

 private:
  std::vector<T> m_data_;
};

class Data {
 public:
  explicit Data(int item = 0) : m_data_(item) {}

  void SetData(int item) { m_data_ = item; }

  [[nodiscard]] int GetData() const { return m_data_; }

 private:
  int m_data_;
};

void ClientCode() {
  {
    std::cout << "Iterator with int" << std::endl;
    Container<int> cont;

    for (int i = 0; i < 10; ++i) {
      cont.Add(i);
    }

    Iterator<int, Container<int>> *iter = cont.CreateIterator();
    for (iter->First(); !iter->IsDone(); iter->Next()) {
      std::cout << *iter->Current() << std::endl;
    }
  }

  {
    std::cout << "Iterator with custom class" << std::endl;
    Container<Data> cont;
    Data item_a(100);
    Data item_b(1000);
    Data item_c(10000);
    cont.Add(item_a);
    cont.Add(item_b);
    cont.Add(item_c);

    Iterator<Data, Container<Data>> *iter = cont.CreateIterator();
    for (iter->First(); !iter->IsDone(); iter->Next()) {
      std::cout << iter->Current()->GetData() << std::endl;
    }
  }
}

int main() { ClientCode(); }
