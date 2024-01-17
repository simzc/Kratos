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

// System incldues

// Project includes
#include "expression/literal_flat_expression.h"
#include "utilities/parallel_utilities.h"

// Include base h
#include "entity_domain_size_expression_io.h"

namespace Kratos {

EntityDomainSizeExpressionIO::EntityDomainSizeExpressionInput::EntityDomainSizeExpressionInput(
    const ModelPart& rModelPart,
    const ContainerType& rContainerType)
    : mrModelPart(rModelPart),
      mContainerType(rContainerType)
{
}

Expression::Pointer EntityDomainSizeExpressionIO::EntityDomainSizeExpressionInput::Execute() const
{
    const auto& r_mesh = mrModelPart.GetCommunicator().LocalMesh();

    LiteralFlatExpression<double>::Pointer expression;

    switch (mContainerType) {
        case ContainerType::ConditionNonHistorical:
            expression = LiteralFlatExpression<double>::Create(r_mesh.NumberOfConditions(), {});
            IndexPartition<IndexType>(r_mesh.NumberOfConditions()).for_each([&r_mesh, &expression](const IndexType Index) {
                const auto& r_condition = *(r_mesh.ConditionsBegin() + Index);
                *(expression->begin() + Index) = r_condition.GetGeometry().DomainSize();
            });
            break;
        case ContainerType::ElementNonHistorical:
            expression = LiteralFlatExpression<double>::Create(r_mesh.NumberOfElements(), {});
            IndexPartition<IndexType>(r_mesh.NumberOfElements()).for_each([&r_mesh, &expression](const IndexType Index) {
                const auto& r_element = *(r_mesh.ElementsBegin() + Index);
                *(expression->begin() + Index) = r_element.GetGeometry().DomainSize();
            });
            break;
        default:
            KRATOS_ERROR << "EntityDomainSizeExpressionInput only supports ConditionNonHistorical and ElementNonHistorical container types.";
            break;
    }

    return expression;
}

template<class TContainerType, MeshType TMeshType>
void EntityDomainSizeExpressionIO::Read(ContainerExpression<TContainerType, TMeshType>& rContainerExpression)
{
    if constexpr(std::is_same_v<TContainerType, ModelPart::ConditionsContainerType>) {
        auto p_expression = EntityDomainSizeExpressionInput(rContainerExpression.GetModelPart(), ContainerType::ConditionNonHistorical).Execute();
        rContainerExpression.SetExpression(p_expression);
    } else if constexpr(std::is_same_v<TContainerType, ModelPart::ElementsContainerType>) {
        auto p_expression = EntityDomainSizeExpressionInput(rContainerExpression.GetModelPart(), ContainerType::ElementNonHistorical).Execute();
        rContainerExpression.SetExpression(p_expression);
    } else {
        static_assert(std::is_same_v<TContainerType, TContainerType>, "Unsupported TContainerType");
    }

}

#define KRATOS_INSTANTIATE_CONTAINER_ENTITY_DOMAIN_SIZE_EXPRESSION_IO(CONTAINER_TYPE, MESH_TYPE)                                                                                                            \
    template KRATOS_API(KRATOS_CORE) void EntityDomainSizeExpressionIO::Read(ContainerExpression<CONTAINER_TYPE, MESH_TYPE>&);

KRATOS_INSTANTIATE_CONTAINER_ENTITY_DOMAIN_SIZE_EXPRESSION_IO(ModelPart::ConditionsContainerType, MeshType::Local)
KRATOS_INSTANTIATE_CONTAINER_ENTITY_DOMAIN_SIZE_EXPRESSION_IO(ModelPart::ElementsContainerType, MeshType::Local)

#undef KRATOS_INSTANTIATE_CONTAINER_ENTITY_DOMAIN_SIZE_EXPRESSION_IO

} // namespace Kratos
