#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
enum CategoriaTema { ANIMAL = 0, FRUTA = 1, PAIS = 2 };
struct Jogador {
    char nome[30];
    int vidas; 
    int ativo; 
};
struct DadosRodada {
    char palavraDificil[50];
    char palavraOculta[50];
    enum CategoriaTema tema;
};
struct JogoForca {
    struct DadosRodada rodada; 
    struct Jogador jogadores[2]; 
    int turnoAtual; 
    char *letrasTentadas; 
    int totalTentativas;
};
void inicializarJogo(struct JogoForca *jogo) {
    char bancoPalavras[3][20] = {"CACHORRO", "ABACAXI", "ARGENTINA"};
  // o "srand" é usado pra que sorteie aleatoriamente a palavra, gerando um contexto
   srand(time(NULL));
    int indiceSorteio = rand() % 3;
    strcpy(jogo->rodada.palavraDificil, bancoPalavras[indiceSorteio]);
    jogo->rodada.tema = (enum CategoriaTema)indiceSorteio;
    int tamanhoPalavra = strlen(jogo->rodada.palavraDificil);
   
  // Aqui faz a montagem da palavra
  for (int i = 0; i < tamanhoPalavra; i++) {
        jogo->rodada.palavraOculta[i] = '_';
    }
  
    jogo->rodada.palavraOculta[tamanhoPalavra] = '\0'; 

    //parte que estamos  Configurando os jogadores
    printf("Digite o nome do Jogador 1: ");
    scanf("%29s", jogo->jogadores[0].nome);
    jogo->jogadores[0].vidas = 3;

    printf("Digite o nome do Jogador 2: ");
    scanf("%29s", jogo->jogadores[1].nome);
    jogo->jogadores[1].vidas = 3;

    jogo->turnoAtual = 0;
    jogo->totalTentativas = 0;
    jogo->letrasTentadas = (char *)malloc(26 * sizeof(char));
    if (jogo->letrasTentadas != NULL) {
        jogo->letrasTentadas[0] = '\0'; 
    }
}
void mostrarStatus(struct JogoForca *jogo) {
    printf("\n===================================\n");
    char dica[100] = "DICA DO JOGO: O tema é: ";
    if (jogo->rodada.tema == ANIMAL) strcat(dica, "ANIMAL");
    else if (jogo->rodada.tema == FRUTA) strcat(dica, "FRUTA");
    else strcat(dica, "PAÍS");
    printf("%s\n", dica);
    printf("Palavra: %s\n", jogo->rodada.palavraOculta);
    printf("Letras já tentadas: %s\n", jogo->letrasTentadas);
    printf("Vidas - %s: %d | %s: %d\n", 
           jogo->jogadores[0].nome, jogo->jogadores[0].vidas,
           jogo->jogadores[1].nome, jogo->jogadores[1].vidas);
    printf("-> Turno de: %s\n", jogo->jogadores[jogo->turnoAtual].nome);
}

void processarLetra(struct JogoForca *jogo, char letra) {
    letra = toupper(letra);
    jogo->letrasTentadas[jogo->totalTentativas] = letra;
    jogo->totalTentativas++;
    jogo->letrasTentadas[jogo->totalTentativas] = '\0';
    int acertou = 0;
    int tamanho = strlen(jogo->rodada.palavraDificil);
    for (int i = 0; i < tamanho; i++) {
        if (jogo->rodada.palavraDificil[i] == letra) {
            jogo->rodada.palavraOculta[i] = letra;
            acertou = 1;
        }
    }
    if (!acertou) {
        printf("\nErrou! A letra %c não existe na palavra.\n", letra);
        jogo->jogadores[jogo->turnoAtual].vidas--;
    } else {
        printf("\nBoa! Você acertou uma letra.\n");
    }
    int proximoTurno = (jogo->turnoAtual + 1) % 2;
    if (jogo->jogadores[proximoTurno].vidas > 0) {
        jogo->turnoAtual = proximoTurno;
    }
}

int main() {
   setlocale(LC_ALL,"Portuguese");
    struct JogoForca *meuJogo = (struct JogoForca *)malloc(sizeof(struct JogoForca));
    if (meuJogo == NULL) {
        printf("Erro de memória!\n");
        return 1;
    }
  
    inicializarJogo(meuJogo);
    int jogoRodando = 1;
    char chute;
    while (jogoRodando) {
        mostrarStatus(meuJogo);
        printf("Digite uma letra: ");
        scanf(" %c", &chute);
        processarLetra(meuJogo, chute);
      
        if (strcmp(meuJogo->rodada.palavraDificil, meuJogo->rodada.palavraOculta) == 0) {
            printf("\nPARABENS! A palavra era %s. Vocês venceram!\n", meuJogo->rodada.palavraDificil);
            jogoRodando = 0;
        } 
        else if (meuJogo->jogadores[0].vidas == 0 && meuJogo->jogadores[1].vidas == 0) {
            printf("\nGAME OVER! Ambos os jogadores perderam suas 3 vidas.\n");
            printf("A palavra era: %s\n", meuJogo->rodada.palavraDificil);
            jogoRodando = 0;
        }
    }
    
    char opcao;
    printf("Deseja jogar novamente? (s/n): ");
    scanf(" %c", &opcao);
    if (opcao == 's' || opcao == 'S') {
        inicializarJogo(meuJogo);
    }
    
    free(meuJogo->letrasTentadas);
    free(meuJogo);
    return 0;
}
