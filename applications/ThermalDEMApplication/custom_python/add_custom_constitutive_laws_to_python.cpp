//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics ThermalDEM Application
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Rafael Rangel (rrangel@cimne.upc.edu)
//

// System includes

// External includes

// Project includes
#include "custom_python/add_custom_constitutive_laws_to_python.h"
#include "custom_constitutive/heat_exchange_mechanism.h"
#include "custom_constitutive/heat_generation_mechanism.h"
#include "custom_constitutive/conduction_direct/direct_conduction_model.h"
#include "custom_constitutive/conduction_direct/direct_conduction_bob.h"
#include "custom_constitutive/conduction_direct/direct_conduction_collision.h"
#include "custom_constitutive/conduction_direct/direct_conduction_pipe.h"
#include "custom_constitutive/conduction_indirect/indirect_conduction_model.h"
#include "custom_constitutive/conduction_indirect/indirect_conduction_surround_layer.h"
#include "custom_constitutive/conduction_indirect/indirect_conduction_vargas.h"
#include "custom_constitutive/conduction_indirect/indirect_conduction_voronoi_a.h"
#include "custom_constitutive/conduction_indirect/indirect_conduction_voronoi_b.h"
#include "custom_constitutive/convection/convection_model.h"
#include "custom_constitutive/convection/nusselt_gunn.h"
#include "custom_constitutive/convection/nusselt_hanz_marshall.h"
#include "custom_constitutive/convection/nusselt_li_mason.h"
#include "custom_constitutive/convection/nusselt_whitaker.h"
#include "custom_constitutive/radiation/radiation_model.h"
#include "custom_constitutive/radiation/radiation_continuum_krause.h"
#include "custom_constitutive/radiation/radiation_continuum_zhou.h"
#include "custom_constitutive/friction/friction_model.h"
#include "custom_constitutive/friction/friction_coulomb.h"

namespace Kratos
{
  namespace Python
  {
    namespace py = pybind11;

    void AddCustomConstitutiveLawsToPython(pybind11::module& m) {

      py::class_<HeatExchangeMechanism, HeatExchangeMechanism::Pointer>(m, "HeatExchangeMechanism")
        .def(py::init<>())
        .def("SetHeatExchangeMechanismInProperties", &HeatExchangeMechanism::SetHeatExchangeMechanismInProperties);

      py::class_<Variable<HeatExchangeMechanism::Pointer>, Variable<HeatExchangeMechanism::Pointer>::Pointer>(m, "HeatExchangeMechanismPointerVariable")
        .def("__str__", &Variable<HeatExchangeMechanism::Pointer>::Info);

      py::class_<Variable<HeatExchangeMechanism*>, Variable<HeatExchangeMechanism*>::Pointer>(m, "HeatExchangeMechanismRawPointerVariable")
        .def("__str__", &Variable<HeatExchangeMechanism*>::Info);


      py::class_<HeatGenerationMechanism, HeatGenerationMechanism::Pointer>(m, "HeatGenerationMechanism")
        .def(py::init<>())
        .def("SetHeatGenerationMechanismInProperties", &HeatGenerationMechanism::SetHeatGenerationMechanismInProperties);

      py::class_<Variable<HeatGenerationMechanism::Pointer>, Variable<HeatGenerationMechanism::Pointer>::Pointer>(m, "HeatGenerationMechanismPointerVariable")
        .def("__str__", &Variable<HeatGenerationMechanism::Pointer>::Info);

      py::class_<Variable<HeatGenerationMechanism*>, Variable<HeatGenerationMechanism*>::Pointer>(m, "HeatGenerationMechanismRawPointerVariable")
        .def("__str__", &Variable<HeatGenerationMechanism*>::Info);


      // Direct conduction ----------------------------------------------------------------------------------------------------------
      py::class_<DirectConductionModel, DirectConductionModel::Pointer, HeatExchangeMechanism>(m, "DirectConductionModel")
        .def(py::init<>());

      py::class_<DirectConductionBOB, DirectConductionBOB::Pointer, DirectConductionModel>(m, "DirectConductionBOB")
        .def(py::init<>());

      py::class_<DirectConductionCollision, DirectConductionCollision::Pointer, DirectConductionModel>(m, "DirectConductionCollision")
        .def(py::init<>());

      py::class_<DirectConductionPipe, DirectConductionPipe::Pointer, DirectConductionModel>(m, "DirectConductionPipe")
        .def(py::init<>());


      // Indirect conduction --------------------------------------------------------------------------------------------------------
      py::class_<IndirectConductionModel, IndirectConductionModel::Pointer, HeatExchangeMechanism>(m, "IndirectConductionModel")
        .def(py::init<>());

      py::class_<IndirectConductionSurroundLayer, IndirectConductionSurroundLayer::Pointer, IndirectConductionModel>(m, "IndirectConductionSurroundLayer")
        .def(py::init<>());

      py::class_<IndirectConductionVargas, IndirectConductionVargas::Pointer, IndirectConductionModel>(m, "IndirectConductionVargas")
        .def(py::init<>());

      py::class_<IndirectConductionVoronoiA, IndirectConductionVoronoiA::Pointer, IndirectConductionModel>(m, "IndirectConductionVoronoiA")
        .def(py::init<>());

      py::class_<IndirectConductionVoronoiB, IndirectConductionVoronoiB::Pointer, IndirectConductionModel>(m, "IndirectConductionVoronoiB")
        .def(py::init<>());


      // Convection -----------------------------------------------------------------------------------------------------------------
      py::class_<ConvectionModel, ConvectionModel::Pointer, HeatExchangeMechanism>(m, "ConvectionModel")
        .def(py::init<>());

      py::class_<NusseltGunn, NusseltGunn::Pointer, ConvectionModel>(m, "NusseltGunn")
        .def(py::init<>());

      py::class_<NusseltHanzMarshall, NusseltHanzMarshall::Pointer, ConvectionModel>(m, "NusseltHanzMarshall")
        .def(py::init<>());

      py::class_<NusseltLiMason, NusseltLiMason::Pointer, ConvectionModel>(m, "NusseltLiMason")
        .def(py::init<>());

      py::class_<NusseltWhitaker, NusseltWhitaker::Pointer, ConvectionModel>(m, "NusseltWhitaker")
        .def(py::init<>());


      // Radiation ------------------------------------------------------------------------------------------------------------------
      py::class_<RadiationModel, RadiationModel::Pointer, HeatExchangeMechanism>(m, "RadiationModel")
        .def(py::init<>());

      py::class_<RadiationContinuumKrause, RadiationContinuumKrause::Pointer, RadiationModel>(m, "RadiationContinuumKrause")
        .def(py::init<>());

      py::class_<RadiationContinuumZhou, RadiationContinuumZhou::Pointer, RadiationModel>(m, "RadiationContinuumZhou")
        .def(py::init<>());


      // Friction -------------------------------------------------------------------------------------------------------------------
      py::class_<FrictionModel, FrictionModel::Pointer, HeatGenerationMechanism>(m, "FrictionModel")
        .def(py::init<>());

      py::class_<FrictionCoulomb, FrictionCoulomb::Pointer, FrictionModel>(m, "FrictionCoulomb")
        .def(py::init<>());
    }

  } // namespace Python
} // namespace Kratos
