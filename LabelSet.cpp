/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/LabelSet.h"
#include "mozilla/dom/LabelSetBinding.h"
#include "nsContentUtils.h"
#include "nsNetUtil.h"
#include "nsIURI.h"
#include "nsCOMPtr.h"
#include "nsScriptSecurityManager.h"
#include "nsServiceManagerUtils.h"
//???
#include "mozilla/dom/BindingUtils.h"

namespace mozilla {
namespace dom {


//NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_1(Label, mPrincipals)
NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(LabelSet)
NS_IMPL_CYCLE_COLLECTING_ADDREF(LabelSet)
NS_IMPL_CYCLE_COLLECTING_RELEASE(LabelSet)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(LabelSet)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END



LabelSet::LabelSet()
{
}

LabelSet::LabelSet(mozilla::dom::Label& s, mozilla::dom::Label& i, mozilla::dom::CapLabel& c, ErrorResult &aRv)
{
  sLabel = s.Clone(aRv);
  iLabel = i.Clone(aRv);
  cLabel = c.Clone(aRv);
}

LabelSet::LabelSet(mozilla::dom::LabelSet& labelSet, ErrorResult& aRv)
{
  nsRefPtr<Label> temS = labelSet.GetSecLabel(aRv);
  sLabel = temS->Clone(aRv);
  nsRefPtr<Label> temI = labelSet.GetIntLabel(aRv);
  iLabel = temI->Clone(aRv);
  nsRefPtr<CapLabel> temC = labelSet.GetCapLabel(aRv);
  cLabel = temC->Clone(aRv);
}

LabelSet::~LabelSet()
{
}

JSObject*
LabelSet::WrapObject(JSContext* aCx)
{
  return LabelSetBinding::Wrap(aCx, this);
}

LabelSet*
LabelSet::GetParentObject() const
{
  return nullptr; //TODO: return something sensible here
}


already_AddRefed<LabelSet>
LabelSet::Constructor(const GlobalObject& global, mozilla::dom::Label& s, mozilla::dom::Label& i, mozilla::dom::CapLabel& c,
                  ErrorResult& aRv)
{
  nsRefPtr<LabelSet> labelset = new LabelSet(s, i, c, aRv);
  if (aRv.Failed())
    return nullptr;
  return labelset.forget();
}

already_AddRefed<LabelSet>
LabelSet::Constructor(const GlobalObject& global, mozilla::dom::LabelSet& labelSet,
                  ErrorResult& aRv)
{
  nsRefPtr<LabelSet> labelset = new LabelSet(labelSet);
  if (aRv.Failed())
    return nullptr;
  return labelset.forget();
}


bool
LabelSet::Equals(mozilla::dom::LabelSet& other)
{
  // Break out early if the other and this are the same.
  if (&other == this)
    return true;

  ErrorResult aRv;
  nsRefPtr<Label> s = other.GetSecLabel(aRv);
  nsRefPtr<Label> i = other.GetIntLabel(aRv);
  nsRefPtr<CapLabel> c = other.GetCapLabel(aRv);


  if (aRv.Failed()){
    return false;
  }

  if (sLabel->Equals(*s)
	   && iLabel->Equals(*i)
       && cLabel->Equals(*c)){
	  return true;
  }

  return false;
}

  already_AddRefed<Label>
LabelSet::GetSecLabel(ErrorResult& aRv){

  nsRefPtr<Label> temLabel = sLabel->Clone(aRv);
  if (aRv.Failed()){
    return nullptr;
  }
  return temLabel.forget();

  }

  bool
  LabelSet::IsFlumeSafe(mozilla::dom::LabelSet& labelSet){
    ErrorResult aRv;
    nsRefPtr<Label> s = labelSet.GetSecLabel(aRv);
    nsRefPtr<Label> i = labelSet.GetIntLabel(aRv);
    if( (s->Subsumes(*sLabel)) && (iLabel->Subsumes(*i))){
      return true;
    }
    return false;
  }

  already_AddRefed<Label>
LabelSet::GetIntLabel(ErrorResult& aRv){

  nsRefPtr<Label> _this = iLabel->Clone(aRv);
  if (aRv.Failed()){
    return nullptr;
  }
  return _this.forget();

  }

