/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */


interface Principal;
interface URI;

[Constructor(DOMString principal, unsigned long capType),
 Constructor(sequence<DOMString> principals, sequence<unsigned long> capType)]
interface CapLabel {

  boolean equals(CapLabel other);

  //boolean subsumes(CapLabel other);

  
  /**
   * Make a new copy of this Label.
  */

  [Throws]
  CapLabel clone();


  stringifier;
};
