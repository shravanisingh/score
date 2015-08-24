#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ObjectPath.hpp>

#include "Plugin/Panel/DeviceExplorerModel.hpp"

#include <DeviceExplorer/Node/Node.hpp>

namespace DeviceExplorer
{
    namespace Command
    {
        class EditData : public iscore::SerializableCommand
        {
            ISCORE_COMMAND_DECL("EditData", "EditData")
            public:
                ISCORE_SERIALIZABLE_COMMAND_DEFAULT_CTOR(EditData, "DeviceExplorerControl")
                EditData(
                    ObjectPath&& device_tree,
                    const iscore::NodePath& nodePath,
                    DeviceExplorerModel::Column column,
                    const QVariant& value,
                    int role);

                virtual void undo() override;
                virtual void redo() override;

            protected:
                virtual void serializeImpl(QDataStream&) const override;
                virtual void deserializeImpl(QDataStream&) override;

            private:
                ObjectPath m_deviceTree;
                iscore::NodePath m_nodePath;
                DeviceExplorerModel::Column m_column;
                QVariant m_oldValue;
                QVariant m_newValue;
                int m_role;
        };
    }
}

