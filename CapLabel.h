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
#include "CapItem.h"

struct JSContext;

namespace mozilla {
namespace dom {


typedef nsTArray< nsCOMPtr<mozilla::dom::CapItem> > CapArray;

class CapLabel MOZ_FINAL : public nsISupports
                     , public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(CapLabel)

protected:
  ~CapLabel();

public:
  CapLabel();
  CapLabel(mozilla::dom::CapItem &cap, ErrorResult& aRv);
  CapLabel(const nsAString& principal, unsigned capType, ErrorResult& aRv);
  CapLabel(nsIPrincipal* principal ,unsigned capType);


  CapLabel* GetParentObject() const; //FIXME

  virtual JSObject* WrapObject(JSContext* aCx) MOZ_OVERRIDE;

  

  static already_AddRefed<CapLabel> Constructor(const GlobalObject& global,
                                            const nsAString& principal, 
											uint32_t capType,
                                            ErrorResult& aRv);
  static already_AddRefed<CapLabel> Constructor(const GlobalObject& global,
                                            const Sequence<nsString>& principals, 
											Sequence<uint32_t>& capType,
                                            ErrorResult& aRv);
  static already_AddRefed<CapLabel> Constructor(const GlobalObject& global,
                                            nsIPrincipal* principal, 
											uint32_t capType,
                                            ErrorResult& aRv);
  static already_AddRefed<CapLabel> Constructor(const GlobalObject& global,
                                            nsIPrincipal* principal, 
											mozilla::dom::CapItem capType,
                                            ErrorResult& aRv);


  //already_AddRefed<CapLabel> Clone(ErrorResult &aRv) const;
  
  bool Equals(mozilla::dom::Label& other);

  void Stringify(nsString& retval);


public: // C++ only:
  //暂时不写在接口里
  already_AddRefed<CapLabel> And(const nsAString& principal, unsigned[] capType, ErrorResult& aRv);
  already_AddRefed<CapLabel> And(nsIPrincipal* principal, unsigned capType, ErrorResult& aRv);
  already_AddRefed<CapLabel> And(mozilla::dom::CapLabel& other, ErrorResult& aRv);

  void _And(const nsAString& principal, unsigned capType, ErrorResult& aRv);
  void _And(nsIPrincipal* principal, unsigned capType);
  void _And(mozilla::dom::CapItem& cap);
  void _And(mozilla::dom::CapLabel& other);
  
    
  void Reduce(mozilla::dom::CapItem& cap);
  
  bool Contains(mozilla::dom::CapItem& cap);

  bool IsEmpty() const
  {
    return !mCaps.Length();
  }

private:

  void InternalAnd(mozilla::dom::CapItem& role, ErrorResult* aRv = nullptr,
                   bool clone = false);

public: // XXX TODO make private, unsafe
  PrincipalArray* GetDirectCaps()
  {
    return &mCaps;
  }

//  friend class Label;

private:
  CapArray mCaps;

};

class nsICapPrincipalComparator {

public:
  int Compare(const nsCOMPtr<mozilla::dom::CapItem> &p1,
              const nsCOMPtr<mozilla::dom::CapItem> &p2) const;

  bool Equals(const nsCOMPtr<mozilla::dom::CapItem> &p1,
              const nsCOMPtr<mozilla::dom::CapItem> &p2) const
  {
    return Compare(p1,p2) == 0;
  }

  bool LessThan(const nsCOMPtr<mozilla::dom::CapItem> &p1,
                const nsCOMPtr<mozilla::dom::CapItem> &p2) const
  {
    return Compare(p1,p2) < 0;
  }

};


} // namespace dom
} // namespace mozilla
