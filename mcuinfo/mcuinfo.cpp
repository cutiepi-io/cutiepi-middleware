#include "mcuinfo.h"

McuInfo::McuInfo(QQuickItem *parent):
    QQuickItem(parent)
{
    // By default, QQuickItem does not draw anything. If you subclass
    // QQuickItem to create a visual item, you will need to uncomment the
    // following line and re-implement updatePaintNode()

    // setFlag(ItemHasContents, true);
}

McuInfo::~McuInfo()
{
}

void McuInfo::start()
{
    qDebug()<<"opening serial port"<<m_portName<<m_portBaudRate;
    m_serialPort.setPortName(m_portName);
    m_serialPort.setBaudRate(m_portBaudRate);
    if(m_serialPort.open(QIODevice::ReadOnly))
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

        case 2: // cmd
            m_cmd = c;
            checksum += c;
            pos++;
            break;

        case 3: //data_msb
            m_data = 256*c;
            checksum += c;
            pos++;
            break;

        case 4: //data_lsb
            m_data += c;
            checksum += c;
            pos++;
            break;

        case 5: // checksum
            if(c == (255 & checksum)){// correct uart msg received
                ExecuteCommand();
            }
            else{
                printf("checksum wrong!\n");
            }
            pos = 0;
            checksum = 0;
            break;

        }
    }

}

void McuInfo::ExecuteCommand()
{
    switch (m_cmd)
    {
    case 1: //button
        emit buttonChanged();
        break;

    case 2: //battery
        emit batteryChanged();
        break;

    case 3: //charging
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
