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

#pragma once

// System includes
#include <string>
#include <vector>

// Project includes
#include "includes/define.h"
#include "containers/container_expression/expressions/expression.h"
#include "xml_ostream_writer.h"

namespace Kratos {

///@name Kratos Classes
///@{

class KRATOS_API(KRATOS_CORE) XmlElement {
public:
    ///@name Type definitions
    ///@{

    using IndexType = std::size_t;

    KRATOS_CLASS_POINTER_DEFINITION(XmlElement);

    ///@}
    ///@name Life cycle
    ///@{

    /**
     * @brief Constructor.
     * @param rTagName The tag name of the XML element.
     */
    XmlElement(const std::string& rTagName);

    /** 
     * @brief Constructor.
     * @param rDataName The name of the data element.
     * @param rExpressions The expressions to write as data.
     */
    XmlElement(
        const std::string& rDataName,
        const std::vector<Expression::Pointer>& rExpressions);

    ///@}
    ///@name Public operations
    ///@{

    /** 
     * @brief Get the tag name of the XML element.
     * @return The tag name.
     */
    const std::string GetTagName() const;

    /** 
     * @brief Add an attribute to the XML element.
     * @param rName The name of the attribute.
     * @param rValue The value of the attribute.
     */
    void AddAttribute(
        const std::string& rName,
        const std::string& rValue);

    /** 
     * @brief Get the attributes of the XML element.
     * @return The attributes.
     */
    const std::vector<std::pair<const std::string, const std::string>>& GetAttributes() const;

    /** 
     * @brief Clear the attributes of the XML element.
     */
    void ClearAttributes();

    /** 
     * @brief Add a sub-element to the XML element.
     * @param pXmlElement The sub-element to add.
     */
    void AddElement(const XmlElement::Pointer pXmlElement);

    /** 
     * @brief Get sub-elements with a specific tag name.
     * @param rTagName The tag name of the sub-elements.
     * @return The vector of sub-elements.
     */
    std::vector<XmlElement::Pointer> GetElements(const std::string& rTagName) const;

    /** 
     * @brief Get all sub-elements of the XML element.
     * @return The vector of sub-elements.
     */
    const std::vector<XmlElement::Pointer>& GetElements() const;

    /** 
     * @brief Clear all sub-elements of the XML element.
     */
    void ClearElements();

    /** 
     * @brief Write the XML element to the XML output stream writer.
     * @param rWriter The XML output stream writer.
     * @param Level The indentation level.
     */
    void Write(
        XmlOStreamWriter& rWriter,
        const IndexType Level = 0) const;

    ///@}

private:
    ///@name Private member variables
    ///@{

    const std::string mTagName; /// The tag name of the XML element.

    std::vector<std::pair<const std::string, const std::string>> mAttributes; /// The attributes of the XML element.

    std::vector<XmlElement::Pointer> mXmlElements; /// The sub-elements of the XML element. 

    const std::vector<Expression::Pointer> mExpressions; /// The expressions to write as data.

    ///@}
};

///@}

} // namespace Kratos
