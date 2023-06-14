//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 license: OptimizationApplication/license.txt
//
//  Main author:     Reza Najian Asl,
//                   Suneth Warnakulasuriya
//

#pragma once

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "containers/variable.h"

namespace Kratos
{
    // Optimization variables

    //Auxilary field
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,AUXILIARY_FIELD);

    //linear function
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,D_LINEAR_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,D_LINEAR_D_CX);

    //symmetry plane
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,D_PLANE_SYMMETRY_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,D_PLANE_SYMMETRY_D_CX);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,NEAREST_NEIGHBOUR_POINT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, NEAREST_NEIGHBOUR_DIST);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, int, NEAREST_NEIGHBOUR_COND_ID);

    // strain energy
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRAIN_ENERGY_1_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRAIN_ENERGY_1_D_CX);
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRAIN_ENERGY_2_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRAIN_ENERGY_2_D_CX);
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRAIN_ENERGY_3_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRAIN_ENERGY_3_D_CX);

    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_1_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_1_D_CD);

    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_2_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_2_D_CD);

    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_3_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_3_D_CD);

	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_1_D_FT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_1_D_CT);

	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_2_D_FT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_2_D_CT);

	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_3_D_FT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRAIN_ENERGY_3_D_CT);

    // partitioning
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_INTERFACE_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_INTERFACE_D_CD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_PARTITION_MASS_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_PARTITION_MASS_D_CD);

    // mass
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MASS_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MASS_D_CX);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MASS_D_FT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MASS_D_CT);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MASS_D_PD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MASS_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MASS_D_CD);

    // max overhang angle
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MAX_OVERHANG_ANGLE_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MAX_OVERHANG_ANGLE_D_CX);

    // stress
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRESS_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_STRESS_D_CX);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRESS_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_STRESS_D_CD);

    // max_stress
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MAX_STRESS_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MAX_STRESS_D_CX);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_PT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_CT);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_PD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_CD);

    // max_stress
	KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MAX_STRES_D_X);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, D_MAX_STRES_D_CX);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_PT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_CT);
	KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_PD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, D_MAX_STRESS_D_CD);

    // shape control
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,CX);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,SX);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,D_CX);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION,D_X);

    // thickness control
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,PT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,PPT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,FT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,CT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_CT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_PT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_PT_D_FT);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_PPT_D_FT);

    // density control
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,PD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,PE);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_PD_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_PE_D_FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,FD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,CD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_CD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION,double,D_PD);

    // Adjoint RHS
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, ADJOINT_RHS);

    // For implicit vertex-morphing with Helmholtz PDE
	KRATOS_DEFINE_VARIABLE( Matrix, HELMHOLTZ_MASS_MATRIX );
	KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_SURF_RADIUS_SHAPE );
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_BULK_RADIUS_SHAPE );
	KRATOS_DEFINE_VARIABLE( bool, COMPUTE_CONTROL_POINTS_SHAPE );
    KRATOS_DEFINE_VARIABLE( double, ELEMENT_STRAIN_ENERGY );
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_SURF_POISSON_RATIO_SHAPE );
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_BULK_POISSON_RATIO_SHAPE );
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( OPTIMIZATION_APPLICATION, HELMHOLTZ_VARS_SHAPE);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( OPTIMIZATION_APPLICATION, HELMHOLTZ_SOURCE_SHAPE);

    // For thickness optimization
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_VAR_THICKNESS);
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_SOURCE_THICKNESS);
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_RADIUS_THICKNESS);

    // For topology optimization
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_VAR_DENSITY);
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_SOURCE_DENSITY);
    KRATOS_DEFINE_VARIABLE( double, HELMHOLTZ_RADIUS_DENSITY);
    KRATOS_DEFINE_VARIABLE( bool, COMPUTE_CONTROL_DENSITIES);

    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, SHAPE);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, CROSS_AREA);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, DENSITY_SENSITIVITY);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, THICKNESS_SENSITIVITY);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, CROSS_AREA_SENSITIVITY);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, YOUNG_MODULUS_SENSITIVITY);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, POISSON_RATIO_SENSITIVITY);

    // For helholtz solvers
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, bool, COMPUTE_HELMHOLTZ_INVERSE);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, bool, HELMHOLTZ_INTEGRATED_FIELD);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, HELMHOLTZ_RADIUS);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, HELMHOLTZ_SCALAR);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, HELMHOLTZ_SCALAR_SOURCE);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( OPTIMIZATION_APPLICATION, HELMHOLTZ_VECTOR);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( OPTIMIZATION_APPLICATION, HELMHOLTZ_VECTOR_SOURCE);

    // do not expose the following variables to python. They are used
    // as temporary data holders. They can be changed
    // at any point of time in an analysis.
    // Hence, not recommended to be used for calculations
    // unless existing values on those variables are not of interest
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, TEMPORARY_SCALAR_VARIABLE_1);
    KRATOS_DEFINE_APPLICATION_VARIABLE(OPTIMIZATION_APPLICATION, double, TEMPORARY_SCALAR_VARIABLE_2);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, TEMPORARY_ARRAY3_VARIABLE_1);
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS(OPTIMIZATION_APPLICATION, TEMPORARY_ARRAY3_VARIABLE_2);

}
