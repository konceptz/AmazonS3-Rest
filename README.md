AmazonS3-Rest
=============

Get a byte portion of a file with a client generated token


  Author:     Arthur hinds
  Purpose:    A function that can receive a partial
              file from Amazon S3 using a signed
              restful request and return the data as
              a BIGNUM.

<code>BIGNUM *s3_signed_range_request(char * server_domain,<br>
                               <tab>char * authorization_token,<br>
                               <tab>char * client_timestamp,<br>
                              <tab>char * object_key,<br>
                                <tab>int start_byte,<br>
                                <tab>int end_byte)</code>


Includes:
stdio.h
stdlib.h
time.h
unistd.h
string.h
sys/types.h
sys/socket.h
netinet/in.h
netdb.h
openssl/bn.h
openssl/crypto.h
openssl/evp.h
