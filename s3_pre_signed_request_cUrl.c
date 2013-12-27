unsigned char * s3_signed_request(   unsigned char * resource,
        unsigned char * server_domain,
        unsigned char * s3_general_key,
        unsigned char * timestamp,
        unsigned char * authorization_token,
        long int start_byte,
        long int end_byte){

    struct MemoryStruct data;
    struct MemoryStruct head_data;
    CURLcode res;

    data.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    data.size = 0;    /* no data at this point */ 
    head_data.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    head_data.size = 0;    /* no data at this point */ 

    struct curl_slist *slist=NULL;

    CURL *curl_handle;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */ 
    curl_handle = curl_easy_init();

    /* set URL to get */ 
    //char url_resource[256];
    char * url_resource = malloc(sizeof(char)*256);
    sprintf(url_resource, "%s%s", server_domain,resource);
    curl_easy_setopt(curl_handle, CURLOPT_URL, url_resource);

    /* no progress meter please */ 
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    /* set to get */
    curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);

    //Set a header
    //char * xamzdate[128];
    char * xamzdate = malloc(sizeof(char)*128);
    char * range = malloc(sizeof(char)*128);
    //char * range[128];
    sprintf(range, "Range: bytes=%li-%li", start_byte, end_byte);

    //char * authorization[128];
    char * authorization = malloc(sizeof(char)*128);

    sprintf(xamzdate, "Date: %s", timestamp);
    //disable the accept header
    //char * removeAccept = "Accept:";
    //sprintf(range, "Range: 0-9",);
    sprintf(authorization, "Authorization: AWS %s:%s", s3_general_key, authorization_token);

    //  slist = curl_slist_append(slist, removeAccept);
    slist = curl_slist_append(slist, "Accept:");
    slist = curl_slist_append(slist, xamzdate);
    slist = curl_slist_append(slist, range);
    slist = curl_slist_append(slist, authorization);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);

    /* send all data to this function  */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we want the headers be written to this file handle */ 
    curl_easy_setopt(curl_handle,   CURLOPT_WRITEHEADER, (void*)&head_data);
    //TODO check for auth errors here

    /* we want the body be written to this file handle instead of stdout */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&data);

    /* get it! */ 
    res = curl_easy_perform(curl_handle);
    if (strstr(head_data.memory, "403 Forbidden")){
        printf("Error 403\n");
        return NULL;
    }

    //printf("Return code: %i\n", res);
    //printf("%s\n", head_data.memory);
    //printf("%s\n", data.memory);
    unsigned char * return_buffer = malloc(sizeof(unsigned char) * data.size +1);
    memcpy(return_buffer, data.memory, data.size);

    //Free all memory from this request except the data
    free(data.memory);
    free(head_data.memory);
    free(url_resource);
    free(xamzdate);
    free(authorization);
    free(range);

    curl_slist_free_all(slist); 
    /* cleanup curl stuff */ 
    curl_easy_cleanup(curl_handle);
    //curl_global_cleanup();


    return return_buffer;

}