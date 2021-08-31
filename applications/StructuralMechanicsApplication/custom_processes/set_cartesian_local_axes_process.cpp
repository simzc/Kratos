// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:		 BSD License
//					 license: structural_mechanics_application/license.txt
//
//  Main authors:    Alejandro Cornejo
//

#include "custom_processes/set_cartesian_local_axes_process.h"
#include "utilities/parallel_utilities.h"
#include "utilities/math_utils.h"

namespace Kratos
{
SetCartesianLocalAxesProcess::SetCartesianLocalAxesProcess(
    ModelPart& rThisModelPart,
    Parameters ThisParameters
    ):mrThisModelPart(rThisModelPart),
      mThisParameters(ThisParameters)
{
    mThisParameters.ValidateAndAssignDefaults(GetDefaultParameters());
}

/***********************************************************************************/
/***********************************************************************************/

void CheckAndNormalizeVector(
    array_1d<double, 3>& rVector
    )
{
    const double norm = MathUtils<double>::Norm3(rVector);
    if (norm > std::numeric_limits<double>::epsilon()) {
        rVector /= norm;
    } else {
        KRATOS_ERROR << "The norm of one LOCAL_AXIS is null" << std::endl;
    }
}

/***********************************************************************************/
/***********************************************************************************/

void SetCartesianLocalAxesProcess::ExecuteInitialize()
{
    KRATOS_TRY

    array_1d<double, 3> local_axis_1;

    if (mrThisModelPart.GetProcessInfo()[DOMAIN_SIZE] == 3) {
        array_1d<double, 3> local_axis_2;
        const Matrix& cartesian_local_axes_matrix = mThisParameters["cartesian_local_axis"].GetMatrix();
        local_axis_1[0] = cartesian_local_axes_matrix(0, 0);
        local_axis_1[1] = cartesian_local_axes_matrix(0, 1);
        local_axis_1[2] = cartesian_local_axes_matrix(0, 2);
        local_axis_2[0] = cartesian_local_axes_matrix(1, 0);
        local_axis_2[1] = cartesian_local_axes_matrix(1, 1);
        local_axis_2[2] = cartesian_local_axes_matrix(1, 2);
        CheckAndNormalizeVector(local_axis_1);
        CheckAndNormalizeVector(local_axis_2);

        block_for_each(mrThisModelPart.Elements(), [&](Element& rElement) {
            rElement.SetValue(LOCAL_AXIS_1, local_axis_1);
            rElement.SetValue(LOCAL_AXIS_2, local_axis_2);
        });
    } else if (mrThisModelPart.GetProcessInfo()[DOMAIN_SIZE] == 2) {
        const Vector& cartesian_local_axes_matrix = mThisParameters["cartesian_local_axis"].GetVector();
        local_axis_1[0] = cartesian_local_axes_matrix[0];
        local_axis_1[1] = cartesian_local_axes_matrix[1];
        local_axis_1[2] = cartesian_local_axes_matrix[2];
        CheckAndNormalizeVector(local_axis_1);
        block_for_each(mrThisModelPart.Elements(), [&](Element& rElement) {
            rElement.SetValue(LOCAL_AXIS_1, local_axis_1);
        });
    }
    KRATOS_CATCH("")
}

/***********************************************************************************/
/***********************************************************************************/

const Parameters SetCartesianLocalAxesProcess::GetDefaultParameters() const
{
    const Parameters default_parameters = Parameters(R"(
    {
        "local_axes_coordinate_system"  : "cartesian",
        "cartesian_local_axis"          : [[1.0,0.0,0.0],[0.0,1.0,0.0]]
    })" );

    return default_parameters;
}

// class SetCartesianLocalAxesProcess
} // namespace Kratos.
