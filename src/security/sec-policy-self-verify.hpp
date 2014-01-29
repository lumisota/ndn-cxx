/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_SEC_POLICY_SELF_VERIFY_HPP
#define NDN_SEC_POLICY_SELF_VERIFY_HPP

#include "sec-policy.hpp"

namespace ndn {

/**
 * A SecPolicySelfVerify implements a PolicyManager to use the public key DER in the data packet's KeyLocator (if available)
 * or look in the IdentityStorage for the public key with the name in the KeyLocator (if available) and use
 * it to verify the data packet, without searching a certificate chain.  If the public key can't be found, the
 * verification fails.
 */
class SecPolicySelfVerify : public SecPolicy {
public:
  /**
   * Create a new SecPolicySelfVerify which will look up the public key in the given identityManager.
   * @param identityManager (optional) The IdentityManager for looking up the public key.  This points to an object must which remain 
   * valid during the life of this SecPolicySelfVerify.  If omitted, then don't look for a public key with the name 
   * in the KeyLocator and rely on the KeyLocator having the full public key DER.
   */
  SecPolicySelfVerify()
  {
  }
  
  /**
   * The virtual destructor.
   */
  virtual
  ~SecPolicySelfVerify();

  /**
   * Check whether the received data packet complies with the verification policy, and get the indication of the next verification step.
   * If there is no next verification step, that imlies policy MUST have already made the verification decision.
   * i.e., either onVerified or onVerifyFailed callback is invoked.
   * @param data The Data object with the signature to check.
   * @param stepCount The number of verification steps that have been done, used to track the verification progress.
   * @param onVerified If the signature is verified, this calls onVerified(data).
   * @param onVerifyFailed If the signature check fails, this calls onVerifyFailed(data).
   * @return the indication of next verification step, null if there is no further step.
   */
  virtual ptr_lib::shared_ptr<ValidationRequest>
  checkVerificationPolicy
    (const ptr_lib::shared_ptr<const Data>& data, int stepCount, const OnVerified& onVerified, const OnVerifyFailed& onVerifyFailed);

  /**
   * Check whether the received interest packet complies with the verification policy, and get the indication of the next verification step.
   * If there is no next verification step, that implies policy MUST have already made the verification decision.
   * i.e., either onVerified or onVerifyFailed callback is invoked.
   * @param data The Data object with the signature to check.
   * @param stepCount The number of verification steps that have been done, used to track the verification progress.
   * @param onVerified If the signature is verified, this calls onVerified(data).
   * @param onVerifyFailed If the signature check fails, this calls onVerifyFailed(data).
   * @return the indication of next verification step, null if there is no further step.
   */
  virtual ptr_lib::shared_ptr<ValidationRequest>
  checkVerificationPolicy
    (const ptr_lib::shared_ptr<const Interest>& interest, int stepCount, const OnVerified& onVerified, const OnVerifyFailed& onVerifyFailed);
};

}

#endif