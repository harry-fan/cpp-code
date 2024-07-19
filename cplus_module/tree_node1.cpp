//
// Created by Administrator on 2024/6/5.
//

#include <iostream>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* insert(TreeNode* root, int val) {
    if (root == nullptr) {
        return new TreeNode(val);
    }
    if (val < root->val) {
        root->left = insert(root->left, val);
    } else {
        root->right = insert(root->right, val);
    }
    return root;
}

// 前序遍历
void preOrder(TreeNode* root) {
    if (nullptr == root) {
        return;
    }
    std::cout << root->val << " ";
    preOrder(root->left);
    preOrder(root->right);
}

// 中序遍历
void midOrder(TreeNode* root) {
    if (nullptr == root) {
        return;
    }
    preOrder(root->left);
    std::cout << root->val << " ";
    preOrder(root->right);
}

// 后续便利
void postOrder(TreeNode* root) {
    if (nullptr == root) {
        return;
    }
    preOrder(root->left);
    preOrder(root->right);
    std::cout << root->val << " ";
}

int main() {
    // 构建一个平衡二叉树
    TreeNode* root = nullptr;
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 3);
    root = insert(root, 7);
    root = insert(root, 12);
    root = insert(root, 17);

    std::cout << "前序遍历结果：";
    preOrder(root);
    std::cout << std::endl;

    std::cout << "中序遍历结果：";
    midOrder(root);
    std::cout << std::endl;

    std::cout << "后序遍历结果：";
    postOrder(root);
    std::cout << std::endl;

    return 0;
}