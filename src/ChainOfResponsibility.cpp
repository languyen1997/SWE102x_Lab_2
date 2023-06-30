#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Handler {
 public:
  virtual Handler *SetNext(Handler *handler) = 0;
  virtual std::string Handle(std::string request) = 0;
};

class AbstractHandler : public Handler {
 private:
  Handler *next_handler_{};

 public:
  AbstractHandler() = default;

  Handler *SetNext(Handler *handler) override {
    this->next_handler_ = handler;
    return handler;
  }

  std::string Handle(std::string request) override {
    if (this->next_handler_) {
      return this->next_handler_->Handle(request);
    }

    return {};
  }
};

class MonkeyHandler : public AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "Banana") {
      return "Monkey: I'll eat the " + request + ".\n";
    }

    return AbstractHandler::Handle(request);
  }
};

class SquirrelHandler : public AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "Nut") {
      return "Squirrel: I'll eat the " + request + ".\n";
    }

    return AbstractHandler::Handle(request);
  }
};

class DogHandler : public AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "MeatBall") {
      return "Dog: I'll eat the " + request + ".\n";
    }

    return AbstractHandler::Handle(request);
  }
};

void ClientCode(Handler &handler) {
  std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};
  for (auto const &item : food) {
    std::cout << "Client: Who wants a " << item << "?\n";
    const std::string result = handler.Handle(item);
    if (!result.empty()) {
      std::cout << "  " << result;
    } else {
      std::cout << "  " << item << " was left untouched.\n";
    }
  }
}

int main() {
  auto monkey = std::make_unique<MonkeyHandler>();
  auto squirrel = std::make_unique<SquirrelHandler>();
  auto dog = std::make_unique<DogHandler>();
  monkey->SetNext(squirrel.get())->SetNext(dog.get());

  std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
  ClientCode(*monkey);
  std::cout << "\n";
  std::cout << "Subchain: Squirrel > Dog\n\n";
  ClientCode(*squirrel);
}
