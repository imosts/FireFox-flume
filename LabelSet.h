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

class LabelSet MOZ_FINAL : public nsISupports
                     , public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(LabelSet)

protected:
  ~LabelSet();

public:
  LabelSet();
  LabelSet(mozilla::dom::Label* s, mozilla::dom::Label* i, mozilla::dom::CapLabel* c);
  LabelSet(mozilla::dom::LabelSet labelSet);


  LabelSet* GetParentObject() const; //FIXME

  virtual JSObject* WrapObject(JSContext* aCx) MOZ_OVERRIDE;


  static already_AddRefed<LabelSet> Constructor(const GlobalObject& global,
                                            mozilla::dom::Label& s,
											mozilla::dom::Label& i,
											mozilla::dom::CapLabel& c,
                                            ErrorResult& aRv);
  static already_AddRefed<LabelSet> Constructor(const GlobalObject& global,
                                            mozilla::dom::LabelSet& labelSet, 
                                            ErrorResult& aRv);
										

  bool Equals(mozilla::dom::LabelSet& other);
  
  bool IsFlumeSafe(mozilla::dom::LabelSet& labelSet);
  
  
  static
  already_AddRefed<Label> GetSecLabel();
  static 
  already_AddRefed<Label> GetIntLabel();
  static 
  already_AddRefed<CapLabel> GetCapLabel();
  
  void AddSecPrincipal(const nsAString& principal, ErrorResult& aRv);
  void AddSecPrincipal(nsIPrincipal* principal, ErrorResult& aRv);
  
  void DelSecPrincipal(const nsAString& principal, ErrorResult& aRv);
  void DelSecPrincipal(nsIPrincipal* principal, ErrorResult& aRv);
  
  void AddIntPrincipal(const nsAString& principal, ErrorResult& aRv);
  void AddIntPrincipal(nsIPrincipal* principal, ErrorResult& aRv);
  
  void DelIntPrincipal(const nsAString& principal, ErrorResult& aRv);
  void DelIntPrincipal(nsIPrincipal* principal, ErrorResult& aRv);
  
  static 
  already_AddRefed<LabelSet> clone(ErrorResult& aRv);
   
  void Stringify(nsString& retval);
  
  

public: // C++ only:

  
  

				  
private:
  Label sLabel;
  Label iLabel;
  CapLabel cLabel;

};


} // namespace dom
} // namespace mozilla
