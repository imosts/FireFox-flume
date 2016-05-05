/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/CapLabel.h"
#include "mozilla/dom/CapLabelBinding.h"
#include "nsContentUtils.h"
#include "nsNetUtil.h"
#include "nsIURI.h"
#include "nsCOMPtr.h"
#include "nsScriptSecurityManager.h"
#include "nsServiceManagerUtils.h"
//???
#include "mozilla/dom/BindingUtils.h"
#include "mozilla/dom/TypedArray.h"

namespace mozilla {
namespace dom {


//NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_1(Label, mPrincipals)
NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(CapLabel)
NS_IMPL_CYCLE_COLLECTING_ADDREF(CapLabel)
NS_IMPL_CYCLE_COLLECTING_RELEASE(CapLabel)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(CapLabel)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END



CapLabel::CapLabel()
{
}

CapLabel::CapLabel(mozilla::dom::CapItem &cap){
  _And(cap);
};

CapLabel::CapLabel(const nsAString& principal, unsigned capType, ErrorResult& aRv)
{
  _And(principal, capType, aRv);
}

CapLabel::CapLabel(nsIPrincipal* principal, unsigned capType)
{
  _And(principal, capType);
}

CapLabel::~CapLabel()
{
}

JSObject*
CapLabel::WrapObject(JSContext* aCx)
{
  return CapLabelBinding::Wrap(aCx, this);
}

CapLabel*
CapLabel::GetParentObject() const
{
  return nullptr; //TODO: return something sensible here
}



already_AddRefed<CapLabel>
CapLabel::Constructor(const GlobalObject& global, const nsAString& principal, unsigned int capType,
                  ErrorResult& aRv)
{
  nsRefPtr<CapLabel> caplabel = new CapLabel(principal, capType, aRv);
  if (aRv.Failed())
    return nullptr;
  return caplabel.forget();
}

already_AddRefed<CapLabel>
CapLabel::Constructor(const GlobalObject& global, const Sequence<nsString >& principals, 
const Sequence<uint32_t>& capType,
                  ErrorResult& aRv)
{
  nsRefPtr<CapLabel> caplabel = new CapLabel();
  if (principals.Length() != capType.Length()) {
	  aRv.Throw(NS_ERROR_FAILURE);
      return nullptr;
}
  //unsigned* type = capType.Data();
  for (unsigned i = 0; i < principals.Length(); ++i) {
    caplabel->_And(principals[i], capType[i], aRv);
    if (aRv.Failed())
      return nullptr;
  }
  return caplabel.forget();
}

/*JS接口中使用nsIPrincipal来构造对象的方法暂且不用	
already_AddRefed<CapLabel> 
CapLabel::Constructor(const GlobalObject& global, nsIPrincipal* principal, uint32_t capType,
                  ErrorResult& aRv)
{
	nsRefPtr<CapLabel> caplabel = new CapLabel(principal, capType, aRv);
    if (aRv.Failed())
      return nullptr;
    return caplabel.forget();
}
*/


/*JS接口中使用nsIPrincipal来构造对象的方法暂且不用	,可以使用Clone代替来构造，若要使用，本函数还需修改，！！！引用指针！！！
already_AddRefed<CapLabel> 
CapLabel::Constructor(const GlobalObject& global, mozilla::dom::CapItem capItem,
                  ErrorResult& aRv)
{
	nsRefPtr<CapLabel> caplabel = new CapLabel(capItem, aRv);
    if (aRv.Failed())
      return nullptr;
    return caplabel.forget();
}
*/

