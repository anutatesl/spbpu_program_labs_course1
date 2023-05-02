#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

template <typename K, typename V, typename Comparator = std::less<K>>

class RedBlackTree
{
public:
    class Iterator;

    explicit RedBlackTree(const Comparator& cmp = Comparator());

    RedBlackTree(const RedBlackTree& other) = delete;
    RedBlackTree(RedBlackTree&& other) noexcept;
    RedBlackTree& operator=(const RedBlackTree& other) = delete;
    RedBlackTree& operator=(RedBlackTree&& other) noexcept;
    ~RedBlackTree();

    void insert(const K& key, const V& value);
    bool contains(const K& key);
    void deleteKey(const K& key);
    V& operator[](const K& key);

    Iterator begin();
    Iterator end();

private:
    struct TreeImpl;

    TreeImpl impl_;
};

namespace treeDetails
{
    template <typename K, typename V>
    struct Node;

    template <typename K, typename V>
    using NodePtr = Node<K, V>*;

    enum color_t
    {
        RED = false, BLACK = true
    };

    template <typename K, typename V>
    struct Node
    {
        K key;
        V value;
        color_t color;
        treeDetails::NodePtr<K, V> parent;
        treeDetails::NodePtr<K, V> left;
        treeDetails::NodePtr<K, V> right;
    };
}

template <typename K, typename V, typename Comparator>
class RedBlackTree<K, V, Comparator>::Iterator
{
public:
    explicit Iterator(treeDetails::NodePtr<K, V> node):
        node_{ node }
    { }

    Iterator& operator++()
    {
        moveNext();
        return *this;
    }

    bool operator==(const Iterator& rhs) const
    {
        return node_ == rhs.node_;
    }

    bool operator!=(const Iterator& rhs) const
    {
        return node_ != rhs.node_;
    }

    K& key()
    {
        return node_->key;
    }

    V& value()
    {
        return node_->value;
    }

private:
    void moveNext()
    {
        if (node_->right) 
        {
            node_ = node_->right;
            while (node_->left) 
            {
                node_ = node_->left;
            }
        }
        else if (node_->parent && (node_->parent->left == node_)) 
        {
            node_ = node_->parent;
        }
        else 
        {
            while (node_->parent && (node_->parent->right == node_)) 
            {
                node_ = node_->parent;
            }
            node_ = node_->parent;
        }
    }

    treeDetails::NodePtr<K, V> node_;
};

template <typename K, typename V, typename Comparator>
struct RedBlackTree<K, V, Comparator>::TreeImpl
{
    treeDetails::NodePtr<K, V> root;
    Comparator cmp;
};

template <typename K, typename V, typename Comparator>
RedBlackTree<K, V, Comparator>::RedBlackTree(const Comparator& cmp):
    impl_{ nullptr, cmp }
{ }

template <typename K, typename V, typename Comparator>
RedBlackTree<K, V, Comparator>::RedBlackTree(RedBlackTree&& other) noexcept: 
    impl_{ other.impl_ }
{
    other.impl_.root = nullptr;
}

template <typename K, typename V, typename Comparator>
RedBlackTree<K, V, Comparator>& RedBlackTree<K, V, Comparator>::operator=(RedBlackTree&& other) noexcept
{
    impl_ = other.impl_;
    other.impl_.root = nullptr;
    return *this;
}

namespace treeDetails
{
    template <typename K, typename V>
    void recursiveDelete(treeDetails::NodePtr<K, V> node);
}

template <typename K, typename V, typename Comparator>
RedBlackTree<K, V, Comparator>::~RedBlackTree()
{
    if (impl_.root) 
    {
        treeDetails::recursiveDelete(impl_.root);
    }
}

namespace treeDetails
{
    template <typename K, typename V>
    void insertNode(treeDetails::NodePtr<K, V> node);
}

