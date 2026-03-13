#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"

namespace vishnyakov
{
  template< class T > class List;

  template< class T >
  class LIter
  {
    friend class List< T >;

  };

  template< class T >
  class LCIter
  {
    friend class List<T>;
  };

  template< class T >
  class List
  {
    Node * pseudo_;
    size_t size;
  };
}

#endif