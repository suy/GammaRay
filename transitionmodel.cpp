
#include "transitionmodel.h"

#include <QtCore/QStringList>
#include <QtCore/QState>
#include <QtCore/QAbstractTransition>
#include <QtCore/QDebug>
#include "util.h"
#include <QtCore/QSignalTransition>

// #include <modeltest.h>

using namespace Endoscope;

namespace Endoscope
{

class TransitionModelPrivate
{
  TransitionModelPrivate(TransitionModel *qq)
    : q_ptr(qq), m_state(0)
  {

  }

  Q_DECLARE_PUBLIC(TransitionModel)
  TransitionModel * const q_ptr;
  QState * m_state;

  QList<QObject*> children( QObject* parent ) const;

  QObject* mapModelIndex2QObject( const QModelIndex& ) const;
};

}

QList<QObject*> TransitionModelPrivate::children( QObject* parent ) const
{
    QList<QObject*> result;
    if (parent == 0)
      parent = m_state;

    foreach(QObject *o, parent->children())
      if (o->inherits("QAbstractTransition"))
        result.append(o);

    qSort( result );
    return result;
}

QObject* TransitionModelPrivate::mapModelIndex2QObject( const QModelIndex& index ) const
{
    if ( index.isValid() ) {
        QObjectList c = children( reinterpret_cast<QObject*>( index.internalPointer() ) );
        return c[index.row()];
    }
    return m_state;
}

TransitionModel::TransitionModel(QObject* parent)
  : ObjectModelBase<QAbstractItemModel>(parent), d_ptr(new TransitionModelPrivate(this))
{
  QHash<int, QByteArray> _roleNames = roleNames();
  // TODO
  setRoleNames(_roleNames);
}

void TransitionModel::setState(QState* state)
{
  Q_D(TransitionModel);
  beginResetModel();
  qDebug() << state;
  d->m_state = state;
  endResetModel();
}

QVariant TransitionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (section < 2)
    return ObjectModelBase<QAbstractItemModel>::headerData(section, orientation, role);
  if (section == 2)
    return "Signal";
  if (section == 3)
    return "Target";
  return QVariant();
}

QVariant TransitionModel::data( const QModelIndex& index, int role ) const
{
    Q_D(const TransitionModel);
    if ( !index.isValid() || !d->m_state )
        return QVariant();

    QObject* obj = d->mapModelIndex2QObject(index);

    if (index.column() == 2 && role == Qt::DisplayRole) {
      QSignalTransition *sigTransition = qobject_cast<QSignalTransition*>(obj);
      if (sigTransition)
        return sigTransition->signal();
    } else if (index.column() == 3 && role == Qt::DisplayRole) {
      QAbstractTransition *transition = qobject_cast<QAbstractTransition*>(obj);
      if (transition)
        return Util::displayString(transition->targetState());
    }


    if ( obj )
      return dataForObject( obj, index, role );
    return QVariant();
}

int TransitionModel::columnCount( const QModelIndex& parent ) const
{
  return ObjectModelBase<QAbstractItemModel>::columnCount(parent) + 2;
}

int TransitionModel::rowCount( const QModelIndex& parent ) const
{
    Q_D(const TransitionModel);
    if (!d->m_state)
      return 0;
    return d->children(d->mapModelIndex2QObject(parent)).count();
}

QModelIndex TransitionModel::index( int row, int column, const QModelIndex & parent ) const
{
    Q_D(const TransitionModel);

    if (parent.isValid() || !d->m_state)
      return QModelIndex();

    if (row < 0 || column < 0 || column > 3)
      return QModelIndex();

    QObject *internalPointer = reinterpret_cast<QObject*>(parent.internalPointer());
    if ( !parent.isValid() ) {
          internalPointer = d->m_state;
    } else {
          QObject *o = reinterpret_cast<QObject*>( parent.internalPointer() ) ;
          QObjectList c = d->children( o );
          internalPointer = c.at(parent.row());
    }

    QObjectList c = d->children(internalPointer);
    if (row >= c.size())
      return QModelIndex();

    return createIndex( row, column, internalPointer );
}

QModelIndex TransitionModel::parent( const QModelIndex & index ) const
{
    Q_D(const TransitionModel);
    return QModelIndex();
}