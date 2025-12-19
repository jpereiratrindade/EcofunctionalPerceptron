# Ecofunctional Perceptron (v0.2.1)

**Subdomínio experimental para inferência e avaliação ecofuncional.**

Este projeto implementa um motor de inferência baseado em **Domain-Driven Design (DDD)** e lógica de **Perceptron** para avaliar a integridade funcional de ecossistemas simulados. O sistema aplica conceitos avançados de ecologia teórica, como **Histerese**, **Trajetórias** e **Resiliência**, para diagnosticar não apenas o estado atual de uma paisagem, mas sua dinâmica temporal de recuperação.

## Funcionalidades Principais

*   **Domínio Ecofuncional**: Vetor ecológico com 10 atributos (solo, hidrologia, vegetação).
*   **Engenharia de Features Temporais (30 entradas)**: Concatenamos estado atual (10) + delta (10) + média móvel (10) para capturar histerese e momentum ecológico.
*   **Capacidade de Recuperação Contínua**: Heurísticas que distinguem estável, recuperação, degradação e colapso.
*   **Ingestão Robusta de CSV**: Validação de cabeçalho/linhas, falha rápida se arquivo não abre ou colunas faltam.
*   **Infraestrutura Híbrida**:
    *   **Core C++17 + CMake**: Inferência e lógica de domínio.
    *   **Pipeline de Dados**: Treino (`training_data.csv`) e inferência (`trajectory_data.csv`) via CSV.
    *   **Visualização Python**: `plot_trajectory.py` para gerar `trajectory_plot.png`.

## Estrutura do Projeto

*   `domain.h/cpp`: Lógica de negócio (Entidades, Value Objects, Trajetórias).
*   `services.h/cpp`: Orquestração da inferência e treinamento.
*   `perceptron.h/cpp`: Implementação da Rede Neural (Infraestrutura).
*   `data_loader.h/cpp`: Adaptadores para leitura de CSV.
*   `plot_trajectory.py`: Visualização dos resultados de inferência.

## Pré-requisitos

*   **CMake** (3.14+)
*   **Compilador C++17** (GCC/Clang/MSVC)
*   **Python 3** + `matplotlib` (para visualização)
    *   Instalação: `pip install matplotlib`

## Como Compilar e Rodar

1.  **Clone o repositório:**
    ```bash
    git clone https://github.com/jpereiratrindade/EcofunctionalPerceptron.git
    cd EcofunctionalPerceptron
    ```

2.  **Compile o projeto:**
    ```bash
    cmake -S . -B build
    cmake --build build
    ```

3.  **Execute a pipeline completa:**
    Treina com `training_data.csv`, roda inferência incremental em `trajectory_data.csv` usando o vetor de 30 features e salva `inference_results.json`.
    ```bash
    ./build/EcofunctionalPerceptron
    ```

4.  **Visualize os resultados:**
    Gera o gráfico `trajectory_plot.png`.
    ```bash
    python3 plot_trajectory.py inference_results.json
    ```

## Licença

Este projeto está licenciado sob a **GNU General Public License v3.0 (GPLv3)**. Consulte o arquivo `LICENSE` para mais detalhes.
