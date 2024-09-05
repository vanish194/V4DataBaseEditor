#ifndef TOOLDESCRIPTION_H
#define TOOLDESCRIPTION_H

#include <QByteArray> // For handling image blobs
#include <QString>

class ToolDescription
{
public:
    // Constructor
    ToolDescription(int id = 0,
                    const QString &description = "",
                    int lengthMm = 0,
                    int outerDiameterMm = 0,
                    int innerDiameterMm = 0,
                    const QByteArray &image = QByteArray(),
                    int producerId = 0);

    // Comparison operator
    bool operator==(const ToolDescription &other) const;

    // Getters
    int getId() const;
    QString getDescription() const;
    int getLengthMm() const;
    int getOuterDiameterMm() const;
    int getInnerDiameterMm() const;
    QByteArray getImage() const;
    int getProducerId() const;

    // Setters
    void setId(int id);
    void setDescription(const QString &description);
    void setLengthMm(int lengthMm);
    void setOuterDiameterMm(int outerDiameterMm);
    void setInnerDiameterMm(int innerDiameterMm);
    void setImage(const QByteArray &image);
    void setProducerId(int producerId);

    void markAsDeleted();

private:
    int m_id;
    QString m_description;
    int m_lengthMm;
    int m_outerDiameterMm;
    int m_innerDiameterMm;
    QByteArray m_image;
    int m_producerId;
};

#endif // TOOLDESCRIPTION_H
