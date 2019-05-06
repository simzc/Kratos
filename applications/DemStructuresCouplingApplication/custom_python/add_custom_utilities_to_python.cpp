/*
 * Author: Miguel Angel Celigueta
 *
 *  maceli@cimne.upc.edu
 */

#include "includes/model_part.h"
#include "includes/kratos_parameters.h"

#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_utilities/dem_structures_coupling_utilities.h"
#include "custom_utilities/compute_dem_face_load_utility.h"
#include "custom_utilities/interpolate_structural_solution_for_dem_utility.h"
#include "custom_utilities/control_module_fem_dem_utilities.hpp"
#include "custom_utilities/stress_failure_check_utilities.hpp"

namespace Kratos {

    namespace Python {

        using namespace pybind11;

        void  AddCustomUtilitiesToPython(pybind11::module& m) {

            class_<DemStructuresCouplingUtilities> (m, "DemStructuresCouplingUtilities")
                .def(init<>())
                .def("TransferStructuresSkinToDem", &DemStructuresCouplingUtilities::TransferStructuresSkinToDem)
                .def("CheckProvidedProperties", &DemStructuresCouplingUtilities::CheckProvidedProperties)
                .def("SmoothLoadTrasferredToFem", &DemStructuresCouplingUtilities::SmoothLoadTrasferredToFem)
            ;

            class_<ComputeDEMFaceLoadUtility> (m, "ComputeDEMFaceLoadUtility")
                .def(init<>())
                .def("ClearDEMFaceLoads", &ComputeDEMFaceLoadUtility::ClearDEMFaceLoads)
                .def("CalculateDEMFaceLoads", &ComputeDEMFaceLoadUtility::CalculateDEMFaceLoads)
            ;

            class_<InterpolateStructuralSolutionForDEM> (m, "InterpolateStructuralSolutionForDEM")
                .def(init<>())
                .def("SaveStructuralSolution", &InterpolateStructuralSolutionForDEM::SaveStructuralSolution)
                .def("InterpolateStructuralSolution", &InterpolateStructuralSolutionForDEM::InterpolateStructuralSolution)
                .def("RestoreStructuralSolution", &InterpolateStructuralSolutionForDEM::RestoreStructuralSolution)
            ;

            class_<ControlModuleFemDemUtilities> (m, "ControlModuleFemDemUtilities")
                .def(init<ModelPart&,ModelPart&,Parameters&>())
                .def("ExecuteInitialize", &ControlModuleFemDemUtilities::ExecuteInitialize)
                .def("ExecuteInitializeSolutionStep", &ControlModuleFemDemUtilities::ExecuteInitializeSolutionStep)
                .def("ExecuteFinalizeSolutionStep", &ControlModuleFemDemUtilities::ExecuteFinalizeSolutionStep)
            ;

            class_<StressFailureCheckUtilities> (m, "StressFailureCheckUtilities")
                .def(init<ModelPart&,Parameters&>())
                .def("ExecuteFinalizeSolutionStep", &StressFailureCheckUtilities::ExecuteFinalizeSolutionStep)
            ;

        }
    }  // namespace Python
} // Namespace Kratos
