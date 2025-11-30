# 🌐 Simulateur de Réseau LAN - Implémentation du protocole STP

> **Projet académique** - Simulation complète d'un réseau local avec switches, stations et protocole Spanning Tree en C

## À propos

Ce projet est un **simulateur de réseau local (LAN)** développé dans le cadre du cours de réseaux et programmation à l'IUT. Il permet de modéliser une topologie réseau complète avec des stations, des switches et leurs interconnexions. Le simulateur implémente le **protocole STP (Spanning Tree Protocol)** pour éviter les boucles réseau et assure la transmission intelligente de trames Ethernet entre équipements.

### Contexte du projet
- **Formation** : IUT - Cours de réseaux et structures de données
- **Objectif** : Comprendre le fonctionnement des réseaux locaux et implémenter les protocoles de couche 2
- **Compétences** : Structures de données avancées, algorithmes de graphes, protocoles réseau, gestion mémoire en C

---

<p>
  <img src="images/image.png" alt="apercu d'un réseau" align="center">
</p>

---

## Technologies utilisées

| Composant | Technologies |
|-----------|-------------|
| **Langage** | C (C99) |
| **Structures de données** | Graphes pondérés, tables de hachage |
| **Protocoles** | Ethernet, STP (IEEE 802.1D) |
| **Compilation** | GCC avec Makefile |
| **Débogage** | Valgrind (détection de fuites mémoire) |

---

## Fonctionnalités

### Gestion du réseau

**Modélisation de la topologie**
- Représentation du réseau sous forme de graphe pondéré
- Support de stations (ordinateurs) et switches (commutateurs)
- Liens physiques avec coûts configurables
- Chargement de configuration depuis fichier `.lan`

**Types d'équipements**
- **Stations** : Équipements terminaux avec adresses MAC et IP
- **Switches** : Commutateurs intelligents avec tables de commutation

### Protocole Spanning Tree (STP)

**Implémentation IEEE 802.1D**
- Élection automatique du switch racine (root bridge)
- Échange de BPDU (Bridge Protocol Data Units)
- Calcul des chemins optimaux vers la racine
- Blocage intelligent des ports redondants

**États des ports**
- `PORT_BLOQUE` : Port désactivé pour éviter les boucles
- `PORT_ACTIF_RACINE` : Port vers le switch racine
- `PORT_ACTIF_DESIGNE` : Port actif en mode transmission

**Algorithme de convergence**
- Comparaison des BPDU selon priorité, MAC, coût et numéro de port
- Stabilisation automatique de la topologie
- Prévention des boucles de diffusion

### Transmission de trames Ethernet

**Format de trame IEEE 802.3**
- Préambule et SFD (Start Frame Delimiter)
- Adresses MAC source et destination
- Champ type/longueur (IPv4, ARP, IPv6)
- Données (46 à 1500 octets)
- FCS (Frame Check Sequence)

**Table de commutation (CAM table)**
- Apprentissage automatique des adresses MAC
- Association MAC ↔ Port
- Forwarding intelligent des trames

**Modes de transmission**
- **Unicast** : Transmission directe vers un destinataire connu
- **Broadcast** : Diffusion sur tous les ports actifs (sauf origine)
- Respect des ports bloqués par STP

### Algorithmes de graphes

**Implémentés**
- Parcours en profondeur (DFS) pour les composantes connexes
- Dijkstra pour les plus courts chemins
- Coloriage de graphe (algorithme glouton)
- Génération de graphes (complets, aléatoires)
- Vérification de régularité et connectivité

**En développement**
- Kruskal pour l'arbre couvrant minimal

---

## Installation

### Prérequis
- **Compilateur** : GCC 7.0+ ou Clang
- **Make** : Pour automatiser la compilation
- **Valgrind** : Pour la détection de fuites mémoire (optionnel)
- **Système** : Linux, macOS, ou WSL sous Windows

### Étapes d'installation

**1. Cloner le dépôt**
```bash
git clone https://github.com/votre-username/simulateur-reseau-stp.git
cd simulateur-reseau-stp
```

**2. Compiler le projet**
```bash
make
```

**3. (Optionnel) Vérifier les fuites mémoire**
```bash
make valgrind
```

**4. Nettoyer les fichiers compilés**
```bash
make clean
```

---

## Utilisation

### Lancer le simulateur
```bash
./programme
```

### Format du fichier de configuration

Le fichier `config.lan` définit la topologie du réseau :
```
<nb_equipements> <nb_liens>

# Équipements (un par ligne)
<type> ; <adresse_MAC> ; [nb_ports] ; [priorite]    # Pour un switch (type=2)
<type> ; <adresse_MAC> ; <adresse_IP>               # Pour une station (type=1)

# Liens (un par ligne)
<id_equipement_1> ; <id_equipement_2> ; <poids>
```

**Exemple de configuration :**
```
5 4

2 ; AA:BB:CC:DD:EE:01 ; 4 ; 32768
2 ; AA:BB:CC:DD:EE:02 ; 4 ; 32769
1 ; 00:11:22:33:44:55 ; 192.168.1.10
1 ; 00:11:22:33:44:66 ; 192.168.1.11
1 ; 00:11:22:33:44:77 ; 192.168.1.12

0 ; 1 ; 10
0 ; 2 ; 15
1 ; 3 ; 5
1 ; 4 ; 5
```

### Déroulement de la simulation

1. **Chargement de la configuration** depuis `config.lan`
2. **Application du protocole STP** pour calculer la topologie sans boucle
3. **Affichage de l'état STP** (switch racine, états des ports)
4. **Envoi de trames** entre stations
5. **Affichage des tables de commutation** apprises

