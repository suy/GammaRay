#include "clientconnectionmanager.h"

#include "client.h"
#include "clienttoolmodel.h"

#include <common/network/objectbroker.h>

#include <ui/mainwindow.h>
#include <ui/splashscreen.h>

#include <QApplication>
#include <QMessageBox>

using namespace GammaRay;

ClientConnectionManager::ClientConnectionManager(QObject* parent) :
  QObject(parent),
  m_client(new Client(this)),
  m_mainWindow(0),
  m_toolModel(0)
{
  showSplashScreen();

  connect(m_client, SIGNAL(disconnected()), QApplication::instance(), SLOT(quit()));
  connect(m_client, SIGNAL(connectionEstablished()), SLOT(connectionEstablished()));
  connect(m_client, SIGNAL(connectionError(QString)), SLOT(connectionError(QString)));
}

ClientConnectionManager::~ClientConnectionManager()
{
}

void ClientConnectionManager::connectToHost(const QString& hostname, quint16 port)
{
    m_client->connectToHost(hostname, port);
}

void ClientConnectionManager::connectionEstablished()
{
  m_toolModel = new ClientToolModel(this);
  ObjectBroker::registerModel("com.kdab.GammaRay.ToolModel", m_toolModel);

  if (m_toolModel->rowCount() <= 0) {
    connect(m_toolModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(toolModelPopulated()));
    connect(m_toolModel, SIGNAL(layoutChanged()), SLOT(toolModelPopulated()));
    connect(m_toolModel, SIGNAL(modelReset()), SLOT(toolModelPopulated()));
  } else {
    toolModelPopulated();
  }
}

void ClientConnectionManager::toolModelPopulated()
{
  if (m_toolModel->rowCount() <= 0)
    return;

  disconnect(m_toolModel, 0, this, 0);

  m_mainWindow = new MainWindow;
  m_mainWindow->show();
  hideSplashScreen();
}

void ClientConnectionManager::connectionError(const QString& msg)
{
  hideSplashScreen();

  QString errorMsg;
  if (m_mainWindow)
    errorMsg = tr("Lost connection to remote host: %1").arg(msg);
  else
    errorMsg = tr("Could not establish connection to remote host: %1").arg(msg);

  QMessageBox::critical(m_mainWindow, tr("GammaRay - Connection Error"), errorMsg);
  QApplication::exit(1);
}

#include "clientconnectionmanager.moc"
