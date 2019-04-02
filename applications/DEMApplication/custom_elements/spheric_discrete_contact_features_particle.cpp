//
// Author: Joaquín Irazábal jirazabal@cimne.upc.edu
//

// System includes
#include <string>
#include <iostream>
#include <cmath>

#include <fstream>

// External includes

// Project includes
#include "spheric_discrete_contact_features_particle.h"

namespace Kratos
{
// using namespace GeometryFunctions;

SphericDiscreteContactFeaturesParticle::SphericDiscreteContactFeaturesParticle()
    : SphericParticle() {}

SphericDiscreteContactFeaturesParticle::SphericDiscreteContactFeaturesParticle(IndexType NewId, GeometryType::Pointer pGeometry)
    : SphericParticle(NewId, pGeometry) {}

SphericDiscreteContactFeaturesParticle::SphericDiscreteContactFeaturesParticle(IndexType NewId, GeometryType::Pointer pGeometry,  PropertiesType::Pointer pProperties)
    : SphericParticle(NewId, pGeometry, pProperties) {}

SphericDiscreteContactFeaturesParticle::SphericDiscreteContactFeaturesParticle(IndexType NewId, NodesArrayType const& ThisNodes)
    : SphericParticle(NewId, ThisNodes) {}

SphericDiscreteContactFeaturesParticle::SphericDiscreteContactFeaturesParticle(Element::Pointer p_spheric_particle)
{
    GeometryType::Pointer p_geom = p_spheric_particle->pGetGeometry();
    PropertiesType::Pointer pProperties = p_spheric_particle->pGetProperties();
    SphericDiscreteContactFeaturesParticle(p_spheric_particle->Id(), p_geom, pProperties);
}

SphericDiscreteContactFeaturesParticle& SphericDiscreteContactFeaturesParticle::operator=(const SphericDiscreteContactFeaturesParticle& rOther) {

    SphericParticle::operator=(rOther);

    mNeighbourContactRadius = rOther.mNeighbourContactRadius;
    mNeighbourRigidContactRadius = rOther.mNeighbourRigidContactRadius;
    mNeighbourIndentation = rOther.mNeighbourIndentation;
    mNeighbourRigidIndentation = rOther.mNeighbourRigidIndentation;
    mNeighbourContactStress = rOther.mNeighbourContactStress;
    mNeighbourRigidContactStress = rOther.mNeighbourRigidContactStress;

    return *this;
}

void SphericDiscreteContactFeaturesParticle::ComputeNewNeighboursHistoricalData(DenseVector<int>& temp_neighbours_ids,
                                                                                std::vector<array_1d<double, 3> >& temp_neighbour_elastic_contact_forces)
{
    std::vector<array_1d<double, 3> > temp_neighbour_elastic_extra_contact_forces;
    std::vector<double> temp_neighbour_contact_radius;
    std::vector<double> temp_neighbour_indentation;
    std::vector<double> temp_neighbour_contact_stress;
    unsigned int new_size = mNeighbourElements.size();
    array_1d<double, 3> vector_of_zeros = ZeroVector(3);
    temp_neighbours_ids.resize(new_size, false);
    temp_neighbour_elastic_contact_forces.resize(new_size);
    temp_neighbour_elastic_extra_contact_forces.resize(new_size);
    temp_neighbour_contact_radius.resize(new_size);
    temp_neighbour_indentation.resize(new_size);
    temp_neighbour_contact_stress.resize(new_size);

    DenseVector<int>& vector_of_ids_of_neighbours = GetValue(NEIGHBOUR_IDS);

    for (unsigned int i = 0; i < new_size; i++) {
        noalias(temp_neighbour_elastic_contact_forces[i]) = vector_of_zeros;
        noalias(temp_neighbour_elastic_extra_contact_forces[i]) = vector_of_zeros;
        temp_neighbour_contact_radius[i] = 0.0;
        temp_neighbour_indentation[i] = 0.0;
        temp_neighbour_contact_stress[i] = 0.0;

        if (mNeighbourElements[i] == NULL) { // This is required by the continuum sphere which reorders the neighbors
            temp_neighbours_ids[i] = -1;
            continue;
        }

        temp_neighbours_ids[i] = mNeighbourElements[i]->Id();

        for (unsigned int j = 0; j < vector_of_ids_of_neighbours.size(); j++) {
            if (int(temp_neighbours_ids[i]) == vector_of_ids_of_neighbours[j] && vector_of_ids_of_neighbours[j] != -1) {
                noalias(temp_neighbour_elastic_contact_forces[i]) = mNeighbourElasticContactForces[j];
                noalias(temp_neighbour_elastic_extra_contact_forces[i]) = mNeighbourElasticExtraContactForces[j]; //TODO: remove this from discontinuum!!
                temp_neighbour_contact_radius[i] = mNeighbourContactRadius[j];
                temp_neighbour_indentation[i] = mNeighbourIndentation[j];
                temp_neighbour_contact_stress[i] = mNeighbourContactStress[j];
                break;
            }
        }
    }

    vector_of_ids_of_neighbours.swap(temp_neighbours_ids);
    mNeighbourElasticContactForces.swap(temp_neighbour_elastic_contact_forces);
    mNeighbourElasticExtraContactForces.swap(temp_neighbour_elastic_extra_contact_forces);
    mNeighbourContactRadius.swap(temp_neighbour_contact_radius);
    mNeighbourIndentation.swap(temp_neighbour_indentation);
    mNeighbourContactStress.swap(temp_neighbour_contact_stress);
}

void SphericDiscreteContactFeaturesParticle::ComputeNewRigidFaceNeighboursHistoricalData()
{
    array_1d<double, 3> vector_of_zeros = ZeroVector(3);
    std::vector<DEMWall*>& rNeighbours = this->mNeighbourRigidFaces;
    unsigned int new_size              = rNeighbours.size();
    std::vector<int> temp_neighbours_ids(new_size); //these two temporal vectors are very small, saving them as a member of the particle loses time (usually they consist on 1 member).
    std::vector<array_1d<double, 3> > temp_neighbours_elastic_contact_forces(new_size);
    std::vector<array_1d<double, 3> > temp_neighbours_contact_forces(new_size);
    std::vector<double> temp_contact_radius(new_size);
    std::vector<double> temp_indentation(new_size);
    std::vector<double> temp_contact_stress(new_size);

    for (unsigned int i = 0; i<rNeighbours.size(); i++){

        noalias(temp_neighbours_elastic_contact_forces[i]) = vector_of_zeros;
        noalias(temp_neighbours_contact_forces[i]) = vector_of_zeros;
        temp_contact_radius[i] = 0.0;
        temp_indentation[i] = 0.0;
        temp_contact_stress[i] = 0.0;

        if (rNeighbours[i] == NULL) { // This is required by the continuum sphere which reorders the neighbors
            temp_neighbours_ids[i] = -1;
            continue;
        }

        temp_neighbours_ids[i] = static_cast<int>(rNeighbours[i]->Id());

        for (unsigned int j = 0; j != mFemOldNeighbourIds.size(); j++) {
            if (static_cast<int>(temp_neighbours_ids[i]) == mFemOldNeighbourIds[j] && mFemOldNeighbourIds[j] != -1) {
                noalias(temp_neighbours_elastic_contact_forces[i]) = mNeighbourRigidFacesElasticContactForce[j];
                noalias(temp_neighbours_contact_forces[i]) = mNeighbourRigidFacesTotalContactForce[j];
                temp_contact_radius[i] = mNeighbourRigidContactRadius[j];
                temp_indentation[i] = mNeighbourRigidIndentation[j];
                temp_contact_stress[i] = mNeighbourRigidContactStress[j];
                break;
            }
        }
    }

    mFemOldNeighbourIds.swap(temp_neighbours_ids);
    mNeighbourRigidFacesElasticContactForce.swap(temp_neighbours_elastic_contact_forces);
    mNeighbourRigidFacesTotalContactForce.swap(temp_neighbours_contact_forces);
    mNeighbourRigidContactRadius.swap(temp_contact_radius);
    mNeighbourRigidIndentation.swap(temp_indentation);
    mNeighbourRigidContactStress.swap(temp_contact_stress);
}

}  // namespace Kratos.
