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


// Project includes
#include "testing/testing.h"
#include "containers/model.h"
#include "includes/model_part.h"
#include "includes/cfd_variables.h"

// Application includes
#include "fluid_dynamics_application.h"

namespace Kratos::Testing {

namespace
{
    auto CreateTestingNavierStokesWallCondition2D2N(ModelPart& rModelPart)
    {
        // Add required nodal variables
        rModelPart.AddNodalSolutionStepVariable(DIVPROJ);
        rModelPart.AddNodalSolutionStepVariable(ADVPROJ);
        rModelPart.AddNodalSolutionStepVariable(NORMAL);
        rModelPart.AddNodalSolutionStepVariable(VELOCITY);
        rModelPart.AddNodalSolutionStepVariable(PRESSURE);
        rModelPart.AddNodalSolutionStepVariable(BODY_FORCE);
        rModelPart.AddNodalSolutionStepVariable(MESH_VELOCITY);
        rModelPart.AddNodalSolutionStepVariable(EXTERNAL_PRESSURE);

        // Set the required properties
        // Note that the condition DENSITY is retrieved from the element one
        auto p_properties_0 = rModelPart.CreateNewProperties(0);
        auto p_properties_1 = rModelPart.CreateNewProperties(1);
        p_properties_1->SetValue(DENSITY, 1000.0);
        p_properties_1->SetValue(DYNAMIC_VISCOSITY, 1.0);
        ConstitutiveLaw::Pointer p_cons_law(new Newtonian2DLaw());
        p_properties_1->SetValue(CONSTITUTIVE_LAW, p_cons_law);

        // Create a fake element to serve as parent of current testing condition
        rModelPart.CreateNewNode(1, 0.0, 0.0, 0.0);
        rModelPart.CreateNewNode(2, 1.0, 0.0, 0.0);
        rModelPart.CreateNewNode(3, 0.0, 1.0, 0.0);
        auto p_element = rModelPart.CreateNewElement("QSVMS2D3N", 1, {1,2,3}, p_properties_1);
        p_element->Initialize(rModelPart.GetProcessInfo()); // Initialize constitutive law

        // Create the testing condition
        auto p_test_condition = rModelPart.CreateNewCondition("NavierStokesWallCondition2D2N", 1, {{3,1}}, p_properties_0);

        // Add DOFs
        for (auto& r_node : rModelPart.Nodes()){
            r_node.AddDof(VELOCITY_X);
            r_node.AddDof(VELOCITY_Y);
            r_node.AddDof(PRESSURE);
        }

        // Manually set the NORMALS
        array_1d<double,3> aux_normal = ZeroVector(3);
        aux_normal[0] = -1.0;
        rModelPart.pGetNode(1)->FastGetSolutionStepValue(NORMAL) = aux_normal;
        rModelPart.pGetNode(3)->FastGetSolutionStepValue(NORMAL) = aux_normal;

        // Manually set the element as condition neighbour
        GlobalPointersVector<Element> neigh_vect;
        neigh_vect.resize(1);
        neigh_vect(0) = p_element;
        p_test_condition->SetValue(NEIGHBOUR_ELEMENTS, neigh_vect);

        return p_test_condition;
    }

