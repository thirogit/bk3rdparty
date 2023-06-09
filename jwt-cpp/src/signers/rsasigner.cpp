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
#include "jwt/rsasigner.h"
#include "private/ssl_compat.h"
#include <openssl/err.h>
#include <openssl/pem.h>
#include <regex>  // NOLINT(*)
#include <sstream>
#include <string>

RSASigner::RSASigner(const std::string &algorithm, const EVP_MD *md, const std::string &private_key)
    : algorithm_(algorithm), private_key_(NULL), md_(md) {
	private_key_ = LoadKey(private_key.c_str());
}



RSASigner::~RSASigner() {    
    EVP_PKEY_free(private_key_);
}



bool RSASigner::Sign(const uint8_t *header, size_t num_header,  uint8_t *signature, size_t *num_signature) const {
    size_t needed = 0;
    bool success = false;

    EvpMdCtx ctx;
    EVP_MD_CTX *evp_md_ctx = ctx.get();
    EVP_MD_CTX_init(evp_md_ctx);
    EVP_DigestSignInit(evp_md_ctx, NULL, md_, NULL, private_key_);
    if (EVP_DigestSignUpdate(evp_md_ctx, header, num_header) != 1) {
        goto Error;
    }

    // Figure out how many bytes we need
    if (EVP_DigestSignFinal(evp_md_ctx, NULL, &needed) != 1) {
        goto Error;
    }

    // We need more bytes please!
    if (signature == NULL || *num_signature < needed) {
        *num_signature = needed;
        goto Error;
    }

    success = EVP_DigestSignFinal(evp_md_ctx, signature, num_signature) == 1;
Error:
    return success;
}

EVP_PKEY *RSASigner::LoadKey(const char *key) {
    EVP_PKEY *evp_pkey = NULL;
    BIO *keybio = !key || !*key ? NULL : BIO_new_mem_buf(
        const_cast<void *>(reinterpret_cast<const void *>(key)), -1);
    if (keybio == NULL) {
        return NULL;
    }

   
    evp_pkey = PEM_read_bio_PrivateKey(keybio, &evp_pkey, NULL, NULL);
    

    BIO_free(keybio);

    if (evp_pkey == NULL) {
        char buffer[120];
        ERR_error_string(ERR_get_error(), buffer);
        throw InvalidValidatorError(std::string("Unable to construct private key due to: ") + buffer);
    }

    return evp_pkey;
}


