#include <libssh/libssh.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

void interactive_shell(ssh_session session) {
    char *e = "ls\n";
    ssh_channel channel = ssh_channel_new(session);
    if (channel == NULL) {
        std::cerr << "Error creating channel!\n";
        return;
    }

    if (ssh_channel_open_session(channel) != SSH_OK) {
        std::cerr << "Error opening channel: " << ssh_get_error(session) << "\n";
        ssh_channel_free(channel);
        return;
    }

    if (ssh_channel_request_pty(channel) != SSH_OK) {
        std::cerr << "Failed to request PTY: " << ssh_get_error(session) << "\n";
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    if (ssh_channel_request_shell(channel) != SSH_OK) {
        std::cerr << "Failed to open shell: " << ssh_get_error(session) << "\n";
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }
    ssh_channel_write(channel, e, strlen(e));

    // Read output
    char buffer[256];
    int nbytes = ssh_channel_read(channel, buffer, sizeof(buffer) - 1, 0);
    if (nbytes > 0) {
        buffer[nbytes] = '\0';
        printf("%s", buffer);
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

int main() {
    ssh_session session = ssh_new();
    if (session == NULL) {
        std::cerr << "Failed to create session!\n";
        return 1;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "193.34.213.252");
    ssh_options_set(session, SSH_OPTIONS_USER, "root");

    if (ssh_connect(session) != SSH_OK) {
        std::cerr << "Error connecting: " << ssh_get_error(session) << "\n";
        ssh_free(session);
        return 1;
    }

    if (ssh_userauth_password(session, "root", "&DrS5w$rz100") != SSH_AUTH_SUCCESS) {
        std::cerr << "Authentication failed: " << ssh_get_error(session) << "\n";
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    std::cout << "Connected! Starting interactive shell...\n";
    interactive_shell(session);

    ssh_disconnect(session);
    ssh_free(session);
    return 0;
}