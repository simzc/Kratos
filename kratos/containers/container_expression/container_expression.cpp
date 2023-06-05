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
//

// System includes
#include <numeric>
#include <sstream>

// Project includes
#include "containers/container_expression/expressions/unary/unary_slice_expression.h"
#include "includes/define.h"
#include "includes/model_part.h"
#include "utilities/parallel_utilities.h"
#include "containers/container_expression/expressions/literal/literal_expression.h"
#include "containers/container_expression/expressions/literal/literal_flat_expression.h"
#include "containers/container_expression/expressions/io/c_array_copy_expression_io.h"
#include "containers/container_expression/expressions/io/c_array_move_expression_input.h"
#include "containers/container_expression/expressions/arithmetic_operators.h"

// Include base h
#include "container_expression.h"

namespace Kratos {

namespace ContainerExpressionHelperUtilities
{
template <class TMeshType>
ModelPart::MeshType& GetMesh(ModelPart& rModelPart);

template<>
ModelPart::MeshType& GetMesh<MeshType::Local>(ModelPart& rModelPart)
{
    return rModelPart.GetCommunicator().LocalMesh();
}

template<>
ModelPart::MeshType& GetMesh<MeshType::Ghost>(ModelPart& rModelPart)
{
    return rModelPart.GetCommunicator().GhostMesh();
}

template<>
ModelPart::MeshType& GetMesh<MeshType::Interface>(ModelPart& rModelPart)
{
    return rModelPart.GetCommunicator().InterfaceMesh();
}

template <class TMeshType>
const ModelPart::MeshType& GetMesh(const ModelPart& rModelPart);

template<>
const ModelPart::MeshType& GetMesh<MeshType::Local>(const ModelPart& rModelPart)
{
    return rModelPart.GetCommunicator().LocalMesh();
}

template<>
const ModelPart::MeshType& GetMesh<MeshType::Ghost>(const ModelPart& rModelPart)
{
    return rModelPart.GetCommunicator().GhostMesh();
}

template<>
const ModelPart::MeshType& GetMesh<MeshType::Interface>(const ModelPart& rModelPart)
{
    return rModelPart.GetCommunicator().InterfaceMesh();
}

template <class TContainerType>
TContainerType& GetContainer(ModelPart::MeshType &rMesh);

template<>
ModelPart::NodesContainerType& GetContainer<ModelPart::NodesContainerType>(ModelPart::MeshType& rMesh)
{
    return rMesh.Nodes();
}

template<>
ModelPart::ConditionsContainerType& GetContainer<ModelPart::ConditionsContainerType>(ModelPart::MeshType& rMesh)
{
    return rMesh.Conditions();
}

template<>
ModelPart::ElementsContainerType& GetContainer<ModelPart::ElementsContainerType>(ModelPart::MeshType& rMesh)
{
    return rMesh.Elements();
}

template <class TContainerType>
const TContainerType& GetContainer(const ModelPart::MeshType &rMesh);

template<>
const ModelPart::NodesContainerType& GetContainer<ModelPart::NodesContainerType>(const ModelPart::MeshType& rMesh)
{
    return rMesh.Nodes();
}

template<>
const ModelPart::ConditionsContainerType& GetContainer<ModelPart::ConditionsContainerType>(const ModelPart::MeshType& rMesh)
{
    return rMesh.Conditions();
}

template<>
const ModelPart::ElementsContainerType& GetContainer<ModelPart::ElementsContainerType>(const ModelPart::MeshType& rMesh)
{
    return rMesh.Elements();
}

template <class TRawDataType, class TContainerExpressionType>
void Read(
    TContainerExpressionType& rContainerExpression,
    TRawDataType const* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize)
{
    KRATOS_TRY

    KRATOS_ERROR_IF_NOT(static_cast<unsigned int>(NumberOfEntities) == rContainerExpression.GetContainer().size())
        << "Number of entities does not match with the local container size. [ "
           "NumberOfEntities = "
        << NumberOfEntities
        << ", local container size = " << rContainerExpression.GetContainer().size() << " ].\n";

    rContainerExpression.SetExpression(CArrayExpressionInput(pBegin, NumberOfEntities, pShapeBegin, ShapeSize).Execute());

    KRATOS_CATCH("");
}

template <class TRawDataType, class TContainerExpressionType>
void MoveFrom(
    TContainerExpressionType& rContainerExpression,
    TRawDataType* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize)
{
    KRATOS_TRY

    KRATOS_ERROR_IF_NOT(static_cast<unsigned int>(NumberOfEntities) == rContainerExpression.GetContainer().size())
        << "Number of entities does not match with the local container size. [ "
           "NumberOfEntities = "
        << NumberOfEntities
        << ", local container size = " << rContainerExpression.GetContainer().size() << " ].\n";

    rContainerExpression.SetExpression(CArrayMoveExpressionInput(pBegin, NumberOfEntities, pShapeBegin, ShapeSize).Execute());

    KRATOS_CATCH("");
}

} // namespace ContainerExpressionHelperUtilities

template <class TContainerType, class TMeshType>
ContainerExpression<TContainerType, TMeshType>::ContainerExpression(ModelPart& rModelPart)
    : mpExpression(),
      mpModelPart(&rModelPart)
{
}

template <class TContainerType, class TMeshType>
ContainerExpression<TContainerType, TMeshType>::ContainerExpression(
    const ContainerExpression& rOther)
    : mpExpression(rOther.mpExpression),
      mpModelPart(rOther.mpModelPart)
{
}
template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::CopyFrom(
    const ContainerExpression<TContainerType, TMeshType>& rOther)
{
    KRATOS_ERROR_IF(this->GetContainer().size() != rOther.GetContainer().size())
        << "Mismatching model parts found with different number of entities in CopyFrom operation.\n"
        << this->mpModelPart->FullName()
        << ", origin model part name: " << rOther.GetModelPart().FullName() << " ].\n";

    mpExpression = rOther.mpExpression;
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::Read(
    double const* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize)
{
    ContainerExpressionHelperUtilities::Read(*this, pBegin, NumberOfEntities, pShapeBegin, ShapeSize);
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::Read(
    int const* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize)
{
    ContainerExpressionHelperUtilities::Read(*this, pBegin, NumberOfEntities, pShapeBegin, ShapeSize);
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::MoveFrom(
    double* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize)
{
    ContainerExpressionHelperUtilities::MoveFrom(*this, pBegin, NumberOfEntities, pShapeBegin, ShapeSize);
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::MoveFrom(
    int* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize)
{
    ContainerExpressionHelperUtilities::MoveFrom(*this, pBegin, NumberOfEntities, pShapeBegin, ShapeSize);
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::Evaluate(
    double* pBegin,
    const int NumberOfEntities,
    int const* pShapeBegin,
    const int ShapeSize) const
{
    KRATOS_TRY

    KRATOS_ERROR_IF_NOT(static_cast<unsigned int>(NumberOfEntities) == this->GetContainer().size())
        << "Number of entities does not match with the local container size. [ "
           "NumberOfEntities = "
        << NumberOfEntities
        << ", local container size = " << this->GetContainer().size() << " ].\n";

    CArrayExpressionOutput(pBegin, NumberOfEntities * std::accumulate(pShapeBegin, pShapeBegin+ShapeSize, 1, [](const int V1, const int V2) { return V1 * V2; })).Execute(**this->mpExpression);

    KRATOS_CATCH("");
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::SetDataToZero()
{
    mpExpression = LiteralExpression<double>::Create(0.0, GetContainer().size());
}

template <class TContainerType, class TMeshType>
void ContainerExpression<TContainerType, TMeshType>::SetExpression(Expression::Pointer pExpression)
{
    this->mpExpression = pExpression;
}

template <class TContainerType, class TMeshType>
bool ContainerExpression<TContainerType, TMeshType>::HasExpression() const
{
    return mpExpression.has_value();
}

template <class TContainerType, class TMeshType>
const Expression& ContainerExpression<TContainerType, TMeshType>::GetExpression() const
{
    return *(*mpExpression);
}


template <class TContainerType, class TMeshType>
const Expression::Pointer ContainerExpression<TContainerType, TMeshType>::pGetExpression() const
{
    return *mpExpression;
}

template <class TContainerType, class TMeshType>
const std::vector<std::size_t> ContainerExpression<TContainerType, TMeshType>::GetItemShape() const
{
    return this->GetExpression().GetItemShape();
}

template <class TContainerType, class TMeshType>
std::size_t ContainerExpression<TContainerType, TMeshType>::GetItemComponentCount() const
{
    return this->GetExpression().GetItemComponentCount();
}

template <class TContainerType, class TMeshType>
ModelPart& ContainerExpression<TContainerType, TMeshType>::GetModelPart()
{
    return *mpModelPart;
}

template <class TContainerType, class TMeshType>
const ModelPart& ContainerExpression<TContainerType, TMeshType>::GetModelPart() const
{
    return *mpModelPart;
}

template <class TContainerType, class TMeshType>
TContainerType& ContainerExpression<TContainerType, TMeshType>::GetContainer()
{
    return ContainerExpressionHelperUtilities::GetContainer<TContainerType>(ContainerExpressionHelperUtilities::GetMesh<TMeshType>(*mpModelPart));
}

template <class TContainerType, class TMeshType>
const TContainerType& ContainerExpression<TContainerType, TMeshType>::GetContainer() const
{
    return ContainerExpressionHelperUtilities::GetContainer<TContainerType>(ContainerExpressionHelperUtilities::GetMesh<TMeshType>(*mpModelPart));
}

template <class TContainerType, class TMeshType>
std::string ContainerExpression<TContainerType, TMeshType>::Info() const
{
    std::stringstream msg;

    msg << "VariableDataInfo: [ ModelPartName: "
        << this->GetModelPart().FullName()
        << ", Number of entities: " << this->GetContainer().size();

    if (mpExpression.has_value()) {
        msg << ", DataDimension: " << this->GetItemComponentCount();
        msg << ", Expression: " << this->GetExpression();
    } else {
        msg << ", Expression: not initialized";
    }

    msg << " ].\n";

    return msg.str();
}

template <class TContainerType, class TMeshType>
std::string ContainerExpression<TContainerType, TMeshType>::PrintData() const
{
    std::stringstream msg;
    msg << this->Info();
    return msg.str();
}


template <class TContainer, class TMesh>
ContainerExpression<TContainer, TMesh> ContainerExpression<TContainer,TMesh>::Slice(IndexType Offset, IndexType Stride) const
{
    auto copy = *this;
    copy.SetExpression(UnarySliceExpression::Create(
        this->pGetExpression(),
        Offset,
        Stride
    ));
    return copy;
}


#define KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR(OPERATOR_NAME)                \
    template <class TContainerType, class TMeshType>                                     \
    ContainerExpression<TContainerType, TMeshType> OPERATOR_NAME(                        \
        const ContainerExpression<TContainerType, TMeshType>& rLeft, const double Right) \
    {                                                                                    \
        ContainerExpression<TContainerType, TMeshType> result(*rLeft.mpModelPart);       \
        result.SetExpression(Kratos::OPERATOR_NAME(rLeft.pGetExpression(), Right));      \
        return result;                                                                   \
    }                                                                                    \
                                                                                         \
    template <class TContainerType, class TMeshType>                                     \
    ContainerExpression<TContainerType, TMeshType> OPERATOR_NAME(                        \
        const double Left, const ContainerExpression<TContainerType, TMeshType>& rRight) \
    {                                                                                    \
        ContainerExpression<TContainerType, TMeshType> result(*rRight.mpModelPart);      \
        result.SetExpression(Kratos::OPERATOR_NAME(Left, rRight.pGetExpression()));      \
        return result;                                                                   \
    }                                                                                    \
                                                                                         \
    template <class TContainerType, class TMeshType>                                     \
    ContainerExpression<TContainerType, TMeshType> OPERATOR_NAME(                        \
        const ContainerExpression<TContainerType, TMeshType>& rLeft,                     \
        const ContainerExpression<TContainerType, TMeshType>& rRight)                    \
    {                                                                                    \
        ContainerExpression<TContainerType, TMeshType> result(*rRight.mpModelPart);      \
        result.SetExpression(Kratos::OPERATOR_NAME(rLeft.pGetExpression(),               \
                                                   rRight.pGetExpression()));            \
        return result;                                                                   \
    }

KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator+)
KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator-)
KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator*)
KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator/)
KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR(Pow)

#define KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR(              \
    OPERATOR_NAME, CONTAINER_TYPE, MESH_TYPE)                                 \
    template ContainerExpression<CONTAINER_TYPE, MESH_TYPE> OPERATOR_NAME(    \
        const ContainerExpression<CONTAINER_TYPE, MESH_TYPE>&, const double); \
    template ContainerExpression<CONTAINER_TYPE, MESH_TYPE> OPERATOR_NAME(    \
        const double, const ContainerExpression<CONTAINER_TYPE, MESH_TYPE>&); \
    template ContainerExpression<CONTAINER_TYPE, MESH_TYPE> OPERATOR_NAME(    \
        const ContainerExpression<CONTAINER_TYPE, MESH_TYPE>&,                \
        const ContainerExpression<CONTAINER_TYPE, MESH_TYPE>&);

#define KRATOS_INSTANTIATE_UNARY_CONTAINER_EXPRESSION_OPERATOR(                                                    \
    OPERATOR_NAME, EXPRESSION_OPERATOR_NAME, CONTAINER_TYPE, MESH_TYPE)                                            \
    template <>                                                                                                    \
    ContainerExpression<CONTAINER_TYPE, MESH_TYPE>& ContainerExpression<CONTAINER_TYPE, MESH_TYPE>::OPERATOR_NAME( \
        const double Value)                                                                                        \
    {                                                                                                              \
        this->mpExpression = EXPRESSION_OPERATOR_NAME(*this->mpExpression, Value);                                 \
        return *this;                                                                                              \
    }                                                                                                              \
    template <>                                                                                                    \
    ContainerExpression<CONTAINER_TYPE, MESH_TYPE>& ContainerExpression<CONTAINER_TYPE, MESH_TYPE>::OPERATOR_NAME( \
        const ContainerExpression<CONTAINER_TYPE, MESH_TYPE>& Value)                                               \
    {                                                                                                              \
        this->mpExpression =                                                                                       \
            EXPRESSION_OPERATOR_NAME(*this->mpExpression, Value.pGetExpression());                                 \
        return *this;                                                                                              \
    }

#define KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP(CONTAINER_TYPE, MESH_TYPE)         \
    template class ContainerExpression<CONTAINER_TYPE, MESH_TYPE>;                                \
    KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator+, CONTAINER_TYPE, MESH_TYPE) \
    KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator-, CONTAINER_TYPE, MESH_TYPE) \
    KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator*, CONTAINER_TYPE, MESH_TYPE) \
    KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR(operator/, CONTAINER_TYPE, MESH_TYPE) \
    KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR(Pow, CONTAINER_TYPE, MESH_TYPE)       \
    KRATOS_INSTANTIATE_UNARY_CONTAINER_EXPRESSION_OPERATOR(operator+=, operator+,                 \
                                                           CONTAINER_TYPE, MESH_TYPE)             \
    KRATOS_INSTANTIATE_UNARY_CONTAINER_EXPRESSION_OPERATOR(operator-=, operator-,                 \
                                                           CONTAINER_TYPE, MESH_TYPE)             \
    KRATOS_INSTANTIATE_UNARY_CONTAINER_EXPRESSION_OPERATOR(operator*=, operator*,                 \
                                                           CONTAINER_TYPE, MESH_TYPE)             \
    KRATOS_INSTANTIATE_UNARY_CONTAINER_EXPRESSION_OPERATOR(operator/=, operator/,                 \
                                                           CONTAINER_TYPE, MESH_TYPE)

#define KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP_CONTAINER_TYPE(CONTAINER_TYPE)   \
    KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP(CONTAINER_TYPE, MeshType::Local)     \
    KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP(CONTAINER_TYPE, MeshType::Interface) \
    KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP(CONTAINER_TYPE, MeshType::Ghost)

KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP_CONTAINER_TYPE(ModelPart::NodesContainerType)
KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP_CONTAINER_TYPE(ModelPart::ConditionsContainerType)
KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP_CONTAINER_TYPE(ModelPart::ElementsContainerType)

#undef KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP_CONTAINER_TYPE
#undef KRATOS_INSTANTIATE_UNARY_CONTAINER_EXPRESSION_OPERATOR
#undef KRATOS_INSTANTIATE_CONTAINER_EXPRESSION_OPERATOR_GROUP
#undef KRATOS_INSTANTIATE_BINARY_CONTAINER_EXPRESSION_OPERATOR
#undef KRATOS_DEFINE_BINARY_CONTAINER_EXPRESSION_OPERATOR

} // namespace Kratos