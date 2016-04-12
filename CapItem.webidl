/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */


interface Principal;
interface URI;

[Constructor(DOMString principal, unsigned long capType)]
interface CapItem {

  /**
   * Returns whether the other Label is equivalent to this Label.
   * Labels are considered equal if they have the same principals.
   */
  boolean equals(CapItem other);


  /**
   * Make a new copy of this Label.
   */

  [Throws]
  CapItem clone();

};