template <typename K, typename V, typename Comparator>
void RedBlackTree<K, V, Comparator>::insert(const K& key, const V& value)
{
    treeDetails::Node<K, V>* current = impl_.root;
    treeDetails::Node<K, V>* prev = treeDetails::NodePtr<K, V>{ nullptr };
    while (current && (current->key != key)) 
    {
        prev = current;
        current = impl_.cmp(current->key, key) ? current->right : current->left;
    }
    if (current) 
    {
        current->value = value;
        return;
    }
    current = new treeDetails::Node<K, V>{ key, value, treeDetails::RED, prev, nullptr, nullptr };
    if (!impl_.root) 
    {
        impl_.root = current;
    }
    else if (impl_.cmp(prev->key, key)) 
    {
        prev->right = current;
    }
    else 
    {
        prev->left = current;
    }
    treeDetails::insertNode(current);
    while (impl_.root->parent)
    {
        impl_.root = impl_.root->parent;
    }
}

namespace treeDetails
{
    template <typename K, typename V, typename Comparator>
    treeDetails::NodePtr<K, V> find(const K& key, treeDetails::NodePtr<K, V> root, const Comparator& cmp = Comparator{ });
}

template <typename K, typename V, typename Comparator>
bool RedBlackTree<K, V, Comparator>::contains(const K& key)
{
    return treeDetails::find(key, impl_.root, impl_.cmp);
}

namespace treeDetails
{
    template <typename K, typename V>
    void replaceNode(treeDetails::NodePtr<K, V> n, treeDetails::NodePtr<K, V> child);

    template <typename K, typename V>
    void deleteNode(treeDetails::NodePtr<K, V> node);
}

template<typename K, typename V, typename Comparator>
void RedBlackTree<K, V, Comparator>::deleteKey(const K& key)
{
    treeDetails::Node<K, V>* current = impl_.root;
    treeDetails::Node<K, V>* prev = treeDetails::NodePtr<K, V>{ nullptr };
    while (current && (current->key != key))
    {
        prev = current;
        current = impl_.cmp(current->key, key) ? current->right : current->left;
    }
    if (!current)
    {
        return;
    }
    if (current->left == nullptr && current->right == nullptr)
    {
        if (current == impl_.root)
        {
            impl_.root = nullptr;
        }
        else if (prev && prev->left == current)
        {
            prev->left = nullptr;
        }
        else if (prev && prev->right == current)
        {
            prev->right = nullptr;
        }
        delete current;
    }
    else if (current->left == nullptr && current->right != nullptr)
    {

        if (prev && prev->left == current)
        {
            prev->left = current->right;
        }
        if (prev && prev->right == current)
        {
            prev->right = current->right;
        }
        delete current;
    }
    else if (current->right == nullptr && current->left != nullptr)
    {
        if (prev && prev->left == current)
        {
            prev->left = current->right;
        }
        if (prev && prev->right == current)
        {
            prev->right = current->left;
        }
        delete current;
    }
    else
    {
        treeDetails::Node<K, V>* replace = current->right;
        while (replace->left)
        {
            replace = replace->left;
        }
        replaceNode(current, replace);
        if (current->color == treeDetails::BLACK)
        {
            if (replace->color == treeDetails::RED)
            {
                replace->color = treeDetails::BLACK;
            }
            else
            {
                deleteNode(replace);
            }
        }
        delete current;
    }
}

template <typename K, typename V, typename Comparator>
V& RedBlackTree<K, V, Comparator>::operator[](const K& key)
{
    treeDetails::Node<K, V>* node = treeDetails::find(key, impl_.root, impl_.cmp);
    if (node) 
    {
        return node->value;
    }
    throw std::invalid_argument{ "No such key in map!" };
}

template <typename K, typename V, typename Comparator>
typename RedBlackTree<K, V, Comparator>::Iterator RedBlackTree<K, V, Comparator>::begin()
{
    treeDetails::Node<K, V>* c = impl_.root;
    while (c->left) 
    {
        c = c->left;
    }
    return Iterator(c);
}

