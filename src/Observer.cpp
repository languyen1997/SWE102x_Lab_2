#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>

class IObserver {
 public:
  virtual ~IObserver() = default;
  virtual void Update(std::string const &message_from_subject) = 0;
};

class ISubject {
 public:
  virtual ~ISubject() = default;
  virtual void Attach(IObserver *observer) = 0;
  virtual void Detach(IObserver *observer) = 0;
  virtual void Notify() = 0;
};

class Subject : public ISubject {
  std::list<IObserver *> observers_;
  std::string message_;

 public:
  ~Subject() override { std::cout << "Goodbye, I was the Subject.\n"; }

  void Attach(IObserver *observer) override { observers_.push_back(observer); }
  void Detach(IObserver *observer) override { observers_.remove(observer); }
  void Notify() override {
    auto iter = observers_.begin();
    HowManyObserver();
    while (iter != observers_.end()) {
      (*iter)->Update(message_);
      ++iter;
    }
  }

  void CreateMessage(std::string message = "Empty") {
    this->message_ = std::move(message);
    Notify();
  }

  void HowManyObserver() {
    std::cout << "There are " << observers_.size()
              << " observers in the list.\n";
  }

  void SomeBusinessLogic() {
    this->message_ = "change message message";
    Notify();
    std::cout << "I'm about to do some thing important\n";
  }
};

class Observer : public IObserver {
  std::string message_from_subject_;
  Subject &subject_;
  static int static_number_;
  int number_;

 public:
  explicit Observer(Subject &subject) : subject_(subject) {
    this->subject_.Attach(this);
    std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_
              << "\".\n";
    this->number_ = Observer::static_number_;
  }
  ~Observer() override {
    std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
  }

  void Update(std::string const &message_from_subject) override {
    message_from_subject_ = message_from_subject;
    PrintInfo();
  }

  void RemoveMeFromTheList() {
    subject_.Detach(this);
    std::cout << "Observer \"" << this->number_
              << "\" removed from the list.\n";
  }

  void PrintInfo() {
    std::cout << "Observer \"" << this->number_
              << "\": a new message is available --> "
              << this->message_from_subject_ << "\n";
  }
};

int Observer::static_number_ = 0;

void ClientCode() {
  auto subject = std::make_unique<Subject>();
  auto observer1 = std::make_unique<Observer>(*subject);
  auto observer2 = std::make_unique<Observer>(*subject);
  auto observer3 = std::make_unique<Observer>(*subject);

  subject->CreateMessage("Hello World! :D");
  observer3->RemoveMeFromTheList();

  subject->CreateMessage("The weather is hot today! :p");
  auto observer4 = std::make_unique<Observer>(*subject);

  observer2->RemoveMeFromTheList();
  auto observer5 = std::make_unique<Observer>(*subject);

  subject->CreateMessage("My new car is great! ;)");
  observer5->RemoveMeFromTheList();

  observer4->RemoveMeFromTheList();
  observer1->RemoveMeFromTheList();
}

int main() { ClientCode(); }
