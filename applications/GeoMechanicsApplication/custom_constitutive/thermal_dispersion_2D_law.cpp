// KRATOS___
//     //   ) )
//    //         ___      ___
//   //  ____  //___) ) //   ) )
//  //    / / //       //   / /
// ((____/ / ((____   ((___/ /  MECHANICS
//
//  License:         geo_mechanics_application/license.txt
//
//  Main authors:    Mohamed Nabi
//                   John van Esch
//

#include "custom_constitutive/thermal_dispersion_2D_law.hpp"
#include "geo_mechanics_application_variables.h"
#include <iostream>

namespace Kratos {

ConstitutiveLaw::Pointer GeoThermalDispersion2DLaw::Clone() const
{
    return Kratos::make_shared<GeoThermalDispersion2DLaw>(*this);
}

SizeType GeoThermalDispersion2DLaw::WorkingSpaceDimension()
{
    constexpr SizeType space_dimension = 2;
    return space_dimension;
}

void GeoThermalDispersion2DLaw::CalculateThermalDispersionMatrix(Matrix& rThermalDispersionMatrix,
                                                                 const Properties& rProp)
{
    KRATOS_TRY

    const double cWater = rProp[POROSITY] * rProp[SATURATION];
    const double cSolid = 1.0 - rProp[POROSITY];

    rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_X, INDEX_2D_THERMAL_FLUX_X) =
        cSolid * rProp[THERMAL_CONDUCTIVITY_SOLID_XX] +
        cWater * rProp[THERMAL_CONDUCTIVITY_WATER];
    rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_X, INDEX_2D_THERMAL_FLUX_Y) =
        cSolid * rProp[THERMAL_CONDUCTIVITY_SOLID_XY];
    rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Y, INDEX_2D_THERMAL_FLUX_Y) =
        cSolid * rProp[THERMAL_CONDUCTIVITY_SOLID_YY] +
        cWater * rProp[THERMAL_CONDUCTIVITY_WATER];
    rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Y, INDEX_2D_THERMAL_FLUX_X) =
        rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_X, INDEX_2D_THERMAL_FLUX_Y);

    if (rThermalDispersionMatrix.size1() == 3 && rThermalDispersionMatrix.size2() == 3) {
        rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Y, INDEX_2D_THERMAL_FLUX_Z) =
            cSolid * rProp[THERMAL_CONDUCTIVITY_SOLID_YZ];
        rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Z, INDEX_2D_THERMAL_FLUX_X) =
            cSolid * rProp[THERMAL_CONDUCTIVITY_SOLID_XZ];
        rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Z, INDEX_2D_THERMAL_FLUX_Z) =
            cSolid * rProp[THERMAL_CONDUCTIVITY_SOLID_ZZ] +
            cWater * rProp[THERMAL_CONDUCTIVITY_WATER];
        rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Z, INDEX_2D_THERMAL_FLUX_Y) =
            rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Y, INDEX_2D_THERMAL_FLUX_Z);
        rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_X, INDEX_2D_THERMAL_FLUX_Z) =
            rThermalDispersionMatrix(INDEX_2D_THERMAL_FLUX_Z, INDEX_2D_THERMAL_FLUX_X);
    }

    KRATOS_CATCH("")
}

} // Namespace Kratos
