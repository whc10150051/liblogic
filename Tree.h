#if !defined(AFX_299B20D1_CDAA_448a_BCE4_C0E12F257D46_)
#define AFX_299B20D1_CDAA_448a_BCE4_C0E12F257D46_
#include <IOSTREAM>
using namespace std;

#include "LinkedList.h"

#define  max(x,y) ((x) > (y) ? (x) : (y))

template<typename T>
class CTreeNode
{
public:
  CTreeNode(T Elem)
    :m_Elem(Elem), m_nHeight(1), m_lpParent(NULL), 
    m_lpLeft(NULL), m_lpRight(NULL), m_lpNext(NULL)
  {
  }
  ~CTreeNode()
  {
    CTreeNode<T> *lpCurNode = this->m_lpNext;
    while(lpCurNode !=  NULL)
    {
      CTreeNode<T> *lpNextNode = lpCurNode->m_lpNext;
      delete (void*)lpCurNode; 
      lpCurNode = lpNextNode;
    }
  }
  
  
  T m_Elem;    //数据元素
  int m_nHeight; //树的高度  
  CTreeNode<T> *m_lpParent;  //父结点
  CTreeNode<T> *m_lpLeft;    //左孩子
  CTreeNode<T> *m_lpRight;   //右孩子
  CTreeNode<T> *m_lpNext;    //链表
};

template<typename T>
class CTree  
{
public:
  CTree();
  virtual ~CTree();
  CTreeNode<T> * GetRoot();
  int GetCount();
  bool IsEmpty();
  void RemoveAll();
  void Insert(const int& newElem);
  void RemoveAt(const int& Elem);
  void RemoveAt(CTreeNode<T> *lpDelNode);
  CTreeNode<T> * FindNode(const int& Elem);
public:
  void _PreOrder(CTreeNode<T> *lpRoot);
  void _MidOrder(CTreeNode<T> *lpRoot);
  void _AftOrder(CTreeNode<T> *lpRoot);
  void _LayOrder(CTreeNode<T> *lpRoot);
private:
  void AdjustHeight(CTreeNode<T> *lpCurNode); 
  CTreeNode<T> * FindMin(CTreeNode<T> *lpRoot);
  int Height(CTreeNode<T> *lpNode);
  void RotateWithLeft(CTreeNode<T> *lpK1, CTreeNode<T> *lpK2);
  void RotateWithRight(CTreeNode<T> *lpK1, CTreeNode<T> *lpK2);
  void RemoveAtAll(CTreeNode<T> *lpDelNode);
private:
  CTreeNode<T> *m_lpRoot;
  int m_nCount;
};


template<typename T>
CTree<T>::CTree()
{
  m_lpRoot = NULL;
  m_nCount = 0;
}

template<typename T>
CTree<T>::~CTree()
{
  RemoveAll();
}

template<typename T>
CTreeNode<T>* CTree<T>::GetRoot()
{
  return m_lpRoot;
}

template<typename T>
int CTree<T>::GetCount()
{
  return m_nCount;
}

template<typename T>
bool CTree<T>::IsEmpty()
{
  return m_lpRoot == NULL ? true : false;
}

template<typename T>
void CTree<T>::RemoveAll()
{
  CStack<CTreeNode<T> *> stack;
  
  CTreeNode<T> *lpCurNode  = m_lpRoot;
  CTreeNode<T> *lpLastNode  = NULL;
  
  while(true)
  {
    while(lpCurNode != NULL)
    {
      stack.push(lpCurNode);
      lpCurNode = lpCurNode->m_lpLeft;
    }
    
    if (stack.Empty())
      break;
    
    lpCurNode = stack.top();
    stack.pop();
    if (lpCurNode->m_lpLeft ==  NULL && lpCurNode->m_lpRight == NULL)
    {
      lpLastNode = lpCurNode;
    }
    else if (lpCurNode->m_lpRight == NULL || 
      lpCurNode->m_lpRight == lpLastNode)
    {
      lpLastNode = lpCurNode;
      RemoveAt(lpCurNode);
      lpCurNode = NULL;
      continue;
    }
    else
    {
      stack.push(lpCurNode); 
      lpCurNode = lpCurNode->m_lpRight;
      continue;
    }
    
    CTreeNode<T> *lpDelNode = lpCurNode;
    lpCurNode = lpCurNode->m_lpRight;
    RemoveAtAll(lpDelNode);
  }
}

