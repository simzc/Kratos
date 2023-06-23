//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Ruben Zorrilla
//

// System includes

// External includes

// Project includes
#include "containers/model.h"
#include "geometries/quadrilateral_2d_4.h"
#include "includes/gid_io.h"
#include "linear_solvers/amgcl_solver.h"
#include "processes/levelset_convection_process.h"
#include "processes/flux_corrected_transport_convection_process.h"
#include "processes/find_global_nodal_neighbours_process.h"
#include "processes/structured_mesh_generator_process.h"
#include "testing/testing.h"

namespace Kratos::Testing {

KRATOS_TEST_CASE_IN_SUITE(FluxCorrectedTransportConvectionProcess2D, KratosCoreFastSuite)
{
    // Set-up a simplicial mesh to calculate its edge data structure
    Model current_model;
    auto& r_model_part = current_model.CreateModelPart("ModelPart");
    auto p_point_1 = Kratos::make_intrusive<Node>(1, -0.5, -0.5, 0.0);
    auto p_point_2 = Kratos::make_intrusive<Node>(2, -0.5, 0.5, 0.0);
    auto p_point_3 = Kratos::make_intrusive<Node>(3, 0.5, 0.5, 0.0);
    auto p_point_4 = Kratos::make_intrusive<Node>(4, 0.5, -0.5, 0.0);
    Quadrilateral2D4<Node> geometry(p_point_1, p_point_2, p_point_3, p_point_4);
    Parameters mesher_parameters(R"({
        "number_of_divisions" : 50,
        "element_name" : "Element2D3N",
        "condition_name" : "LineCondition",
        "create_skin_sub_model_part" : true
    })");
    r_model_part.SetBufferSize(3);
    r_model_part.AddNodalSolutionStepVariable(DISTANCE);
    r_model_part.AddNodalSolutionStepVariable(VELOCITY);
    StructuredMeshGeneratorProcess(geometry, r_model_part, mesher_parameters).Execute();

    // Calculate nodal neighbours
    // TODO: Temporary solution until we skip the neighbours calculation
    FindGlobalNodalNeighboursProcess nodal_neighs_process(r_model_part);
    nodal_neighs_process.Execute();

