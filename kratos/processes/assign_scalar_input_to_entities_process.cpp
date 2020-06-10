//
//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "containers/model.h"
#include "utilities/string_utilities.h"
#include "utilities/variable_utils.h"
#include "processes/assign_scalar_input_to_entities_process.h"

namespace Kratos
{

/// Local Flags
template<class TEntity>
const Kratos::Flags AssignScalarInputToEntitiesProcess<TEntity>::GEOMETRIC_DEFINITION(Kratos::Flags::Create(0));

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
AssignScalarInputToEntitiesProcess<TEntity>::AssignScalarInputToEntitiesProcess(
    ModelPart& rModelPart,
    Parameters rParameters
    ) : Process(Flags()) ,
        mrModelPart(rModelPart)
{
    KRATOS_TRY

    // Validate against defaults -- this ensures no type mismatch
    const Parameters default_parameters = GetDefaultParameters();
    rParameters.ValidateAndAssignDefaults(default_parameters);

    const std::string& r_variable_name = rParameters["variable_name"].GetString();
    KRATOS_ERROR_IF_NOT(KratosComponents<Variable<double>>::Get(r_variable_name)) << "The variable " << r_variable_name << " does not exist" << std::endl;
    mpVariable = &KratosComponents<Variable<double>>::Get(r_variable_name);

    // Getting algorithm
    mAlgorithm = ConvertAlgorithmString(rParameters["transfer_algorithm"].GetString());

    // Get the geometry or entities
    const std::string& r_filename = rParameters["file"].GetString();
    if (StringUtilities::ContainsPartialString(r_filename, ".txt")) {
        IdentifyDataTXT(r_filename);
    } else if (StringUtilities::ContainsPartialString(r_filename, ".json")) {
        IdentifyDataJSON(r_filename);
    } else {
        KRATOS_ERROR << "The process is only compatible with JSON and TXT" << std::endl;
    }

    // Read the input file
    if (StringUtilities::ContainsPartialString(r_filename, ".txt")) {
        ReadDataTXT(r_filename);
    } else if (StringUtilities::ContainsPartialString(r_filename, ".json")) {
        ReadDataJSON(r_filename);
    } else {
        KRATOS_ERROR << "The process is only compatible with JSON and TXT" << std::endl;
    }

    // Compute the extrpolation weights
    ComputeExtrapolationWeight();

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
void AssignScalarInputToEntitiesProcess<TEntity>::ExecuteInitializeSolutionStep()
{
    KRATOS_TRY;

    // Get time
    const double time = mrModelPart.GetProcessInfo().GetValue(TIME);

    // Case of only one entity defined
    const SizeType number_of_databases = mCoordinates.size();
    const auto& r_var_database = mDatabase.GetVariableData(*mpVariable);
    if (number_of_databases == 1) {
        InternalAssignValue<>(*mpVariable, r_var_database.GetValue(0, time));
    } else {
        // Getting entities array
        auto& r_entities_array = GetEntitiesContainer();
        const int number_of_entities = static_cast<int>(r_entities_array.size());

        // Initialize values
        VariableUtils().SetNonHistoricalVariable(*mpVariable, 0.0, r_entities_array);

        if(number_of_entities != 0) {
            const auto it_begin = r_entities_array.begin();

            #pragma omp parallel for
            for(int i = 0; i < number_of_entities; i++) {
                auto it_entity = it_begin + i;

                const auto& r_weights = mWeightExtrapolation[i];
                for (auto& r_weight : r_weights) {
                    it_entity->GetValue(*mpVariable) += r_weight.second * r_var_database.GetValue(r_weight.first, time);
                }
            }
        }
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
const Parameters AssignScalarInputToEntitiesProcess<TEntity>::GetDefaultParameters() const
{
    const Parameters default_parameters( R"(
    {
        "model_part_name"    : "MODEL_PART_NAME",
        "mesh_id"            : 0,
        "variable_name"      : "VARIABLE_NAME",
        "file"               : "",
        "transfer_algorithm" : "nearest_neighbour"
    }  )" );
    return default_parameters;
}

/***********************************************************************************/
/***********************************************************************************/

template<>
array_1d<double, 3> AssignScalarInputToEntitiesProcess<Node<3>>:: GetCoordinatesEntity(const IndexType Id)
{
    return mrModelPart.pGetNode(Id)->Coordinates();
}

/***********************************************************************************/
/***********************************************************************************/

template<>
array_1d<double, 3> AssignScalarInputToEntitiesProcess<Condition>:: GetCoordinatesEntity(const IndexType Id)
{
    return mrModelPart.pGetCondition(Id)->GetGeometry().Center().Coordinates();
}

/***********************************************************************************/
/***********************************************************************************/

template<>
array_1d<double, 3> AssignScalarInputToEntitiesProcess<Element>:: GetCoordinatesEntity(const IndexType Id)
{
    return mrModelPart.pGetElement(Id)->GetGeometry().Center().Coordinates();
}

/***********************************************************************************/
/***********************************************************************************/

template<>
PointerVectorSet<Node<3>, IndexedObject>& AssignScalarInputToEntitiesProcess<Node<3>>::GetEntitiesContainer()
{
    return mrModelPart.GetMesh().Nodes();
}

/***********************************************************************************/
/***********************************************************************************/

template<>
PointerVectorSet<Condition, IndexedObject>& AssignScalarInputToEntitiesProcess<Condition>::GetEntitiesContainer()
{
    return mrModelPart.GetMesh().Conditions();
}

/***********************************************************************************/
/***********************************************************************************/

template<>
PointerVectorSet<Element, IndexedObject>& AssignScalarInputToEntitiesProcess<Element>::GetEntitiesContainer()
{
    return mrModelPart.GetMesh().Elements();
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
void AssignScalarInputToEntitiesProcess<TEntity>::IdentifyDataTXT(const std::string& rFileName)
{
    KRATOS_TRY;

    // Read txt
    std::ifstream infile(rFileName);
    KRATOS_ERROR_IF_NOT(infile.good()) << "TXT file: " << rFileName << " cannot be found" << std::endl;
    std::stringstream buffer;
    buffer << infile.rdbuf();

    // First line
    std::string line;
    std::getline(buffer, line);

    // Checking if geometric definition or entity identifier
    if (StringUtilities::ContainsPartialString(line, "(") && StringUtilities::ContainsPartialString(line, ")")) {
        this->Set(GEOMETRIC_DEFINITION, true);
    } else {
        this->Set(GEOMETRIC_DEFINITION, false);
    }

    std::istringstream iss(line);
    std::string token;
    SizeType counter = 0;
    std::string::size_type sz;     // alias of size_t
    if (this->Is(GEOMETRIC_DEFINITION)) {
        array_1d<double, 3> aux_array;
        while(std::getline(iss, token, '\t')) {
            if (counter > 0) {
                std::string aux_string = StringUtilities::ErasePartialString(token, "(");
                aux_string = StringUtilities::ErasePartialString(aux_string, ")");
                std::stringstream s_stream(aux_string); // Create string stream from the string
                SizeType sub_counter = 0;
                std::string substr;
                while(s_stream.good()) {
                    std::getline(s_stream, substr, ','); // Get first string delimited by comma
                    aux_array[sub_counter] = std::stod(substr, &sz);
                    ++sub_counter;
                }
                mCoordinates.push_back(aux_array);
            }
            ++counter;
        }
    } else {
        while(std::getline(iss, token, '\t')) {
            if (counter > 0) {
                const IndexType id = std::stod(token, &sz);
                mCoordinates.push_back(GetCoordinatesEntity(id));
            }
            ++counter;
        }
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
void AssignScalarInputToEntitiesProcess<TEntity>::IdentifyDataJSON(const std::string& rFileName)
{
    KRATOS_TRY;

    // Reading json file
    std::ifstream infile(rFileName);
    KRATOS_ERROR_IF_NOT(infile.good()) << "JSON file: " << rFileName << " cannot be found" << std::endl;
    std::stringstream buffer;
    buffer << infile.rdbuf();
    Parameters json_input(buffer.str());

    // Getting label
    const std::string ent_label = GetEntitiesLabel();

    // TODO

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
void AssignScalarInputToEntitiesProcess<TEntity>::ReadDataTXT(const std::string& rFileName)
{
    KRATOS_TRY;

    // Initialize the databases
    std::vector<IndexType> variables_ids(1);
    variables_ids[0] = mpVariable->Key();
    std::vector<IndexType> values_sizes(1, 1);
    const SizeType number_of_definitions = mCoordinates.size();
    mDatabase.Initialize(variables_ids, values_sizes, number_of_definitions);

    // Read txt
    std::ifstream infile(rFileName);
    KRATOS_ERROR_IF_NOT(infile.good()) << "TXT file: " << rFileName << " cannot be found" << std::endl;
    std::stringstream buffer;
    buffer << infile.rdbuf();

    // First line
    std::string line;
    std::getline(buffer, line);

    // The other lines
    SizeType number_time_steps = 0;
    while(std::getline(buffer, line)) {
        ++number_time_steps;
    }

    Vector time = ZeroVector(number_time_steps);
    std::vector<Vector> values(number_of_definitions, time);

    // Reset buffer
    buffer.str("");
    buffer << infile.rdbuf();

    // First line
    std::getline(buffer, line);

    // The other lines
    SizeType counter = 0;
    SizeType sub_counter = 0;
    std::string::size_type sz;     // alias of size_t
    while(std::getline(buffer, line)) {
        std::istringstream iss(line);
        std::string token;
        sub_counter = 0;
        while(std::getline(iss, token, '\t')) {
            if (sub_counter == 0) {
                time[counter] = std::stod(token, &sz);
            } else {
                values[sub_counter - 1][counter] = std::stod(token, &sz);
            }
            ++sub_counter;
        }
        ++counter;
    }

    // Set the time table
    mDatabase.SetCommonColumn(time);

    // Set the entities values
    auto& r_var_database = mDatabase.GetVariableData(*mpVariable);
    for (IndexType i = 0; i < values.size(); ++i) {
        r_var_database.SetValues(time, values[i], i);
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
void AssignScalarInputToEntitiesProcess<TEntity>::ReadDataJSON(const std::string& rFileName)
{
    KRATOS_TRY;

    // Reading json file
    std::ifstream infile(rFileName);
    KRATOS_ERROR_IF_NOT(infile.good()) << "JSON file: " << rFileName << " cannot be found" << std::endl;
    std::stringstream buffer;
    buffer << infile.rdbuf();
    Parameters json_input(buffer.str());

    // Initialize the databases
    std::vector<IndexType> variables_ids(1);
    variables_ids[0] = mpVariable->Key();
    std::vector<IndexType> values_sizes(1, 1);
    const SizeType number_of_definitions = mCoordinates.size();
    mDatabase.Initialize(variables_ids, values_sizes, number_of_definitions);

    // Get the time vector
    const Vector& r_time = json_input["TIME"].GetVector();
    mDatabase.SetCommonColumn(r_time);

    // Fill database
    auto& r_var_database = mDatabase.GetVariableData(*mpVariable);
    const std::string& r_variable_name = mpVariable->Name();
    for (IndexType i = 0; i < number_of_definitions; ++i) {
        const std::string identifier = "VALUE_" + std::to_string(i);
        const auto& r_vector = json_input[identifier][r_variable_name].GetVector();
        r_var_database.SetValues(r_time, r_vector, i);
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template<class TEntity>
void AssignScalarInputToEntitiesProcess<TEntity>::ComputeExtrapolationWeight()
{
    KRATOS_TRY;

    // Some definitions
    const auto& r_entities_array = GetEntitiesContainer();
    const auto it_ent_begin = r_entities_array.begin();
    const SizeType number_of_entities = r_entities_array.size();

    // Resize the weight extrapolation vector
    if (mWeightExtrapolation.size() != number_of_entities) {
        mWeightExtrapolation.resize(number_of_entities);
    }

    // Considering different algorithms to fill the weights
    const SizeType number_of_definitions = mCoordinates.size();
    if (mAlgorithm == Algorithm::NEAREST_NEIGHBOUR) {
        #pragma omp parallel for
        for (int i = 0; i < static_cast<int>(number_of_entities); ++i) {
            auto it_ent = it_ent_begin + i;
            const IndexType id = it_ent->Id();
            const array_1d<double, 3> coordinates = GetCoordinatesEntity(id);
            double distance = 1.0e24;
            IndexType index = 0;
            for (IndexType i = 0; i < number_of_definitions; ++i) {
                const double aux_distance = norm_2(coordinates - mCoordinates[i]);
                if (aux_distance < distance) {
                    distance = aux_distance;
                    index = i;
                }
            }
            std::unordered_map<IndexType, double> aux_map({{index, 1.0}});
            mWeightExtrapolation[i] = aux_map;
        }
    } else {
        KRATOS_ERROR << "Algorithm not defined" << std::endl;
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

template class AssignScalarInputToEntitiesProcess<Node<3>>;
template class AssignScalarInputToEntitiesProcess<Condition>;
template class AssignScalarInputToEntitiesProcess<Element>;

}  // namespace Kratos.
