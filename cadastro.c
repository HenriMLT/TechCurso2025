#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void decode(char *src, char *dest) {
    char *p = src;
    while (*p) {
        if (*p == '+') {
            *dest = ' ';
        } else if (*p == '%' && *(p+1) && *(p+2)) {
            sscanf(p+1, "%2x", (unsigned int*)dest);
            p += 2;
        } else {
            *dest = *p;
        }
        p++;
        dest++;
    }
    *dest = '\0';
}

int main(void) {
    printf("Content-Type: text/html\n\n");

    // Lê os dados do POST
    char *content_length_str = getenv("CONTENT_LENGTH");
    int content_length = content_length_str ? atoi(content_length_str) : 0;

    if (content_length <= 0) {
        printf("<p>Erro: Nenhum dado recebido.</p>");
        return 1;
    }

    char *data = malloc(content_length + 1);
    fread(data, 1, content_length, stdin);
    data[content_length] = '\0';

    // Variáveis
    char nome[100], cpf[20], telefone[20], data_nasc[20], email[100], senha[100];
    char cep[20], rua[100], numero[20], complemento[100], bairro[50], cidade[50], estado[50];

    // Extrair os valores
    sscanf(data,
        "nome=%99[^&]&cfp=%19[^&]&telefone=%19[^&]&date=%19[^&]&email=%99[^&]&senha=%99[^&]&cep=%19[^&]&rua=%99[^&]&numero=%19[^&]&complemento=%99[^&]&bairro=%49[^&]&cidade=%49[^&]&estado=%49[^&]",
        nome, cpf, telefone, data_nasc, email, senha, cep, rua, numero, complemento, bairro, cidade, estado);

    // Decodificar os valores
    char d_nome[100], d_cpf[20], d_telefone[20], d_data_nasc[20], d_email[100], d_senha[100];
    char d_cep[20], d_rua[100], d_numero[20], d_complemento[100], d_bairro[50], d_cidade[50], d_estado[50];

    decode(nome, d_nome); decode(cpf, d_cpf); decode(telefone, d_telefone); decode(data_nasc, d_data_nasc);
    decode(email, d_email); decode(senha, d_senha); decode(cep, d_cep); decode(rua, d_rua);
    decode(numero, d_numero); decode(complemento, d_complemento); decode(bairro, d_bairro);
    decode(cidade, d_cidade); decode(estado, d_estado);

    // Conectar ao MySQL
    MYSQL *conn;
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, "localhost", "root", "SENHA", "bancocadastro", 0, NULL, 0)) {
        printf("<p>Erro ao conectar: %s</p>", mysql_error(conn));
        return 1;
    }

    // Query
    char query[2000];
    snprintf(query, sizeof(query),
        "INSERT INTO cadastro (nome, cpf, telefone, data_nascimento, email, senha, cep, rua, numero, complemento, bairro, cidade, estado) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
        d_nome, d_cpf, d_telefone, d_data_nasc, d_email, d_senha, d_cep, d_rua, d_numero, d_complemento, d_bairro, d_cidade, d_estado);

    if (mysql_query(conn, query)) {
        printf("<p>Erro ao inserir: %s</p>", mysql_error(conn));
    } else {
        printf("<h1>Cadastro realizado com sucesso!</h1>");
    }

    mysql_close(conn);
    free(data);
    return 0;
}