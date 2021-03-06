#if !defined(D35D2C21_B9E0_41b3_A54C_135A752AC3D5_)
#define D35D2C21_B9E0_41b3_A54C_135A752AC3D5_

#include <assert.h>

/* 链表节点 */
template<typename TYPE>
struct CNode
{
  CNode(TYPE Elem)
    :m_Elem(Elem), m_pNext(NULL), m_pPrev(NULL)
  {
  }
  
  TYPE m_Elem;     //数据元素
  CNode<TYPE> *m_pPrev; //前驱
  CNode<TYPE> *m_pNext; //后继
};

/* 链表 */
template<typename TYPE>
class CLinkedList  
{
public:
  class iterator
  {
    friend class CLinkedList<TYPE>;
  public:
    iterator()
    {
      m_pNode = NULL;
    }
    iterator(CNode<TYPE> *pNode)
    {
      m_pNode = pNode;
    }

    iterator operator++()
    {
      m_pNode = m_pNode->m_pNext;
      return m_pNode;
    }

    iterator operator++(int)
    {
      CNode<TYPE> *pTemp = m_pNode;
      m_pNode = m_pNode->m_pNext;
      return pTemp;
    }
    iterator operator--()
    {
      m_pNode = m_pNode->m_pPrev;
      return m_pNode;
    }
    
    iterator operator--(int)
    {
      CNode<TYPE> *pTemp = m_pNode;
      m_pNode = m_pNode->m_pPrev;
      return pTemp;
    }

    TYPE& operator* ()
    {
      return m_pNode->m_Elem;
    }

    bool operator!= (const iterator& obj)
    {
      return m_pNode != obj.m_pNode;
    }
    bool operator== (const iterator& obj)
    {
      return m_pNode == obj.m_pNode;
    }
  private:
    CNode<TYPE> *m_pNode;
  };
  iterator begin()
  {
    return m_pHead;
  }
  iterator end()
  {
    return NULL;
  }
public:
  CLinkedList();
  virtual ~CLinkedList();
  void Clear();
  inline int Length() const;  //表长
  inline TYPE GetAt(iterator pos) const;
  inline void SetAt(iterator pos, TYPE newElem);
  inline bool Insert(iterator pos, TYPE newElem);
  inline bool RemoveTail();
  inline bool RemoveHead();
  inline bool RemoveAt(iterator pos);
  inline bool Empty() const;
  inline void AddTail(TYPE newElem);
  inline void AddHead(TYPE newElem);
public:
  inline iterator GetTail();
  inline iterator GetHead();
private:
  CNode<TYPE> *m_pHead;  //头结点
  CNode<TYPE> *m_pTail;  //尾结点
  int    m_nLength;
};


template<typename TYPE>
inline int CLinkedList<TYPE>::Length() const
{
  return m_nLength;
}

template<typename TYPE>
inline bool  CLinkedList<TYPE>::Empty() const
{
  return m_pHead == NULL ? true : false;
}

template<typename TYPE>
CLinkedList<TYPE>::CLinkedList()
{
  m_pHead = NULL;
  m_pTail = NULL;
  m_nLength = 0;
}

template<typename TYPE>
CLinkedList<TYPE>::~CLinkedList()
{
  Clear();
}


template<typename TYPE>
TYPE CLinkedList<TYPE>::GetAt(iterator pos) const
{
  return *pos;
}

template<typename TYPE>
void CLinkedList<TYPE>::SetAt(iterator pos, TYPE newElem)
{
  *pos = newElem;
}

/* O(1) 这里用了迭代器当位置点方便遍历的时候插入 */
template<typename TYPE>
bool CLinkedList<TYPE>::Insert(iterator pos, TYPE newElem)
{
  CNode<TYPE> *pNewNode = new CNode<TYPE>(newElem);
  CNode<TYPE> *pPrev = pos.m_pNode;
  CNode<TYPE> *pNext = pPrev->m_pNext;
  /*
     1 2 3 [6] 4 5 
     3.next = 6
     4.prev = 6
     6.prev = 3
     6.next = 4

      1 2 3 4 5 [6]
  */
  pPrev->m_pNext = pNewNode;
  if (pNext != NULL)
    pNext->m_pPrev = pNewNode;
  pNewNode->m_pPrev = pPrev;
  pNewNode->m_pNext = pNext;
  m_nLength++;
  return 0;
}

