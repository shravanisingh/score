project(score_lib_base LANGUAGES CXX)
score_common_setup()

# Packages
find_package(Qt5 5.7 REQUIRED COMPONENTS Core Widgets Svg )

# Files & main target
qt5_add_resources(QRCS "${CMAKE_CURRENT_SOURCE_DIR}/resources/score.qrc")

set(HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/ApplicationInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/ApplicationRegistrar.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/ApplicationSettings.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/CommandBackupFile.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/MockApplication.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/OpenDocumentsFile.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/application/SafeQApplication.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/command/CommandStack.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/command/CommandStackSerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/Document.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentBackupManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentBackups.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentBuilder.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentMetadata.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentPresenter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentView.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/documentation/intro.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/documentation/models.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/messages/MessagesPanel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/plugin/PluginDependencyGraph.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/plugin/PluginManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/AboutDialog.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/CoreActions.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/CoreApplicationPlugin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/DocumentManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/Presenter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/settings/Settings.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/settings/SettingsPresenter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/settings/SettingsView.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/undo/Panel/UndoPanelDelegate.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/undo/Panel/UndoPanelFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/undo/Panel/Widgets/UndoListWidget.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/undo/UndoApplicationPlugin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/view/Window.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/view/QRecentFilesMenu.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/score/actions/Action.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/actions/ActionManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/actions/Menu.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/actions/MenuManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/actions/Toolbar.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/actions/ToolbarManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/application/ApplicationComponents.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/application/ApplicationContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/application/GUIApplicationContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/AggregateCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Command.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandData.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandFactoryKey.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandGeneratorMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandStackFacade.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/CommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/ICommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/MacroCommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/MultiOngoingCommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/OngoingCommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/RuntimeDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/SendStrategy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/SettingsCommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Dispatchers/SingleOngoingCommandDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/PropertyCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/SettingsCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/command/Validity/ValidityChecker.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/document/ChangeId.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/document/DocumentContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/document/DocumentInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/locking/ObjectLocker.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/ColorReference.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/Component.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/ComponentFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/ComponentHierarchy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/ComponentSerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/Entity.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/EntityBase.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/EntityMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/EntityImpl.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/HierarchicalModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/IdentifiedObject.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/IdentifiedObjectAbstract.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/IdentifiedObjectMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/Identifier.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/ModelMetadata.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/Skin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/ObjectIdentifier.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/ObjectPath.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/ObjectRemover.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/Path.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/PathSerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/RelativePath.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/InvisibleRootNode.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/TreeNode.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/TreeNodeItemModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/TreeNodeSerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/TreePath.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/VariantBasedNode.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/Addon.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/PluginInstances.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ProjectSettings/ProjectSettingsFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ProjectSettings/ProjectSettingsModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ProjectSettings/ProjectSettingsPresenter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ProjectSettings/ProjectSettingsView.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/application/GUIApplicationPlugin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/InterfaceList.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/Interface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/FactorySetup.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ModelFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/SerializableInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/StringFactoryKey.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/StringFactoryKeySerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/UuidKey.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegateFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegateModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegatePresenter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegateView.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/plugin/DocumentPluginBase.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/plugin/SerializableDocumentPlugin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/plugin/DocumentPlugin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/plugin/DocumentPluginCreator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/panel/PanelDelegate.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/panel/PanelDelegateFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/CommandFactory_QtInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/FactoryFamily_QtInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/GUIApplicationPlugin_QtInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/settingsdelegate/SettingsDelegateFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/settingsdelegate/SettingsDelegateModel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/settingsdelegate/SettingsDelegatePresenter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/settingsdelegate/SettingsDelegateView.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/prefix.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/selection/FocusManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/selection/Selectable.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/selection/Selection.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/selection/SelectionDispatcher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/selection/SelectionStack.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/AnySerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/DataStreamVisitor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/IsTemplate.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/JSONValueVisitor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/JSONVisitor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/MimeVisitor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/StringConstants.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/VariantSerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/VisitorCommon.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/VisitorInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/VisitorTags.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/CommonSelectionState.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/GraphicsSceneTool.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/GraphicsSceneToolPalette.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/StateMachineTools.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/StateMachineUtils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/Clamp.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/DeleteAll.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/File.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/ForEachType.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/IdentifierGeneration.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/MapCopy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/Metadata.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/QMapHelper.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/RandomNameProvider.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/SubtypeVariant.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/Todo.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/Version.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/exceptions/MissingCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/opaque/OpaqueString.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/Algorithms.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/ArrayView.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/HashMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/HashMapSerialization.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/IndirectContainer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/Invoke.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/Optional.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/OwningVector.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/Pointer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/String.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/StringHash.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/GraphicWidgets.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/GraphicsItem.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/GraphicsProxyObject.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/RectItem.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/TextItem.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/ClearLayout.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/ControlWidgets.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/CommandSpinBox.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/DoubleSlider.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/JS/JSEdit.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/MarginLess.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/MimeData.hpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/QmlContainerPanel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/Pixmap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/ReactiveLabel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/Separator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/SetIcons.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/SignalUtils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/SpinBoxes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/TextLabel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/WidgetWrapper.hpp"

     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/QProgressIndicator/QProgressIndicator.h"

     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_palette_model.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_palette_widget.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/colorwidgets_global.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_wheel.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_line_edit.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/swatch.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_delegate.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_palette.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/hue_slider.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/abstract_widget_list.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/bound_color_selector.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_dialog.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_2d_slider.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_selector.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_list_widget.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_preview.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/gradient_slider.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/QtColorWidgets/color_names.hpp"
)

