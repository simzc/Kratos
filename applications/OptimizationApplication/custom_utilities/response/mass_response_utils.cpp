//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   license: OptimizationApplication/license.txt
//
//  Main author:     Reza Najian Asl
//                   Suneth Warnakulasuriya
//

// System includes
#include <sstream>

// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "utilities/parallel_utilities.h"
#include "utilities/reduction_utilities.h"
#include "utilities/element_size_calculator.h"
#include "utilities/variable_utils.h"

// Application includes
#include "custom_utilities/optimization_utils.h"
#include "optimization_application_variables.h"

// Include base h
#include "mass_response_utils.h"

namespace Kratos
{

///@name Kratos Classes
///@{

bool MassResponseUtils::HasVariableInProperties(
    const ModelPart& rModelPart,
    const Variable<double>& rVariable)
{
    KRATOS_TRY

    bool local_has_variable = false;
    if (rModelPart.NumberOfElements() > 0) {
        const auto& r_properties = rModelPart.Elements().front().GetProperties();
        local_has_variable = r_properties.Has(rVariable);
    }
    return rModelPart.GetCommunicator().GetDataCommunicator().OrReduceAll(local_has_variable);

    KRATOS_CATCH("");
}

void MassResponseUtils::Check(const ModelPart& rModelPart)
{
    const auto& r_data_communicator = rModelPart.GetCommunicator().GetDataCommunicator();

    if (!OptimizationUtils::IsVariableExistsInAllContainerProperties(rModelPart.Elements(), DENSITY, r_data_communicator)) {
        KRATOS_ERROR << "Some elements' properties in " << rModelPart.FullName()
                     << " does not have DENSITY variable.";
    }

    if (OptimizationUtils::IsVariableExistsInAtLeastOneContainerProperties(rModelPart.Elements(), THICKNESS, r_data_communicator) &&
        OptimizationUtils::IsVariableExistsInAtLeastOneContainerProperties(rModelPart.Elements(), CROSS_AREA, r_data_communicator)) {
        KRATOS_ERROR << rModelPart.FullName() << " has elements consisting THICKNESS and CROSS_AREA. "
                     << "Please break down this response to SumResponseFunction where each sub "
                     << "response function only has elements with either THICKNESS or CROSS_AREA.";
    }

    if (OptimizationUtils::GetContainerEntityGeometryType(rModelPart.Elements(), r_data_communicator) == GeometryData::KratosGeometryType::Kratos_generic_type) {
        KRATOS_ERROR << rModelPart.FullName() << " has elements with different geometry types. "
                     << "Please break down this response to SumResponseFunction where each sub "
                     << "response function only has elements with one geometry type.";
    }
}

double MassResponseUtils::CalculateValue(const ModelPart& rModelPart)
{
    KRATOS_TRY

    if (rModelPart.GetCommunicator().GlobalNumberOfElements() == 0) {
        return 0.0;
    }

    KRATOS_ERROR_IF_NOT(HasVariableInProperties(rModelPart, DENSITY))
        << "DENSITY is not found in element properties of " << rModelPart.FullName() << ".\n";

    KRATOS_ERROR_IF(HasVariableInProperties(rModelPart, THICKNESS) && HasVariableInProperties(rModelPart, CROSS_AREA))
        << rModelPart.FullName()
        << " has elements with properties having both THICKNESS and CROSS_AREA. Please separate the model part such that either one of them is present in elemental properties.\n";

    const auto get_thickness = HasVariableInProperties(rModelPart, THICKNESS)
                                    ? [](const ModelPart::ElementType& rElement) { return rElement.GetProperties()[THICKNESS]; }
                                    : [](const ModelPart::ElementType& rElement) { return 1.0; };

    const auto get_cross_area = HasVariableInProperties(rModelPart, CROSS_AREA)
                                    ? [](const ModelPart::ElementType& rElement) { return rElement.GetProperties()[CROSS_AREA]; }
                                    : [](const ModelPart::ElementType& rElement) { return 1.0; };

    const double local_mass = block_for_each<SumReduction<double>>(rModelPart.Elements(), [&](const auto& rElement) {
        return rElement.GetGeometry().DomainSize() * rElement.GetProperties()[DENSITY] * get_thickness(rElement) * get_cross_area(rElement);
    });

    return rModelPart.GetCommunicator().GetDataCommunicator().SumAll(local_mass);

    KRATOS_CATCH("")
}

template<class TDataType>
void MassResponseUtils::CalculateSensitivity(
    ModelPart& rSensitivityModelPart,
    const Variable<TDataType>& rSensitivityVariable)
{
    KRATOS_TRY

    std::stringstream error_msg;

    error_msg << "Unsupported sensitivity w.r.t. " << rSensitivityVariable.Name()
              << " requested for " << rSensitivityModelPart.FullName()
              << ". Followings are supported variables:"
              << "\n\tSHAPE_SENSITIVITY"
              << "\n\tDENSITY_SENSITIVITY"
              << "\n\tTHICKNESS_SENSITIVITY"
              << "\n\tCROSS_AREA_SENSITIVITY";

    if constexpr (std::is_same_v<TDataType, double>) {
        if (rSensitivityVariable == DENSITY_SENSITIVITY) {
            CalculateMassDensitySensitivity(rSensitivityModelPart, DENSITY_SENSITIVITY);
        } else if (rSensitivityVariable == THICKNESS_SENSITIVITY) {
            CalculateMassThicknessSensitivity(rSensitivityModelPart, THICKNESS_SENSITIVITY);
        } else if (rSensitivityVariable == CROSS_AREA_SENSITIVITY) {
            CalculateMassCrossAreaSensitivity(rSensitivityModelPart, CROSS_AREA_SENSITIVITY);
        } else {
            KRATOS_ERROR << error_msg.str();
        }
    } else if constexpr (std::is_same_v<TDataType, array_1d<double, 3>>) {
        if (rSensitivityVariable == SHAPE_SENSITIVITY) {
            CalculateMassShapeSensitivity(rSensitivityModelPart, SHAPE_SENSITIVITY);
        } else {
            KRATOS_ERROR << error_msg.str();
        }
    }

    KRATOS_CATCH("");
}

void MassResponseUtils::CalculateMassShapeSensitivity(
    ModelPart& rModelPart,
    const Variable<array_1d<double, 3>>& rOutputSensitivityVariable)
{
    KRATOS_TRY

    VariableUtils().SetNonHistoricalVariableToZero(rOutputSensitivityVariable, rModelPart.Nodes());

    if (rModelPart.NumberOfElements() == 0) {
        return;
    }

    KRATOS_ERROR_IF_NOT(HasVariableInProperties(rModelPart, DENSITY))
        << "DENSITY is not found in element properties of " << rModelPart.FullName() << ".\n";

    KRATOS_ERROR_IF(HasVariableInProperties(rModelPart, THICKNESS) && HasVariableInProperties(rModelPart, CROSS_AREA))
        << rModelPart.FullName()
        << " has elements with properties having both THICKNESS and CROSS_AREA. Please separate the model part such that either one of them is present in elemental properties.\n";

    const auto get_thickness = HasVariableInProperties(rModelPart, THICKNESS)
                                    ? [](const ModelPart::ElementType& rElement) { return rElement.GetProperties()[THICKNESS]; }
                                    : [](const ModelPart::ElementType& rElement) { return 1.0; };

    const auto get_cross_area = HasVariableInProperties(rModelPart, CROSS_AREA)
                                    ? [](const ModelPart::ElementType& rElement) { return rElement.GetProperties()[CROSS_AREA]; }
                                    : [](const ModelPart::ElementType& rElement) { return 1.0; };

    using VolumeDerivativeMethodType = std::function<double(IndexType, IndexType, const GeometryType&)>;

    VolumeDerivativeMethodType volume_derivative_method;
    switch (rModelPart.Elements().begin()->GetGeometry().GetGeometryType()) {
        case GeometryData::KratosGeometryType::Kratos_Line2D2:
            volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
                const double lx = rGeometry[0].X() - rGeometry[1].X();
                const double lx_derivative = ((NodeIndex == 0) - (NodeIndex == 1)) * (DirectionIndex == 0);

                const double ly = rGeometry[0].Y() - rGeometry[1].Y();
                const double ly_derivative = ((NodeIndex == 0) - (NodeIndex == 1)) * (DirectionIndex == 1);

                const double length = lx * lx + ly * ly;
                const double length_derivative = 2 * lx * lx_derivative + 2 * ly * ly_derivative;

                return 0.5 * length_derivative / std::sqrt( length );
            };
            break;
        case GeometryData::KratosGeometryType::Kratos_Line3D2:
            volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
                const double lx = rGeometry[0].X() - rGeometry[1].X();
                const double lx_derivative = ((NodeIndex == 0) - (NodeIndex == 1)) * (DirectionIndex == 0);

                const double ly = rGeometry[0].Y() - rGeometry[1].Y();
                const double ly_derivative = ((NodeIndex == 0) - (NodeIndex == 1)) * (DirectionIndex == 1);

                const double lz = rGeometry[0].Z() - rGeometry[1].Z();
                const double lz_derivative = ((NodeIndex == 0) - (NodeIndex == 1)) * (DirectionIndex == 2);

                const double length = lx * lx + ly * ly + lz * lz;
                const double length_derivative = 2 * lx * lx_derivative + 2 * ly * ly_derivative + 2 * lz * lz_derivative;

                return 0.5 * length_derivative / std::sqrt( length );
            };
            break;
        case GeometryData::KratosGeometryType::Kratos_Triangle2D3:
            volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
                return 2.0 * ElementSizeCalculator<2, 3>::AverageElementSize(rGeometry) * ElementSizeCalculator<2, 3>::AverageElementSizeDerivative(NodeIndex, DirectionIndex, rGeometry);
            };
            break;
        case GeometryData::KratosGeometryType::Kratos_Quadrilateral2D4:
            volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
                return 2.0 * ElementSizeCalculator<2, 4>::AverageElementSize(rGeometry) * ElementSizeCalculator<2, 4>::AverageElementSizeDerivative(NodeIndex, DirectionIndex, rGeometry);
            };
            break;
        case GeometryData::KratosGeometryType::Kratos_Tetrahedra3D4:
            volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
                return 3.0 * std::pow(ElementSizeCalculator<3, 4>::AverageElementSize(rGeometry), 2) * ElementSizeCalculator<3, 4>::AverageElementSizeDerivative(NodeIndex, DirectionIndex, rGeometry);
            };
            break;
        case GeometryData::KratosGeometryType::Kratos_Prism3D6:
            volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
                return 3.0 * std::pow(ElementSizeCalculator<3, 6>::AverageElementSize(rGeometry), 2) * ElementSizeCalculator<3, 6>::AverageElementSizeDerivative(NodeIndex, DirectionIndex, rGeometry);
            };
            break;
        // Following is not consistent with the DomainSize calculation, hence commented out for the time being.
        // case GeometryData::KratosGeometryType::Kratos_Hexahedra3D8:
        //     volume_derivative_method = [](IndexType NodeIndex, IndexType DirectionIndex,  const GeometryType& rGeometry) {
        //         return 3.0 * std::pow(ElementSizeCalculator<3, 8>::AverageElementSize(rGeometry), 2) * ElementSizeCalculator<3, 8>::AverageElementSizeDerivative(NodeIndex, DirectionIndex, rGeometry);
        //     };
        //     break;
        default:
            KRATOS_ERROR << "Non supported geometry type for mass shape sensitivity calculation (CalculateMassShapeSensitivity())." << std::endl;
    }

    block_for_each(rModelPart.Elements(), [&](auto& rElement){
        auto& r_geometry = rElement.GetGeometry();
        const IndexType dimension = r_geometry.WorkingSpaceDimension();

        const double density = rElement.GetProperties()[DENSITY];
        const double thickness = get_thickness(rElement);
        const double cross_area = get_cross_area(rElement);

        for (IndexType c = 0; c < r_geometry.PointsNumber(); ++c) {
            auto& r_derivative_value = r_geometry[c].GetValue(rOutputSensitivityVariable);

            for (IndexType k = 0; k < dimension; ++k) {
                const double derivative_value = volume_derivative_method(c, k, r_geometry) * thickness * density * cross_area;
                AtomicAdd(r_derivative_value[k], derivative_value);
            }
        }
    });

    rModelPart.GetCommunicator().AssembleNonHistoricalData(rOutputSensitivityVariable);

    KRATOS_CATCH("")
}

