
# This script generates a client certificate and
# a private/public key pair for use in a tls
# client or server program. The output is two
# files: the private key, and a self-signed
# certificate.  Both are in a "C" format to be
# included as C program data.  Because mbedtls
# expectes null terminated ASCII strings, the
# files each have a 0 byte appended before
# conversion to C headers

#
# Generate a CSR and Private Key
#
openssl req \
	-newkey rsa:2048 -nodes -keyout ht_domain.key \
	-subj '/C=US/ST=Massachusetts/L=Boston/CN=ht' \
	-out ht_domain.csr

#
# Generate a self-signed certificate from the CSR and PK
#
openssl x509 \
	-signkey ht_domain.key \
    -in ht_domain.csr \
   	-req -days 3650 -out ht_domain.crt

# convert the private key to a "C" ready output
#
dd if=/dev/zero bs=1 count=1 >> ht_domain.key
xxd -i ht_domain.key ht_privkey.h

# convert the self-signed certificate to a "C" ready output
#
dd if=/dev/zero bs=1 count=1 >> ht_domain.crt
xxd -i ht_domain.crt ht_ourcrt.h




