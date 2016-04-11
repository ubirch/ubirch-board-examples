# wolfssl RSA encryption and signing example

### 1. Generate key pair

```
openssl genrsa -out test.key 2048
```

### 2. Export public key in DER format

```
openssl rsa -in test.key -pubout -outform DER > key.der
# convert into C code
xxd -i test.der > public_key.h
```

### 3. Run code

Now run the code on the board. It will output several strings of hexadecimal numbers.

### 4. Verify signature and decrypt

You can revert the hex-strings back into binary using the following commands:

* save the board public key in PEM format (paste BOARD PUBLIC KEY)
  ```
  xxd -l 294 -ps  -r | openssl rsa -inform der -pubin -out board.pub.pem
  ```

* decrypt message (paste CIPHER)
  ```
  xxd -l 256 -ps  -r | openssl rsautl -decrypt -inkey test.key
  ```
  The output should be what's set in the ```plaintext``` constant in the code.
  ```
  We love things.\n
  0a1b2c3d4e5f6g7h8i9j-UBIRCH\n
  ```

* extract the message signature (paste SIGNATURE)
  ```
  xxd -l 256 -ps  -r | openssl rsautl -verify -inkey board.pub.pem -pubin | xxd -ps -c 32
  ```
  The message signature should be: ```ff5ba36633591ef708c0af6bb9722c7cedd6717272af9273702d0ed55697a5a9```

* decrypt message and compare message has with extracted signature hash (paste CIPHER)
  ```
  xxd -l 256 -ps  -r | openssl rsautl -decrypt -inkey test.key | shasum -a256
  ```
  Compare the result with the result of the previous operation. Both hashes
  should be the same.
