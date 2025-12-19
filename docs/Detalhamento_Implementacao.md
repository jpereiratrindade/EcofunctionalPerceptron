# Detalhamento da Implementação: Perceptron Ecofuncional

**Atualizado para v0.2.1 (vetor temporal de 30 features)**

**Autor**: Documentação Técnica do Projeto  
**Data**: Atualizada automaticamente no repositório

## Introdução

Este documento detalha a implementação em C++ do subdomínio de inferência ecofuncional, conforme definido no DDD do projeto. O release v0.2.1 introduz ingestão robusta de CSV (falha cedo em caso de arquivo ausente ou colunas faltantes) e um vetor temporal de 30 features (estado + delta + média móvel) para capturar histerese.

## Arquitetura do Sistema

O sistema foi construído seguindo uma arquitetura em camadas para isolar a lógica de domínio da infraestrutura de aprendizado de máquina.

### Camada de Domínio e Infraestrutura

- **Domain Layer**: Contém as regras de negócio, entidades e objetos de valor (`EcofunctionalVector`, `EcofunctionalExperiment`).
- **Service Layer**: Orquestra o fluxo de dados entre o domínio e o modelo (`PerceptronTrainingService`, `PerceptronInferenceService`).
- **Infrastructure/Core**: Implementação pura do algoritmo (`Perceptron`).

## Implementação do Domínio

### EcofunctionalVector (Value Object)

Representa o estado imutável de um patch da paisagem. Mapeia os 10 atributos ecofuncionais essenciais.

```cpp
struct EcofunctionalVector {
    float soilDepth;
    float soilCompaction;
    // ... outros atributos
    float propagulePotential;

    std::vector<float> toVector() const;
};
```

### EcofunctionalExperiment (Aggregate Root)

Gerencia um ciclo de experimento, contendo um conjunto de amostras (`EcofunctionalSample`) e uma identidade única. Garante que todas as amostras pertençam ao mesmo contexto experimental.

### EcofunctionalTrajectory

Armazena o histórico ordenado de amostras e expõe utilitários para engenharia de features temporais:

- **calculateDelta**: derivada discreta entre os dois últimos estados.
- **calculateAverage**: média móvel (janela configurável, uso padrão 3).
- **analyzeState**: classifica a trajetória (estável, recuperação, degradação, colapso).

### Feature Vector Temporal (30)

O vetor efetivamente consumido pelo Perceptron concatena:

1. Estado atual (10 atributos)
2. Delta entre amostras consecutivas (10 atributos)
3. Média móvel (janela 3) dos mesmos atributos (10 atributos)

## Implementação de Serviços

### PerceptronTrainingService

Atua como uma fachada que traduz objetos de domínio para o vetor temporal de 30 entradas. O serviço acumula a trajetória ao iterar as amostras do experimento, gerando as features de estado + delta + média móvel antes de chamar `model.train`.

```cpp
void trainFullExperiment(Perceptron& model, 
                         const EcofunctionalExperiment& exp, 
                         float lr, 
                         int epochs);
```

## Build System

O projeto utiliza **CMake** para gerenciamento de build, garantindo portabilidade.

- **Compilador**: Requer suporte a C++17.
- **Dependências**: *nlohmann_json* para serialização do modelo.

## Verificação e Uso

O arquivo `main.cpp` serve como prova de conceito, realizando o seguinte fluxo com dados externos:

1. Carrega `data/training_data.csv` (10 atributos + *FunctionalIntegrity*) com validação de colunas e cabeçalho.
2. Treina supervisionadamente via `PerceptronTrainingService`, gerando features temporais (30).
3. Carrega `data/trajectory_data.csv` (10 atributos) e executa inferência incremental para cada passo.
4. Persiste o log de inferência em JSON (`outputs/inference_results.json`) para visualização.

## Fase 2: Engenharia de Features Temporais

Para capturar a dinâmica não-linear de recuperação ecológica, o sistema foi evoluído para processar **Trajetórias** em vez de apenas estados estáticos.

### Trajetória Ecofuncional

A entidade `EcofunctionalTrajectory` armazena o histórico temporal (t0, t1, ..., tn) de amostras.

```cpp
struct EcofunctionalTrajectory {
    std::vector<EcofunctionalSample> history;

    // Calcula a variacao (derivada discreta)
    EcofunctionalVector calculateDelta() const;

    // Suaviza ruidos temporais
    EcofunctionalVector calculateAverage(int windowSize) const;
};
```

### Lógica de Inferência Contextual

O serviço de inferência consome o vetor de 30 entradas (estado + delta + média móvel) e usa a classificação da trajetória (estável/recuperação/degradação/colapso) para atribuir a métrica de *RecoveryCapacity*.

Isso permite diferenciar um ecossistema estagnado de um que está em processo ativo de regeneração, mesmo que ambos tenham o mesmo estado estático atual.

## Fase 3: Nuance Ecológica e Histerese

A lógica de recuperação foi refinada para abandonar a binariedade (0/1) em favor de uma escala contínua e heurísticas de histerese.

### Capacidade de Recuperação Contínua

A métrica `RecoveryCapacity` agora opera no intervalo [0.0, 1.0], calculada pela combinação de *Integridade Funcional* e *Estado da Trajetória*.

```cpp
enum class TrajectoryState {
    STABLE,     // Climax ou estagnacao
    RECOVERING, // Ganho ativo de funcao
    DEGRADING,  // Perda ativa
    COLLAPSING  // Perda critica
};
```

### Regras Heurísticas

- **High State + Stable** -> Recovery 0.9 (Clímax, robusto)
- **High State + Recovering** -> Recovery 1.0 (Ganho por histerese/momento)
- **Low State + Recovering** -> Recovery Variable (Resiliência emergente)
- **Degrading** -> Penalização severa

Esta abordagem permite ao modelo distinguir ecossistemas maduros de ecossistemas em recuperação ativa, alinhando-se melhor com a teoria ecológica de Holling.

## Fase 4: Engenharia de Dados e Visualização

Para escalar a validação do modelo, foi implementada uma pipeline de ingestão de dados externos e ferramentas de análise visual. A ingestão valida abertura de arquivo e número mínimo de colunas, falhando cedo para preservar reprodutibilidade.

### Pipeline de Carga (Data Loader)

A classe `DataLoader` foi introduzida para permitir a leitura de arquivos CSV (`.csv`), desacoplando os dados de teste do código fonte.

- **Training Adapter**: Converte linhas CSV em `EcofunctionalExperiment`.
- **Trajectory Adapter**: Converte séries temporais em `EcofunctionalTrajectory`.

### Visualização de Histerese

Um script auxiliar em Python (`scripts/plot_trajectory.py`) consome os logs de inferência (JSON) gerados pelo sistema C++. Ele plota a *Integridade Funcional* ao longo do tempo e ajuda a visualizar visualmente os fenômenos de histerese e inércia ecológica capturados pela lógica de domínio.
