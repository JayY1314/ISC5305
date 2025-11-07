#include "PolynomialBinaryTree.h"
#include <sstream>
#include <stdexcept>
#include <cmath>

PolynomialBinaryTree::PolynomialBinaryTree() : root(nullptr) {
}

PolynomialBinaryTree::PolynomialBinaryTree(std::shared_ptr<Node> root) : root(root) {
}

PolynomialBinaryTree::PolynomialBinaryTree(const std::string& expr) {
    if (expr == "x") {
        root = std::make_shared<Node>('x');
    } else {
        throw std::runtime_error("Simple string constructor only supports 'x'");
    }
}

PolynomialBinaryTree::PolynomialBinaryTree(double constant) {
    root = std::make_shared<Node>(constant);
}

PolynomialBinaryTree::PolynomialBinaryTree(const PolynomialBinaryTree& other) {
    root = copyTree(other.root);
}

PolynomialBinaryTree& PolynomialBinaryTree::operator=(const PolynomialBinaryTree& other) {
    if (this != &other) {
        root = copyTree(other.root);
    }
    return *this;
}

std::shared_ptr<Node> PolynomialBinaryTree::copyTree(std::shared_ptr<Node> node) const {
    if (!node) {
        return nullptr;
    }
    
    std::shared_ptr<Node> newNode;
    if (node->type == NodeType::Constant) {
        newNode = std::make_shared<Node>(node->getConstant());
    } else if (node->type == NodeType::Variable) {
        newNode = std::make_shared<Node>(node->getVariable());
    } else {
        newNode = std::make_shared<Node>(
            node->getOperator(),
            copyTree(node->getLeft()),
            copyTree(node->getRight())
        );
    }
    return newNode;
}

double PolynomialBinaryTree::evaluate(double x) const {
    return evaluateNode(root, x);
}

double PolynomialBinaryTree::evaluateNode(std::shared_ptr<Node> node, double x) const {
    if (!node) {
        return 0.0;
    }

    switch (node->type) {
        case NodeType::Constant:
            return node->getConstant();
        case NodeType::Variable:
            return x;
        case NodeType::Operator: {
            char op = node->getOperator();
            double leftVal = evaluateNode(node->getLeft(), x);
            double rightVal = evaluateNode(node->getRight(), x);
            
            switch (op) {
                case '+':
                    return leftVal + rightVal;
                case '-':
                    return leftVal - rightVal;
                case '*':
                    return leftVal * rightVal;
                default:
                    throw std::runtime_error("Unsupported operator");
            }
        }
        default:
            return 0.0;
    }
}

std::string PolynomialBinaryTree::to_string() const {
    return toStringNode(root);
}

std::string PolynomialBinaryTree::toStringNode(std::shared_ptr<Node> node) const {
    if (!node) {
        return "";
    }
    return node->to_string();
}

std::vector<std::shared_ptr<Node>> PolynomialBinaryTree::inorder() const {
    std::vector<std::shared_ptr<Node>> result;
    inorderTraversal(root, result);
    return result;
}

void PolynomialBinaryTree::inorderTraversal(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>>& result) const {
    if (!node) {
        return;
    }
    inorderTraversal(node->getLeft(), result);
    result.push_back(node);
    inorderTraversal(node->getRight(), result);
}

std::vector<std::shared_ptr<Node>> PolynomialBinaryTree::preorder() const {
    std::vector<std::shared_ptr<Node>> result;
    preorderTraversal(root, result);
    return result;
}

void PolynomialBinaryTree::preorderTraversal(std::shared_ptr<Node> node, std::vector<std::shared_ptr<Node>>& result) const {
    if (!node) {
        return;
    }
    result.push_back(node);
    preorderTraversal(node->getLeft(), result);
    preorderTraversal(node->getRight(), result);
}

PolynomialBinaryTree PolynomialBinaryTree::operator+(const PolynomialBinaryTree& other) const {
    std::shared_ptr<Node> newRoot = std::make_shared<Node>('+', root, other.root);
    return PolynomialBinaryTree(newRoot);
}

PolynomialBinaryTree PolynomialBinaryTree::operator-(const PolynomialBinaryTree& other) const {
    std::shared_ptr<Node> newRoot = std::make_shared<Node>('-', root, other.root);
    return PolynomialBinaryTree(newRoot);
}

PolynomialBinaryTree PolynomialBinaryTree::operator*(const PolynomialBinaryTree& other) const {
    std::shared_ptr<Node> newRoot = std::make_shared<Node>('*', root, other.root);
    return PolynomialBinaryTree(newRoot);
}

bool PolynomialBinaryTree::isZero(std::shared_ptr<Node> node) const {
    if (!node) return false;
    return node->type == NodeType::Constant && std::abs(node->getConstant()) < 1e-10;
}

bool PolynomialBinaryTree::isConstant(std::shared_ptr<Node> node) const {
    if (!node) return false;
    if (node->type == NodeType::Constant) return true;
    if (node->type == NodeType::Operator) {
        return isConstant(node->getLeft()) && isConstant(node->getRight());
    }
    return false;
}

