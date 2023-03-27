// KRATOS    ______            __             __  _____ __                  __                   __
//          / ____/___  ____  / /_____ ______/ /_/ ___// /________  _______/ /___  ___________ _/ /
//         / /   / __ \/ __ \/ __/ __ `/ ___/ __/\__ \/ __/ ___/ / / / ___/ __/ / / / ___/ __ `/ / 
//        / /___/ /_/ / / / / /_/ /_/ / /__/ /_ ___/ / /_/ /  / /_/ / /__/ /_/ /_/ / /  / /_/ / /  
//        \____/\____/_/ /_/\__/\__,_/\___/\__//____/\__/_/   \__,_/\___/\__/\__,_/_/   \__,_/_/  MECHANICS
//
//  License:         BSD License
//                   license: ContactStructuralMechanicsApplication/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

// System includes
#include <cmath>

// External includes

// Project includes
/* Mortar includes */
#include "custom_conditions/penalty_frictional_mortar_contact_axisym_condition.h"

/* Utilities */

namespace Kratos
{
/************************************* OPERATIONS **********************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
Condition::Pointer PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes,TNormalVariation, TNumNodesMaster>::Create(
    IndexType NewId,
    NodesArrayType const& rThisNodes,
    PropertiesPointerType pProperties ) const
{
    return Kratos::make_intrusive< PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes, TNormalVariation, TNumNodesMaster > >( NewId, this->GetParentGeometry().Create( rThisNodes ), pProperties );
}

/***********************************************************************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
Condition::Pointer PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes,TNormalVariation, TNumNodesMaster>::Create(
    IndexType NewId,
    GeometryPointerType pGeom,
    PropertiesPointerType pProperties) const
{
    return Kratos::make_intrusive< PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes, TNormalVariation, TNumNodesMaster> >( NewId, pGeom, pProperties );
}

/***********************************************************************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
Condition::Pointer PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes,TNormalVariation, TNumNodesMaster>::Create(
    IndexType NewId,
    GeometryPointerType pGeom,
    PropertiesPointerType pProperties,
    GeometryPointerType pMasterGeom) const
{
    return Kratos::make_intrusive< PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes, TNormalVariation, TNumNodesMaster> >( NewId, pGeom, pProperties, pMasterGeom );
}

/************************************* DESTRUCTOR **********************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes, TNormalVariation, TNumNodesMaster>::~PenaltyMethodFrictionalMortarContactAxisymCondition( )
= default;

/***********************************************************************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
bool PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes,TNormalVariation, TNumNodesMaster>::IsAxisymmetric() const
{
    return true;
}

/***********************************************************************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
double PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes,TNormalVariation, TNumNodesMaster>::GetAxisymmetricCoefficient(const GeneralVariables& rVariables) const
{
    const double radius = CalculateRadius(rVariables);
    const double thickness = this->GetProperties()[THICKNESS];
    return (2.0 * Globals::Pi * radius/thickness);
}

/*************************COMPUTE AXYSIMMETRIC RADIUS*******************************/
/***********************************************************************************/

template< std::size_t TNumNodes, bool TNormalVariation, std::size_t TNumNodesMaster >
double PenaltyMethodFrictionalMortarContactAxisymCondition<TNumNodes,TNormalVariation, TNumNodesMaster>::CalculateRadius(const GeneralVariables& rVariables) const
{
    KRATOS_TRY;

    double current_radius = 0.0;

    for (IndexType i_node = 0; i_node < TNumNodes; ++i_node) {
        // Displacement from the reference to the current configuration
        const array_1d<double, 3 >& r_current_position = this->GetParentGeometry()[i_node].Coordinates();

        current_radius += r_current_position[0] * rVariables.NSlave[i_node];
    }

    return current_radius;

    KRATOS_CATCH( "" );
}

/***********************************************************************************/
/***********************************************************************************/

template class PenaltyMethodFrictionalMortarContactAxisymCondition<2, false>;
template class PenaltyMethodFrictionalMortarContactAxisymCondition<2, true>;

} // Namespace Kratos
