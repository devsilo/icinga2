/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#include "base/utility.hpp"
#include <chrono>
#include <BoostTestTargetConfig.h>

using namespace icinga;

BOOST_AUTO_TEST_SUITE(base_utility)

BOOST_AUTO_TEST_CASE(parse_version)
{
	BOOST_CHECK(Utility::ParseVersion("2.11.0-0.rc1.1") == "2.11.0");
	BOOST_CHECK(Utility::ParseVersion("v2.10.5") == "2.10.5");
	BOOST_CHECK(Utility::ParseVersion("r2.11.1") == "2.11.1");
	BOOST_CHECK(Utility::ParseVersion("v2.11.0-rc1-58-g7c1f716da") == "2.11.0");

	BOOST_CHECK(Utility::ParseVersion("v2.11butactually3.0") == "v2.11butactually3.0");
}

BOOST_AUTO_TEST_CASE(compare_version)
{
	BOOST_CHECK(Utility::CompareVersion("2.10.5", Utility::ParseVersion("v2.10.4")) < 0);
	BOOST_CHECK(Utility::CompareVersion("2.11.0", Utility::ParseVersion("2.11.0-0")) == 0);
	BOOST_CHECK(Utility::CompareVersion("2.10.5", Utility::ParseVersion("2.11.0-0.rc1.1")) > 0);
}

BOOST_AUTO_TEST_CASE(comparepasswords_works)
{
	BOOST_CHECK(Utility::ComparePasswords("", ""));

	BOOST_CHECK(!Utility::ComparePasswords("x", ""));
	BOOST_CHECK(!Utility::ComparePasswords("", "x"));

	BOOST_CHECK(Utility::ComparePasswords("x", "x"));
	BOOST_CHECK(!Utility::ComparePasswords("x", "y"));

	BOOST_CHECK(Utility::ComparePasswords("abcd", "abcd"));
	BOOST_CHECK(!Utility::ComparePasswords("abc", "abcd"));
	BOOST_CHECK(!Utility::ComparePasswords("abcde", "abcd"));
}

BOOST_AUTO_TEST_CASE(comparepasswords_issafe)
{
	using std::chrono::duration_cast;
	using std::chrono::microseconds;
	using std::chrono::steady_clock;

	String a, b;

	a.Append(200000001, 'a');
	b.Append(200000002, 'b');

	auto start1 (steady_clock::now());

	Utility::ComparePasswords(a, a);

	auto duration1 (steady_clock::now() - start1);

	auto start2 (steady_clock::now());

	Utility::ComparePasswords(a, b);

	auto duration2 (steady_clock::now() - start2);

	double diff = (double)duration_cast<microseconds>(duration1).count() / (double)duration_cast<microseconds>(duration2).count();
	BOOST_WARN(0.9 <= diff && diff <= 1.1);
}

BOOST_AUTO_TEST_CASE(comparebcryptpasswords)
{
	BOOST_CHECK(Utility::CompareBCryptPasswords("123456", "$2y$10$uJUA0HVV3lHVVE0LsxPYZ.ioK2b7Lt82h6vwpgVBQSPqYjL7Jke7q"));
	BOOST_CHECK(!Utility::CompareBCryptPasswords("123457", "$2y$10$uJUA0HVV3lHVVE0LsxPYZ.ioK2b7Lt82h6vwpgVBQSPqYjL7Jke7q"));
	BOOST_CHECK(!Utility::CompareBCryptPasswords("123457", "invalid"));
}

BOOST_AUTO_TEST_CASE(validateutf8)
{
	BOOST_CHECK(Utility::ValidateUTF8("") == "");
	BOOST_CHECK(Utility::ValidateUTF8("a") == "a");
	BOOST_CHECK(Utility::ValidateUTF8("\xC3") == "\xEF\xBF\xBD");
	BOOST_CHECK(Utility::ValidateUTF8("\xC3\xA4") == "\xC3\xA4");
}

BOOST_AUTO_TEST_SUITE_END()
