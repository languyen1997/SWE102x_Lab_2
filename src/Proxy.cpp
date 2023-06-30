#include <iostream>
#include <memory>

class Subject {
 public:
  virtual void Request() const = 0;
};

class RealSubject : public Subject {
 public:
  void Request() const override {
    std::cout << "RealSubject: Handling request.\n";
  }
};

class Proxy : public Subject {
 private:
  std::unique_ptr<RealSubject> real_subject_;

  [[nodiscard]] bool CheckAccess() const {
    std::cout << "Proxy: Checking access prior to firing a real request.\n";
    return true;
  }

  void LogAccess() const {
    std::cout << "Proxy: Logging the time of request.\n";
  }

 public:
  explicit Proxy(RealSubject *real_subject)
      : real_subject_(new RealSubject(*real_subject)) {}

  void Request() const override {
    if (this->CheckAccess()) {
      this->real_subject_->Request();
      this->LogAccess();
    }
  }
};

void ClientCode(Subject const &subject) { subject.Request(); }

int main() {
  std::cout << "Client: Executing the client code with a real subject:\n";
  RealSubject real_subject;
  ClientCode(real_subject);
  std::cout << "\n";
  std::cout << "Client: Executing the same client code with a proxy:\n";
  Proxy proxy{&real_subject};
  ClientCode(proxy);
}