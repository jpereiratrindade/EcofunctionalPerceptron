# DDD — Teste e Treinamento do Perceptron Ecofuncional

_Subdomínio experimental para inferência e avaliação ecofuncional_  
**Atualizado para v0.2.1 (features temporais de 30 entradas)**

**Autor**: José Pedro Trindade  
**Plataforma**: Plataforma de Ecologia Computacional -- SisTerApp  
**Data**: Dezembro de 2025

## Domínio

**Domínio:**
Teste, treinamento e validação experimental de mecanismos de inferência ecofuncional, aplicados à leitura de estados integrados de paisagens campestres simuladas.

Este domínio não tem como objetivo substituir ou alterar os processos ecológicos explícitos do modelo principal, mas interpretar e avaliar estados ecofuncionais a partir de dados gerados pela simulação.

## Subdomínio e Limites de Contexto

O domínio de teste e treinamento constitui um *Bounded Context* distinto do domínio ecológico principal.

- **Domínio Principal**: Paisagem Ecofuncional Integrada
- **Subdomínio Experimental**: Inferência Ecofuncional

A comunicação entre os domínios ocorre exclusivamente por meio de dados exportados (snapshots ecofuncionais), garantindo isolamento conceitual e reprodutibilidade.

## Objetivo do Subdomínio

- Treinar modelos de inferência ecofuncional a partir de estados simulados.
- Testar a capacidade do perceptron em reconhecer padrões funcionais.
- Avaliar a coerência entre estados ecológicos explícitos e inferências.
- Produzir artefatos versionados (pesos) para uso em inferência.

Este subdomínio não toma decisões ecológicas nem altera estados do sistema.

## Agregado Raiz

### EcofunctionalExperiment

Representa uma execução experimental completa de teste ou treinamento.

**Identidade**
- ExperimentID

**Responsabilidades**
- Definir o objetivo do experimento (treino ou teste).
- Referenciar conjunto de dados ecofuncionais.
- Definir parâmetros experimentais.
- Produzir resultados e artefatos versionados.

## Entidades

### EcofunctionalSample

Representa uma amostra ecofuncional individual.

**Atributos**
- inputVector (estado ecofuncional)
- targetLabel (interpretação funcional esperada: *FunctionalIntegrity*)

Cada amostra corresponde a uma leitura instantânea de uma célula ou patch da paisagem simulada.

### EcofunctionalTrajectory

Representa a sequência temporal de amostras ecofuncionais (t0, t1, ..., tn) para capturar histerese e momentum ecológico.

**Responsabilidades**
- Armazenar histórico ordenado de *EcofunctionalSample*.
- Expor métricas derivadas: delta (derivada discreta), média móvel.
- Fornecer sinais de tendência (vegetação, hidrologia) usados na inferência.

## Value Objects

### EcofunctionalVector

- soilDepth
- soilCompaction
- soilInfiltration
- hydroFlux
- erosionRisk
- vegetationCoverageEI
- vegetationCoverageES
- vegetationVigorEI
- vegetationVigorES
- propagulePotential

Este vetor é imutável e representa o estado ecofuncional observado.

### FeatureVector Temporal (30 entradas)

Concatenação de:
- Estado atual (10 atributos)
- Delta entre amostras consecutivas (10 atributos)
- Média móvel (janela 3) dos mesmos atributos (10 atributos)

É o vetor efetivamente consumido pelo Perceptron.

### InferenceOutput

- resiliencePotential
- functionalIntegrity
- recoveryCapacity

Representa a saída interpretativa do perceptron.

## Serviços de Domínio

### PerceptronTrainingService

Responsável por ajustar os pesos do perceptron a partir de um conjunto de amostras ecofuncionais, gerando o vetor temporal de 30 entradas a partir da trajetória acumulada.

- train(trajectory, parameters)
- validate(dataset)

### PerceptronInferenceService

Responsável por executar inferência ecofuncional utilizando pesos previamente treinados, consumindo o vetor de 30 entradas derivado da trajetória para capturar tendências.

- infer(trajectory)

Esses serviços não têm acesso direto ao domínio ecológico principal.

## Artefatos de Domínio

### PerceptronModel

Artefato versionado contendo:
- pesos
- bias
- definição explícita das entradas
- versão e metadados do experimento

O modelo é tratado como produto científico reprodutível.

## Regras e Invariantes

1. O treinamento nunca ocorre em tempo de simulação.
2. Pesos não são ajustados durante inferência.
3. Inferência não altera estados ecológicos.
4. A resiliência não é inferida diretamente.
5. O experimento é sempre reprodutível a partir dos dados e parâmetros.
6. CSVs devem possuir 10 atributos + alvo (treino) ou 10 atributos (trajetória); a ingestão falha caso o arquivo não seja aberto ou tenha colunas insuficientes.

## Relação com a Resiliência Ecológica

A resiliência, conforme definida por Holling, não é uma saída direta do perceptron nem um atributo do experimento. Ela emerge da avaliação temporal das inferências ecofuncionais aplicadas a trajetórias do sistema.

O subdomínio experimental contribui fornecendo sinais locais e interpretações funcionais que subsidiam essa avaliação emergente.

## Síntese

Este DDD define formalmente o subdomínio de teste e treinamento do perceptron ecofuncional como um espaço experimental isolado, coerente com o domínio ecológico principal e alinhado à definição clássica de resiliência ecológica. A separação entre simulação, inferência e avaliação garante rigor científico, transparência metodológica e extensibilidade da plataforma.
