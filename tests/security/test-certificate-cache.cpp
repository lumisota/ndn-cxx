/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Yingdi Yu <yingdi0@cs.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#include "security/certificate-cache-ttl.hpp"
#include "face.hpp"

#include "boost-test.hpp"

using namespace std;

namespace ndn {

BOOST_AUTO_TEST_SUITE(TestCertificateCache)

void
getCertificateTtl(shared_ptr<CertificateCacheTtl> cache, const Name &name, bool cached)
{
  BOOST_CHECK_EQUAL(static_cast<bool>(cache->getCertificate(name)), cached);
}


BOOST_AUTO_TEST_CASE (Ttl)
{
  shared_ptr<boost::asio::io_service> io = make_shared<boost::asio::io_service>();
  shared_ptr<CertificateCacheTtl> cache = make_shared<CertificateCacheTtl>(io, time::seconds(1));
  Scheduler scheduler(*io);

  shared_ptr<IdentityCertificate> cert1 = make_shared<IdentityCertificate>();
  Name certName1("/tmp/KEY/ksk-1/ID-CERT/1");
  cert1->setName(certName1);
  cert1->setFreshnessPeriod(time::milliseconds(500));
  shared_ptr<IdentityCertificate> cert2 = make_shared<IdentityCertificate>();
  Name certName2("/tmp/KEY/ksk-2/ID-CERT/2");
  cert2->setName(certName2);
  cert2->setFreshnessPeriod(time::milliseconds(1000));

  Name name1 = certName1.getPrefix(-1);
  Name name2 = certName2.getPrefix(-1);

  cache->insertCertificate(cert1);
  cache->insertCertificate(cert2);

  scheduler.scheduleEvent(time::milliseconds(200),  bind(&getCertificateTtl, cache, name1, true));
  scheduler.scheduleEvent(time::milliseconds(200),  bind(&getCertificateTtl, cache, name2, true));
  scheduler.scheduleEvent(time::milliseconds(900),  bind(&getCertificateTtl, cache, name1, false));
  scheduler.scheduleEvent(time::milliseconds(900),  bind(&getCertificateTtl, cache, name2, true));
  scheduler.scheduleEvent(time::milliseconds(900),  bind(&CertificateCache::insertCertificate, cache, cert2));
  scheduler.scheduleEvent(time::milliseconds(1500), bind(&getCertificateTtl, cache, name2, true));
  scheduler.scheduleEvent(time::milliseconds(2500), bind(&getCertificateTtl, cache, name2, false));

  io->run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