template <typename K, typename V, typename Comparator>
typename RedBlackTree<K, V, Comparator>::Iterator RedBlackTree<K, V, Comparator>::end()
{
    return Iterator(nullptr);
}

namespace treeDetails
{
    template <typename K, typename V>
    void recursiveDelete(treeDetails::NodePtr<K, V> node)
    {
        if (node->left) 
        {
            recursiveDelete(node->left);
        }
        if (node->right) 
        {
            recursiveDelete(node->right);
        }
        delete node;
    }

    template <typename K, typename V, typename Comparator>
    treeDetails::NodePtr<K, V> find(const K& key, treeDetails::NodePtr<K, V> root, const Comparator& cmp)
    {
        treeDetails::NodePtr<K, V> current = root;
        while (current && (current->key != key))
        {
            current = cmp(current->key, key) ? current->right : current->left;
        }
        return current;
    }

    template <typename K, typename V>
    void insertCase1(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertNode(treeDetails::NodePtr<K, V> node)
    {
        insertCase1(node);
    }

    template <typename K, typename V>
    void insertCase2(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertCase1(treeDetails::NodePtr<K, V> n)
    {
        if (!n->parent) 
        {
            n->color = BLACK;
        }
        else 
        {
            insertCase2(n);
        }
    }

    template <typename K, typename V>
    void insertCase3(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertCase2(treeDetails::NodePtr<K, V> n)
    {
        if (n->parent->color != BLACK) 
        {
            insertCase3(n);
        }
    }

    template <typename K, typename V>
    treeDetails::NodePtr<K, V> grandparent(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    treeDetails::NodePtr<K, V> uncle(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertCase4(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertCase3(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* u = uncle(n);
        if (u && (u->color == RED)) 
        {
            n->parent->color = BLACK;
            u->color = BLACK;
            auto gp = grandparent(n);
            gp->color = RED;
            insertCase1(gp);
        }
        else 
        {
            insertCase4(n);
        }
    }

    template <typename K, typename V>
    void rotateLeft(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void rotateRight(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertCase5(treeDetails::NodePtr<K, V> n);

    template <typename K, typename V>
    void insertCase4(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* p = n->parent;
        treeDetails::Node<K, V>* gp = grandparent(n);
        if ((n == p->right) && (p == gp->left)) 
        {
            rotateLeft(p);
            n = n->left;
        }
        else if ((n == p->left) && (p == gp->right)) 
        {
            rotateRight(p);
            n = n->right;
        }
        insertCase5(n);
    }

    template <typename K, typename V>
    void insertCase5(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* p = n->parent;
        treeDetails::Node<K, V>* gp = grandparent(n);
        if ((n == p->left) && (p == gp->left)) 
        {
            rotateRight(gp);
        }
        else 
        {
            rotateLeft(gp);
        }
        p->color = BLACK;
        gp->color = RED;
    }

    template <typename K, typename V>
    treeDetails::NodePtr<K, V> grandparent(treeDetails::NodePtr<K, V> n)
    {
        return (n && n->parent) ? n->parent->parent : nullptr;
    }

    template <typename K, typename V>
    treeDetails::NodePtr<K, V> uncle(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* gp = grandparent(n);
        return gp ? (gp->left == n->parent) ? gp->right : gp->left : nullptr;
    }

    template <typename K, typename V>
    void replaceNode(treeDetails::NodePtr<K, V> n, treeDetails::NodePtr<K, V> child)
    {
        child->parent = n->parent;
        if (n == n->parent->left) 
        {
            n->parent->left = child;
        }
        else 
        {
            n->parent->right = child;
        }
    }
    
    template <typename K, typename V>
    treeDetails::NodePtr<K, V> sibling(treeDetails::NodePtr<K, V> n)
    {
        return (n == n->parent->left) ? n->parent->right : n->parent->left;
    }

    template <typename K, typename V>
    void rotateLeft(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* pivot = n->right;
        treeDetails::Node<K, V>* p = n->parent;
        n->right = pivot->left;
        pivot->left = n;
        n->parent = pivot;
        if (n->right) 
        {
            n->right->parent = n;
        }
        if (p) 
        {
            if (p->left == n) 
            {
                p->left = pivot;
            }
            else 
            {
                p->right = pivot;
            }
        }
        pivot->parent = p;
    }

    template <typename K, typename V>
    void rotateRight(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* pivot = n->left;
        treeDetails::Node<K, V>* p = n->parent;
        n->left = pivot->right;
        pivot->right = n;
        n->parent = pivot;
        if (n->left) 
        {
            n->left->parent = n;
        }
        if (p) 
        {
            if (p->left == n)
            {
                p->left = pivot;
            }
            else 
            {
                p->right = pivot;
            }
        }
        pivot->parent = p;
    }

    template <typename K, typename V>
    void deleteCase1(treeDetails::NodePtr<K, V> n);
    
    template <typename K, typename V>
    void deleteNode(treeDetails::NodePtr<K, V> node)
    {
        deleteCase1(node);
    }
    
    template <typename K, typename V>
    void deleteCase2(treeDetails::NodePtr<K, V> n);
    
    template <typename K, typename V>
    void deleteCase1(treeDetails::NodePtr<K, V> n)
    {
        if (n->parent != NULL)
        {
            deleteCase2(n);
        }
    }
    
    template <typename K, typename V>
    void deleteCase3(treeDetails::NodePtr<K, V> n);
    
    template <typename K, typename V>
    void deleteCase2(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* son = sibling(n);
        if (son->color == RED)
        {
            n->parent->color = RED;
            son->color = BLACK;
            if (n == n->parent->left)
            {
                rotateLeft(n->parent);
            }
            else
            {
                rotateRight(n->parent);
            }
        }
        deleteCase3(n);
    }
    
    template <typename K, typename V>
    void deleteCase4(treeDetails::NodePtr<K, V> n);
    
    template <typename K, typename V>
    void deleteCase3(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* son = sibling(n);
        if ((n->parent->color == BLACK) && (son->color == BLACK) &&
            (son->left->color == BLACK) && (son->right->color == BLACK))
        {
            son->color = RED;
            deleteCase1(n->parent);
        }
        else
            deleteCase4(n);
    }
    
    template <typename K, typename V>
    void deleteCase5(treeDetails::NodePtr<K, V> n);
    
    template <typename K, typename V>
    void deleteCase4(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* son = sibling(n);
        if ((n->parent->color == RED) && (son->color == BLACK) &&
            (son->left->color == BLACK) && (son->right->color == BLACK))
        {
            son->color = RED;
            n->parent->color = BLACK;
        }
        else
            deleteCase5(n);
    }
    
    template <typename K, typename V>
    void deleteCase6(treeDetails::NodePtr<K, V> n);
    
    template <typename K, typename V>
    void deleteCase5(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* son = sibling(n);
        if (son->color == BLACK)
        {
            if ((n == n->parent->left) && (son->right->color == BLACK) && (son->left->color == RED))
            {
                son->color = RED;
                son->left->color = BLACK;
                rotateRight(son);
            }
            else if ((n == n->parent->right) && (son->left->color == BLACK) && (son->right->color == RED))
            {
                son->color = RED;
                son->right->color = BLACK;
                rotateLeft(son);
            }
        }
        deleteCase6(n);
    }
    
    template <typename K, typename V>
    void deleteCase6(treeDetails::NodePtr<K, V> n)
    {
        treeDetails::Node<K, V>* son = sibling(n);
        son->color = n->parent->color;
        n->parent->color = BLACK;
        if (n == n->parent->left)
        {
            son->right->color = BLACK;
            rotateLeft(n->parent);
        }
        else
        {
            son->left->color = BLACK;
            rotateRight(n->parent);
        }
    }
}

#endif