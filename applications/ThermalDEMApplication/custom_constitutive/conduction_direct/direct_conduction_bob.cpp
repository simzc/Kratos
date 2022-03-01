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
#include "direct_conduction_bob.h"

namespace Kratos {
  //-----------------------------------------------------------------------------------------------------------------------
  DirectConductionBOB::DirectConductionBOB() {}
  DirectConductionBOB::~DirectConductionBOB() {}

  //------------------------------------------------------------------------------------------------------------
  double DirectConductionBOB::ComputeHeatFlux(const ProcessInfo& r_process_info, ThermalSphericParticle* particle) {
    KRATOS_TRY

    // Check for contact
    if (!particle->mNeighborInContact)
      return 0.0;

    const double Rc        = particle->mContactRadiusAdjusted;
    const double keff      = particle->ComputeEffectiveConductivity();
    const double temp_grad = particle->GetNeighborTemperature() - particle->GetParticleTemperature();

    return 4.0 * keff * Rc * temp_grad;

    KRATOS_CATCH("")
  }

} // namespace Kratos
