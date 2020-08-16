/////////////////////////////
// Table.cpp:
//

#include "stdafx.h"
#include <cmath>
#include <vector>
#include <map>
#include "enums.h"
#include "table.h"
#include "node.h"

namespace Calc
{

    //////////////////////////////////////
    // FunctionTable function definitions.
    //
    FunctionTable::FunctionTable()
    {
        //Load the function table with a string and function pointer
        m_functions["log"] = log;
        m_functions["log10"] = log10;
        m_functions["exp"] = exp;
        m_functions["sqrt"] = sqrt;
        m_functions["sin"] = sin;
        m_functions["cos"] = cos;
        m_functions["tan"] = tan;
        m_functions["cotan"] = cotan;
        m_functions["sinh"] = sinh;
        m_functions["cosh"] = cosh;
        m_functions["tanh"] = tanh;
        m_functions["asin"] = asin;
        m_functions["acos"] = acos;
        m_functions["atan"] = atan;
    }

    FunctionTable::~FunctionTable()
    {
    }

    PFun FunctionTable::GetFun(const CString& funName)
    {
        PFun pFunpointer = 0;
        std::map<CString, PFun>::iterator it = m_functions.find(funName);
        if (it != m_functions.end())
            pFunpointer = (*it).second;

        return pFunpointer;
    }

    bool    FunctionTable::IsFunction(const CString& funName)
    {
        return (m_functions.find(funName) != m_functions.end());
    }

    double FunctionTable::cotan(double x)
    {
        double y = tan(x);
        return 1.0 / y;
    }


    ////////////////////////////////////
    // SymbolTable function definitions.
    //
    SymbolTable::SymbolTable()
    {
        SetValue("pi", 2 * acos((double)0));
        SetValue("e", exp((double)1));
    }

    SymbolTable::~SymbolTable()
    {
    }

    // Returns true if the specified symbol is in the symbol table.
    bool SymbolTable::IsSymbol(const CString& symbol)
    {
        return (m_symbolMap.find(symbol) != m_symbolMap.end());
    }

    // Retrieves the value of the specified symbol.
    double SymbolTable::GetValue(const CString& symbol) const
    {
        double value = 0;
        std::map<CString, double>::const_iterator it = m_symbolMap.find(symbol);
        if (it != m_symbolMap.end())
            value = (*it).second;

        // Asserts if the specified symbol is not in the symbol table.
        assert(it != m_symbolMap.end());
        return value;
    }

    // Adds or modifies a symbol table entry
    void SymbolTable::SetValue(const CString& symbol, double value)
    {
        std::map<CString, double>::iterator it = m_symbolMap.find(symbol);
        if (it != m_symbolMap.end())
            (*it).second = value;
        else
            m_symbolMap.insert(std::make_pair(symbol, value));
    }

} // namespace Calc