template<typename T>
void CTree<T>::Insert(const int& newElem)
{
  CTreeNode<T> *lpNewNode = new CTreeNode<T>(newElem);
  
  if (m_lpRoot == NULL)
  {
    m_lpRoot = lpNewNode;
    m_nCount++;
    return;
  }
  
  CTreeNode<T> *lpCurNode = m_lpRoot;
  
  while(lpCurNode != NULL)
  {
    if (newElem > lpCurNode->m_Elem)
    {
      if (lpCurNode->m_lpRight == NULL)
      {
        lpCurNode->m_lpRight = lpNewNode;
        break;
      }
      lpCurNode = lpCurNode->m_lpRight;
    }
    else if (newElem < lpCurNode->m_Elem)
    {
      if (lpCurNode->m_lpLeft == NULL)
      {
        lpCurNode->m_lpLeft = lpNewNode;
        break;
      }
      lpCurNode = lpCurNode->m_lpLeft;
    }
    else
    {
    /*
    1
    2  [2] 2  2  2 
    */
      lpNewNode->m_lpNext = lpCurNode->m_lpNext;
      lpCurNode->m_lpNext = lpNewNode;
      return;
    }
  }
  
  lpNewNode->m_lpParent = lpCurNode;
  m_nCount++;
  AdjustHeight(lpNewNode);
}

template<typename T>
void CTree<T>::RemoveAt(const int& Elem)
{
  CTreeNode<T> *lpDelNode = FindNode(Elem);
  if (lpDelNode == NULL)
    return;
  
  RemoveAt(lpDelNode);
}

template<typename T>
void CTree<T>::RemoveAt(CTreeNode<T> *lpDelNode)
{
  while(true)
  {
    if (lpDelNode->m_lpLeft == NULL && lpDelNode->m_lpRight == NULL)
    {
      CTreeNode<T> *lpParent = lpDelNode->m_lpParent;
      if (lpParent != NULL) 
      {
        if (lpDelNode == lpParent->m_lpLeft)
        {
          lpParent->m_lpLeft = NULL;
        }
        else
        {
          lpParent->m_lpRight = NULL;
        }
      }
      else 
      {
        m_lpRoot = NULL;
      }
      break;
    }
    else if (lpDelNode->m_lpLeft != NULL && lpDelNode->m_lpRight != NULL)
    {
      CTreeNode<T> *lpMinNode = FindMin(lpDelNode);
      
      lpDelNode->m_Elem = lpMinNode->m_Elem;
      
      CTreeNode<T> *lpList = lpDelNode->m_lpNext;
      lpDelNode->m_lpNext = lpMinNode->m_lpNext;
      lpMinNode->m_lpNext = lpList;
      
      lpDelNode = lpMinNode;
      continue;
    }
    else
    {
      CTreeNode<T> *lpParent = lpDelNode->m_lpParent;
      CTreeNode<T> *lpChild = NULL;
      if (lpDelNode->m_lpLeft != NULL)
        lpChild = lpDelNode->m_lpLeft;
      else
        lpChild = lpDelNode->m_lpRight;
      
      if (lpParent == NULL)
      {
        m_lpRoot = lpChild;
      }
      else
      {
        if (lpDelNode == lpParent->m_lpLeft)
          lpParent->m_lpLeft = lpChild;
        else
          lpParent->m_lpRight = lpChild;
      }
      
      lpChild->m_lpParent = lpParent;
      break;
    }
  }
  
  AdjustHeight(lpDelNode);
  delete lpDelNode;
  m_nCount--;
}

template<typename T>
void CTree<T>::RemoveAtAll(CTreeNode<T> *lpDelNode)
{
  while(true)
  {
    if (lpDelNode->m_lpLeft == NULL && lpDelNode->m_lpRight == NULL)
    {
      CTreeNode<T> *lpParent = lpDelNode->m_lpParent;
      if (lpParent != NULL) 
      {
        if (lpDelNode == lpParent->m_lpLeft)
        {
          lpParent->m_lpLeft = NULL;
        }
        else
        {
          lpParent->m_lpRight = NULL;
        }
      }
      else 
      {
        m_lpRoot = NULL;
      }
      break;
    }
    else if (lpDelNode->m_lpLeft != NULL && lpDelNode->m_lpRight != NULL)
    {
      CTreeNode<T> *lpMinNode = FindMin(lpDelNode);
      
      lpDelNode->m_Elem = lpMinNode->m_Elem;
      
      lpDelNode = lpMinNode;
      continue;
    }
    else
    {
      CTreeNode<T> *lpParent = lpDelNode->m_lpParent;
      CTreeNode<T> *lpChild = NULL;
      if (lpDelNode->m_lpLeft != NULL)
        lpChild = lpDelNode->m_lpLeft;
      else
        lpChild = lpDelNode->m_lpRight;
      
      if (lpParent == NULL)
      {
        m_lpRoot = lpChild;
      }
      else
      {
        if (lpDelNode == lpParent->m_lpLeft)
          lpParent->m_lpLeft = lpChild;
        else
          lpParent->m_lpRight = lpChild;
      }
      
      lpChild->m_lpParent = lpParent;
      break;
    }
  }
  
  delete lpDelNode;
  m_nCount--;
}

