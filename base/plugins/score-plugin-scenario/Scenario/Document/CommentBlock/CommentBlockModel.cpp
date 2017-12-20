// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "CommentBlockModel.hpp"

#include <QTextDocument>

namespace Scenario
{
CommentBlockModel::CommentBlockModel(
    const Id<CommentBlockModel>& id,
    const TimeVal& date,
    double yPos,
    QObject* parent)
    : IdentifiedObject<CommentBlockModel>{id, "CommentBlockModel", parent}
    , m_date{date}
    , m_yposition{yPos}
{
}

void CommentBlockModel::setDate(const TimeVal& date)
{
  if (date != m_date)
  {
    m_date = date;
    emit dateChanged(m_date);
  }
}

const TimeVal& CommentBlockModel::date() const
{
  return m_date;
}

double CommentBlockModel::heightPercentage() const
{
  return m_yposition;
}

void CommentBlockModel::setHeightPercentage(double y)
{
  if (y != m_yposition)
  {
    m_yposition = y;
    emit heightPercentageChanged(y);
  }
}

const QString CommentBlockModel::content() const
{
  return m_HTMLcontent;
}

void CommentBlockModel::setContent(const QString content)
{
  if (m_HTMLcontent == content)
    return;
  m_HTMLcontent = content;
  emit contentChanged(m_HTMLcontent);
}
}
