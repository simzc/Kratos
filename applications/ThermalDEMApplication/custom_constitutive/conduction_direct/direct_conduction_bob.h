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

#if !defined(DIRECT_CONDUCTION_MODEL_BOB_H_INCLUDED)
#define DIRECT_CONDUCTION_MODEL_BOB_H_INCLUDED

// System includes

// External includes

// Project includes
#include "direct_conduction_model.h"

namespace Kratos
{
  class KRATOS_API(THERMAL_DEM_APPLICATION) DirectConductionBOB : public DirectConductionModel
  {
    public:

      // Pointer definition of DirectConductionBOB
      KRATOS_CLASS_POINTER_DEFINITION(DirectConductionBOB);

      // Constructor / Destructor
      DirectConductionBOB();
      virtual ~DirectConductionBOB();

      // Public methods
      double ComputeHeatFlux(const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;

      // Clone
      HeatExchangeMechanism* CloneRaw() const override {
        HeatExchangeMechanism* cloned_model(new DirectConductionBOB(*this));
        return cloned_model;
      }

      HeatExchangeMechanism::Pointer CloneShared() const override {
        HeatExchangeMechanism::Pointer cloned_model(new DirectConductionBOB(*this));
        return cloned_model;
      }

      // Print information about this object
      virtual std::string Info() const override {
        std::stringstream buffer;
        buffer << "DirectConductionBOB";
        return buffer.str();
      }

      // Print object information
      virtual void PrintInfo(std::ostream& rOStream) const override { rOStream << "DirectConductionBOB"; }
      virtual void PrintData(std::ostream& rOStream) const override {}

    private:

      // Assignment operator
      DirectConductionBOB& operator=(DirectConductionBOB const& rOther) {
        return *this;
      }

      // Copy constructor
      DirectConductionBOB(DirectConductionBOB const& rOther) {
        *this = rOther;
      }

  }; // Class DirectConductionBOB

  // input stream function
  inline std::istream& operator>>(std::istream& rIStream,
    DirectConductionBOB& rThis) {
    return rIStream;
  }

  // output stream function
  inline std::ostream& operator<<(std::ostream& rOStream,
    const DirectConductionBOB& rThis) {
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
  }

} // namespace Kratos

#endif // DIRECT_CONDUCTION_MODEL_BOB_H_INCLUDED