set(SRCS

"${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/StringConstants.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/AnySerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/tree/InvisibleRootNodeSerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/ObjectPathSerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/tools/IdentifierGeneration.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandDataSerialization.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/Interface.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/InterfaceList.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/SerializableInterface.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/command/AggregateCommand.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/command/Command.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandData.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/command/CommandGeneratorMap.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/command/PropertyCommand.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/command/Validity/ValidityChecker.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/model/Component.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/application/ApplicationContext.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/application/ApplicationComponents.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/actions/Action.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/actions/ActionManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/actions/MenuManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/actions/ToolbarManager.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/document/DocumentInterface.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/locking/ObjectLocker.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegateFactory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegateModel.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegateView.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/DocumentDelegatePresenter.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/documentdelegate/plugin/DocumentPlugin.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/application/GUIApplicationPlugin.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/panel/PanelDelegate.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/panel/PanelDelegateFactory.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/settingsdelegate/SettingsDelegateFactory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/settingsdelegate/SettingsDelegateModel.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ProjectSettings/ProjectSettingsFactory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/ProjectSettings/ProjectSettingsModel.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/Addon.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/PluginInstances.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/FactoryFamily_QtInterface.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/FactoryInterface_QtInterface.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/GUIApplicationPlugin_QtInterface.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/PluginRequirements_QtInterface.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/plugins/qt_interfaces/CommandFactory_QtInterface.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/selection/SelectionDispatcher.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/selection/SelectionStack.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/DataStreamVisitor.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/JSONObjectVisitor.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/serialization/QtTypesJsonVisitors.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/ObjectIdentifierSerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/IdentifiedObjectAbstract.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/model/path/ObjectPath.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/ObjectRemover.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/ModelMetadata.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/Skin.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/model/ColorReference.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/GraphicsSceneToolPalette.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/statemachine/CommonSelectionState.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/tools/std/String.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/tools/RandomNameProvider.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/GraphicsItem.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/GraphicWidgets.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/RectItem.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/graphics/TextItem.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/ClearLayout.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/ControlWidgets.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/DoubleSlider.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/SetIcons.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/QmlContainerPanel.cpp" # TODO MOVEME
"${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/Separator.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/score/widgets/JS/JSEdit.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/core/application/SafeQApplication.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/application/ApplicationInterface.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/application/ApplicationRegistrar.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/application/ApplicationSettings.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/command/CommandStackSerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/application/OpenDocumentsFile.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/application/CommandBackupFile.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/command/CommandStack.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentPresenter.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentView.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentBackupManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentBackups.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentBuilder.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentMetadata.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/Document.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentModel.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/document/DocumentSerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/messages/MessagesPanel.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/plugin/PluginManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/AboutDialog.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/CoreApplicationPlugin.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/DocumentManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/presenter/Presenter.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/settings/Settings.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/undo/Panel/Widgets/UndoListWidget.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/undo/Panel/UndoPanelDelegate.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/undo/Panel/UndoPanelFactory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/undo/UndoApplicationPlugin.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/view/Window.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/view/QRecentFilesMenu.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/QProgressIndicator/QProgressIndicator.cpp"

# "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_list_widget.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/gradient_slider.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_line_edit.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/swatch.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/bound_color_selector.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_palette_widget.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_palette_model.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_2d_slider.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/abstract_widget_list.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_delegate.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_utils.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_dialog.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_palette.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_selector.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_preview.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/hue_slider.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_wheel.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets/src/color_names.cpp"
)

add_library(score_lib_base ${SRCS} ${HEADERS} ${QRCS})
target_link_libraries(score_lib_base
    PUBLIC Qt5::Core Qt5::Widgets ossia # Qt5::Qml Qt5::QuickWidgets
    PRIVATE Qt5::Svg )
target_include_directories(score_lib_base PUBLIC 3rdparty/QProgressIndicator)
target_include_directories(score_lib_base PUBLIC 3rdparty/Qt-Color-Widgets)
target_include_directories(score_lib_base PUBLIC 3rdparty/Qt-Color-Widgets/src)
target_include_directories(score_lib_base PUBLIC 3rdparty/Qt-Color-Widgets/QtColorWidgets)

