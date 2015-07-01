#include "OSSIAControl.hpp"
#include "DocumentPlugin/OSSIADocumentPlugin.hpp"
#include "DocumentPlugin/OSSIABaseScenarioElement.hpp"

#include <API/Headers/Network/Device.h>
#include <API/Headers/Network/Node.h>
#include <API/Headers/Network/Protocol.h>

#include <API/Headers/Editor/TimeEvent.h>
#include <API/Headers/Editor/Expression.h>
#include <API/Headers/Editor/ExpressionNot.h>
#include <API/Headers/Editor/ExpressionAtom.h>
#include <API/Headers/Editor/ExpressionComposition.h>
#include <API/Headers/Editor/Value.h>

#include <core/document/DocumentModel.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>

OSSIAControl::OSSIAControl(iscore::Presenter* pres):
    iscore::PluginControlInterface {pres, "OSSIAControl", nullptr}
{
    using namespace OSSIA;
    Local localDevice;
    m_localDevice = Device::create(localDevice, "i-score");
    // Two parts :
    // One that maintains the devices for each document
    // (and disconnects / reconnects them when the current document changes)
    // Also during execution, one shouldn't be able to switch document.

    // Another part that, at execution time, creates structures corresponding
    // to the Scenario plug-in with the OSSIA API.
}


void OSSIAControl::populateMenus(iscore::MenubarManager* menu)
{
    QAction* play = new QAction {tr("Play (OSSIA engine)"), this};
    connect(play, &QAction::triggered,
            [&] ()
    {
        auto plug = currentDocument()->model()->pluginModel("OSSIADocumentPlugin");
        static_cast<OSSIADocumentPlugin*>(plug)->baseScenario()->startEvent()->event()->play();

    });

    menu->insertActionIntoToplevelMenu(iscore::ToplevelMenuElement::PlayMenu,
                                       play);
}

iscore::DocumentDelegatePluginModel*OSSIAControl::loadDocumentPlugin(
        const QString& name,
        const VisitorVariant& var,
        iscore::DocumentModel* model)
{
    // We don't have anything to load; it's easier to just recreate.
    return nullptr;
}

void OSSIAControl::on_newDocument(iscore::Document* doc)
{
    doc->model()->addPluginModel(new OSSIADocumentPlugin{doc->model(), doc->model()});
}

void OSSIAControl::on_loadedDocument(iscore::Document *doc)
{
    on_newDocument(doc);
}

void OSSIAControl::on_documentChanged()
{
}
