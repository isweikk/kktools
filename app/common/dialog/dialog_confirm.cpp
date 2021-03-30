#include "dialog_confirm.h"
#include "ui_dialog_confirm.h"

DialogConfirm::DialogConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfirm)
{
    ui->setupUi(this);
    ui->buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Abort |
                                      QDialogButtonBox::StandardButton::Close);
}

DialogConfirm::~DialogConfirm()
{
    delete ui;
}
