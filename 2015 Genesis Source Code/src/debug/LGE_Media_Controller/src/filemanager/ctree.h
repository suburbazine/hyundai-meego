#ifndef CTREE_H
#define CTREE_H

#include <QString>
#include <QStringList>
#include <QList>

#define CTREE_MAX_FOLDERS         10000
#define CTREE_MAX_FILES           10000

struct DirEntryIndex{
    QString     m_strFileFolderName;
    int         m_nIndex;
};

class CTreeNode 
{
public:
    //
    DirEntryIndex           m_tFolderIndex;     // This will be current node(current folder)
    QList<DirEntryIndex>    m_tFileList;        // Each element in this will be a file.
    int                     m_nChildren;        // Number of child folders.
    CTreeNode              *m_pLeftChild;       // left most child
    CTreeNode              *m_pRightSibling;    // right sibling
    CTreeNode              *m_pParent;          // parent
    CTreeNode              *m_pLeftSibling;     // left sibling

    //treenode constructor
    CTreeNode(QString nodeName = QString(), int numChildren = 0);
};

class CTree
{
public:
    CTree(int nMaxFolders = CTREE_MAX_FOLDERS, int nMaxFilePtrs = CTREE_MAX_FILES);
    ~CTree();
    
    bool            IsEmpty();
    void            SetRoot(CTreeNode *pNode);
    void            AttachChildSubtree(CTreeNode &childTree);
    void            AttachChild(CTreeNode &subTree, CTreeNode &newItem);
    void            AttachChildSubtree(CTreeNode &subTree, CTreeNode &childTree);
    void            DeleteChild(const QString &fileFolderName);
    CTreeNode       ChildSubtree(const QString &fileFolderName);
    void            PreorderTraverse();
    void            InorderTraverse();
    void            PostorderTraverse();
    void            BreadthFirstTraversal();
    CTreeNode*      GetNextFolder(const QString &fileFolderName);
    QStringList     GetNextFolderList(const QString &fileFolderName);
    CTreeNode*      GetPreviousFolder(const QString &fileFolderName);
    QStringList     GetPreviousFolderList(const QString &fileFolderName);

    CTreeNode*      GetFolder(int nIndex);
    CTreeNode*      GetRightMostFolder(CTreeNode *pNode);
    CTreeNode*      GetFilePtr(int nIndex);
    void            SetFilePtr(int nIndex, CTreeNode *pNode);

private:
    CTreeNode      *m_pRoot;    // root of tree
    CTreeNode      *m_pFolderList;
    CTreeNode     **m_ppFilePtrList;

    int             m_nMaxFolders;
    int             m_nMaxFilePtrs;
};

#endif // CTREE_H
