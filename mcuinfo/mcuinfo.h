#ifndef MCUINFO_H
#define MCUINFO_H

#include <QQuickItem>
#include <QSerialPort>

class McuInfo : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(McuInfo)
    Q_PROPERTY(QString portName WRITE setPortName)
    Q_PROPERTY(int portBaudRate WRITE setPortBaudRate)
    Q_PROPERTY(int battery READ battery NOTIFY batteryChanged)
    Q_PROPERTY(int button READ button NOTIFY buttonChanged)
    Q_PROPERTY(int charge READ charge NOTIFY chargeChanged)
    Q_PROPERTY(bool status READ button NOTIFY statusChanged)

public:
    McuInfo(QQuickItem *parent = nullptr);
    ~McuInfo();

    Q_INVOKABLE void start();
    Q_INVOKABLE void confirmShutdown();
    Q_INVOKABLE void cancelShutdown();

    int battery() const {return m_data;}
    int button() const { return m_data; }
    int charge() const { return m_data; }
    bool status() const {return m_status;}

    void setPortName(QString portname)
    {
        m_portName = portname;
    }

    void setPortBaudRate(int baudRate)
    {
        m_portBaudRate = baudRate;
    }

signals:
    void batteryChanged();
    void buttonChanged();
    void chargeChanged();
    void statusChanged();

private:
    QString m_portName;
    int m_portBaudRate;
    bool m_status;
    QSerialPort m_serialPort;
    QByteArray m_readData;
    QByteArray m_payload;

    int pos=0;
    int checksum=0;
    int m_cmd=0;
    // int m_data=0;
    long m_data=0;
    int data_bytes=0;
    int payloadLength=0;

    void handleReadyRead();
    void ExecuteCommand();
    void handleError(QSerialPort::SerialPortError serialPortError);

};

#endif // MCUINFO_H
