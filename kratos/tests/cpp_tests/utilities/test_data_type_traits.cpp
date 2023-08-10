//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya
//

// System includes

// External includes

// Project includes
#include "testing/testing.h"
#include "utilities/data_type_traits.h"

namespace Kratos::Testing {

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsInt, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<int>;

    static_assert(std::is_same_v<type_trait::PrimitiveDataType, int>);
    static_assert(type_trait::HasContiguousPrimitiveData);
    static_assert(!type_trait::HasDynamicMemoryAllocation);

    int test = 1;

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 1U);
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), std::vector<unsigned int>{});
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, std::vector<unsigned int>{}));
    KRATOS_CHECK_EQUAL(type_trait::GetContiguousData(test), &test);

    int dummy = -1;
    type_trait::FillToContiguousData(&dummy, test);
    KRATOS_CHECK_EQUAL(dummy, test);

    dummy = -2;
    type_trait::FillFromContiguousData(dummy, &test);
    KRATOS_CHECK_EQUAL(dummy, test);

    #ifdef KRATOS_DEBUG
        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            type_trait::Reshape(test, std::vector<unsigned int>{1}),
            "Invalid shape given for primitive data type [ Expected shape = [], provided shape = [1] ]."
        );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsArray1dDouble, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<array_1d<double, 5>>;

    static_assert(std::is_same_v<type_trait::ContainerType, array_1d<double, 5>>);
    static_assert(std::is_same_v<type_trait::ValueType, double>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, double>);
    static_assert(type_trait::HasContiguousPrimitiveData);
    static_assert(!type_trait::HasDynamicMemoryAllocation);

    array_1d<double, 5> test{};

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 5);
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), std::vector<unsigned int>{5});
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, std::vector<unsigned int>{5}));
    KRATOS_CHECK_EQUAL(type_trait::GetContiguousData(test), &test[0]);

    array_1d<double, 5> dummy;
    type_trait::FillToContiguousData(dummy.data().begin(), test);
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    dummy = array_1d<double, 5>(5, -2);
    type_trait::FillFromContiguousData(dummy, test.data().begin());
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    #ifdef KRATOS_DEBUG
        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            type_trait::Reshape(test, std::vector<unsigned int>{}),
            "Invalid shape given for array_1d data type [ Expected shape = [5], provided shape = [] ]."
        );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsVectorDouble, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<Vector>;

    static_assert(std::is_same_v<type_trait::ContainerType, Vector>);
    static_assert(std::is_same_v<type_trait::ValueType, double>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, double>);
    static_assert(type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    Vector test(7, 1);

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 7);
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), std::vector<unsigned int>{7});
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, std::vector<unsigned int>{7}));
    KRATOS_CHECK(type_trait::Reshape(test, std::vector<unsigned int>{9}));
    KRATOS_CHECK_EQUAL(test.size(), 9);
    KRATOS_CHECK_EQUAL(type_trait::GetContiguousData(test), &test[0]);

    Vector dummy(9);
    type_trait::FillToContiguousData(dummy.data().begin(), test);
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    dummy = Vector(9, -2);
    type_trait::FillFromContiguousData(dummy, test.data().begin());
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    #ifdef KRATOS_DEBUG
        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            type_trait::Reshape(test, std::vector<unsigned int>{}),
            "Invalid shape given for DenseVector data type."
        );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsMatrixDouble, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<Matrix>;

    static_assert(std::is_same_v<type_trait::ContainerType, Matrix>);
    static_assert(std::is_same_v<type_trait::ValueType, double>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, double>);
    static_assert(type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    Matrix test(3, 4, 1);

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 12);

    std::vector<unsigned int> shape{3, 4};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    shape[0] = 4;
    shape[1] = 5;
    KRATOS_CHECK(type_trait::Reshape(test, shape));
    KRATOS_CHECK_EQUAL(test.size1(), 4);
    KRATOS_CHECK_EQUAL(test.size2(), 5);
    KRATOS_CHECK_EQUAL(type_trait::GetContiguousData(test), &test(0, 0));

    Matrix dummy(4, 5);
    type_trait::FillToContiguousData(dummy.data().begin(), test);
    KRATOS_CHECK_MATRIX_EQUAL(dummy, test);

    dummy = Matrix(4, 5, -2);
    type_trait::FillFromContiguousData(dummy, test.data().begin());
    KRATOS_CHECK_MATRIX_EQUAL(dummy, test);

    #ifdef KRATOS_DEBUG
        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            type_trait::Reshape(test, std::vector<unsigned int>{}),
            "Invalid shape given for DenseMatrix data type."
        );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsString, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<std::string>;

    static_assert(std::is_same_v<type_trait::ContainerType, std::string>);
    static_assert(std::is_same_v<type_trait::ValueType, char>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, char>);
    static_assert(type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    std::string test = "test";

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 4);
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), std::vector<unsigned int>{4});
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, std::vector<unsigned int>{4}));
    KRATOS_CHECK(type_trait::Reshape(test, std::vector<unsigned int>{6}));
    KRATOS_CHECK_EQUAL(test.size(), 6);
    KRATOS_CHECK_EQUAL(type_trait::GetContiguousData(test), &test[0]);

    std::string dummy = "000000";
    type_trait::FillToContiguousData(dummy.data(), test);
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    dummy = "000000";
    type_trait::FillFromContiguousData(dummy, test.data());
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    #ifdef KRATOS_DEBUG
        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            type_trait::Reshape(test, std::vector<unsigned int>{}),
            "Invalid shape given for std::string data type."
        );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsStdVectorInt, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<std::vector<int>>;

    static_assert(std::is_same_v<type_trait::ContainerType, std::vector<int>>);
    static_assert(std::is_same_v<type_trait::ValueType, int>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, int>);
    static_assert(type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    std::vector<int> test(7, 1);

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 7);
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), std::vector<unsigned int>{7});
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, std::vector<unsigned int>{7}));
    KRATOS_CHECK(type_trait::Reshape(test, std::vector<unsigned int>{9}));
    KRATOS_CHECK_EQUAL(test.size(), 9);
    KRATOS_CHECK_EQUAL(type_trait::GetContiguousData(test), &test[0]);

    std::vector<int> dummy(9);
    type_trait::FillToContiguousData(dummy.data(), test);
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    dummy = std::vector<int>(9, -2);
    type_trait::FillFromContiguousData(dummy, test.data());
    KRATOS_CHECK_VECTOR_EQUAL(dummy, test);

    #ifdef KRATOS_DEBUG
        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            type_trait::Reshape(test, std::vector<unsigned int>{}),
            "Invalid shape given for std::vector data type."
        );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsArray1dNested, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<array_1d<array_1d<array_1d<int, 10>, 3>, 5>>;

    static_assert(std::is_same_v<type_trait::ContainerType, array_1d<array_1d<array_1d<int, 10>, 3>, 5>>);
    static_assert(std::is_same_v<type_trait::ValueType, array_1d<array_1d<int, 10>, 3>>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, int>);
    static_assert(!type_trait::HasContiguousPrimitiveData);
    static_assert(!type_trait::HasDynamicMemoryAllocation);

    array_1d<array_1d<array_1d<int, 10>, 3>, 5> test{}, result{};
    std::vector<int> ref_values(150);
    for (unsigned int i = 0; i < 5; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            for (unsigned int k = 0; k < 10; ++k) {
                test[i][j][k] = (i+1)*(j+1)*(k+1);
                ref_values[i * 30 + j * 10 + k] = test[i][j][k];
            }
        }
    }

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 150);

    std::vector<unsigned int> shape{5, 3, 10};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    std::vector<int> values(150, -1);
    type_trait::FillToContiguousData(values.data(), test);
    KRATOS_CHECK_EQUAL(values, ref_values);

    type_trait::FillFromContiguousData(result, values.data());
    KRATOS_CHECK_EQUAL(test, result);
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsVectorNested, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<DenseVector<DenseVector<Vector>>>;

    static_assert(std::is_same_v<type_trait::ContainerType, DenseVector<DenseVector<Vector>>>);
    static_assert(std::is_same_v<type_trait::ValueType, DenseVector<Vector>>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, double>);
    static_assert(!type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    DenseVector<DenseVector<Vector>> test(2), result(2);
    std::vector<double> ref_values(24);
    for (unsigned int i = 0; i < 2; ++i) {
        test[i] = DenseVector<Vector>(3);
        result[i] = DenseVector<Vector>(3);
        for (unsigned int j = 0; j < 3; ++j) {
            test[i][j] = Vector(4);
            result[i][j] = Vector(4, -1);
            for (unsigned int k = 0; k < 4; ++k) {
                test[i][j][k] = (i+1)*(j+1)*(k+1);
                ref_values[i * 12 + j * 4 + k] = test[i][j][k];
            }
        }
    }

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 24);

    std::vector<unsigned int> shape{2, 3, 4};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    auto temp = test;
    shape[0] = 5;
    shape[1] = 6;
    shape[2] = 7;
    KRATOS_CHECK(type_trait::Reshape(temp, shape));
    KRATOS_CHECK_EQUAL(temp.size(), 5);
    for (unsigned int i = 0; i < 5; ++i) {
        KRATOS_CHECK_EQUAL(temp[i].size(), 6);
        for (unsigned int j = 0; j < 6; ++j) {
            KRATOS_CHECK_EQUAL(temp[i][j].size(), 7);
        }
    }

    std::vector<double> values(24, -1);
    type_trait::FillToContiguousData(values.data(), test);
    KRATOS_CHECK_EQUAL(values, ref_values);

    type_trait::FillFromContiguousData(result, values.data());
    for (unsigned int i = 0; i < 2; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            KRATOS_CHECK_VECTOR_EQUAL(result[i][j], test[i][j]);
        }
    }
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsMatrixNested, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<DenseMatrix<DenseMatrix<Matrix>>>;

    static_assert(std::is_same_v<type_trait::ContainerType, DenseMatrix<DenseMatrix<Matrix>>>);
    static_assert(std::is_same_v<type_trait::ValueType, DenseMatrix<Matrix>>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, double>);
    static_assert(!type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    DenseMatrix<DenseMatrix<Matrix>> test(2, 3), result(2, 3);
    std::vector<double> ref_values(5040);
    for (unsigned int i = 0; i < 6; ++i) {
        test.data()[i] = DenseMatrix<Matrix>(4, 5);
        result.data()[i] = DenseMatrix<Matrix>(4, 5);
        for (unsigned int j = 0; j < 20; ++j) {
            test.data()[i].data()[j] = Matrix(6, 7);
            result.data()[i].data()[j] = Matrix(6, 7, -1);
            for (unsigned int k = 0; k < 42; ++k) {
                test.data()[i].data()[j].data()[k] = (i+1)*(j+1)*(k+1);
                ref_values[i * 840 + j * 42 + k] = (i+1)*(j+1)*(k+1);
            }
        }
    }

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 2*3*4*5*6*7);

    std::vector<unsigned int> shape{2, 3, 4, 5, 6, 7};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    auto temp = test;
    shape[0] = 3;
    shape[1] = 4;
    shape[2] = 5;
    shape[3] = 6;
    shape[4] = 7;
    shape[5] = 8;
    KRATOS_CHECK(type_trait::Reshape(temp, shape));
    KRATOS_CHECK_EQUAL(temp.size1(), 3);
    KRATOS_CHECK_EQUAL(temp.size2(), 4);
    for (unsigned int i = 0; i < 12; ++i) {
        KRATOS_CHECK_EQUAL(temp.data()[i].size1(), 5);
        KRATOS_CHECK_EQUAL(temp.data()[i].size2(), 6);
        for (unsigned int j = 0; j < 30; ++j) {
            KRATOS_CHECK_EQUAL(temp.data()[i].data()[j].size1(), 7);
            KRATOS_CHECK_EQUAL(temp.data()[i].data()[j].size2(), 8);
        }
    }

    std::vector<double> values(5040, -1);
    type_trait::FillToContiguousData(values.data(), test);
    KRATOS_CHECK_EQUAL(values, ref_values);

    type_trait::FillFromContiguousData(result, values.data());
    for (unsigned int i = 0; i < 6; ++i) {
        for (unsigned int j = 0; j < 20; ++j) {
            KRATOS_CHECK_MATRIX_EQUAL(result.data()[i].data()[j], test.data()[i].data()[j]);
        }
    }
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsStdVectorNested, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<std::vector<std::vector<int>>>;

    static_assert(std::is_same_v<type_trait::ContainerType, std::vector<std::vector<int>>>);
    static_assert(std::is_same_v<type_trait::ValueType, std::vector<int>>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, int>);
    static_assert(!type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    std::vector<std::vector<int>> test(2), result(2);
    std::vector<int> ref_values(6);
    for (unsigned int i = 0; i < 2; ++i) {
        test[i] = std::vector<int>(3);
        result[i] = std::vector<int>(3, -1);
        for (unsigned int j = 0; j < 3; ++j) {
            test[i][j] = (i+1)*(j+1);
            ref_values[i * 3 + j] = test[i][j];
        }
    }

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 6);

    std::vector<unsigned int> shape{2, 3};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    auto temp = test;
    shape[0] = 3;
    shape[1] = 4;
    KRATOS_CHECK(type_trait::Reshape(temp, shape));
    KRATOS_CHECK_EQUAL(temp.size(), 3);
    for (unsigned int i = 0; i < 3; ++i) {
        KRATOS_CHECK_EQUAL(temp[i].size(), 4);
    }

    std::vector<int> values(6, -1);
    type_trait::FillToContiguousData(values.data(), test);
    KRATOS_CHECK_VECTOR_EQUAL(values, ref_values);

    type_trait::FillFromContiguousData(result, values.data());
    for (unsigned int i = 0; i < 2; ++i) {
        KRATOS_CHECK_VECTOR_EQUAL(result[i], test[i]);
    }
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsMixedNested1, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<std::vector<DenseVector<DenseMatrix<array_1d<std::string, 6>>>>>;

    static_assert(std::is_same_v<type_trait::ContainerType, std::vector<DenseVector<DenseMatrix<array_1d<std::string, 6>>>>>);
    static_assert(std::is_same_v<type_trait::ValueType, DenseVector<DenseMatrix<array_1d<std::string, 6>>>>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, char>);
    static_assert(!type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    type_trait::ContainerType test(3), result(3);
    std::vector<char> ref_values(2160);
    for (unsigned int i = 0; i < 3; ++i) {
        // loop of std::vector
        test[i] = DenseVector<DenseMatrix<array_1d<std::string, 6>>>(4);
        result[i] = DenseVector<DenseMatrix<array_1d<std::string, 6>>>(4);
        for (unsigned int j = 0; j < 4; ++j) {
            // loop of DenseVector
            test[i][j] = DenseMatrix<array_1d<std::string, 6>>(5, 6);
            result[i][j] = DenseMatrix<array_1d<std::string, 6>>(5, 6);
            for (unsigned int k = 0; k < 30; ++k) {
                test[i][j].data()[k] = array_1d<std::string, 6>(6, std::to_string(k % 10));
                result[i][j].data()[k] = array_1d<std::string ,6>(6, "a");
                std::fill(ref_values.begin() + i * 720 + j * 180 + k * 6, ref_values.begin() + i * 720 + j * 180 + k * 6 + 6, test[i][j].data()[k][0][0]);
            }
        }
    }

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 2160);

    std::vector<unsigned int> shape{3, 4, 5, 6, 6, 1};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    auto temp = test;
    shape[0] = 4;
    shape[1] = 5;
    shape[2] = 6;
    shape[3] = 7;
    shape[4] = 6;
    shape[5] = 9;
    KRATOS_CHECK(type_trait::Reshape(temp, shape));
    KRATOS_CHECK_EQUAL(temp.size(), 4);
    for (unsigned int i = 0; i < 4; ++i) {
        KRATOS_CHECK_EQUAL(temp[i].size(), 5);
        for (unsigned int j = 0; j < 5; ++j) {
            KRATOS_CHECK_EQUAL(temp[i][j].size1(), 6);
            KRATOS_CHECK_EQUAL(temp[i][j].size2(), 7);
            for (unsigned int k = 0; k < 42; ++k) {
                KRATOS_CHECK_EQUAL(temp[i][j].data()[k].size(), 6);
                for (unsigned int l = 0; l < 6; ++l) {
                    KRATOS_CHECK_EQUAL(temp[i][j].data()[k][l].size(), 9);
                }
            }
        }
    }

    std::vector<char> values(2160, 0);
    type_trait::FillToContiguousData(values.data(), test);
    KRATOS_CHECK_EQUAL(values, ref_values);

    type_trait::FillFromContiguousData(result, values.data());
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 4; ++j) {
            for (unsigned int k = 0; k < 30; ++k) {
                for (unsigned int l = 0; l < 6; ++l) {
                    KRATOS_CHECK_EQUAL(result[i][j].data()[k][l], test[i][j].data()[k][l]);
                }
            }
        }
    }
}

