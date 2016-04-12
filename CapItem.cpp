/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "mozilla/dom/CapItem.h"
#include "mozilla/dom/CapItemBinding.h"
#include "nsContentUtils.h"
#include "nsNetUtil.h"
#include "nsIURI.h"
#include "nsCOMPtr.h"
#include "nsScriptSecurityManager.h"
#include "nsServiceManagerUtils.h"
//???
#include "mozilla/dom/BindingUtils.h"

struct JSContext;

namespace mozilla {
namespace dom {
	
//NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_1(Label, mPrincipals)
NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(CapItem)
NS_IMPL_CYCLE_COLLECTING_ADDREF(CapItem)
NS_IMPL_CYCLE_COLLECTING_RELEASE(CapItem)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(CapItem)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

  CapItem::~CapItem(){};

  CapItem::CapItem(){};
  
  CapItem::CapItem(const nsAString& principal, unsigned capType, ErrorResult& aRv){
	  _Init(principal, capType, aRv);
  };
  
  CapItem::CapItem(nsIPrincipal* principal ,unsigned capType, ErrorResult& aRv){
	   _Init(principal, capType, aRv);
  };
  
  /*
  CapItem::CapItem(mozilla::dom::CapItem& other){
	   _Init(other);
  };
*/

CapItem*
CapItem::GetParentObject() const
{
  return nullptr; //TODO: return something sensible here
}

JSObject*
CapItem::WrapObject(JSContext* aCx)
{
  return CapItemBinding::Wrap(aCx, this);
}

already_AddRefed<CapItem>
CapItem::Constructor(const GlobalObject& global, const nsAString& principal, uint32_t capType,
                  ErrorResult& aRv)
{
  nsRefPtr<CapItem> capLaItem = new CapItem(principal, capType, aRv);
  if (aRv.Failed())
    return nullptr;
  return capLaItem.forget();
}


/*JS接口中使用nsIPrincipal来构造对象的方法暂且不用
already_AddRefed<CapItem>
CapItem::Constructor(const GlobalObject& global, nsIPrincipal* principal, uint32_t capType,
                  ErrorResult& aRv)
{
  nsRefPtr<CapItem> capLaItem = new CapItem(principal, capType, aRv);
  if (aRv.Failed())
    return nullptr;
  return capLaItem.forget();
}
*/


 already_AddRefed<CapItem> 
 CapItem::Clone(ErrorResult &aRv) const{
  nsRefPtr<CapItem> capItem = new CapItem(this->cPrincipal, this->cType, aRv);

  if(!capItem) {
    aRv = NS_ERROR_OUT_OF_MEMORY;
    return nullptr;
  }
  
  return capItem.forget();
 }
 


bool CapItem::Equals(CapItem& other){
  bool res;
  nsCOMPtr<nsIPrincipal> p1 = *(other.GetPrincipal());
  char *origin1, *origin2;
  
  nsresult rv;

  rv = p1->GetOrigin(&origin1);
  NS_ASSERTION(NS_SUCCEEDED(rv), "nsIPrincipal::GetOrigin failed");
  if (NS_FAILED(rv)) return false;

  rv = cPrincipal->GetOrigin(&origin2);
  NS_ASSERTION(NS_SUCCEEDED(rv), "nsIPrincipal::GetOrigin failed");
  if (NS_FAILED(rv)) return false;

  res = strcmp(origin1, origin2);

  nsMemory::Free(origin1);
  nsMemory::Free(origin2);
  
  if(res == 0){
	if((other.GetType()) == (cType)){
		return true;
	}
  }
  
  return false;


};

  
  void CapItem::_Init(const nsAString& principal, unsigned capType, ErrorResult& aRv){
	if (capType == 0 || capType > 3) {
		aRv.Throw(NS_ERROR_FAILURE);
		return;
	}
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

  _Init(nPrincipal, capType, aRv);
  
  };
  void CapItem::_Init(nsIPrincipal* principal, unsigned capType, ErrorResult& aRv){
	  if (capType == 0 || capType > 3) {
    aRv.Throw(NS_ERROR_FAILURE);
		return;
	  }
	  cPrincipal = principal;
	  cType = capType;
	  
  };
  
  void CapItem::_Init(mozilla::dom::CapItem& other){
	  if (other.GetType() == 0 || other.GetType() > 3) {
		return;
	  }
	  this->cPrincipal = *(other.GetPrincipal());
	  this->cType = other.GetType();
  };



} // namespace dom
} // namespace mozilla
