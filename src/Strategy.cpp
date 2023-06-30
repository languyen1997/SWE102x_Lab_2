#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Strategy {
 public:
  virtual ~Strategy() = default;
  [[nodiscard]] virtual std::string DoAlgorithm(
      std::string_view data) const = 0;
};

class Context {
  std::unique_ptr<Strategy> strategy_;

 public:
  explicit Context(std::unique_ptr<Strategy> &&strategy = {})
      : strategy_(std::move(strategy)) {}

  void SetStrategy(std::unique_ptr<Strategy> &&strategy) {
    this->strategy_ = std::move(strategy);
  }

  void DoSomeBusinessLogic() const {
    if (strategy_) {
      std::cout << "Context: Sorting data using the strategy (not sure how "
                   "it'll do it)\n";
      std::string result = strategy_->DoAlgorithm("aecbd");
      std::cout << result << "\n";
    } else {
      std::cout << "Context: Strategy isn't set\n";
    }
  }
};

class ConcreteStrategyA : public Strategy {
 public:
  [[nodiscard]] std::string DoAlgorithm(std::string_view data) const override {
    std::string result{data};
    std::sort(result.begin(), result.end());
    return result;
  }
};

class ConcreteStrategyB : public Strategy {
 public:
  [[nodiscard]] std::string DoAlgorithm(std::string_view data) const override {
    std::string result{data};
    std::sort(result.begin(), result.end(), std::greater<>());
    return result;
  }
};

void ClientCode() {
  Context context{std::make_unique<ConcreteStrategyA>()};
  std::cout << "Client: Strategy is set no normal sorting.\n";
  context.DoSomeBusinessLogic();
  std::cout << "\n";
  std::cout << "Client: Strategy is set no reverse sorting.\n";
  context.SetStrategy(std::make_unique<ConcreteStrategyB>());
  context.DoSomeBusinessLogic();
}

int main() { ClientCode(); }
