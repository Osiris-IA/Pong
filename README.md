# Projet d'Exemple: Comprendre les Makefiles

Ce projet sert d'exemple pour expliquer le fonctionnement d'un Makefile. Il s'agit d'un petit jeu nommé "monjeu" compilé en C++.

## Structure du Makefile

Le Makefile de ce projet est conçu pour compiler un jeu simple en C++ avec les fichiers suivants:
- `src/main.cpp`
- `src/Character.cpp`
- `src/Game.cpp`

## Explication du Makefile

### Variables de Configuration

```make
NAME = monjeu

CC = g++
CFLAGS = -Wall -Wextra -std=c++17
INC = -I./include

SRC = src/main.cpp src/Character.cpp src/Game.cpp
OBJ = $(SRC:.cpp=.o)
```

- `NAME`: Le nom de l'exécutable final (`monjeu`)
- `CC`: Le compilateur utilisé (`g++`)
- `CFLAGS`: Les flags de compilation (warnings et standard C++17)
- `INC`: Le chemin vers les fichiers d'en-tête (`./include`)
- `SRC`: La liste des fichiers source à compiler
- `OBJ`: Les fichiers objets générés à partir des fichiers source (transformation automatique de `.cpp` en `.o`)

### Règles de Compilation

```make
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS) $(INC)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INC)
```

- `all`: La règle par défaut, qui dépend de `$(NAME)`
- `$(NAME)`: Cette règle crée l'exécutable final en liant tous les fichiers objets
- `%.o: %.cpp`: Une règle de pattern qui indique comment compiler chaque fichier `.cpp` en fichier `.o`
  - `$<`: Le nom du fichier source (la dépendance)
  - `$@`: Le nom du fichier cible (le fichier objet)

### Règles de Nettoyage

```make
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
```

- `clean`: Supprime les fichiers objets
- `fclean`: Supprime les fichiers objets et l'exécutable
- `re`: Effectue un nettoyage complet puis recompile tout

### Règles Spéciales

```make
.PHONY: all clean fclean re
```

- `.PHONY`: Indique que ces règles ne correspondent pas à des fichiers réels, mais à des actions à effectuer

## Comment Utiliser ce Makefile

### Prérequis

- **Linux/macOS**: Make est généralement préinstallé
- **Windows**: Vous devrez installer MSYS2 pour avoir accès à Make et aux outils de compilation
  - Téléchargez et installez MSYS2 depuis [https://www.msys2.org/](https://www.msys2.org/)
  - Ouvrez le terminal MSYS2 et exécutez: `pacman -S mingw-w64-x86_64-gcc make cmake ninja`
  - Ajoutez le chemin MSYS2 à votre PATH système pour utiliser les commandes depuis le terminal Windows

### Commandes

1. Pour compiler le projet: `make` ou `make all`
2. Pour nettoyer les fichiers objets: `make clean`
3. Pour nettoyer tous les fichiers générés: `make fclean`
4. Pour recompiler entièrement le projet: `make re`

## Compilation Multi-Plateforme

Ce Makefile supporte automatiquement macOS, Linux et Windows. Il détecte le système d'exploitation et utilise les bonnes bibliothèques.

### Structure des Bibliothèques

Les bibliothèques statiques SFML doivent être organisées comme suit:

```
lib/
├── macOS/          # Bibliothèques pour macOS (.a)
│   ├── libsfml-system.a
│   ├── libsfml-window.a
│   ├── libsfml-graphics.a
│   ├── libsfml-audio.a
│   ├── libsfml-network.a
│   └── ... (dépendances)
└── Windows/        # Bibliothèques pour Windows (.a avec MinGW)
    ├── libsfml-system.a
    ├── libsfml-window.a
    ├── libsfml-graphics.a
    ├── libsfml-audio.a
    ├── libsfml-network.a
    └── ... (dépendances)

```

### Obtenir les Bibliothèques SFML pour Windows

Les bibliothèques statiques SFML pour Windows sont déjà incluses dans le projet dans le dossier `lib/Windows/`. Si vous devez les obtenir ou les mettre à jour:

1. **Télécharger SFML 3.0.2** depuis [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)

2. **Installer les dépendances** (dans MSYS2):
   ```bash
   pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
   ```

3. **Compiler SFML en mode statique**:
   ```bash
   cd /chemin/vers/SFML-3.0.2
   cmake -S . -B build-static \
         -G Ninja \
         -DCMAKE_BUILD_TYPE=Release \
         -DBUILD_SHARED_LIBS=OFF \
         -DSFML_BUILD_EXAMPLES=OFF \
         -DSFML_BUILD_TEST_SUITE=OFF
   
   cmake --build build-static
   ```

4. **Copier les bibliothèques** dans `lib/Windows/`:
   ```bash
   mkdir -p /chemin/vers/sample-sfml-project/lib/Windows
   cp build-static/lib/*.a /chemin/vers/sample-sfml-project/lib/Windows/
   ```

5. **Vérifier que les bibliothèques sont présentes**:
   ```bash
   ls lib/Windows/
   # Devrait afficher: libsfml-*.a et les dépendances
   ```

### Obtenir les Bibliothèques SFML pour macOS

Les bibliothèques macOS sont déjà dans `lib/macOS/`. Elles ont été compilées avec:
```bash
cmake -S . -B build-static -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      -DSFML_BUILD_EXAMPLES=OFF \
      -DSFML_BUILD_TEST_SUITE=OFF
cmake --build build-static
```

Si vous devez les recompiler, suivez les mêmes étapes que pour Windows en adaptant les chemins.

## Comprendre le Processus de Compilation

1. Lorsque vous exécutez `make`, la règle `all` est appelée
2. `all` dépend de `$(NAME)` (l'exécutable final)
3. Pour créer `$(NAME)`, make vérifie si tous les fichiers objets `$(OBJ)` existent
4. Pour chaque fichier `.cpp` qui n'a pas de fichier `.o` correspondant ou qui a été modifié, la règle `%.o: %.cpp` est appliquée
5. Une fois tous les fichiers objets créés, ils sont liés ensemble pour former l'exécutable final

