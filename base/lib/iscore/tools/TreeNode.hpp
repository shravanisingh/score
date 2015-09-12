#pragma once
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
/*
 * @brief The TreeNode class
 *
 * This class adds a tree structure around a data type.
 * It can then be used in abstract item models easily.
 */
template<typename DataType>
class TreeNode : public DataType
{
    public:

        using iterator = typename std::vector<TreeNode>::iterator;
        using const_iterator = typename std::vector<TreeNode>::const_iterator;

        auto begin() { return m_children.begin(); }
        auto begin() const { return cbegin(); }
        auto cbegin() const { return m_children.cbegin(); }

        auto end() { return m_children.end(); }
        auto end() const { return cend(); }
        auto cend() const { return m_children.cend(); }

        TreeNode():
            DataType{}
        {

        }

        TreeNode(const DataType& data):
            DataType{data}
        {

        }

        // The parent has to be set afterwards.
        TreeNode(const TreeNode& other):
            TreeNode{other, nullptr}
        {

        }

        TreeNode(TreeNode&& other):
            TreeNode{other, nullptr} //OPTIMIZEME
        {

        }

        TreeNode(DataType&& data):
            DataType{std::move(data)}
        {

        }

        template<typename T>
        TreeNode(const T& data, TreeNode* parent):
            DataType(data),
            m_parent{parent}
        {
        }

        // Clone
        explicit TreeNode(
                const TreeNode& source,
                 TreeNode* parent):
            DataType{static_cast<const DataType&>(source)},
            m_parent{parent}
        {
            for(const auto& child : source.m_children)
            {
                emplace_back(child, this);
            }
        }

        void push_back(const TreeNode& child)
        {
            emplace_back(child, this);
        }
        void push_back(TreeNode&& child)
        {
            emplace_back(std::move(child), this);
        }

        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            m_children.emplace_back(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void emplace(Args&&... args)
        {
            m_children.emplace(std::forward<Args>(args)...);
        }

        // TODO do move operators.
        // Only the top-level child need to have their parents changd, no need to do a recusion.
/*      TreeNode& operator=(TreeNode&& source)
        {
            static_cast<DataType&>(*this) = static_cast<const DataType&>(source);

            qDeleteAll(m_children);
            m_children.clear();
            m_children.swap(source.m_children);
            qDebug() << source.children().size();

            for(auto& n : m_children)
            {
                n->setParent(this);
            }

            return *this;
        }
*/
        TreeNode& operator=(const TreeNode& source)
        {
            static_cast<DataType&>(*this) = static_cast<const DataType&>(source);

            m_children = source.m_children;
            for(auto& child : m_children)
            {
                child.setParent(this);
            }

            return *this;
        }

        TreeNode* parent() const
        {
            return m_parent;
        }

        bool hasChild(std::size_t index) const
        { return m_children.size() > index; }

        TreeNode& childAt(int index)
        {
            ISCORE_ASSERT(hasChild(index));
            return m_children.at(index);
        }

        const TreeNode& childAt(int index) const
        {
            ISCORE_ASSERT(hasChild(index));
            return m_children.at(index);
        }

        // returns -1 if not found
        int indexOfChild(const TreeNode* child) const
        {
            for(std::size_t i = 0U; i < m_children.size(); i++)
                if(child == &m_children[i])
                    return i;

            return -1;
        }

        int childCount() const
        { return m_children.size(); }

        bool hasChildren() const
        { return ! m_children.empty(); }

        const auto& children() const
        { return m_children;  }

        [[deprecated]]
        const auto& insertChild(int index, const TreeNode& n)
        {
            auto it = m_children.insert(index, n);
            it->m_parent = this;
            return *it;
        }

        [[deprecated]]
        const auto& addChild(const TreeNode& n)
        {
            m_children.push_back(n);
            const auto& last = m_children.back();
            last.m_parent = this;
            return last;
        }

        void swapChildren(int oldIndex, int newIndex)
        {
            ISCORE_ASSERT(oldIndex < m_children.size());
            ISCORE_ASSERT(newIndex < m_children.size());

            m_children.swap(oldIndex, newIndex);
        }

        TreeNode* takeChild(int index)
        {
            TreeNode* n = m_children.takeAt(index);
            ISCORE_ASSERT(n);
            n->m_parent = 0;
            return n;
        }

        // Won't delete the child!
        void removeChild(const_iterator it)
        {
            m_children.erase(it);
            //m_children.removeAll(child);
        }

    private:
        void setParent(TreeNode* parent)
        {
            m_parent = parent;
        }


        TreeNode* m_parent {};
        std::vector<TreeNode> m_children; // TODO boost::ptr_container
};


template<typename T>
void Visitor<Reader<DataStream>>::readFrom(const TreeNode<T>& n)
{
    readFrom(static_cast<const T&>(n));

    m_stream << n.childCount();
    for(const auto& child : n)
    {
        readFrom(child);
    }

    insertDelimiter();
}


template<typename T>
void Visitor<Writer<DataStream>>::writeTo(TreeNode<T>& n)
{
    writeTo(static_cast<T&>(n));

    int childCount;
    m_stream >> childCount;
    for (int i = 0; i < childCount; ++i)
    {
        TreeNode<T> child;
        writeTo(child);
        n.push_back(std::move(child));
    }

    checkDelimiter();
}

template<typename T>
void Visitor<Reader<JSONObject>>::readFrom(const TreeNode<T>& n)
{
    readFrom(static_cast<const T&>(n));
    m_obj["Children"] = toJsonArray(n);
}

template<typename T>
void Visitor<Writer<JSONObject>>::writeTo(TreeNode<T>& n)
{
    writeTo(static_cast<T&>(n));
    for (const auto& val : m_obj["Children"].toArray())
    {
        TreeNode<T> child;
        Deserializer<JSONObject> nodeWriter(val.toObject());

        nodeWriter.writeTo(child);
        n.push_back(std::move(child));
    }
}
