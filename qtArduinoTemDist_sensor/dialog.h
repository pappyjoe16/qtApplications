#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void readSerial();
    void updateTemperature(QString temperature_reading, QString humidity_reading, QString distance_reading);

private:
    Ui::Dialog *ui;

    QSerialPort *ESP;
    static const quint16 ESP_uno_vendor_id = 6790;
    static const quint16 ESP_uno_product_id = 29987;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    QString temperature_value;
    QString humidity_value;
    QString distance_value;

};

#endif // DIALOG_H
