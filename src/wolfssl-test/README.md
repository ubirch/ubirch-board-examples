# wolfssl encryption example

### 1. Generate key pair

```
openssl genrsa -out test.key 2048
```

### 2. Export public key in DER format

```
openssl rsa -in test.key -pubout > key.pem
openssl x509 -outform der -in key.pem -out test.der
# convert into C code
xxd -i test.der > test_key.c
```

Now run the code on the board. It will output a string of hexadecimal numbers.
You can revert that hex-string back into binary using
```
xxd -l 256 -ps  -r test.cipher > test.encr
```

### 3. Decrypt message
```
openssl rsautl -decrypt -inkey test.key -in test.encr
```
