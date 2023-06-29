#include <iostream>
#include <list>
#include <memory>
#include <string>

#define UNUSED(x) ((void)x)

class Component {
 protected:
  Component *parent_;

 public:
  virtual ~Component() = default;

  void SetParent(Component *parent) { this->parent_ = parent; }
  [[nodiscard]] Component *GetParent() const { return this->parent_; }

  virtual void Add(Component *component) { UNUSED(component); }
  virtual void Remove(Component *component) { UNUSED(component); }
  [[nodiscard]] virtual bool IsComposite() const { return false; }
  [[nodiscard]] virtual std::string Operation() const = 0;
};

class Leaf : public Component {
 public:
  [[nodiscard]] std::string Operation() const override { return "Leaf"; }
};

class Composite : public Component {
 protected:
  std::list<Component *> children_;

 public:
  void Add(Component *component) override {
    this->children_.push_back(component);
    component->SetParent(this);
  }

  void Remove(Component *component) override {
    children_.remove(component);
    component->SetParent(nullptr);
  }

  [[nodiscard]] bool IsComposite() const override { return true; }

  [[nodiscard]] std::string Operation() const override {
    std::string result;
    for (auto const *component : children_) {
      if (component == children_.back()) {
        result += component->Operation();
      } else {
        result += component->Operation() + "+";
      }
    }

    return "Branch(" + result + ")";
  }
};

void ClientCode(Component *component) {
  std::cout << "RESULT: " << component->Operation();
}

void ClientCode2(Component *component1, Component *component2) {
  if (component1->IsComposite()) {
    component1->Add(component2);
  }

  std::cout << "RESULT: " << component1->Operation();
}

int main() {
  auto simple = std::make_unique<Leaf>();
  std::cout << "Client: I've got a simple component:\n";
  ClientCode(simple.get());
  std::cout << "\n\n";

  auto tree = std::make_unique<Composite>();
  auto branch1 = std::make_unique<Composite>();

  auto leaf_1 = std::make_unique<Leaf>();
  auto leaf_2 = std::make_unique<Leaf>();
  auto leaf_3 = std::make_unique<Leaf>();
  branch1->Add(leaf_1.get());
  branch1->Add(leaf_2.get());
  auto branch2 = std::make_unique<Composite>();
  branch2->Add(leaf_3.get());
  tree->Add(branch1.get());
  tree->Add(branch2.get());
  std::cout << "Client: Now I've got a composite tree:\n";
  ClientCode(tree.get());
  std::cout << "\n\n";

  std::cout << "Client: I don't need to check the components classes event "
               "when managing the tree:\n";
  ClientCode2(tree.get(), simple.get());
  std::cout << "\n";
}
