/**
 * wolfSSL benchmark.
 *
 * Generates a key pair on the board and loads a public key from elsewhere.
 * Then encrypts and signs a message and prints the resulting ciphers in
 * DER encoding.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-01
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

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <drivers/fsl_trng.h>
#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/signature.h>
#include <drivers/fsl_pit.h>

#define BENCHMARK_LOOPS 100

static const char plaintext[] = "We love things.\n0a1b2c3d4e5f6g7h8i9j-UBIRCH\n";

unsigned char test_der[] = {
  0x30, 0x82, 0x04, 0xa4, 0x02, 0x01, 0x00, 0x02, 0x82, 0x01, 0x01, 0x00,
  0xac, 0xc7, 0x63, 0x4c, 0x7b, 0xc3, 0x73, 0xd5, 0x01, 0xa5, 0xf6, 0x7f,
  0x4d, 0x58, 0xce, 0xfd, 0x6c, 0xef, 0x9b, 0x04, 0xbb, 0x0a, 0x92, 0x7a,
  0x2d, 0x10, 0x8d, 0xe1, 0xa8, 0x6d, 0x9a, 0x76, 0xdc, 0x26, 0x38, 0x90,
  0x90, 0xb2, 0xec, 0xe3, 0xa6, 0xf6, 0x4d, 0xdc, 0x55, 0x30, 0xf6, 0xb9,
  0xfd, 0xdd, 0xc4, 0x8d, 0xe5, 0xb2, 0xc1, 0x06, 0xad, 0x9e, 0x20, 0xef,
  0x5d, 0xec, 0xb7, 0x73, 0x31, 0xfd, 0x08, 0x65, 0xa2, 0x9f, 0xe7, 0x55,
  0xe3, 0x2d, 0x86, 0x15, 0xc7, 0xf8, 0xbb, 0xeb, 0x5f, 0x36, 0x94, 0x61,
  0x7a, 0xf8, 0x2e, 0x8d, 0x40, 0x52, 0xec, 0x48, 0x42, 0x01, 0xd8, 0xc5,
  0x67, 0x90, 0xdb, 0xb2, 0xa2, 0x52, 0x0c, 0xff, 0xcd, 0x25, 0x30, 0x0b,
  0x62, 0x9a, 0xac, 0x36, 0x75, 0x32, 0x8e, 0x45, 0xa8, 0x6a, 0x38, 0x3b,
  0xae, 0x25, 0x69, 0x07, 0x8d, 0x8e, 0x0c, 0xd3, 0x1c, 0xb5, 0xd5, 0xb3,
  0x15, 0x2d, 0xea, 0x59, 0x9b, 0xae, 0xf2, 0x10, 0x67, 0x40, 0x83, 0xb2,
  0xb8, 0x28, 0xe4, 0x8a, 0x45, 0x60, 0x47, 0x44, 0x51, 0x28, 0x50, 0xcd,
  0x66, 0x6b, 0x86, 0x22, 0x2c, 0x9d, 0xaa, 0x4b, 0x96, 0x8d, 0x27, 0x65,
  0x31, 0x72, 0xbc, 0xe1, 0xb0, 0xd9, 0xa4, 0xea, 0xd9, 0xa1, 0x30, 0x30,
  0x51, 0x13, 0x26, 0x66, 0xcc, 0x3d, 0x9c, 0x00, 0xb9, 0xe1, 0xc1, 0x4a,
  0xe0, 0x2b, 0xa5, 0x15, 0x6c, 0x9e, 0xac, 0x32, 0x25, 0xfa, 0x05, 0x88,
  0x98, 0x35, 0x2f, 0x3f, 0x50, 0xbc, 0xd5, 0x34, 0xc0, 0xee, 0xf4, 0xb5,
  0xab, 0x01, 0x3f, 0x20, 0xc6, 0xa4, 0xc0, 0xff, 0x5f, 0x3e, 0x95, 0x2f,
  0x44, 0xd5, 0x90, 0x1c, 0x7d, 0xe2, 0xa3, 0x3d, 0x1f, 0x55, 0xe4, 0x80,
  0x49, 0x2f, 0x7f, 0x2a, 0xb8, 0xe4, 0xfb, 0xd4, 0x52, 0x38, 0x24, 0x1a,
  0xb7, 0xbf, 0x80, 0xe9, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x82, 0x01,
  0x01, 0x00, 0x8a, 0x0f, 0x89, 0x39, 0x44, 0x6c, 0x19, 0xd2, 0xcb, 0x4e,
  0x62, 0x79, 0xb6, 0xc4, 0x90, 0x21, 0xd1, 0x3a, 0x65, 0x14, 0x92, 0xa1,
  0x10, 0x0a, 0xb0, 0x97, 0xd9, 0xc7, 0xc7, 0x5e, 0x4d, 0x5f, 0x4c, 0xea,
  0xfd, 0xa8, 0x06, 0xf7, 0xf2, 0x72, 0xb6, 0xa7, 0x79, 0xef, 0xe5, 0xe3,
  0x0a, 0x43, 0xf4, 0xa7, 0x90, 0xb8, 0x12, 0x23, 0x94, 0x3a, 0x51, 0xe1,
  0x59, 0x4f, 0xec, 0x2d, 0x8b, 0x29, 0x71, 0x78, 0x4f, 0x8d, 0x67, 0x35,
  0x7f, 0x15, 0x12, 0x42, 0xfe, 0x31, 0xc9, 0x68, 0xe8, 0x40, 0x5c, 0x04,
  0xb0, 0xec, 0x73, 0xe6, 0x7c, 0xcc, 0x41, 0xea, 0x01, 0x04, 0x7c, 0x3a,
  0x1d, 0x8d, 0x20, 0x04, 0x90, 0x89, 0x5d, 0x4b, 0x53, 0x37, 0xd4, 0x25,
  0x33, 0x0e, 0xe5, 0xeb, 0xd2, 0x50, 0x71, 0xb5, 0xc4, 0x19, 0xc5, 0x49,
  0x9b, 0x45, 0x95, 0x12, 0x2f, 0xaa, 0xe9, 0x61, 0xe8, 0xbc, 0x38, 0xc8,
  0x5a, 0x33, 0x9c, 0x0a, 0xbf, 0x29, 0x08, 0xe0, 0x08, 0xc3, 0x3f, 0x47,
  0x27, 0x1f, 0x55, 0x04, 0xd9, 0x0a, 0xe4, 0x69, 0x4a, 0x15, 0xf2, 0x07,
  0x07, 0x75, 0x88, 0xaf, 0x33, 0x6e, 0x6b, 0xac, 0xb1, 0xa8, 0x6b, 0xec,
  0x1d, 0x1d, 0x16, 0x57, 0x89, 0x6f, 0xb0, 0x23, 0x9b, 0xe2, 0x08, 0x01,
  0xc7, 0xca, 0x4b, 0x24, 0xba, 0x4f, 0x9d, 0x5b, 0xf5, 0x1f, 0x1b, 0xb8,
  0x0c, 0x8c, 0x01, 0xb7, 0xb7, 0x8a, 0xef, 0xc9, 0x00, 0x30, 0xd3, 0x12,
  0xe4, 0xcc, 0xc4, 0x89, 0xeb, 0x67, 0x61, 0xaf, 0x8d, 0xa4, 0x98, 0x26,
  0x14, 0xdb, 0xd1, 0x4a, 0x8b, 0x44, 0x11, 0x6f, 0xc5, 0x83, 0x98, 0xb9,
  0x81, 0xeb, 0x90, 0x06, 0x4f, 0xe4, 0x2b, 0xa4, 0x48, 0xcb, 0xf2, 0x22,
  0x45, 0x56, 0xa4, 0x29, 0xca, 0x83, 0xb7, 0x50, 0x8c, 0x1f, 0x06, 0x0f,
  0x18, 0xbb, 0x6f, 0x74, 0xca, 0x41, 0x02, 0x81, 0x81, 0x00, 0xd2, 0xe6,
  0x4e, 0xe5, 0xb4, 0xf7, 0xb8, 0xec, 0x08, 0xa8, 0x4f, 0x88, 0x8c, 0xa5,
  0xe6, 0x92, 0x29, 0x24, 0x2a, 0x5d, 0x38, 0xcb, 0xe2, 0x3e, 0xce, 0xb1,
  0xc4, 0x51, 0xd2, 0x5f, 0x88, 0xfd, 0x70, 0x1b, 0xa1, 0xdf, 0xc0, 0x95,
  0xc5, 0x42, 0x31, 0xdd, 0x67, 0xc5, 0x2c, 0x38, 0xc4, 0xf7, 0x4a, 0x9a,
  0xb2, 0x3e, 0xad, 0x98, 0x0e, 0x4a, 0xd4, 0x3c, 0xb0, 0x56, 0x75, 0xb0,
  0x44, 0x86, 0xc2, 0xca, 0x8a, 0xae, 0xde, 0x7a, 0xd4, 0xec, 0xe3, 0xf8,
  0x09, 0x04, 0x70, 0x19, 0xfd, 0x71, 0x7d, 0x7a, 0xfd, 0x11, 0x43, 0x17,
  0x78, 0xf9, 0x9b, 0x36, 0x66, 0x45, 0x53, 0xd3, 0x17, 0x81, 0x9b, 0xd8,
  0x49, 0xe0, 0xaf, 0x26, 0xdd, 0x03, 0x09, 0xd7, 0xa1, 0x2a, 0xbf, 0xba,
  0xc5, 0x82, 0x06, 0x23, 0xf4, 0x29, 0xaf, 0x77, 0x18, 0x01, 0x6d, 0x79,
  0x5c, 0x13, 0x91, 0x13, 0x69, 0x9b, 0x02, 0x81, 0x81, 0x00, 0xd1, 0xba,
  0x28, 0xba, 0x7e, 0xcd, 0x54, 0x4b, 0x32, 0xc6, 0xb9, 0x82, 0x92, 0xfc,
  0x5a, 0x24, 0x91, 0x08, 0x19, 0x99, 0xb4, 0x04, 0xe0, 0x4b, 0x0f, 0xf8,
  0x57, 0x28, 0x0b, 0x11, 0x67, 0x9b, 0x50, 0xd4, 0x2c, 0x24, 0x0b, 0xad,
  0xa8, 0xf4, 0x85, 0x77, 0x4c, 0x78, 0xec, 0x17, 0x78, 0x21, 0xaf, 0xdb,
  0x79, 0x6d, 0x34, 0x7f, 0x5e, 0x8f, 0x74, 0xb3, 0x79, 0x92, 0x90, 0x1b,
  0x13, 0xc7, 0xe0, 0xaa, 0x6c, 0x97, 0xa3, 0x8d, 0x46, 0x9a, 0x07, 0x17,
  0xd9, 0x2a, 0x15, 0xb1, 0x5e, 0x75, 0x74, 0xa9, 0x17, 0xa1, 0x27, 0xe8,
  0xa0, 0x90, 0x9f, 0x35, 0x78, 0xd0, 0xab, 0xbd, 0xc2, 0xe5, 0x18, 0x47,
  0x1c, 0xbe, 0x6c, 0x74, 0x1d, 0xd0, 0x07, 0xd4, 0x50, 0x49, 0x4e, 0x40,
  0x32, 0xa8, 0xf5, 0xd8, 0x15, 0x61, 0x3b, 0xb3, 0x7c, 0x30, 0xa9, 0xdb,
  0xa6, 0x1b, 0xea, 0x99, 0xf9, 0xcb, 0x02, 0x81, 0x80, 0x56, 0xbb, 0x70,
  0x66, 0xf7, 0x6d, 0x89, 0x17, 0x48, 0xd8, 0x2f, 0x1d, 0x30, 0xfa, 0x69,
  0x40, 0x62, 0xbb, 0x04, 0xd1, 0x21, 0xb2, 0x77, 0x70, 0xed, 0x00, 0x1e,
  0x84, 0xa7, 0xc7, 0xb2, 0x46, 0xc7, 0xfc, 0x15, 0x2e, 0x37, 0xbd, 0x60,
  0x4a, 0x40, 0xc7, 0x95, 0x0d, 0x79, 0x8a, 0x3f, 0x36, 0x93, 0xef, 0xda,
  0xad, 0xfc, 0x26, 0x34, 0x7a, 0x81, 0x9e, 0x13, 0xc6, 0xfa, 0xcf, 0x6a,
  0xdf, 0x20, 0xec, 0xba, 0xeb, 0xff, 0x82, 0xf4, 0x5e, 0x17, 0xc2, 0xc1,
  0xb2, 0x99, 0x87, 0x96, 0xbb, 0x3d, 0xa7, 0x7e, 0xbb, 0xc2, 0xa3, 0xbc,
  0x94, 0xaa, 0x48, 0x4c, 0xf6, 0xe5, 0xea, 0x42, 0x18, 0x83, 0x47, 0x07,
  0xda, 0xe7, 0x2d, 0x4c, 0x67, 0x1b, 0x7d, 0x1a, 0x42, 0xc2, 0x66, 0xc5,
  0x23, 0x1b, 0xf3, 0x5d, 0x4e, 0x67, 0x62, 0xca, 0xf4, 0x8b, 0x59, 0x05,
  0x8c, 0xe6, 0x74, 0xdd, 0x51, 0x02, 0x81, 0x80, 0x09, 0x54, 0xe3, 0x23,
  0x09, 0x29, 0x51, 0xaa, 0xd6, 0xe3, 0x88, 0xb5, 0x8a, 0x9c, 0xb7, 0xd6,
  0x10, 0xad, 0xef, 0x95, 0xab, 0x30, 0xdc, 0x59, 0xfa, 0xa7, 0xee, 0x18,
  0xd6, 0x05, 0x3c, 0x3a, 0xe8, 0xe9, 0xc7, 0xdd, 0x02, 0xb8, 0x03, 0x60,
  0x7d, 0xdf, 0xc5, 0x03, 0x4c, 0x5d, 0xee, 0xf6, 0x70, 0xc0, 0xaf, 0x32,
  0x70, 0xb7, 0x8a, 0x19, 0x8f, 0xf3, 0xbc, 0x61, 0x0d, 0x10, 0x67, 0x5c,
  0xe9, 0x5e, 0x2b, 0xde, 0xc3, 0x64, 0x31, 0xd4, 0x00, 0xa4, 0xcb, 0x00,
  0xd8, 0x5e, 0x3f, 0xf9, 0xb1, 0xeb, 0x24, 0x26, 0x03, 0x84, 0x92, 0x40,
  0x5d, 0x15, 0x1f, 0xff, 0x08, 0xe5, 0x34, 0x9a, 0xc4, 0xed, 0xc5, 0x15,
  0xa3, 0xf5, 0xb6, 0xad, 0xde, 0xeb, 0x4f, 0x77, 0x33, 0x88, 0x1f, 0xb8,
  0xec, 0xd7, 0x64, 0x7c, 0x23, 0x0f, 0xe5, 0x62, 0xb4, 0xec, 0x15, 0x04,
  0x00, 0xf8, 0x40, 0xe9, 0x02, 0x81, 0x81, 0x00, 0xaf, 0x07, 0x84, 0x12,
  0xa1, 0x78, 0xf6, 0x37, 0xf8, 0x88, 0xcb, 0x0e, 0x75, 0x7a, 0x7d, 0x65,
  0x3e, 0xf8, 0x9c, 0x80, 0x77, 0x78, 0x48, 0x07, 0x12, 0x63, 0x69, 0xa5,
  0x4b, 0x88, 0xaa, 0x61, 0x6c, 0x66, 0xfc, 0xbc, 0xc8, 0x2f, 0x06, 0x0b,
  0x2b, 0xf5, 0xcd, 0x9e, 0x46, 0x1d, 0x2d, 0x9a, 0x04, 0xe6, 0xe5, 0xc7,
  0xc1, 0xa6, 0x37, 0x17, 0xa0, 0x44, 0xa9, 0x26, 0x04, 0xe0, 0x34, 0xe5,
  0xcd, 0x31, 0x92, 0x64, 0xa3, 0xfe, 0x15, 0x96, 0x9d, 0x89, 0x96, 0x44,
  0x30, 0xf4, 0x55, 0xdc, 0xb3, 0xc6, 0x91, 0xa1, 0x5b, 0xb6, 0x89, 0xd2,
  0x8b, 0x53, 0x13, 0xda, 0xad, 0x62, 0x36, 0xee, 0xb0, 0x80, 0x21, 0xfb,
  0xb6, 0xcd, 0x95, 0xd5, 0x1c, 0x00, 0x39, 0xa1, 0xaf, 0xde, 0x15, 0x88,
  0xaf, 0x24, 0x54, 0x61, 0xb0, 0x37, 0xea, 0x4d, 0x40, 0x5b, 0x6e, 0xc6,
  0x85, 0xc3, 0x45, 0x0a
};
unsigned int test_der_len = 1192;

unsigned char recipient_pubkey[] = {
  0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00,
  0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xbd, 0x77, 0x49,
  0x26, 0xd1, 0x6b, 0x54, 0x2b, 0xf7, 0x5e, 0x7c, 0x3a, 0x6c, 0xc3, 0xf3,
  0x0d, 0x88, 0x22, 0xf6, 0x0b, 0x60, 0x5d, 0x5f, 0xa7, 0x51, 0x91, 0x4d,
  0x40, 0x1c, 0x3f, 0xaf, 0x3d, 0x99, 0x2e, 0x43, 0x8a, 0xc3, 0xcf, 0xcd,
  0xe0, 0xdb, 0x64, 0x60, 0x68, 0xc2, 0x41, 0x5f, 0x59, 0x43, 0xde, 0xfd,
  0xb0, 0x86, 0x18, 0x4d, 0xae, 0xbb, 0xb2, 0x7b, 0x22, 0xe7, 0x14, 0xb1,
  0xb7, 0x61, 0x17, 0x2f, 0x9c, 0x2e, 0x5e, 0x01, 0xa8, 0x9c, 0xb6, 0x04,
  0xdc, 0xd0, 0x20, 0x76, 0xe1, 0x26, 0xf5, 0x6f, 0xa9, 0x48, 0x66, 0x81,
  0xd8, 0x34, 0x8b, 0x21, 0x3b, 0xc0, 0x0b, 0x5f, 0x15, 0xc9, 0xf4, 0x0f,
  0x08, 0xe2, 0x4b, 0x0b, 0x42, 0x16, 0xa3, 0x68, 0x52, 0x3e, 0x5f, 0xce,
  0x0b, 0x56, 0x79, 0x35, 0xf5, 0xc9, 0xa7, 0xcd, 0x75, 0x43, 0x6e, 0x63,
  0x11, 0xfd, 0xa0, 0x0d, 0xb4, 0xfb, 0x58, 0x4f, 0x20, 0xf8, 0xcc, 0x23,
  0x3b, 0x1c, 0xe9, 0x8e, 0xed, 0x2c, 0x15, 0xac, 0x4e, 0xac, 0x4a, 0x65,
  0xdf, 0xa4, 0x14, 0xdd, 0x56, 0x9b, 0xd1, 0xb8, 0xbe, 0xf8, 0x08, 0x19,
  0x60, 0xbd, 0xbf, 0xe6, 0x1e, 0x23, 0x43, 0x5b, 0x2f, 0xb6, 0x1b, 0x02,
  0x14, 0x3c, 0x35, 0x30, 0x83, 0x72, 0xe9, 0x9b, 0x32, 0xf7, 0xce, 0xba,
  0xb3, 0x96, 0xc3, 0xbc, 0x37, 0xca, 0x08, 0xbe, 0x8e, 0x59, 0xa9, 0xfa,
  0xc2, 0xb4, 0x2c, 0xbb, 0xff, 0xfa, 0x68, 0x48, 0x1c, 0xfb, 0x7b, 0xe8,
  0xd7, 0xe7, 0xcf, 0xd9, 0x48, 0xe0, 0x8f, 0x61, 0x73, 0x78, 0x61, 0x74,
  0xf3, 0xe7, 0x86, 0x03, 0xf6, 0x82, 0xc9, 0x1e, 0xc3, 0x04, 0x91, 0x99,
  0x73, 0x28, 0xfd, 0x58, 0xc2, 0xa2, 0x10, 0xe4, 0x8f, 0xbd, 0xa8, 0x57,
  0xf2, 0x9b, 0x56, 0x78, 0x2e, 0xe8, 0xfb, 0x28, 0x2e, 0x1a, 0x9a, 0x0b,
  0x3d, 0x02, 0x03, 0x01, 0x00, 0x01
};
unsigned int recipient_pubkey_length = 294;

void print_buffer_hex(const byte *out, int len) {
  for (int i = 0; i < len; i++) {
    PRINTF("%02x", out[i]);
  }
  PRINTF("\r\n");
}

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);

}

static volatile uint64_t ms_time = 0;

void PIT0_IRQHandler() {
  PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
  ms_time++;
}

void init_pit() {
  pit_config_t pitConfig;
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (uint32_t) USEC_TO_COUNT(1000U, CLOCK_GetFreq(kCLOCK_BusClk)));
  PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
  PIT_StartTimer(PIT, kPIT_Chnl_0);
  EnableIRQ(PIT0_IRQn);
}

static uint64_t start;

uint64_t timer_start() {
  return start = ms_time;
}

uint64_t timer_stop() {
  return ms_time - start;
}

void timestamp(const char *msg, uint64_t ms) {
  PRINTF("--  %s %lus %lums\r\n", msg, (uint32_t) ms / 1000, (uint32_t) ms % 1000);
}

void print_public_key(RsaKey *key) {
  byte encoded_key[294];
  wc_RsaKeyToPublicDer(key, encoded_key, 2048);
  print_buffer_hex(encoded_key, sizeof(encoded_key));
}

void print_private_key(RsaKey *key) {
  byte encoded_key[1024];
  int len;
  len = wc_RsaKeyToDer(key, encoded_key, 2048);
  print_buffer_hex(encoded_key, len);
}


void error(char *message) {
  PRINTF("] ERROR: %s\r\n", message);
  while (true);
}

WC_RNG rng;
RsaKey board_rsa_key;
RsaKey recipient_public_key;

int init_trng() {
  PRINTF("- initializing random number generator\r\n");
  trng_config_t trngConfig;
  TRNG_GetDefaultConfig(&trngConfig);
  trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
  int r = TRNG_Init(TRNG0, &trngConfig);
  return r == kStatus_Success ? wc_InitRng(&rng) : r;
}

int init_board_key(unsigned int size) {
  PRINTF("- generating board private key (please wait)\r\n");

  wc_InitRsaKey(&board_rsa_key, NULL);

  word32 idx = 0;
  int r = wc_RsaPrivateKeyDecode(test_der, &idx, &board_rsa_key, test_der_len);
  // int r = wc_MakeRsaKey(&board_rsa_key, size, 65537, &rng);
  if (r != 0) return r;

//  PRINTF("-- BOARD PRIVATE KEY\r\n");
//  print_private_key(&board_rsa_key);
//
  PRINTF("-- BOARD PUBLIC KEY\r\n");
  print_public_key(&board_rsa_key);
  return 0;
}

int init_recipient_public_key(byte *key, size_t length) {
  PRINTF("- loading recipient public key\r\n");
  word32 idx = 0;
  wc_InitRsaKey(&recipient_public_key, NULL); // not using heap hint. No custom memory
  return wc_RsaPublicKeyDecode(key, &idx, &recipient_public_key, length);
}


int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(SystemCoreClock / 100U);
  init_pit();

  PRINTF("ubirch #1 r0.2 RSA encryption/signature benchmark\r\n");
  if (init_trng() != 0) error("failed to initialize TRNG");
  if (init_board_key(2048) != 0) error("failed to generate key pair");
  if (init_recipient_public_key(recipient_pubkey, recipient_pubkey_length))
    error("failed to load recipient public key");


  byte cipher[256]; // 256 bytes is large enough to store 2048 bit RSA ciphertext
  word32 plaintextLength = strlen(plaintext);
  word32 cipherLength = sizeof(cipher);

  PRINTF("- signing message with board private key\r\n");

  uint64_t total = 0;
  int signatureLength = 0;
  byte *signature = NULL;
  for(int i = 0; i < BENCHMARK_LOOPS; i++) {
    timer_start();
    signatureLength = wc_SignatureGetSize(WC_SIGNATURE_TYPE_RSA, &board_rsa_key, sizeof(board_rsa_key));
    signature = malloc((size_t) signatureLength);

    if (wc_SignatureGenerate(
      WC_HASH_TYPE_SHA256, WC_SIGNATURE_TYPE_RSA,
      (const byte *) plaintext, plaintextLength,
      signature, (word32 *) &signatureLength,
      &board_rsa_key, sizeof(board_rsa_key),
      &rng) != 0)
      error("failed to sign plain text message");

    const uint64_t elapsed = timer_stop();
    total += elapsed;
    char loop_str[64];
    sprintf(loop_str, "%d", i);
    timestamp(loop_str, elapsed);
  }
  timestamp("Average:", total / BENCHMARK_LOOPS);
  PRINTF("-- SIGNATURE\r\n");

  PRINTF("- encrypting message\r\n");

  total = 0;
  int r = -1;
  for(int i = 0; i < BENCHMARK_LOOPS; i++) {
    timer_start();
    r = wc_RsaPublicEncrypt((const byte *) plaintext, plaintextLength, cipher, cipherLength, &recipient_public_key,
                              &rng);
    if (r < 0) error("failed to encrypt message");

    const uint64_t elapsed = timer_stop();
    total += elapsed;
    char loop_str[64];
    sprintf(loop_str, "%d", i);
    timestamp(loop_str, elapsed);
  }
  timestamp("Average:", total / BENCHMARK_LOOPS);
  PRINTF("-- CIPHER (%d bytes)\r\n", r);

  wc_FreeRsaKey(&board_rsa_key);
  wc_FreeRsaKey(&recipient_public_key);

  PRINTF("THE END\r\n");
  while (true) {
    uint8_t ch = (uint8_t) GETCHAR();
    if (ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}

