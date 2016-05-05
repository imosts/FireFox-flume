/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */


interface Principal;
interface URI;
//interface CapItem;
//interface Label;
//interface CapLabel;

[Constructor(Label s, Label i, CapLabel c),
 Constructor(LabelSet labelSet)]
interface LabelSet {

  /**
   * Returns whether the other Label is equivalent to this Label.
   * Labels are considered equal if they have the same principals.
   */
  boolean equals(LabelSet other);

  //boolean isFlumeSafe(LabelSet other);
  
  [Throws] Label getSecLabel();
  
  [Throws] Label getIntLabel();
  
  [Throws] CapLabel getCapLabel();
  
  [Throws] void addSecPrincipal(DOMString? principal);
  
  [Throws] void addSecPrincipal(Principal principal);
  
  [Throws] void delSecPrincipal(DOMString? principal);
  
  [Throws] void delSecPrincipal(Principal principal);
  
  [Throws] void addIntPrincipal(DOMString? principal);
  
  [Throws] void addIntPrincipal(Principal principal);
  
  [Throws] void delIntPrincipal(DOMString? principal);
  
  [Throws] void delIntPrincipal(Principal principal);
  
  /**
   * Make a new copy of this LabelSet.
   */
  [Throws]
  LabelSet clone();

  stringifier;
};
