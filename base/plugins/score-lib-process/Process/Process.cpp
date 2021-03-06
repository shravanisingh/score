// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Process.hpp"

#include <Process/Dataflow/Port.hpp>
#include <Process/ExpandMode.hpp>
#include <Process/TimeValue.hpp>

#include <score/model/IdentifiedObject.hpp>
#include <score/model/Identifier.hpp>
#include <score/model/ModelMetadata.hpp>

#include <ossia/detail/algorithms.hpp>

#include <QObject>

#include <wobjectimpl.h>

#include <algorithm>
#include <stdexcept>
W_OBJECT_IMPL(Process::ProcessModel)
template class IdentifiedObject<Process::ProcessModel>;
template class score::SerializableInterface<Process::ProcessModelFactory>;
namespace Process
{
ProcessModel::ProcessModel(
    TimeVal duration, const Id<ProcessModel>& id, const QString& name,
    QObject* parent)
    : Entity{id, name, parent}
    , m_duration{std::move(duration)}
    , m_slotHeight{300}
{
  con(metadata(), &score::ModelMetadata::NameChanged, this,
      [=] { prettyNameChanged(); });
  // metadata().setInstanceName(*this);
}

ProcessModel::~ProcessModel()
{
  identified_object_destroying(this);
}

void ProcessModel::setDurationAndScale(const TimeVal& newDuration) noexcept
{
  setDuration(newDuration);
}

void ProcessModel::setDurationAndGrow(const TimeVal& newDuration) noexcept
{
  setDuration(newDuration);
}

void ProcessModel::setDurationAndShrink(const TimeVal& newDuration) noexcept
{
  setDuration(newDuration);
}

ProcessModel::ProcessModel(DataStream::Deserializer& vis, QObject* parent)
    : Entity(vis, parent)
{
  vis.writeTo(*this);
  con(metadata(), &score::ModelMetadata::NameChanged, this,
      [=] { prettyNameChanged(); });
}

ProcessModel::ProcessModel(JSONObject::Deserializer& vis, QObject* parent)
    : Entity(vis, parent)
{
  vis.writeTo(*this);
  con(metadata(), &score::ModelMetadata::NameChanged, this,
      [=] { prettyNameChanged(); });
}

QString ProcessModel::prettyName() const noexcept
{
  return metadata().getName();
}

void ProcessModel::setParentDuration(ExpandMode mode, const TimeVal& t) noexcept
{
  switch (mode)
  {
    case ExpandMode::Scale:
      setDurationAndScale(t);
      break;
    case ExpandMode::GrowShrink:
    {
      if (duration() < t)
        setDurationAndGrow(t);
      else
        setDurationAndShrink(t);
      break;
    }
    case ExpandMode::ForceGrow:
    {
      if (duration() < t)
        setDurationAndGrow(t);
      break;
    }
    case ExpandMode::CannotExpand:
    default:
      break;
  }
}

bool ProcessModel::contentHasDuration() const noexcept
{
  return false;
}

TimeVal ProcessModel::contentDuration() const noexcept
{
  return TimeVal::zero();
}

void ProcessModel::setDuration(const TimeVal& other) noexcept
{
  m_duration = other;
  durationChanged(m_duration);
}

const TimeVal& ProcessModel::duration() const noexcept
{
  return m_duration;
}

void ProcessModel::startExecution()
{
}

void ProcessModel::stopExecution()
{
}

void ProcessModel::reset()
{
}

ProcessStateDataInterface* ProcessModel::startStateData() const noexcept
{
  return nullptr;
}

ProcessStateDataInterface* ProcessModel::endStateData() const noexcept
{
  return nullptr;
}

Selection ProcessModel::selectableChildren() const noexcept
{
  return {};
}

Selection ProcessModel::selectedChildren() const noexcept
{
  return {};
}

void ProcessModel::setSelection(const Selection& s) const noexcept
{
}

Process::Inlet* ProcessModel::inlet(const Id<Process::Port>& p) const noexcept
{
  for (auto e : m_inlets)
    if (e->id() == p)
      return e;
  return nullptr;
}

Process::Outlet* ProcessModel::outlet(const Id<Process::Port>& p) const noexcept
{
  for (auto e : m_outlets)
    if (e->id() == p)
      return e;
  return nullptr;
}

double ProcessModel::getSlotHeight() const noexcept
{
  return m_slotHeight;
}

void ProcessModel::setSlotHeight(double v) noexcept
{
  m_slotHeight = v;
  slotHeightChanged(v);
}

ProcessModel* parentProcess(QObject* obj) noexcept
{
  while (obj && !qobject_cast<ProcessModel*>(obj))
  {
    obj = obj->parent();
  }

  if (obj)
    return static_cast<ProcessModel*>(obj);
  return nullptr;
}

const ProcessModel* parentProcess(const QObject* obj) noexcept
{
  while (obj && !qobject_cast<const ProcessModel*>(obj))
  {
    obj = obj->parent();
  }

  if (obj)
    return static_cast<const ProcessModel*>(obj);
  return nullptr;
}
}
