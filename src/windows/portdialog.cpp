/*  FanMan: A GUI for FanBoy
 *
 *  Copyright (C) 2020  Alexander Koch
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "portdialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>


PortDialog::PortDialog(Port *port, QWidget* parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    port(port)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    gridLayout = new QGridLayout();

    gridLayout->addWidget(new QLabel("Name:"), 0, 0);
    nameEdit = new QLineEdit(port->name());
    gridLayout->addWidget(nameEdit, 0, 1);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(gridLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply |
                                                       QDialogButtonBox::Close,
                                                       Qt::Horizontal);
    vLayout->addWidget(buttonBox);

    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            this, &PortDialog::apply);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(vLayout);
}

void PortDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_Enter || key == Qt::Key_Return)
        return;

    QDialog::keyPressEvent(event);
}

void PortDialog::apply()
{
    port->setName(nameEdit->text());
}

void PortDialog::accept()
{
    apply();
    QDialog::accept();
}