    // FCT convection settings
    Parameters fct_parameters(R"({
        "model_part_name" : "ModelPart",
        "echo_level" : 1,
        "max_CFL" : 0.1,
        "diffusion_constant" : 1.0
    })");

    // Set nodal values

    // // Gaussian hill
    // const double a = 1.0; // height
    // const double c = 0.1; // width
    // const double b_x = 0.15; // x-coordinate of the center
    // const double b_y = 0.15; // y-coordinate of the center
    // auto dist_func = [&](Node& rNode){return a*std::exp(-(std::pow(rNode.X()-b_x,2)/2/std::pow(c,2)+(std::pow(rNode.Y()-b_y,2)/2/std::pow(c,2))));};
    // auto vel_func = [&](Node& rNode, array_1d<double,3>& rVel){rVel[0] = -rNode.Y();rVel[1] = rNode.X();rVel[2] = 0.0;};

    // // "1D" Gaussian hill
    // const double a = 1.0; // height
    // const double c = 0.05; // width
    // const double b = 0.0; // x-coordinate of the center
    // auto dist_func = [&](Node& rNode){return a*std::exp(-(std::pow(rNode.X()-b,2)/2/std::pow(c,2)));};
    // auto vel_func = [&](Node& rNode, array_1d<double,3>& rVel){rVel[0] = 1.0;rVel[1] = 0.0;rVel[2] = 0.0;};

    // "1D" hill
    const double a = 1.0; // height
    const double c = 0.075; // width
    const double b = 0.0; // x-coordinate of the center
    auto dist_func = [&](Node& rNode){return std::abs(rNode.X() - b) < c ? a : 0.0;};
    auto vel_func = [&](Node& rNode, array_1d<double,3>& rVel){rVel[0] = 1.0;rVel[1] = 0.0;rVel[2] = 0.0;};

    array_1d<double,3> aux_v;
    for (auto& r_node : r_model_part.Nodes()) {
        vel_func(r_node, aux_v);
        r_node.FastGetSolutionStepValue(DISTANCE, 0) = dist_func(r_node);
        r_node.FastGetSolutionStepValue(DISTANCE, 1) = dist_func(r_node);
        r_node.FastGetSolutionStepValue(VELOCITY, 0) = aux_v;
        r_node.FastGetSolutionStepValue(VELOCITY, 1) = aux_v;
        r_node.FastGetSolutionStepValue(VELOCITY, 2) = aux_v;
    }

    // Set and execute the FCT convection process (time loop)
    GidIO<> gid_io_convection("/home/rzorrilla/Desktop/FluxCorrectedTransportProcess2D", GiD_PostAscii, SingleFile, WriteDeformed, WriteConditions);
    gid_io_convection.InitializeMesh(0);
    gid_io_convection.WriteMesh(r_model_part.GetMesh());
    gid_io_convection.FinalizeMesh();
    gid_io_convection.InitializeResults(0, r_model_part.GetMesh());
    gid_io_convection.WriteNodalResults(DISTANCE, r_model_part.Nodes(), 0, 1);
    gid_io_convection.WriteNodalResults(VELOCITY, r_model_part.Nodes(), 0, 1);

    const double dt = 5e-3;
    const double end_time = 0.5;
    r_model_part.GetProcessInfo()[TIME] = 0.0;
    r_model_part.GetProcessInfo()[DELTA_TIME] = dt;
    FluxCorrectedTransportConvectionProcess<2> fct_convection_process(current_model, fct_parameters);
    while (r_model_part.GetProcessInfo()[TIME] < end_time) {
        const double new_time = r_model_part.GetProcessInfo()[TIME] + dt;
        r_model_part.CloneTimeStep(new_time);
        fct_convection_process.Execute();
        gid_io_convection.WriteNodalResults(DISTANCE, r_model_part.Nodes(), new_time, 0);
        gid_io_convection.WriteNodalResults(VELOCITY, r_model_part.Nodes(), new_time, 0);
    }

    gid_io_convection.FinalizeResults();

    // Parameters ls_conv_parameters(R"({
    //     "model_part_name" : "ModelPart",
    //     "echo_level" : 1,
    //     "max_CFL" : 1.0
    // })");

    // typedef UblasSpace<double, Matrix, Vector> LocalSpaceType;
    // typedef UblasSpace<double, CompressedMatrix, Vector> SparseSpaceType;
    // typedef LinearSolver<SparseSpaceType, LocalSpaceType> LinearSolverType;
    // auto p_ls_linear_solver = Kratos::make_shared<AMGCLSolver<SparseSpaceType, LocalSpaceType>>();
    // LevelSetConvectionProcess<2,SparseSpaceType,LocalSpaceType,LinearSolverType> ls_conv_process(current_model, p_ls_linear_solver, ls_conv_parameters);
    // ls_conv_process.Execute();

    // GidIO<> gid_io_convection("/home/rzorrilla/Desktop/LevelSetConvectionProcess2D", GiD_PostAscii, SingleFile, WriteDeformed, WriteConditions);
    // gid_io_convection.InitializeMesh(0);
    // gid_io_convection.WriteMesh(r_model_part.GetMesh());
    // gid_io_convection.FinalizeMesh();
    // gid_io_convection.InitializeResults(0, r_model_part.GetMesh());
    // gid_io_convection.WriteNodalResults(DISTANCE, r_model_part.Nodes(), 0, 1);
    // gid_io_convection.WriteNodalResults(VELOCITY, r_model_part.Nodes(), 0, 1);
    // gid_io_convection.WriteNodalResults(DISTANCE, r_model_part.Nodes(), 1, 0);
    // gid_io_convection.WriteNodalResults(VELOCITY, r_model_part.Nodes(), 1, 0);
    // gid_io_convection.FinalizeResults();
}

