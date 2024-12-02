#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> //biblioteca para utilizar a funcao tolower()
#include <conio.h>
#define SIM 's'
#define NAO 'n'
#define NOME 0
#define ENDERECO 1
#define ID 2
#define CAMPOS 3
#define TAMANHO 100

//bloco de variaveis globais
typedef struct {
	char autor[50];
	char titulo[50];
	char genero[20];
    char codigoISBN[14];
	float preco;
    int anoDePublicacao;
    char emprestado;
} LIVRO;


//prototipos de funcoes
void adicionarLivro();
void toLowerCase(char*);
LIVRO buscarLivro();
void resultadoBusca(LIVRO);
void mostrarLivros();
void emprestarLivro(LIVRO);
void devolverLivro(LIVRO);
void cadastrarUsuario();
int verificarUsuario(char[]);
void mostrarTelaDeInicio(int*);
void salvarUsuarios();

void adicionarLivro(){
    //abre um arquivo de dados e adiciona uma strcut LIVRO
    LIVRO novoLivro;
    FILE *f;
    f = fopen("biblioteca.dat", "ab");
    if(!f){
        printf("Biblioteca nao disponivel.");
        exit(1);
    }
    printf("Digite o titulo do livro: ");
    fflush(stdin);
    fgets(novoLivro.titulo, 50, stdin);
    printf("Digite o autor do livro: ");
    fflush(stdin);
    fgets(novoLivro.autor, 50, stdin);
    printf("Digite o genero do livro: ");
    fflush(stdin);
    fgets(novoLivro.genero, 20, stdin);
    printf("Digite o codigo ISBN: ");
    fflush(stdin);
    fgets(novoLivro.codigoISBN, 14, stdin);
    printf("Digite o ano de publicacao: ");
    scanf(" %d", &novoLivro.anoDePublicacao);
    printf("Digite o valor do livro: ");
    scanf(" %f", &novoLivro.preco);
    novoLivro.emprestado = NAO;
    if (!(fwrite(&novoLivro, sizeof(LIVRO), 1, f))) { //se nao houver leitura, emite mensagem de erro
        printf("Erro ao escrever no arquivo.\n");
    } else {
        printf("Titulo adicionado com sucesso!\n");
    }
    
    fclose(f);
    system("pause");
}

void toLowerCase(char* str){
    //recebe um vetor de char, le caracter por caracter, guarda numa variavel char c, muda para lower case e guarda novamente
    //na string.
    char c;

    for(int i=0;i<50;i++){
        if(str[i]==' ') break;
        c = str[i];
        c = tolower(c);//funcao que transforma um char maiusculo em minusculo
        str[i] = c;
    }

}

LIVRO buscarLivro(){
    //pede uma palavra chave para busca e verifica se a palavra chave esta presente em algum livro do banco de dados.
    LIVRO livro;

    //criando uma struct copia que recebera os caracteres convertidos em minusculos
    struct COPIA{
        char copiaAutor[50];
        char copiaTitulo[50];
        char copiaGenero[20];
    } l;

    char palavraChave[50];
    FILE* f;
    f = fopen("biblioteca.dat", "rb");
    if(f==NULL){
        printf("Biblioteca nao disponivel.");
        exit(1);
    }
    printf("Digite uma palavra chave para busca: ");
    fflush(stdin);
    fgets(palavraChave, 50, stdin);
    toLowerCase(palavraChave);

    while (fread(&livro, sizeof(LIVRO), 1, f)) {
        //passando para minusculas
        strcpy(l.copiaAutor, livro.autor);
        strcpy(l.copiaGenero, livro.genero);
        strcpy(l.copiaTitulo, livro.titulo);
        toLowerCase(l.copiaAutor);
        toLowerCase(l.copiaGenero);
        toLowerCase(l.copiaTitulo);
        // Verificar se a palavra-chave está presente
        if (strstr(l.copiaAutor, palavraChave) || 
            strstr(l.copiaTitulo, palavraChave) || 
            strstr(l.copiaGenero, palavraChave) ||
            strstr(livro.codigoISBN, palavraChave)) {
            fclose(f); // Fechar o arquivo antes de retornar
            return livro; //retorna o primeiro livro encontrado
        }
    }
    // Fechar o arquivo se nada foi encontrado
    fclose(f);

    // Retornar um livro vazio em caso de não encontrar
    LIVRO vazio = {"", "", "", "", 0.0, 0, '\0'};
    system("cls");
    return vazio;
}

