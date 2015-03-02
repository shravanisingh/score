#pragma once
#include <tools/IdentifiedObject.hpp>
#include <ProcessInterface/TimeValue.hpp>
#include <source/Document/Properties/Selectable.hpp>
#include <core/interface/selection/Selection.hpp>

class QDataStream;

class InspectorSectionWidget;
class ProcessViewModelInterface;
/**
     * @brief The ProcessSharedModelInterface class
     *
     * Interface to implement to make a process.
     */
class ProcessSharedModelInterface: public IdentifiedObject<ProcessSharedModelInterface>
{
        Q_OBJECT
    public:
        // TODO Maybe the selectability should in fact
        // be part of the parent element ???
        Selectable selection;

        using IdentifiedObject<ProcessSharedModelInterface>::IdentifiedObject;
        virtual ProcessSharedModelInterface* clone(id_type<ProcessSharedModelInterface> newId, QObject* newParent) = 0;

        /**
         * @brief processName
         * @return the name of the process.
         *
         * Needed for serialization - deserialization, in order to recreate
         * a new process from the same plug-in.
         */
        virtual QString processName() const = 0; // Needed for serialization.

        virtual ~ProcessSharedModelInterface() = default;

        //// View models interface
        // TODO pass the name of the view model to be created
        // (e.g. temporal / logical...).
        virtual ProcessViewModelInterface* makeViewModel(id_type<ProcessViewModelInterface> viewModelId,
                QObject* parent) = 0;

        // To be called by createProcessViewModel only.
        virtual ProcessViewModelInterface* makeViewModel(SerializationIdentifier identifier,
                void* data,
                QObject* parent) = 0;

        // "Copy" factory. TODO replace by clone methode on PVM ?
        virtual ProcessViewModelInterface* makeViewModel(id_type<ProcessViewModelInterface> newId,
                const ProcessViewModelInterface* source,
                QObject* parent) = 0;

        // Do a copy.
        QVector<ProcessViewModelInterface*> viewModels()
        {
            return m_viewModels;
        }


        //// Features of a process
        /// Duration
        // Used to scale the process.
        virtual void setDurationWithScale(TimeValue newDuration) = 0;
        virtual void setDurationWithoutScale(TimeValue newDuration) = 0;

        // TODO might not be useful... put in protected ?
        // Constructor needs it, too.
        void setDuration(const TimeValue& other)
        {
            m_duration = other;
        }

        TimeValue duration() const
        {
            return m_duration;
        }

        /// Selection
        virtual QList<QObject*> selectedChildren() const = 0;
        virtual void setSelection(const Selection& s) = 0;

        // protected:
        virtual void serialize(SerializationIdentifier identifier,
                               void* data) const = 0;

    signals:
        void selectedChildrenChanged(ProcessSharedModelInterface* proc);

    protected:
        void addViewModel(ProcessViewModelInterface* m)
        {
            m_viewModels.push_back(m);
        }

        void removeViewModel(ProcessViewModelInterface* m)
        {
            int index = m_viewModels.indexOf(m);

            if(index != -1)
            {
                m_viewModels.remove(index);
            }
        }

    private:
        // Ownership ? The parent is the Deck. Always.
        // A process view is never displayed alone, it is always in a view, which is in a box.
        QVector<ProcessViewModelInterface*> m_viewModels;
        TimeValue m_duration;
};

template<typename T>
QVector<typename T::view_model_type*> viewModels(T* processViewModel)
{
    QVector<typename T::view_model_type*> v;

    for(auto& elt : processViewModel->viewModels())
    {
        v.push_back(static_cast<typename T::view_model_type*>(elt));
    }

    return v;
}

inline ProcessSharedModelInterface* parentProcess(QObject* obj)
{
    QString objName {obj ? obj->objectName() : "INVALID"};
    while(obj && !obj->inherits("ProcessSharedModelInterface"))
    {
        obj = obj->parent();
    }

    if(!obj)
         throw std::runtime_error(
                         QString("Object (name: %1) is not child of a Process!")
                         .arg(objName)
                         .toStdString());

    return static_cast<ProcessSharedModelInterface*>(obj);
}
