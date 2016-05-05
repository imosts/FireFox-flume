/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

interface FLUME {

  // Enable confinement for compartment, if not enabled.
  static void enable();
  static boolean isEnabled();


  // Get the compartment principal, stringified
  static DOMString getPrincipal();

  [SetterThrows, GetterThrows] static attribute LabelSet labelSet;

  [Throws] static void addSecPrincipal(DOMString principal);
  [Throws] static void addIntPrincipal(DOMString principal);

  [Throws] static void delSecPrincipal(DOMString principal);
  [Throws] static void delIntPrincipal(DOMString principal);
};
