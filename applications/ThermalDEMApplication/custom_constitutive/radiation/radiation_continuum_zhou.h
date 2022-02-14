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

#if !defined(RADIATION_MODEL_CONTINUUM_ZHOU_H_INCLUDED)
#define RADIATION_MODEL_CONTINUUM_ZHOU_H_INCLUDED

// System includes

// External includes

// Project includes
#include "radiation_model.h"

namespace Kratos
{
  class KRATOS_API(THERMAL_DEM_APPLICATION) RadiationContinuumZhou : public RadiationModel
  {
    public:

      // Pointer definition
      KRATOS_CLASS_POINTER_DEFINITION(RadiationContinuumZhou);

      // Constructor / Destructor
      RadiationContinuumZhou();
      virtual ~RadiationContinuumZhou();

      // Public methods
      double ComputeHeatFlux                  (const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;
      double FinalizeHeatFlux                 (const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;
      void   AccumulateEnvironmentTemperature (const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;

      // Clone
      HeatExchangeMechanism* CloneRaw() const override {
        HeatExchangeMechanism* cloned_model(new RadiationContinuumZhou(*this));
        return cloned_model;
      }

      HeatExchangeMechanism::Pointer CloneShared() const override {
        HeatExchangeMechanism::Pointer cloned_model(new RadiationContinuumZhou(*this));
        return cloned_model;
      }

      // Print information about this object
      virtual std::string Info() const override {
        std::stringstream buffer;
        buffer << "RadiationContinuumZhou";
        return buffer.str();
      }

      // Print object information
      virtual void PrintInfo(std::ostream& rOStream) const override { rOStream << "RadiationContinuumZhou"; }
      virtual void PrintData(std::ostream& rOStream) const override {}

    private:

      // Assignment operator
      RadiationContinuumZhou& operator=(RadiationContinuumZhou const& rOther) {
        return *this;
      }

      // Copy constructor
      RadiationContinuumZhou(RadiationContinuumZhou const& rOther) {
        *this = rOther;
      }

  }; // Class RadiationContinuumZhou

  // input stream function
  inline std::istream& operator>>(std::istream& rIStream,
    RadiationContinuumZhou& rThis) {
    return rIStream;
  }

  // output stream function
  inline std::ostream& operator<<(std::ostream& rOStream,
    const RadiationContinuumZhou& rThis) {
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
  }

} // namespace Kratos

#endif // RADIATION_MODEL_CONTINUUM_ZHOU_H_INCLUDED
