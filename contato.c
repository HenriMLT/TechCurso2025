#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void decode(char *src, char *dest) {
    while (*src) {
        if (*src == '+') {
            *dest = ' ';
        } else if (*src == '%' && src[1] && src[2]) {
            sscanf(src + 1, "%2x", (unsigned int *)dest);
            src += 2;
        } else {
            *dest = *src;
        }
        src++;
        dest++;
    }
    *dest = '\0';
}

int main(void) {
    printf("Content-Type: text/html\n\n");

    char *len_str = getenv("CONTENT_LENGTH");
    int len = len_str ? atoi(len_str) : 0;
    if (len <= 0) {
        printf("<p>Erro: Nenhum dado recebido</p>");
        return 1;
    }

    char *data = malloc(len + 1);
    fread(data, 1, len, stdin);
    data[len] = '\0';

    char nome[100], email[100], assunto[100], mensagem[500];
    sscanf(data, "nome=%99[^&]&email=%99[^&]&assunto=%99[^&]&mensagem=%499[^&]", nome, email, assunto, mensagem);

    char d_nome[100], d_email[100], d_assunto[100], d_mensagem[500];
    decode(nome, d_nome); decode(email, d_email); decode(assunto, d_assunto); decode(mensagem, d_mensagem);

    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "SENHA", "bancocadastro", 0, NULL, 0)) {
        printf("<p>Erro na conexão: %s</p>", mysql_error(conn));
        return 1;
    }

    char query[1500];
    snprintf(query, sizeof(query),
        "INSERT INTO contato (nome, email, assunto, mensagem) VALUES ('%s', '%s', '%s', '%s')",
        d_nome, d_email, d_assunto, d_mensagem);

    if (mysql_query(conn, query)) {
        printf("<p>Erro ao inserir: %s</p>", mysql_error(conn));
    } else {
        printf("<h1>Mensagem enviada com sucesso!</h1>");
    }

    mysql_close(conn);
    free(data);
    return 0;
}
