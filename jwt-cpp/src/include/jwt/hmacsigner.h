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
#ifndef SRC_INCLUDE_JWT_HMACSIGNER_H_
#define SRC_INCLUDE_JWT_HMACSIGNER_H_

#include "jwt/messagesigner.h"
#include <openssl/hmac.h>
#include <string>

// Maximum length of a signature in bytes
// Note that SHA512 is 64 bytes.
#define MAX_HMAC_KEYLENGTH 64

/**
 * Can sign & validate a message using an openssl digest function.
 */
class HMACSigner : public MessageSigner {
public:
	using json = nlohmann::json;

  explicit HMACSigner(const std::string &algorithm, const EVP_MD *md, const std::string &key);
  virtual ~HMACSigner();

  bool Sign(const uint8_t *header, size_t num_header, uint8_t *signature, size_t *num_signature) const;
 
  inline unsigned int key_size() const { return key_size_; }
  inline std::string algorithm() const { return algorithm_; }
 

private:
  HMACSigner(const HMACSigner &);
  HMACSigner &operator=(const HMACSigner &);
 
  const EVP_MD *md_;
  std::string algorithm_;
  unsigned int key_size_;
  std::string key_;
};

/**
 * HMAC using SHA-256
 */
class HS256Signer : public HMACSigner {
public:
  /**
   * Initializez the HMAC using SHA-256 algorithm with the given key
   * @param key The secret used when calculating the SHA-256
   */
  explicit HS256Signer(const std::string &key)
      : HMACSigner("HS256", EVP_sha256(), key) {}
};

/**
 * HMAC using SHA-384
 */
class HS384Signer : public HMACSigner {
public:
  /**
   * Initializez the HMAC using SHA-384 algorithm with the given key
   * @param key The secret used when calculating the SHA-384
   */
  explicit HS384Signer(const std::string &key)
      : HMACSigner("HS384", EVP_sha384(), key) {}
};

/**
 * HMAC using SHA-512
 */
class HS512Signer : public HMACSigner {
public:
  /**
   * Initializez the HMAC using SHA-512 algorithm with the given key
   * @param key The secret used when calculating the SHA-512
   */
  explicit HS512Signer(const std::string &key)
      : HMACSigner("HS512", EVP_sha512(), key) {}
};
#endif // SRC_INCLUDE_JWT_HMACVALIDATOR_H_
