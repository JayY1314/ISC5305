#include "Node.h"
#include <sstream>
#include <stdexcept>

Node::Node(double constant) 
    : type(NodeType::Constant), value(constant), left(nullptr), right(nullptr) {
}

Node::Node(char variable) 
    : type(NodeType::Variable), value(variable), left(nullptr), right(nullptr) {
}

Node::Node(char op, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
    : type(NodeType::Operator), value(op), left(left), right(right) {
}

NodeType Node::getType() const {
    return type;
}

double Node::getConstant() const {
    if (type != NodeType::Constant) {
        throw std::runtime_error("Node is not a constant");
    }
    return std::get<double>(value);
}

char Node::getVariable() const {
    if (type != NodeType::Variable) {
        throw std::runtime_error("Node is not a variable");
    }
    return std::get<char>(value);
}

char Node::getOperator() const {
    if (type != NodeType::Operator) {
        throw std::runtime_error("Node is not an operator");
    }
    return std::get<char>(value);
}

std::shared_ptr<Node> Node::getLeft() const {
    return left;
}

std::shared_ptr<Node> Node::getRight() const {
    return right;
}

std::string Node::to_string() const {
    switch (type) {
        case NodeType::Constant: {
            std::ostringstream oss;
            oss << std::get<double>(value);
            return oss.str();
        }
        case NodeType::Variable:
            return std::string(1, std::get<char>(value));
        case NodeType::Operator: {
            char op = std::get<char>(value);
            std::string leftStr = left ? left->to_string() : "";
            std::string rightStr = right ? right->to_string() : "";
            return "(" + leftStr + " " + op + " " + rightStr + ")";
        }
        default:
            return "";
    }
}

