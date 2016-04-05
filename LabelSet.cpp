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

LabelSet::LabelSet(mozilla::dom::Label* s, mozilla::dom::Label* i, mozilla::dom::CapLabel* c)
{
  sLabel = *s;
  iLabel = *i;
  cLabel = *c;
}

LabelSet::LabelSet(mozilla::dom::LabelSet* labelSet)
{
  this.sLabel = labelSet->GetSecLabel();
  this.iLabel = labelSet->GetIntLabel();
  this.cLabel = labelSet->GetCapLabel();
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
  nsRefPtr<LabelSet> labelset = new LabelSet(s, i, c);
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

  if(this.sLabel.Equals(other.sLabel)
	  && this.iLabel.Equals(other.iLabel)
      && this.cLabel.Equals(other.cLabel)){
	  return true;
  }
  
  return false;
}

static
  already_AddRefed<Label> GetSecLabel(){
	  
  nsRefPtr<Label> _this = this.sLabel;
  return _this.forget();
  
  }
  
  static
  already_AddRefed<Label> GetIntLabel(){
	  
  nsRefPtr<Label> _this = this.iLabel;
  return _this.forget();
  
  }
  
  static
  already_AddRefed<CapLabel> GetCapLabel(){
	  
  nsRefPtr<CapLabel> _this = this.cLabel;
  return _this.forget();
  
  }
  
void 
Label::Stringify(nsString& retval)
{
  retval = NS_LITERAL_STRING("S-");
  sLabel.Stringify(retval);
  retval.Append(NS_LITERAL_STRING(" I-"));
  iLabel.Stringify(retval);
  retval.Append(NS_LITERAL_STRING(" C-"));
  cLabel.Stringify(retval);
}

  // Create Principal
  rv = secMan->GetNoAppCodebasePrincipal(uri, getter_AddRefs(nPrincipal));
  if (NS_FAILED(rv)) {
    aRv.Throw(rv);
    return;
  }
}

void AddSecPrincipal(const nsAString& principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 1);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	nsCOMPtr<Label> nsPrincipal = new Label(principal);
	sLabel.And(principal, aRv);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void AddSecPrincipal(nsIPrincipal* principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 1);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	sLabel.And(principal, aRv);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void DelSecPrincipal(const nsAString& principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 2);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	nsCOMPtr<Label> nsPrincipal = new Label(principal);
	sLabel.Reduce(nsPrincipal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void DelSecPrincipal(nsIPrincipal* principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 2);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	sLabel.Reduce(principal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void AddIntPrincipal(const nsAString& principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 1);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	nsCOMPtr<Label> nsPrincipal = new Label(principal);
	iLabel.And(nsPrincipal, aRv);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void AddIntPrincipal(nsIPrincipal* principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 1);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	iLabel.And(principal, aRv);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void DelSIntPrincipal(const nsAString& principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 2);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	nsCOMPtr<Label> nsPrincipal = new Label(principal);
	iLabel.Reduce(nsPrincipal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

void DelIntPrincipal(nsIPrincipal* principal, ErrorResult& aRv){
	
  nsCOMPtr<CapItem> item = new CapItem(principal, 2);
  nsCOMPtr<CapItem> Ditem = new CapItem(principal, 3);
  nsIPrincipalComparator cmp;
  
  if (cLabel.Contains(item) || cLabel.Contains(Ditem)) {
	iLabel.Reduce(principal);
  }else{
	aRv.Throw(NS_ERROR_FAILURE);
  }
}

} // namespace dom
} // namespace mozilla