set_property(TARGET score_lib_base PROPERTY COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/score/prefix.hpp")
setup_score_library(score_lib_base)
setup_score_tests(tests)

set(_score_license_written 0 CACHE INTERNAL "")
if(NOT _score_license_written)
  set(_score_license_written 1 CACHE INTERNAL "")
  include(GenerateLicense)
  set(license_text "")
  AddLicenseFile(license_text ossia "${CMAKE_SOURCE_DIR}/API/LICENSE")
  AddLicenseFile(license_text score "${CMAKE_SOURCE_DIR}/LICENSE.txt")
  AddLicenseFile(license_text Asio "${OSSIA_3RDPARTY_FOLDER}/asio/asio/LICENSE_1_0.txt")
  AddLicenseFile(license_text bitset2 "${OSSIA_3RDPARTY_FOLDER}/bitset2/LICENSE.txt")
  AddLicenseFile(license_text Brigand "${OSSIA_3RDPARTY_FOLDER}/brigand/LICENSE")
  AddLicenseFile(license_text chobo "${OSSIA_3RDPARTY_FOLDER}/chobo-shl/LICENSE.txt")
  AddLicenseFile(license_text readerwriterqueue "${OSSIA_3RDPARTY_FOLDER}/readerwriterqueue/LICENSE.md")
  AddLicenseFile(license_text concurrentqueue "${OSSIA_3RDPARTY_FOLDER}/concurrentqueue/LICENSE.md")
  AddLicenseFile(license_text flat "${OSSIA_3RDPARTY_FOLDER}/flat/LICENSE_1_0.txt")
  AddLicenseFile(license_text fmt "${OSSIA_3RDPARTY_FOLDER}/fmt/LICENSE.rst")
  AddLicenseFile(license_text frozen "${OSSIA_3RDPARTY_FOLDER}/frozen/LICENSE")
  AddLicenseFile(license_text GSL "${OSSIA_3RDPARTY_FOLDER}/GSL/LICENSE")
  AddLicenseFile(license_text hopscotchmap "${OSSIA_3RDPARTY_FOLDER}/hopscotch-map/LICENSE")
  AddLicenseFile(license_text rtmidi "${OSSIA_3RDPARTY_FOLDER}/RtMidi17/LICENSE.ModernMidi")
  AddLicenseFile(license_text modernmidi "${OSSIA_3RDPARTY_FOLDER}/RtMidi17/LICENSE.ModernMidi")
  AddLicenseFile(license_text multiindex "${OSSIA_3RDPARTY_FOLDER}/multi_index/LICENSE.md")
  AddLicenseFile(license_text nanosignal "${OSSIA_3RDPARTY_FOLDER}/nano-signal-slot/LICENSE")
  AddLicenseFile(license_text oscpack "${OSSIA_3RDPARTY_FOLDER}/oscpack/LICENSE")
  AddLicenseFile(license_text pd "${OSSIA_3RDPARTY_FOLDER}/pure-data/LICENSE.txt")
  AddLicenseFile(license_text portaudio "${OSSIA_3RDPARTY_FOLDER}/pure-data/portaudio/portaudio/LICENSE.txt")
  AddLicenseFile(license_text pybind11 "${OSSIA_3RDPARTY_FOLDER}/pybind11/LICENSE")
  AddLicenseFile(license_text rapidjson "${OSSIA_3RDPARTY_FOLDER}/rapidjson/license.txt")
  AddLicenseFile(license_text servus "${OSSIA_3RDPARTY_FOLDER}/Servus/LICENSE.txt")
  AddLicenseFile(license_text smallfun "${OSSIA_3RDPARTY_FOLDER}/SmallFunction/LICENSE")
  AddLicenseFile(license_text spdlog "${OSSIA_3RDPARTY_FOLDER}/spdlog/LICENSE")
  AddLicenseFile(license_text variant "${OSSIA_3RDPARTY_FOLDER}/variant/LICENSE.txt")
  AddLicenseFile(license_text verdigris "${OSSIA_3RDPARTY_FOLDER}/verdigris/LICENSE.LGPLv3")
  AddLicenseFile(license_text whereami "${OSSIA_3RDPARTY_FOLDER}/whereami/LICENSE")
  AddLicenseFile(license_text websocketpp "${OSSIA_3RDPARTY_FOLDER}/websocketpp/COPYING")

  AddLicenseFile(license_text qprogressindicator "${CMAKE_SOURCE_DIR}/base/lib/3rdparty/QProgressIndicator/LICENSE")
  AddLicenseFile(license_text qtcolorwidgets "${CMAKE_SOURCE_DIR}/base/lib/3rdparty/Qt-Color-Widgets/COPYING")

  score_write_file("${CMAKE_BINARY_DIR}/score_licenses.hpp" "${license_text}")
endif()
