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

#pragma once

// Project includes
#include "containers/container_expression/expressions/expression.h"
#include "includes/define.h"


namespace Kratos {


/// @name Arithmetic Operators
/// @{

KRATOS_API(KRATOS_CORE) Expression::Pointer operator+(const Expression::Pointer& rpLeft, double Right);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator+(double Left, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator+(const Expression::Pointer& rpLeft, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator-(const Expression::Pointer& rpLeft, double Right);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator-(double Left, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator-(const Expression::Pointer& rpLeft, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator*(const Expression::Pointer& rpLeft, double Right);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator*(double Left, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator*(const Expression::Pointer& rpLeft, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator/(const Expression::Pointer& rpLeft, double Right);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator/(double Left, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer operator/(const Expression::Pointer& rpLeft, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer Scale(const Expression::Pointer& rpLeft, const Expression::Pointer& rpRight);

KRATOS_API(KRATOS_CORE) Expression::Pointer Pow(const Expression::Pointer& rpBase, double Exponent);

KRATOS_API(KRATOS_CORE) Expression::Pointer Pow(const Expression::Pointer& rpBase, const Expression::Pointer& rpExponent);

/// @}


} // namespace Kratos
