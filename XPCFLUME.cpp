/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Assertions.h"
#include "xpcprivate.h"
#include "xpcpublic.h"
#include "jsfriendapi.h"
#include "mozilla/dom/FLUME.h"
#include "mozilla/dom/Label.h"
#include "mozilla/dom/LabelSet.h"
#include "nsIContentSecurityPolicy.h"
#include "nsDocument.h"

using namespace xpc;
using namespace JS;
using namespace mozilla;
using namespace mozilla::dom;

namespace xpc {
namespace flume {

#define FLUME_CONFIG(compartment) \
    static_cast<CompartmentPrivate*>(JS_GetCompartmentPrivate((compartment)))->flumeConfig

NS_EXPORT_(void)
EnableCompartmentConfinement(JSCompartment *compartment)
{
  MOZ_ASSERT(compartment);

  NS_ASSERTION(false, "!!!EnableCompartmentConfinement!!! : point 1");
  if (IsCompartmentConfined(compartment))
    return;
  NS_ASSERTION(false, "!!!EnableCompartmentConfinement!!! : point 2");
  nsRefPtr<LabelSet> labelSet = new LabelSet();
  NS_ASSERTION(false, "!!!EnableCompartmentConfinement!!! : point 3");
  MOZ_ASSERT(labelSet);
  NS_ASSERTION(false, "!!!EnableCompartmentConfinement!!! : point 4");
  FLUME_CONFIG(compartment).SetLabelSet(labelSet);
}

NS_EXPORT_(bool)
IsCompartmentConfined(JSCompartment *compartment)
{
  MOZ_ASSERT(compartment);
  return FLUME_CONFIG(compartment).isEnabled();
}

  NS_EXPORT_(void)
  SetCompartmentLabelSet(JSCompartment *compartment,
                      mozilla::dom::LabelSet *aLabelSet)
  {
    MOZ_ASSERT(compartment);
    MOZ_ASSERT(aLabelSet);


    NS_ASSERTION(false, "!!!SetCompartmentLabelSet!!! : point 1");
    NS_ASSERTION(IsCompartmentConfined(compartment),
                 "Must call EnableCompartmentConfinement() first");
    if (!IsCompartmentConfined(compartment))
      return;
    NS_ASSERTION(false, "!!!SetCompartmentLabelSet!!! : point 2");
    ErrorResult aRv;
    nsRefPtr<LabelSet> labelSet = (aLabelSet)->Clone(aRv);
    NS_ASSERTION(false, "!!!SetCompartmentLabelSet!!! : point 3");
    MOZ_ASSERT(!(aRv).Failed());
    FLUME_CONFIG(compartment).SetLabelSet(labelSet);
    NS_ASSERTION(false, "!!!SetCompartmentLabelSet!!! : point 4");
  }


  NS_EXPORT_(already_AddRefed<mozilla::dom::LabelSet>)
  GetCompartmentLabelSet(JSCompartment *compartment)
  {
    MOZ_ASSERT(compartment);
    MOZ_ASSERT(flume::IsCompartmentConfined(compartment));
    return FLUME_CONFIG(compartment).GetLabelSet();
  }

  NS_EXPORT_(void)
  AddCompartmentSec(JSCompartment *compartment,
                      const nsAString& principal)
  {
    MOZ_ASSERT(compartment);
    MOZ_ASSERT(principal[0] != '\0');

    NS_ASSERTION(IsCompartmentConfined(compartment),
                 "Must call EnableCompartmentConfinement() first");
    if (!IsCompartmentConfined(compartment))
      return;

    FLUME_CONFIG(compartment).AddSecLabel(principal);
  }

  NS_EXPORT_(void)
  AddCompartmentInt(JSCompartment *compartment,
                      const nsAString& principal)
  {
    MOZ_ASSERT(compartment);
    MOZ_ASSERT(principal[0] != '\0');

    NS_ASSERTION(IsCompartmentConfined(compartment),
                 "Must call EnableCompartmentConfinement() first");
    if (!IsCompartmentConfined(compartment))
      return;

    FLUME_CONFIG(compartment).AddIntLabel(principal);
  }

  NS_EXPORT_(void)
  DelCompartmentSec(JSCompartment *compartment,
                      const nsAString& principal)
  {
    MOZ_ASSERT(compartment);
    MOZ_ASSERT(principal[0] != '\0');

    NS_ASSERTION(IsCompartmentConfined(compartment),
                 "Must call EnableCompartmentConfinement() first");
    if (!IsCompartmentConfined(compartment))
      return;

    FLUME_CONFIG(compartment).DelSecLabel(principal);
  }

