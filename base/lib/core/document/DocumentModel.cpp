#include <core/document/DocumentModel.hpp>
#include <iscore/plugins/documentdelegate/DocumentDelegateFactoryInterface.hpp>
#include <iscore/plugins/documentdelegate/DocumentDelegateModelInterface.hpp>
#include <iscore/plugins/documentdelegate/plugin/DocumentDelegatePluginModel.hpp>
#include <iscore/plugins/panel/PanelModel.hpp>
#include <iscore/tools/SettableIdentifierGeneration.hpp>

using namespace iscore;

DocumentModel::DocumentModel(
        DocumentDelegateFactoryInterface* fact,
        QObject* parent) :
    IdentifiedObject {Id<DocumentModel>(iscore::id_generator::getFirstId()), "DocumentModel", parent},
    m_model{fact->makeModel(this)}
{
}

void DocumentModel::addPanel(PanelModel *m)
{
    m_panelModels.append(m);
}




void DocumentModel::addPluginModel(DocumentDelegatePluginModel *m)
{
    m->setParent(this);
    m_pluginModels.append(m);
    emit pluginModelsChanged();
}



void DocumentModel::setNewSelection(const Selection& s)
{
    m_model->setNewSelection(s);
}

QString DocumentModel::docFileName() const
{
    return m_docFileName;
}

void DocumentModel::setDocFileName(const QString &docFileName)
{
    m_docFileName = docFileName;
}
