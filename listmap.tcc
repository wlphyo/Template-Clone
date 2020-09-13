// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $
/*William Phyo (wlphyo@ucsc.edu)
Yuance Lin (ylin198@ucsc.edu)*/
#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//

template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   while(anchor() != anchor()->next) this->erase(this->begin());
   DEBUGF ('l', reinterpret_cast<const void*> (this));
}

//
// iterator listmap::insert (const value_type&)
//

template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   less_t lessTemp;
   listmap<key_t,mapped_t,less_t>::iterator it;
   for(it = begin();it!=end() && lessTemp(it->first,pair.first);++it)
   {}
   node* temp;
   if(it!=end() && !lessTemp(pair.first,it->first))
   {
       it->second = pair.second;
       return it;
   }
   temp = new node(it.where,it.where->prev,pair);
   temp->prev->next = temp;
   temp->next->prev = temp;
   return iterator(temp);
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   listmap<key_t,mapped_t,less_t>::iterator it;
   for(it=begin();it!=end();++it)
   {
      if(it->first == that) return it;
   }
   return it;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   listmap<key_t,mapped_t,less_t>::iterator it =position;
    ++it;
   position.erase();
  
   return it;
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>::iterator::erase() {
   if(where)
   {
      where->prev->next = where->next;
      where->next->prev = where->prev;
      delete where;
   }
}


//helper functions
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t> listmap<key_t,mapped_t,less_t>::collect_k(const key_type& val)
{
    listmap temp;
   for(auto it=begin();it!=end();++it)
   {
        if(it->second == val)   
        {
           temp.insert(*it);
        }
    }
    return temp;
}

