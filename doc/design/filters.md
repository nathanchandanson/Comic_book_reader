# Principe
Pour les filtres, on a un menu qui va choisir quels sont les filtres voulus.
A chaque changement, on va envoyer un signal contenant un entier qui représentera les filtres voulus pour que le Comic réceptionne les filtres.

Pour le message à mettre dans le signal, on prend un charactère qu'on code en binaire, et chaque bit représentera un booléen pour savoir si tel filtre est actif.
Ex. : 0 0 0 0 0 0 1 0
                  | > Filtre 1 non voulu
                  | > Filtre 2 voulu

# Correspondance bit/filtre
Bit : Filtre
1   : Contraste
2   : Saturation