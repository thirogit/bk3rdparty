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
#include "jwt/rsavalidator.h"
#include "private/ssl_compat.h"
#include <openssl/err.h>
#include <openssl/pem.h>
#include <regex>  // NOLINT(*)
#include <sstream>
#include <string>

RSAValidator::RSAValidator(const std::string &algorithm, const EVP_MD *md,
                           const std::string &key)
    : algorithm_(algorithm), public_key_(NULL), md_(md) {
    public_key_ = LoadKey(key.c_str());
}

  

RSAValidator::~RSAValidator() {
    EVP_PKEY_free(public_key_);   
}

bool RSAValidator::Verify(const json &jsonHeader, const uint8_t *header,
                          size_t num_header, const uint8_t *signature,
                          size_t num_signature) const {
    EvpMdCtx ctx;
    EVP_MD_CTX *evp_md_ctx = ctx.get();
    EVP_MD_CTX_init(evp_md_ctx);
    EVP_VerifyInit_ex(evp_md_ctx, md_, NULL);
    bool valid =
        EVP_VerifyUpdate(evp_md_ctx, header, num_header) == 1 &&
        EVP_VerifyFinal(evp_md_ctx, signature, num_signature, public_key_) == 1;
    return valid;
}

EVP_PKEY *RSAValidator::LoadKey(const char *key) {
    EVP_PKEY *evp_pkey = NULL;
    BIO *keybio = !key || !*key ? NULL : BIO_new_mem_buf(
        const_cast<void *>(reinterpret_cast<const void *>(key)), -1);
    if (keybio == NULL) {
        return NULL;
    }

   
    evp_pkey = PEM_read_bio_PUBKEY(keybio, &evp_pkey, NULL, NULL);
   

    BIO_free(keybio);

    if (evp_pkey == NULL) {
        char buffer[120];
        ERR_error_string(ERR_get_error(), buffer);
        throw InvalidValidatorError(
            std::string("Unable to construct public key due to: ") + buffer);
    }

    return evp_pkey;
}

std::string RSAValidator::toJson() const {
    std::ostringstream msg;
    char *key;
    std::regex newline("\n");

    msg << "{ \"" << algorithm() << "\" : { ";

    if (public_key_) {
        BIO *out = BIO_new(BIO_s_mem());
        PEM_write_bio_PUBKEY(out, public_key_);
        uint64_t len = BIO_get_mem_data(out, &key);
        std::string pubkey = std::string(key, len);
        msg << "\"public\" : \"" << std::regex_replace(pubkey, newline, "\\n")
            << "\"";
        BIO_free(out);       
    }

    msg << "} }";
    return msg.str();
}
