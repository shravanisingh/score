#include "DefaultHeaderDelegate.hpp"
#include <Dataflow/Commands/EditConnection.hpp>
#include <Dataflow/Commands/EditPort.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>
#include <score/command/Dispatchers/CommandDispatcher.hpp>
#include <Process/Style/ScenarioStyle.hpp>
#include <Dataflow/UI/CableItem.hpp>
#include <score/tools/IdentifierGeneration.hpp>
#include <Dataflow/UI/PortItem.hpp>
#include <Process/Process.hpp>
#include <Explorer/Widgets/AddressAccessorEditWidget.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <Dataflow/Commands/EditPort.hpp>
#include <QFormLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QDialog>
#include <QDialogButtonBox>
#include <score/widgets/SignalUtils.hpp>
#include <QCheckBox>
namespace Scenario
{
class PortWidget : public QWidget
{
  public:
    PortWidget(const score::DocumentContext& ctx, Process::Port& p)
      : m_disp{ctx.commandStack}
      , m_edit{ctx.plugin<Explorer::DeviceDocumentPlugin>().explorer(), this}
    {
      auto lay = new QFormLayout{this};
      lay->addRow(p.customData(), (QWidget*)nullptr);
      lay->addRow(tr("Address"), &m_edit);
      if(p.outlet && p.type == Process::PortType::Audio)
      {
        auto cb = new QCheckBox{this};
        cb->setChecked(p.propagate());
        lay->addRow(tr("Propagate"), cb);
        connect(cb, &QCheckBox::toggled,
                this, [&] (auto ok) {
          if(ok != p.propagate()) {
            CommandDispatcher<> d{ctx.commandStack};
            d.submitCommand<Dataflow::SetPortPropagate>(p, ok);
          }
        });
        con(p, &Process::Port::propagateChanged,
            this, [=] (bool p) {
          if(p != cb->isChecked()) {
            cb->setChecked(p);
          }
        });
      }

      m_edit.setAddress(p.address());
      con(p, &Process::Port::addressChanged,
          this, [this] (const State::AddressAccessor& addr) {
        if(addr != m_edit.address().address)
        {
          m_edit.setAddress(addr);
        }
      });
      con(m_edit, &Explorer::AddressAccessorEditWidget::addressChanged,
          this, [this,&p] (const Device::FullAddressAccessorSettings& set) {
        if(set.address != p.address())
          m_disp.submitCommand<Dataflow::ChangePortAddress>(p, set.address);
      });
    }

