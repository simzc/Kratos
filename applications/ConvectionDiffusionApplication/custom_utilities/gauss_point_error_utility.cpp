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
//

// System includes

// External includes

// Project includes
#include "containers/model.h"
#include "includes/checks.h"
#include "includes/model_part.h"
#include "utilities/binbased_fast_point_locator.h"
#include "utilities/variable_utils.h"
#include "utilities/divide_triangle_2d_3.h"
#include "utilities/divide_tetrahedra_3d_4.h"
#include "modified_shape_functions/triangle_2d_3_modified_shape_functions.h"
#include "modified_shape_functions/tetrahedra_3d_4_modified_shape_functions.h"
#include "modified_shape_functions/triangle_2d_3_ausas_modified_shape_functions.h"
#include "modified_shape_functions/tetrahedra_3d_4_ausas_modified_shape_functions.h"

// Application includes
#include "gauss_point_error_utility.h"

namespace Kratos
{

/* Public functions *******************************************************/
Parameters GaussPointErrorUtility::GetDefaultSettings()
{
    Parameters default_settings(R"(
    {
        "model_part_name"                       : "",
        "level_set_type"                        : "",
        "shape_functions"                       : "",
        "distance_variable_name"                : ""
    })");

    return default_settings;
}

void GaussPointErrorUtility::CheckAndSetLevelSetType(
    const Parameters rParameters,
    LevelSetType& rLevelSetType)
{
    const std::string level_set_type = rParameters["level_set_type"].GetString();
    KRATOS_ERROR_IF(level_set_type == "") << "\'level_set_type\' is not prescribed. Admissible values are: \'continuous\' and \'discontinuous\'." << std::endl;

    if (level_set_type == "continuous") {
        rLevelSetType = LevelSetType::Continuous;
    } else if (level_set_type == "discontinuous") {
        rLevelSetType = LevelSetType::Discontinuous;
    } else {
        std::stringstream error_msg;
        error_msg << "Currently prescribed \'level_set_type\': " << level_set_type << std::endl;
        error_msg << "Admissible values are : \'continuous\' and \'discontinuous\'" << std::endl;
        KRATOS_ERROR << error_msg.str();
    }
}

void GaussPointErrorUtility::CheckAndSetShapeFunctionsType(
    const Parameters rParameters,
    ShapeFunctionsType& rShapeFunctionsType)
{
    const std::string shape_functions = rParameters["shape_functions"].GetString();
    KRATOS_ERROR_IF(shape_functions == "") << "\'shape_functions\' is not prescribed. Admissible values are: \'standard\' and \'ausas\'." << std::endl;

    if (shape_functions == "ausas") {
        rShapeFunctionsType = ShapeFunctionsType::Ausas;
    } else if (shape_functions == "standard") {
        rShapeFunctionsType = ShapeFunctionsType::Standard;
    } else {
        std::stringstream error_msg;
        error_msg << "Currently prescribed \'shape_functions\': " << shape_functions << std::endl;
        error_msg << "Admissible values are : \'standard\' and \'ausas\'" << std::endl;
        KRATOS_ERROR << error_msg.str();
    }
}

const std::string GaussPointErrorUtility::CheckAndReturnDistanceVariableName(
    const Parameters rParameters,
    const LevelSetType& rLevelSetType)
{
    std::string distance_variable_name = rParameters["distance_variable_name"].GetString();
    // If the distance variable name is not provided, try to deduce it from the FE space type
    if (distance_variable_name == "") {
        switch (rLevelSetType) {
            // Nodal-based level set
            case LevelSetType::Continuous:
                distance_variable_name = "DISTANCE";
                break;
            // Element-based level set
            case LevelSetType::Discontinuous:
                distance_variable_name = "ELEMENTAL_DISTANCES";
                break;
            default:
                KRATOS_ERROR << "Default \"distance_variable_name\" cannot be deduced from the shape functions type" << std::endl;
        }
        KRATOS_INFO("GaussPointErrorUtility") << "\'distance_variable_name\' is not prescribed. Using default " << distance_variable_name << std::endl;
    // User-defined distance variable name case
    } else {
        KRATOS_ERROR_IF_NOT(KratosComponents<Variable<double>>::Has(distance_variable_name) || KratosComponents<Variable<Vector>>::Has(distance_variable_name))
           << "Provided \"distance_variable_name\" " << distance_variable_name << " is not in the KratosComponents. Please check the provided value." << std::endl;
    }

    return distance_variable_name;
}

GaussPointErrorUtility::GaussPointErrorUtility(
    ModelPart& rModelPart,
    const LevelSetType& rLevelSetType,
    const ShapeFunctionsType& rShapeFunctionsType)
    : mrModelPart(rModelPart)
    , mLevelSetType(rLevelSetType)
    , mShapeFunctionsType(rShapeFunctionsType)
{
}

GaussPointErrorUtility::GaussPointErrorUtility(
    ModelPart& rModelPart,
    Parameters rParameters)
    : mrModelPart(rModelPart)
    , mLevelSetType(
        [&] (Parameters& x) {
            x.ValidateAndAssignDefaults(GetDefaultSettings());
            LevelSetType aux_level_set_type;
            CheckAndSetLevelSetType(x, aux_level_set_type);
            return aux_level_set_type;
        } (rParameters)
    )
    , mShapeFunctionsType(
        [&] (Parameters& x) {
            x.ValidateAndAssignDefaults(GetDefaultSettings());
            ShapeFunctionsType aux_shape_func_type;
            CheckAndSetShapeFunctionsType(x, aux_shape_func_type);
            return aux_shape_func_type;
        } (rParameters)
    )
    , mpNodalDistanceVariable(
        [&] (Parameters& x) -> const Variable<double>* {
            const Variable<double>* p_aux;
            switch (mLevelSetType) {
                case LevelSetType::Continuous: {
                    x.ValidateAndAssignDefaults(GetDefaultSettings());
                    const std::string dist_var_name(CheckAndReturnDistanceVariableName(x, mLevelSetType));
                    p_aux = &(KratosComponents<Variable<double>>::Get(dist_var_name));
                    return p_aux;
                } default: {
                    p_aux = nullptr;
                    return p_aux;
                }
            }
        } (rParameters)
    )
    , mpElementalDistanceVariable(
        [&] (Parameters& x) -> const Variable<Vector>* {
            const Variable<Vector>* p_aux;
            switch (mLevelSetType) {
                case LevelSetType::Discontinuous: {
                    x.ValidateAndAssignDefaults(GetDefaultSettings());
                    const std::string dist_var_name(CheckAndReturnDistanceVariableName(x, mLevelSetType));
                    p_aux = &(KratosComponents<Variable<Vector>>::Get(dist_var_name));
                    return p_aux;
                } default: {
                    p_aux = nullptr;
                    return p_aux;
                }
            }
        } (rParameters)
    )
{
}

GaussPointErrorUtility::GaussPointErrorUtility(
    Model& rModel,
    Parameters rParameters)
    : GaussPointErrorUtility(
        [&] (Model& x, Parameters& y) -> ModelPart& {
            y.ValidateAndAssignDefaults(GetDefaultSettings());
            KRATOS_ERROR_IF(y["model_part_name"].GetString() == "") << "\'model_part_name\' is empty. Please provide the origin model part name." << std::endl;
            return x.GetModelPart(y["model_part_name"].GetString());
        } (rModel, rParameters),
        rParameters)
{
}

double GaussPointErrorUtility::Execute()
{
    // Initialize values
    double t_error_norm = 0.0;

    // Loop the elements to calculate the error
    const int n_elems = mrModelPart.NumberOfElements();
    for (int i_elem = 0; i_elem < n_elems; ++i_elem) {
        auto it_elem = mrModelPart.ElementsBegin() + i_elem;
        const auto p_geom = it_elem->pGetGeometry();
        const int n_nodes = p_geom->PointsNumber();
        const auto d_vect = SetDistancesVector(it_elem);

        // Initialize values
        Matrix r_N_container;
        ModifiedShapeFunctions::ShapeFunctionsGradientsType r_DN_DX_container;
        const GeometryData::IntegrationMethod integration_method = GeometryData::IntegrationMethod::GI_GAUSS_2;
        const auto& r_integrations_points = p_geom->IntegrationPoints(integration_method);

        double i_gauss_t_err;
        double i_gauss_t_exact;

        // Check if element is ACTIVE (inactive elements are not assembled to the system)
        if (ElementIsPositive(p_geom, d_vect)) {

            r_N_container = p_geom->ShapeFunctionsValues(integration_method);
            Vector det_J_vector(r_integrations_points.size());
            p_geom->DeterminantOfJacobian(det_J_vector, integration_method);

            const unsigned int n_gauss = r_integrations_points.size();
            for (int i_gauss = 0; i_gauss < n_gauss; ++i_gauss ) {
                // Get Gauss pt. data
                const Vector& rN = row(r_N_container, i_gauss);
                const double w = r_integrations_points[i_gauss].Weight() * det_J_vector[i_gauss];

                // Calculate the Gauss pt. coordinates
                array_1d<double,3> coords = ZeroVector(3);
                coords[0] = ((*p_geom)[0]).X()*rN[0] + ((*p_geom)[1]).X()*rN[1] + ((*p_geom)[2]).X()*rN[2];
                coords[1] = ((*p_geom)[0]).Y()*rN[0] + ((*p_geom)[1]).Y()*rN[1] + ((*p_geom)[2]).Y()*rN[2];

                // Interpolate the Gauss pt. error
                i_gauss_t_err = 0.0;
                i_gauss_t_exact = 0.0;
                for (int i_node = 0; i_node < n_nodes; ++i_node) {
                    const auto& r_node = (*p_geom)[i_node];
                    i_gauss_t_err += rN[i_node] * r_node.FastGetSolutionStepValue(TEMPERATURE);
                    i_gauss_t_exact += rN[i_node] * CalculateTemperatureExactSolution(r_node.Coordinates());
                }
                i_gauss_t_err -= i_gauss_t_exact;

                // Add the current Gauss pt. error to the total norm
                t_error_norm += w * std::pow(i_gauss_t_err, 2);
            }
        }
    }

    t_error_norm = std::sqrt(t_error_norm);
    return t_error_norm;
}

double GaussPointErrorUtility::ExecuteModelPart(
    const Variable<array_1d<double,3>>& rVariable,
    ModelPart& rReferenceModelPart)
{
    // Initialize values
    double val_error_norm = 0.0;

    // Get DOMAIN_SIZE from reference model part
    KRATOS_ERROR_IF_NOT(rReferenceModelPart.GetProcessInfo().Has(DOMAIN_SIZE)) 
        << "'DOMAIN_SIZE' is not found in reference model part." << std::endl;
    const std::size_t domain_size = rReferenceModelPart.GetProcessInfo()[DOMAIN_SIZE];
        
    // Set the search structure
    KRATOS_ERROR_IF_NOT(domain_size == 2) << "DOMAIN_SIZE 3 to be implemented!" << std::endl;
    auto p_point_locator = Kratos::make_shared<BinBasedFastPointLocator<2>>(rReferenceModelPart);
    p_point_locator->UpdateSearchDatabase();

    // Loop the elements to calculate the error
    Vector ref_mesh_N;
    Element::Pointer p_ref_mesh_elem;
    const int n_elems = mrModelPart.NumberOfElements();
    for (int i_elem = 0; i_elem < n_elems; ++i_elem) {
        auto it_elem = mrModelPart.ElementsBegin() + i_elem;
        const auto p_geom = it_elem->pGetGeometry();
        const int n_nodes = p_geom->PointsNumber();
        const auto d_vect = SetDistancesVector(it_elem);

        // Initialize values
        Matrix r_N_container;
        ModifiedShapeFunctions::ShapeFunctionsGradientsType r_DN_DX_container;
        const GeometryData::IntegrationMethod integration_method = GeometryData::IntegrationMethod::GI_GAUSS_2;
        const auto& r_integrations_points = p_geom->IntegrationPoints(integration_method);

        array_1d<double,3> i_gauss_err;
        array_1d<double,3> i_gauss_val;
        array_1d<double,3> i_gauss_val_exact;

        // Check if element is ACTIVE (inactive elements are not assembled to the system)
        if (ElementIsPositive(p_geom, d_vect)) {

            r_N_container = p_geom->ShapeFunctionsValues(integration_method);
            Vector det_J_vector(r_integrations_points.size());
            p_geom->DeterminantOfJacobian(det_J_vector, integration_method);

            const unsigned int n_gauss = r_integrations_points.size();
            for (int i_gauss = 0; i_gauss < n_gauss; ++i_gauss ) {
                // Get Gauss pt. data
                const Vector& rN = row(r_N_container, i_gauss);
                const double w = r_integrations_points[i_gauss].Weight() * det_J_vector[i_gauss];

                // Calculate the Gauss pt. coordinates
                array_1d<double,3> coords = ZeroVector(3);
                coords[0] = ((*p_geom)[0]).X()*rN[0] + ((*p_geom)[1]).X()*rN[1] + ((*p_geom)[2]).X()*rN[2];
                coords[1] = ((*p_geom)[0]).Y()*rN[0] + ((*p_geom)[1]).Y()*rN[1] + ((*p_geom)[2]).Y()*rN[2];

                // Find current Gauss pt. in reference mesh
                const bool is_found = p_point_locator->FindPointOnMeshSimplified(
                    coords,
                    ref_mesh_N,
                    p_ref_mesh_elem);
                KRATOS_ERROR_IF_NOT(is_found) << "Point (" << coords[0] << "," << coords[1] << "," << coords[2] << ") not found in reference mesh." << std::endl;

                // Interpolate the Gauss pt. value in reference mesh
                noalias(i_gauss_val_exact) = ZeroVector(3);
                const std::size_t n_nodes_ref = ref_mesh_N.size();
                for (std::size_t i_node_ref = 0; i_node_ref < n_nodes_ref; ++i_node_ref) {
                    const auto& r_node_ref = p_ref_mesh_elem->GetGeometry()[i_node_ref];
                    i_gauss_val_exact += ref_mesh_N[i_node_ref] * r_node_ref.FastGetSolutionStepValue(rVariable);
                }

                // Interpolate the Gauss pt. value in current mesh
                noalias(i_gauss_val) = ZeroVector(3);
                for (int i_node = 0; i_node < n_nodes; ++i_node) {
                    const auto& r_node = (*p_geom)[i_node];
                    i_gauss_val += rN[i_node] * r_node.FastGetSolutionStepValue(rVariable);
                }

                // Add the current Gauss pt. error to the total norm
                i_gauss_err = i_gauss_val - i_gauss_val_exact;
                val_error_norm += w * (std::pow(i_gauss_err[0], 2) + std::pow(i_gauss_err[1], 2) + std::pow(i_gauss_err[2], 2));
            }
        }
    }

    val_error_norm = std::sqrt(val_error_norm);
    return val_error_norm;
}

double GaussPointErrorUtility::ExecuteModelPartGradient(
    const Variable<array_1d<double,3>>& rVariable,
    ModelPart& rReferenceModelPart)
{
    // Initialize values
    double val_error_norm = 0.0;

    // Get DOMAIN_SIZE from reference model part
    KRATOS_ERROR_IF_NOT(rReferenceModelPart.GetProcessInfo().Has(DOMAIN_SIZE)) 
        << "'DOMAIN_SIZE' is not found in reference model part." << std::endl;
    const std::size_t domain_size = rReferenceModelPart.GetProcessInfo()[DOMAIN_SIZE];
        
    // Set the search structure
    KRATOS_ERROR_IF_NOT(domain_size == 2) << "DOMAIN_SIZE 3 to be implemented!" << std::endl;
    auto p_point_locator = Kratos::make_shared<BinBasedFastPointLocator<2>>(rReferenceModelPart);
    p_point_locator->UpdateSearchDatabase();

    // Loop the elements to calculate the error
    Vector ref_mesh_N;
    Element::Pointer p_ref_mesh_elem;
    const int n_elems = mrModelPart.NumberOfElements();
    for (int i_elem = 0; i_elem < n_elems; ++i_elem) {
        auto it_elem = mrModelPart.ElementsBegin() + i_elem;
        const auto p_geom = it_elem->pGetGeometry();
        const int n_nodes = p_geom->PointsNumber();
        const auto d_vect = SetDistancesVector(it_elem);

        // Initialize values
        Matrix r_N_container;
        ModifiedShapeFunctions::ShapeFunctionsGradientsType r_DN_DX_container;
        const GeometryData::IntegrationMethod integration_method = GeometryData::IntegrationMethod::GI_GAUSS_2;
        const auto& r_integrations_points = p_geom->IntegrationPoints(integration_method);

        Matrix i_gauss_err;
        Matrix i_gauss_val;
        Matrix i_gauss_val_exact;

        // Check if element is ACTIVE (inactive elements are not assembled to the system)
        if (ElementIsPositive(p_geom, d_vect)) {

            r_N_container = p_geom->ShapeFunctionsValues(integration_method);
            p_geom->ShapeFunctionsIntegrationPointsGradients(r_DN_DX_container, integration_method);
            Vector det_J_vector(r_integrations_points.size());
            p_geom->DeterminantOfJacobian(det_J_vector, integration_method);

            const unsigned int n_gauss = r_integrations_points.size();
            for (int i_gauss = 0; i_gauss < n_gauss; ++i_gauss ) {
                // Get Gauss pt. data
                const Vector& rN = row(r_N_container, i_gauss);
                const Matrix& rDN_DX = r_DN_DX_container[i_gauss];
                const double w = r_integrations_points[i_gauss].Weight() * det_J_vector[i_gauss];

                // Calculate the Gauss pt. coordinates
                array_1d<double,3> coords = ZeroVector(3);
                coords[0] = ((*p_geom)[0]).X()*rN[0] + ((*p_geom)[1]).X()*rN[1] + ((*p_geom)[2]).X()*rN[2];
                coords[1] = ((*p_geom)[0]).Y()*rN[0] + ((*p_geom)[1]).Y()*rN[1] + ((*p_geom)[2]).Y()*rN[2];

                // Find current Gauss pt. in reference mesh
                const bool is_found = p_point_locator->FindPointOnMeshSimplified(
                    coords,
                    ref_mesh_N,
                    p_ref_mesh_elem);
                KRATOS_ERROR_IF_NOT(is_found) << "Point (" << coords[0] << "," << coords[1] << "," << coords[2] << ") not found in reference mesh." << std::endl;

                // Calculate shape functions gradients in reference mesh
                const auto& r_ref_geom = p_ref_mesh_elem->GetGeometry();
                const std::size_t n_nodes_ref = r_ref_geom.PointsNumber();
                array_1d<double,3> loc_coords;
                double jacobian_det;
                Matrix jacobian_mat(domain_size, domain_size);
                Matrix jacobian_mat_inv(domain_size, domain_size);
                Matrix DN_DX_loc(n_nodes_ref, domain_size);
                r_ref_geom.PointLocalCoordinates(loc_coords, coords);
                r_ref_geom.Jacobian(jacobian_mat, loc_coords);
                MathUtils<double>::InvertMatrix(jacobian_mat, jacobian_mat_inv, jacobian_det);
                r_ref_geom.ShapeFunctionsLocalGradients(DN_DX_loc, loc_coords);
                Matrix DN_DX = prod(DN_DX_loc, jacobian_mat_inv);

                // Interpolate the Gauss pt. value in reference mesh
                i_gauss_val_exact = ZeroMatrix(domain_size, domain_size);
                for (std::size_t d1 = 0; d1 < domain_size; ++d1) {
                    for (std::size_t d2 = 0; d2 < domain_size; ++d2) {
                        for (std::size_t i_node_ref = 0; i_node_ref < n_nodes_ref; ++i_node_ref) {
                            const auto& r_node_ref = p_ref_mesh_elem->GetGeometry()[i_node_ref];
                            const auto& r_val = r_node_ref.FastGetSolutionStepValue(rVariable);
                            i_gauss_val_exact(d1, d2) += DN_DX(i_node_ref, d2) * r_val[d1];
                        }
                    }
                }

                // Interpolate the Gauss pt. value in current mesh
                i_gauss_val = ZeroMatrix(domain_size,domain_size);
                for (std::size_t d1 = 0; d1 < domain_size; ++d1) {
                    for (std::size_t d2 = 0; d2 < domain_size; ++d2) {
                        for (int i_node = 0; i_node < n_nodes; ++i_node) {
                            const auto& r_node = (*p_geom)[i_node];
                            const auto& i_DN_DX = row(rDN_DX, i_node);
                            const auto& r_val = r_node.FastGetSolutionStepValue(rVariable);
                            i_gauss_val(d1, d2) += i_DN_DX(d2) * r_val[d1];
                        }
                    }
                }

                // Add the current Gauss pt. error to the total norm
                i_gauss_err = i_gauss_val - i_gauss_val_exact;
                val_error_norm += w * (std::pow(norm_frobenius(i_gauss_err),2));
            }
        }
    }

    val_error_norm = std::sqrt(val_error_norm);
    return val_error_norm;
}

double GaussPointErrorUtility::ExecuteGradient()
{
    // Initialize values
    double grad_error_norm = 0.0;

    // Loop the elements to calculate the error
    const int n_elems = mrModelPart.NumberOfElements();
    for (int i_elem = 0; i_elem < n_elems; ++i_elem) {
        auto it_elem = mrModelPart.ElementsBegin() + i_elem;
        const auto p_geom = it_elem->pGetGeometry();
        const int n_nodes = p_geom->PointsNumber();
        const auto d_vect = SetDistancesVector(it_elem);

        // Initialize values
        Matrix r_N_container;
        ModifiedShapeFunctions::ShapeFunctionsGradientsType r_DN_DX_container;
        const GeometryData::IntegrationMethod integration_method = GeometryData::IntegrationMethod::GI_GAUSS_2;
        const auto& r_integrations_points = p_geom->IntegrationPoints(integration_method);

        // Check if element is ACTIVE (inactive elements are not assembled to the system)
        if (ElementIsPositive(p_geom, d_vect)) {

            r_N_container = p_geom->ShapeFunctionsValues(integration_method);
            p_geom->ShapeFunctionsIntegrationPointsGradients(r_DN_DX_container, integration_method);
            Vector det_J_vector(r_integrations_points.size());
            p_geom->DeterminantOfJacobian(det_J_vector, integration_method);

            const unsigned int n_gauss = r_integrations_points.size();
            for (int i_gauss = 0; i_gauss < n_gauss; ++i_gauss ) {
                // Get Gauss pt. data
                const Vector& rN = row(r_N_container, i_gauss);
                const Matrix& rDN_DX = r_DN_DX_container[i_gauss];
                const double w = r_integrations_points[i_gauss].Weight() * det_J_vector[i_gauss];

                // Calculate the Gauss pt. coordinates
                array_1d<double,3> coords = ZeroVector(3);
                coords[0] = ((*p_geom)[0]).X()*rN[0] + ((*p_geom)[1]).X()*rN[1] + ((*p_geom)[2]).X()*rN[2];
                coords[1] = ((*p_geom)[0]).Y()*rN[0] + ((*p_geom)[1]).Y()*rN[1] + ((*p_geom)[2]).Y()*rN[2];

                // Interpolate the Gauss pt. error
                array_1d<double,3> i_gauss_grad = ZeroVector(3);
                array_1d<double,3> i_gauss_exact_grad = ZeroVector(3);
                for (int i_node = 0; i_node < n_nodes; ++i_node) {
                    const auto& r_node = (*p_geom)[i_node];
                    const auto& i_DN_DX = row(rDN_DX, i_node);
                    for (std::size_t d = 0; d < i_DN_DX.size(); ++d) {
                        i_gauss_grad(d) += i_DN_DX(d) * r_node.FastGetSolutionStepValue(TEMPERATURE);
                        // i_gauss_exact_grad(d) += i_DN_DX(d) * CalculateTemperatureExactSolution(r_node.Coordinates());
                    }
                }
                i_gauss_exact_grad += CalculateTemperatureExactSolutionGradient(coords);
                array_1d<double,3> i_gauss_grad_err = i_gauss_grad - i_gauss_exact_grad;

                // Add the current Gauss pt. error to the total norm
                grad_error_norm += w * std::pow(norm_2(i_gauss_grad_err), 2);
            }
        }
    }

    grad_error_norm = std::sqrt(grad_error_norm);
    return grad_error_norm;
}

double GaussPointErrorUtility::ExecuteOnConditions(ModelPart& rModelPart)
{
    // Initialize values
    double f_error_norm = 0.0;

    // Loop the elements to calculate the error
    const int n_conds = rModelPart.NumberOfConditions();
    for (int i_cond = 0; i_cond < n_conds; ++i_cond) {
        auto it_cond = rModelPart.ConditionsBegin() + i_cond;
        const auto& r_geom = it_cond->GetGeometry();
        const std::size_t n_nodes = r_geom.PointsNumber();

        const double w = it_cond->GetValue(INTEGRATION_WEIGHT);
        const array_1d<double,3>& r_normal = it_cond->GetValue(NORMAL);
        const auto& r_sh_func_grad = it_cond->GetValue(SHAPE_FUNCTIONS_GRADIENT_MATRIX);

        array_1d<double,3> temp_grad = ZeroVector(3);
        for (std::size_t i_node = 0; i_node < n_nodes; ++i_node) {
            const double i_temp = r_geom[i_node].FastGetSolutionStepValue(TEMPERATURE);
            for (std::size_t d = 0; d < r_sh_func_grad.size2(); ++d) {
                temp_grad(d) += r_sh_func_grad(i_node,d) * i_temp;
            }
        }

        const double flux = inner_prod(temp_grad, r_normal);
        const double flux_exact = CalculateTemperatureFluxExactSolution(it_cond->GetValue(INTEGRATION_COORDINATES), r_normal);
        f_error_norm += w * std::pow(flux-flux_exact, 2);
    }

    f_error_norm = std::sqrt(f_error_norm);
    return f_error_norm;
}

double GaussPointErrorUtility::ExecuteOnConditionsGradient(ModelPart& rModelPart)
{
    // Initialize values
    double grad_error_norm = 0.0;

    // Loop the elements to calculate the error
    const int n_conds = rModelPart.NumberOfConditions();
    for (int i_cond = 0; i_cond < n_conds; ++i_cond) {
        auto it_cond = rModelPart.ConditionsBegin() + i_cond;
        const auto& r_geom = it_cond->GetGeometry();
        const std::size_t n_nodes = r_geom.PointsNumber();

        const double w = it_cond->GetValue(INTEGRATION_WEIGHT);
        const auto& r_sh_func_grad = it_cond->GetValue(SHAPE_FUNCTIONS_GRADIENT_MATRIX);

        array_1d<double,3> temp_grad = ZeroVector(3);
        for (std::size_t i_node = 0; i_node < n_nodes; ++i_node) {
            const double i_temp = r_geom[i_node].FastGetSolutionStepValue(TEMPERATURE);
            for (std::size_t d = 0; d < r_sh_func_grad.size2(); ++d) {
                temp_grad(d) += r_sh_func_grad(i_node,d) * i_temp;
            }
        }

        const auto grad_exact = CalculateTemperatureExactSolutionGradient(it_cond->GetValue(INTEGRATION_COORDINATES));
        array_1d<double,3> i_gauss_grad_err = temp_grad - grad_exact;
        grad_error_norm += w * std::pow(norm_2(i_gauss_grad_err), 2);
    }

    grad_error_norm = std::sqrt(grad_error_norm);
    return grad_error_norm;
}

double GaussPointErrorUtility::ExecuteOnConditionsSolution(ModelPart& rModelPart)
{
    // Initialize values
    double error_norm = 0.0;

    // Loop the elements to calculate the error
    const int n_conds = rModelPart.NumberOfConditions();
    for (int i_cond = 0; i_cond < n_conds; ++i_cond) {
        auto it_cond = rModelPart.ConditionsBegin() + i_cond;
        const auto& r_geom = it_cond->GetGeometry();
        const std::size_t n_nodes = r_geom.PointsNumber();

        const double w = it_cond->GetValue(INTEGRATION_WEIGHT);
        const auto& r_sh_func = it_cond->GetValue(SHAPE_FUNCTIONS_VECTOR);

        double temp = 0.0;
        for (std::size_t i_node = 0; i_node < n_nodes; ++i_node) {
            const double i_temp = r_geom[i_node].FastGetSolutionStepValue(TEMPERATURE);
            temp += r_sh_func(i_node) * i_temp;
        }

        const double temp_exact = CalculateTemperatureExactSolution(it_cond->GetValue(INTEGRATION_COORDINATES));
        error_norm += w * std::pow(temp-temp_exact, 2);
    }

    error_norm = std::sqrt(error_norm);
    return error_norm;
}

int GaussPointErrorUtility::Check()
{
    // Check that model part is not empty
    KRATOS_ERROR_IF(mrModelPart.NumberOfNodes() == 0) << "There are no nodes in the origin model part." << std::endl;
    KRATOS_ERROR_IF(mrModelPart.NumberOfElements() == 0) << "There are no elements in the origin model part." << std::endl;

    return 0;
}

/* Protected functions ****************************************************/

/* Private functions ******************************************************/

bool GaussPointErrorUtility::ElementIsPositive(
    Geometry<Node<3>>::Pointer pGeometry,
    const Vector &rNodalDistances)
{
    const unsigned int pts_number = pGeometry->PointsNumber();
    unsigned int n_pos (0);

    for (unsigned int i_node = 0; i_node < pts_number; ++i_node){
        if (rNodalDistances[i_node] > 0.0)
            n_pos++;
    }

    const bool is_positive = (n_pos == pts_number) ? true : false;

    return is_positive;
}

bool GaussPointErrorUtility::ElementIsSplit(
    Geometry<Node<3>>::Pointer pGeometry,
    const Vector &rNodalDistances)
{
    const unsigned int pts_number = pGeometry->PointsNumber();
    unsigned int n_pos (0), n_neg(0);

    for (unsigned int i_node = 0; i_node < pts_number; ++i_node){
        if (rNodalDistances[i_node] > 0.0)
            n_pos++;
        else
            n_neg++;
    }

    const bool is_split = (n_pos > 0 && n_neg > 0) ? true : false;

    return is_split;
}

const Vector GaussPointErrorUtility::SetDistancesVector(ModelPart::ElementIterator ItElem)
{
    const auto &r_geom = ItElem->GetGeometry();
    Vector nodal_distances(r_geom.PointsNumber());

    switch (mLevelSetType) {
        // Continuous nodal distance function case
        case LevelSetType::Continuous:
            for (unsigned int i_node = 0; i_node < r_geom.PointsNumber(); ++i_node) {
                nodal_distances[i_node] = r_geom[i_node].FastGetSolutionStepValue(*mpNodalDistanceVariable);
            }
            break;
        // Discontinuous elemental distance function case
        case LevelSetType::Discontinuous:
            nodal_distances = ItElem->GetValue(*mpElementalDistanceVariable);
            break;
        // Default error case
        default:
            KRATOS_ERROR << "Asking for a non-implemented modified shape functions type.";
    }

    return nodal_distances;
}

ModifiedShapeFunctions::Pointer GaussPointErrorUtility::SetModifiedShapeFunctionsUtility(
    const Geometry<Node<3>>::Pointer pGeometry,
    const Vector& rNodalDistances)
{
    // Get the geometry type
    const GeometryData::KratosGeometryType geometry_type = pGeometry->GetGeometryType();

    // Return the modified shape functions utility
    switch (mShapeFunctionsType) {
        case ShapeFunctionsType::Standard:
            switch (geometry_type) {
                case GeometryData::KratosGeometryType::Kratos_Triangle2D3:
                    return Kratos::make_shared<Triangle2D3ModifiedShapeFunctions>(pGeometry, rNodalDistances);
                case GeometryData::KratosGeometryType::Kratos_Tetrahedra3D4:
                    return Kratos::make_shared<Tetrahedra3D4ModifiedShapeFunctions>(pGeometry, rNodalDistances);
                default:
                    KRATOS_ERROR << "Asking for a non-implemented modified shape functions geometry.";
            }
        case ShapeFunctionsType::Ausas:
            switch (geometry_type) {
                case GeometryData::KratosGeometryType::Kratos_Triangle2D3:
                    return Kratos::make_shared<Triangle2D3AusasModifiedShapeFunctions>(pGeometry, rNodalDistances);
                case GeometryData::KratosGeometryType::Kratos_Tetrahedra3D4:
                    return Kratos::make_shared<Tetrahedra3D4AusasModifiedShapeFunctions>(pGeometry, rNodalDistances);
                default:
                    KRATOS_ERROR << "Asking for a non-implemented Ausas modified shape functions geometry.";
            }
        default:
            KRATOS_ERROR << "Asking for a non-implemented modified shape functions type.";
    }
}

// // Parabolic solution for a unit radious circle with unit source term
// double GaussPointErrorUtility::CalculateTemperatureExactSolution(const array_1d<double,3>& rCoords)
// {
//     return (1.0 - std::pow(rCoords[0],2) - std::pow(rCoords[1],2)) / 4.0;
// }

// // Parabolic solution for a unit radious circle with unit source term
// double GaussPointErrorUtility::CalculateTemperatureExactSolution(const array_1d<double,3>& rCoords)
// {
//     return rCoords[0] + rCoords[1];
// }

// array_1d<double,3> GaussPointErrorUtility::CalculateTemperatureExactSolutionGradient(const array_1d<double,3>& rCoords)
// {
//     array_1d<double,3> grad;
//     grad[0] = 1.0;
//     grad[1] = 1.0;
//     grad[2] = 0.0;
//     return grad;
// }

// // Parabolic solution for 1x1 square test
// array_1d<double,3> GaussPointErrorUtility::CalculateTemperatureExactSolutionGradient(const array_1d<double,3>& rCoords)
// {
//     const double phi_max = 1.0;
//     array_1d<double,3> grad;
//     grad[0] = 0.0;
//     grad[1] = -2.0*phi_max*rCoords[1];
//     grad[2] = 0.0;
//     return grad;
// }

// array_1d<double,3> GaussPointErrorUtility::CalculateTemperatureExactSolutionGradient(const array_1d<double,3>& rCoords)
// {
//     array_1d<double,3> grad;
//     grad[0] = 2.0*rCoords[0];
//     grad[1] = 2.0*rCoords[1];
//     grad[2] = 0.0;
//     return grad;
// }

// array_1d<double,3> GaussPointErrorUtility::CalculateTemperatureExactSolutionGradient(const array_1d<double,3>& rCoords)
// {
//     array_1d<double,3> grad;
//     grad[0] = -2.0*rCoords[0];
//     grad[1] = 0.0;
//     grad[2] = 0.0;
//     return grad;
// }

// array_1d<double,3> GaussPointErrorUtility::CalculateTemperatureExactSolutionGradient(const array_1d<double,3>& rCoords)
// {
//     array_1d<double,3> grad;
//     grad[0] = -0.5*rCoords[0];
//     grad[1] = -0.5*rCoords[1];
//     grad[2] = 0.0;
//     return grad;
// }

// Manufactured solution gradient for the "annulus" plate test
array_1d<double,3> GaussPointErrorUtility::CalculateTemperatureExactSolutionGradient(const array_1d<double,3>& rCoords)
{
    array_1d<double,3> grad;
    const double x = rCoords[0];
    const double y = rCoords[1];
    grad[0] = 0.25*(-2*x + 2*x/(std::pow(x,2) + std::pow(y,2))) + 0.25*std::cos(x)*std::sinh(y);
    grad[1] = 0.25*(-2*y + 2*y/(std::pow(x,2) + std::pow(y,2))) + 0.25*std::sin(x)*std::cosh(y);
    grad[2] = 0.0;
    return grad;
}

// // Manufactured solution for the "zig-zag" plate test
// double GaussPointErrorUtility::CalculateTemperatureExactSolution(const array_1d<double,3>& rCoords)
// {
//     const double x = rCoords[0];
//     const double y = rCoords[1];
//     return y*std::sin(2.0*Globals::Pi*x) - x*std::cos(2.0*Globals::Pi*y);
// }

// Manufactured solution for the "annulus" plate test
double GaussPointErrorUtility::CalculateTemperatureExactSolution(const array_1d<double,3>& rCoords)
{
    const double x = rCoords[0];
    const double y = rCoords[1];
    return 0.25*(9.0 - std::pow(x,2) - std::pow(y,2) - 2.0*std::log(3.0) + std::log(std::pow(x,2)+std::pow(y,2))) + 0.25*std::sin(x)*std::sinh(y);
}

// // Parabolic solution for 1x1 square test
// double GaussPointErrorUtility::CalculateTemperatureExactSolution(const array_1d<double,3>& rCoords)
// {
//     const double phi_max = 1.0;
//     const double x = rCoords[0];
//     const double y = rCoords[1];
//     return phi_max*(1.0-std::pow(y,2));
// }

// double GaussPointErrorUtility::CalculatePressureExactSolution(const array_1d<double,3>& rCoords)
// {
//     return 0.0;
// }

// array_1d<double,3> GaussPointErrorUtility::CalculateVelocityExactSolution(const array_1d<double,3>& rCoords)
// {
//     array_1d<double,3> v_exact = ZeroVector(3);
//     v_exact[0] = 1.0 - (1.0/1.1) * rCoords[1];
//     v_exact[1] = 0.0;
//     return v_exact;
// }

// double GaussPointErrorUtility::CalculateTemperatureFluxExactSolution(
//     const array_1d<double,3>& rCoords,
//     const array_1d<double,3>& rNormal)
// {
//     const double x = rCoords[0];
//     const double y = rCoords[1];
//     array_1d<double,3> temp_grad = ZeroVector(3);
//     temp_grad[0] = 2*x;
//     temp_grad[1] = 2*y;
//     return inner_prod(temp_grad, rNormal);
// }

// double GaussPointErrorUtility::CalculateTemperatureFluxExactSolution(
//     const array_1d<double,3>& rCoords,
//     const array_1d<double,3>& rNormal)
// {
//     array_1d<double,3> temp_grad = ZeroVector(3);
//     temp_grad[0] = 1.0;
//     temp_grad[1] = 1.0;
//     return inner_prod(temp_grad, rNormal);
// }

// Parabolic solution for 1x1 square test
double GaussPointErrorUtility::CalculateTemperatureFluxExactSolution(
    const array_1d<double,3>& rCoords,
    const array_1d<double,3>& rNormal)
{
    const double phi_max = 1.0;
    array_1d<double,3> temp_grad = ZeroVector(3);
    temp_grad[0] = 0.0;
    temp_grad[1] = -2.0*phi_max*rCoords[1];
    return inner_prod(temp_grad, rNormal);
}

// double GaussPointErrorUtility::CalculateTemperatureFluxExactSolution(
//     const array_1d<double,3>& rCoords,
//     const array_1d<double,3>& rNormal)
// {
//     const double x = rCoords[0];
//     const double y = rCoords[1];
//     array_1d<double,3> temp_grad = ZeroVector(3);
//     temp_grad[0] = -2.0*x;
//     temp_grad[1] = 0.0;
//     return inner_prod(temp_grad, rNormal);
// }

// double GaussPointErrorUtility::CalculateTemperatureFluxExactSolution(
//     const array_1d<double,3>& rCoords,
//     const array_1d<double,3>& rNormal)
// {
//     const double x = rCoords[0];
//     const double y = rCoords[1];
//     array_1d<double,3> temp_grad = ZeroVector(3);
//     temp_grad[0] = -0.5*x;
//     temp_grad[1] = -0.5*y;
//     return inner_prod(temp_grad, rNormal);
// }

};  // namespace Kratos.
