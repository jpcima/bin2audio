/*
Convertir fichier binaire en fichier WAVE.

  Construction :    g++ -O2 -g -o sndfile sndfile.cc -lsndfile
*/

#include <sndfile.hh>
#include <getopt.h>
#include <stdlib.h>

void print_usage(const char *progname)
{
    fprintf(stderr,
            u8"Usage: %s -i <fichier-entrée> -o <fichier-sortie>\n"
              "          [-s taux-échantillonnage]\n",
            progname);
}

int main(int argc, char *argv[])
{
    const char *inputfile = nullptr;
    const char *outputfile = nullptr;
    float samplerate = 44100;

    for (int c; (c = getopt(argc, argv, "i:o:s:")) != -1;) {
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
    int16_t sample;
    while (fread(&sample, 1, sizeof(sample), fh) == sizeof(sample)) {

        sample = (((uint16_t)sample << 8) |
            ((uint16_t)sample >> 8)); // si on échanger les octets haut et bas

        snd.write(&sample, 1); // écriture de l'échantillon
    }

    snd.writeSync();

    return 0;
}
