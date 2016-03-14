# wolfssl RSA encryption and signing example

### 1. Generate key pair

```
openssl genrsa -out test.key 2048
```

> For now we also have to create the key also for the board, as we can't recover the public key
> on-board for RSA: ```openssl genrsa -out board.key 2048```

### 2. Export public key in DER format

```
openssl rsa -in test.key -pubout -outform DER > key.der
# convert into C code
xxd -i test.der > public_key.h
```

> Also create a ```board_key.h``` (public and private key) for our board key-pair:
> ```
> openssl rsa -in board.key -outform DER > board.der
> # convert into C code
> xxd -i board.der > board_key.h
> ```

Now run the code on the board. It will output a string of hexadecimal numbers.
You can revert that hex-string back into binary using
```
xxd -l 256 -ps  -r test.cipher > test.encr
```

### 3. Decrypt message

```
openssl rsautl -decrypt -inkey test.key -in test.encr
```

> *one-liner:*
> ```
> cat - | xxd -l 256 -ps  -r | openssl rsautl -decrypt -inkey test.key
> ```
> Now paste the the output from the board.

### 4. Verify signature

```
# recover the sha256 sum from the signature
cat - | xxd -l 256 -ps  -r | openssl rsautl -verify -inkey test.key | xxd -ps -c 32
# verify that the decrypted message has the same signature
cat - | xxd -l 256 -ps  -r | openssl rsautl -decrypt -inkey test.key | shasum -a256
```

Both hashes the are the result of those commands should be the same.
