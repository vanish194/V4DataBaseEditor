#include "tooldescription.h"

// Constructor
ToolDescription::ToolDescription(int id,
                                 const QString &description,
                                 int lengthMm,
                                 int outerDiameterMm,
                                 int innerDiameterMm,
                                 const QByteArray &image,
                                 int producerId)
    : m_id(id)
    , m_description(description)
    , m_lengthMm(lengthMm)
    , m_outerDiameterMm(outerDiameterMm)
    , m_innerDiameterMm(innerDiameterMm)
    , m_image(image)
    , m_producerId(producerId)
{}

// Comparison operator
bool ToolDescription::operator==(const ToolDescription &other) const
{
    return m_id == other.m_id && m_description == other.m_description
           && m_lengthMm == other.m_lengthMm && m_outerDiameterMm == other.m_outerDiameterMm
           && m_innerDiameterMm == other.m_innerDiameterMm && m_image == other.m_image
           && m_producerId == other.m_producerId;
}

// Getters
int ToolDescription::getId() const
{
    return m_id;
}

QString ToolDescription::getDescription() const
{
    return m_description;
}

int ToolDescription::getLengthMm() const
{
    return m_lengthMm;
}

int ToolDescription::getOuterDiameterMm() const
{
    return m_outerDiameterMm;
}

int ToolDescription::getInnerDiameterMm() const
{
    return m_innerDiameterMm;
}

QByteArray ToolDescription::getImage() const
{
    return m_image;
}

int ToolDescription::getProducerId() const
{
    return m_producerId;
}

// Setters
void ToolDescription::setId(int id)
{
    m_id = id;
}

void ToolDescription::setDescription(const QString &description)
{
    m_description = description;
}

void ToolDescription::setLengthMm(int lengthMm)
{
    m_lengthMm = lengthMm;
}

void ToolDescription::setOuterDiameterMm(int outerDiameterMm)
{
    m_outerDiameterMm = outerDiameterMm;
}

void ToolDescription::setInnerDiameterMm(int innerDiameterMm)
{
    m_innerDiameterMm = innerDiameterMm;
}

void ToolDescription::setImage(const QByteArray &image)
{
    m_image = image;
}

void ToolDescription::setProducerId(int producerId)
{
    m_producerId = producerId;
}

void ToolDescription::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
