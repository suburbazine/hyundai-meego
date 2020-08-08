#include "ctree.h"
#include "util.h"

CTreeNode::CTreeNode(QString nodeName, int numChildren):
m_nChildren(numChildren),
m_pLeftChild(0),
m_pRightSibling(0),
m_pParent(0),
m_pLeftSibling(0)
{
    m_tFolderIndex.m_strFileFolderName = nodeName;
    m_tFolderIndex.m_nIndex            = 0;
}

CTree::CTree(int nMaxFolders, int nMaxFilePtrs):
m_pRoot(0),
m_pFolderList(0),
m_ppFilePtrList(0),
m_nMaxFolders(nMaxFolders),
m_nMaxFilePtrs(nMaxFilePtrs)
{
    m_pFolderList   = new CTreeNode[nMaxFolders];
    m_ppFilePtrList = new CTreeNode*[nMaxFilePtrs];

    memset(m_ppFilePtrList, 0x00, sizeof(CTreeNode*)*nMaxFilePtrs);
}

CTree::~CTree()
{
    MC_LOG << "~CTree()" << LOG_ENDL;
    
    if(m_pFolderList) {
        delete m_pFolderList;
    }
    
    if(m_ppFilePtrList) {
        delete m_ppFilePtrList;
    }
}
    
bool CTree::IsEmpty()
{
    return false;
}

void CTree::SetRoot(CTreeNode* node)
{
    m_pRoot = node;
    
    Q_UNUSED(node);
}

void CTree::AttachChildSubtree(CTreeNode& childTree)
{
    Q_UNUSED(childTree);
}

void CTree::AttachChild(CTreeNode &subTree, CTreeNode &newItem)
{
    Q_UNUSED(subTree);
    Q_UNUSED(newItem);
}

void CTree::AttachChildSubtree(CTreeNode &subTree, CTreeNode &childTree)
{
    Q_UNUSED(subTree);
    Q_UNUSED(childTree);
}

void CTree::DeleteChild(const QString &fileFolderName)
{
    Q_UNUSED(fileFolderName);
}

CTreeNode CTree::ChildSubtree(const QString &fileFolderName)
{
    Q_UNUSED(fileFolderName);
    return CTreeNode(0, 0);
}

void CTree::PreorderTraverse()
{
}

void CTree::InorderTraverse()
{
}

void CTree::PostorderTraverse()
{
}

void CTree::BreadthFirstTraversal()
{
}

CTreeNode* CTree::GetNextFolder(const QString &fileFolderName)
{
    Q_UNUSED(fileFolderName);
    return 0;
}

QStringList CTree::GetNextFolderList(const QString &fileFolderName)
{
    Q_UNUSED(fileFolderName);
    return QStringList();
}

CTreeNode* CTree::GetPreviousFolder(const QString &fileFolderName)
{
    Q_UNUSED(fileFolderName);
    return 0;
}

QStringList CTree::GetPreviousFolderList(const QString &fileFolderName)
{
    Q_UNUSED(fileFolderName);
    return QStringList();
}

CTreeNode* CTree::GetFolder(int nIndex)
{
    if(nIndex < 0 || nIndex >= m_nMaxFolders) return 0;

    return &m_pFolderList[nIndex];
}

CTreeNode* CTree::GetRightMostFolder(CTreeNode *pNode)
{
    if(!pNode) return 0;

    CTreeNode *p = pNode;
    
    while(p) {
        if(p->m_pRightSibling == 0) return p;
        p = p->m_pRightSibling;
    }

    return p;
}

CTreeNode* CTree::GetFilePtr(int nIndex)
{
    if(nIndex < 0 || nIndex >= m_nMaxFilePtrs) return 0;

    return m_ppFilePtrList[nIndex];
}

void CTree::SetFilePtr(int nIndex, CTreeNode *pNode)
{
    if(nIndex < 0 || nIndex >= m_nMaxFilePtrs) return;

    m_ppFilePtrList[nIndex] = pNode;
}

