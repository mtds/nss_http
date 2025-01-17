#include "nss_http.h"

/* define ip and hostname struct*/
struct host
{
    char hostname[128];
    char ip[128];
};

void readconfig(struct config *configstruct, char *filename)
{
        // fix httpser & port wrong result
        memset(configstruct->httpserver, '\0', sizeof(configstruct->httpserver));
        memset(configstruct->port, '\0', sizeof(configstruct->port));
        memset(configstruct->debug, '\0', sizeof(configstruct->debug));
        FILE *file = fopen (filename, "r");

        if (file != NULL)
        {
                char line[MAXBUF];

                while(fgets(line, sizeof(line), file) != NULL)
                {
                    // remove \n
                    if(strlen(line) != sizeof(line)-1){line[strlen(line)-1] = '\0';}
                    char *cfline;

                    // escape comment line
                    if (strncmp(DELIM, line, strlen(DELIM)) == 0) continue;
                    cfline = strstr((char *)line,DELIM);
                    // pointer
                    cfline = cfline + strlen(DELIM);
                    char *part = strtok(line, DELIM);

                    if  (strcmp(part, "HTTPSERVER") == 0)
                    {
                        memcpy(configstruct->httpserver,cfline,strlen(cfline));
                    }
                    else if (strcmp(part, "PORT") == 0)
                    {
                        memcpy(configstruct->port,cfline,strlen(cfline));
                    }
                    else if (strcmp(part, "DEBUG") == 0)
                    {
                        memcpy(configstruct->debug,cfline,strlen(cfline));
                    }
                    else if (strcmp(part, "TIMEOUT") == 0)
                    {
                        char *e;
                        errno=0;
                        // force convert string to long number
                        configstruct->timeout = strtoll(cfline, &e, 0);
                    }
                }
                fclose(file);
        }
        else
            fprintf(stderr, "读取配置文件失败!");
}

/*Network part*/
void networks(struct host *hosts)
{
    memset(hosts->ip, '\0', sizeof(hosts->ip));
    memset(hosts->hostname, '\0', sizeof(hosts->hostname));
    //get hostname
    gethostname(hosts->hostname, 128);

    //get default interface
    FILE *f;
    char line[100], *interface, *c;
    f = fopen("/proc/net/route", "r");
    while(fgets(line, 100, f))
    {
        interface = strtok(line , " \t");
        c = strtok(NULL , " \t");
        if(interface!=NULL && c!=NULL)
        {
            if(strcmp(c , "00000000") == 0) break;
        }
    }

    /* get ip from default interface*/
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    snprintf(hosts->ip, 128, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}


void genurl(char* url, const char *type, const char *key)
{
    struct config con;
    struct host hosts;
    readconfig(&con, CONFIG_FILE);
    networks(&hosts);
    if (strlen(key) == 0){
        snprintf(url, 512, "http://%s:%s/%s?userid=%d&ip=%s&hostname=%s", con.httpserver, con.port, type,getuid(), hosts.ip, hosts.hostname);
    }
    else if ( strlen(key) != 0){
        snprintf(url, 512, "http://%s:%s/%s?%s&userid=%d&ip=%s&hostname=%s", con.httpserver, con.port, type, key, getuid(), hosts.ip, hosts.hostname);
    }
}

void debug_print(const char *func)
{
    struct config con;
    readconfig(&con, CONFIG_FILE);
    if (strcmp("true", con.debug) == 0)
        fprintf(stderr, "NSS DEBUG: Called %s \n", func);
}