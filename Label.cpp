/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/Label.h"
#include "mozilla/dom/LabelBinding.h"
#include "nsContentUtils.h"
#include "nsNetUtil.h"
#include "nsIURI.h"
#include "nsCOMPtr.h"
#include "nsScriptSecurityManager.h"
#include "nsServiceManagerUtils.h"
#include "mozilla/dom/BindingUtils.h"

namespace mozilla {
namespace dom {


//NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_1(Label, mPrincipals)
NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(Label)
NS_IMPL_CYCLE_COLLECTING_ADDREF(Label)
NS_IMPL_CYCLE_COLLECTING_RELEASE(Label)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(Label)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END



Label::Label()
{
}

Label::Label(const nsAString& principal, ErrorResult& aRv)
{
  _And(principal,aRv);
}

Label::Label(nsIPrincipal* principal)
{
  _And(principal);
}

Label::~Label()
{
}

JSObject*
Label::WrapObject(JSContext* aCx)
{
  return LabelBinding::Wrap(aCx, this);
}

Label*
Label::GetParentObject() const
{
  return nullptr; //TODO: return something sensible here
}

already_AddRefed<Label>
Label::Constructor(const GlobalObject& global, const nsAString& principal, 
                  ErrorResult& aRv)
{
  nsRefPtr<Label> label = new Label(principal, aRv);
  if (aRv.Failed())
    return nullptr;
  return label.forget();
}

already_AddRefed<Label>
Label::Constructor(const GlobalObject& global, const Sequence<nsString >& principals, 
                  ErrorResult& aRv)
{
  nsRefPtr<Label> label = new Label();
  for (unsigned i = 0; i < principals.Length(); ++i) {
    label->_And(principals[i],aRv);
    if (aRv.Failed())
      return nullptr;
  }
  return label.forget();
}

bool
Label::Equals(mozilla::dom::Label& other)
{
  // Break out early if the other and this are the same.
  if (&other == this)
    return true;

  PrincipalArray *otherPrincipals = other.GetDirectPrincipals();

  // The other label is of a different size, can't be equal.
  if (otherPrincipals->Length() != mPrincipals.Length())
    return false;

  nsIPrincipalComparator cmp;
  for (unsigned i=0; i< mPrincipals.Length(); ++i) {
    /* This role contains a principal that the other role does not, 
     * hence it cannot be equal to it. */
    if(!cmp.Equals(mPrincipals[i], (*otherPrincipals)[i]))
      return false;
  }

  return true;
}

bool
Label::Subsumes(const mozilla::dom::Label& other)
{
  // Break out early if the other points to this
  if (&other == this)
    return true;

  PrincipalArray *otherPrincipals = other.GetDirectPrincipals();

  // The other label is smaller, this label cannot imply (subsume) it.
  if (otherPrincipals->Length() < mPrincipals.Length())
    return false;

  nsIPrincipalComparator cmp;
  for (unsigned i=0; i< mPrincipals.Length(); ++i) {
    /* This label contains a principal that the other label does not, 
     * hence it cannot imply (subsume) it. */
    if (!otherPrincipals->Contains(mPrincipals[i],cmp))
      return false;
  }

  return true;
}

/*
already_AddRefed<Label>
Label::And(const nsAString& principal, ErrorResult& aRv)
{
  _And(principal, aRv);
  if (aRv.Failed())
    return nullptr;

  nsRefPtr<Label> _this = this;
  return _this.forget();
}

already_AddRefed<Label>
Label::And(nsIPrincipal* principal, ErrorResult& aRv)
{
  _And(principal);
  nsRefPtr<Label> _this = this;
  return _this.forget();
}

already_AddRefed<Label>
Label::And(Label& other, ErrorResult& aRv)
{
  _And(other);
  nsRefPtr<Label> _this = this;
  return _this.forget();
}
*/


void 
Label::Stringify(nsString& retval)
{
  if (retval == NULL) {
	retval = NS_LITERAL_STRING("Label(");
  }else{
	retval.Append(NS_LITERAL_STRING("Label(")); 
  }

  for (unsigned i=0; i < mPrincipals.Length(); ++i) {
    char *origin = NULL;
    nsresult rv = mPrincipals[i]->GetOrigin(&origin);
    if (NS_FAILED(rv) || !origin) {
      retval.Append(NS_LITERAL_STRING("x-bogus:<unknown-principal>"));
    } else {
      AppendASCIItoUTF16(origin, retval);
      NS_Free(origin);
    }

     if (i != (mPrincipals.Length() -1))
       retval.Append(NS_LITERAL_STRING(").and("));

  }
  retval.Append(NS_LITERAL_STRING(")"));
}

void
Label::Reduce(nsIPrincipal &principal)
{
  if (principal == NULL) return;
  
  nsIPrincipalComparator cmp;
  while (mPrincipals.RemoveElement(principal, cmp)) ;
}

already_AddRefed<Label>
Label::Clone(ErrorResult &aRv) const
{
  nsRefPtr<Label> label = new Label();

  if(!label) {
    aRv = NS_ERROR_OUT_OF_MEMORY;
    return nullptr;
  }

  PrincipalArray *newPrincipals = label->GetDirectPrincipals();
  for (unsigned i = 0; i < mPrincipals.Length(); ++i) {
    newPrincipals->InsertElementAt(i, mPrincipals[i]);
  }
  return label.forget();
}


//
// Internals
//

void
Label::_And(const nsAString& principal, ErrorResult& aRv)
{
  nsCOMPtr<nsIScriptSecurityManager> secMan =
    nsContentUtils::GetSecurityManager();

  if (!secMan) {
    aRv.Throw(NS_ERROR_FAILURE);
    return;
  }

  nsresult rv;

  // Create URI
  nsCOMPtr<nsIURI> uri;
  rv = NS_NewURI(getter_AddRefs(uri), principal);
  if (NS_FAILED(rv)) {
    rv = NS_NewURI(getter_AddRefs(uri), 
                   NS_LITERAL_STRING("moz-role:") + principal);
    if (NS_FAILED(rv)) {
      aRv.Throw(rv);
      return;
    }
  }

  // Create Principal
  nsCOMPtr<nsIPrincipal> nPrincipal;
  rv = secMan->GetNoAppCodebasePrincipal(uri, getter_AddRefs(nPrincipal));
  if (NS_FAILED(rv)) {
    aRv.Throw(rv);
    return;
  }

  _And(nPrincipal);
}

void
Label::_And(nsIPrincipal* principal)
{
  // Add principal if it's not there
  nsIPrincipalComparator cmp;
  if (!mPrincipals.Contains(principal, cmp))
    mPrincipals.InsertElementSorted(principal, cmp);
}


void
Label::_And(Label& other)
{
  PrincipalArray *otherPrincipals = other.GetDirectPrincipals();
  for (unsigned i=0; i< otherPrincipals->Length(); ++i) {
    _And(otherPrincipals->ElementAt(i));
  }
}

// Comparator helpers

int
nsIPrincipalComparator::Compare(const nsCOMPtr<nsIPrincipal> &p1,
                                const nsCOMPtr<nsIPrincipal> &p2) const
{
  bool res;
  char *origin1, *origin2;
  nsresult rv;

  rv = p1->GetOrigin(&origin1);
  NS_ASSERTION(NS_SUCCEEDED(rv), "nsIPrincipal::GetOrigin failed");
  if (NS_FAILED(rv)) return false;

  rv = p2->GetOrigin(&origin2);
  NS_ASSERTION(NS_SUCCEEDED(rv), "nsIPrincipal::GetOrigin failed");
  if (NS_FAILED(rv)) return false;

  res = strcmp(origin1, origin2);

  nsMemory::Free(origin1);
  nsMemory::Free(origin2);

  return res;
}

} // namespace dom
} // namespace mozilla
