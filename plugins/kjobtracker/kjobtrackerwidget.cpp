/*
  kjobtrackerwidget.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2012-2013 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "kjobtrackerwidget.h"
#include "ui_kjobtrackerwidget.h"

#include <common/network/objectbroker.h>

using namespace GammaRay;

#include <QDebug>
#include <QtPlugin>
#include <QSortFilterProxyModel>

using namespace GammaRay;

KJobTrackerWidget::KJobTrackerWidget(QWidget *parent)
  : QWidget(parent), ui(new Ui::KJobTrackerWidget)
{
  ui->setupUi(this);

  QSortFilterProxyModel *filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(ObjectBroker::model("com.kdab.GammaRay.KJobModel"));
  ui->searchLine->setProxy(filter);
  ui->jobView->setModel(filter);
}

KJobTrackerWidget::~KJobTrackerWidget()
{
}

#include "kjobtrackerwidget.moc"
