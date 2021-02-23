// KRATOS___
//     //   ) )
//    //         ___      ___
//   //  ____  //___) ) //   ) )
//  //    / / //       //   / /
// ((____/ / ((____   ((___/ /  MECHANICS
//
//  License:         geo_mechanics_application/license.txt
//
//  Main authors:    Vahid Galavi
//

#include "geo_mechanics_application_variables.h"

namespace Kratos
{
/* Variables from GeoMechanicsApplication */

KRATOS_CREATE_VARIABLE( double, VELOCITY_COEFFICIENT )
KRATOS_CREATE_VARIABLE( double, DT_PRESSURE_COEFFICIENT )

KRATOS_CREATE_VARIABLE( double, DT_WATER_PRESSURE )
KRATOS_CREATE_VARIABLE( double, NORMAL_FLUID_FLUX )

KRATOS_CREATE_VARIABLE( double, DENSITY_SOLID )
KRATOS_CREATE_VARIABLE( double, BULK_MODULUS_SOLID )
KRATOS_CREATE_VARIABLE( double, BULK_MODULUS_FLUID )

KRATOS_CREATE_VARIABLE( int,    K0_MAIN_DIRECTION )
KRATOS_CREATE_VARIABLE( double, K0_VALUE_XX )
KRATOS_CREATE_VARIABLE( double, K0_VALUE_YY )
KRATOS_CREATE_VARIABLE( double, K0_VALUE_ZZ )

KRATOS_CREATE_VARIABLE( double, PERMEABILITY_XX )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_YY )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_ZZ )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_XY )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_YZ )
KRATOS_CREATE_VARIABLE( double, PERMEABILITY_ZX )

KRATOS_CREATE_VARIABLE( double, MINIMUM_JOINT_WIDTH )
KRATOS_CREATE_VARIABLE( double, TRANSVERSAL_PERMEABILITY )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( FLUID_FLUX_VECTOR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_FLUID_FLUX_VECTOR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_STRESS_VECTOR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_RELATIVE_DISPLACEMENT_VECTOR )
KRATOS_CREATE_VARIABLE( Matrix, PERMEABILITY_MATRIX )
KRATOS_CREATE_VARIABLE( Matrix, LOCAL_PERMEABILITY_MATRIX )

KRATOS_CREATE_VARIABLE( double, CRITICAL_DISPLACEMENT )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(TOTAL_DISPLACEMENT)

KRATOS_CREATE_VARIABLE( bool, IS_CONVERGED)

KRATOS_CREATE_VARIABLE( Matrix, TOTAL_STRESS_TENSOR )
KRATOS_CREATE_VARIABLE( Vector, TOTAL_STRESS_VECTOR )

KRATOS_CREATE_VARIABLE( Matrix, CAUCHY_STRAIN_TENSOR )
KRATOS_CREATE_VARIABLE( Vector, CAUCHY_STRAIN_VECTOR )

KRATOS_CREATE_VARIABLE( double, STATE_VARIABLE )
KRATOS_CREATE_VARIABLE( double, ARC_LENGTH_LAMBDA )
KRATOS_CREATE_VARIABLE( double, ARC_LENGTH_RADIUS_FACTOR )

KRATOS_CREATE_VARIABLE( double, TIME_UNIT_CONVERTER )

KRATOS_CREATE_VARIABLE( double, LOCAL_EQUIVALENT_STRAIN )
KRATOS_CREATE_VARIABLE( double, NONLOCAL_EQUIVALENT_STRAIN )

KRATOS_CREATE_VARIABLE( double, JOINT_WIDTH )

KRATOS_CREATE_VARIABLE( bool, NODAL_SMOOTHING )
KRATOS_CREATE_VARIABLE( Matrix, NODAL_CAUCHY_STRESS_TENSOR )
KRATOS_CREATE_VARIABLE( double, NODAL_DAMAGE_VARIABLE )
KRATOS_CREATE_VARIABLE( double, NODAL_JOINT_AREA )
KRATOS_CREATE_VARIABLE( double, NODAL_JOINT_WIDTH )
KRATOS_CREATE_VARIABLE( double, NODAL_JOINT_DAMAGE )

