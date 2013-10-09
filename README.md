<h1>AmazonS3-Rest</h1>

<p>Get a byte portion of a file with a client generated token</p>

<p>Author:     Arthur Hinds
  Purpose:    A function that can receive a partial
              file from Amazon S3 using a signed
              restful request and return the data as
              a BIGNUM.</p>

<p>BIGNUM *s3<em>signed</em>range<em>request(char * server</em>domain,
                                char * authorization<em>token,
                                char * client</em>timestamp,
                                char * object<em>key,
                                int start</em>byte,
                                int end_byte)</p>

<p>Includes:
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
openssl/evp.h</p>

