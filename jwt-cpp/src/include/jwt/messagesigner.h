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
#ifndef SRC_INCLUDE_JWT_MESSAGESIGNER_H_
#define SRC_INCLUDE_JWT_MESSAGESIGNER_H_

#include <stdint.h>
#include <string.h>
#include <string>
#include "jwt/allocators.h"
#include "jwt/json.hpp"
#include "jwt/jwt_error.h"


/**
 * A MessageSigner is capable of signing a JWS Joseheader
 */
class MessageSigner {
   public:
    virtual ~MessageSigner() {}

    /**
     * Signs the given jose header.
     * if signature == 0, or *num_signate is less than what is needed for a
     * signature the method should return false, and num_signature should
     * contain the number of bytes needed to place the signature in.
     *
     * @param header the urlbase64 encoded JOSEHeader
     * @param num_header Number of bytes in the header
     * @param signature The binary signature. (Note base64 encoded).
     * @param num_signature The length of the signature.
     */
    virtual bool Sign(const uint8_t *header, size_t num_header,
                      uint8_t *signature, size_t *num_signature) const = 0;

    /**
     * Creates the digital signature
     *
     * @param header the jose header to be signed
     * @return The signature
     */
    std::string Digest(const std::string &header) const;

	/**
	* The JWS algorithm that this message validator can handle, if any.
	*/
	virtual std::string algorithm() const = 0;
};

typedef std::unique_ptr<MessageSigner> signer_ptr;
#endif  // SRC_INCLUDE_JWT_MESSAGESIGNER_H_
