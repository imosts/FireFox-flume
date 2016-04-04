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
  
  CapItem(nsIPrincipal* principal ,unsigned capType);
  
  //CapItem(CapItem& other);


  CapItem* GetParentObject() const; //FIXME

  virtual JSObject* WrapObject(JSContext* aCx) MOZ_OVERRIDE;
  
  bool Equals(mozilla::dom::CapItem& other);

  
  static already_AddRefed<CapLabel> Constructor(const GlobalObject& global,
                                            const nsAString& principal, 
											uint32_t capType,
                                            ErrorResult& aRv);
											
  static already_AddRefed<CapLabel> Constructor(const GlobalObject& global,
                                            nsIPrincipal* principal, 
											uint32_t capType,
                                            ErrorResult& aRv);

  /*
  already_AddRefed<Label> Clone(ErrorResult &aRv) const;
  */

private: // C++ only:
  void _Init(const nsAString& principal, unsigned capType, ErrorResult& aRv);
  void _Init(nsIPrincipal* principal, unsigned capType);
  void _Init(mozilla::dom::CapItem& other);


public: 

  bool IsEmpty() const
  {
    return !cPrincipal;
  }
  
  already_AddRefed<nsIPrincipal> GetPrincipal()
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