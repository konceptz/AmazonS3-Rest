/*  Author:  Arthur Hinds
 *  Purpose:    We want to retreive a peice, or all, of a file from 
 *              Amazon S3 without knowing the secret key.
 * 
 * Use Case:    An S3 bucket owner wants to allow temporary access to some or all
 *              of a file.  The user does not wish to share his/her private key
 *              for signing purposes but would like to still use low level calls.
 */

int recv_block(int sock, void *buffer, int length){
    unsigned char head_buffer[1];
    int r = 0;
    int ret = 0;

    while (1){
        ret = recv(sock, &head_buffer, 1, 0);

        //We are nesting buffer character checks to look for the sequence
        //'\r\n\r\n'.  How can we find a better way than checking each char?
        if (ret < 0){
            printf("Error\n");
            return -1;
        }
        if (head_buffer[0] == '\r') {
            ret = recv(sock, &head_buffer, 1, 0);
            if (head_buffer[0] == '\n') {
                ret = recv(sock, &head_buffer, 1, 0);
            }
            if (head_buffer[0] == '\r') {
                ret = recv(sock, &head_buffer, 1, 0);
            }
            if (head_buffer[0] == '\n') {
                break;
            }
        }
    }

    //Here we have found the end of the variable length header and we may begin 
    //collecting data of size length.

    while (r < length){
        ret = recv(sock, buffer+r, length - r, 0);
        if (ret < 0){
            printf("Error\n");
            return -1;
        }
        r += ret;
    }
    return r;

}

unsigned char * s3_signed_request(  unsigned char * resource,
        unsigned char * server_domain,
        unsigned char * s3_general_key,
        unsigned char * timestamp,
        unsigned char * authorization_token,
        long int start_byte,
        long int end_byte){

    //Set up the socket to Amazon
    int s3sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    portno = 80;
    s3sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(server_domain);
    if (server == NULL){
        printf("trouble connecting");
        return NULL;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(s3sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("ERROR CONNECTING");
        return NULL;
    }


    /*
     *  If we want the function to create the timestamp.
     *  This is difficult because it must match the 
     *  signed token to the second or Amazon will deny it.

     time_t timer;
     char time_buffer[55];
     struct tm* tm_info;
     time(&timer);
     tm_info = localtime(&timer);
     strftime(time_buffer, 55, "%a, %d %b %Y %OH:%M:%S %Z", tm_info);
     */

    unsigned char * request_string;
    request_string = malloc(sizeof(unsigned char)*256);

    //Write the hand crafted string.  
    sprintf(request_string, "GET %s HTTP/1.1\r\nHost: %s\r\n"
            "Date: %s\r\nRange: bytes=%i-%i\r\n"
            "Authorization: AWS %s:%s\r\n\r\n",
            resource, 
            server_domain, 
            timestamp,
            start_byte,
            end_byte,
            s3_general_key,
            authorization_token);

    //Send the string and receive the file
    if (write(s3sockfd,curlbuffer,strlen(request_string)) < 0 ){
        printf("error writing to socket\n");
        return -1;
    }

    int receive_size = (end_byte - start_byte)+1;
    unsigned char * data_buffer = malloc(sizeof(unsigned char)* receive_size);
    int bytes_received = recv_block(s3sockfd, data_buffer, receive_size);

    //CLEANUP
    free(request_string);
    close(s3sockfd);

    return (data_buffer);

}
