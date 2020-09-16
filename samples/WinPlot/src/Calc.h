/////////////////////////////
// Calculator.h:
//

//////////////////////////////////////////////////////////////////////
// This code is adapted from:
//
// C++ In Action, Industrial-Strength Programming Techniques
//
//  by Bartosz Milewski
//  www.relisoft.com\book\index.htm
//////////////////////////////////////////////////////////////////////

#ifndef _CALCULATOR_H_CF6AD5B7_507F_4DE4_8980_A3940530D108_INCLUDED
#define _CALCULATOR_H_CF6AD5B7_507F_4DE4_8980_A3940530D108_INCLUDED

#include <vector>
#include <map>
#include <math.h>

#include "enums.h"
#include "table.h"

// An Expression:
//   - is any valid self-contained function
//   - can contain zero or more products
//   - can contain zero or more powers
//   - can contain one or more units (separated by operators)
//   - can contain any operator
//
// A product:
//   - can contain zero or more products
//   - can contain zero or more powers
//   - can contain one or more units (separated by operators)
//   - can contain any operator except "+" or "-"  operators
//
// A Power:
//   - can contain zero or more powers
//   - can contain one or more units (separated by operators)
//   - can contain only operator "^"
//
// A unit:
//   - is a self contained number or value
//   - can be a number   eg. -3.14
//   - can be a variable eg. x
//   - can be a function eg. cos(x)
//   - can be an expression enclosed within paranthesis

namespace Calc
{

    class Node;

    //////////////////////////////////////////////////////
    // Calculator evaluates the expression supplied by the
    // Input function.
    class Calculator
    {
    public:
        Calculator(const CString& buffer = "0");
        ~Calculator();

        void        Input(const CString& buffer);
        double      Eval();
        int const   Get_Status() const;
        void        SetVar(const CString& szVar, double Val);

    private:
        bool IsOverflow(double value);

        ////////////////////////////////////////////////////////////////////////
        // A class used by the Calculator.  It loads m_buffer with the expression.
        // It identifies the parts of the expression as tokens.
        class Parser
        {
        public:
            Parser(const CString& buffer = "0");
            virtual ~Parser();
            eToken  GetToken();
            bool    AcceptToken();
            void    Input(const CString& buffer);
            const CString& GetAlphaName() const { return m_alphaName; };
            const double GetNumber() const { return m_number; };

        private:
            CString         m_buffer;
            int             m_index;
            double          m_number;
            CString         m_alphaName;
        };

        Node*       Expression();
        Node*       Product();
        Node*       Power();
        Node*       Unit();

        Node*       m_pTree;
        eStatus     m_status;
        SymbolTable m_symTab;
        Parser      m_parse;
    };

} // namespace Calc

#endif // #define _CALCULATOR_H_CF6AD5B7_507F_4DE4_8980_A3940530D108_INCLUDED
