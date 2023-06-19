//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//
//

// System includes

// External includes

// Project includes
#include "testing/testing.h"
#include "geometries/point.h"
#include "mpi/utilities/mpi_search_utilities.h"

namespace Kratos::Testing 
{

KRATOS_DISTRIBUTED_TEST_CASE_IN_SUITE(MPISynchronousPointSynchronizationAllPointsAreTheSame, KratosMPICoreFastSuite) 
{
    // The data communicator
    const DataCommunicator& r_data_comm = Testing::GetDefaultDataCommunicator();

    // Define test data
    std::vector<Point> points = {Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0), Point(2.0, 2.0, 2.0)};
    std::vector<double> expected_coordinates = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0};

    // Call the function
    std::vector<double> all_points_coordinates;
    const int number_of_points = MPISearchUtilities::MPISynchronousPointSynchronization(points.begin(), points.end(), all_points_coordinates, r_data_comm);

    // Check the results
    KRATOS_CHECK_EQUAL(number_of_points, 3);
    KRATOS_CHECK_EQUAL(all_points_coordinates, expected_coordinates);
}

KRATOS_DISTRIBUTED_TEST_CASE_IN_SUITE(MPISynchronousPointSynchronizationAllPointsAreDifferent, KratosMPICoreFastSuite) 
{
    // The data communicator
    const DataCommunicator& r_data_comm = Testing::GetDefaultDataCommunicator();

    // MPI data communicator
    const int rank = r_data_comm.Rank();
    const int world_size = r_data_comm.Size();

    // Define test data
    double value = static_cast<double>(rank);
    double value2 = 2 * value;
    std::vector<Point> points({Point(value, value, value), Point(value2, value2, value2)});

    // Call the function
    std::vector<double> all_points_coordinates;
    MPISearchUtilities::MPISynchronousPointSynchronization(points.begin(), points.end(), all_points_coordinates, r_data_comm);

    // Check the results
    KRATOS_CHECK_EQUAL(static_cast<int>(all_points_coordinates.size()), 2 * 3 * world_size);
    for (int i_rank = 0; i_rank < world_size; ++i_rank) {
        value = static_cast<double>(i_rank);
        value2 = 2 * value;
        for (int j = 0; j < 3; ++j) {
            KRATOS_CHECK_DOUBLE_EQUAL(all_points_coordinates[i_rank * 6 + j    ], value );
            KRATOS_CHECK_DOUBLE_EQUAL(all_points_coordinates[i_rank * 6 + j + 3], value2);
        }
    }
}

KRATOS_DISTRIBUTED_TEST_CASE_IN_SUITE(MPISynchronousPointSynchronizationAllPointsAreDifferentWithDistances, KratosMPICoreFastSuite) 
{
    // The data communicator
    const DataCommunicator& r_data_comm = Testing::GetDefaultDataCommunicator();

    // MPI data communicator
    const int rank = r_data_comm.Rank();
    const int world_size = r_data_comm.Size();

    // Define test data
    double value = static_cast<double>(rank);
    double value2 = 2 * value;
    std::vector<Point> points({Point(value, value, value), Point(value2, value2, value2)});
    std::vector<double> local_distances = {value, value2};

    // Call the function
    std::vector<double> all_points_coordinates;
    auto distances = MPISearchUtilities::MPISynchronousPointSynchronizationWithDistances(points.begin(), points.end(), all_points_coordinates, local_distances, r_data_comm);

    // Check the results
    KRATOS_CHECK_EQUAL(static_cast<int>(all_points_coordinates.size()), 2 * 3 * world_size);
    for (int i_rank = 0; i_rank < world_size; ++i_rank) {
        value = static_cast<double>(i_rank);
        value2 = 2 * value;
        KRATOS_CHECK_DOUBLE_EQUAL(distances[i_rank * 2    ], value );
        KRATOS_CHECK_DOUBLE_EQUAL(distances[i_rank * 2 + 1], value2);
        for (int j = 0; j < 3; ++j) {
            KRATOS_CHECK_DOUBLE_EQUAL(all_points_coordinates[i_rank * 6 + j    ], value );
            KRATOS_CHECK_DOUBLE_EQUAL(all_points_coordinates[i_rank * 6 + j + 3], value2);
        }
    }
}

}  // namespace Kratos::Testing.