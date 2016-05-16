/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */


interface Principal;
interface URI;

[Constructor,
 Constructor(DOMString principal),
 Constructor(sequence<DOMString> principals)]
interface Label {

  /**
   * Returns whether the other Label is equivalent to this Label.
   * Labels are considered equal if they have the same principals.
   */
  boolean equals(Label other);

  /**
   * Returns whether the other Label is equal to or weaker than this
   * Label.
   *
   * This Label subsumes the other Label if the set of principals it
   * contains is a (non-empty) subset of other's principals. That is,
   * the other Label must have all the principals of this Label. An
   * empty Label only subsumes and is subsumed by another empty
   * Label.
   *
   * Thus a Label always subsumes itself.
   */
  boolean subsumes(Label other);

  /**
   * Add principal to this Label and return the Label.
   */

   /*
  [Throws] Label _and(DOMString principal);
  [Throws] Label _and(Principal principal);
  [Throws] Label _and(Label other);
*/
  /**
   * Make a new copy of this Label.
   */
  [Throws]
  Label clone();

  stringifier;
};