 already_AddRefed<CapLabel> 
 CapLabel::Clone(ErrorResult &aRv) const{
  nsRefPtr<CapLabel> capLabel = new CapLabel();

  if(!capLabel) {
    aRv = NS_ERROR_OUT_OF_MEMORY;
    return nullptr;
  }

  CapArray *newCapl = capLabel->GetDirectCaps();
  for (unsigned i = 0; i < mCaps.Length(); i++) {
    nsRefPtr<CapItem> item = mCaps[i]->Clone(aRv);
    if (aRv.Failed())
      return nullptr;
    newCapl->InsertElementAt(i, item);
  }

  return capLabel.forget();
 }

bool
CapLabel::Equals(mozilla::dom::CapLabel& other)
{
  // Break out early if the other and this are the same.
  if (&other == this)
    return true;

  CapArray *otherCaps = other.GetDirectCaps();

  // The other label is of a different size, can't be equal.
  if (otherCaps->Length() != mCaps.Length())
    return false;

  nsICapPrincipalComparator cmp;
  for (unsigned i=0; i< mCaps.Length(); ++i) {
    /* This role contains a principal that the other role does not, 
     * hence it cannot be equal to it. */
    if(cmp.Equals(mCaps[i],(*otherCaps)[i]))
      return false;
  }

  return true;
}



already_AddRefed<CapLabel>
CapLabel::And(const nsAString& principal, unsigned capType, ErrorResult& aRv)
{
  _And(principal, capType, aRv);
  if (aRv.Failed())
    return nullptr;

  nsRefPtr<CapLabel> _this = this;
  return _this.forget();
}

already_AddRefed<CapLabel>
CapLabel::And(nsIPrincipal* principal, unsigned capType, ErrorResult& aRv)
{
  _And(principal, capType);
  nsRefPtr<CapLabel> _this = this;
  return _this.forget();
}

already_AddRefed<CapLabel>
CapLabel::And(CapLabel& other, ErrorResult& aRv)
{
  _And(other);
  nsRefPtr<CapLabel> _this = this;
  return _this.forget();
}


void 
CapLabel::Stringify(nsString& retval)
{
  if (retval == NULL) {
	retval = NS_LITERAL_STRING("Label(");
  }else{
	retval.Append(NS_LITERAL_STRING("Label(")); 
  }

  nsCOMPtr<nsIPrincipal> temPrin = *(mCaps[0]->GetPrincipal());

  for (unsigned i=0; i < mCaps.Length(); ++i) {
    char *origin = NULL;
    temPrin = *(mCaps[i]->GetPrincipal());
    nsresult rv = temPrin->GetOrigin(&origin);
    if (NS_FAILED(rv) || !origin) {
      retval.Append(NS_LITERAL_STRING("x-bogus:<unknown-principal>"));
    } else {
      AppendASCIItoUTF16(origin, retval);
      NS_Free(origin);
	  if (mCaps[i]->GetType() == 1){
		  retval.Append(NS_LITERAL_STRING(" Type:+"));
	  }else if(mCaps[i]->GetType() == 2){
		  retval.Append(NS_LITERAL_STRING(" Type:-"));
	  }else if(mCaps[i]->GetType() == 3){
		  retval.Append(NS_LITERAL_STRING(" Type:D"));
	  }else{
		  retval.Append(NS_LITERAL_STRING(" Type:error!"));
	  }
    }

     if (i != (mCaps.Length() -1))
       retval.Append(NS_LITERAL_STRING(").and("));

  }
  retval.Append(NS_LITERAL_STRING(")"));
}

void
CapLabel::Reduce(mozilla::dom::CapItem& cap)
{
  
  nsICapPrincipalComparator cmp;
  while (mCaps.RemoveElement(&cap, cmp));
  
}

 bool 
 CapLabel::Contains(mozilla::dom::CapItem& cap){
  nsRefPtr<CapItem> temCap = new CapItem(cap);
  nsICapPrincipalComparator cmp;
  if (!mCaps.Contains(temCap, cmp)){
	return true; 
  }
  return false;
 }

/*
already_AddRefed<CapLabel>
CapLabel::Clone(ErrorResult &aRv) const
{
  nsRefPtr<CapLabel> caplabel = new CapLabel();

  if(!caplabel) {
    aRv = NS_ERROR_OUT_OF_MEMORY;
    return nullptr;
  }

  CapArray *newCaps = caplabel->GetPrincipal();
  for (unsigned i = 0; i < mCaps.Length(); ++i) {
    newCaps->InsertElementAt(i, mCaps[i]);
  }
  return caplabel.forget();
}

*/


//
// Internals
//

void
CapLabel::_And(const nsAString& principal, unsigned capType, ErrorResult& aRv)
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

  _And(nPrincipal, capType);
}

void
CapLabel::_And(nsIPrincipal* principal, unsigned capType)
{
  ErrorResult aRv;
  nsRefPtr<CapItem> item = new CapItem(principal, capType, aRv);
  _And(*item);
}

void
CapLabel::_And(mozilla::dom::CapItem& cap)
{
  // Add principal if it's not there
  nsRefPtr<CapItem> tempCap = new CapItem(cap);
  nsICapPrincipalComparator cmp;
  if (!mCaps.Contains(tempCap, cmp))
    mCaps.InsertElementSorted(tempCap, cmp);
}


void
CapLabel::_And(mozilla::dom::CapLabel& other)
{
  CapArray *otherCaps = other.GetDirectCaps();
  for (unsigned i=0; i< otherCaps->Length(); ++i) {
    _And(*(otherCaps->ElementAt(i)));
  }
}

// Comparator helpers

int
nsICapPrincipalComparator::Compare(const nsRefPtr<mozilla::dom::CapItem> &c1,
                                   const nsRefPtr<mozilla::dom::CapItem> &c2) const
{
  bool res;
  nsCOMPtr<nsIPrincipal> p1 = *(c1->GetPrincipal());
  nsCOMPtr<nsIPrincipal> p2 = *(c2->GetPrincipal());
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

  if(c1->GetType() == c2->GetType()){
  return res;
  }

  return 1;


}

} // namespace dom
} // namespace mozilla