  NS_EXPORT_(void)
  DelCompartmentInt(JSCompartment *compartment,
                      const nsAString& principal)
  {
    MOZ_ASSERT(compartment);
    MOZ_ASSERT(principal[0] != '\0');

    NS_ASSERTION(IsCompartmentConfined(compartment),
                 "Must call EnableCompartmentConfinement() first");
    if (!IsCompartmentConfined(compartment))
      return;

    FLUME_CONFIG(compartment).DelIntLabel(principal);
  }

// Check if compartment can write to dst
NS_EXPORT_(bool)
GuardWrite(JSCompartment *compartment, JSCompartment *dst)
{
#if FLUME_DEBUG
    {
        printf("GuardWrite :");
        {
            char *origin;
            uint32_t status = 0;
            GetCompartmentPrincipal(compartment)->GetOrigin(&origin);
            GetCompartmentPrincipal(compartment)->GetAppId(&status);
            printf(" %s [%x] to", origin, status);
            nsMemory::Free(origin);
        }
        {
            char *origin;
            uint32_t status = 0;
            GetCompartmentPrincipal(dst)->GetOrigin(&origin);
            GetCompartmentPrincipal(dst)->GetAppId(&status);
            printf("%s [%x] \n", origin, status);
            nsMemory::Free(origin);
        }
    }
#endif


  if (!IsCompartmentConfined(dst)) {
    NS_WARNING("Destination compartmetn is not confined");
    return false;
  }

  return FLUME_CONFIG(compartment).IsFlumeCompartmentSafe(dst);
}


// Check if information can flow from compartment |source| to
// compartment |compartment|.
NS_EXPORT_(bool)
GuardRead(JSCompartment *compartment, JSCompartment *source)
{
#if FLUME_DEBUG
    {
        printf("GuardRead :");
        {
            char *origin;
            uint32_t status = 0;
            GetCompartmentPrincipal(source)->GetOrigin(&origin);
            GetCompartmentPrincipal(source)->GetAppId(&status);
            printf(" %s [%x] to", origin, status);
            nsMemory::Free(origin);
        }
        {
            char *origin;
            uint32_t status = 0;
            GetCompartmentPrincipal(compartment)->GetOrigin(&origin);
            GetCompartmentPrincipal(compartment)->GetAppId(&status);
            printf("%s [%x] \n", origin, status);
            nsMemory::Free(origin);
        }
    }
#endif


  if (!IsCompartmentConfined(source)) {
    NS_WARNING("Source compartmetn is not confined");
    return false;
  }

  return FLUME_CONFIG(source).IsFlumeCompartmentSafe(compartment);
}



static inline uint32_t flumeSandboxFlags() {
  // Sandbox flags
  nsAttrValue sandboxAttr;
  sandboxAttr.ParseAtomArray(NS_LITERAL_STRING("allow-scripts allow-forms"));
  return nsContentUtils::ParseSandboxAttributeToFlags(&sandboxAttr);
}

// This function adjusts the "security permieter".
// Specifically, it adjusts:
// 1. The CSP policy to restrict with whom the current compartment may
// network-communicate with.
// 2. The compartment principal to restrict writing to storage
// cnannels.
//
NS_EXPORT_(void)
RefineCompartmentCSP(JSCompartment *compartment)
{
  nsresult rv;
  ErrorResult aRv;
  // Get the compartment security label:
  nsRefPtr<LabelSet> labelSet = GetCompartmentLabelSet(compartment);
  nsRefPtr<Label> security = labelSet->GetSecLabel(aRv);

  // Get the compartment principal
  nsCOMPtr<nsIPrincipal> compPrincipal = GetCompartmentPrincipal(compartment);

  // Get underlying CSP
  nsCOMPtr<nsIContentSecurityPolicy> csp;
  rv = compPrincipal->GetCsp(getter_AddRefs(csp));
  MOZ_ASSERT(NS_SUCCEEDED(rv));

  // If there is a CSP and we set it before, remove our existing policy
  if (csp) {
    uint32_t numPolicies = 0;
    rv = csp->GetPolicyCount(&numPolicies);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    uint32_t flumeCSPPolicy = FLUME_CONFIG(compartment).mCSPIndex ;
    if (flumeCSPPolicy > 0 && flumeCSPPolicy <= numPolicies) {
      rv = csp->RemovePolicy(flumeCSPPolicy-1);
      MOZ_ASSERT(NS_SUCCEEDED(rv));
      FLUME_CONFIG(compartment).mCSPIndex = 0;
    }
  }

  // Get the compartment global
  nsCOMPtr<nsIGlobalObject> global =
    NativeGlobal(JS_GetGlobalForCompartmentOrNull(compartment));
  MOZ_ASSERT(global);

  // Get the underlying window, if it exists
  nsCOMPtr<nsIDOMWindow> win(do_QueryInterface(global));
  nsCOMPtr<nsIDOMDocument> domDoc;
  nsCOMPtr<nsIDocument> doc;

  // Channel for reporting CSP violations, if there is an underlying
  // document:
  nsCOMPtr<nsIChannel> reportChan;

  if (win) {
    // Get the window document
    win->GetDocument(getter_AddRefs(domDoc));
    MOZ_ASSERT(domDoc);

    doc = do_QueryInterface(domDoc);
    MOZ_ASSERT(doc);

    // Set the report channel to the document channel
    reportChan = doc->GetChannel();

    // Reset sandbox flags, if set
    if (doc->GetSandboxFlags() == flumeSandboxFlags() &&
        FLUME_CONFIG(compartment).SetSandboxFlags()) {
      doc->SetSandboxFlags(FLUME_CONFIG(compartment).GetSandboxFlags());
      FLUME_CONFIG(compartment).ClearSandboxFlags();
    }
  }

  // Case 1: Empty/public label, don't loosen/impose new restrictions
  if (security->IsEmpty()) {
//#if FLUME_DEBUG
    printf("Refine: Security label is empty, do nothing\n");
//#endif
    return;
  }

  nsString policy;
  PrincipalArray* labelPrincipals = security->GetDirectPrincipals();

    // Create list of origins
    nsString origins;
    for (unsigned i = 0; i < labelPrincipals->Length(); ++i) {
      char *origin = NULL;
      rv = labelPrincipals->ElementAt(i)->GetOrigin(&origin);
      MOZ_ASSERT(NS_SUCCEEDED(rv));
      AppendASCIItoUTF16(origin, origins);
      NS_Free(origin);
      origins.Append(NS_LITERAL_STRING(" "));
    }

    policy = NS_LITERAL_STRING("default-src 'unsafe-inline' 'unsafe-eval' ")  + origins
           + NS_LITERAL_STRING(";script-src 'unsafe-inline' 'unsafe-eval' ")  + origins
           + NS_LITERAL_STRING(";object-src ")                  + origins
           + NS_LITERAL_STRING(";style-src 'unsafe-inline' ")   + origins
           + NS_LITERAL_STRING(";img-src ")                     + origins
           + NS_LITERAL_STRING(";media-src ")                   + origins
           + NS_LITERAL_STRING(";frame-src ")                   + origins
           + NS_LITERAL_STRING(";font-src ")                    + origins
           + NS_LITERAL_STRING(";connect-src ")                 + origins
           + NS_LITERAL_STRING(";");
//#if FLUME_DEBUG
    printf("Refine: Security label is disjunctive\n");
//#endif


  // A. Set sandbox flags to disallow storage access

  if (doc) {
    // Save current flags:
    FLUME_CONFIG(compartment).SetSandboxFlags(doc->GetSandboxFlags());
    doc->SetSandboxFlags(flumeSandboxFlags());
  }

  // B. Set new CSP policy

  // Principal doesn't have a CSP, create it:
  if (!csp) {
    csp = do_CreateInstance("@mozilla.org/cspcontext;1", &rv);
    MOZ_ASSERT(NS_SUCCEEDED(rv) && csp);
    // Set the csp since we create a new principal
    rv = compPrincipal->SetCsp(csp);
    MOZ_ASSERT(NS_SUCCEEDED(rv));

    // Get the principal URI
    nsCOMPtr<nsIURI> baseURI;
    rv = compPrincipal->GetURI(getter_AddRefs(baseURI));
    MOZ_ASSERT(NS_SUCCEEDED(rv));

    rv = csp->SetRequestContext(baseURI, nullptr, reportChan);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }

  // Append policy
  rv = csp->AppendPolicy(policy, false);
  MOZ_ASSERT(NS_SUCCEEDED(rv));

  // Track which policy is FLUMEs
  rv = csp->GetPolicyCount(&FLUME_CONFIG(compartment).mCSPIndex);
  MOZ_ASSERT(NS_SUCCEEDED(rv));

 //#ifdef FLUME_DEBUG
  printf("Refine: appended policy [%d] to principal %p [csp=%p]: %s\n", FLUME_CONFIG(compartment).mCSPIndex, compPrincipal.get(), csp.get(), NS_ConvertUTF16toUTF8(policy).get());
 //#endif


//#ifdef FLUME_DEBUG
  {
    unsigned numPolicies = 0;
    if (csp) {
      nsresult rv = csp->GetPolicyCount(&numPolicies);
      MOZ_ASSERT(NS_SUCCEEDED(rv));
      printf("Refine: Number of CSP policies: %d\n", numPolicies);
      for (unsigned i=0; i<numPolicies; i++) {
        nsAutoString policy;
        csp->GetPolicy(i, policy);
        printf("Refine: Principal has CSP[%d]: %s\n", i,
            NS_ConvertUTF16toUTF8(policy).get());
      }
    }
  }
//#endif

}



#undef FLUME_CONFIG

}; // flume
}; // xpc
