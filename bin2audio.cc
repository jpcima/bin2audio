/*
Convertir fichier binaire en fichier WAVE.
*/

#include <sndfile.hh>
#include <getopt.h>
#include <stdlib.h>

void print_usage(const char *progname)
{
    fprintf(stderr,
            u8"Usage: %s -i <fichier-entrée> -o <fichier-sortie>\n"
              "          [-s taux]  : définit le taux déchantillonnage du fichier de sortie\n"
              "          [-b]       : pour effecture une lecture en big-endian\n",
            progname);
}

int main(int argc, char *argv[])
{
    const char *inputfile = nullptr;
    const char *outputfile = nullptr;
    float samplerate = 44100;
    bool littleendian = true;

    for (int c; (c = getopt(argc, argv, "i:o:s:b")) != -1;) {
        switch (c) {
        case 'i':
            inputfile = optarg;
            break;
        case 'o':
            outputfile = optarg;
            break;
        case 's':
            samplerate = atof(optarg);
            break;
        case 'b':
            littleendian = false;
            break;
        default:
            return 1;
        }
    }

    if (!inputfile || !outputfile || samplerate <= 0) {
        print_usage(argv[0]);
        return 1;
    }

    SndfileHandle snd(outputfile, SFM_WRITE, SF_FORMAT_WAV|SF_FORMAT_PCM_16, 1, samplerate);
    if (!snd) {
        fprintf(stderr, u8"Impossible d'ouvrir le fichier de sortie.\n");
        return 1;
    }

    FILE *fh = fopen(inputfile, "rb");
    if (!fh) {
        fprintf(stderr, u8"Impossible d'ouvrir le fichier d'entrée.\n");
        return 1;
    }

    // on récupère des échantillons 16 bits
    uint8_t octets[2];
    while (fread(octets, 1, 2, fh) == 2) {
        uint16_t echantillon;

        if (littleendian)
            echantillon = ((octets[0]) | (octets[1] << 8));
        else
            echantillon = ((octets[1]) | (octets[0] << 8));

        snd.write((int16_t *)&echantillon, 1); // écriture de l'échantillon
    }

    snd.writeSync();

    return 0;
}
