/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/FLUME.h"
#include "mozilla/dom/FLUMEBinding.h"
//#include "mozilla/dom/RoleBinding.h"
#include "mozilla/dom/LabelSetBinding.h"
//#include "mozilla/dom/PrivilegeBinding.h"
#include "xpcprivate.h"

namespace mozilla {
namespace dom {

#define FLUME_CONFIG(compartment) \
    static_cast<xpc::CompartmentPrivate*>(JS_GetCompartmentPrivate((compartment)))->flumeConfig


NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(FLUME)
NS_IMPL_CYCLE_COLLECTING_ADDREF(FLUME)
NS_IMPL_CYCLE_COLLECTING_RELEASE(FLUME)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(FLUME)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

FLUME::FLUME()
{
}

FLUME::~FLUME()
{
}

JSObject*
FLUME::WrapObject(JSContext* aCx)
{
  return FLUMEBinding::Wrap(aCx, this);
}

FLUME*
FLUME::GetParentObject() const
{
  return nullptr; //TODO: return something sensible here
}

void
FLUME::Enable(const GlobalObject& global, JSContext *cx)
{
  if (IsEnabled(global)) return;

  JSCompartment *compartment =
    js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);
  xpc::flume::EnableCompartmentConfinement(compartment);
  // Start using FLUME wrappers:
  js::RecomputeWrappers(cx, js::AllCompartments(), js::AllCompartments());
}

bool
FLUME::IsEnabled(const GlobalObject& global)
{
  JSCompartment *compartment =
    js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);
  return xpc::flume::IsCompartmentConfined(compartment);
}

void
FLUME::GetPrincipal(const GlobalObject& global, JSContext* cx, nsString& retval)
{
  retval = NS_LITERAL_STRING("");
  JSCompartment *compartment =
    js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);

  nsIPrincipal* prin = xpc::GetCompartmentPrincipal(compartment);
  if (!prin) return;

  char *origin = NULL;
  if (NS_FAILED(prin->GetOrigin(&origin)))
    return;
  AppendASCIItoUTF16(origin, retval);
  NS_Free(origin);
}

already_AddRefed<LabelSet>
FLUME::GetLabelSet(const GlobalObject& global, JSContext* cx,
                      ErrorResult& aRv)
{
  aRv.MightThrowJSException();
  if (!IsEnabled(global)) {
    JSErrorResult(cx, aRv, "FLUME is not enabled.");
    return nullptr;
  }

  JSCompartment *compartment = js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);
  nsRefPtr<LabelSet> l = xpc::flume::GetCompartmentLabelSet(compartment);

  if (!l) return nullptr;
  nsRefPtr<LabelSet> clone = l->Clone(aRv);
  if (aRv.Failed()) return nullptr;
  return clone.forget();
}

void
FLUME::SetLabelSet(const GlobalObject& global, JSContext* cx,
                    LabelSet& aLabelSet, ErrorResult& aRv)
{
  aRv.MightThrowJSException();
  Enable(global, cx);

  JSCompartment *compartment = js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);

  NS_ASSERTION(false, "!!!FLUME::SetLabelSet!!! : point 1");
  xpc::flume::SetCompartmentLabelSet(compartment, &aLabelSet);
  NS_ASSERTION(false, "!!!FLUME::SetLabelSet!!! : point 2");
  // This affects cross-compartment communication. Adjust wrappers:
  js::RecomputeWrappers(cx, js::AllCompartments(), js::AllCompartments());
  xpc::flume::RefineCompartmentCSP(compartment);
}

void
FLUME::AddSecPrincipal(const GlobalObject& global,
                                      JSContext* cx,
                                      const nsAString& principal,
                                      ErrorResult& aRv)
{
  aRv.MightThrowJSException();
  if (!IsEnabled(global)) {
    JSErrorResult(cx, aRv, "FLUME is not enabled.");
    return;
  }

  JSCompartment *compartment = js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);

  xpc::flume::AddCompartmentSec(compartment, principal);
  // This affects cross-compartment communication. Adjust wrappers:
  js::RecomputeWrappers(cx, js::AllCompartments(), js::AllCompartments());
  xpc::flume::RefineCompartmentCSP(compartment);

}

void
FLUME::AddIntPrincipal(const GlobalObject& global,
                                      JSContext* cx,
                                      const nsAString& principal,
                                      ErrorResult& aRv)
{
  aRv.MightThrowJSException();
  if (!IsEnabled(global)) {
    JSErrorResult(cx, aRv, "FLUME is not enabled.");
    return;
  }

  JSCompartment *compartment = js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);

  xpc::flume::AddCompartmentInt(compartment, principal);
  // This affects cross-compartment communication. Adjust wrappers:
  js::RecomputeWrappers(cx, js::AllCompartments(), js::AllCompartments());
  xpc::flume::RefineCompartmentCSP(compartment);
}

void
FLUME::DelSecPrincipal(const GlobalObject& global,
                                      JSContext* cx,
                                      const nsAString& principal,
                                      ErrorResult& aRv)
{
  aRv.MightThrowJSException();
  if (!IsEnabled(global)) {
    JSErrorResult(cx, aRv, "FLUME is not enabled.");
    return;
  }

  JSCompartment *compartment = js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);

  xpc::flume::DelCompartmentSec(compartment, principal);
  // This affects cross-compartment communication. Adjust wrappers:
  js::RecomputeWrappers(cx, js::AllCompartments(), js::AllCompartments());
  xpc::flume::RefineCompartmentCSP(compartment);
}

void
FLUME::DelIntPrincipal(const GlobalObject& global,
                                      JSContext* cx,
                                      const nsAString& principal,
                                      ErrorResult& aRv)
{
  aRv.MightThrowJSException();
  if (!IsEnabled(global)) {
    JSErrorResult(cx, aRv, "FLUME is not enabled.");
    return;
  }

  JSCompartment *compartment = js::GetObjectCompartment(global.Get());
  MOZ_ASSERT(compartment);

  xpc::flume::DelCompartmentInt(compartment, principal);
  // This affects cross-compartment communication. Adjust wrappers:
  js::RecomputeWrappers(cx, js::AllCompartments(), js::AllCompartments());
  xpc::flume::RefineCompartmentCSP(compartment);
}

// Helper for setting the ErrorResult to a string.  This function
// should only be called after MightThrowJSException() is called.
void
FLUME::JSErrorResult(JSContext *cx, ErrorResult& aRv, const char *msg)
{
  JSString *err = JS_NewStringCopyZ(cx,msg);
  if (err) {
    JS::RootedValue errv(cx, STRING_TO_JSVAL(err));
    aRv.ThrowJSException(cx,errv);
  } else {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
  }
}

} // namespace dom
} // namespace mozilla