template<typename TYPE>
bool CLinkedList<TYPE>::RemoveTail()
{
  assert(m_pHead != NULL);

   /* 1 2 [3] */
  CNode<TYPE> *pPrev = m_pTail->m_pPrev;
  if (pPrev != NULL)
    pPrev->m_pNext = NULL;
  else
    m_pHead = NULL;

  delete m_pTail;
  m_nLength--;
  m_pTail = pPrev;
  return 0;
}

template<typename TYPE>
bool CLinkedList<TYPE>::RemoveHead()
{
 
  assert(m_pHead != NULL);
  
  /* [1] 2 3 */
  CNode<TYPE> *pNext = m_pHead->m_pNext;
  if (pNext != NULL)
    pNext->m_pPrev = NULL;
  else
    m_pTail = NULL;
  
  delete m_pHead;
  m_nLength--;
  m_pHead = pNext;
  return 0;
}

template<typename TYPE>
bool CLinkedList<TYPE>::RemoveAt(CLinkedList<TYPE>::iterator pos)
{
  assert(m_pHead != NULL);

  CNode<TYPE> *pDelNode = pos.m_pNode;
  /*
    1 2 [3] 4 5 
    2.next = 4
    4.prev = 2

    [1] 2 3 4 5 

    1 2 3 4 [5]

    [1]
  */
  CNode<TYPE> *pPrev = pDelNode->m_pPrev;
  CNode<TYPE> *pNext = pDelNode->m_pNext;
  if (pPrev != NULL)
    pPrev->m_pNext = pNext;
  else
    m_pHead = pNext;

  if (pNext != NULL)
    pNext->m_pPrev = pPrev;
  else
    m_pTail = pPrev;

  delete pDelNode;
  m_nLength--;

  return 0;
}

template<typename TYPE>
void CLinkedList<TYPE>::Clear()
{
  CNode<TYPE> *pCurNode = m_pHead;
  CNode<TYPE> *pNextNode = NULL;
  while(pCurNode != NULL)
  {
    pNextNode = pCurNode->m_pNext;
    delete pCurNode;
    pCurNode = pNextNode;
  }
  m_pHead = m_pTail = NULL;
  m_nLength = 0;
}

/* O(1) */
template<typename TYPE>
void CLinkedList<TYPE>::AddTail(TYPE newElem)
{
  CNode<TYPE> *pNewNode = new CNode<TYPE>(newElem);

  /* 空表 */
  if (m_pHead == NULL)
  {
    m_pHead = m_pTail = pNewNode;
  }
  else 
  {
    /* 1 2 3 4 5 [6]  5.next = 6   6.prev = 5  tail = 6 */
    m_pTail->m_pNext = pNewNode;
    pNewNode->m_pPrev = m_pTail;
    m_pTail = pNewNode;
  }
  m_nLength++;
}

/* O(1) */
template<typename TYPE>
void CLinkedList<TYPE>::AddHead(TYPE newElem)
{
  CNode<TYPE> *pNewNode = new CNode<TYPE>(newElem);
  
  //空表
  if (m_pHead == NULL)
  {
    m_pHead = m_pTail = pNewNode;
  }
  else 
  {
    /* [6] 1 2 3 4 5 1.prev = 6    6.next = 1  head = 6 */ 
    m_pHead->m_pPrev = pNewNode;
    pNewNode->m_pNext = m_pHead;
    m_pHead = pNewNode;
  }
  m_nLength++;
}

template<typename TYPE>
CLinkedList<TYPE>::iterator CLinkedList<TYPE>::GetTail()
{
  return m_pTail;
}

template<typename TYPE>
CLinkedList<TYPE>::iterator CLinkedList<TYPE>::GetHead()
{
  return m_pHead;
}

template<typename T>
class CStack :private CLinkedList<T>
{
public:
  void push(const T& Elem)
  {
    AddHead(Elem);
  }
  void pop()
  {
    RemoveHead();
  }
  T top()
  {
    return *GetHead();
  }
public:
  CLinkedList<T>::Clear;
  CLinkedList<T>::Empty;
  CLinkedList<T>::Length;
};

#endif // !defined(D35D2C21_B9E0_41b3_A54C_135A752AC3D5_)

