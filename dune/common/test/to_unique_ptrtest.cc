// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:

// make sure assert works even when not compiling for debugging
#ifdef NDEBUG
#undef NDEBUG
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>

#include <dune/common/to_unique_ptr.hh>

int* f_old() { return new int(0); }

Dune::ToUniquePtr<int> f1() { return new int(1); }
auto f2() { return Dune::makeToUnique<int>(1); }

struct A { double x = 1.0; };
Dune::ToUniquePtr<A> g() { return new A{}; }

int main()
{
  using namespace Dune;
  int ret = 0;

  {
    int* ptr = new int(1);

    // test constructor
    ToUniquePtr<int> w1( ptr );

    // test assignment from makeToUnique
    ToUniquePtr<int> w2 = makeToUnique<int>(2);

    // test conversion to pointer
    int* p1 = f1();
    A* p2 = g();

    delete p1;
    delete p2;

    // test conversion to unique_ptr
    {
      std::unique_ptr<int> u1 = f1();
      std::unique_ptr<int> u2 = f2();
      std::unique_ptr<A> u3 = g();
    }

    // test conversion to shared_ptr
    {
      std::shared_ptr<int> s1 = f1();
      std::shared_ptr<A> s2 = g();
      std::shared_ptr<int> s3 = w2;
      assert( !bool(w2) && "w2 should be invalidated" );
    }

    // test move assignment
    {
      ToUniquePtr<int> w3( new int(3) );
      w1 = std::move(w3);

      assert( !bool(w3) && "w3 should be invalidated after move assignment" );
    }
    assert( bool(w1) && "w1 should not be invalidated" );

    // test move construction
    {
      ToUniquePtr<int> w4( std::move(w1) );
      assert( !bool(w1) && "w1 should be invalidated after move construction" );
    }

    // test management of ownership in ToUniquePtr
    {
      auto w5 = makeToUnique<int>(5); // should free at the end of scope
    }

    // test unique_ptr-like interface of ToUniquePtr
    auto w6 = makeToUnique<int>(6);
    assert( *w6 == 6 && "Access to value of ToUniquePtr" );

    w6.reset(new int(7));
    assert( *w6 == 7 && "Access to value of ToUniquePtr after reset" );

    w6.reset();
    assert( !bool(w6) && "w6 should be invalidated" );
  }

  std::unique_ptr<int> x0{ f_old() };
  std::unique_ptr<int> x1{ f1() };
  std::unique_ptr<int> x2 = f1();
  return ret;
}
