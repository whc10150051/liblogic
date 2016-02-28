#if !defined(AFX_HASHMAP1_H__9A01B4A2_1F59_465C_8529_43F453D1EFCD__INCLUDED_)
#define AFX_HASHMAP1_H__9A01B4A2_1F59_465C_8529_43F453D1EFCD__INCLUDED_

#include <string.h>
#define HASH_MAP_SIZE sizeof(CHashMapNode<KEY,VALUE>*)
inline int Hash(int Key)
{
  return Key;
}

inline int Hash(const char *Key)
{
  int nKey = 0;
  int nLen = strlen(Key);
  for (int i = 0; i < nLen; i++)
    nKey += Key[i];

  return nKey;
}

template<typename KEY, typename VALUE>
struct CHashMapNode
{
  CHashMapNode()
  {
    m_lpNext = NULL;
  }
  
  KEY    m_Key;      
  VALUE  m_Value;
  CHashMapNode<KEY,VALUE> *m_lpNext;  /* 链表下一个 */
};

template<typename KEY, typename VALUE>
class CHashMap  
{
public:
	CHashMap();
	virtual ~CHashMap();
public:
  void InitHashMap(int nSize);
  void hset(KEY key, VALUE Value);
  void hdel(KEY key, VALUE Value);
  bool hget(KEY key,  VALUE& Value);
private:
  CHashMapNode<KEY,VALUE> * GetNodeAt(KEY nKey, unsigned int& uHash);
private:
  CHashMapNode<KEY,VALUE> **m_lpMap;
  int           m_nTableSize;
  int           m_nCount;
};

template<typename KEY, typename VALUE>
CHashMap<KEY, VALUE>::CHashMap()
{
  m_lpMap = NULL;
  m_nTableSize = 0;
  m_nCount = 0;
}

template<typename KEY, typename VALUE>
CHashMap<KEY, VALUE>::~CHashMap()
{
  
}

template<typename KEY, typename VALUE>
void CHashMap<KEY, VALUE>::InitHashMap(int nSize = 7)
{
  m_nTableSize = nSize;
  m_lpMap = new CHashMapNode<KEY,VALUE>*[nSize];
  memset(m_lpMap, 0, HASH_MAP_SIZE * nSize);
}

template<typename KEY, typename VALUE>
void CHashMap<KEY, VALUE>::hset(KEY key, VALUE Value)
{
  unsigned int uHash;
  CHashMapNode<KEY,VALUE> *lpNewNode = new CHashMapNode<KEY,VALUE>();
  CHashMapNode<KEY,VALUE> *lpFindNode = GetNodeAt(key, uHash);
  if (lpFindNode == NULL)
  {
    lpNewNode->m_Key = key;
    lpNewNode->m_Value = Value;

    /* 哈希表项为NULL */
    if (m_lpMap[uHash] == NULL)
    {
      m_lpMap[uHash] = lpNewNode;
    }
    else
    {
      /* 插入头部  1 [4] 2 3 */
      lpNewNode->m_lpNext = m_lpMap[uHash]->m_lpNext;
      m_lpMap[uHash]->m_lpNext = lpNewNode;
    }
    m_nCount++;
  }
  else
  {
    lpFindNode->m_Value = Value;
  }
}

template<typename KEY, typename VALUE>
void CHashMap<KEY, VALUE>::hdel(KEY key, VALUE Value)
{
}

template<typename KEY, typename VALUE>
bool CHashMap<KEY, VALUE>::hget(KEY key,  VALUE& Value)
{
  unsigned int uHash;
  CHashMapNode<KEY,VALUE> *lpNode = GetNodeAt(key, uHash);
  if (lpNode == NULL)
    return false;

  Value = lpNode->m_Value;
  return true;
}

template<typename KEY, typename VALUE>
CHashMapNode<KEY,VALUE> * CHashMap<KEY, VALUE>::GetNodeAt(KEY nKey, unsigned int& uHash)
{
  uHash = Hash(nKey) * 4 % m_nTableSize; /* 求哈希值 */

  /* 查询 */
  CHashMapNode<KEY,VALUE> *lpFind = m_lpMap[uHash];
  if (lpFind == NULL)
  {
    return NULL; /* 没有找到 */
  }

  CHashMapNode<KEY,VALUE> *lpCurNode = lpFind;
  /* 查询链表结点 */
  while(lpCurNode != NULL)
  {
    if (lpCurNode->m_Key == nKey) /* 找到 */
      return lpCurNode;

    lpCurNode = lpCurNode->m_lpNext;
  }
  return NULL;
}


#endif // !defined(AFX_HASHMAP1_H__9A01B4A2_1F59_465C_8529_43F453D1EFCD__INCLUDED_)