### Exemple de sortie
```
=== État STP du réseau ===
Switch 0 - MAC: AA:BB:CC:DD:EE:01 - Est racine: Oui
Meilleur BPDU reçu: Root MAC: AA:BB:CC:DD:EE:01, Priorité: 32768, Coût: 0, Port: -1
Ports (4):
  Port 0: Désigné
  Port 1: Désigné
  Port 2: Désigné
  Port 3: Bloqué

[TRAME de 2 à 4] Salut station 4, ici station 2 !
═╗ [Switch 0] Trame reçue via port 0 (de 2)
║  Apprentissage : MAC 00:11:22:33:44:55 → port 0
║  Destination connue : MAC 00:11:22:33:44:77 → port 2 (vers 4)
🎯 [Station 4] Trame reçue par la destination (00:11:22:33:44:77)

======== TABLES DE COMMUTATION ========
Switch 0 :
Table de commutation du switch (MAC → port) :
MAC 00:11:22:33:44:55 → Port 0
MAC 00:11:22:33:44:77 → Port 2
```

---

## Structure du projet
```
simulateur-reseau-stp/
│
├── include/
│   ├── adresse.h          # Structures MAC/IP et conversions
│   ├── graphe.h           # Structure de graphe et opérations
│   ├── station.h          # Définition d'une station
│   ├── switch.h           # Définition d'un switch + STP
│   ├── tram.h             # Structure de trame Ethernet
│   ├── reseau.h           # Réseau global et routage
│   ├── algos.h            # Algorithmes sur graphes
│   ├── config.h           # Parsing de configuration
│   └── utils.h            # Fonctions utilitaires
│
├── src/
│   ├── adresse.c          # Implémentation adressage
│   ├── graphe.c           # Implémentation graphe
│   ├── station.c          # Création et affichage stations
│   ├── switch.c           # STP et tables de commutation
│   ├── tram.c             # Gestion des trames Ethernet
│   ├── reseau.c           # Propagation de trames
│   ├── algos.c            # DFS, Dijkstra, coloriage...
│   ├── config.c           # Lecture fichier .lan
│   └── utils.c            # Permutations, aléatoire
│
├── main.c                 # Point d'entrée du programme
├── Makefile              # Automatisation compilation
├── config.lan            # Fichier de configuration réseau
└── README.md             # Ce fichier
```

---

## Concepts réseaux implémentés

### Spanning Tree Protocol (STP)

Le STP est un protocole de couche 2 qui :
- **Prévient les boucles** dans les réseaux commutés
- **Élit un switch racine** (priorité la plus basse)
- **Calcule le meilleur chemin** vers la racine depuis chaque switch
- **Bloque les ports redondants** pour créer une topologie arborescente

**Comparaison des BPDU** (dans l'ordre) :
1. Priorité du switch racine (plus petit = meilleur)
2. Adresse MAC du switch racine (plus petite = meilleure)
3. Coût du chemin vers la racine (plus petit = meilleur)
4. Numéro de port (plus petit = meilleur)

### Table de commutation (CAM table)

Chaque switch apprend dynamiquement :
- **D'où viennent les trames** (MAC source → port d'arrivée)
- **Où envoyer les trames** (MAC destination → port de sortie)
- Si la destination est inconnue → **diffusion** (broadcast)

### Trame Ethernet IEEE 802.3
```
┌──────────┬─────┬──────────┬────────┬──────┬──────────┬─────┐
│ Préambule│ SFD │   MAC    │  MAC   │ Type │  Données │ FCS │
│  7 oct.  │1 oct│  Dest    │ Source │2 oct.│ 46-1500  │4 oct│
│          │     │  6 oct.  │ 6 oct. │      │  octets  │     │
└──────────┴─────┴──────────┴────────┴──────┴──────────┴─────┘
```

---

## Références et documentation

### Standards IEEE
- [IEEE 802.3 - Ethernet](https://standards.ieee.org/ieee/802.3/)
- [IEEE 802.1D - Spanning Tree Protocol](https://standards.ieee.org/ieee/802.1D/)

### Ressources pédagogiques
- *Computer Networks* - Andrew S. Tanenbaum
- *TCP/IP Illustrated, Volume 1* - W. Richard Stevens
- [Cisco - Understanding STP](https://www.cisco.com/c/en/us/tech/lan-switching/spanning-tree-protocol/)

---

## 👥 Auteurs

Muharremoglu Açelya
Hellich Alexis

---

## Contexte pédagogique

Ce projet illustre plusieurs concepts fondamentaux en réseaux et en programmation :

**Programmation système**
- Allocation dynamique et gestion de la mémoire
- Structures de données complexes (graphes, unions, énumérations)
- Modularité et séparation en modules
- Détection de fuites mémoire avec Valgrind

**Réseaux informatiques**
- Architecture en couches (modèle OSI - Couche 2)
- Protocoles de commutation (Ethernet, STP)
- Apprentissage automatique (tables CAM)
- Prévention des boucles réseau

**Algorithmique**
- Parcours de graphes (DFS, BFS)
- Plus courts chemins (Dijkstra)
- Arbres couvrants (Kruskal)
- Algorithmes distribués (élection STP)

---

## Notes de développement

### Conventions de codage
- Fonctions : `snake_case`
- Types : `PascalCase` pour structs
- Constantes : `UPPER_CASE`
- Headers guards : `#pragma once`

---

## 📄 Licence

Projet académique - IUT

---

**Bon courage pour vos simulations réseau ! 🚀**
