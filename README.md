# Wolfestein Raycaster Engine

Uma engine de raycasting inspirada no Wolfenstein 3D, desenvolvida do zero usando C++20, SDL2 e Dear ImGui.

## 📋 Descrição

Este projeto é uma implementação de uma engine de raycasting, similar à utilizada no clássico Wolfenstein 3D. A renderização é feita usando a técnica de raycasting, que permite criar uma visão 3D a partir de um mapa 2D. O projeto é desenvolvido em C++20 e utiliza a biblioteca SDL2 para gerenciamento de janelas, renderização e input, além de Dear ImGui para interfaces.

## 🚀 Características

- ✅ Renderização 3D usando raycasting
- ✅ Mapa 2D com paredes e espaços vazios
- ✅ Movimento do jogador (andar e girar)
- ✅ Visualização de raios no mapa 2D para debugging
- ✅ Interface separada: mapa 2D à esquerda e visão 3D à direita
- ✅ Projeto modular e organizado em vários arquivos

**Funcionalidades futuras:**

- Colisão com paredes
- Texturas para paredes
- Sprites para objetos

## 🛠️ Tecnologias Utilizadas

- **Linguagem**: C++20
- **Bibliotecas**: SDL2, Dear ImGui
- **Sistema de Build**: XMake
- **Sistema Operacional**: Windows/Linux/macOS compatível

## 📁 Estrutura do Projeto

```
📁 root/ # Raiz do projeto
├── 📁 assets/ # Recursos do jogo (texturas, sons, etc.)
├── 📁 include/ # Headers do projeto e bibliotecas de terceiros
│ ├── 📁 imgui/ # Dear ImGui
│ ├── 📁 imgui-sdl2/ # Backend SDL2 para ImGui
│ ├── 📁 core/ # Headers do núcleo do jogo
│ │ ├── 📘game.hpp
│ │ ├── 📘constants.hpp
│ │ └── 📘types.hpp
│ ├── 📁 rendering/ # Headers de renderização
│ │ ├── 📘renderer.hpp
│ │ └── 📘raycaster.hpp
│ ├── 📁 entities/ # Headers de entidades
│ │ └── 📘player.hpp
│ ├── 📁 world/ # Headers do mundo do jogo
│ │ └── 📘map.hpp
│ └── 📁 utils/ # Headers de utilitários
│ └── 📘mathUtils.hpp
├── 📁 src/ # Código fonte (.cpp)
│ ├── 📁 core/ # Núcleo do jogo
│ │ └── 📘game.cpp
│ ├── 📁 rendering/ # Renderização
│ │ ├── 📘renderer.cpp
│ │ └── 📘raycaster.cpp
│ ├── 📁 entities/ # Entidades
│ │ └── 📘player.cpp
│ ├── 📁 world/ # Mundo do jogo
│ │ └── 📘map.cpp
│ └── 📁 utils/ # Utilitários
│ └── 📘mathUtils.cpp
├── ⚙️ xmake.lua # Configuração do XMake
└── 📝README.md # Este arquivo
```

## 🔧 Compilação e Execução

**Pré-requisitos:**

- XMake instalado (veja [xmake.io](https://xmake.io))
- SDL2 e OpenGL (geralmente instalados via pacotes do sistema ou pelo xmake)

**Compilação:**

```bash
git clone https://github.com/Kyhaiu/wolfenstein-raycaster.git
cd wolfenstein-raycaster
xmake
```

### Execução:

```bash
xmake run
```

ou

```bash
xmake run app
```

## Comandos Úteis:

### Compilar em modo debug

```bash
xmake build app
```

### Limpar build

```bash
xmake clean
```

### Reconfigurar projeto

```bash
xmake f -c
```

## 🎮 Como Jogar

**Controles:**

- W: Mover para frente
- S: Mover para trás
- A: Girar para a esquerda
- D: Girar para a direita

## Interface:

### Lado Esquerdo: Mapa 2D com visualização dos raios

### Lado Direito: Visão 3D em primeira pessoa

## 🔬 Como Funciona o Raycasting

### Algoritmo Principal:

- Lançamento de Raios: Para cada coluna da tela, um raio é lançado do jogador
- DDA Algorithm: Usa Digital Differential Analysis para encontrar paredes
- Cálculo de Distância: Calcula a distância até a parede mais próxima
- Projeção 3D: Converte a distância em altura da parede na tela
- Correção Fish-Eye: Corrige a distorção visual típica do raycasting

### Matemática Envolvida:

- Trigonometria: Para cálculos de ângulos e direções
- Geometria Vetorial: Para movimento e direção do jogador
- Projeção Perspectiva: Para converter 3D em 2D

### 🛠️ Desenvolvimento

Adicionando Novas Funcionalidades:

- Novas Entidades: Adicione classes nas pastas include/entities/ e src/entities/
- Novos Efeitos Visuais: Modifique Raycaster.cpp
- Novos Mapas: Edite o array em Map.cpp

## Estrutura de Código:

```
Game (gerencia o loop principal)
├── Renderer (SDL2 + ImGui)
├── Player (entidade do jogador)
├── Map (mapa do jogo)
└── Raycaster (renderização 3D)
```

## Debugging:

- Os raios são mostrados em azul (verticais) e verde (horizontais)
- A linha vermelha indica a direção do jogador
- Use ImGui para adicionar ferramentas de debug

## 🐛 Problemas Conhecidos

- Colisão: Ainda não implementada completamente
- Performance: Pode variar dependendo do hardware
- Texturas: Não implementadas ainda

## 📈 Próximos Passos

- Implementar colisão com paredes
- Adicionar sistema de texturas
- Implementar sprites para objetos
- Adicionar sons
- Criar editor de mapas
- Otimizar algoritmo de raycasting

## 🤝 Contribuindo

1. Faça um fork do projeto
2. Crie uma branch para sua feature (git checkout -b feature/AmazingFeature)
3. Commit suas mudanças (git commit -m 'Add some AmazingFeature')
4. Push para a branch (git push origin feature/AmazingFeature)
5. Abra um Pull Request

## 📝 Licença

Este projeto está licenciado sob a GPL 3.0. Veja o arquivo LICENSE para mais detalhes.

## 🙋‍♂️ Autor

Desenvolvido como projeto educacional para aprendizado de computação gráfica.

## 📚 Referências

- [LodeV's Computer Graphics Tutorial](https://lodev.org/cgtutor)
- [SDL2 Documentation](https://wiki.libsdl.org/SDL2/FrontPage)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [ImGui - Online Manual](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)