template<typename T>
CTreeNode<T> * CTree<T>::FindNode(const int& Elem)
{
  CTreeNode<T> *lpCurNode = m_lpRoot;
  while(lpCurNode != NULL)
  {
    if (Elem > lpCurNode->m_Elem)
      lpCurNode = lpCurNode->m_lpRight;
    else if (Elem < lpCurNode->m_Elem)
      lpCurNode = lpCurNode->m_lpLeft;
    else
      break;
  }
  
  return lpCurNode;
}

template<typename T>
void CTree<T>::_PreOrder(CTreeNode<T> *lpRoot)
{
  CStack<CTreeNode<T> *> stack;
  
  CTreeNode<T> *lpCurNode  = lpRoot;
  while(true)
  {
    while(lpCurNode != NULL)
    {
      cout << lpCurNode->m_Elem << ' ';
      stack.push(lpCurNode);
      lpCurNode = lpCurNode->m_lpLeft;
    }
    
    if (stack.Empty())
      break;
    
    lpCurNode = stack.top();
    stack.pop();
    
    lpCurNode = lpCurNode->m_lpRight;
  }
}

template<typename T>
void CTree<T>::_MidOrder(CTreeNode<T> *lpRoot)
{
  CStack<CTreeNode<T> *> stack;
  
  CTreeNode<T> *lpCurNode  = lpRoot;
  
  while(true)
  {
    while(lpCurNode != NULL)
    {
      stack.push(lpCurNode);
      lpCurNode = lpCurNode->m_lpLeft;
    }
    
    if (stack.Empty())
      break;
    
    lpCurNode = stack.top();
    stack.pop();
    cout << lpCurNode->m_Elem << ' ';
    
    lpCurNode = lpCurNode->m_lpRight;
  }
}

template<typename T>
void CTree<T>::_AftOrder(CTreeNode<T> *lpRoot)
{
  CStack<CTreeNode<T> *> stack;
  
  CTreeNode<T> *lpCurNode  = lpRoot;
  CTreeNode<T> *lpLastNode  = NULL;
  
  while(true)
  {
    while(lpCurNode != NULL)
    {
      stack.push(lpCurNode);
      lpCurNode = lpCurNode->m_lpLeft;
    }
    
    if (stack.Empty())
      break;
    
    lpCurNode = stack.top();
    stack.pop();
    if (lpCurNode->m_lpLeft ==  NULL && lpCurNode->m_lpRight == NULL)
    {
      lpLastNode = lpCurNode;
      cout << lpCurNode->m_Elem << ' ';
    }
    else if (lpCurNode->m_lpRight == NULL || 
      lpCurNode->m_lpRight == lpLastNode)
    {
      lpLastNode = lpCurNode;
      cout << lpCurNode->m_Elem << ' ';
      lpCurNode = NULL;
      continue;
    }
    else
    {
      stack.push(lpCurNode); 
    }
    
    
    lpCurNode = lpCurNode->m_lpRight;
  }
}


template<typename T>
void CTree<T>::_LayOrder(CTreeNode<T> *lpRoot)
{
  CTreeNode<T> *lpCurNode = lpRoot;
  CQueue<CTreeNode<T> *> queue;
  
  while(true)
  {
    cout << lpCurNode->m_Elem  << '[' << lpCurNode->m_nHeight << ']' << ' ';
    if (lpCurNode->m_lpLeft != NULL)
      queue.push(lpCurNode->m_lpLeft);
    if (lpCurNode->m_lpRight != NULL)
      queue.push(lpCurNode->m_lpRight);
    
    if (queue.Empty())
      break;
    
    lpCurNode = queue.top();
    queue.pop();
    
  }
}


template<typename T>
CTreeNode<T> * CTree<T>::FindMin(CTreeNode<T> *lpRoot)
{
  CTreeNode<T> *lpCurNode = lpRoot->m_lpRight;
  if (lpCurNode == NULL)
    return NULL;
  
  while(lpCurNode->m_lpLeft != NULL)
  {
    lpCurNode = lpCurNode->m_lpLeft;
  }
  return lpCurNode;
}

