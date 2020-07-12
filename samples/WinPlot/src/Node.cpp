// Node.cpp
//
// implemetation of the Node and related classes
//////////////////////////////////////////////

#include "stdafx.h"
#include <cmath>
#include <vector>
#include <map>
#include "enums.h"
#include "table.h"
#include "Node.h"

namespace Calc
{

    ////////////////////////
    // Node_Number functions
    ////////////////////////
    Node_Number::Node_Number(double Number) : m_number(Number)
    {
    }

    double Node_Number::Calc() const
    {
        return m_number;
    }

    ////////////////////////
    // Node_Branch Functions
    ////////////////////////
    Node_Branch::~Node_Branch()
    {
        // Delete each leaf node.
        for (unsigned u = 0; u < m_leaves.size(); ++u)
            delete m_leaves[u];
    }
    //////////////////////
    // Node_Branch Functions
    //////////////////////
    double Node_Branch::Calc() const
    {
        double Value = 0.0;
        for (unsigned u = 0; u < m_leaves.size(); ++u)
        {
            double NodeVal = m_leaves[u]->Calc();
            switch (m_tokens[u])
            {
            case tPlus:
                Value += NodeVal;
                break;

            case tMinus:
                Value -= NodeVal;
                break;

            case tMultiply:
                Value *= NodeVal;
                break;

            case tDivide:
                Value /= NodeVal;
                break;

            case tPower:
                Value = pow(Value, NodeVal);
                break;
            default:
                break;
            }
        }
        return Value;
    }

    //////////////////////////
    // Node_Variable functions
    //////////////////////////
    Node_Variable::Node_Variable(SymbolTable& symTab, const CString& symbol)
        : m_symTab(symTab), m_symbol(symbol)
    {
    }
    
    double Node_Variable::Calc() const
    {
        return m_symTab.GetValue(m_symbol);
    }

    //////////////////////////
    // Node_Assign functions
    //////////////////////////
    Node_Assign::Node_Assign(Node* pNode, SymbolTable& symTab, const CString& symbol)
        : m_node(pNode), m_symTab(symTab), m_symbol(symbol)
    {
    }

    Node_Assign::~Node_Assign()
    {
        delete m_node;
    }

    double Node_Assign::Calc() const
    {
        double var = m_node->Calc();
        m_symTab.SetValue(m_symbol, var);
        return var;
    }

    ///////////////////////////
    // Node_Function functions
    ///////////////////////////

    Node_Function::Node_Function(CString funName, Node* pnode)
        : m_funcName(funName), m_pNode(pnode)
    {
    }

    Node_Function::~Node_Function()
    {
        //delete our node tree
        delete m_pNode;
    }

    double Node_Function::Calc() const
    {
        FunctionTable funTab;
        PFun pFun = funTab.GetFun(m_funcName);

        // call the function pointed to by pFun on the evaluated expression
        return (*pFun)(m_pNode->Calc());
    }

} // namespace Calc
