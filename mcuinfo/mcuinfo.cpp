#include "mcuinfo.h"

McuInfo::McuInfo(QQuickItem *parent):
    QQuickItem(parent)
{
}

McuInfo::~McuInfo()
{
}

void McuInfo::start()
{
    qDebug()<<"opening serial port"<<m_portName<<m_portBaudRate;
    m_serialPort.setPortName(m_portName);
    m_serialPort.setBaudRate(m_portBaudRate, QSerialPort::AllDirections);
    if(m_serialPort.open(QIODevice::ReadWrite))
    {
        connect(&m_serialPort, &QSerialPort::readyRead, this, &McuInfo::handleReadyRead);
        connect(&m_serialPort, &QSerialPort::errorOccurred, this, &McuInfo::handleError);
        m_status = true;
    }
    else
    {
        m_status = false;
    }

    emit statusChanged();
}

void McuInfo::getVersion() 
{
    m_payload.resize(7);

    // header 
    m_payload[0] = 0x5A;
    m_payload[1] = 0xA5;

    // msg type
    m_payload[2] = 1;

    // payload length 
    m_payload[3] = 1;
    m_payload[4] = 0;

    // payload: get version 
    m_payload[5] = 0xF6;

    // CRC
    m_payload[6] = 0; 
    for (int i = 0; i < 6; i++)
        m_payload[6] = (m_payload[6] + m_payload[i]);

    // write to UART 
    m_serialPort.write(m_payload);
    m_serialPort.flush();
}

void McuInfo::confirmShutdown() 
{
    m_payload.resize(7);

    // header
    m_payload[0] = 0x5A;
    m_payload[1] = 0xA5;

    // msg type
    m_payload[2] = 1;

    // payload length 
    m_payload[3] = 1;
    m_payload[4] = 0;

    // payload: confirm shutdown 
    m_payload[5] = 0xF3;

    // CRC
    m_payload[6] = 0; 
    for (int i = 0; i < 6; i++)
        m_payload[6] = (m_payload[6] + m_payload[i]);

    // write to UART 
    m_serialPort.write(m_payload);
    m_serialPort.flush();
}

void McuInfo::cancelShutdown() 
{
    m_payload.resize(7);

    // header
    m_payload[0] = 0x5A;
    m_payload[1] = 0xA5;

    // msg type 
    m_payload[2] = 1;

    // payload length 
    m_payload[3] = 1;
    m_payload[4] = 0;

    // payload: cancel shutdown 
    m_payload[5] = 0xFC;

    // CRC
    m_payload[6] = 0;
    for (int i = 0; i < 6; i++)
        m_payload[6] = (m_payload[6] + m_payload[i]);

    // write to UART 
    m_serialPort.write(m_payload);
    m_serialPort.flush();
}

void McuInfo::handleReadyRead()
{
    m_readData = m_serialPort.readAll();

    for(int i=0; i<m_readData.length(); i++){
        bool ok;
        QByteArray e = m_readData.mid(i,1).toHex();
        int c=e.toInt(&ok, 16);

        switch (pos)
        {
        case 0:	// header 1 (0x5A)
            if(c == 90){
                checksum += c;
                pos++;
            }
            else{
                checksum=0;
                pos=0;
            }
            break;

        case 1:	//header 2 (0xA5)
            if(c == 165){
                checksum += c;
                pos++;
            }
            else{
                checksum=0;
                pos=0;
            }
            break;

        case 2: // msg type 
            m_cmd = c;
            checksum += c;
            pos++;
            break;

        case 3: // length of msg payload
            payloadLength = c;
            checksum += c;
            pos++;
            break;

        case 4: // length of msg payload
            payloadLength += 256 * c;
            m_data = 0;
            data_bytes = 0;
            checksum += c;
            pos++;
            break;

        case 5:
            checksum += c;
            if (m_cmd==4) 
                m_version.append(static_cast<char>(c));
            else
                m_data += (c << data_bytes*8);
            data_bytes++;

            if(data_bytes >= payloadLength){
                // full data is received regardless number of bytes
                pos++;
            }

            break;

        case 6: // checksum
            if(c == (255 & checksum)){// correct uart msg received
                ExecuteCommand();
            }
            else{
                printf("checksum wrong!\n");
            }
            pos = 0;
            checksum = 0;
            payloadLength = 0;
            m_version = "";
            break;
        }
    }

}

void McuInfo::ExecuteCommand()
{
    switch (m_cmd)
    {
    case 1: // button
        emit buttonChanged();
        if(m_data == 1){
            m_payload.resize(7);
            m_payload[0] = 0x5A;
            m_payload[1] = 0xA5;
            m_payload[2] = 1;
            m_payload[3] = 1;
            m_payload[4] = 0;
            m_payload[5] = 0xF1;
            m_payload[6] = 0;
            for (int i = 0; i < 6; i++)
                m_payload[6] = (m_payload[6] + m_payload[i]);
            m_serialPort.write(m_payload);
            m_serialPort.flush();
        }
        break;

    case 2: // battery
        emit batteryChanged();
        break;

    case 3: // charging
        emit chargeChanged();
        m_payload.resize(7);
        m_payload[0] = 0x5A;
        m_payload[1] = 0xA5;
        m_payload[2] = 1;
        m_payload[3] = 1;
        m_payload[4] = 0;
        if (m_data == 4)
            m_payload[5] = 0xF4;
        else
            m_payload[5] = 0xF5;
        m_payload[6] = 0;
        for (int i = 0; i < 6; i++)
            m_payload[6] = (m_payload[6] + m_payload[i]);
        m_serialPort.write(m_payload);
        m_serialPort.flush();
        break;
    case 4: // version
        emit versionChanged();
        break;
    }
}

void McuInfo::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError)
    {
        qDebug() << QObject::tr("reading error, port:%1, error:%2").arg(m_serialPort.portName()).arg(m_serialPort.errorString());
    }
}
