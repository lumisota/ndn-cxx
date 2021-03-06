/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2014 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include "signature-info.hpp"

#include "boost-test.hpp"

namespace ndn {

BOOST_AUTO_TEST_SUITE(TestSignatureInfo)

const uint8_t sigInfoRsa[] = {
0x16, 0x1b, // SignatureInfo
  0x1b, 0x01, // SignatureType
    0x01, // Sha256WithRsa
  0x1c, 0x16, // KeyLocator
    0x07, 0x14, // Name
      0x08, 0x04,
        0x74, 0x65, 0x73, 0x74,
      0x08, 0x03,
        0x6b, 0x65, 0x79,
      0x08, 0x07,
        0x6c, 0x6f, 0x63, 0x61, 0x74, 0x6f, 0x72
};

BOOST_AUTO_TEST_CASE(Constructor)
{
  SignatureInfo info;
  BOOST_CHECK_EQUAL(info.getSignatureType(), -1);
  BOOST_CHECK_EQUAL(info.hasKeyLocator(), false);
  BOOST_CHECK_THROW(info.getKeyLocator(), SignatureInfo::Error);

  SignatureInfo sha256Info(tlv::DigestSha256);
  BOOST_CHECK_EQUAL(sha256Info.getSignatureType(), tlv::DigestSha256);
  BOOST_CHECK_EQUAL(sha256Info.hasKeyLocator(), false);
  BOOST_CHECK_THROW(sha256Info.getKeyLocator(), SignatureInfo::Error);

  KeyLocator keyLocator("/test/key/locator");
  SignatureInfo sha256RsaInfo(tlv::SignatureSha256WithRsa, keyLocator);
  BOOST_CHECK_EQUAL(sha256RsaInfo.getSignatureType(), tlv::SignatureSha256WithRsa);
  BOOST_CHECK_EQUAL(sha256RsaInfo.hasKeyLocator(), true);
  BOOST_CHECK_NO_THROW(sha256RsaInfo.getKeyLocator());
  BOOST_CHECK_EQUAL(sha256RsaInfo.getKeyLocator().getName(), Name("/test/key/locator"));

  const Block& encoded = sha256RsaInfo.wireEncode();
  Block sigInfoBlock(sigInfoRsa, sizeof(sigInfoRsa));

  BOOST_CHECK_EQUAL_COLLECTIONS(sigInfoBlock.wire(),
                                sigInfoBlock.wire() + sigInfoBlock.size(),
                                encoded.wire(),
                                encoded.wire() + encoded.size());

  sha256RsaInfo = SignatureInfo(sigInfoBlock);
  BOOST_CHECK_EQUAL(sha256RsaInfo.getSignatureType(), tlv::SignatureSha256WithRsa);
  BOOST_CHECK_EQUAL(sha256RsaInfo.hasKeyLocator(), true);
  BOOST_CHECK_NO_THROW(sha256RsaInfo.getKeyLocator());
  BOOST_CHECK_EQUAL(sha256RsaInfo.getKeyLocator().getName(), Name("/test/key/locator"));
}

BOOST_AUTO_TEST_CASE(ConstructorError)
{
  const uint8_t error1[] = {
    0x15, 0x1b, // Wrong SignatureInfo (0x16, 0x1b)
      0x1b, 0x01, // SignatureType
        0x01, // Sha256WithRsa
      0x1c, 0x16, // KeyLocator
        0x07, 0x14, // Name
          0x08, 0x04,
            0x74, 0x65, 0x73, 0x74,
          0x08, 0x03,
            0x6b, 0x65, 0x79,
          0x08, 0x07,
            0x6c, 0x6f, 0x63, 0x61, 0x74, 0x6f, 0x72
  };
  Block errorBlock1(error1, sizeof(error1));
  BOOST_CHECK_THROW(SignatureInfo info(errorBlock1), tlv::Error);

  const uint8_t error2[] = {
    0x16, 0x01, // SignatureInfo
      0x01 // Wrong SignatureInfo value
  };
  Block errorBlock2(error2, sizeof(error2));
  BOOST_CHECK_THROW(SignatureInfo info(errorBlock2), tlv::Error);

  const uint8_t error3[] = {
    0x16, 0x01, // SignatureInfo
      0x1a, 0x01, // Wrong SignatureType (0x1b, 0x1b)
        0x01, // Sha256WithRsa
      0x1c, 0x16, // KeyLocator
        0x07, 0x14, // Name
          0x08, 0x04,
            0x74, 0x65, 0x73, 0x74,
          0x08, 0x03,
            0x6b, 0x65, 0x79,
          0x08, 0x07,
            0x6c, 0x6f, 0x63, 0x61, 0x74, 0x6f, 0x72
  };
  Block errorBlock3(error3, sizeof(error3));
  BOOST_CHECK_THROW(SignatureInfo info(errorBlock3), tlv::Error);

  const uint8_t error4[] = {
    0x16, 0x00 // Empty SignatureInfo
  };
  Block errorBlock4(error4, sizeof(error4));
  BOOST_CHECK_THROW(SignatureInfo info(errorBlock4), tlv::Error);

}

BOOST_AUTO_TEST_CASE(SetterGetter)
{
  SignatureInfo info;
  BOOST_CHECK_EQUAL(info.getSignatureType(), -1);
  BOOST_CHECK_EQUAL(info.hasKeyLocator(), false);
  BOOST_CHECK_THROW(info.getKeyLocator(), SignatureInfo::Error);

  info.setSignatureType(tlv::SignatureSha256WithRsa);
  BOOST_CHECK_EQUAL(info.getSignatureType(), tlv::SignatureSha256WithRsa);
  BOOST_CHECK_EQUAL(info.hasKeyLocator(), false);

  KeyLocator keyLocator("/test/key/locator");
  info.setKeyLocator(keyLocator);
  BOOST_CHECK_EQUAL(info.hasKeyLocator(), true);
  BOOST_CHECK_NO_THROW(info.getKeyLocator());
  BOOST_CHECK_EQUAL(info.getKeyLocator().getName(), Name("/test/key/locator"));

  const Block& encoded = info.wireEncode();
  Block sigInfoBlock(sigInfoRsa, sizeof(sigInfoRsa));

  BOOST_CHECK_EQUAL_COLLECTIONS(sigInfoBlock.wire(),
                                sigInfoBlock.wire() + sigInfoBlock.size(),
                                encoded.wire(),
                                encoded.wire() + encoded.size());

  info.unsetKeyLocator();
  BOOST_CHECK_EQUAL(info.hasKeyLocator(), false);
  BOOST_CHECK_THROW(info.getKeyLocator(), SignatureInfo::Error);
}

BOOST_AUTO_TEST_CASE(OtherTlvs)
{
  SignatureInfo info;
  BOOST_CHECK_EQUAL(info.getSignatureType(), -1);
  BOOST_CHECK_EQUAL(info.hasKeyLocator(), false);
  BOOST_CHECK_THROW(info.getKeyLocator(), SignatureInfo::Error);

  const uint8_t tlv1[] = {
    0x81, // T
    0x01, // L
    0x01, // V
  };
  Block block1(tlv1, sizeof(tlv1));

  info.appendTypeSpecificTlv(block1);
  BOOST_CHECK_THROW(info.getTypeSpecificTlv(0x82), SignatureInfo::Error);
  BOOST_REQUIRE_NO_THROW(info.getTypeSpecificTlv(0x81));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
