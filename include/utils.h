#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>


/**
 * Liest den gesamten Inhalt einer Datei und gibt diesen als nullterminierte Zeichenkette zurück.
 * Der Aufrufer ist für das Freigeben des Speichers verantwortlich.
 *
 * @param filename Der Pfad zur Datei.
 * @return Ein Zeiger auf den gelesenen String oder NULL bei einem Fehler.
 */
char *readFile(const char *filename);

#endif // UTILS_H