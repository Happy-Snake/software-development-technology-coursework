#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>

class CustomDialog : public QDialog
{
public:
    CustomDialog(const QStringList& items)
    {
        setLayout(new QHBoxLayout());

        box = new QComboBox;
        box->addItem("");
        box->addItems(items);
        layout()->addWidget(box);

        QPushButton* ok = new QPushButton("ok");
        layout()->addWidget(ok);
        connect(ok, &QPushButton::clicked, this, [this]()
        {
           accept();
        });
    }

    QComboBox* comboBox() { return box; }

private:
    QComboBox* box;
};

#endif // CUSTOMDIALOG_H
