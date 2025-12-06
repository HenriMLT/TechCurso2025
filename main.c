#include <stdio.h>
#include <mysql.h>

int main() {
    MYSQL *conn;
    conn = mysql_init(NULL);

    // Conexão com o MySQL
    if (mysql_real_connect(conn, "localhost", "root", "MHmv124578", "projeto", 0, NULL, 0)) {
        printf("Conectado com sucesso ao MySQL!\n");
    } else {
        printf("Falha na conexão: %s\n", mysql_error(conn));
        return 1;
    }

    // Fechar a conexão
    mysql_close(conn);

    return 0;
}
