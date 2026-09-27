/////////////////////////////
// Table.h:
//

#pragma once

namespace Calc
{
    /////////////////////////////
    // A table of math functions.
    //
    class FunctionTable
    {
    public:
        FunctionTable();
        ~FunctionTable();
        PFun    GetFun(const CString& funName);
        bool    IsFunction(const CString& funName);

    private:
        std::map<CString, PFun> m_functions;

        // cotan needs class scope, not just instance scope, so it
        //  is declared as static
        static double cotan(double x);
    };


    //////////////////////////////////////////////////////
    // This class provides a table of symbols.  Used for variables
    // e.g. x, and constants such as pi (3.141592654) and e (2.718281828).
    // Each symbol has a defined value.
    class SymbolTable
    {
    public:
        SymbolTable();
        ~SymbolTable();

        bool IsSymbol(const CString& symbol);
        double GetValue(const CString& symbol) const;
        void SetValue(const CString& symbol, double value);

    private:
        std::map<CString, double> m_symbolMap;
    };

} // namespace Calc

