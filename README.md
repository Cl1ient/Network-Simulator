# Simulateur de réseau – Projet SAE2.3

> **Projet académique – BUT Informatique, 2ᵉ semestre**  
> Module : Réseaux – IUT Robert Schuman, Université de Strasbourg

---

## Objectif

Ce projet consiste à développer un simulateur de **réseau local** en langage C, dans le cadre de la SAE2.3 – Réseaux.  

Les objectifs principaux sont :

- Manipuler une **structure de données représentant un graphe étiqueté**  
- Concevoir des **structures pour modéliser des équipements réseau** (stations, switchs)  
- Simuler la **diffusion de messages Ethernet** dans un réseau local  
- Implémenter le **protocole STP** pour la synchronisation automatique des switchs  
- Produire un code clair, structuré et commenté  

---

## Étapes du projet

### 1. Structures de données
- **Stations** : adresse MAC, adresse IP  
- **Switchs** : adresse MAC, nombre de ports, priorité, table de commutation  
- Réseau local représenté à l’aide d’un **graphe étiqueté**  
- Fonctions d’affichage pour toutes les structures (adresses, tables, etc.)  

### 2. Chargement d’une architecture
- Réseau encodé dans un **fichier de configuration**  
- Format : nombre d’équipements, nombre de liens, détails des équipements et connexions  
- Poids des liens dépend du débit :  
  - 10Mb/s → 100  
  - 100Mb/s → 19  
  - 1Gb/s → 4  

### 3. Commutation Ethernet
- Simulation de l’échange de **trames Ethernet**  
- Structure fidèle au format réel : préambule, SFD, adresses source/destination, type, données, FCS  
- Affichage en **mode utilisateur** et **hexadécimal**  

### 4. Spanning Tree Protocol (STP)
- Implémentation du **protocole STP**  
- Gestion des ports : racine, désignés, bloqués  
- Convergence automatique du réseau après échange de **BPDU**  

---

## Structure du dépôt

.
├── README.md
├── Makefile
├── src/
│ ├── main.c
│ ├── reseau.c
│ └── ...
└── include/
├── reseau.h
└── ...

---
