//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "director_utilities.h"
#include "factories/linear_solver_factory.h"
#include "iga_application_variables.h"
#include "custom_elements/shell_5p_element.h"

namespace Kratos
{
    DirectorUtilities::DirectorUtilities(
        ModelPart& rModelPart,
        Parameters JsonParameters)
        : mrModelPart(rModelPart)
        , mParameters(JsonParameters)
    {
    }

    void DirectorUtilities::ComputeDirectors()
    {
        Vector brep_ids = mParameters["brep_ids"].GetVector();
        for (IndexType i = 0; i < brep_ids.size(); ++i) {
            auto& r_geometry = mrModelPart.GetRootModelPart().GetGeometry((IndexType)brep_ids[i]);
            size_t number_of_control_points = r_geometry.size();

            std::vector<IndexType> eqID(number_of_control_points);
            for (IndexType inodes = 0; inodes < number_of_control_points; ++inodes)
                eqID[inodes] = r_geometry[inodes].GetId();

            SparseMatrixType NTN(number_of_control_points, number_of_control_points, number_of_control_points*number_of_control_points); //inital guess how much non-zero are there
            Matrix directorAtIntgrationPoints{ ZeroMatrix(number_of_control_points, 3) };
            SparseSpaceType::SetToZero(NTN);

            PointerVector<Geometry<Node<3>>> quad_points;
            r_geometry.CreateQuadraturePointGeometries(quad_points, 3);
            for (IndexType iP = 0; iP < quad_points.size(); ++iP)
            {
                const Matrix& r_N = quad_points[iP].ShapeFunctionsValues();
                const array_1d<double, 3> A3 = quad_points[iP].UnitNormal(0);

                NTN += outer_prod(row(r_N,0), row(r_N, 0));

                for (IndexType inodes = 0; inodes < quad_points.size(); ++inodes) {
                    auto it = find(eqID.begin(), eqID.end(), quad_points[iP][inodes].Id());
                    IndexType node_index = it - eqID.begin();

                    row(directorAtIntgrationPoints, node_index) += r_N(0, inodes) * trans(A3);
                }
            }
            Parameters solver_parameters(mParameters["linear_solver_settings"]);
            solver_parameters.AddString("solver_type", "skyline_lu_factorization");

            Matrix nodalDirectors = ZeroMatrix(number_of_control_points, 3);
            auto solver = LinearSolverFactory<SparseSpaceType, LocalSpaceType>().Create(solver_parameters);

            solver->Solve(NTN, nodalDirectors, directorAtIntgrationPoints);
            for (IndexType i = 0; i < number_of_control_points; ++i)
            {
                r_geometry[i].SetValue(DIRECTOR, row(nodalDirectors, i));
                r_geometry[i].SetValue(DIRECTORLENGTH, norm_2(row(nodalDirectors, i)));
                r_geometry[i].SetValue(DIRECTORTANGENTSPACE, Shell5pElement::TangentSpaceFromStereographicProjection(row(nodalDirectors, i)));
            }
        }
    }
}  // namespace Kratos.
