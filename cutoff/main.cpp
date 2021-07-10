#include <QSerialPort>
#include <QCoreApplication>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QSerialPort m_serialPort;
    QByteArray m_payload;

    m_serialPort.setPortName("/dev/ttyS0");
    m_serialPort.setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
    m_serialPort.open(QIODevice::ReadWrite);
    
    m_payload.resize(7);

    m_payload[0] = 0x5A;
    m_payload[1] = 0xA5;
    m_payload[2] = 5;
    m_payload[3] = 1;
    m_payload[4] = 0;
    m_payload[5] = 0xF7;

    // CRC
    m_payload[6] = 0;
    for (int i = 0; i < 6; i++)
        m_payload[6] = (m_payload[6] + m_payload[i]);

    // write to UART 
    m_serialPort.write(m_payload);
    m_serialPort.flush();

    return app.exec();
}
