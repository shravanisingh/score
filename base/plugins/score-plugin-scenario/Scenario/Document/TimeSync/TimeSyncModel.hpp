#pragma once
#include <Process/TimeValue.hpp>
#include <Scenario/Document/Event/ExecutionStatus.hpp>
#include <Scenario/Document/VerticalExtent.hpp>
#include <State/Expression.hpp>

#include <score/model/Component.hpp>
#include <score/model/EntityImpl.hpp>
#include <score/model/IdentifiedObject.hpp>
#include <score/model/Identifier.hpp>
#include <score/selection/Selectable.hpp>
#include <score/tools/Metadata.hpp>
#include <score/tools/std/Optional.hpp>

#include <QObject>
#include <QString>
#include <QVector>

#include <score_plugin_scenario_export.h>
#include <wobjectdefs.h>

#include <chrono>
class DataStream;
class JSONObject;

namespace Scenario
{
class EventModel;
class ScenarioInterface;

class SCORE_PLUGIN_SCENARIO_EXPORT TimeSyncModel final
    : public score::Entity<TimeSyncModel>
{
  W_OBJECT(TimeSyncModel)

  SCORE_SERIALIZE_FRIENDS

public:
  /** Properties of the class **/
  Selectable selection;

  /** The class **/
  TimeSyncModel(
      const Id<TimeSyncModel>& id, const VerticalExtent& extent,
      const TimeVal& date, QObject* parent);

  template <typename DeserializerVisitor>
  TimeSyncModel(DeserializerVisitor&& vis, QObject* parent)
      : Entity{vis, parent}
  {
    vis.writeTo(*this);
  }

  // Data of the TimeSync
  const VerticalExtent& extent() const;
  void setExtent(const VerticalExtent& extent);

  const TimeVal& date() const;
  void setDate(const TimeVal&);

  void addEvent(const Id<EventModel>&);
  bool removeEvent(const Id<EventModel>&);
  void clearEvents();

  using EventIdVec = ossia::small_vector<Id<EventModel>, 2>;
  const EventIdVec& events() const;
  void setEvents(const TimeSyncModel::EventIdVec& events);

  State::Expression expression() const
  {
    return m_expression;
  }
  void setExpression(const State::Expression& expression);

  bool active() const;
  void setActive(bool active);

public:
  void extentChanged(const Scenario::VerticalExtent& arg_1)
      E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, extentChanged, arg_1);
  void dateChanged(const TimeVal& arg_1)
      E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, dateChanged, arg_1);

  void newEvent(const Id<Scenario::EventModel>& eventId)
      E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, newEvent, eventId);
  void eventRemoved(const Id<Scenario::EventModel>& eventId)
      E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, eventRemoved, eventId);

  void triggerChanged(const State::Expression& arg_1)
      E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, triggerChanged, arg_1);
  void activeChanged() E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, activeChanged);

  void triggeredByGui() const
      E_SIGNAL(SCORE_PLUGIN_SCENARIO_EXPORT, triggeredByGui);

private:
  TimeVal m_date{std::chrono::seconds{0}};
  State::Expression m_expression;

  EventIdVec m_events;
  VerticalExtent m_extent;

  bool m_active{false};
};
}

DEFAULT_MODEL_METADATA(Scenario::TimeSyncModel, "Sync")
TR_TEXT_METADATA(, Scenario::TimeSyncModel, PrettyName_k, "Sync")

Q_DECLARE_METATYPE(Id<Scenario::TimeSyncModel>)
W_REGISTER_ARGTYPE(Id<Scenario::TimeSyncModel>)
