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

#pragma once

// System includes

// External includes

// Project includes
#include "spatial_containers/spatial_search.h"

namespace Kratos
{
///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

/**
 * @brief This enum defines the different spatial containers available
 * @details The different spatial containers available are:
 * - KDTree: This is a tree based on the k-d tree algorithm
 * - Octree: This is a tree based on the octree algorithm
 * - BinsStatic: This is a bin based on the static bins algorithm
 * - BinsDynamic: This is a bin based on the dynamic bins algorithm
 * - BinsStaticObjects: This is a bin based on the static bins algorithm, but with objects
 * - BinsDynamicObjects: This is a bin based on the dynamic bins algorithm, but with objects
 */
enum class SpatialContainer
{
    KDTree,
    Octree,
    BinsStatic,
    BinsDynamic,
    BinsStaticObjects,
    BinsDynamicObjects
};

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/**
* @class SpecializedSpatialSearch
* @ingroup KratosCore
* @brief This class is used to search for elements, conditions and nodes in a given model part
* @details In order to perform the search it uses as backend some of the the spatial containers defined `spatial_containers` folder
* @tparam TSearhcBackend The spatial container to be used as backend
* @author Vicente Mataix Ferrandiz
*/
template<SpatialContainer TSearhcBackend>
class KRATOS_API(KRATOS_CORE) SpecializedSpatialSearch
    : public SpatialSearch
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of SpecializedSpatialSearch
    KRATOS_CLASS_POINTER_DEFINITION(SpecializedSpatialSearch);

    /// The base type
    using BaseType = SpatialSearch;

    /// Common Defines
    using PointType = BaseType::PointType;

    using ElementsContainerType = BaseType::ElementsContainerType;
    using ElementType = BaseType::ElementType;
    using ElementPointerType = BaseType::ElementPointerType;
    using ResultElementsContainerType = BaseType::ResultElementsContainerType;
    using VectorResultElementsContainerType = BaseType::VectorResultElementsContainerType;

    using NodesContainerType = BaseType::NodesContainerType;
    using NodeType = BaseType::NodeType;
    using NodePointerType = BaseType::NodePointerType;
    using ResultNodesContainerType = BaseType::ResultNodesContainerType;
    using VectorResultNodesContainerType = BaseType::VectorResultNodesContainerType;

    using ConditionsContainerType = BaseType::ConditionsContainerType;
    using ConditionType = BaseType::ConditionType;
    using ConditionPointerType = BaseType::ConditionPointerType;
    using ResultConditionsContainerType = BaseType::ResultConditionsContainerType;
    using VectorResultConditionsContainerType = BaseType::VectorResultConditionsContainerType;

    using RadiusArrayType = BaseType::RadiusArrayType;
    using DistanceType = BaseType::DistanceType;
    using VectorDistanceType = BaseType::VectorDistanceType;

    using ResultIteratorType = BaseType::ResultIteratorType;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    SpecializedSpatialSearch()
    {
    }

    /// Destructor.
    virtual ~SpecializedSpatialSearch(){}

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    /**
    * @brief Search neighbours for every element in "rInputElements" excluding itself
    * @param rStructureElements   List of elements against which the neighbours are searched
    * @param rInputElements       List of elements to be searched
    * @param rRadius              List of search radius for every element
    * @param rResults             Array of results for each element
    * @param rResultDistance      Array of distances for each result of each element
    */
    void SearchElementsInRadiusExclusive(
        const ElementsContainerType& rStructureElements,
        const ElementsContainerType& rInputElements,
        const RadiusArrayType& rRadius,
        VectorResultElementsContainerType& rResults,
        VectorDistanceType& rResultsDistance
        ) override;

    /**
    * @brief Search neighbours for every element in "rInputElements" including itself
    * @param rStructureElements   List of elements against which the neighbours are searched
    * @param rInputElements       List of elements to be searched
    * @param rRadius              List of search radius for every element
    * @param rResults             Array of results for each element
    * @param rResultDistance      Array of distances for each result of each element
    */
    void SearchElementsInRadiusInclusive(
        const ElementsContainerType& rStructureElements,
        const ElementsContainerType& rInputElements,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults,
        VectorDistanceType& rResultsDistance
        ) override;

    /**
    * @brief Search neighbours for every element in "rInputElements" excluding itself
    * @param rStructureElements   List of elements against which the neighbours are searched
    * @param rInputElements       List of elements to be searched
    * @param rRadius              List of search radius for every element
    * @param rResults             Array of results for each element
    */
    void SearchElementsInRadiusExclusive(
        const ElementsContainerType& rStructureElements,
        const ElementsContainerType& rInputElements,
        const RadiusArrayType& rRadius,
        VectorResultElementsContainerType& rResults
        ) override;

    /**
    * @brief Search neighbours for every element in "InputElements" including itself
    * @param StructureElements   List of elements against which the neighbours are searched
    * @param InputElements       List of elements to be searched
    * @param rRadius              List of search radius for every element
    * @param rResults            Array of results for each element
    */
    void SearchElementsInRadiusInclusive(
        const ElementsContainerType& rStructureElements,
        const ElementsContainerType& rInputElements,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults
        ) override;

    /**
    * @brief Search neighbours for every node in "rInputNodes" excluding itself
    * @param rModelPart          List of nodes against which the neighbours are searched
    * @param rInputNodes          List of nodes to be searched
    * @param rRadius             List of search radius for every node
    * @param rResults            Array of results for each node
    * @param rResultDistance     Array of distances for each result of each node
    */
    void SearchNodesInRadiusExclusive(
        const NodesContainerType& rStructureNodes,
        const NodesContainerType& rInputNodes,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults,
        VectorDistanceType& rResultsDistance
        ) override;

