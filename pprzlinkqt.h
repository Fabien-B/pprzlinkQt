#ifndef PPRZLINKQT_H
#define PPRZLINKQT_H

#include <QObject>

class PprzlinkQt : public QObject
{
    Q_OBJECT
public:
    explicit PprzlinkQt(QString filename, QObject *parent = nullptr);

signals:

};

#endif // PPRZLINKQT_H
