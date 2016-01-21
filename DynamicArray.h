#if !defined(AFX_MYARRAY_H__5AC7F74B_82DE_4F4F_8B7C_0F4E20D4068A__INCLUDED_)
#define AFX_MYARRAY_H__5AC7F74B_82DE_4F4F_8B7C_0F4E20D4068A__INCLUDED_

#include <assert.h>
#include <stdio.h>

template<typename T>
class CDynamicArray  
{
public:
  CDynamicArray();
  virtual ~CDynamicArray();
  void Insert(int nIndex, T newElem);
  void Remove(int nIndex);
  void AddTail(T newElem);
  T& operator[] (int nIndex);
public:
  inline void Clear();
  inline void SetAt(int nIndex, T newElem);
public:
  inline int GetLength() const;  //表长
  inline T GetAt(int nIndex) const;
  inline bool IsEmpty() const;
private:
  void Alloc(int nOldSize, int nNewSize);
private:
  T   *m_pData;
  int  m_nLength;
  int  m_nSize;
};

/* O(1) */
template<typename T>
inline int CDynamicArray<T>::GetLength() const
{
  return m_nLength;
}

/* O(1) */
template<typename T>
inline bool CDynamicArray<T>::IsEmpty() const
{
  return m_pData == NULL ? true : false;
}


template<typename T>
CDynamicArray<T>::CDynamicArray()
{
  m_pData = NULL;
  m_nLength = 0;
  m_nSize = 0;
}

template<typename T>
CDynamicArray<T>::~CDynamicArray()
{
  Clear();
}

/* O(1) */
template<typename T>
T CDynamicArray<T>::GetAt(int nIndex) const
{
  assert(nIndex >= 0);
  assert(nIndex < m_nLength);

  return m_pData[nIndex];
}

/* O(1) */
template<typename T>
void CDynamicArray<T>::SetAt(int nIndex, T newElem)
{
  assert(nIndex >= 0);
  assert(nIndex < m_nLength);
  
  m_pData[nIndex] = newElem;
}

/* 
   最好O(1):线性表为空
   最坏O(n):尽量不要用插入 
*/
template<typename T>
void CDynamicArray<T>::Insert(int nIndex, T newElem)
{
  assert(nIndex >= 0);
  assert(nIndex < m_nLength);
  
  if (m_pData == NULL)
  {
    AddTail(newElem);
    return;
  }
  //空间不够
  else if (m_nLength >= m_nSize)
  {
    Alloc(m_nSize, m_nSize * 2);
  }

  for (int i = m_nLength; i > nIndex; i--)
  {
    m_pData[i] = m_pData[i - 1];
  }
  m_pData[nIndex] = newElem;
  m_nLength++;
}

/* 
   O(n),此处考虑到对象的浅拷贝问题所以没用memmove
*/
template<typename T>
void CDynamicArray<T>::Remove(int nIndex)
{
  assert(nIndex >= 0);
  assert(nIndex < m_nLength);

  if (m_pData == NULL)
    return;

  for (int i = nIndex; i < m_nLength - 1; i++)
  {
    m_pData[i] = m_pData[i + 1];
  }
  m_nLength--;
}

template<typename T>
void CDynamicArray<T>::Clear()
{
  if (m_pData != NULL)
  {
    delete[] m_pData;
    m_pData = NULL;
    m_nLength = 0;
    m_nSize = 0;
  }
}

/* O(n) */
template<typename T>
void CDynamicArray<T>::AddTail(T newElem)
{
  //空表
  if (m_pData == NULL)
  {
    Alloc(m_nSize, 1);
  }
  //空间不够
  else  if (m_nLength >= m_nSize)
  {
    Alloc(m_nSize, m_nSize * 2);
  }

  m_pData[m_nLength] = newElem;
  m_nLength++;
}

template<typename T>
void CDynamicArray<T>::Alloc(int nOldSize, int nNewSize)
{
  //申请新空间
  T *pNewData = new T[nNewSize];
  assert(pNewData != NULL);

  //拷贝数据
  if (m_pData != NULL)
  {
    for (int i = 0; i < nOldSize; i++)
    {
      pNewData[i] = m_pData[i];
    }
    delete[] m_pData;
  }

  m_pData = pNewData;
  m_nSize = nNewSize;
}

/* arr[n] */
template<typename T>
T& CDynamicArray<T>::operator[] (int nIndex)
{
  assert(nIndex >= 0);
  assert(nIndex < m_nLength);

  return m_pData[nIndex];
}

#endif
