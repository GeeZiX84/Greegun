#include <libssh/libssh.h>
#include <iostream>
#include <cstdlib>
void m(){
    ssh_session my_ssh_session = ssh_new();
    if (my_ssh_session == NULL) {
        std::cerr << "Error initializing SSH session." << std::endl;
        return;
    }

    // Set SSH session options
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "193.34.213.252" );
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "root");
    
    // Connect to server
    if (ssh_connect(my_ssh_session) != SSH_OK) {
        std::cerr << "Connection failed: " << ssh_get_error(my_ssh_session) << std::endl;
        ssh_free(my_ssh_session);
        return;
    }

    // Authenticate
    if (ssh_userauth_password(my_ssh_session, NULL,"&DrS5w$rz100") != SSH_AUTH_SUCCESS) {
        std::cerr << "Authentication failed: " << ssh_get_error(my_ssh_session) << std::endl;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return;
    }
}
int main(){
    m();
    return 0;
}