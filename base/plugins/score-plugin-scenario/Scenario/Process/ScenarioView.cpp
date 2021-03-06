// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ScenarioView.hpp"

#include <Process/LayerView.hpp>
#include <Process/ProcessMimeSerialization.hpp>
#include <Scenario/Application/Menus/ScenarioCopy.hpp>

#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QEvent>
#include <QFlags>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QKeyEvent>
#include <QMimeData>
#include <QPainter>
#include <QPen>
#include <qnamespace.h>

#include <wobjectimpl.h>

namespace Scenario
{
ScenarioView::ScenarioView(
    const ProcessModel& m, QGraphicsItem* parent)
    : LayerView{parent}, m_scenario{m}
{
  this->setFlags(
      ItemIsSelectable | ItemIsFocusable | ItemClipsChildrenToShape);
  setAcceptDrops(true);

  this->setZValue(1);
}

ScenarioView::~ScenarioView() = default;

void ScenarioView::paint_impl(QPainter* painter) const
{
  painter->setRenderHint(QPainter::Antialiasing, false);
  if (m_lock)
  {
    painter->setBrush({Qt::red, Qt::DiagCrossPattern});
    painter->drawRect(boundingRect());
  }

  if (m_selectArea != QRectF{})
  {
    painter->setCompositionMode(QPainter::CompositionMode_Xor);
    painter->setPen(QPen{QColor{0, 0, 0, 127}, 2, Qt::DashLine, Qt::SquareCap,
                         Qt::BevelJoin});
    painter->setBrush(Qt::transparent);
    painter->drawRect(m_selectArea);
    painter->setCompositionMode(
        QPainter::CompositionMode::CompositionMode_SourceOver);
  }

  if (m_dragLine)
  {
    painter->setRenderHint(QPainter::Antialiasing, true);
    const QRectF& rec = *m_dragLine;
    painter->setPen(QPen{Qt::gray, 2, Qt::DashLine});
    painter->drawLine(rec.topLeft(), rec.bottomLeft());
    painter->drawLine(rec.bottomLeft(), rec.bottomRight());
    painter->drawEllipse(rec.bottomRight(), 3., 3.);
    painter->setRenderHint(QPainter::Antialiasing, false);
  }
}

void ScenarioView::drawDragLine(QPointF left, QPointF right)
{
  m_dragLine = QRectF(left, right);
  update();
}

void ScenarioView::stopDrawDragLine()
{
  m_dragLine = ossia::none;
  update();
}

void ScenarioView::movedAsked(const QPointF& p)
{
  QRectF r{m_previousPoint.x(), m_previousPoint.y(), 1, 1};
  ensureVisible(mapRectFromScene(r), 30, 30);
  moved(p);

  // we use the last pos, because if not there's a larsen and crash
  m_previousPoint = p;
}

void ScenarioView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  m_moving = false;
  if (event->button() == Qt::LeftButton)
    pressed(event->scenePos());

  event->accept();
}

void ScenarioView::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->buttons() & Qt::MiddleButton)
  {
    auto obj = copySelectedScenarioElements(m_scenario);
    if (!obj.empty())
    {
      QDrag d{this};
      auto m = new QMimeData;
      QJsonDocument doc{obj};
      ;
      m->setData(
          score::mime::scenariodata(), doc.toJson(QJsonDocument::Indented));
      d.setMimeData(m);
      d.exec();
    }
  }
  else
  {
    if (m_moving
        || (event->buttonDownScreenPos(Qt::LeftButton) - event->screenPos())
                   .manhattanLength()
               > QApplication::startDragDistance())
    {
      m_moving = true;
      moved(event->scenePos());
    }
  }
  event->accept();
}

void ScenarioView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  m_moving = false;
  released(event->scenePos());

  event->accept();
}

void ScenarioView::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent* event)
{
  doubleClicked(event->pos());

  event->accept();
}

void ScenarioView::contextMenuEvent(
    QGraphicsSceneContextMenuEvent* event)
{
  askContextMenu(event->screenPos(), event->scenePos());

  event->accept();
}

void ScenarioView::keyPressEvent(QKeyEvent* event)
{
  QGraphicsItem::keyPressEvent(event);
  if (event->key() == Qt::Key_Escape)
  {
    escPressed();
  }
  else if (
      event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control
      || event->key() == Qt::Key_Alt)
  {
    keyPressed(event->key());
  }

  event->accept();
}

void ScenarioView::keyReleaseEvent(QKeyEvent* event)
{
  QGraphicsItem::keyReleaseEvent(event);
  if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control
      || event->key() == Qt::Key_Alt)
  {
    keyReleased(event->key());
  }

  event->accept();
}

void ScenarioView::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
  dragEnter(event->pos(), *event->mimeData());

  event->accept();
}

void ScenarioView::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
  dragMove(event->pos(), *event->mimeData());

  event->accept();
}

void ScenarioView::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
  dragLeave(event->pos(), *event->mimeData());

  event->accept();
}

void ScenarioView::dropEvent(QGraphicsSceneDragDropEvent* event)
{
  dropReceived(event->pos(), *event->mimeData());

  event->accept();
}
}