void MassResponseUtils::CalculateMassDensitySensitivity(
    ModelPart& rModelPart,
    const Variable<double>& rOutputSensitivityVariable)
{
    KRATOS_TRY

    KRATOS_ERROR_IF_NOT(HasVariableInProperties(rModelPart, DENSITY))
        << "DENSITY is not found in element properties of " << rModelPart.FullName() << ".\n";

    KRATOS_ERROR_IF(HasVariableInProperties(rModelPart, THICKNESS) && HasVariableInProperties(rModelPart, CROSS_AREA))
        << rModelPart.FullName()
        << " has elements with properties having both THICKNESS and CROSS_AREA. Please separate the model part such that either one of them is present in elemental properties.\n";

    const auto get_thickness = HasVariableInProperties(rModelPart, THICKNESS)
                                    ? [](const ModelPart::ElementType& rElement) { return rElement.GetProperties()[THICKNESS]; }
                                    : [](const ModelPart::ElementType& rElement) { return 1.0; };

    const auto get_cross_area = HasVariableInProperties(rModelPart, CROSS_AREA)
                                    ? [](const ModelPart::ElementType& rElement) { return rElement.GetProperties()[CROSS_AREA]; }
                                    : [](const ModelPart::ElementType& rElement) { return 1.0; };

    block_for_each(rModelPart.Elements(), [&](auto& rElement) {
        rElement.GetProperties().SetValue(rOutputSensitivityVariable, rElement.GetGeometry().DomainSize() * get_thickness(rElement) * get_cross_area(rElement));
    });

    KRATOS_CATCH("")
}

