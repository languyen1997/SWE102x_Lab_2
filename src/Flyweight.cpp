#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

struct SharedState {
  std::string brand_;
  std::string model_;
  std::string color_;

  SharedState(std::string brand, std::string model, std::string color)
      : brand_(std::move(brand)),
        model_(std::move(model)),
        color_(std::move(color)) {}

  friend std::ostream &operator<<(std::ostream &out_stream,
                                  SharedState const &shared_state) {
    return out_stream << "[ " << shared_state.brand_ << " , "
                      << shared_state.model_ << " , " << shared_state.color_
                      << " ]";
  }
};

struct UniqueState {
  std::string owner_;
  std::string plates_;

  UniqueState(std::string owner, std::string plates)
      : owner_(std::move(owner)), plates_(std::move(plates)) {}

  friend std::ostream &operator<<(std::ostream &out_stream,
                                  UniqueState const &unique_state) {
    return out_stream << "[ " << unique_state.owner_ << " , "
                      << unique_state.plates_ << " ]";
  }
};

class Flyweight {
 private:
  std::unique_ptr<SharedState> shared_state_;

 public:
  explicit Flyweight(SharedState const *shared_state)
      : shared_state_(new SharedState(*shared_state)) {}
  Flyweight(Flyweight const &other)
      : shared_state_(new SharedState(*other.shared_state_)) {}

  [[nodiscard]] SharedState *GetSharedState() const {
    return shared_state_.get();
  }

  void Operation(UniqueState const &unique_state) const {
    std::cout << "Flyweight: Displaying shared (" << *shared_state_
              << ") and unique (" << unique_state << ") state.\n";
  }
};

class FlyweightFactory {
 private:
  std::unordered_map<std::string, Flyweight> flyweights_;

  std::string GetKey(SharedState const &shared_state) const {
    return shared_state.brand_ + "_" + shared_state.model_ + "_" +
           shared_state.color_;
  }

 public:
  FlyweightFactory(std::initializer_list<SharedState> shared_states) {
    for (auto const &shared_state : shared_states) {
      this->flyweights_.insert(
          std::make_pair(this->GetKey(shared_state), Flyweight{&shared_state}));
    }
  }

  Flyweight GetFlyweight(SharedState const &shared_state) {
    std::string key = this->GetKey(shared_state);
    if (this->flyweights_.find(key) == this->flyweights_.end()) {
      std::cout
          << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
      this->flyweights_.insert(std::make_pair(key, Flyweight{&shared_state}));
    } else {
      std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
    }

    return this->flyweights_.at(key);
  }

  void ListFlyweights() const {
    size_t count = this->flyweights_.size();
    std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
    for (auto const &pair : this->flyweights_) {
      std::cout << pair.first << "\n";
    }
  }
};

void AddCarToPoliceDatabase(FlyweightFactory &factory,
                            std::string const &plates, std::string const &owner,
                            std::string const &brand, std::string const &model,
                            std::string const &color) {
  std::cout << "\nClient: Adding a car to database.\n";
  Flyweight const &flyweight = factory.GetFlyweight({brand, model, color});
  flyweight.Operation({owner, plates});
}

int main() {
  FlyweightFactory factory({
      {"Chevrolet", "Camaro2018", "pink"},
      {"Mercedes Benz", "C300", "black"},
      {"Mercedes Benz", "C500", "red"},
      {"BMW", "M5", "red"},
      {"BMW", "X6", "white"},
  });
  factory.ListFlyweights();

  AddCarToPoliceDatabase(factory, "CL234IR", "James Doe", "BMW", "M5", "red");
  AddCarToPoliceDatabase(factory, "CL234IR", "James Doe", "BMW", "X1", "red");
  factory.ListFlyweights();
}