    /**
    * @brief Search neighbours for every node in "rInputNodes" including itself
    * @param rStructureNodes      List of nodes against which the neighbours are searched
    * @param rInputNodes          List of nodes to be searched
    * @param rRadius              List of search radius for every node
    * @param rResults            Array of results for each node
    * @param rResultDistance     Array of distances for each result of each node
    */
    void SearchNodesInRadiusInclusive(
        const NodesContainerType& rStructureNodes,
        const NodesContainerType& rInputNodes,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults,
        VectorDistanceType& rResultsDistance
        ) override;

    /**
    * @brief Search neighbours for every node in "rInputNodes" excluding itself
    * @param rModelPart          List of nodes against which the neighbours are searched
    * @param rInputNodes          List of nodes to be searched
    * @param rRadius              List of search radius for every node
    * @param rResults            Array of results for each node
    */
    void SearchNodesInRadiusExclusive(
        const NodesContainerType& rStructureNodes,
        const NodesContainerType& rInputNodes,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults
        ) override;

    /**
    * @brief Search neighbours for every node in "rInputNodes" including itself
    * @param rStructureNodes      List of nodes against which the neighbours are searched
    * @param rInputNodes          List of nodes to be searched
    * @param rRadius              List of search radius for every node
    * @param rResults             Array of results for each node
    */
    void SearchNodesInRadiusInclusive(
        const NodesContainerType& rStructureNodes,
        const NodesContainerType& rInputNodes,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults
        ) override;

    /**
    * @brief Search neighbours for every Condition in "rInputConditions" excluding itself
    * @param rStructureConditions   List of Conditions against which the neighbours are searched
    * @param rInputConditions       List of Conditions to be searched
    * @param rRadius                List of search radius for every Condition
    * @param rResults               Array of results for each Condition
    * @param rResultDistance        Array of distances for each result of each Condition
    */
    void SearchConditionsInRadiusExclusive(
        const ConditionsContainerType& rStructureConditions,
        const ConditionsContainerType& rInputConditions,
        const RadiusArrayType& rRadius,
        VectorResultConditionsContainerType& rResults,
        VectorDistanceType& rResultsDistance
        ) override;

    /**
    * @brief Search neighbours for every Condition in "rInputConditions" including itself
    * @param rStructureConditions   List of Conditions against which the neighbours are searched
    * @param rInputConditions       List of Conditions to be searched
    * @param rRadius                List of search radius for every Condition
    * @param rResults               Array of results for each Condition
    * @param rResultDistance        Array of distances for each result of each Condition
    */
    void SearchConditionsInRadiusInclusive(
        const ConditionsContainerType& rStructureConditions,
        const ConditionsContainerType& rInputConditions,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults,
        VectorDistanceType& rResultsDistance
        ) override;

    /**
    * @brief Search neighbours for every Condition in "rInputConditions" excluding itself
    * @param rStructureConditions   List of Conditions against which the neighbours are searched
    * @param rInputConditions       List of Conditions to be searched
    * @param rRadius                List of search radius for every Condition
    * @param rResults               Array of results for each Condition
    */
    void SearchConditionsInRadiusExclusive(
        const ConditionsContainerType& rStructureConditions,
        const ConditionsContainerType& rInputConditions,
        const RadiusArrayType& rRadius,
        VectorResultConditionsContainerType& rResults
        ) override;

    /**
    * @brief Search neighbours for every Condition in "rInputConditions" including itself
    * @param rStructureConditions   List of Conditions against which the neighbours are searched
    * @param rInputConditions       List of Conditions to be searched
    * @param rRadius                List of search radius for every Condition
    * @param rResults               Array of results for each Condition
    */
    void SearchConditionsInRadiusInclusive(
        const ConditionsContainerType& rStructureConditions,
        const ConditionsContainerType& rInputConditions,
        const RadiusArrayType& rRadius,
        VectorResultNodesContainerType& rResults
        ) override;

    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "SpecializedSpatialSearch" ;

        return buffer.str();
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "SpecializedSpatialSearch";
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {

    }

    ///@}
    ///@name Friends
    ///@{

    ///@}
protected:
    ///@name Protected static Member Variables
    ///@{

    ///@}
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operators
    ///@{

    ///@}
    ///@name Protected Operations
    ///@{

    ///@}
    ///@name Protected  Access
    ///@{

    ///@}
    ///@name Protected Inquiry
    ///@{

    ///@}
    ///@name Protected LifeCycle
    ///@{

    ///@}
private:
    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    ///@}
    ///@name Private  Access
    ///@{

    ///@}
    ///@name Private Inquiry
    ///@{

    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    SpecializedSpatialSearch& operator=(SpecializedSpatialSearch const& rOther)
    {
        return *this;
    }

    /// Copy constructor.
    SpecializedSpatialSearch(SpecializedSpatialSearch const& rOther)
    {
        *this = rOther;
    }

    ///@}
}; // Class SpecializedSpatialSearch

///@}
///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

/// output stream function
template<SpatialContainer TSearhcBackend>
inline std::ostream& operator << (std::ostream& rOStream, 
                const SpecializedSpatialSearch<TSearhcBackend>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@}addtogroup block

}  // namespace Kratos.