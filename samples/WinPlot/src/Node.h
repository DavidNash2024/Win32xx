/////////////////////////////
// Node.h:
//

#ifndef _CF6AD5B7_507F_4DE4_8980_A3940530D108_NODE_H
#define _CF6AD5B7_507F_4DE4_8980_A3940530D108_NODE_H

#include <iostream>

namespace Calc
{

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( push )
#pragma warning ( disable : 26812 )       // enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

    ///////////////////////////////////////////////////////////
    //"Node" is the parent class of all nodes
    // The member function "Calc" is a pure virtual function,
    // This makes "Node" and abstract class, so no instances of
    // can be made of this class.  Classes inherited from
    // this must implement the Calc function.
    class Node
    {
    public:
        Node() {}
        virtual double Calc() const = 0;
        virtual ~Node() {}
    private:
        Node& operator=(const Node&);       // Disable copy constructor
    };

    /////////////////////////////////////////////////////
    // a node type which holds a simple number, but not a
    //  variable or constant from the SymbolTable
    class Node_Number : public Node
    {
    public:
        Node_Number(double Number);
        virtual double Calc() const;
        virtual ~Node_Number() {}

    private:
        const double m_number;
    };

    ////////////////////////////////////////////////////
    // A branch node has one or more leaf nodes.
    // All leaves will have the same precedence,
    // so a branch will combine add with subtract, or
    // multiply with divide, but not add with multiply.
    // Each leaf will in turn be either a branch node,
    // a number node, a variable node or a function node.
    class Node_Branch : public Node
    {
    public:
        Node_Branch(Node * pNode)
        {
            //branch is constructed with it's first leaf
            m_leaves.push_back(pNode);
            m_tokens.push_back(tPlus);
        }
        virtual ~Node_Branch();
        void AddLeaf(Node * pNode, eToken Token)
        {
            m_leaves.push_back(pNode);
            m_tokens.push_back(Token);
        }
        virtual double Calc() const;

    private:
        std::vector<Node*>  m_leaves;
        std::vector<eToken> m_tokens;
    };

    /////////////////////////////////////////
    // retrieves values from the symbol table
    class Node_Variable : public Node
    {
    public:
        Node_Variable(SymbolTable& symTab, const CString& symbol);
        virtual ~Node_Variable() {}
        virtual double Calc() const;

    private:
        const SymbolTable& m_symTab;
        CString m_symbol;
    };

    ///////////////////////////////////////////////////
    // Assigns the value of an expression to a variable
    class Node_Assign : public Node
    {
    public:
        Node_Assign(Node* pNode, SymbolTable& symTab, const CString& symbol);
        virtual ~Node_Assign();
        virtual double Calc() const;

    private:
        Node* m_node;
        SymbolTable& m_symTab;
        CString m_symbol;
    };

    //////////////////////////////////////////////////////////////
    // calls a math function on the expression pointed to by pnode
    class Node_Function : public Node
    {
    public:
        Node_Function(CString funName, Node* pnode);
        virtual ~Node_Function();
        virtual double Calc() const;

    private:
        CString         m_funcName;
        Node*           m_pNode;
    };

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( pop )  // ( disable : 26812 )    enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

}

#endif  //define _CF6AD5B7_507F_4DE4_8980_A3940530D108_NODE_H
