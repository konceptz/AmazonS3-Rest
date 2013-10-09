/*  Author:     Arthur hinds
 *  Purpose:    A function that can receive a partial
 *              file from Amazon S3 using a signed
 *              restful request and return the data as
 *              a BIGNUM.
 *
 *  Note:       A full list of includes available in
 *              the readme.
 */

#ifdef USE_S3
#DEFINE BUFFER_SIZE = 4096

BIGNUM *s3_signed_range_request(char * server_domain,
                                char * s3_id,
                                char * authorization_token,
                                char * client_timestamp,
                                char * object_key,
                                int start_byte,
                                int end_byte){

    //Set up the socket to Amazon
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    portno = 80;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname("s3.amazonaws.com");
    if (server == NULL){
        printf("trouble connecting");
        return 0;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("ERROR CONNECTING");
        return 1;
    }


    /*
     *  If we want the function to create the timestamp.
     *  This is difficult because it must match the 
     *  signature to the second or Amazon will deny it.

    time_t timer;
    char time_buffer[55];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(time_buffer, 55, "%a, %d %b %Y %OH:%M:%S %Z", tm_info);
    */
    
    char * curlbuffer;
    curlbuffer = malloc(sizeof(char)*255);

    //Working signed with date
    sprintf(curlbuffer, "GET %s HTTP/1.1\r\nHost: %s\r\n"
                        "Date: %s\r\nRange: bytes=%i-%i\r\n"
                        "Authorization: AWS %s:%s\r\n\r\n",
                        object_key, 
                        server_domain, 
                        client_timestamp,
                        start_byte,
                        end_byte,
                        s3_id,
                        authorization_token);

    //Send the string and receive the file
    if (write(sockfd,curlbuffer,strlen(curlbuffer)) < 0 ){
        printf("error writing to socket\n");
        return 0;
    }

    unsigned char recv_buffer[BUFFER_SIZE];
    memset(recv_buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sockfd,recv_buffer, BUFFER_SIZE, 0);

    //copy the buffer into a string only the size of the received message
    unsigned char message[bytes_received+1];
    memcpy(message, recv_buffer, bytes_received);
    message[bytes_received] = '\0';
    //printf("%s\n", message);

    //get the actual content out into a biginteger
    int data_size = end_byte - start_byte;
    unsigned char content[10]; //hard coded to match the requested number of bytes
    strncpy(content, message+(bytes_received - data_size), data_size);
    BIGNUM *content_as_bignum;
    content_as_bignum = BN_new();
    BN_bin2bn(content, data_size, content_as_bignum);

    //Free Memory
    free(curlbuffer);
    close(sockfd);

    return (content_as_bignum);

}