void resultadoBusca(LIVRO l){
    //apresenta na tela o livro buscado
    printf("Livro encontrado. \n");
    printf("#######################################################\n");
    printf("Titulo do livro: %s", l.titulo);
    printf("Autor do livro: %s", l.autor);
    printf("Genero do livro: %s", l.genero);
    printf("Preco do livro: R$ %.2f\n", l.preco);
    printf("Codigo ISBN: %s\n", l.codigoISBN);
    printf("Ano de publicacao: %d", l.anoDePublicacao);
    printf("\n");
    system("pause");
}

void mostrarLivros(){
    //Mostra todos os livros disponiveis na biblioteca
    LIVRO livro;
    FILE *f;
    f = fopen("biblioteca.dat", "rb");
    if(!f)printf("Biblioteca nao disponivel.");
    while(fread(&livro, sizeof(LIVRO), 1, f)){
        printf("Titulo: %s", livro.titulo);
        printf("Autor: %s", livro.autor);
        printf("Genero: %s", livro.genero);
        printf("Preco: %.2f\n", livro.preco);
        printf("Codigo ISBN: %s\n", livro.codigoISBN);
        printf("Ano de publicacao: %d", livro.anoDePublicacao);
        printf("\n");
    }
    fclose(f);
    system("pause");
    system("cls");
}

void emprestarLivro(LIVRO l){
    //recebe um livro da busca e altera o status de emprestimo do livro na biblioteca
    FILE *f;
    LIVRO livroBusca;
    f = fopen("biblioteca.dat", "rb+");
    if(!f){
        printf("Biblioteca nao disponivel");
        exit(1);
    }
    char usuario[10];
    printf("Digite o numero do usuario: "); //validando o usuario para emprestimo
    fflush(stdin);
    fgets(usuario, 10, stdin);
    printf("\n");
    if(!(verificarUsuario(usuario)))return;//verifica se o usuario ja esta cadastrado, caso contrario, nao segue adiante.
    if(l.emprestado == NAO){
        while(fread(&livroBusca, sizeof(LIVRO), 1, f)){ //le struct por struct do arquivo ate encontrar o titulo da busca
            if(!(strcmp(l.titulo, livroBusca.titulo))){ //para o loop assim que encontra o titulo da busca
                livroBusca.emprestado = SIM;
                fseek(f, -(sizeof(LIVRO)), SEEK_CUR); //retorna para posicao anterior da leitura
                fwrite(&livroBusca, sizeof(LIVRO), 1, f);  //atualiza status de emprestimo do livro
                break; 
            }
        }
        printf("Livro emprestado com sucesso. \n");
        fclose(f);
        system("pause");
    }else{
        printf("Desculpe. Este titulo encontra-se emprestado.");
        system("pause");
        fclose(f);
        return;
    }
}

void devolverLivro(LIVRO l){
    //Recebe um livro da busca e altera o status de emprestado na biblioteca
    FILE *f;
    LIVRO livroBusca;
    f = fopen("biblioteca.dat", "rb+");
    if(!f){
        printf("Biblioteca nao disponivel");
        exit(1);
    }
    if(l.emprestado==SIM){
        while(fread(&livroBusca, sizeof(LIVRO), 1, f)){
            if(!(strcmp(l.titulo, livroBusca.titulo))){
                livroBusca.emprestado = NAO;
                fseek(f, -(sizeof(LIVRO)), SEEK_CUR);
                fwrite(&livroBusca, sizeof(LIVRO), 1, f);
                break;
            }
        }
        printf("Livro devolvido. Obrigado.\n");
        fclose(f);
        system("pause");
    }else{
        printf("Livro disponviel para emprestimo. \n");
        fclose(f);
        system("pause");
    }
    
}

void cadastrarUsuario(){
    //cadastra novos usuarios no sistema ate o limite de 20 e depois salva
    char usuario[CAMPOS][TAMANHO];
    printf("Digite o nome do usuario: ");
    fflush(stdin);
    fgets(usuario[NOME], 50, stdin);
    printf("Digite o endereco do usuario: ");
    fflush(stdin);
    fgets(usuario[ENDERECO], 100, stdin);
    printf("Digite o numero de identificacao com no maximo 10 numeros: ");
    fflush(stdin);
    fgets(usuario[ID], 10, stdin);
    salvarUsuarios(usuario);
    system("pause");
    system("cls");
}

