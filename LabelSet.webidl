/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */


interface Principal;
interface URI;
interface CapItem;
interface Label;
interface CapLabel;

[Constructor(Label s, Label i, CapLabel c),
 Constructor(LabelSet labelSet,)]
interface LabelSet {

  /**
   * Returns whether the other Label is equivalent to this Label.
   * Labels are considered equal if they have the same principals.
   */
  boolean equals(LabelSet other);

  boolean isFlumeSafe(LabelSet other);
  
  static Label getSecLabel();
  
  static Label getIntLabel();
  
  static CapLabel getCapLabel();
  
  [Throws] void addSecPrincipal(const DOMString? principal);
  
  [Throws] void addSecPrincipal(Principal principal);
  
  [Throws] void delSecPrincipal(const DOMString? principal);
  
  [Throws] void delSecPrincipal(Principal principal);
  
  [Throws] void dddIntPrincipal(const DOMString? principal);
  
  [Throws] void addIntPrincipal(Principal principal);
  
  [Throws] void delIntPrincipal(const DOMString? principal);
  
  [Throws] void delIntPrincipal(Principal principal);
  
  /**
   * Make a new copy of this Label.
   */
  [Throws]
  Label clone();

  stringifier;
};