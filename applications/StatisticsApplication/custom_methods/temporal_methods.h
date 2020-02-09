//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya (https://github.com/sunethwarna)
//

#if !defined(KRATOS_TEMPORAL_METHODS_H_INCLUDED)
#define KRATOS_TEMPORAL_METHODS_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/model_part.h"

// Application includes

// Application method includes
#include "custom_methods/temporal_mean_method.h"
#include "custom_methods/temporal_sum_method.h"
#include "custom_methods/temporal_min_method.h"
#include "custom_methods/temporal_variance_method.h"

namespace Kratos
{
///@addtogroup RANSApplication
///@{

///@name Kratos Globals
///@{

namespace TemporalMethods
{
template <typename TContainerType, typename TContainerItemType, template <typename T> typename TDataRetrievalFunctor, template <typename T> typename TDataStorageFunctor>
class ContainerTemporalMethods
{
public:
    using VarianceMethod =
        TemporalVarianceMethod<TContainerType, TContainerItemType, TDataRetrievalFunctor, TDataStorageFunctor>;
    using MeanMethod =
        TemporalMeanMethod<TContainerType, TContainerItemType, TDataRetrievalFunctor, TDataStorageFunctor>;
    using SumMethod =
        TemporalSumMethod<TContainerType, TContainerItemType, TDataRetrievalFunctor, TDataStorageFunctor>;
    using MinMethod =
        TemporalMinMethod<TContainerType, TContainerItemType, TDataRetrievalFunctor, TDataStorageFunctor>;
};

using NodeType = ModelPart::NodeType;
using ElementType = ModelPart::ElementType;
using ConditionType = ModelPart::ConditionType;

using NodesContainerType = ModelPart::NodesContainerType;
using ElementsContainerType = ModelPart::ElementsContainerType;
using ConditionsContainerType = ModelPart::ConditionsContainerType;

template <template <typename T> typename TDataStorageFunctor>
class HistoricalTemporalMethods
    : public ContainerTemporalMethods<NodesContainerType, NodeType, MethodsUtilities::HistoricalDataValueRetrievalFunctor, TDataStorageFunctor>
{
};

class HistoricalInputHistoricalOutputTemporalMethods
    : public HistoricalTemporalMethods<MethodsUtilities::HistoricalDataValueRetrievalFunctor>
{
};

class HistoricalInputNonHistoricalOutputTemporalMethods
    : public HistoricalTemporalMethods<MethodsUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

class NodalNonHistoricalTemporalMethods
    : public ContainerTemporalMethods<NodesContainerType, NodeType, MethodsUtilities::NonHistoricalDataValueRetrievalFunctor, MethodsUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

class ConditionNonHistoricalTemporalMethods
    : public ContainerTemporalMethods<ConditionsContainerType, ConditionType, MethodsUtilities::NonHistoricalDataValueRetrievalFunctor, MethodsUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

class ElementNonHistoricalTemporalMethods
    : public ContainerTemporalMethods<ElementsContainerType, ElementType, MethodsUtilities::NonHistoricalDataValueRetrievalFunctor, MethodsUtilities::NonHistoricalDataValueRetrievalFunctor>
{
};

} // namespace TemporalMethods
} // namespace Kratos
#endif // KRATOS_TEMPORAL_METHODS_H_INCLUDED