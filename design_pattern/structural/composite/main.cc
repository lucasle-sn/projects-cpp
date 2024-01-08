#include <algorithm>
#include <iostream>
#include <list>
#include <string>

/**
 * @brief Composite is a "structural design pattern" that lets you compose
 * objects into tree structures and then work with these structures as if they
 * were individual object.
 *
 * # Problem: Having 2 types of objects: "Products" and "Boxes". "Boxes" may
 * contain multi "Products". The expectation is to treat "Boxes" and "Products"
 * uniformly, e.g. <get_price()>.
 *
 * # Structure: refers to "composite_sttructure.png"
 *
 * # Applicability:
 *  + When you have to implement a tree-like object structure.
 *  + When you want to treat both simple and complex elements uniformly.
 * E.g. A tree has <branches> and <leaves>. <branches> has <brnaches> and
 * <leaves>. -> <branches> and <leaves> are treated equally.
 *
 * # Pros & cons:
 *  + Pros: - work with complex tree structures more conveniently, using
 *            polymophism and recursion.
 *          - align with "Open/Closed principle"
 *  + Cons: - be difficult to provide a common interface. e.g. <branches> &
 *            <leaves> are different.
 */

/**
 * @brief The base Component class declares common operations for both simple
 * and complex objects of a composition.
 */
class Component {
 public:
  /**
   * @brief Destructor
   */
  virtual ~Component() {}

  /**
   * @note In some cases, it would be beneficial to define the child-management
   * operations right in the base Component class. This way, you won't need to
   * expose any concrete component classes to the client code, even during the
   * object tree assembly. The downside is that these methods will be empty for
   * the leaf-level components. => CONS
   */
  virtual void Add(Component *component) {}
  virtual void Remove(Component *component) {}

  /**
   * @brief Check if object is composite
   */
  virtual bool IsComposite() const { return false; }

  /**
   * @note The base Component may implement some default behavior or leave it to
   * concrete classes (by declaring the method containing the behavior as
   * "abstract").
   */
  virtual std::string Operation() const = 0;

  /**
   * @brief Parent setter
   *
   * @param parent Parent
   */
  void SetParent(Component *parent) { this->parent_ = parent; }

  /**
   * @brief Parent getter
   *
   * @return Component*
   */
  Component *GetParent() const { return this->parent_; }

 protected:
  /**
   * @brief Parent component
   *
   * @note Optionally, the base Component can declare an interface for setting
   * and accessing a parent of the component in a tree structure. It can also
   * provide some default implementation for these methods.
   */
  Component *parent_;
};

/**
 * @brief The Leaf class represents the end objects of a composition. A leaf
 * can't have any children.
 *
 * Usually, it's the Leaf objects that do the actual work, whereas Composite
 * objects only delegate to their sub-components.
 */
class Leaf : public Component {
 public:
  /**
   * @brief Override the implementation of operation()
   *
   * @return std::string
   */
  std::string Operation() const override { return "Leaf"; }
};

/**
 * @brief The Composite class represents the complex components that may have
 * children.
 *
 * Usually, the Composite objects delegate the actual work to their children and
 * then "sum-up" the result.
 */
class Composite : public Component {
 public:
  /**
   * @brief Add child to composite
   *
   * @note A composite object can add or remove other components (both simple or
   * complex) to or from its child list.
   */
  void Add(Component *component) override {
    this->children_.push_back(component);
    component->SetParent(this);
  }

  /**
   * @brief Remove child from the composite
   *
   * @note Have in mind that this method removes the pointer to the list but
   * doesn't frees the memory, you should do it manually or better use smart
   * pointers.
   */
  void Remove(Component *component) override {
    children_.remove(component);
    component->SetParent(nullptr);
  }

  /**
   * @brief Check if the object is composite
   *
   * @return bool
   */
  bool IsComposite() const override { return true; }

  /**
   * @note The Composite executes its primary logic in a particular way. It
   * traverses recursively through all its children, collecting and summing
   * their results. Since the composite's children pass these calls to their
   * children and so forth, the whole object tree is traversed as a result.
   */
  std::string Operation() const override {
    std::string result;
    for (const Component *c : children_) {
      if (c == children_.back()) {
        result += c->Operation();
      } else {
        result += c->Operation() + "+";
      }
    }
    return "Branch(" + result + ")";
  }

 protected:
  /**
   * @brief List of children
   */
  std::list<Component *> children_;
};

/**
 * @brief The client code works with all of the components via the base
 * interface.
 */
void ClientCode(Component *component) {
  // ...
  std::cout << "RESULT: " << component->Operation();
  // ...
}

int main() {
  /**
   * Simple component (leaf)
   */
  Component *simple = new Leaf;
  std::cout << "Client: I've got a simple component:\n";
  ClientCode(simple);
  std::cout << "\n\n";

  /**
   * Complex composites.
   */
  Component *leaf_1 = new Leaf;
  Component *leaf_2 = new Leaf;
  Component *leaf_3 = new Leaf;

  Component *branch1 = new Composite;
  branch1->Add(leaf_1);
  branch1->Add(leaf_2);

  Component *branch2 = new Composite;
  branch2->Add(leaf_3);

  Component *tree = new Composite;
  tree->Add(branch1);
  tree->Add(branch2);

  std::cout << "Client: Now I've got a composite tree:\n";
  ClientCode(tree);
  std::cout << "\n\n";

  delete simple;
  delete tree;
  delete branch1;
  delete branch2;
  delete leaf_1;
  delete leaf_2;
  delete leaf_3;

  return 0;
}
