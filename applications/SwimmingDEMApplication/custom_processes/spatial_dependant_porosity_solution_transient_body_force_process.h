//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Joaquin Gonzalez-Usua
//
//

#ifndef KRATOS_SPATIAL_DEPENDANT_POROSITY_SOLUTION_TRANSIENT_BODY_FORCE_PROCESS_H
#define KRATOS_SPATIAL_DEPENDANT_POROSITY_SOLUTION_TRANSIENT_BODY_FORCE_PROCESS_H

// System includes
#include <string>
#include <iostream>

// External includes

// Project includes
#include "includes/define.h"
#include "processes/process.h"
#include "includes/kratos_parameters.h"

// Application includes


namespace Kratos
{
///@addtogroup SwimmingDEMApplication
///@{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

class KRATOS_API(SWIMMING_DEM_APPLICATION) SpatialDependantPorositySolutionTransientBodyForceProcess : public Process
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of SpatialDependantPorositySolutionTransientBodyForceProcess
    KRATOS_CLASS_POINTER_DEFINITION(SpatialDependantPorositySolutionTransientBodyForceProcess);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Constructor.
    SpatialDependantPorositySolutionTransientBodyForceProcess(
        ModelPart& rModelPart);

    /// Constructor with Kratos parameters.
    SpatialDependantPorositySolutionTransientBodyForceProcess(
        ModelPart& rModelPart,
        Parameters& rParameters);

    /// Constructor with Kratos model
    SpatialDependantPorositySolutionTransientBodyForceProcess(
        Model& rModel,
        Parameters& rParameters);

    /// Destructor.
    ~SpatialDependantPorositySolutionTransientBodyForceProcess() override {}

    ///@}
    ///@name Operators
    ///@{

    void Execute() override;

    void ExecuteInitialize() override;

    void ExecuteBeforeSolutionLoop() override;

    void ExecuteInitializeSolutionStep() override;

    void ExecuteFinalizeSolutionStep() override;

    ///@}
    ///@name Operations
    ///@{

    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "SpatialDependantPorositySolutionTransientBodyForceProcess" ;
        return buffer.str();
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override {rOStream << "SpatialDependantPorositySolutionTransientBodyForceProcess";}

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override {}


    ///@}
    ///@name Friends
    ///@{

    ///@}

    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    ModelPart&                                       mrModelPart;
    double                                              mDensity;
    double                                            mViscosity;
    double                                      mIndependentTerm;
    double                                         mMaximumAlpha;
    double                                             mCenterx1;
    double                                             mCenterx2;
    bool                                      mInitialConditions;

    ///@}
    ///@name Protected Operators
    ///@{

    void CheckDefaultsAndProcessSettings(Parameters &rParameters);

    const Parameters GetDefaultParameters() const override;

    void SetInitialBodyForceAndPorosityField();

    void SetBodyForceAndPorosityField();

    void SetFluidProperties();

    ///@}
private:

    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Default constructor.
    SpatialDependantPorositySolutionTransientBodyForceProcess() = delete;

    /// Assignment operator.
    SpatialDependantPorositySolutionTransientBodyForceProcess& operator=(SpatialDependantPorositySolutionTransientBodyForceProcess const& rOther) = delete;

    /// Copy constructor.
    SpatialDependantPorositySolutionTransientBodyForceProcess(SpatialDependantPorositySolutionTransientBodyForceProcess const& rOther) = delete;

    ///@}

}; // Class SpatialDependantPorositySolutionTransientBodyForceProcess

///@}
///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

///@}

///@} addtogroup block

};  // namespace Kratos.

#endif // KRATOS_SPATIAL_DEPENDANT_POROSITY_SOLUTION_TRANSIENT_BODY_FORCE_PROCESS_H