    auto CreateTestingNavierStokesWallCondition3D3N(ModelPart& rModelPart)
    {
        // Add required nodal variables
        rModelPart.AddNodalSolutionStepVariable(DIVPROJ);
        rModelPart.AddNodalSolutionStepVariable(ADVPROJ);
        rModelPart.AddNodalSolutionStepVariable(NORMAL);
        rModelPart.AddNodalSolutionStepVariable(VELOCITY);
        rModelPart.AddNodalSolutionStepVariable(PRESSURE);
        rModelPart.AddNodalSolutionStepVariable(BODY_FORCE);
        rModelPart.AddNodalSolutionStepVariable(MESH_VELOCITY);
        rModelPart.AddNodalSolutionStepVariable(EXTERNAL_PRESSURE);

        // Set the required properties
        // Note that the condition DENSITY is retrieved from the element one
        auto p_properties_0 = rModelPart.CreateNewProperties(0);
        auto p_properties_1 = rModelPart.CreateNewProperties(1);
        p_properties_1->SetValue(DENSITY, 1000.0);
        p_properties_1->SetValue(DYNAMIC_VISCOSITY, 1.0);
        ConstitutiveLaw::Pointer p_cons_law(new Newtonian3DLaw());
        p_properties_1->SetValue(CONSTITUTIVE_LAW, p_cons_law);

        // Create a fake element to serve as parent of current testing condition
        rModelPart.CreateNewNode(1, 0.0, 0.0, 0.0);
        rModelPart.CreateNewNode(2, 1.0, 0.0, 0.0);
        rModelPart.CreateNewNode(3, 0.0, 1.0, 0.0);
        rModelPart.CreateNewNode(4, 0.0, 0.0, 1.0);
        auto p_element = rModelPart.CreateNewElement("QSVMS3D4N", 1, {1,2,3,4}, p_properties_1);
        p_element->Initialize(rModelPart.GetProcessInfo()); // Initialize constitutive law

        // Create the testing condition
        auto p_test_condition = rModelPart.CreateNewCondition("NavierStokesWallCondition3D3N", 1, {{3,1,2}}, p_properties_0);

        // Add DOFs
        for (auto& r_node : rModelPart.Nodes()){
            r_node.AddDof(VELOCITY_X);
            r_node.AddDof(VELOCITY_Y);
            r_node.AddDof(PRESSURE);
        }

        // Manually set the NORMALS
        array_1d<double,3> aux_normal = ZeroVector(3);
        aux_normal[2] = -1.0;
        rModelPart.pGetNode(1)->FastGetSolutionStepValue(NORMAL) = aux_normal;
        rModelPart.pGetNode(2)->FastGetSolutionStepValue(NORMAL) = aux_normal;
        rModelPart.pGetNode(3)->FastGetSolutionStepValue(NORMAL) = aux_normal;

        // Manually set the element as condition neighbour
        GlobalPointersVector<Element> neigh_vect;
        neigh_vect.resize(1);
        neigh_vect(0) = p_element;
        p_test_condition->SetValue(NEIGHBOUR_ELEMENTS, neigh_vect);

        return p_test_condition;
    }
}

KRATOS_TEST_CASE_IN_SUITE(NavierStokesWallCondition2D2NZero, FluidDynamicsApplicationFastSuite)
{
    // Create the test model part
    Model model;
    std::size_t buffer_size = 2;
    auto& r_model_part = model.CreateModelPart("TestModelPart",buffer_size);

    // Create the testing condition
    auto p_test_condition = CreateTestingNavierStokesWallCondition2D2N(r_model_part);

    // Set the testing nodal values
    array_1d<double,3> aux_v = ZeroVector(3);
    for (auto& r_node: r_model_part.Nodes()) {
        aux_v[0] = r_node.Id();
        aux_v[1] = 2.0*r_node.Id();
        r_node.FastGetSolutionStepValue(VELOCITY) = aux_v;
    }

    // Calculate the RHS and LHS
    // Note that in this case it must have zero contribution
    Vector RHS;
    Matrix LHS;
    p_test_condition->CalculateLocalSystem(LHS, RHS, r_model_part.GetProcessInfo());

    // Check results
    KRATOS_CHECK_VECTOR_NEAR(RHS, ZeroVector(6), 1.0e-12)
    KRATOS_CHECK_MATRIX_NEAR(LHS, ZeroMatrix(6,6), 1.0e-12)
}

KRATOS_TEST_CASE_IN_SUITE(NavierStokesWallCondition2D2NOutletInflow, FluidDynamicsApplicationFastSuite)
{
    // Create the test model part
    Model model;
    std::size_t buffer_size = 2;
    auto& r_model_part = model.CreateModelPart("TestModelPart",buffer_size);

    // Create the testing condition
    auto p_test_condition = CreateTestingNavierStokesWallCondition2D2N(r_model_part);

    // Set the testing nodal values
    array_1d<double,3> aux_v = ZeroVector(3);
    for (auto& r_node: r_model_part.Nodes()) {
        aux_v[0] = r_node.Id();
        aux_v[1] = 2.0*r_node.Id();
        r_node.FastGetSolutionStepValue(VELOCITY) = aux_v;
    }

    // Activate the outlet inflow contribution and set required values
    p_test_condition->Set(OUTLET, true);
    r_model_part.GetProcessInfo().SetValue(CHARACTERISTIC_VELOCITY, 1.0);
    r_model_part.GetProcessInfo().SetValue(OUTLET_INFLOW_CONTRIBUTION_SWITCH, true);

    // Calculate the RHS and LHS
    // Note that in this case it must have zero contribution
    Vector RHS;
    Matrix LHS;
    p_test_condition->CalculateLocalSystem(LHS, RHS, r_model_part.GetProcessInfo());

    // Check results
    std::vector<double> rhs_out = {-7083.333333333,0,0,-3750.0,0,0};
    KRATOS_CHECK_VECTOR_NEAR(RHS, rhs_out, 1.0e-8)
    KRATOS_CHECK_MATRIX_NEAR(LHS, ZeroMatrix(6,6), 1.0e-12)
}

KRATOS_TEST_CASE_IN_SUITE(NavierStokesWallCondition2D2NSlipTangentialCorrection, FluidDynamicsApplicationFastSuite)
{
    // Create the test model part
    Model model;
    std::size_t buffer_size = 2;
    auto& r_model_part = model.CreateModelPart("TestModelPart",buffer_size);

    // Create the testing condition
    auto p_test_condition = CreateTestingNavierStokesWallCondition2D2N(r_model_part);

    // Set the testing nodal values
    array_1d<double,3> aux_v = ZeroVector(3);
    for (auto& r_node: r_model_part.Nodes()) {
        aux_v[0] = r_node.Id();
        aux_v[1] = 2.0*r_node.Id();
        r_node.FastGetSolutionStepValue(VELOCITY) = aux_v;
    }

    // Modify the nodal normal in one node to make it different to the condition one in order to get a LHS contribution
    array_1d<double,3> aux_normal = ZeroVector(3);
    aux_normal[0] = -1.0;
    aux_normal[1] = 1.0;
    r_model_part.GetNode(1).FastGetSolutionStepValue(NORMAL) = aux_normal;

    // Activate the outlet inflow contribution and set required values
    p_test_condition->Set(SLIP, true);
    r_model_part.GetProcessInfo().SetValue(SLIP_TANGENTIAL_CORRECTION_SWITCH, true);

    // Calculate the RHS and LHS
    // Note that in this case it must have zero contribution
    Vector RHS;
    Matrix LHS;
    p_test_condition->CalculateLocalSystem(LHS, RHS, r_model_part.GetProcessInfo());

    // Check results
    std::vector<double> rhs_out = {0,-2.0,0,-2.0/3.0,-2.0/3.0,0};
    std::vector<double> lhs_row_3_out = {0,0,-1.0/12.0,0,0,-1.0/6.0};
    KRATOS_CHECK_VECTOR_NEAR(RHS, rhs_out, 1.0e-8)
    KRATOS_CHECK_VECTOR_NEAR(row(LHS,3), lhs_row_3_out, 1.0e-8)
}

KRATOS_TEST_CASE_IN_SUITE(NavierStokesWallCondition3D3NSlipTangentialCorrection, FluidDynamicsApplicationFastSuite)
{
    // Create the test model part
    Model model;
    std::size_t buffer_size = 2;
    auto& r_model_part = model.CreateModelPart("TestModelPart",buffer_size);

    // Create the testing condition
    auto p_test_condition = CreateTestingNavierStokesWallCondition3D3N(r_model_part);

    // Set the testing nodal values
    array_1d<double,3> aux_v = ZeroVector(3);
    for (auto& r_node: r_model_part.Nodes()) {
        aux_v[0] = r_node.Id();
        aux_v[1] = 2.0*r_node.Id();
        aux_v[2] = 3.0*r_node.Id();
        r_node.FastGetSolutionStepValue(VELOCITY) = aux_v;
    }

    // Modify the nodal normal some nodes to make it different to the condition one in order to get a LHS contribution
    array_1d<double,3> aux_normal = ZeroVector(3);
    aux_normal[0] = -1.0;
    aux_normal[2] = -1.0;
    r_model_part.GetNode(1).FastGetSolutionStepValue(NORMAL) = aux_normal;
    r_model_part.GetNode(2).FastGetSolutionStepValue(NORMAL) = aux_normal;
    r_model_part.GetNode(3).FastGetSolutionStepValue(NORMAL) = aux_normal;

    // Activate the outlet inflow contribution and set required values
    p_test_condition->Set(SLIP, true);
    r_model_part.GetProcessInfo().SetValue(SLIP_TANGENTIAL_CORRECTION_SWITCH, true);

    // Calculate the RHS and LHS
    // Note that in this case it must have zero contribution
    Vector RHS;
    Matrix LHS;
    p_test_condition->CalculateLocalSystem(LHS, RHS, r_model_part.GetProcessInfo());

    // Check results
    std::vector<double> rhs_out = {-0.2222222222,2,0.2222222222,0,-0.2222222222,2,0.2222222222,0,-0.2222222222,2,0.2222222222,0};
    std::vector<double> lhs_row_2_out = {0,0,0,0.04166666667,0,0,0,0.02083333333,0,0,0,0.02083333333};
    KRATOS_CHECK_VECTOR_NEAR(RHS, rhs_out, 1.0e-8)
    KRATOS_CHECK_VECTOR_NEAR(row(LHS,2), lhs_row_2_out, 1.0e-8)
}

}  // namespace Kratos::Testing