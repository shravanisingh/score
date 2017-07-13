// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Scenario/Process/Algorithms/StandardCreationPolicy.hpp>
#include <Scenario/Process/Algorithms/VerticalMovePolicy.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <iscore/tools/RandomNameProvider.hpp>

#include <QDataStream>
#include <QtGlobal>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/multi_index/detail/hash_index_iterator.hpp>
#include <iscore/tools/IdentifierGeneration.hpp>
#include <vector>

#include "SplitEvent.hpp"
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/State/StateModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <iscore/model/ModelMetadata.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/model/EntityMap.hpp>
#include <iscore/model/path/Path.hpp>
#include <iscore/model/path/PathSerialization.hpp>

namespace Scenario
{
namespace Command
{
SplitEvent::SplitEvent(
    const Scenario::ProcessModel& scenario,
    Id<EventModel>
        event,
    QVector<Id<StateModel>>
        movingstates)
    : m_scenarioPath{scenario}
    , m_originalEvent{std::move(event)}
    , m_newEvent{getStrongId(scenario.events)}
    , m_createdName{RandomNameProvider::generateRandomName()}
    , m_movingStates{std::move(movingstates)}
{
}

void SplitEvent::undo(const iscore::DocumentContext& ctx) const
{
  auto& scenar = m_scenarioPath.find(ctx);
  auto& originalEvent = scenar.events.at(m_originalEvent);

  for (auto& st : m_movingStates)
  {
    originalEvent.addState(st);
    scenar.states.at(st).setEventId(m_originalEvent);
  }

  ScenarioCreate<EventModel>::undo(m_newEvent, m_scenarioPath.find(ctx));

  updateEventExtent(m_originalEvent, scenar);
}

void SplitEvent::redo(const iscore::DocumentContext& ctx) const
{
  auto& scenar = m_scenarioPath.find(ctx);
  auto& originalEvent = scenar.event(m_originalEvent);
  ScenarioCreate<EventModel>::redo(
      m_newEvent,
      scenar.timeNodes.at(originalEvent.timeNode()),
      originalEvent.extent(),
      scenar);

  auto& newEvent = scenar.events.at(m_newEvent);
  newEvent.metadata().setName(m_createdName);

  for (auto& st : m_movingStates)
  {
    originalEvent.removeState(st);
    newEvent.addState(st);
    scenar.states.at(st).setEventId(m_newEvent);
  }

  updateEventExtent(m_newEvent, scenar);
  updateEventExtent(m_originalEvent, scenar);
}

void SplitEvent::serializeImpl(DataStreamInput& s) const
{
  s << m_scenarioPath << m_originalEvent << m_newEvent << m_createdName
    << m_movingStates;
}

void SplitEvent::deserializeImpl(DataStreamOutput& s)
{
  s >> m_scenarioPath >> m_originalEvent >> m_newEvent >> m_createdName
      >> m_movingStates;
}
}
}