KRATOS_CREATE_VARIABLE(bool, RESET_DISPLACEMENTS)
KRATOS_CREATE_VARIABLE(bool, CONSIDER_GEOMETRIC_STIFFNESS)

KRATOS_CREATE_VARIABLE(bool, USE_CONSISTENT_MASS_MATRIX)

KRATOS_CREATE_VARIABLE(bool, IGNORE_UNDRAINED)

// retention models
KRATOS_CREATE_VARIABLE(double, SATURATED_SATURATION )
KRATOS_CREATE_VARIABLE(double, RESIDUAL_SATURATION )
KRATOS_CREATE_VARIABLE(double, VAN_GENUCHTEN_AIR_ENTRY_PRESSURE )
KRATOS_CREATE_VARIABLE(double, VAN_GENUCHTEN_GN )
KRATOS_CREATE_VARIABLE(double, VAN_GENUCHTEN_GL )
KRATOS_CREATE_VARIABLE(double, MINIMUM_RELATIVE_PERMEABILITY )

KRATOS_CREATE_VARIABLE(std::string, RETENTION_LAW )
KRATOS_CREATE_VARIABLE(double,      DEGREE_OF_SATURATION )
KRATOS_CREATE_VARIABLE(double,      EFFECTIVE_SATURATION )
KRATOS_CREATE_VARIABLE(double,      BISHOP_COEFICIENT )
KRATOS_CREATE_VARIABLE(double,      DERIVATIVE_OF_SATURATION )
KRATOS_CREATE_VARIABLE(double,      RELATIVE_PERMEABILITY )

// UDSM
KRATOS_CREATE_VARIABLE( std::string, UDSM_NAME ) // Also for UMAT
KRATOS_CREATE_VARIABLE( int,         UDSM_NUMBER )
KRATOS_CREATE_VARIABLE( bool,        IS_FORTRAN_UDSM ) // Also for UMAT

KRATOS_CREATE_VARIABLE( double,      PARAMETER_1 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_2 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_3 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_4 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_5 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_6 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_7 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_8 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_9 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_10 )

KRATOS_CREATE_VARIABLE( double,      PARAMETER_11 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_12 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_13 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_14 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_15 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_16 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_17 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_18 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_19 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_20 )

KRATOS_CREATE_VARIABLE( double,      PARAMETER_21 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_22 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_23 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_24 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_25 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_26 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_27 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_28 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_29 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_30 )

KRATOS_CREATE_VARIABLE( double,      PARAMETER_31 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_32 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_33 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_34 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_35 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_36 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_37 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_38 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_39 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_40 )

KRATOS_CREATE_VARIABLE( double,      PARAMETER_41 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_42 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_43 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_44 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_45 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_46 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_47 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_48 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_49 )
KRATOS_CREATE_VARIABLE( double,      PARAMETER_50 )

KRATOS_CREATE_VARIABLE(int, NUMBER_OF_UMAT_STATE_VARIABLES)
KRATOS_CREATE_VARIABLE(int, NUMBER_OF_UMAT_PARAMETERS)

KRATOS_CREATE_VARIABLE( Vector,      STATE_VARIABLES )

KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_1 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_2 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_3 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_4 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_5 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_6 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_7 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_8 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_9 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_10 )

KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_11 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_12 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_13 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_14 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_15 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_16 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_17 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_18 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_19 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_20 )

KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_21 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_22 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_23 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_24 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_25 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_26 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_27 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_28 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_29 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_30 )

KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_31 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_32 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_33 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_34 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_35 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_36 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_37 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_38 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_39 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_40 )

KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_41 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_42 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_43 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_44 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_45 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_46 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_47 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_48 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_49 )
KRATOS_CREATE_VARIABLE( double,      STATE_VARIABLE_50 )

}
