#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

int main(void) {
    printf("Content-Type: text/html\n\n");

    // Recebe os dados
    char *content_length = getenv("CONTENT_LENGTH");
    int len = content_length ? atoi(content_length) : 0;
    if (len <= 0) {
        printf("<p>Erro: Nenhum dado recebido</p>");
        return 1;
    }

    char *data = malloc(len + 1);
    fread(data, 1, len, stdin);
    data[len] = '\0';

    // Extrair email e senha
    char email[100], senha[100];
    sscanf(data, "email=%99[^&]&senha=%99s", email, senha);

    // Decodificar
    for (int i = 0; i < strlen(email); i++) if (email[i] == '+') email[i] = ' ';
    for (int i = 0; i < strlen(senha); i++) if (senha[i] == '+') senha[i] = ' ';

    // Conectar ao banco
    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "SENHA", "bancocadastro", 0, NULL, 0)) {
        printf("<p>Erro ao conectar: %s</p>", mysql_error(conn));
        return 1;
    }

    // Query
    char query[500];
    snprintf(query, sizeof(query),
        "SELECT * FROM cadastro WHERE email='%s' AND senha='%s'", email, senha);

    if (mysql_query(conn, query)) {
        printf("<p>Erro ao buscar: %s</p>", mysql_error(conn));
    } else {
        MYSQL_RES *res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) {
            printf("<h1>Login realizado com sucesso!</h1>");
        } else {
            printf("<h1>Email ou senha inválidos!</h1>");
        }
        mysql_free_result(res);
    }

    mysql_close(conn);
    free(data);
    return 0;
}
