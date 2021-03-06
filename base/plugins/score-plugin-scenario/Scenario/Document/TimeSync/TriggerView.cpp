// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "TriggerView.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include <wobjectimpl.h>
W_OBJECT_IMPL(Scenario::TriggerView)
namespace Scenario
{
TriggerView::TriggerView(QGraphicsItem* parent)
    : QGraphicsSvgItem{":/images/trigger.svg", parent}
{
  this->setCacheMode(QGraphicsItem::NoCache);
  this->setScale(1.5);
  this->setAcceptDrops(true);
  setFlag(ItemStacksBehindParent, true);
}

void TriggerView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->button() == Qt::MouseButton::LeftButton)
    pressed(event->scenePos());
}

void TriggerView::dropEvent(QGraphicsSceneDragDropEvent* event)
{
  dropReceived(event->scenePos(), *event->mimeData());
}
}
