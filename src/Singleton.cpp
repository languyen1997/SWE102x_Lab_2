#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

class Singleton {
 private:
  static std::unique_ptr<Singleton> singleton_;
  static std::mutex mutex_;

 protected:
  explicit Singleton(std::string value) : value_(std::move(value)) {}
  std::string value_;

 public:
  Singleton(Singleton &other) = delete;
  void operator=(Singleton const &) = delete;

  static std::unique_ptr<Singleton> GetInstance(std::string const &value);

  [[nodiscard]] std::string Value() const { return value_; }
};

std::unique_ptr<Singleton> Singleton::singleton_{nullptr};
std::mutex Singleton::mutex_;

std::unique_ptr<Singleton> Singleton::GetInstance(std::string const &value) {
  std::lock_guard<std::mutex> lock{mutex_};
  if (singleton_ == nullptr) {
    singleton_ = std::unique_ptr<Singleton>(new Singleton(value));
  }

  return std::move(singleton_);
}

void ThreadFoo() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto singleton = Singleton::GetInstance("FOO");
  std::cout << singleton->Value() << "\n";
}

void ThreadBar() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto singleton = Singleton::GetInstance("BAR");
  std::cout << singleton->Value() << "\n";
}

int main() {
  std::cout << "If you see the same value, then singleton was reused (yay!\n"
            << "If you see different values, then 2 singletons were created "
               "(booo!!)\n\n"
            << "RESULT:\n";
  std::thread th1(ThreadFoo);
  std::thread th2(ThreadBar);
  th1.join();
  th2.join();
}
