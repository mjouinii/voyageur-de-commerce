#!/bin/bash

# --- Configuration ---
PROJECT_NAME="voyageur-de-commerce"
BACKUP_DIR="backups"
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
ARCHIVE_NAME="${PROJECT_NAME}_${TIMESTAMP}.tar.gz"

# 1. Créer le dossier de backup s'il n'existe pas
mkdir -p "$BACKUP_DIR"

echo "--- Début de la sauvegarde pour $PROJECT_NAME ---"

# 2. Nettoyer le projet avant sauvegarde (optionnel mais recommandé)
# Cela évite de sauvegarder les fichiers .o et l'exécutable
if [ -f "Makefile" ]; then
    echo "Nettoyage des fichiers temporaires (make clean)..."
    make clean
fi

# 3. Création de l'archive compressée
# On exclut le dossier backups lui-même pour éviter l'auto-sauvegarde
# On exclut aussi .git 
echo "Compression des fichiers..."
tar -czf "$BACKUP_DIR/$ARCHIVE_NAME" \
    --exclude="$BACKUP_DIR" \
    --exclude=".git" \
    .

if [ $? -eq 0 ]; then
    echo "Sauvegarde réussie : $BACKUP_DIR/$ARCHIVE_NAME"
    echo "Taille de l'archive : $(du -h "$BACKUP_DIR/$ARCHIVE_NAME" | cut -f1)"
else
    echo "Erreur lors de la sauvegarde !"
    exit 1
fi

echo "--- Terminé ---"