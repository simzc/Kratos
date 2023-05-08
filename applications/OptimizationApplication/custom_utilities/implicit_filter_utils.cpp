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
//

// System includes
#include <cmath>

// Project includes
#include "utilities/parallel_utilities.h"
#include "utilities/reduction_utilities.h"
#include "utilities/variable_utils.h"

// Application includes

// Include base h
#include "implicit_filter_utils.h"

namespace Kratos
{

void ImplicitFilterUtils::CalculateNodeNeighbourCount(
    ModelPart& rModelPart)
{
    // Calculate number of neighbour elements for each node.
    VariableUtils().SetNonHistoricalVariableToZero(NUMBER_OF_NEIGHBOUR_ELEMENTS, rModelPart.Nodes());
    block_for_each(rModelPart.Elements(), [&](ModelPart::ElementType& rElement) {
        auto& r_geometry = rElement.GetGeometry();
        for (unsigned j = 0; j < r_geometry.PointsNumber(); ++j) {
            double& r_num_neighbour =
                r_geometry[j].GetValue(NUMBER_OF_NEIGHBOUR_ELEMENTS);
            AtomicAdd(r_num_neighbour, 1.0);
        }
    });
    rModelPart.GetCommunicator().AssembleNonHistoricalData(NUMBER_OF_NEIGHBOUR_ELEMENTS);
}

}