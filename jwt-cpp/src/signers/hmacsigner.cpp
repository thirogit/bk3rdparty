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
#include "jwt/hmacsigner.h"
#include "private/ssl_compat.h"
#include <memory>
#include <sstream>
#include <string.h>
#include <string>

HMACSigner::HMACSigner(const std::string &algorithm, const EVP_MD *md,
                             const std::string &key)
    : md_(md), algorithm_(algorithm), key_size_(EVP_MD_size(md)), key_(key) {}

HMACSigner::~HMACSigner() {}


bool HMACSigner::Sign(const uint8_t *header, size_t num_header, uint8_t *signature, size_t *num_signature) const {
  if (signature == NULL || *num_signature < key_size_) {
    *num_signature = key_size_;
    return false;
  }
  HMacCtx hctx;
  HMAC_CTX *ctx = hctx.get();

  HMAC_Init_ex(ctx, key_.c_str(), key_.size(), md_, NULL);
  bool sign = HMAC_Update(ctx, header, num_header) &&
              HMAC_Final(ctx, signature, (unsigned int *)num_signature);

  return sign;
}


