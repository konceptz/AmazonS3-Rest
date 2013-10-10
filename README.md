<pre><code>Author:     Arthur hinds
Purpose:    A function that can receive a partial
            file from Amazon S3 using a signed
            restful request and return the data as
            a BIGNUM.

BIGNUM *s3_signed_range_request(char * server_domain,
                                char * authorization_token,
                                char * client_timestamp,
                                char * object_key,
                                int start_byte,
                                int end_byte)

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
<code></pre>
