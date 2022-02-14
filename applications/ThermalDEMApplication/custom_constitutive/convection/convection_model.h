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

#if !defined(CONVECTION_MODEL_H_INCLUDED)
#define CONVECTION_MODEL_H_INCLUDED

// System includes

// External includes

// Project includes
#include "custom_constitutive/heat_exchange_mechanism.h"
#include "custom_elements/thermal_spheric_particle.h"

namespace Kratos
{
  class KRATOS_API(THERMAL_DEM_APPLICATION) ConvectionModel : public HeatExchangeMechanism
  {
    public:

      // Pointer definition of ConvectionModel
      KRATOS_CLASS_POINTER_DEFINITION(ConvectionModel);

      // Constructor / Destructor
      ConvectionModel();
      virtual ~ConvectionModel();

      // Public methods
      void           SetHeatExchangeMechanismInProperties (Properties::Pointer pProp, bool verbose = true) const override;
      double         ComputeHeatFlux                      (const ProcessInfo& r_process_info, ThermalSphericParticle* particle) override;
      virtual double ComputeNusseltNumber                 (const ProcessInfo& r_process_info, ThermalSphericParticle* particle);

      // Clone
      HeatExchangeMechanism* CloneRaw() const override {
        HeatExchangeMechanism* cloned_model(new ConvectionModel(*this));
        return cloned_model;
      }

      HeatExchangeMechanism::Pointer CloneShared() const override {
        HeatExchangeMechanism::Pointer cloned_model(new ConvectionModel(*this));
        return cloned_model;
      }

      // Print information about this object
      virtual std::string Info() const override {
        std::stringstream buffer;
        buffer << "ConvectionModel";
        return buffer.str();
      }

      // Print object information
      virtual void PrintInfo(std::ostream& rOStream) const override { rOStream << "ConvectionModel"; }
      virtual void PrintData(std::ostream& rOStream) const override {}

    private:

      // Assignment operator
      ConvectionModel& operator=(ConvectionModel const& rOther) {
        return *this;
      }

      // Copy constructor
      ConvectionModel(ConvectionModel const& rOther) {
        *this = rOther;
      }

  }; // Class ConvectionModel

  // input stream function
  inline std::istream& operator>>(std::istream& rIStream,
    ConvectionModel& rThis) {
    return rIStream;
  }

  // output stream function
  inline std::ostream& operator<<(std::ostream& rOStream,
    const ConvectionModel& rThis) {
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
  }

} // namespace Kratos

#endif // CONVECTION_MODEL_H_INCLUDED