template<typename T>
void CTree<T>::AdjustHeight(CTreeNode<T> *lpCurNode)
{
  CTreeNode<T> *lpParent = lpCurNode->m_lpParent;
  
  //调整所有父结点的高度
  while(lpParent != NULL)
  {
    //调整高度差
    int nLeftHeight = Height(lpParent->m_lpLeft);
    int nRightHeight = Height(lpParent->m_lpRight);
    lpParent->m_nHeight = max(nLeftHeight, nRightHeight) + 1;
    
    //检查平衡
    if (abs(nLeftHeight - nRightHeight) >= 2) //不平衡
    {
      CTreeNode<T> *lpK1 = lpParent;
      CTreeNode<T> *lpK2 = NULL;
      CTreeNode<T> *lpK3 = NULL;
      
      //K1左边比较重
      if (nLeftHeight > nRightHeight)
        lpK2 = lpK1->m_lpLeft;
      else
        lpK2 = lpK1->m_lpRight;
      
      
      nLeftHeight = Height(lpK2->m_lpLeft);
      nRightHeight = Height(lpK2->m_lpRight);
      //K2左边比较重
      if (nLeftHeight > nRightHeight)
        lpK3 = lpK2->m_lpLeft;
      else
        lpK3 = lpK2->m_lpRight;
      
      if (lpK2 == lpK1->m_lpRight && 
        lpK3 == lpK2->m_lpRight)
      {
        RotateWithLeft(lpK1, lpK2);
      }
      else if (lpK2 == lpK1->m_lpLeft && 
        lpK3 == lpK2->m_lpLeft)
      {
        RotateWithRight(lpK1, lpK2);
      }
      else if (lpK2 == lpK1->m_lpLeft && 
        lpK3 == lpK2->m_lpRight)
      {
        RotateWithLeft(lpK2, lpK3);
        RotateWithRight(lpK1, lpK3);
      }
      else if (lpK2 == lpK1->m_lpRight && 
        lpK3 == lpK2->m_lpLeft)
      {
        RotateWithRight(lpK2, lpK3);
        RotateWithLeft(lpK1, lpK3);  
      }
    }
    
    lpParent = lpParent->m_lpParent;
  }
}

template<typename T>
int CTree<T>::Height(CTreeNode<T> *lpNode)
{
  return lpNode == NULL ? 0 : lpNode->m_nHeight;
}

template<typename T>
void CTree<T>::RotateWithLeft(CTreeNode<T> *lpK1, CTreeNode<T> *lpK2)
{
/*
        G                     G
     K1                     K2
   A     K2       ==>     K1  C   
       B   C             A  B 
  */
  CTreeNode<T> *lpA = lpK1->m_lpLeft;
  CTreeNode<T> *lpB = lpK2->m_lpLeft;
  CTreeNode<T> *lpC = lpK2->m_lpRight;
  CTreeNode<T> *lpG = lpK1->m_lpParent;
  
  if (lpB != NULL)
    lpB->m_lpParent = lpK1;
  
  if (lpG != NULL)
  {
    if (lpK1 == lpG->m_lpLeft)
      lpG->m_lpLeft = lpK2;
    else
      lpG->m_lpRight = lpK2;
  }
  else
    m_lpRoot = lpK2;
  
  lpK1->m_lpRight = lpB;
  lpK1->m_lpParent = lpK2;
  
  
  lpK2->m_lpLeft = lpK1;
  lpK2->m_lpParent = lpG;
  
  //调整K1，K2的高度
  lpK1->m_nHeight = max(Height(lpK1->m_lpLeft), Height(lpK1->m_lpRight)) + 1;
  lpK2->m_nHeight = max(Height(lpK2->m_lpLeft), Height(lpK2->m_lpRight)) + 1;
}

template<typename T>
void CTree<T>::RotateWithRight(CTreeNode<T> *lpK1, CTreeNode<T> *lpK2)
{
/*
         G                      G
       K1                    K2  
     K2   C       ==>      A    K1
    A   B                     B    C
  */
  CTreeNode<T> *lpA = lpK2->m_lpLeft;
  CTreeNode<T> *lpB = lpK2->m_lpRight;
  CTreeNode<T> *lpC = lpK1->m_lpRight;
  CTreeNode<T> *lpG = lpK1->m_lpParent;
  
  if (lpB != NULL)
    lpB->m_lpParent = lpK1;
  
  if (lpG != NULL)
  {
    if (lpK1 == lpG->m_lpLeft)
      lpG->m_lpLeft = lpK2;
    else
      lpG->m_lpRight = lpK2;
  }
  else
    m_lpRoot = lpK2;
  
  lpK1->m_lpLeft = lpB;
  lpK1->m_lpParent = lpK2;
  
  
  lpK2->m_lpRight = lpK1;
  lpK2->m_lpParent = lpG;
  
  //调整K1，K2的高度
  lpK1->m_nHeight = max(Height(lpK1->m_lpLeft), Height(lpK1->m_lpRight)) + 1;
  lpK2->m_nHeight = max(Height(lpK2->m_lpLeft), Height(lpK2->m_lpRight)) + 1;
}



#endif // !defined(AFX_299B20D1_CDAA_448a_BCE4_C0E12F257D46_)