  private:
    CommandDispatcher<> m_disp;
    Explorer::AddressAccessorEditWidget m_edit;
};
class PortDialog final
    : public QDialog
{
  public:
    PortDialog(const score::DocumentContext& ctx, Process::Port& p, QWidget* parent):
      QDialog{parent}
    , m_pw{ctx, p}
    , m_bb{QDialogButtonBox::Ok}
    {
      this->setLayout(&m_lay);
      m_lay.addWidget(&m_pw);
      m_lay.addWidget(&m_bb);
      connect(&m_bb, &QDialogButtonBox::accepted,
              this, [=] {
        close();
      });
    }

  private:
    PortWidget m_pw;
    QDialogButtonBox m_bb;
    QHBoxLayout m_lay;
};
/** TODO
class PortPanel final
    : public QObject
    , public QGraphicsItem
{
    QRectF m_rect;
    QRectF m_widgrect;
    PortWidget* m_pw{};
    QGraphicsProxyWidget* m_proxy{};
  public:
    PortPanel(const score::DocumentContext& ctx, Process::Port& p, QGraphicsItem* parent):
      QGraphicsItem{parent}
    {
      m_pw = new PortWidget{ctx, p};
      m_proxy = new QGraphicsProxyWidget{this};
      m_proxy->setWidget(m_pw);
      m_proxy->setPos(10, 10);
      connect(m_proxy, &QGraphicsProxyWidget::geometryChanged,
              this, &PortPanel::updateRect);
      updateRect();
    }

    void updateRect()
    {
      prepareGeometryChange();

      m_widgrect = m_proxy->subWidgetRect(m_pw);
      m_rect = m_widgrect.adjusted(0, 0, 30, 20);
    }

    QRectF boundingRect() const override
    {
      return m_rect;
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
    {
      painter->setRenderHint(QPainter::Antialiasing, true);
      painter->setPen(QColor("#1A2024"));
      painter->setBrush(QColor("#1A2024"));

      QPainterPath p;
      p.moveTo(12, 12);
      p.lineTo(0, 22);
      p.lineTo(12, 34);
      p.lineTo(12, 12);
      p.closeSubpath();

      painter->drawPath(p);
      painter->fillPath(p, painter->brush());

      painter->drawRoundedRect(m_rect.adjusted(10, 0, 0, 0), 10, 10);

      painter->setRenderHint(QPainter::Antialiasing, false);
    }
};
*/


void DefaultHeaderDelegate::onCreateCable(Dataflow::PortItem* p1, Dataflow::PortItem* p2)
{
  auto& ctx = presenter.context().context;
  auto& plug = ctx.model<Scenario::ScenarioDocumentModel>();
  CommandDispatcher<> disp{ctx.commandStack};
  Process::CableData cd;
  cd.type = Process::CableType::ImmediateStrict;

  if(p1->port().outlet)
  {
    cd.source = p1->port();
    cd.sink = p2->port();
  }
  else
  {
    cd.source = p2->port();
    cd.sink = p1->port();
  }

  disp.submitCommand<Dataflow::CreateCable>(
        plug,
        getStrongId(plug.cables),
        cd);
}

DefaultHeaderDelegate::DefaultHeaderDelegate(Process::LayerPresenter& p)
  : presenter{p}
{
  con(presenter.model(), &Process::ProcessModel::prettyNameChanged,
      this, &DefaultHeaderDelegate::updateName);
  updateName();
  m_textcache.setFont(ScenarioStyle::instance().Medium8Pt);
  m_textcache.setCacheEnabled(true);

  con(p.model(), &Process::ProcessModel::inletsChanged,
      this, [=] { updatePorts(); });
  con(p.model(), &Process::ProcessModel::inletsChanged,
      this, [=] { updatePorts(); });
  updatePorts();
}

void DefaultHeaderDelegate::updateName()
{
  m_textcache.setText(presenter.model().prettyName());
  m_textcache.beginLayout();

  QTextLine line = m_textcache.createLine();
  line.setPosition(QPointF{0., 0.});

  m_textcache.endLayout();

  update();
}

void DefaultHeaderDelegate::setSize(QSizeF sz)
{
  auto old_w = this->boundingRect().width();
  GraphicsShapeItem::setSize(sz);
  const auto pw = minPortWidth();
  if(sz.width() < pw || sz.width() < pw)
  {
    int i = 0;
    for(auto item : m_inPorts)
    {
      item->setPos(0., qreal(i) * sz.height() / qreal(m_inPorts.size()));
      i++;
    }

    i = 0;
    for(auto item : m_outPorts)
    {
      item->setPos(sz.width(), qreal(i) * sz.height() / qreal(m_outPorts.size()));
      i++;
    }
  }
  else if(old_w < pw && sz.width() > pw)
  {
    qreal x = 16;
    for(auto item : m_inPorts)
    {
      item->setPos(x, 15.);
      x += 10.;
    }

    x = 16.;
    for(auto item : m_outPorts)
    {
      item->setPos(x, 24.);
      x += 10.;
    }
  }
}

double DefaultHeaderDelegate::minPortWidth() const
{
  qreal inWidth =  10. * m_inPorts.size();
  qreal outWidth = 10. * m_outPorts.size();
  return std::max(inWidth, outWidth);
}

void DefaultHeaderDelegate::updatePorts()
{
  qDeleteAll(m_inPorts);
  m_inPorts.clear();
  qDeleteAll(m_outPorts);
  m_outPorts.clear();
  qreal x = 16;
  for(auto& port : presenter.model().inlets())
  {
    auto item = new Dataflow::PortItem{*port, this};
    item->setPos(x, 15.);
    connect(item, &Dataflow::PortItem::showPanel,
            this, [&,&pt=*port,item] {
      auto panel = new PortDialog{presenter.context().context, pt, nullptr};
      //scene()->addItem(panel);
      //panel->setPos(item->mapToScene(item->pos()));
      panel->exec();
      panel->deleteLater();
    });
    connect(item, &Dataflow::PortItem::createCable,
            this, &DefaultHeaderDelegate::onCreateCable);
    m_inPorts.push_back(item);
    x += 10.;
  }
  x = 16.;
  for(auto& port : presenter.model().outlets())
  {
    auto item = new Dataflow::PortItem{*port, this};
    item->setPos(x, 24.);
    connect(item, &Dataflow::PortItem::showPanel,
            this, [&,&pt=*port,item] {
      auto panel = new PortDialog{presenter.context().context, pt, nullptr};
      //scene()->addItem(panel);
      //panel->setPos(item->mapToScene(item->pos()));
      panel->exec();
      panel->deleteLater();
    });
    m_outPorts.push_back(item);
    x += 10.;
  }

}

void DefaultHeaderDelegate::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  if(boundingRect().width() > minPortWidth()) {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(ScenarioStyle::instance().IntervalHeaderSeparator);
    m_textcache.draw(painter, QPointF{8.,-3.});

    painter->setPen(ScenarioStyle::instance().TimenodePen);
    painter->setFont(ScenarioStyle::instance().Medium8Pt);
    painter->drawText(QPointF{4, 18}, "→");
    painter->drawText(QPointF{4, 27}, "←");

    painter->setRenderHint(QPainter::Antialiasing, false);
  }
}

}