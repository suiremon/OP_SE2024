#pragma once
#include <utility>
#include <memory>
#include <iostream>
#include <vector>
#include <limits>

struct inOrder{};
struct preOrder{};
struct postOrder{};

template<class _Key, class _Compare = std::less<_Key>, class _Allocator = std::allocator<_Key>, class _Order = inOrder>
class BST {
public:
    class BSTNode {
    public:
        _Key _value;
        size_t height;
        BSTNode* left;
        BSTNode* right;
        BSTNode* parent;

        explicit BSTNode(_Key value) : _value(value), height(1), left(nullptr), right(nullptr), parent(nullptr) {}
        ~BSTNode() = default;
        BSTNode& operator=(const BSTNode& other) {
            if (this == &other) {
                return *this;
            }
            _value = other._value;
            height = other.height;
            left = other.left;
            right = other.right;
            parent = other.parent;
            return *this;
        }
        bool operator==(const BSTNode& other) {
            if (_value == other._value && height == other.height
                && left == other.left && right == other.right && parent == other.parent) {
                return true;
            }
            return false;
        }
        bool operator!=(const BSTNode& other) {
            return this != other;
        }
    };
    class iterator {
    private:
        friend class BST;

    public:
        BSTNode* _node;
        bool is_end;

        iterator& inorder_increment() {
            BSTNode* from_node = _node;
            if (is_end) {
                return *this;
            }
            if (_node->right) {
                _node = _node->right;
                while (_node->left)
                    _node = _node->left;

                return *this;
            }

            bool from_right = true;

            while (_node->parent) {
                if (_node == _node->parent->left) {
                    from_right = false;
                    break;
                }
                _node = _node->parent;

            }

            if (_node->parent && _node == _node->parent->left) {
                _node = _node->parent;

                return *this;
            } else if (from_right) {
                is_end = true;
                _node = from_node;
                return *this;
            }

            if (_node->right) {
                _node = _node->right;
                while (_node->left)
                    _node = _node->left;
            }

            return *this;
        }

        iterator& preorder_increment() {
            if (is_end) {
                return *this;
            }
            if (_node->left != nullptr) {
                _node = _node->left;
                return *this;
            } else if (_node->right != nullptr) {
                _node = _node->right;
                return *this;
            } else {
                is_end = true;
                BSTNode* current_copy = _node;
                while (current_copy->parent != nullptr) {
                    if (current_copy == current_copy->parent->left && current_copy->parent->right != nullptr) {
                        _node = current_copy->parent->right;
                        is_end = false;
                        return *this;
                    }
                    current_copy = current_copy->parent;
                }
            }
            return *this;
        };

        iterator& postorder_increment() {
            if (!_node->parent) {
                is_end = true;
                return *this;
            }
            if (_node->parent && _node == _node->parent->right) {
                _node = _node->parent;

                return *this;
            }
            if (_node->parent)
                _node = _node->parent;
            if (_node->right) {
                _node = _node->right;
                while (_node->left || _node->right) {
                    if (_node->left)
                        _node = _node->left;
                    else
                        _node = _node->right;
                }
            }
            return *this;
        };

        iterator& inorder_decrement() {
            if (is_end) {
                is_end = false;
                return *this;
            }
            if (_node->left) {
                _node = _node->left;
                while (_node->right) {
                    _node = _node->right;
                }
                return *this;
            }

            while (_node->parent && _node != _node->parent->right) {
                _node = _node->parent;
            }

            if (_node->parent && _node == _node->parent->right)
                _node = _node->parent;

            return *this;
        };

        iterator& preorder_decrement() {
            if (is_end) {
                is_end = false;
                return *this;
            }
            if (_node->parent != nullptr) {
                if (_node == _node->parent->right && _node->parent->left != nullptr) {
                    _node = _node->parent->left;
                    while (_node->right || _node->left) {
                        if (_node->right != nullptr) {
                            _node = _node->right;
                        } else {
                            _node = _node->left;
                        }
                    }
                    return *this;
                } else {
                    _node = _node->parent;
                    return *this;
                }
            }
            return *this;
        };

        iterator& postorder_decrement() {
            if (is_end) {
                is_end = false;
                return *this;
            }
            if (_node->right) {
                _node = _node->right;
                return *this;
            }
            if (_node->left) {
                _node = _node->left;
                return *this;
            }

            while (_node->parent && !(_node == _node->parent->right && _node->parent->left)) {
                _node = _node->parent;
            }

            if (_node->parent && _node->parent->left) {
                _node = _node->parent->left;
            }
            return *this;
        };

