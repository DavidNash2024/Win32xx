/////////////////////////////
// Enums.h
//

#pragma once

namespace Calc
{
    // Declare a function pointer for the math functions.
    using PFun = double (*)(double);

    enum eToken
    {
        tNumber,
        tPlus,
        tMinus,
        tMultiply,
        tDivide,
        tPower,
        tVariable,
        tFunction,
        tEqual,
        tLeftParenth,
        tRightParenth,
        tUnknown,
        tEnd
    };

    enum eStatus
    {
        st_ERROR,
        st_OK,                  // Invalid sequence of numbers and operators
        st_OVERFLOW             // Infinity, or invalid function argument
    };

} // namespace Calc

