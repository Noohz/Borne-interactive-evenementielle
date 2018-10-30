# Borne interactive événementielle

Ce projet a été réalisé en équipe durant la seconde année de BTS SNIR (Systèmes Numériques option Informatique et Réseaux). Il consistait en la réalisation d'un système de borne interactive événementielle pour le restaurant-club "BAB" dans le but d'être utilisé lors de soirées organisées par des clients. Ce système propose aux clients de pouvoir bénéficier d’un système de prise de photos à la demande avec diffusion sur un écran visible de tous pendant la soirée, avec la possibilité d'envoyer les photos par mail et de mémoriser sur un support externe l’ensemble des photos.

La borne interactive événementielle est composée de deux sous-systèmes :
- système de prise de vue (accessible par le public)
- système de diffusion (accessible uniquement par le client loueur)

Ayant travaillé uniquement sur le système de prise de vue, seul les fichiers en rapport sont présents.
Le logiciel de prise de vue dispose de deux versions, une pour windows et l'autre pour Raspbian (utilisation de la cross-compilation pour déployer le logiciel sur la carte Raspberry Pi).

## Utilisation du logiciel de prise de vue

Lors d'une première utilisation  (fichier options.ini manquant), la page du mode paramétrage s'affiche.

<img src="https://github.com/Noohz/Borne-interactive-evenementielle/blob/master/Images/img_1.png" width="602"/>

L'adresse IP et le port d'écoute du système de diffusion sont modifiables et testables directement. Il est aussi possible de modifier les différentes durées des décomptes présents sur le logiciel.

Après avoir sauvegardé ou annulé, la page de choix du mode apparaît.

<img src="https://github.com/Noohz/Borne-interactive-evenementielle/blob/master/Images/img_2.png" width="602"/>

Cette page est celle qui s'affiche lorsque le logiciel est lancé (fichier options.ini présent).

Le mode utilisateur permet de prendre les photos et de choisir une action sur celle-ci.

<img src="https://github.com/Noohz/Borne-interactive-evenementielle/blob/master/Images/img_3.png" width="602"/>

Le cadre orange représente l'endroit où s'affiche la prévisualisation de la photo. Sur la version Raspbian, elle n'est pas présente. Après l'appui sur le bouton, un décompte apparaît à la place du texte du bouton. La photo est ensuite sauvegardée temporairement et une page s'affiche avec le résultat.

<img src="https://github.com/Noohz/Borne-interactive-evenementielle/blob/master/Images/img_4.png" width="602"/>

Elle permet de choisir ce que devient la photo prise :
- suppression
- envoi sur l'écran (envoi au système de diffusion)
- envoi sur l'écran avec la possibilité de l'envoyer par mail
- attendre la fin du décompte (envoi à l'écran)

Le décompte peut être arrêté en effectuant une action sur la page.

Suite à l'action effectué par l'utilisateur, une page avec un message de confirmation s'affichera, la photo est ensuite supprimée du système de prise de vue.

<img src="https://github.com/Noohz/Borne-interactive-evenementielle/blob/master/Images/img_5.png" width="602"/>

<img src="https://github.com/Noohz/Borne-interactive-evenementielle/blob/master/Images/img_6.png" width="602"/>


En appuyant sur "Ok", la page pour prendre en photo se réaffiche.
