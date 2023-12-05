#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
//#include <string>
#include <QDebug>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->tempLcdNumber->display("-------");
    ui->humLcdNumber->display("-------");
    ui->distLcdNumber->display("-------");
    ESP = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";
    temperature_value = "";
    humidity_value = "";
    distance_value = "";

    /*
     *  Testing code, prints the description, vendor id, and product id of all ports.
     *  Used it to determine the values for the arduino uno.
     *
     *
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
         qDebug() << "Product Name: " << serialPortInfo.portName() << "\n";
    }
    //*/


    /*
     *   Identify the port the arduino uno is on.
     */
    bool ESP_is_available = false;
    QString ESP_uno_port_name;
    //
    //  For each available serial port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        //  check if the serialport has both a product identifier and a vendor identifier
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            //  check if the product ID and the vendor ID match those of the arduino uno
            if((serialPortInfo.productIdentifier() == ESP_uno_product_id)
                && (serialPortInfo.vendorIdentifier() == ESP_uno_vendor_id)){
                ESP_is_available = true; //    arduino uno is available on this port
                ESP_uno_port_name = serialPortInfo.portName();
            }
        }
    }

    /*
     *  Open and configure the arduino port if available
     */
    if(ESP_is_available){
        qDebug() << "Found the ESP port...\n";
        ESP->setPortName(ESP_uno_port_name);
        ESP->open(QSerialPort::ReadOnly);
        ESP->setBaudRate(QSerialPort::Baud115200);
        ESP->setDataBits(QSerialPort::Data8);
        ESP->setFlowControl(QSerialPort::NoFlowControl);
        ESP->setParity(QSerialPort::NoParity);
        ESP->setStopBits(QSerialPort::OneStop);
        QObject::connect(ESP, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the ESP.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to ESP.");
    }
}

Dialog::~Dialog()
{
    if(ESP->isOpen()){
        ESP->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void Dialog::readSerial()
{
    /*
     * readyRead() doesn't guarantee that the entire message will be received all at once.
     * The message can arrive split into parts.  Need to buffer the serial data and then parse for the temperature value.


    QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator

    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    if(buffer_split.length() < 3){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = ESP->readAll();
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber
        serialBuffer = "";
        qDebug() << buffer_split << "\n";
        parsed_data = buffer_split[1];
        //temperature_value = (9/5.0) * (parsed_data.toDouble()) + 32; // convert to fahrenheit
        qDebug() << "Temperature: " << parsed_data << "\n";
        //parsed_data = QString::number(parsed_data.toDouble(), 'g', 4); // format precision of temperature_value to 4 digits or fewer
        Dialog::updateTemperature(parsed_data);
    }*/

    serialData = ESP->readAll();
    serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
    QStringList buffer_split = serialBuffer.split(",");
    serialData.clear();
    qDebug() << serialBuffer << "\n";
    temperature_value = buffer_split[0];
    humidity_value = buffer_split[1];
    distance_value = buffer_split[2];
    Dialog::updateTemperature(temperature_value, humidity_value, distance_value);
    serialBuffer = "";
}

void Dialog::updateTemperature(QString temperature_reading, QString humidity_reading, QString distance_reading)
{
    //  update the value displayed on the lcdNumber
    ui->tempLcdNumber->display(temperature_reading);
    ui->humLcdNumber->display(humidity_reading);
    ui->distLcdNumber->display(distance_reading);
}
