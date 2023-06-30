#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Command {
 public:
  virtual ~Command() = default;
  virtual void Execute() const = 0;
};

class SimpleCommand : public Command {
 private:
  std::string payload_;

 public:
  explicit SimpleCommand(std::string payload) : payload_(std::move(payload)) {}

  void Execute() const override {
    std::cout << "SimpleCommand: See, I can do simple things like printing ("
              << this->payload_ << ")\n";
  }
};

class Receiver {
 public:
  void DoSomething(std::string const &action_a) {
    std::cout << "Receiver: Working on (" << action_a << ")\n";
  }

  void DoSomethingElse(std::string const &action_b) {
    std::cout << "Receiver: Also working on (" << action_b << ")\n";
  }
};

class ComplexCommand : public Command {
 private:
  Receiver *receiver_;

  std::string action_a_;
  std::string action_b_;

 public:
  ComplexCommand(Receiver *receiver, std::string action_a, std::string action_b)
      : receiver_(receiver),
        action_a_(std::move(action_a)),
        action_b_(std::move(action_b)) {}

  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver "
                 "object.\n";
    this->receiver_->DoSomething(action_a_);
    this->receiver_->DoSomethingElse(action_b_);
  }
};

class Invoker {
 private:
  std::unique_ptr<Command> on_start_;
  std::unique_ptr<Command> on_finish_;

 public:
  void SetOnStart(std::unique_ptr<Command> command) {
    this->on_start_ = std::move(command);
  }
  void SetOnFinish(std::unique_ptr<Command> command) {
    this->on_finish_ = std::move(command);
  }

  void DoSomethingImportant() {
    std::cout << "Invoker: Does anybody want something done before I begin?\n";
    if (this->on_start_) {
      this->on_start_->Execute();
    }
    std::cout << "Invoker: ...doing something really important...\n";
    std::cout << "Invoker: Does anybody want something done after I finish?\n";
    if (this->on_finish_) {
      this->on_finish_->Execute();
    }
  }
};

int main() {
  auto invoker = std::make_unique<Invoker>();
  invoker->SetOnStart(std::make_unique<SimpleCommand>("Say Hi!"));
  auto receiver = std::make_unique<Receiver>();
  invoker->SetOnFinish(std::make_unique<ComplexCommand>(
      receiver.get(), "Send email", "Save report"));
  invoker->DoSomethingImportant();
}
