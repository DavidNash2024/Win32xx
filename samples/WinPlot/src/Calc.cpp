/////////////////////////////
// Calculator.cpp:
//

#include "stdafx.h"
#include <cmath>
#include <float.h>
#include <vector>
#include <map>
#include <cctype>
#include "enums.h"
#include "table.h"
#include "node.h"
#include "Calc.h"

//////////////////////////////////////////////////////////////////////
// This code is adapted from:
//
// C++ In Action, Industrial-Strength Programming Techniques
//
//  by Bartosz Milewski
//  www.relisoft.com\book\index.htm
//////////////////////////////////////////////////////////////////////


using namespace std;

namespace Calc
{

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( push )
#pragma warning ( disable : 26812 )       // enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)


    ////////////////////////////////////////
    // Calculator function definitions.
    //
    
    // Constructor.
    Calculator::Calculator(const CString& buffer)
        : m_status(st_ERROR), m_parse(buffer)
    {
        m_pTree = NULL;
    }

    // Destructor.
    Calculator::~Calculator()
    {
        delete m_pTree; //deletes all nodes created
    }

    void Calculator::Input(const CString& buffer)
    {
        // Loads the calculator buffer, and checks for
        //  correct syntax. Use Get_Status() to verify the
        //  expression syntax.

        m_parse.Input(buffer);

        // Delete the old node tree
        delete m_pTree;
        m_pTree = NULL;

        m_status = st_OK;
        m_pTree = Expression();

        // Should be at the end of the buffer, otherwise the expression is invalid
        eToken Token = m_parse.GetToken();
        if ((Token != tEnd) || (m_status == st_ERROR))
        {
            m_status = st_ERROR;
            // delete the invalid tree if it exists
            delete m_pTree;
            m_pTree = NULL;
        }
    }

    int const Calculator::Get_Status() const
    {
        // Simply returns the status of the previous operation
        // Should be used AFTER Input() or Eval(),
        // otherwise it will always return OK.

        // 0 = Syntax Error in Expression
        // 1 = OK
        // 2 = Expression valid, but calculation produced overflow

        return m_status;
    }

    double Calculator::Eval()
        // Evaluates the expression.  GetStatus() is set as follows:
        //  0 = ERROR
        //  1 = OK
        //  2 = OVERFLOW
    {
        double RetValue = 0;

        if (m_status != st_ERROR)
        {
            RetValue = m_pTree->Calc(); //calculate the value
            if (IsOverflow(RetValue))
            {
                m_status = st_OVERFLOW;
            }
            else
                m_status = st_OK;
        }
        return RetValue;
    }

    Node* Calculator::Expression()
    {
        // x-y+z is evaluated as:  (x-y) + z
        // refer to additional comments in the header file
        Node* pnode = Product();
        eToken Token = m_parse.GetToken();
        if (Token == tPlus || Token == tMinus)
        {
            //Create a BranchNode to hold one or more products
            // {Product} +- {Product} +- {Product} ....
            // Products can be: Units or Powers or Products
            Node_Branch * pBranchNode = new Node_Branch(pnode);
            do
            {
                m_parse.AcceptToken();
                Node * pNew = Product();
                pBranchNode->AddLeaf(pNew, Token);
                Token = m_parse.GetToken();
            } while (Token == tPlus || Token == tMinus);
            pnode = pBranchNode;
        }
        return pnode;
    }

    Node* Calculator::Product()
    {
        //x/y/z  is evaluated as (x / y)/z
        Node* pnode = Power();
        eToken Token = m_parse.GetToken();
        if (Token == tMultiply || Token == tDivide)
        {
            //Create a BranchNode to hold one or more powers
            // {Power} */ {Power} */ {Power} ...
            // Powers can be:  Units or Powers
            Node_Branch * pBranchNode = new Node_Branch(pnode);
            do
            {
                m_parse.AcceptToken();
                Node * pNew = Power();
                pBranchNode->AddLeaf(pNew, Token);
                Token = m_parse.GetToken();
            } while (Token == tMultiply || Token == tDivide);
            pnode = pBranchNode;
        }

        return pnode;
    }

    Node* Calculator::Power()
    {
        Node* pnode = Unit();
        eToken Token = m_parse.GetToken();
        if (Token == tPower)
        {
            //Create a BranchNode to hold one or more units
            // {Unit} ^ {Unit} ^ {Unit} ...
            Node_Branch * pBranchNode = new Node_Branch(pnode);
            do
            {
                m_parse.AcceptToken();
                Node * pNew = Unit();
                pBranchNode->AddLeaf(pNew, Token);
                Token = m_parse.GetToken();
            } while (Token == tPower);
            pnode = pBranchNode;
        }
        return pnode;
    }