double PolynomialBinaryTree::getConstantValue(std::shared_ptr<Node> node) const {
    if (!node) return 0.0;
    if (node->type == NodeType::Constant) {
        return node->getConstant();
    }
    if (node->type == NodeType::Operator) {
        char op = node->getOperator();
        double leftVal = getConstantValue(node->getLeft());
        double rightVal = getConstantValue(node->getRight());
        switch (op) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            default: return 0.0;
        }
    }
    return 0.0;
}

std::shared_ptr<Node> PolynomialBinaryTree::simplifyNode(std::shared_ptr<Node> node) const {
    if (!node) {
        return nullptr;
    }

    if (node->type == NodeType::Constant || node->type == NodeType::Variable) {
        return node;
    }

    char op = node->getOperator();
    std::shared_ptr<Node> left = simplifyNode(node->getLeft());
    std::shared_ptr<Node> right = simplifyNode(node->getRight());

    bool leftZero = isZero(left);
    bool rightZero = isZero(right);
    bool leftConst = isConstant(left);
    bool rightConst = isConstant(right);

    switch (op) {
        case '+':
            if (leftZero) return right;
            if (rightZero) return left;
            if (leftConst && rightConst) {
                double val = getConstantValue(left) + getConstantValue(right);
                return std::make_shared<Node>(val);
            }
            break;
        case '-':
            if (leftZero) {
                if (rightConst) {
                    double val = -getConstantValue(right);
                    return std::make_shared<Node>(val);
                }
                return std::make_shared<Node>('*', std::make_shared<Node>(-1.0), right);
            }
            if (rightZero) return left;
            if (leftConst && rightConst) {
                double val = getConstantValue(left) - getConstantValue(right);
                return std::make_shared<Node>(val);
            }
            break;
        case '*':
            if (leftZero || rightZero) {
                return std::make_shared<Node>(0.0);
            }
            if (leftConst && rightConst) {
                double val = getConstantValue(left) * getConstantValue(right);
                return std::make_shared<Node>(val);
            }
            break;
    }

    return std::make_shared<Node>(op, left, right);
}

PolynomialBinaryTree PolynomialBinaryTree::simplify() const {
    return PolynomialBinaryTree(simplifyNode(root));
}

PolynomialBinaryTree PolynomialBinaryTree::differentiate() const {
    std::shared_ptr<Node> diffRoot = differentiateNode(root);
    PolynomialBinaryTree result(diffRoot);
    return result.simplify();
}

std::shared_ptr<Node> PolynomialBinaryTree::differentiateNode(std::shared_ptr<Node> node) const {
    if (!node) {
        return std::make_shared<Node>(0.0);
    }

    switch (node->type) {
        case NodeType::Constant:
            return std::make_shared<Node>(0.0);
        case NodeType::Variable:
            return std::make_shared<Node>(1.0);
        case NodeType::Operator: {
            char op = node->getOperator();
            std::shared_ptr<Node> left = node->getLeft();
            std::shared_ptr<Node> right = node->getRight();

            switch (op) {
                case '+':
                case '-': {
                    std::shared_ptr<Node> leftDiff = differentiateNode(left);
                    std::shared_ptr<Node> rightDiff = differentiateNode(right);
                    return std::make_shared<Node>(op, leftDiff, rightDiff);
                }
                case '*': {
                    std::shared_ptr<Node> leftDiff = differentiateNode(left);
                    std::shared_ptr<Node> rightDiff = differentiateNode(right);
                    std::shared_ptr<Node> term1 = std::make_shared<Node>('*', leftDiff, right);
                    std::shared_ptr<Node> term2 = std::make_shared<Node>('*', left, rightDiff);
                    return std::make_shared<Node>('+', term1, term2);
                }
                default:
                    throw std::runtime_error("Unsupported operator for differentiation");
            }
        }
        default:
            return std::make_shared<Node>(0.0);
    }
}

PolynomialBinaryTree::iterator PolynomialBinaryTree::begin() {
    std::vector<std::shared_ptr<Node>> nodes;
    inorderTraversal(root, nodes);
    return PolynomialIterator(nodes, 0);
}

PolynomialBinaryTree::iterator PolynomialBinaryTree::end() {
    std::vector<std::shared_ptr<Node>> nodes;
    inorderTraversal(root, nodes);
    return PolynomialIterator(nodes, nodes.size());
}

PolynomialBinaryTree::const_iterator PolynomialBinaryTree::begin() const {
    std::vector<std::shared_ptr<Node>> nodes;
    inorderTraversal(root, nodes);
    return PolynomialConstIterator(nodes, 0);
}

PolynomialBinaryTree::const_iterator PolynomialBinaryTree::end() const {
    std::vector<std::shared_ptr<Node>> nodes;
    inorderTraversal(root, nodes);
    return PolynomialConstIterator(nodes, nodes.size());
}

PolynomialBinaryTree::const_iterator PolynomialBinaryTree::cbegin() const {
    return begin();
}

PolynomialBinaryTree::const_iterator PolynomialBinaryTree::cend() const {
    return end();
}