    public:
        typedef _Order type;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef const _Key value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const value_type* pointer;
        typedef const value_type& reference;

        iterator() : _node(nullptr), is_end(false) {};

        explicit iterator(BSTNode* node) : _node(node) {
            if (node) {
                is_end = false;
            } else
                is_end = true;
        };

        iterator(const iterator& iter) : _node(iter._node), is_end(iter.is_end) {};

        iterator& operator=(const iterator& other) {
            if (this != &other) {
                _node = other._node;
                is_end = other.is_end;
            }
            return *this;
        }

        bool operator==(const iterator& other) const {
            return (_node == other._node) && (is_end == other.is_end);
        }

        bool operator!=(const iterator& other) const {
            return *this != other;
        }

        const _Key& operator*() {
            return _node->data;
        }

        iterator& operator++() {
            if (std::is_same_v<inOrder, type>)
                return inorder_increment();
            else if (std::is_same_v<preOrder, type>)
                return preorder_increment();
            else if (std::is_same_v<postOrder, type>)
                return postorder_increment();
        }

        iterator operator++(int) {
            iterator previous = *this;
            this->operator++();
            return previous;
        }

        iterator& operator--() {
            if (std::is_same_v<inOrder, type>)
                return inorder_decrement();
            else if (std::is_same_v<preOrder, type>)
                return preorder_decrement();
            else if (std::is_same_v<postOrder, type>)
                return postorder_decrement();
        }

        iterator operator--(int) {
            iterator previous = *this;
            this->operator--();
            return previous;
        }

    };

public:
    using key_type = _Key;
    using value_type = _Key;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = _Compare;
    using value_compare = _Compare;
    using allocator_type = _Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<_Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<_Allocator>::const_pointer;
    using iterator = iterator;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = BSTNode;
    using insert_return_type = node_type;
    using NodeAllocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<BSTNode>;
private:
    NodeAllocator nodeAllocator;
    _Allocator bstAllocator;
    _Compare comp;
    BSTNode* _root;
    size_t _size;
    BSTNode* allocateNode(const key_type& key, const value_type& value) {
        BSTNode* node = nodeAllocator.allocate(1);
        nodeAllocator.construct(node, key, value);
        return node;
    }

    void deallocateNode(BSTNode*& node) {
        --_size;
        delete node;
//        nodeAllocator.destroy(node);
//        nodeAllocator.deallocate(node, 1);
    }

    static size_t height(BSTNode *temp) {
        if (!temp) {
            return 0;
        }
        return temp->height;
    }

    BSTNode *updateHeight(BSTNode *node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
        return node;
    }

    key_type heightDiff(BSTNode *n) {
        return height(n->left) - height(n->right);
    }

    BSTNode *balance(BSTNode *curr) {
        long long bal_factor = heightDiff(curr);
        if (bal_factor > 1) {
            if (heightDiff(curr->left) > 0)
                curr = leftRotate(curr);
            else {
                curr = leftRotate(curr);
                curr = rightRotate(curr);
            }
        } else if (bal_factor < -1) {
            if (heightDiff(curr->right) > 0) {
                curr = rightRotate(curr);
                curr = leftRotate(curr);
            } else
                curr = rightRotate(curr);
        }
        return curr;
    }

