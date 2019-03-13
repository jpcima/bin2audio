#!/bin/bash
set -e

if test "$#" -ne 1; then
    echo "$0: il faut indiquer le chemin du dossier à traiter"
    exit 1
fi

dossier=$1

find "$dossier" -type f | while read fichier; do
    fichsortie="$fichier".wav

    case "$fichier" in
        *.wav) # on ignore les fichiers de type WAV
            ;;

        *) # fichier quelconque, on le transforme en WAV
            echo "Conversion: '$fichier' vers '$fichsortie'"
            bin2audio -i "$fichier" -o "$fichsortie"
            ;;
    esac
done
