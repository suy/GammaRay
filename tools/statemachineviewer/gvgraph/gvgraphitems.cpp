/*
  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Kevin Funk <kevin.funk@kdab.com>

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

#include "gvgraphitems.h"

#include <QPen>

using namespace GammaRay;

GVNodeItem::GVNodeItem(const GVNode& node, QGraphicsItem* parent, QGraphicsScene* scene)
  : QGraphicsEllipseItem(parent, scene)
  , m_node(node)
  , m_textItem(new QGraphicsTextItem(node.name(), this))
{
  const QSizeF size = node.size();
  const QRectF rect(-size.width()/2, -size.height()/2, size.width(), size.height());
  setRect(rect);
  setPos(m_node.centerPos());
  setToolTip(QObject::tr("State: %1").arg(node.name()));
  setFlags(ItemIsSelectable);

  // init text item child
  {
    QGraphicsTextItem* item = m_textItem;
    QRectF textRect = item->boundingRect();
    while(size.width() < textRect.size().width()) {
      QFont font = item->font();
      font.setPointSize(font.pointSize() - 1);
      item->setFont(font);
      textRect = item->boundingRect();
    }
    const QSizeF size = textRect.size();
    item->setPos(QPointF(-size.width()/2, -size.height()/2));
  }
}

GVEdgeItem::GVEdgeItem(const GVEdge& edge, QGraphicsItem* parent, QGraphicsScene* scene)
  : QGraphicsPathItem(parent, scene)
  , m_edge(edge)
{
  setPath(edge.m_path);
  setToolTip(QObject::tr("Transition: %1 -> %2").arg(edge.m_source).arg(edge.m_target));

  // arrow head quick-fix
  QPainterPath path(QPointF(0, 0));
  path.addRect(-2, -2, 4, 4);
  QGraphicsPathItem* pathItem = new QGraphicsPathItem(this);
  pathItem->setPath(path);
  pathItem->setPos(edge.m_path.pointAtPercent(1.0));
}

GVGraphItem::GVGraphItem(const GVSubGraph& graph, QGraphicsItem* parent, QGraphicsScene* scene)
  : QGraphicsPathItem(parent, scene)
  , m_graph(graph)
  , m_textItem(0)
{
  setZValue(-1);

  setPath(graph.path());
  setPen(QColor(Qt::gray));
  setBrush(QColor(100, 100, 100, 20));
  setToolTip(QObject::tr("Graph: %1").arg(graph.name()));

  {
    // TODO: Implement text item
  }
}