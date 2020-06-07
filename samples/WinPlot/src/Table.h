// Table.h: interface for the FunctionTable and SymbolTable classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TABLE_H_CF6AD5B7_507F_4DE4_8980_A3940530D108_INCLUDED)
#define _TABLE_H_CF6AD5B7_507F_4DE4_8980_A3940530D108_INCLUDED

namespace Calc
{

	class FunctionTable
		// A table of math functions.
	{
	public:
		FunctionTable();
		~FunctionTable();
		PFun	GetFun(const std::string& funName);
		bool	IsFunction(const std::string& funName);

	private:
		std::map<std::string, PFun> m_functions;

		// cotan needs class scope, not just instance scope, so it
		//  is declared as static
		static double cotan(double x);
	};

	//////////////////////////////////////////////////////
	// This class provides a table of symbols.  Used for variables eg x,
	// and constants such as   pi (3.141592654) and e = (2.718281828).
	// Each symbol has a defined value.
	class SymbolTable
	{
	public:
		SymbolTable();
		~SymbolTable();

		bool IsSymbol(const std::string& symbol);
		double GetValue(const std::string& symbol) const;
		void SetValue(const std::string& symbol, double value);

	private:
		std::map<std::string, double> m_symbolMap;
	};

} // namespace Calc

#endif // !defined(_TABLE_H_CF6AD5B7_507F_4DE4_8980_A3940530D108_INCLUDED)