void MassResponseUtils::CalculateMassThicknessSensitivity(
    ModelPart& rModelPart,
    const Variable<double>& rOutputSensitivityVariable)
{
    CalculateMassGeometricalPropertySensitivity(rModelPart, THICKNESS, CROSS_AREA, rOutputSensitivityVariable);
}

void MassResponseUtils::CalculateMassCrossAreaSensitivity(
    ModelPart& rModelPart,
    const Variable<double>& rOutputSensitivityVariable)
{
    CalculateMassGeometricalPropertySensitivity(rModelPart, CROSS_AREA, THICKNESS, rOutputSensitivityVariable);
}

void MassResponseUtils::CalculateMassGeometricalPropertySensitivity(
    ModelPart& rModelPart,
    const Variable<double>& rGeometricalPropertySensitivityVariable,
    const Variable<double>& rGeometricalConflictingPropertySensitivityVariable,
    const Variable<double>& rOutputSensitivityVariable)
{
    KRATOS_TRY

    KRATOS_ERROR_IF_NOT(HasVariableInProperties(rModelPart, DENSITY))
        << "DENSITY is not found in element properties of " << rModelPart.FullName() << ".\n";

    KRATOS_ERROR_IF_NOT(HasVariableInProperties(rModelPart, rGeometricalPropertySensitivityVariable))
        << rGeometricalPropertySensitivityVariable.Name() << " is not found in element properties of "
        << rModelPart.FullName() << " which is required to compute sensitivities w.r.t. "
        << rGeometricalPropertySensitivityVariable.Name() << ".\n";

    KRATOS_ERROR_IF(HasVariableInProperties(rModelPart, rGeometricalConflictingPropertySensitivityVariable))
        << rModelPart.FullName() << " has elements with properties having both "
        << rGeometricalPropertySensitivityVariable.Name() << " and " << rGeometricalConflictingPropertySensitivityVariable.Name()
        << ". Please separate the model part such that either one of them is present in elemental properties.\n";

    block_for_each(rModelPart.Elements(), [&](auto& rElement) {
        rElement.GetProperties().SetValue(rOutputSensitivityVariable, rElement.GetGeometry().DomainSize() * rElement.GetProperties()[DENSITY]);
    });

    KRATOS_CATCH("")
}

// template instantiations
template void MassResponseUtils::CalculateSensitivity(ModelPart&, const Variable<double>&);
template void MassResponseUtils::CalculateSensitivity(ModelPart&, const Variable<array_1d<double, 3>>&);

///@}
}