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

#if !defined(INDIRECT_CONDUCTION_MODEL_H_INCLUDED)
#define INDIRECT_CONDUCTION_MODEL_H_INCLUDED

// System includes

// External includes

// Project includes
#include "custom_constitutive/heat_exchange_mechanism.h"
#include "custom_elements/thermal_spheric_particle.h"

namespace Kratos
{
  class KRATOS_API(THERMAL_DEM_APPLICATION) IndirectConductionModel : public HeatExchangeMechanism
  {
    public:

      // Pointer definition
      KRATOS_CLASS_POINTER_DEFINITION(IndirectConductionModel);

      // Constructor / Destructor
      IndirectConductionModel();
      virtual ~IndirectConductionModel();

      // Public methods
      void   SetHeatExchangeMechanismInProperties (Properties::Pointer pProp, bool verbose = true) const override;
      double GetSearchDistance                    (const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;
      double ComputeHeatFlux                      (const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;

      // Clone
      HeatExchangeMechanism* CloneRaw() const override {
        HeatExchangeMechanism* cloned_model(new IndirectConductionModel(*this));
        return cloned_model;
      }

      HeatExchangeMechanism::Pointer CloneShared() const override {
        HeatExchangeMechanism::Pointer cloned_model(new IndirectConductionModel(*this));
        return cloned_model;
      }

      // Print information about this object
      virtual std::string Info() const override {
        std::stringstream buffer;
        buffer << "IndirectConductionModel";
        return buffer.str();
      }

      // Print object information
      virtual void PrintInfo(std::ostream& rOStream) const override { rOStream << "IndirectConductionModel"; }
      virtual void PrintData(std::ostream& rOStream) const override {}

    private:

      // Assignment operator / Copy constructor
      IndirectConductionModel& operator=(IndirectConductionModel const& rOther) {return *this;}
      IndirectConductionModel(IndirectConductionModel const& rOther) {*this = rOther;}

  }; // Class IndirectConductionModel

  // input stream function
  inline std::istream& operator>>(std::istream& rIStream,
    IndirectConductionModel& rThis) {
    return rIStream;
  }

  // output stream function
  inline std::ostream& operator<<(std::ostream& rOStream,
    const IndirectConductionModel& rThis) {
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
  }

} // namespace Kratos

#endif // INDIRECT_CONDUCTION_MODEL_H_INCLUDED
