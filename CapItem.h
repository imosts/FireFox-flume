/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "mozilla/Attributes.h"
#include "mozilla/ErrorResult.h"
#include "nsCycleCollectionParticipant.h"
#include "nsWrapperCache.h"
#include "nsCOMPtr.h"
#include "nsString.h"
#include "nsIDocument.h"
#include "nsTArray.h"
#include "nsContentUtils.h"

struct JSContext;

namespace mozilla {
namespace dom {

class CapItem MOZ_FINAL : public nsISupports
                     , public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(CapItem)

protected:
  ~CapItem();

public:
  CapItem();
  CapItem(const nsAString& principal, unsigned capType, ErrorResult& aRv);
  CapItem(nsIPrincipal* principal ,unsigned capType, ErrorResult& aRv);
  
  //CapItem(CapItem& other);


  CapItem* GetParentObject() const; //FIXME

  virtual JSObject* WrapObject(JSContext* aCx) MOZ_OVERRIDE;
  
  bool Equals(CapItem& other);

  static already_AddRefed<CapItem> Constructor(const GlobalObject& global,
                                            const nsAString& principal, 
											uint32_t capType,
                                            ErrorResult& aRv);

/*JS接口中使用nsIPrincipal来构造对象的方法暂且不用				
  static already_AddRefed<CapItem> Constructor(const GlobalObject& global,
                                            nsIPrincipal* principal, 
											uint32_t capType,
                                            ErrorResult& aRv);
											*/

//每个类中加入Clone函数
  already_AddRefed<CapItem> Clone(ErrorResult &aRv) const;

private: // C++ only:
  void _Init(const nsAString& principal, unsigned capType, ErrorResult& aRv);
  void _Init(nsIPrincipal* principal, unsigned capType, ErrorResult& aRv);
  void _Init(mozilla::dom::CapItem& other);


public: 

  bool IsEmpty() const
  {
    return !cPrincipal;
  }
  
  nsCOMPtr<nsIPrincipal>* GetPrincipal()
  {
    return &cPrincipal;
  }
  
  unsigned GetType(){
	  return cType;
  }

private:
  nsCOMPtr<nsIPrincipal> cPrincipal;
  unsigned cType;
};




} // namespace dom
} // namespace mozilla
