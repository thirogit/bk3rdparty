// Copyright (c) 2015 Erwin Jansen
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef SRC_INCLUDE_JWT_RSASIGNER_H_
#define SRC_INCLUDE_JWT_RSASIGNER_H_

#include "jwt/messagesigner.h"
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>
#include <string>

/**
 * The RSAValidator can sign and validate the RSASSA-PKCX-v1_5 family of
 * signature algorithms.
 */
class RSASigner : public MessageSigner {
	using json = nlohmann::json;
public:
  explicit RSASigner(const std::string &algorithm, const EVP_MD *md, const std::string &private_key);
  virtual ~RSASigner();
 
  bool Sign(const uint8_t *header, size_t num_header, uint8_t *signature, size_t *num_signature) const override;

  inline std::string algorithm() const override { return algorithm_; }
  
private:
  EVP_PKEY *LoadKey(const char *key);

  std::string algorithm_; 
  EVP_PKEY *private_key_;
  const EVP_MD *md_;
};

/**
 * RSASSA-PKCS-v1_5 using SHA-256 hash
 */
class RS256Signer : public RSASigner {
public:
  explicit RS256Signer(const std::string &private_key)    : RSASigner("RS256", EVP_sha256(), private_key) {}
};

/**
 * RSASSA-PKCS-v1_5 using SHA-384 hash
 */
class RS384Signer : public RSASigner {
public:
  explicit RS384Signer(const std::string &private_key)  : RSASigner("RS384", EVP_sha384(), private_key) {}
  
};

/**
 * RSASSA-PKCS-v1_5 using SHA-512 hash
 */
class RS512Signer : public RSASigner {
public:
  explicit RS512Signer(const std::string &private_key)
      : RSASigner("RS512", EVP_sha512(), private_key) {}
};
#endif // SRC_INCLUDE_JWT_RSAVALIDATOR_H_
