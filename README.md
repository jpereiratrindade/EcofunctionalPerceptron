# Ecofunctional Perceptron

**Subdomínio experimental para inferência e avaliação ecofuncional.**

Este projeto implementa um motor de inferência baseado em **Domain-Driven Design (DDD)** e lógica de **Perceptron** para avaliar a integridade funcional de ecossistemas simulados. O sistema aplica conceitos avançados de ecologia teórica, como **Histerese**, **Trajetórias** e **Resiliência**, para diagnosticar não apenas o estado atual de uma paisagem, mas sua dinâmica temporal de recuperação.

## Funcionalidades Principais

*   **Domínio Ecofuncional**: Modelagem rica de vetores ecológicos (Solo, Hidrologia, Vegetação).
*   **Análise de Trajetória**: O sistema avalia sequências temporais ($t_0, \dots, t_n$) para detectar tendências ($\Delta$) e inércia.
*   **Capacidade de Recuperação Contínua**: Lógica heurística que distingue entre estados "Estáveis" (Clímax) e "Em Recuperação" (Histerese positiva).
*   **Infraestrutura Híbrida**:
    *   **Core C++**: Alta performance para inferência e lógica de domínio.
    *   **Pipeline de Dados**: Ingestão de CSVs para treinamento e inferência.
    *   **Visualização Python**: Scripts para plotagem de trajetórias e diagnóstico visual.

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
    mkdir build
    cd build
    cmake ..
    make
    ```

3.  **Execute a Pipeline:**
    Isso treinará o modelo com `training_data.csv`, rodará a inferência em `trajectory_data.csv` e salvará os resultados.
    ```bash
    ./EcofunctionalPerceptron
    ```

4.  **Visualize os Resultados:**
    Gera o gráfico `trajectory_plot.png`.
    ```bash
    python3 ../plot_trajectory.py inference_results.json
    ```

## Licença

Este projeto está licenciado sob a **GNU General Public License v3.0 (GPLv3)**. Consulte o arquivo `LICENSE` para mais detalhes.