    BSTNode *insert(BSTNode *root, const value_type& value) {
        if (root == nullptr) {
            ++_size;
            return new BSTNode(value);
        }
        if (_Compare()(root->_value, value)) {
            root->right = insert(root->right, value);
            root->right->parent = root;
        } else {
            root->left = insert(root->left, value);
            root->left->parent = root;
        }
        root = updateHeight(root);
        long long balance_factor = heightDiff(root);
        if (balance_factor > 1) {
            if (_Compare()(root->left->_value, value)) {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
            return rightRotate(root);
        }
        if (balance_factor < -1) {
            if (_Compare()(root->right->_value, value)) {
                return leftRotate(root);
            }
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    BSTNode *insert(BSTNode *root, const value_type& value, BSTNode* parent, BSTNode* leftChild) {
        if (root == nullptr) {
            ++_size;
            return new BSTNode(value);
        }
        if (_Compare()(root->_value, value)) {
            root->right = insert(root->right, value, parent, leftChild);
            root->right->parent = root;
        } else {
            root->left = insert(root->left, value, parent, leftChild);
            root->left->parent = root;
        }
        root = updateHeight(root);
        long long balance_factor = heightDiff(root);
        if (balance_factor > 1) {
            if (_Compare()(root->left->_value, value)) {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
            return rightRotate(root);
        }
        if (balance_factor < -1) {
            if (_Compare()(root->right->_value, value)) {
                return leftRotate(root);
            }
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    BSTNode* remove(BSTNode *node, int value) {
        if (node == nullptr) {
            return nullptr;
        }
        if (_Compare()(value, node->_value)) {
            node->left = remove(node->left, value);
        } else if (_Compare()(node->_value, value)) {
            node->right = remove(node->right, value);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                BSTNode *temp = node->left ?
                                node->left :
                                node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else
                    *node = *temp;
                free(temp);
            } else {
                BSTNode *temp = findMin(node->right);
                node->_value = temp->_value;
                node->right = remove(node->right, temp->_value);
            }
        }
        if (node == nullptr)
            return node;
        node->height = 1 + std::max(height(node->left), height(node->right));
        node = balance(node);
        --_size;
        return node;
    }

    BSTNode* extractNode(BSTNode* root, int value) {
        if (root == nullptr) {
            return nullptr;
        }
        if (_Compare()(value, root->_value)) {
            root->left = extractNode(root->left, value);
        } else if (_Compare()(value, root->_value)) {
            root->right = extractNode(root->right, value);
        } else {
            if (root->left == nullptr) {
                BSTNode* temp = root->right;
                root->right = nullptr;
                return temp;
            }
            if (root->right == nullptr) {
                BSTNode* temp = root->left;
                root->left = nullptr;
                return temp;
            }
            BSTNode* temp = findMin(root->right);
            root->_value = temp->_value;
            root->right = extractNode(root->right, temp->_value);
        }
        if (root == nullptr) {
            return root;
        }
        root = updateHeight(root);
        long long balance_factor = heightDiff(root);
        if (balance_factor > 1) {
            if (_Compare()(root->left->_value, value)) {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
            return rightRotate(root);
        }
        if (balance_factor < -1) {
            if (_Compare()(root->right->_value, value)) {
                return leftRotate(root);
            }
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    BSTNode* mergeTrees(BSTNode* root1, BSTNode* root2) {
        if (!root1) return root2;
        if (!root2) return root1;
        if (comp(root1->_value, root2->_value)) {
            root1->left = mergeTrees(root1->left, root2);
            root1->right = mergeTrees(root1->right, nullptr);
        } else {
            root2->left = mergeTrees(root1, root2->left);
            root2->right = mergeTrees(nullptr, root2->right);
            root1 = root2;
        }
        root1->height = 1 + std::max(height(root1->left), height(root1->right));
        return balance(root1);
    }

    BSTNode* rightRotate(BSTNode* y) {
        BSTNode* x = y->left;
        BSTNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    BSTNode*  leftRotate(BSTNode* x) {
        BSTNode* y = x->right;
        BSTNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    BSTNode *find(BSTNode *x, value_type value) {
        if (x == nullptr || value == x->_value) {
            return x;
        }
        if (_Compare()(value, x->_value)) {
            return find(x->left, value);
        }
        return find(x->right, value);
    }

    bool exist(BSTNode* root, value_type value) {
        BSTNode *res = find(root, value);
        if (res == nullptr) {
            return false;
        }
        return true;
    }

    BSTNode* findMin(BSTNode* root) const {
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    BSTNode* findMax(BSTNode* root) const {
        while (root->right) {
            root = root->right;
        }
        return root;
    }
    void clear(BSTNode* node) {
        if (node->left) {
            clear(node->left);
        }
        if (node->right) {
            clear(node->right);
        }
        deallocateNode(node);
    }

    std::vector<_Key> getNodes() const {
        std::vector<_Key> result;
        inOrderTraversal<_Key>(_root, result);
        return result;
    }

    BSTNode* constructBST(const std::vector<_Key>& inorder, int start, int end) {
        if (start > end) {
            return nullptr;
        }
        int mid = (start + end) / 2;
        auto* root = new BSTNode(inorder[mid]);
        root->left = constructBST(inorder, start, mid - 1);
        root->right = constructBST(inorder, mid + 1, end);
        return root;
    }

    void printInOrderTraversal(BSTNode *node) const {
        if (node != nullptr) {
            printInOrderTraversal(node->left);
            std::cout << node->_value << ' ';
            printInOrderTraversal(node->right);
        }
    }
public:
    BSTNode* getRoot() {
        return _root;
    }
    BST() : _root(nullptr), _size(0) {};
    ~BST() {
        if (_root)
            clear(_root);
    }
    BST& operator=(const BST& other) { //working
        if (this == &other) {
            return *this;
        }
        clear(_root);
        std::vector<_Key> nodes = other.getNodes();
        _root = constructBST(nodes, 0, nodes.size() - 1);
        return *this;
    }
    allocator_type get_allocator() const { //working
        return bstAllocator;
    }
    iterator begin() noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return iterator(findMin(_root));
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return iterator(findMax(findMin(_root)));
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return iterator(_root);
        }
        return iterator(nullptr);
    }
    const_iterator begin() const noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return const_iterator(findMin(_root));
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return const_iterator(findMax(findMin(_root)));
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return const_iterator(_root);
        }
        return const_iterator(nullptr);
    }
    iterator end() noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return iterator(findMax(_root)->right);
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return iterator(_root->parent);
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return iterator(findMax(findMin(_root))->right);
        }
        return iterator(nullptr);
    }
    const_iterator end() const noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return const_iterator(findMax(_root)->right);
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return const_iterator(_root->parent);
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return const_iterator(findMax(findMin(_root))->right);
        }
        return const_iterator(nullptr);
    }
    iterator rbegin() noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return iterator(findMax(_root));
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return iterator(_root);
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return iterator(findMax(findMin(_root)));
        }
        return iterator(nullptr);
    }
    const_iterator rbegin() const noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return const_iterator(findMax(_root));
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return const_iterator(_root);
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return const_iterator(findMax(findMin(_root)));
        }
        return const_iterator(nullptr);
    }
    iterator rend() noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return iterator(findMin(_root)->left);
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return iterator(findMax(findMin(_root))->right);
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return iterator(_root->parent);
        }
        return iterator(nullptr);
    }
    const_iterator rend() const noexcept { //working
        if constexpr (std::is_same_v<_Order, inOrder>) {
            return const_iterator(findMin(_root)->left);
        } else if constexpr (std::is_same_v<_Order, postOrder>) {
            return const_iterator(findMax(findMin(_root))->right);
        } else if constexpr (std::is_same_v<_Order, preOrder>) {
            return const_iterator(_root->parent);
        }
        return const_iterator(nullptr);
    }
    [[nodiscard]] bool empty() const noexcept { //working
        return (_root == nullptr);
    }
    [[nodiscard]] size_type size() const noexcept { //working
        return _size;
    }
    [[nodiscard]] size_type max_size() const noexcept { //working
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
    void clear() noexcept { //working
        clear(_root);
        _root = nullptr;
    }
    std::pair<iterator, bool> insert(const value_type& value) { //working
        if (!exist(_root, value)) {
            _root = insert(_root, value);
            return std::make_pair(iterator(_root), true);
        }
        return std::make_pair(iterator(_root), false);
    }
    iterator insert(const_iterator pos, const value_type& value) { //working
        if (pos._node == nullptr) {
            return insert(value);
        }
        BSTNode* parent = pos._node->parent;
        bool isLeftChild = (parent && parent->left == pos._node);
        BSTNode* newNode = insert(_root, value, parent, isLeftChild);
        return iterator(newNode);
    };
    iterator erase(iterator pos) {
        if (pos == end()) {
            return end();
        }
        BSTNode* nodeToDelete = pos._node;
        BSTNode* parent = nodeToDelete->parent;
        _root = remove(_root, nodeToDelete->_value);
        if (parent) {
            if (parent->left == nodeToDelete) {
                parent->left = _root;
            } else {
                parent->right = _root;
            }
        }
        return iterator(_root);
    };
    iterator erase(const_iterator first, const_iterator last) {
        for (auto i = first; i != last; ++i) {
            erase(i);
        }
        return iterator(_root);
    };
    size_type erase(const _Key& key) {
        if (!exist(_root, key)) {
            return 0;
        }
        _root = remove(_root, key);
        return 1;
    };
    void swap(BST& other) noexcept {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }
    node_type* extract(const_iterator position) {
        if (position == end()) {
            return nullptr;
        }
        BSTNode* temp = position;
        BSTNode* parent = temp->parent;
        bool isLeftChild = (parent && parent->left == temp);
        BSTNode* extractedNode = extractNode(_root, temp->_value);
        if (isLeftChild) {
            parent->left = extractedNode;
        } else {
            parent->right = extractedNode;
        }
        --_size;
        return extractedNode;
    };
    node_type* extract(const _Key& k) {
        BSTNode* position = find(_root, k);
        if (iterator(position) == end()) {
            return nullptr;
        }
        BSTNode* temp = position;
        BSTNode* parent = temp->parent;
        bool isLeftChild = (parent && parent->left == temp);
        BSTNode* extractedNode = extractNode(_root, temp->_value);
        if (isLeftChild) {
            parent->left = extractedNode;
        } else {
            parent->right = extractedNode;
        }
        --_size;
        return extractedNode;
    };

    void merge(BST& source) {
        _root = mergeTrees(_root, source._root);
        source._root = nullptr;
    };

    size_type count(const _Key& key) {
        if (exist(_root, key)) {
            return 1;
        }
        return 0;
    };

    iterator find( const _Key& key ) {
        return iterator(find(_root, key));
    };
    const_iterator find( const _Key& key ) const {
        return const_iterator(find(_root, key));
    };
    bool contains( const _Key& key ) {
        return exist(_root, key);
    };
    std::pair<iterator, iterator> equal_range( const _Key& key ) {
        if (exist(_root, key)) {
            auto curr = find(_root, key);
            return std::make_pair(curr, ++curr);
        }
        auto curr = find(_root, key);
        return std::make_pair(--curr, ++curr);
    };
    std::pair<const_iterator, const_iterator> equal_range( const _Key& key ) const {
        if (exist(_root, key)) {
            auto curr = const_iterator(find(_root, key));
            return std::make_pair(curr, ++curr);
        }
        auto curr = const_iterator(find(_root, key));
        return std::make_pair(--curr, ++curr);
    };
    iterator lower_bound( const _Key& key ) {
        if (exist(_root, key)) {
            return iterator(find(_root, key));
        }
        return iterator(++find(key));
    };
    const_iterator lower_bound( const _Key& key ) const{
        if (exist(_root, key)) {
            return const_iterator(find(_root, key));
        }
        return const_iterator(++find(key));
    };
    iterator upper_bound( const _Key& key ) {
        return iterator(++find(key));
    };
    const_iterator upper_bound( const _Key& key ) const {
        return const_iterator(++find(key));
    };
    key_compare key_comp() const {
        return comp;
    };
    value_compare value_comp() const{
        return comp;
    }
    bool operator==(const BST& rhs) {
        if (_size != rhs._size) {
            return false;
        }
        iterator j = rhs.begin();
        for (iterator i = begin(); i != end(); ++i) {
            if (i._node != j._node) {
                return false;
            }
            ++j;
        }
        return true;
    };
    std::__detail::__synth3way_t<_Key> operator<=>(const BST& rhs) {
        return std::lexicographical_compare_three_way(begin(), end(),
                                                      rhs.begin(), rhs.end(), std::__detail::__synth3way_t<_Key>());
    };
    static void swap(BST& lhs, BST& rhs ) noexcept;
    template<class Pred>
    size_type erase_if(BST& c, Pred pred );
    void printInOrderTraversal() {
        printInOrderTraversal(_root);
    };
};

template <class _Key, class _Compare, class _Allocator, class _Order>
void BST<_Key, _Compare, _Allocator, _Order>::swap(BST& lhs, BST& rhs) noexcept {
    lhs.swap(rhs);
}

template <class _Key, class _Compare, class _Allocator, class _Order>
template <class Pred>
typename BST<_Key, _Compare, _Allocator, _Order>::size_type BST<_Key, _Compare, _Allocator, _Order>::erase_if(BST& c, Pred pred) {
    auto old_size = c.size();
    for (auto first = c.begin(), last = c.end(); first != last;) {
        if (pred(*first))
            first = c.erase(first);
        else
            ++first;
    }
    return old_size - c.size();
}
