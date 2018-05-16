// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: structural_mechanics_application/license.txt
//
//  Main authors:    Alejandro Cornejo
//

#if !defined (KRATOS_GENERIC_SMALL_STRAIN_ISOTROPIC_PLASTICITY_3D_H_INCLUDED)
#define  KRATOS_GENERIC_SMALL_STRAIN_ISOTROPIC_PLASTICITY_3D_H_INCLUDED

// System includes
#include <string>
#include <iostream>

// Project includes
#include "includes/define.h"
#include "includes/serializer.h"
#include "includes/properties.h"
#include "utilities/math_utils.h"

#include "includes/constitutive_law.h"
#include "structural_mechanics_application_variables.h"

namespace Kratos
{
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
/**
 * @class GenericConstitutiveLawIntegrator
 * @ingroup StructuralMechanicsApplication
 * @brief
 * @details
 * @author Alejandro Cornejo
 */
template <class ConstLawIntegratorType>
class KRATOS_API(STRUCTURAL_MECHANICS_APPLICATION) GenericSmallStrainIsotropicPlasticity3D
    : public ConstitutiveLaw
{
public:
    ///@name Type Definitions
    ///@{

    /// Counted pointer of GenericYieldSurface
    KRATOS_CLASS_POINTER_DEFINITION(GenericSmallStrainIsotropicPlasticity3D);

    ///@}
    ///@name Life Cycle
    ///@{

    /**
    * Default constructor.
    */
    GenericSmallStrainIsotropicPlasticity3D()
    {
    }

    /**
    * Clone.
    */
    ConstitutiveLaw::Pointer Clone() const override
    {
        GenericSmallStrainIsotropicPlasticity3D<YieldSurfaceType, ConstLawIntegratorType>::Pointer p_clone
            (new GenericSmallStrainIsotropicPlasticity3D< YieldSurfaceType,  ConstLawIntegratorType>(*this));
        return p_clone;
    }

    /**
    * Copy constructor.
    */
    GenericSmallStrainIsotropicPlasticity3D (const GenericSmallStrainIsotropicPlasticity3D& rOther)
    : ConstitutiveLaw(rOther)
    {
    }
    /**
    * Destructor.
    */
    ~GenericSmallStrainIsotropicPlasticity3D() override
    {
    }

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    int GetVoigtSize(){return 6;}
    int GetWorkingSpaceDimension() {return 3;}

    double GetThreshold() {return mThreshold;}
    double GetPlasticDissipation() {return mPlasticDissipation;}
    Vector GetPlasticStrain(){return mPlasticStrain;}

    double GetNonConvThreshold() {return mNonConvThreshold;}
    double GetNonConvPlasticDissipation() {return mNonConvPlasticDissipation;}
    Vector GetNonConvPlasticStrain(){return mNonConvPlasticStrain;}

    void SetThreshold(const double& toThreshold) {mThreshold = toThreshold;}
    void SetPlasticDissipation(const double& toCapap) {mPlasticDissipation = toCapap;}
    void SetPlasticStrain(const Vector& Ep){mPlasticStrain = Ep;}

    void SetNonConvThreshold(const double& toThreshold) {mNonConvThreshold = toThreshold;}
    void SetNonConvPlasticDissipation(const double& toCapap) {mNonConvPlasticDissipation = toCapap;}
    void SetNonConvPlasticStrain(const Vector& Ep){mNonConvPlasticStrain = Ep;}


    void CalculateMaterialResponsePK1(ConstitutiveLaw::Parameters& rValues)
    {
        this->CalculateMaterialResponseCauchy(rValues);
    }
    void CalculateMaterialResponsePK2(ConstitutiveLaw::Parameters& rValues)
    {
        this->CalculateMaterialResponseCauchy(rValues);
    }
    void CalculateMaterialResponseKirchhoff(ConstitutiveLaw::Parameters& rValues)
    {
        this->CalculateMaterialResponseCauchy(rValues);
    }

    void CalculateMaterialResponseCauchy(ConstitutiveLaw::Parameters& rValues)
    {
        // Integrate Stress plasticity
        const Properties& rMaterialProperties = rValues.GetMaterialProperties();
        int VoigtSize = this->GetVoigtSize();
        Vector& IntegratedStressVector = rValues.GetStressVector();
        Matrix& tangent_tensor = rValues.GetConstitutiveMatrix(); // todo modify after integration

        // Elastic Matrix
        Matrix C;
        this->CalculateElasticMatrix(C, rMaterialProperties);

        double Kp = 0.0, Capap = 0.0;
        Vector plastic_strain = ZeroVector(this->GetVoigtSize());

        Kp             = this->GetThreshold();
        Capap          = this->GetPlasticDissipation();
        plastic_strain = this->GetPlasticStrain();

        // S0 = C:(E-Ep)
        Vector PredictiveStressVector = prod(C, rValues.GetStrainVector()-plastic_strain);

        // Initialize Plastic Parameters
        double UniaxialStress = 0.0, PlasticDenominator = 0.0;
        Vector Fflux = ZeroVector(VoigtSize), Gflux = ZeroVector(VoigtSize); // DF/DS & DG/DS
        Vector PlasticStrainIncrement = ZeroVector(VoigtSize);

        ConstLawIntegratorType::CalculatePlasticParameters(PredictiveStressVector, rValues.GetStrainVector(),
            UniaxialStress, Kp, PlasticDenominator, Fflux, Gflux, Capap, PlasticStrainIncrement, C);

        double F = UniaxialStress - Kp;

        if (F <= std::abs(1.0e-8 * Kp)) { // Elastic case
            IntegratedStressVector = PredictiveStressVector;
            tangent_tensor = C;
            this->SetNonConvPlasticDissipation(Capap);
            this->SetNonConvPlasticStrain(plastic_strain);
            this->SetNonConvThreshold(Kp);
        } 
        else 
        {    // Plastic case

            // while loop backward euler
            /* Inside IntegrateStressVector the PredictiveStressVector
               is updated to verify the yield criterion */

            ConstLawIntegratorType::IntegrateStressVector(PredictiveStressVector, rValues.GetStrainVector(), 
                UniaxialStress, Kp, PlasticDenominator, Fflux, Gflux, Capap, PlasticStrainIncrement, 
                C, plastic_strain);

            this->SetNonConvPlasticDissipation(Capap);
            this->SetNonConvPlasticStrain(plastic_strain);
            this->SetNonConvThreshold(Kp);

            this->CalculateTangentTensor(C);
            tangent_tensor = C;
        }


    } // End CalculateMaterialResponseCauchy

    void CalculateTangentTensor(Matrix& C) 
    {

    }

    void CalculateElasticMatrix(Matrix &rElasticityTensor,
        const Properties &rMaterialProperties
    )
    {
        const double E = rMaterialProperties[YOUNG_MODULUS];
        const double poisson_ratio = rMaterialProperties[POISSON_RATIO];
        const double lambda =
            E * poisson_ratio / ((1. + poisson_ratio) * (1.0 - 2.0 * poisson_ratio));
        const double mu = E / (2.0 + 2.0 * poisson_ratio);

        if (rElasticityTensor.size1() != 6 || rElasticityTensor.size2() != 6)
            rElasticityTensor.resize(6, 6, false);
        rElasticityTensor.clear();

        rElasticityTensor(0, 0) = lambda + 2.0 * mu;
        rElasticityTensor(0, 1) = lambda;
        rElasticityTensor(0, 2) = lambda;
        rElasticityTensor(1, 0) = lambda;
        rElasticityTensor(1, 1) = lambda + 2.0 * mu;
        rElasticityTensor(1, 2) = lambda;
        rElasticityTensor(2, 0) = lambda;
        rElasticityTensor(2, 1) = lambda;
        rElasticityTensor(2, 2) = lambda + 2.0 * mu;
        rElasticityTensor(3, 3) = mu;
        rElasticityTensor(4, 4) = mu;
        rElasticityTensor(5, 5) = mu;
    }

    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    ///@}
    ///@name Friends
    ///@{

    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{

    ///@}
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operators
    ///@{

    ///@}
    ///@name Protected Operations
    ///@{

    ///@}
    ///@name Protected  Access
    ///@{

    ///@}
    ///@name Protected Inquiry
    ///@{

    ///@}
    ///@name Protected LifeCycle
    ///@{

    ///@}
private:
    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    // Converged values
    double mPlasticDissipation = 0.0;
    double mThreshold = 0.0;
    Vector mPlasticStrain = ZeroVector(this->GetVoigtSize());

    // Non Converged values
    double mNonConvPlasticDissipation = 0.0;
    double mNonConvThreshold = 0.0;
    Vector mNonConvPlasticStrain = ZeroVector(this->GetVoigtSize());

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    ///@}
    ///@name Private  Access
    ///@{

    ///@}
    ///@name Private Inquiry
    ///@{

    ///@}
    ///@name Un accessible methods
    ///@{

    // Serialization

    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, ConstitutiveLaw )
        rSerializer.save("PlasticDissipation", mPlasticDissipation);
        rSerializer.save("Threshold", mThreshold);
        rSerializer.save("PlasticStrain", mPlasticStrain);
        rSerializer.save("NonConvPlasticDissipation", mNonConvPlasticDissipation);
        rSerializer.save("NonConvThreshold", mNonConvThreshold);
        rSerializer.save("NonConvPlasticStrain", mNonConvPlasticStrain);
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, ConstitutiveLaw)
        rSerializer.load("PlasticDissipation", mPlasticDissipation);
        rSerializer.load("Threshold", mThreshold);
        rSerializer.load("PlasticStrain", mPlasticStrain);
        rSerializer.load("NonConvPlasticDissipation", mNonConvPlasticDissipation);
        rSerializer.load("NonConvThreshold", mNonConvThreshold);
        rSerializer.load("NonConvPlasticStrain", mNonConvPlasticStrain);
    }

    ///@}

}; // Class GenericYieldSurface

} // namespace kratos
#endif
