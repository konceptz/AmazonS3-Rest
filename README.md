AmazonS3-Rest

Get a byte portion of a file with a client generated token</p>

Author:    Arthur Hinds
  
Purpose:    
  
A function that can receive a partial
              file from Amazon S3 using a signed
              restful request and return the data as
              a BIGNUM.

<code>BIGNUM *s3_signed_range_request(char * server_domain,
                                char * authorization_token,<p>
                                char * client_timestamp,
                                char * object_key,<p>
                                int start_byte,
                                int end_byte)</code>

<code>Includes:<p>
stdio.h stdlib.h  time.h<p>
unistd.h  string.h<p>
sys/types.h sys/socket.h<p>
netinet/in.h  netdb.h<p>
openssl/bn.h  openssl/crypto.h  openssl/evp.h</code>
