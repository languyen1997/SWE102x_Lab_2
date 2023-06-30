#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Memento {
 public:
  virtual ~Memento() = default;
  [[nodiscard]] virtual std::string GetName() const = 0;
  [[nodiscard]] virtual std::string GetDate() const = 0;
  [[nodiscard]] virtual std::string GetState() const = 0;
};

class ConcreteMemento : public Memento {
  std::string state_;
  std::string date_;

 public:
  explicit ConcreteMemento(std::string state) : state_(std::move(state)) {
    std::time_t now = std::time(nullptr);
    this->date_ = std::ctime(&now);
  }

  [[nodiscard]] std::string GetState() const override { return this->state_; }
  [[nodiscard]] std::string GetName() const override {
    return this->date_ + " / (" + this->state_.substr(0, 9) + "...)";
  }
  [[nodiscard]] std::string GetDate() const override { return this->date_; }
};

class Originator {
  std::string state_;

  std::string GenerateRandomString(int length = 10) {
    char const alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    int string_length = sizeof(alphanum) - 1;
    std::string random_string;
    for (int i = 0; i < length; ++i) {
      random_string += alphanum[std::rand() % string_length];
    }
    return random_string;
  }

 public:
  explicit Originator(std::string state) : state_(std::move(state)) {
    std::cout << "Originator: My initial state is: " << this->state_ << "\n";
  }

  void DoSomething() {
    std::cout << "Originator: I'm doing something important.\n";
    this->state_ = this->GenerateRandomString(30);
    std::cout << "Originator: and my state has changed to: " << this->state_
              << "\n";
  }

  Memento *Save() { return new ConcreteMemento(this->state_); }

  void Restore(Memento *memento) {
    this->state_ = memento->GetState();
    std::cout << "Originator: My state has changed to: " << this->state_
              << "\n";
  }
};

class Caretaker {
  std::vector<std::shared_ptr<Memento>> mementos_;
  Originator *originator_;

 public:
  explicit Caretaker(Originator *originator) : originator_(originator) {}

  void Backup() {
    std::cout << "\nCaretaker: Saving Originator's state...\n";
    this->mementos_.push_back(
        std::shared_ptr<Memento>(this->originator_->Save()));
  }

  void Undo() {
    if (this->mementos_.empty()) {
      return;
    }

    std::shared_ptr<Memento> memento = this->mementos_.back();
    this->mementos_.pop_back();
    std::cout << "Caretaker: Restoring state to: " << memento->GetName()
              << "\n";
    try {
      this->originator_->Restore(memento.get());
    } catch (...) {
      this->Undo();
    }
  }

  void ShowHistory() const {
    std::cout << "Caretaker: Here's the list of mementos:\n";
    for (auto const &memento : this->mementos_) {
      std::cout << memento->GetName() << "\n";
    }
  }
};

void ClientCode() {
  auto originator =
      std::make_unique<Originator>("Super-duper-super-puper-super.");
  auto caretaker = std::make_unique<Caretaker>(originator.get());
  caretaker->Backup();
  originator->DoSomething();
  caretaker->Backup();
  originator->DoSomething();
  caretaker->Backup();
  originator->DoSomething();
  std::cout << "\n";
  caretaker->ShowHistory();
  std::cout << "\nClient: Now, let's rollback!\n\n";
  caretaker->Undo();
  std::cout << "\nClient: Once more!\n\n";
  caretaker->Undo();
}

int main() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  ClientCode();
}