KRATOS_TEST_CASE_IN_SUITE(FluxCorrectedTransportConvectionProcessZalesak, KratosCoreFastSuite)
{
    // Set-up a simplicial mesh to calculate its edge data structure
    Model current_model;
    auto& r_model_part = current_model.CreateModelPart("ModelPart");
    auto p_point_1 = Kratos::make_intrusive<Node>(1, 0.0, 0.0, 0.0);
    auto p_point_2 = Kratos::make_intrusive<Node>(2, 0.0, 1.0, 0.0);
    auto p_point_3 = Kratos::make_intrusive<Node>(3, 1.0, 1.0, 0.0);
    auto p_point_4 = Kratos::make_intrusive<Node>(4, 1.0, 0.0, 0.0);
    Quadrilateral2D4<Node> geometry(p_point_1, p_point_2, p_point_3, p_point_4);
    Parameters mesher_parameters(R"({
        "number_of_divisions" : 128,
        "element_name" : "Element2D3N",
        "condition_name" : "LineCondition",
        "create_skin_sub_model_part" : true
    })");
    r_model_part.SetBufferSize(2);
    r_model_part.AddNodalSolutionStepVariable(DISTANCE);
    r_model_part.AddNodalSolutionStepVariable(VELOCITY);
    StructuredMeshGeneratorProcess(geometry, r_model_part, mesher_parameters).Execute();

    // Calculate nodal neighbours
    // TODO: Temporary solution until we skip the neighbours calculation
    FindGlobalNodalNeighboursProcess nodal_neighs_process(r_model_part);
    nodal_neighs_process.Execute();

    // FCT convection settings
    Parameters fct_parameters(R"({
        "model_part_name" : "ModelPart",
        "echo_level" : 1,
        "max_CFL" : 0.2,
        "echo_level" : 2
    })");

    // Set nodal values
    auto dist_func = [&](Node& rNode){
        double dist;
        const double aux_1 = std::sqrt(std::pow(rNode.X() - 0.5,2) + std::pow(rNode.Y() - 0.75,2));
        if (aux_1 <= 0.15 && (std::abs(rNode.X() - 0.5) >= 0.025 || rNode.Y() >= 0.85)) {
            dist = 1.0;
        } else {
            dist = 0.0;
        }
        return dist;
    };
    auto vel_func = [&](Node& rNode, array_1d<double,3>& rVel){
        rVel[0] = 0.5 - rNode.Y();
        rVel[1] = rNode.X() - 0.5;
        rVel[2] = 0.0;
    };

    array_1d<double,3> aux_v;
    for (auto& r_node : r_model_part.Nodes()) {
        vel_func(r_node, aux_v);
        r_node.FastGetSolutionStepValue(DISTANCE, 0) = dist_func(r_node);
        r_node.FastGetSolutionStepValue(DISTANCE, 1) = dist_func(r_node);
        r_node.FastGetSolutionStepValue(VELOCITY, 0) = aux_v;
        r_node.FastGetSolutionStepValue(VELOCITY, 1) = aux_v;
    }

    // Set and execute the FCT convection process (time loop)
    GidIO<> gid_io_convection("/home/rzorrilla/Desktop/FluxCorrectedTransportProcessZalesak", GiD_PostAscii, SingleFile, WriteDeformed, WriteConditions);
    gid_io_convection.InitializeMesh(0);
    gid_io_convection.WriteMesh(r_model_part.GetMesh());
    gid_io_convection.FinalizeMesh();
    gid_io_convection.InitializeResults(0, r_model_part.GetMesh());
    gid_io_convection.WriteNodalResults(DISTANCE, r_model_part.Nodes(), 0, 1);
    gid_io_convection.WriteNodalResults(VELOCITY, r_model_part.Nodes(), 0, 1);

    const double dt = 1.0e-1;
    const double t_end = 6.28;
    r_model_part.GetProcessInfo()[TIME] = 0.0;
    r_model_part.GetProcessInfo()[DELTA_TIME] = dt;
    FluxCorrectedTransportConvectionProcess<2> fct_convection_process(current_model, fct_parameters);
    while (r_model_part.GetProcessInfo()[TIME] < t_end) {
        const double new_time = r_model_part.GetProcessInfo()[TIME] + dt;
        r_model_part.CloneTimeStep(new_time);
        fct_convection_process.Execute();
        gid_io_convection.WriteNodalResults(DISTANCE, r_model_part.Nodes(), new_time, 0);
        gid_io_convection.WriteNodalResults(VELOCITY, r_model_part.Nodes(), new_time, 0);
    }

    gid_io_convection.FinalizeResults();
}

} // namespace Kratos::Testing.
