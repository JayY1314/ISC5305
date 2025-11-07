#ifndef POLYNOMIAL_BINARY_TREE_H
#define POLYNOMIAL_BINARY_TREE_H

#include "Node.h"
#include <string>
#include <vector>
#include <memory>
#include <iterator>
#include <stack>

struct Term {
    double coeff;
    int exponent;
    
    Term(double c, int exp) : coeff(c), exponent(exp) {}
};

class PolynomialIterator {
private:
    std::vector<std::shared_ptr<Node>> nodes;
    size_t current;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Node;
    using difference_type = std::ptrdiff_t;
    using pointer = std::shared_ptr<Node>*;
    using reference = std::shared_ptr<Node>&;

    PolynomialIterator() : current(0) {}
    PolynomialIterator(const std::vector<std::shared_ptr<Node>>& n) : nodes(n), current(0) {}
    PolynomialIterator(const std::vector<std::shared_ptr<Node>>& n, size_t pos) : nodes(n), current(pos) {}

    reference operator*() {
        return nodes[current];
    }

    pointer operator->() {
        return &nodes[current];
    }

    PolynomialIterator& operator++() {
        if (current < nodes.size()) {
            ++current;
        }
        return *this;
    }

    PolynomialIterator operator++(int) {
        PolynomialIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const PolynomialIterator& other) const {
        return current == other.current && nodes.size() == other.nodes.size();
    }

    bool operator!=(const PolynomialIterator& other) const {
        return !(*this == other);
    }
};

class PolynomialConstIterator {
private:
    std::vector<std::shared_ptr<Node>> nodes;
    size_t current;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const Node;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::shared_ptr<Node>*;
    using reference = const std::shared_ptr<Node>&;

    PolynomialConstIterator() : current(0) {}
    PolynomialConstIterator(const std::vector<std::shared_ptr<Node>>& n) : nodes(n), current(0) {}
    PolynomialConstIterator(const std::vector<std::shared_ptr<Node>>& n, size_t pos) : nodes(n), current(pos) {}

    reference operator*() const {
        return nodes[current];
    }

    pointer operator->() const {
        return &nodes[current];
    }

    PolynomialConstIterator& operator++() {
        if (current < nodes.size()) {
            ++current;
        }
        return *this;
    }

    PolynomialConstIterator operator++(int) {
        PolynomialConstIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const PolynomialConstIterator& other) const {
        return current == other.current && nodes.size() == other.nodes.size();
    }

    bool operator!=(const PolynomialConstIterator& other) const {
        return !(*this == other);
    }
};

class PolynomialBinaryTree {
private:
    std::shared_ptr<Node> root;

    double evaluateNode(std::shared_ptr<Node> node, double x) const;
    std::string toStringNode(std::shared_ptr<Node> node) const;
    void inorderTraversal(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>>& result) const;
    void preorderTraversal(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>>& result) const;
    std::shared_ptr<Node> copyTree(std::shared_ptr<Node> node) const;
    PolynomialBinaryTree simplify() const;
    std::shared_ptr<Node> simplifyNode(std::shared_ptr<Node> node) const;
    bool isZero(std::shared_ptr<Node> node) const;
    bool isConstant(std::shared_ptr<Node> node) const;
    double getConstantValue(std::shared_ptr<Node> node) const;

public:
    PolynomialBinaryTree();
    PolynomialBinaryTree(std::shared_ptr<Node> root);
    PolynomialBinaryTree(const std::string& expr);
    PolynomialBinaryTree(double constant);
    PolynomialBinaryTree(const PolynomialBinaryTree& other);
    PolynomialBinaryTree& operator=(const PolynomialBinaryTree& other);

    double evaluate(double x) const;
    std::string to_string() const;
    
    std::vector<std::shared_ptr<Node>> inorder() const;
    std::vector<std::shared_ptr<Node>> preorder() const;

    PolynomialBinaryTree operator+(const PolynomialBinaryTree& other) const;
    PolynomialBinaryTree operator-(const PolynomialBinaryTree& other) const;
    PolynomialBinaryTree operator*(const PolynomialBinaryTree& other) const;

    PolynomialBinaryTree differentiate() const;
    std::shared_ptr<Node> differentiateNode(std::shared_ptr<Node> node) const;

    std::shared_ptr<Node> getRoot() const { return root; }

    using value_type = Node;
    using iterator = PolynomialIterator;
    using const_iterator = PolynomialConstIterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
};

#endif // POLYNOMIAL_BINARY_TREE_H

