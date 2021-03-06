// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "IntervalInspectorWidget.hpp"

#include <Inspector/InspectorLayout.hpp>
#include <Scenario/Application/ScenarioApplicationPlugin.hpp>
#include <Scenario/Document/Interval/IntervalModel.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentPresenter.hpp>
#include <Scenario/Inspector/Interval/SpeedSlider.hpp>
#include <Scenario/Inspector/Interval/Widgets/DurationSectionWidget.hpp>
#include <Scenario/Inspector/MetadataWidget.hpp>
#include <Scenario/Inspector/SelectionButton.hpp>
#include <Scenario/Process/ScenarioInterface.hpp>

#include <score/document/DocumentContext.hpp>
#include <score/widgets/MarginLess.hpp>
#include <score/widgets/Separator.hpp>
#include <score/widgets/TextLabel.hpp>

namespace Scenario
{
IntervalInspectorWidget::IntervalInspectorWidget(
    const Inspector::InspectorWidgetList& widg, const IntervalModel& object,
    const score::DocumentContext& ctx, QWidget* parent)
    : InspectorWidgetBase{object, ctx, parent,
                          tr("Interval (%1)").arg(object.metadata().getName())}
    , m_model{object}
{
  using namespace score;
  using namespace score::IDocument;
  setObjectName("Interval");

  std::vector<QWidget*> parts;
  ////// HEADER
  // metadata
  auto meta = new MetadataWidget{m_model.metadata(), ctx.commandStack,
                                 &m_model, this};

  meta->setupConnections(m_model);
  addHeader(meta);

  ////// BODY
  // Separator
  parts.push_back(new score::HSeparator{this});
  auto w = new QWidget;
  auto lay = new Inspector::Layout{w};
  parts.push_back(w);

  // Full View
  auto fullview = new QPushButton{tr("Full view")};
  connect(fullview, &QPushButton::clicked, this, [this] {
    auto base = get<ScenarioDocumentPresenter>(*documentFromObject(m_model));

    if (base)
      base->setDisplayedInterval(model());
  });

  lay->addRow(fullview);

  // Speed
  auto speedWidg = new SpeedWidget{m_model, ctx, true, this};
  lay->addRow(tr("Speed"), speedWidg);

  // Durations
  auto& ctrl = ctx.app.guiApplicationPlugin<ScenarioApplicationPlugin>();
  auto dur = new DurationWidget{ctrl.editionSettings(), *lay, this};
  lay->addWidget(dur);

  // Display data
  updateAreaLayout(parts);
}

IntervalInspectorWidget::~IntervalInspectorWidget() = default;

IntervalModel& IntervalInspectorWidget::model() const
{
  return const_cast<IntervalModel&>(m_model);
}
}
