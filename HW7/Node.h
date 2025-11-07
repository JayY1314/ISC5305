#ifndef NODE_H
#define NODE_H

#include <variant>
#include <memory>
#include <string>

enum class NodeType {
    Constant,
    Variable,
    Operator
};

class Node {
public:
    NodeType type;
    std::variant<double, char> value;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(double constant);
    Node(char variable);
    Node(char op, std::shared_ptr<Node> left, std::shared_ptr<Node> right);

    NodeType getType() const;
    double getConstant() const;
    char getVariable() const;
    char getOperator() const;
    std::shared_ptr<Node> getLeft() const;
    std::shared_ptr<Node> getRight() const;

    std::string to_string() const;
};

#endif // NODE_H

