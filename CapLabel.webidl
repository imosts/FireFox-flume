/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */


interface Principal;
interface URI;
interface CapItem;

[Constructor(DOMString principal, unsigned capType),
 Constructor(sequence<DOMString> principals), Sequence<uint32_t>& capType,
 Constructor(Principal principal, unsigned capType),
 Constructor(CapItem capType)]
interface CapLabel {

  boolean equals(CapLabel other);

  //boolean subsumes(CapLabel other);

  
  /**
   * Make a new copy of this Label.
  [Throws]
  CapLabel clone();
  */

  stringifier;
};