  already_AddRefed<CapLabel>
LabelSet::GetCapLabel(ErrorResult& aRv){

  nsRefPtr<CapLabel> _this = cLabel->Clone(aRv);
  if (aRv.Failed()){
    return nullptr;
  }
  return _this.forget();

  }

void
LabelSet::Stringify(nsString& retval)
{
  retval = NS_LITERAL_STRING("S-");
  sLabel->Stringify(retval);
  retval.Append(NS_LITERAL_STRING(" I-"));
  iLabel->Stringify(retval);
  retval.Append(NS_LITERAL_STRING(" C-"));
  cLabel->Stringify(retval);
}


void LabelSet::AddSecPrincipal(const nsAString& principal, ErrorResult& aRv){

  nsCOMPtr<nsIPrincipal> nPrincipal;
  StrToPrin(principal, aRv, nPrincipal);
  AddSecPrincipal(nPrincipal, aRv);

}

void LabelSet::AddSecPrincipal(nsIPrincipal* principal, ErrorResult& aRv){

  nsRefPtr<CapItem> item = new CapItem(principal, (unsigned)1, aRv);
  nsRefPtr<CapItem> Ditem = new CapItem(principal, (unsigned)3, aRv);


  if (cLabel->Contains(*item) || cLabel->Contains(*Ditem)) {
	sLabel->_And(principal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void LabelSet::DelSecPrincipal(const nsAString& principal, ErrorResult& aRv){

  nsCOMPtr<nsIPrincipal> nPrincipal;
  StrToPrin(principal, aRv, nPrincipal);
  DelSecPrincipal(nPrincipal, aRv);

}

void LabelSet::DelSecPrincipal(nsIPrincipal* principal, ErrorResult& aRv){

  nsRefPtr<CapItem> item = new CapItem(principal, (unsigned)2, aRv);
  nsRefPtr<CapItem> Ditem = new CapItem(principal, (unsigned)3, aRv);

  if (cLabel->Contains(*item) || cLabel->Contains(*Ditem)) {
	sLabel->Reduce(*principal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void LabelSet::AddIntPrincipal(const nsAString& principal, ErrorResult& aRv){

  nsCOMPtr<nsIPrincipal> nPrincipal;
  StrToPrin(principal, aRv, nPrincipal);
  AddIntPrincipal(nPrincipal, aRv);

}

void LabelSet::AddIntPrincipal(nsIPrincipal* principal, ErrorResult& aRv){

  nsRefPtr<CapItem> item = new CapItem(principal, (unsigned)1, aRv);
  nsRefPtr<CapItem> Ditem = new CapItem(principal, (unsigned)3, aRv);

  if (cLabel->Contains(*item) || cLabel->Contains(*Ditem)) {
	iLabel->_And(principal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void LabelSet::DelIntPrincipal(const nsAString& principal, ErrorResult& aRv){

  nsCOMPtr<nsIPrincipal> nPrincipal;
  StrToPrin(principal, aRv, nPrincipal);
  DelIntPrincipal(nPrincipal, aRv);

}

void LabelSet::DelIntPrincipal(nsIPrincipal* principal, ErrorResult& aRv){

  nsRefPtr<CapItem> item = new CapItem(principal, (unsigned)2, aRv);
  nsRefPtr<CapItem> Ditem = new CapItem(principal, (unsigned)3, aRv);

  if (cLabel->Contains(*item) || cLabel->Contains(*Ditem)) {
	iLabel->Reduce(*principal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

already_AddRefed<LabelSet>
LabelSet::Clone(ErrorResult& aRv) const{
    nsRefPtr<Label> s = sLabel->Clone(aRv);
    nsRefPtr<Label> i = iLabel->Clone(aRv);
    nsRefPtr<CapLabel> c = cLabel->Clone(aRv);

    nsRefPtr<LabelSet> labelSet = new LabelSet(*s, *i, *c, aRv);

  return labelSet.forget();
};

void
LabelSet::StrToPrin(const nsAString& principal, ErrorResult& aRv, nsCOMPtr<nsIPrincipal>& prinPtr){
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
  prinPtr = nPrincipal;
}

} // namespace dom
} // namespace mozilla
