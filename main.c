#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <nss.h>
#include <pwd.h>
#include <grp.h>

// test with getent passwd
// add libnss_demo.so.2 to /lib
// add passwd: demo ... to /etc/nsswitch.conf
// http://www.gnu.org/software/libc/manual/html_node/NSS-Module-Function-Internals.html#NSS-Module-Function-Internals
// example: https://github.com/gmjosack/nss_http/blob/master/libnss_http/nss_http-passwd.c
//

struct user_t
{
	char name[64];
	char pw[64];
	uid_t uid;
	gid_t gid;
	char gecos[128];
	char dir[64];
	char shell[64];
};

int add_string_to_buffer(const char *string, char **str_buf_pos, char **buffer, size_t *buflen)
{
	if(*buflen < strlen((string) + 1)) 
		return -1;
	*str_buf_pos = strncpy(*buffer, string, *buflen);
	*buffer += strlen(string) + 1;
	*buflen -= strlen(string) + 1;

	return 0;
}

int pack_passwd(struct user_t user, struct passwd *result, char *buffer, size_t buflen)
{

	char *p_buffer = buffer;
	size_t buf_left = buflen;

	memset(buffer, '\0', buflen);

	add_string_to_buffer(user.name, &result->pw_name, &p_buffer, &buf_left);
	add_string_to_buffer(user.pw, &result->pw_passwd, &p_buffer, &buf_left);

	result->pw_uid = user.uid;
	result->pw_gid = user.gid;

	add_string_to_buffer(user.gecos, &result->pw_gecos, &p_buffer, &buf_left);
	
	add_string_to_buffer(user.dir, &result->pw_dir, &p_buffer, &buf_left);
	add_string_to_buffer(user.shell, &result->pw_shell, &p_buffer, &buf_left);

	return 0;
}

// called to open the passwd file
enum nss_status
_nss_demo_setpwent (int stay_open) {
	printf( "@ %s %d\n", __FUNCTION__, stay_open) ;
	return NSS_STATUS_SUCCESS ;
} ;

//called to close passwd file
enum nss_status
_nss_demo_endpwent (void) {
	printf( "@ %s\n", __FUNCTION__ ) ;
	return NSS_STATUS_SUCCESS ;
} ;

//called to look up next entry in passwd file
enum nss_status
_nss_demo_getpwent_r (struct passwd *result, char *buffer,
		size_t buflen, int *errnop) {

	static int i = 0 ;
	struct user_t user;

	printf( "@ %s with %lu\n", __FUNCTION__, buflen ) ;
	snprintf(user.name, sizeof(user.name), "demo");
	snprintf(user.pw, sizeof(user.pw), "demo");
	user.uid = 1002;
	user.gid = 1002;
	snprintf(user.gecos, sizeof(user.gecos), "user info");
	user.dir[0] = '\0';
	snprintf(user.shell, sizeof(user.shell), "/bin/false");

	if( i++ == 0 ) {
		printf( "@ %s %ld\n", __FUNCTION__, buflen ) ;
		pack_passwd(user, result, buffer, buflen);
		return NSS_STATUS_SUCCESS;
	} else {
		i = 0 ;
		return NSS_STATUS_NOTFOUND ;
	}
} ;

// find pwd by uid
enum nss_status
_nss_demo_getpwuid_r(uid_t uid, struct passwd *result, char *buffer, size_t buflen, int *errnop)
{
	printf( "@ %s with uid %u %lu\n", __FUNCTION__, uid, buflen ) ;
	struct user_t user;
	char buf[1024];
	static int i = 0;

	snprintf(user.name, sizeof(user.name), "demo");
	snprintf(user.pw, sizeof(user.pw), "demo");
	user.uid = 1002;
	user.gid = 1002;
	snprintf(user.gecos, sizeof(user.gecos), "user info");
	user.dir[0] = '\0';
	snprintf(user.shell, sizeof(user.shell), "/bin/false");

	if(uid == 1002)
	{
		pack_passwd(user, result, buffer, buflen);
		return NSS_STATUS_SUCCESS;
	}

	return NSS_STATUS_NOTFOUND;
}

// find pwd by name
enum nss_status
_nss_demo_getpwnam (const char *nam, struct passwd *result, char *buffer,
		size_t buflen, int *errnop) {
	printf( "@ %s with nam %s\n", __FUNCTION__, nam ) ;
	return NSS_STATUS_NOTFOUND ;
} ;

enum nss_status
_nss_demo_getpwnam_r (const char *nam, struct passwd *result, char *buffer,
		size_t buflen, int *errnop) {
	printf( "@ %s with nam_r %s\n", __FUNCTION__, nam ) ;
	struct user_t user;
	char buf[1024];
	static int i = 0;

	snprintf(user.name, sizeof(user.name), "demo");
	snprintf(user.pw, sizeof(user.pw), "demo");
	user.uid = 1002;
	user.gid = 1002;
	snprintf(user.gecos, sizeof(user.gecos), "user info");
	user.dir[0] = '\0';
	snprintf(user.shell, sizeof(user.shell), "/bin/false");

	if(!strcmp(user.name, nam))
	{
		pack_passwd(user, result, buffer, buflen);
		return NSS_STATUS_SUCCESS;
	}
	return NSS_STATUS_NOTFOUND ;
} ;
