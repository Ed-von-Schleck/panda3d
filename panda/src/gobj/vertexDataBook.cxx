// Filename: vertexDataBook.cxx
// Created by:  drose (16May07)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "vertexDataBook.h"
#include "reMutexHolder.h"

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
VertexDataBook::
VertexDataBook(size_t block_size) : _block_size(block_size) {
}

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::Destructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
VertexDataBook::
~VertexDataBook() {
}

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::count_total_page_size
//       Access: Published
//  Description: Returns the total size of all bytes owned by all
//               pages owned by this book.
////////////////////////////////////////////////////////////////////
size_t VertexDataBook::
count_total_page_size() const {
  MutexHolder holder(_lock);

  size_t total = 0;
  Pages::const_iterator pi;
  for (pi = _pages.begin(); pi != _pages.end(); ++pi) {
    total += (*pi)->get_max_size();
  }
  return total;
}

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::count_total_page_size
//       Access: Published
//  Description: Returns the total size of all bytes owned by all
//               pages owned by this book that have the indicated ram
//               class.
////////////////////////////////////////////////////////////////////
size_t VertexDataBook::
count_total_page_size(VertexDataPage::RamClass ram_class) const {
  MutexHolder holder(_lock);

  size_t total = 0;
  Pages::const_iterator pi;
  for (pi = _pages.begin(); pi != _pages.end(); ++pi) {
    if ((*pi)->get_ram_class() == ram_class) {
      total += (*pi)->get_max_size();
    }
  }
  return total;
}

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::count_allocated_size
//       Access: Published
//  Description: Returns the total size of all bytes allocated within
//               pages owned by this book.
////////////////////////////////////////////////////////////////////
size_t VertexDataBook::
count_allocated_size() const {
  MutexHolder holder(_lock);

  size_t total = 0;
  Pages::const_iterator pi;
  for (pi = _pages.begin(); pi != _pages.end(); ++pi) {
    total += (*pi)->get_total_size();
  }
  return total;
}

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::count_allocated_size
//       Access: Published
//  Description: Returns the total size of all bytes allocated within
//               pages owned by this book that have the indicated ram
//               class.
////////////////////////////////////////////////////////////////////
size_t VertexDataBook::
count_allocated_size(VertexDataPage::RamClass ram_class) const {
  MutexHolder holder(_lock);

  size_t total = 0;
  Pages::const_iterator pi;
  for (pi = _pages.begin(); pi != _pages.end(); ++pi) {
    if ((*pi)->get_ram_class() == ram_class) {
      total += (*pi)->get_total_size();
    }
  }
  return total;
}

////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::save_to_disk
//       Access: Published
//  Description: Writes all pages to disk immediately, just in case
//               they get evicted later.  It makes sense to make this
//               call just before taking down a loading screen, to
//               minimize chugs from saving pages inadvertently later.
////////////////////////////////////////////////////////////////////
void VertexDataBook::
save_to_disk() {
  MutexHolder holder(_lock);

  Pages::iterator pi;
  for (pi = _pages.begin(); pi != _pages.end(); ++pi) {
    (*pi)->save_to_disk();
  }
}


////////////////////////////////////////////////////////////////////
//     Function: VertexDataBook::do_alloc
//       Access: Private
//  Description: Allocates and returns a new VertexDataBuffer of the
//               requested size.
//
//               Assumes the lock is already held.
////////////////////////////////////////////////////////////////////
VertexDataBlock *VertexDataBook::
do_alloc(size_t size) {
  // Look for an empty page of the appropriate size.  The _pages set
  // is sorted so that the pages with the smallest available blocks
  // are at the front.
  
  // Create a dummy page to use to search the set.
  VertexDataPage size_page(size);
  Pages::iterator pi = _pages.lower_bound(&size_page);

  // Now we can start from the first element of the set that is
  // possibly large enough to contain this block, and work up from
  // there.
  while (pi != _pages.end()) {
    Pages::iterator pnext = pi;
    ++pnext;

    VertexDataPage *page = (*pi);

    // Allocating a block may change the page's available contiguous
    // size, and thereby change its position in the set, invalidating
    // the iterator pi.  This is why we've already computed pnext.
    VertexDataBlock *block = page->do_alloc(size);

    if (block != (VertexDataBlock *)NULL) {
      // This page worked.
      return block;
    }

    // Try the next page.
    pi = pnext;
  }

  // No page was good enough.  Create a new page.  Make it at least
  // large enough to hold this requested block.
  VertexDataPage *page = create_new_page(size);
  _pages.insert(page);

  VertexDataBlock *block = page->do_alloc(size);
  return block;
}