    Node* Calculator::Unit()
    {
        Node* pnode = NULL;
        eToken Token = m_parse.GetToken();

        switch (Token)
        {
        case tNumber:   //number
            if (m_parse.AcceptToken())
            {
                pnode = new Node_Number(m_parse.GetNumber());
            }
            else m_status = st_ERROR;
            break;
        case tMinus:    //unary Minus
            m_parse.AcceptToken();
            if (m_parse.GetToken() == tNumber)
            {
                m_parse.AcceptToken();
                pnode = new Node_Number(-m_parse.GetNumber());
            }
            else
            {
                m_status = st_ERROR;
            }
            break;
        case tLeftParenth://an expression within parenthesis
            m_parse.AcceptToken();
            pnode = Expression();

            //Expression began with '(' so must end with ')'
            if (m_parse.GetToken() == tRightParenth)
                m_parse.AcceptToken();
            else
                m_status = st_ERROR;
            break;
        case tVariable:
        {
            m_parse.AcceptToken();
            if (!m_symTab.IsSymbol(m_parse.GetAlphaName()))
            {
                //Add new variable to symbol table
                m_symTab.SetValue(m_parse.GetAlphaName(), 0);
            }

            if (m_parse.GetToken() == tEqual)
            {
                m_parse.AcceptToken();
                Node* pNew = Expression();
                pnode = new Node_Assign(pNew, m_symTab, m_parse.GetAlphaName());
            }
            else
            {
                pnode = new Node_Variable(m_symTab, m_parse.GetAlphaName());
            }
        }
        break;
        case tFunction:
            m_parse.AcceptToken();
            if (m_parse.GetToken() == tLeftParenth)
            {
                FunctionTable funTab;
                CString FunctionName = m_parse.GetAlphaName();
                PFun pFun = funTab.GetFun(FunctionName);  //pointer to the function
                if (pFun != NULL)
                {
                    m_parse.AcceptToken();

                    //Create node tree for expression between parenthesis
                    Node* pnodeTree = Expression();

                    if (m_status != st_ERROR)
                        pnode = new Node_Function(FunctionName, pnodeTree);

                    //normally the destructor for Node_Function does this but
                    //we need to do it manually if a Node_Function isn't created
                    else delete pnodeTree;

                    //Expression began with '(' so must end with ')'
                    if (m_parse.GetToken() == tRightParenth) m_parse.AcceptToken();
                    else m_status = st_ERROR;
                }
                else m_status = st_ERROR;
            }
            else m_status = st_ERROR;
            break;
        default:
            m_status = st_ERROR;
            break;
        }
        //clean up any mess before returning the pnode
        if (m_status == st_ERROR)
        {
            delete pnode;
            pnode = NULL;
        }
        return pnode;
    }

    void Calculator::SetVar(const CString& Var, double Val)
    {
        m_symTab.SetValue(Var, Val);
    }

    bool Calculator::IsOverflow(double value)
    {
        // This code could give rise to compatability issues.
        // Verify when migrating to a different version of compiler

        double inf = HUGE_VAL;       //Infinity
        double neg_inf = -HUGE_VAL;  //Negative Infinity

        // if value is "Not A Number" (ie NaN) then (value != value)
        // examples of NaN are:  0/0,  sqrt(-1), infinity/infinity
        if ((value == inf) || (value == neg_inf) || (value != value))
            return true;
        else
            return false;
    }


    ///////////////////////////////////////////////
    // Parser member function definitions
    ///////////////////////////////////////////////
    Calculator::Parser::Parser(const CString& buffer)
        : m_buffer(buffer), m_index(0), m_number(0)
    {
    }

    Calculator::Parser::~Parser()
    {
    }

    eToken Calculator::Parser::GetToken()
    {
        eToken Token;
        int Index = m_index;
        LPCTSTR charArray = m_buffer.c_str();

        //Move past white space
        while (isspace(charArray[Index]))
            Index++;

        //Check the next char
        switch (charArray[Index])
        {
        case '+':
            Token = tPlus;
            break;
        case '-':
            Token = tMinus;
            break;
        case '*':
            Token = tMultiply;
            break;
        case '/':
            Token = tDivide;
            break;
        case '^':
            Token = tPower;
            break;
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '.':
            Token = tNumber;
            break;
        case '(':
            Token = tLeftParenth;
            break;
        case ')':
            Token = tRightParenth;
            break;
        case '=':
            Token = tEqual;
            break;
        case '\0':
            Token = tEnd;
            break;
        default:
            if (isalpha(charArray[Index]) || charArray[Index] == '_')
            {
                m_alphaName = "";
                int TempIndex = Index;
                do
                {
                    m_alphaName += charArray[TempIndex++];
                } while (isalnum(charArray[TempIndex]));

                //AlphaName could be either a function or a variable
                FunctionTable funTab;
                if (funTab.IsFunction(m_alphaName)) Token = tFunction;
                else Token = tVariable;
            }
            else
            {
                Token = tUnknown;
            }
            break;
        }
        return Token;
    }

    bool Calculator::Parser::AcceptToken()
    {
        bool bRet = true;
        eToken Token = GetToken();

        //Move past white space
        while (isspace(m_buffer[m_index]))
            m_index++;

        switch (Token)
        {
        case tNumber:
        {
            LPTSTR p;
            LPCTSTR charArray = m_buffer.c_str();
            double value = _tcstod(charArray + m_index, &p);
            if (p != charArray)
            {
                m_index = p - charArray;
                m_number = value;
            }
            else
            {
                //a valid number could not be formed
                bRet = false;
                m_index++;
            }
            break;
        }
        case tVariable:
            m_index += m_alphaName.GetLength();
            break;
        case tFunction:
            m_index += m_alphaName.GetLength();
            break;
        default:
            m_index++;
            break;
        }

        return bRet;
    }

    void Calculator::Parser::Input(const CString& buffer)
    {
        m_buffer = buffer;
        m_index = 0;
    }

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( pop )  // ( disable : 26812 )    enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

} // namespace Calc
