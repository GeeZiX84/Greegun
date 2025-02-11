#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>


// Function to establish an SSH connection
ssh_session connect_ssh(const char *hostname, const char *username, const char *password, int port) {
    ssh_session session = ssh_new();
    if (!session) {
        fprintf(stderr, "Error creating SSH session\n");
        return NULL;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, hostname);
    ssh_options_set(session, SSH_OPTIONS_USER, username);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);

    if (ssh_connect(session) != SSH_OK) {
        fprintf(stderr, "SSH connection failed: %s\n", ssh_get_error(session));
        ssh_free(session);
        return NULL;
    }

    if (ssh_userauth_password(session, NULL, password) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "SSH authentication failed: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return NULL;
    }

    printf("✅ Connected to %s\n", hostname);
    return session;
}

// Function to upload a file via SFTP
int send_file_via_sftp(ssh_session session, const char *local_file, const char *remote_path) {
    sftp_session sftp = sftp_new(session);
    if (!sftp) {
        fprintf(stderr, "SFTP session creation failed: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }

    if (sftp_init(sftp) != SSH_OK) {
        fprintf(stderr, "SFTP initialization failed: %s\n", ssh_get_error(session));
        sftp_free(sftp);
        return SSH_ERROR;
    }

    FILE *file = fopen(local_file, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open local file: %s\n", local_file);
        sftp_free(sftp);
        return SSH_ERROR;
    }

    // Open remote file for writing
    sftp_file remote_file = sftp_open(sftp, remote_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (!remote_file) {
        fprintf(stderr, "Failed to open remote file: %s\n", remote_path);
        fclose(file);
        sftp_free(sftp);
        return SSH_ERROR;
    }

    // Read from local file and write to remote file
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (sftp_write(remote_file, buffer, bytes_read) != bytes_read) {
            fprintf(stderr, "Failed to write to remote file\n");
            fclose(file);
            sftp_close(remote_file);
            sftp_free(sftp);
            return SSH_ERROR;
        }
    }

    // Cleanup
    fclose(file);
    sftp_close(remote_file);
    sftp_free(sftp);
    printf("✅ File successfully uploaded: %s → %s\n", local_file, remote_path);
    return SSH_OK;
}
