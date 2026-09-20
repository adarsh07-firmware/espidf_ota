#ifndef JSONVERSION_H
#define JSONVERSION_H

/* Fetch the server's firmware version. Returns -1 if the request or parsing fails.
 * Call after Wi-Fi has connected and obtained an IP address.
 */
int get_server_version(void);

#endif // JSONVERSION_H
