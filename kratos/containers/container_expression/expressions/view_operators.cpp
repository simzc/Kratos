//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya
//                   Máté Kelemen
//

// Project includes
#include "containers/container_expression/expressions/view_operators.h"
#include "containers/container_expression/expressions/unary/unary_slice_expression.h"
#include "containers/container_expression/expressions/unary/unary_reshape_expression.h"
#include "containers/container_expression/expressions/unary/unary_combine_expression.h"


namespace Kratos {


Expression::Pointer Slice(const Expression::Pointer& rpExpression,
                          std::size_t Offset,
                          std::size_t Stride)
{
    return UnarySliceExpression::Create(
        rpExpression,
        Offset,
        Stride
    );
}


Expression::Pointer Reshape(const Expression::Pointer& rpExpression,
                            const std::vector<std::size_t>& rNewShape)
{
    return Reshape(rpExpression, rNewShape.begin(), rNewShape.end());
}


Expression::Pointer Comb(const std::vector<Expression::Pointer>& rExpressions)
{
    return Comb(rExpressions.begin(), rExpressions.end());
}


} // namespace Kratos
