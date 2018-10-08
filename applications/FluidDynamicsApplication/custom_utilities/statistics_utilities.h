//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Jordi Cotela
//                   Author2 Fullname
//

#ifndef KRATOS_STATISTICS_UTILITIES_H_INCLUDED
#define KRATOS_STATISTICS_UTILITIES_H_INCLUDED

// System includes
#include <string>
#include <iostream>
#include <functional>

// External includes

// Project includes
#include "includes/define.h"
#include "includes/node.h"
#include "geometries/geometry.h"
#include "includes/ublas_interface.h"

namespace Kratos
{
///@addtogroup FluidDynamicsApplication
///@{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

class StatisticsSampler
{
public:

StatisticsSampler(unsigned int NumValues, unsigned int Offset = 0):
    mNumValues(NumValues),
    mOffset(Offset)
{}

KRATOS_CLASS_POINTER_DEFINITION(StatisticsSampler);

virtual ~StatisticsSampler() {}

virtual void SampleDataPoint(const Geometry< Node<3> >& rGeometry, const Vector& rShapeFunctions, const Matrix& rShapeDerivatives, std::vector<double>::iterator& BufferIterator)
{}

unsigned int GetSize() const {
    return mNumValues;
}

virtual unsigned int GetValueOffset() const
{
    KRATOS_ERROR << "Method not implemented" << std::endl;
}

virtual unsigned int GetComponentOffset(unsigned int i) const
{
    KRATOS_ERROR << "Method not implemented" << std::endl;
}

virtual unsigned int GetComponentOffset(unsigned int i, unsigned int j) const
{
    KRATOS_ERROR << "Method not implemented" << std::endl;
}

unsigned int GetOffset() const
{
    return mOffset;
}

void SetOffset(std::size_t Offset) {
    mOffset = Offset;
}

template<class TIteratorType>
void Finalize(TIteratorType& rBufferIterator, std::size_t NumberOfMeasurements)
{
    for (std::size_t i = 0; i < mNumValues; i++) {
        *rBufferIterator /= NumberOfMeasurements;
        ++rBufferIterator;
    }
}


private:

unsigned int mNumValues;

unsigned int mOffset;

friend class Serializer;

void save(Serializer& rSerializer) const {}

void load(Serializer& rSerializer) {}

StatisticsSampler():
    mNumValues(0),
    mOffset(0)
{}

};

class ScalarAverageSampler: public StatisticsSampler
{
public:

ScalarAverageSampler(std::function<double(const Geometry< Node<3> >&, const Vector&, const Matrix&)> Getter):
  StatisticsSampler(1),
  mGetter(Getter)
{}

~ScalarAverageSampler() override {}

void SampleDataPoint(const Geometry< Node<3> >& rGeometry, const Vector& rShapeFunctions, const Matrix& rShapeDerivatives, std::vector<double>::iterator& BufferIterator) override
{
    *BufferIterator = mGetter(rGeometry,rShapeFunctions,rShapeDerivatives);
    ++BufferIterator;
}

unsigned int GetValueOffset() const override {
    return this->GetOffset();
}

private:

std::function<double(const Geometry< Node<3> >& rGeometry, const Vector& rShapeFunctions, const Matrix& rShapeDerivatives)> mGetter;

};

template< class VectorType >
class VectorAverageSampler: public StatisticsSampler
{
public:

VectorAverageSampler(std::function<VectorType(const Geometry< Node<3> >&, const Vector&, const Matrix&)> Getter, unsigned int VectorSize):
    StatisticsSampler(VectorSize),
    mGetter(Getter)
{}

~VectorAverageSampler() override {}

void SampleDataPoint(const Geometry< Node<3> >& rGeometry, const Vector& rShapeFunctions, const Matrix& rShapeDerivatives, std::vector<double>::iterator& BufferIterator) override {
    VectorType result = mGetter(rGeometry,rShapeFunctions,rShapeDerivatives);
    for (unsigned int i = 0; i < this->GetSize(); i++) {
        *BufferIterator = result[i];
        ++BufferIterator;
    }
}

private:

std::function<VectorType(const Geometry< Node<3> >& rGeometry, const Vector& rShapeFunctions, const Matrix& rShapeDerivatives)> mGetter;
};

class VarianceSampler : public StatisticsSampler
{
public:

VarianceSampler(const StatisticsSampler::Pointer pQuantity1, const StatisticsSampler::Pointer pQuantity2):
    StatisticsSampler(pQuantity1->GetSize() * pQuantity2->GetSize()),
    mpQuantity1(pQuantity1),
    mpQuantity2(pQuantity2)
{}

//virtual void SampleDataPoint(std::vector<double>::iterator& BufferIterator, ) override

private:

const StatisticsSampler::Pointer mpQuantity1;

const StatisticsSampler::Pointer mpQuantity2;

};

///@}

///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

/// input stream function
inline std::istream &operator>>(std::istream &rIStream,
                                StatisticsSampler &rThis)
{
    return rIStream;
}

/// output stream function
inline std::ostream &operator<<(std::ostream &rOStream,
                                const StatisticsSampler &rThis)
{
    return rOStream;
}

///@}

///@} addtogroup block

} // namespace Kratos.

#endif // KRATOS_STATISTICS_UTILITIES_H_INCLUDED  defined
