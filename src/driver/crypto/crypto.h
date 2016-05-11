/*!
 * @brief ubirch#1 SIM800H debug helpers.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-09
 *
 * Copyright 2016 ubirch GmbH (https://ubirch.com)
 *
 * == LICENSE ==
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

typedef enum crypto_status {
    cStatus_Success = 0,
    cStatus_Failure = -1
} crypto_status_t;

/*!
 * @brief Create an SHA512 hash from the given message.
 * @param message the message to hash
 * @param size the size of the message
 * @param hash the hash to store the SHA512 digest in (must be preallocated to SHA512_DIGEST_SIZE)
 * @return cStatus_Success for succes or cStatus_Failure if it fails
 */
int crypto_sha512(const byte *message, size_t size, byte *hash);

/*!
 * @brief Encode a byte array in Base64 encoding.
 * @param in the byte array input
 * @param inlen the length of the input array
 * @return a pointer to a character string containing the base64 coded data
 */
char *crypto_base64_encode(const byte *in, size_t inlen);

/*!
 * @brief Decode a Base64 encoded character string into a byte array.
 * @param in the encoded character string
 * @patam outlen the length of the decoded byte array
 * @return the decoded byte array
 */
byte *crypto_base64_decode(const char *in, size_t *outlen);
