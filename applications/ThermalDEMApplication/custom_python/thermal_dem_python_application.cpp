//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics ThermalDEM Application
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Rafael Rangel (rrangel@cimne.upc.edu)
//

#if defined(KRATOS_PYTHON)

// System includes
#include <pybind11/pybind11.h>

// External includes

// Project includes
#include "includes/define_python.h"
#include "includes/define.h"
#include "thermal_dem_application.h"
#include "thermal_dem_application_variables.h"
#include "custom_python/add_custom_constitutive_laws_to_python.h"
#include "custom_python/add_custom_processes_to_python.h"
#include "custom_python/add_custom_strategies_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"

namespace Kratos
{
namespace Python
{

namespace py = pybind11;

PYBIND11_MODULE(KratosThermalDEMApplication, m)
{
    py::class_<KratosThermalDEMApplication, KratosThermalDEMApplication::Pointer, KratosApplication>(m, "KratosThermalDEMApplication")
        .def(py::init<>())
        ;

    AddCustomConstitutiveLawsToPython(m);
    AddCustomProcessesToPython(m);
    AddCustomStrategiesToPython(m);
    AddCustomUtilitiesToPython(m);

    // Registering variables in python
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, DIRECT_CONDUCTION_MODEL)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, INDIRECT_CONDUCTION_MODEL)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, CONVECTION_MODEL)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, RADIATION_MODEL)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ADJUSTED_CONTACT_MODEL)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, VORONOI_METHOD)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, POSORITY_METHOD)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, MOTION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, DIRECT_CONDUCTION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, INDIRECT_CONDUCTION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, CONVECTION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, RADIATION_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FRICTION_HEAT_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ADJUSTED_CONTACT_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, TEMPERATURE_DEPENDENT_RADIUS_OPTION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FIXED_TEMPERATURE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ADIABATIC)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, THERMAL_FREQUENCY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, THERMAL_CONDUCTIVITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, REAL_YOUNG_MODULUS_RATIO)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, HEATFLUX)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, HEATSOURCE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, MIN_CONDUCTION_DISTANCE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, MAX_CONDUCTION_DISTANCE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ISOTHERMAL_CORE_RADIUS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, MAX_RADIATION_DISTANCE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FRICTION_HEAT_CONVERSION)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, AVERAGE_POROSITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ALPHA_SHAPE_PARAMETER)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, INTEGRAL_TOLERANCE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_LAYER_THICKNESS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_DENSITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_VISCOSITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_THERMAL_CONDUCTIVITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_HEAT_CAPACITY)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_TEMPERATURE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, FLUID_VELOCITY)
}

} // namespace Python
} // namespace Kratos

#endif // KRATOS_PYTHON defined
