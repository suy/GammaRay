/*
  resourcebrowserwidget.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2013 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Stephen Kelly <stephen.kelly@kdab.com>

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

#include "resourcebrowserwidget.h"
#include "ui_resourcebrowserwidget.h"

#include <3rdparty/qt/resourcemodel.h>
#include <network/objectbroker.h>

#include <QDebug>
#include <QFileInfo>

using namespace GammaRay;

ResourceBrowserWidget::ResourceBrowserWidget(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::ResourceBrowserWidget)
{
  ui->setupUi(this);
  ui->treeView->setModel(ObjectBroker::model("com.kdab.GammaRay.ResourceModel"));

  // TODO this and most of the following needs to be delayed when being used remotely
  ui->treeView->expandAll();

  // date modifier - not really useful and mostly empty anyways - hide it
  ui->treeView->hideColumn(3);

  ui->treeView->header()->setResizeMode(QHeaderView::ResizeToContents);

  QMetaObject::invokeMethod(this, "setupLayout", Qt::QueuedConnection);

  connect(ui->treeView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          SLOT(resourceSelected(QItemSelection,QItemSelection)));

  ui->resourceLabel->setText(tr("Select a Resource to Preview"));
  ui->stackedWidget->setCurrentWidget(ui->contentLabelPage);
}

ResourceBrowserWidget::~ResourceBrowserWidget()
{
}

void ResourceBrowserWidget::resourceSelected(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
  Q_UNUSED(deselected)
  // TODO this doesn't work remotely yet
  const QModelIndex selectedRow = selected.first().topLeft();
  const QFileInfo fi(selectedRow.data(ResourceModel::FilePathRole).toString());

  if (fi.isFile()) {
    const QStringList l = QStringList() << "jpg" << "png" << "jpeg";
    if (l.contains(fi.suffix())) {
      ui->resourceLabel->setPixmap(fi.absoluteFilePath());
      ui->stackedWidget->setCurrentWidget(ui->contentLabelPage);
    } else {
      QFile f(fi.absoluteFilePath());
      f.open(QFile::ReadOnly | QFile::Text);
      ui->textBrowser->setText(f.readAll());
      ui->stackedWidget->setCurrentWidget(ui->contentTextPage);
    }
  } else {
    ui->resourceLabel->setText(tr("Select a Resource to Preview"));
    ui->stackedWidget->setCurrentWidget(ui->contentLabelPage);
  }
}

void ResourceBrowserWidget::setupLayout()
{
  // now the view was setup properly and we can mess with the splitter to resize
  // the widgets for nicer display

  int viewWidth = ui->treeView->columnWidth(0) +
                  ui->treeView->columnWidth(1) +
                  ui->treeView->columnWidth(2) +
                  ui->treeView->contentsMargins().left() +
                  ui->treeView->contentsMargins().right() + 25;
  const int totalWidth = ui->splitter_7->width();
  const int minPreviewWidth = 150;
  if (totalWidth > viewWidth + minPreviewWidth) {
    ui->splitter_7->setSizes(QList<int>() << viewWidth << (totalWidth - viewWidth));
    ui->splitter_7->setStretchFactor(1, 3);
  }
}

#include "resourcebrowserwidget.moc"
