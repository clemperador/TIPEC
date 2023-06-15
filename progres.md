1. Création du fichier 'domineering.c' (1/6/23)
- Création d'une fonction qui permet de réaliser le jeu le plus 'basique'
  C'est-à-dire, chaque joueur place un domino dans la première place légale qu'il trouve.
  Cette fonction peut aussi montrer dans le terminal le déroulement du jeu.
  
- Creation d'une fonction qui garde chacun des mouvements fait dans un jeu dans une liste de
  'mouvements', les mouvements définis avec une structure 'play'.

  Objectifs:
- Faire une fonction qui permet de simuler chaque jeu possible.

On considère le type 'game' qui équivaut à un tableau de 'plays'.
On veut créer une fonction qui crée une tableau de tableaux de plays, et donc un tableau des jeus possibles.

Pour cela, on commence par calculer le nombre de jeus possibles dans un tableau de mxn.