KRATOS_TEST_CASE_IN_SUITE(DataTypeTraitsMixedNested2, KratosCoreFastSuite)
{
    using type_trait = DataTypeTraits<std::vector<DenseVector<DenseMatrix<array_1d<double, 6>>>>>;

    static_assert(std::is_same_v<type_trait::ContainerType, std::vector<DenseVector<DenseMatrix<array_1d<double, 6>>>>>);
    static_assert(std::is_same_v<type_trait::ValueType, DenseVector<DenseMatrix<array_1d<double, 6>>>>);
    static_assert(std::is_same_v<type_trait::PrimitiveDataType, double>);
    static_assert(!type_trait::HasContiguousPrimitiveData);
    static_assert(type_trait::HasDynamicMemoryAllocation);

    type_trait::ContainerType test(3), result(3);
    std::vector<double> ref_values(2160);
    for (unsigned int i = 0; i < 3; ++i) {
        // loop of std::vector
        test[i] = DenseVector<DenseMatrix<array_1d<double, 6>>>(4);
        result[i] = DenseVector<DenseMatrix<array_1d<double, 6>>>(4);
        for (unsigned int j = 0; j < 4; ++j) {
            // loop of DenseVector
            test[i][j] = DenseMatrix<array_1d<double, 6>>(5, 6);
            result[i][j] = DenseMatrix<array_1d<double, 6>>(5, 6);
            for (unsigned int k = 0; k < 30; ++k) {
                test[i][j].data()[k] = array_1d<double, 6>(6);
                result[i][j].data()[k] = array_1d<double ,6>(6, -1);
                for (unsigned int l = 0; l < 6; ++l) {
                    test[i][j].data()[k][l] = (i+1) * (j+1) * (k+1) * (l+1);
                    ref_values[i * 720 + j * 180 + k * 6 + l] = test[i][j].data()[k][l];
                }
            }
        }
    }

    KRATOS_CHECK_EQUAL(type_trait::Size(test), 2160);

    std::vector<unsigned int> shape{3, 4, 5, 6, 6};
    KRATOS_CHECK_EQUAL(type_trait::Shape(test), shape);
    KRATOS_CHECK_IS_FALSE(type_trait::Reshape(test, shape));

    auto temp = test;
    shape[0] = 4;
    shape[1] = 5;
    shape[2] = 6;
    shape[3] = 7;
    shape[4] = 6;
    KRATOS_CHECK(type_trait::Reshape(temp, shape));
    KRATOS_CHECK_EQUAL(temp.size(), 4);
    for (unsigned int i = 0; i < 4; ++i) {
        KRATOS_CHECK_EQUAL(temp[i].size(), 5);
        for (unsigned int j = 0; j < 5; ++j) {
            KRATOS_CHECK_EQUAL(temp[i][j].size1(), 6);
            KRATOS_CHECK_EQUAL(temp[i][j].size2(), 7);
            for (unsigned int k = 0; k < 42; ++k) {
                KRATOS_CHECK_EQUAL(temp[i][j].data()[k].size(), 6);
            }
        }
    }

    std::vector<double> values(2160, 0);
    type_trait::FillToContiguousData(values.data(), test);
    KRATOS_CHECK_EQUAL(values, ref_values);

    type_trait::FillFromContiguousData(result, values.data());
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 4; ++j) {
            for (unsigned int k = 0; k < 30; ++k) {
                for (unsigned int l = 0; l < 6; ++l) {
                    KRATOS_CHECK_EQUAL(result[i][j].data()[k][l], test[i][j].data()[k][l]);
                }
            }
        }
    }
}

} // namespace Kratos::Testing