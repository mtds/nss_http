### NSS(name service switch) Configuration

```
passwd:     files http
shadow:     files http
group:      files http
```

### NSS

No service module is forced to have functions for all databases and all kinds to access them.
If a function is not available it is simply treated as if the function would return `unavail`.

### System query rules for http module: /lib64/libnss_modulename.so

Examples:
```
/lib64/libnss_files.so, /lib64/libnss_http.so
```

### Glibc Functions prototypes

* [getpwent](https://man7.org/linux/man-pages/man3/getpwent.3.html): `getpwent`, `setpwent`, `endpwent` - get password file entry
* [getgrent](https://www.man7.org/linux/man-pages/man3/getgrent.3.html):  `getgrent`, `setgrent`, `endgrent` - get group file entry