int verificarUsuario(char u[]){
    //verifica se o usuario ja esta cadastrado no sistema
    char usuario[CAMPOS][TAMANHO];
    FILE *f;
    f = fopen("usuarios.dat", "rb+");
    if(!f){
        printf("Arquivo de usuarios nao encontrado\n");
        exit(1);
    }
    while(fread(usuario, sizeof(char), CAMPOS*TAMANHO, f)){
        if(!(strcmp(usuario[ID], u))){
        fclose(f);
        return 1;
        }
    }
    printf("Usuario nao cadastrado.\n");
    system("pause");
    fclose(f);
    return 0;

}

void salvarUsuarios(char usuario[CAMPOS][TAMANHO]) {
    FILE *f;
    char buffer[CAMPOS][TAMANHO]; // Buffer para armazenar os dados lidos do arquivo

    f = fopen("usuarios.dat", "ab+");
    if (!f) {
        printf("Erro ao abrir arquivo.\n");
        exit(1);
    }

    // Verificar se o ID já existe no arquivo
    while (fread(buffer, sizeof(char), CAMPOS * TAMANHO, f)) {
        if (strcmp(buffer[ID], usuario[ID]) == 0) { // Comparação direta do ID
            printf("Esse numero de usuario ja existe.\n");
            fclose(f);
            return;
        }
    }

    // Se não existe, adicionar o novo usuário ao final do arquivo
    fwrite(usuario, sizeof(char), CAMPOS * TAMANHO, f);
    fclose(f);
    printf("Usuario cadastrado com sucesso.\n");
}

void mostrarTelaDeInicio(int* opcao){
    printf("00000000000000000    00000000000000000      0000000000000      000000000000000      0000    000000000000000	    \n");
    printf("00000000000000000    00000000000000000    00000000000000000    00000000000000000    0000    00000000000000000	\n");
    printf("      00000          0000                 0000          000    0000          000            0000          000	\n");
    printf("      00000          0000                 0000                 0000         000     0000    0000         000	\n");
    printf("      00000          0000000000           0000                 000000000000000      0000    000000000000000	    \n");
    printf("      00000          0000000000           0000                 00000000000000000    0000    00000000000000000	\n");
    printf("      00000          0000                 0000                 0000          000    0000    0000          000	\n");
    printf("      00000          0000                 0000          000    0000         000     0000    0000         000	\n");
    printf("      00000          00000000000000000    00000000000000000    000000000000000      0000    000000000000000	    \n");
    printf("      00000          00000000000000000      0000000000000      0000000000000        0000    0000000000000	    \n");
    printf("#############################################################################################################   \n");
    printf("                Bem vindo a biblioteca TecBib. Para iniciar, selecione uma das opcoes  abaixo                   \n");
    printf("#############################################################################################################   \n");
    printf("1. Cadastrar usuario\n2. Adicionar livro a biblioteca\n3. Buscar livro na bilbioteca\n4. Mostrar todos os livros disponiveis\n5. Emprestar livro\n6. Devolver livro\n7. Sair \n");
    scanf(" %d", opcao);
    while(*opcao<1||*opcao>7){
        printf("Por favor, digite uma opcao valida, ou digite 7 para sair \n");
        scanf(" %d", opcao);
    }
}

int main(){
    int opcao;
    char resp;

    LIVRO livro;
    mostrarTelaDeInicio(&opcao);

    while(opcao!=7){
        switch(opcao){
        case 1:
            cadastrarUsuario();
            break;
        case 2:
            adicionarLivro();
            system("cls");
            break;
        case 3:
            livro = buscarLivro();
            if((signed)strlen(livro.titulo)>0){
                resultadoBusca(livro);
                system("cls");
            }else{
                printf("Livro nao encontrado.\n");
                system("pause");
                system("cls");
            }
            break;
        case 4:
            mostrarLivros();
            break;
        case 5:
            livro = buscarLivro();
            if((signed)strlen(livro.titulo)>0){
                resultadoBusca(livro);
                printf("\nEmprestar este livro? (s/n)\n");
                if((resp = getch())==SIM)emprestarLivro(livro);
                system("cls");
            }else{
                printf("Livro nao encontrado.\n");
                system("pause");
                system("cls");
            }
            break;
        case 6:
            livro = buscarLivro();
            if((signed)strlen(livro.titulo)>0){
                resultadoBusca(livro);
                devolverLivro(livro);
                system("cls");
            }else{
                printf("Livro nao encontrado.\n");
                system("pause");
                system("cls");
            }
            break;
        }
        mostrarTelaDeInicio(&opcao);
    }

    printf("Ate mais!");

    return 0;

}