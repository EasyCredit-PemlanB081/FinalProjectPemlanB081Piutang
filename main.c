#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
const char seperator[] = "____";
const char enter[] = "\n";

int isExistFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {

        return 0;
    }
    else
    {
        fclose(fp);
        return 1;
    }
}

void makeFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    fclose(fp);
}

void writeFile(char *fileName, char *data)
{

    FILE *fp;
    fp = fopen(fileName, "w"); // append

    fprintf(fp, "%s", data);
    fflush(fp);
    fclose(fp);
}

void appendFile(char *fileName, char *data)
{
    printf("%s", data);
    FILE *fp;
    fp = fopen(fileName, "a"); // append

    fprintf(fp, "%s", data);
    fflush(fp);
    fclose(fp);
}

char *readFileText(char *fileName)
{
    FILE *fp;
    char *text;
    int size;
    fp = fopen(fileName, "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    text = (char *)malloc(sizeof(char) * size);
    fread(text, sizeof(char), size, fp);
    fclose(fp);
    return text;
}

typedef struct Pelanggan
{
    char *id;
    char *nama;
    char *alamat;
    char *no_hp;
} Pelanggan;

Pelanggan *dataPelanggan;
int sizeDataPelanggan = 0;

void addPelanggan(Pelanggan p)
{
    int size = sizeof(dataPelanggan) / sizeof(dataPelanggan[0]);
    dataPelanggan = (Pelanggan *)realloc(dataPelanggan, sizeof(Pelanggan) * (size + 1));
    dataPelanggan[sizeDataPelanggan] = p;
    sizeDataPelanggan++;
}

void editPelanggan(int index, Pelanggan p)
{
    dataPelanggan[index] = p;
}

void deletePelanggan(int index)
{
    int size = sizeof(dataPelanggan) / sizeof(dataPelanggan[0]);
    for (int i = index; i < sizeDataPelanggan; i++)
    {
        dataPelanggan[i] = dataPelanggan[i + 1];
    }
    dataPelanggan = (Pelanggan *)realloc(dataPelanggan, sizeof(Pelanggan) * (size - 1));
    sizeDataPelanggan--;
}

void loadTabelPelanggan()
{
    sizeDataPelanggan = 0;
    char *filename = "pelanggan.data";
    if (isExistFile(filename))
    {
        char *data = readFileText(filename);
        char *a, *c;
        for (a = strtok_r(data, enter, &c); a != NULL; a = strtok_r(NULL, enter, &c))
        {
            Pelanggan p;
            p.id = strtok(a, seperator);
            p.nama = strtok(NULL, seperator);
            p.alamat = strtok(NULL, seperator);
            p.no_hp = strtok(NULL, seperator);
            addPelanggan(p);
        }
    }
    else
    {
        makeFile(filename);
    }
}

void printAllPelanggan()
{
    int size = sizeDataPelanggan;
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", dataPelanggan[i].id);
        printf("%s\n", dataPelanggan[i].nama);
        printf("%s\n", dataPelanggan[i].alamat);
        printf("%s\n\n", dataPelanggan[i].no_hp);
    }
}

typedef struct Piutang
{
    char *nama_pelanggan;
    char *tanggal;
    double jumlahPiutang;
    double bunga;
    double sisaSaldo;
    double periode;
} Piutang;

Piutang *dataPiutang;
int sizeDataPiutang = 0;

void addPiutang(Piutang p)
{
    int size = sizeof(dataPiutang) / sizeof(dataPiutang[0]);
    dataPiutang = (Piutang *)realloc(dataPiutang, sizeof(Piutang) * (size + 1));
    dataPiutang[sizeDataPiutang] = p;
    sizeDataPiutang++;
}

char *piutangToText()
{
    for (int i = 0; i < sizeDataPiutang; i++)
    {
        char *data = (char *)malloc(sizeof(char) * 1000);
    }
}

void printAllPiutang()
{
    int size = sizeDataPiutang;
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", dataPiutang[i].nama_pelanggan);
        printf("%s\n", dataPiutang[i].tanggal);
        printf("%f\n", dataPiutang[i].jumlahPiutang);
        printf("%f\n", dataPiutang[i].bunga);
        printf("%f\n", dataPiutang[i].sisaSaldo);
        printf("%f\n\n", dataPiutang[i].periode);
    }
}

char *savePiutang()
{
    char *data = "a";

    for (int i = 0; i < 1; i++)
    {
        printf("%s", dataPiutang[i].nama_pelanggan);

        // data = strcat(data, dataPiutang[i].tanggal);
        // data = strcat(data, seperator);
        // // data = strcat(data, (char)dataPiutang[i].jumlahPiutang);
        // data = strcat(data, seperator);
        // // data = strcat(data, dataPiutang[i].bunga);
        // data = strcat(data, seperator);
        // // data = strcat(data, dataPiutang[i].sisaSaldo);
        // data = strcat(data, seperator);
        // // data = strcat(data, dataPiutang[i].periode);
        // data = strcat(data, enter);
    }
    return data;
}

void loadTabelPiutang()
{
    sizeDataPiutang = 0;
    char *filename = "piutang.data";
    if (isExistFile(filename))
    {
        char *data = readFileText(filename);
        char *a, *c;
        for (a = strtok_r(data, enter, &c); a != NULL; a = strtok_r(NULL, enter, &c))
        {
            Piutang p;
            p.nama_pelanggan = strtok(a, seperator);
            p.tanggal = strtok(NULL, seperator);
            p.jumlahPiutang = atof(strtok(NULL, seperator));
            p.bunga = atof(strtok(NULL, seperator));
            p.sisaSaldo = atof(strtok(NULL, seperator));
            p.periode = atof(strtok(NULL, seperator));
            addPiutang(p);
        }
    }
    else
    {
        makeFile(filename);
    }
}

void formPiutang()
{
    printf("Masukkan Pelanggan : ");
    char nama_pelanggan[30];
    getchar();
    gets(nama_pelanggan);
    printf("Masukkan Tanggal : ");
    char tanggal[10];
    getchar();
    gets(tanggal);
    printf("Masukkan Jumlah Piutang : ");
    double jumlahPiutang = 0;
    scanf("%lf", &jumlahPiutang);
    printf("Masukkan Bunga : ");
    double bunga = 0;
    scanf("%lf", &bunga);
    printf("Masukkan Berapa bulan cicilan : ");
    double periode = 0;
    scanf("%lf", &periode);
    Piutang p;
    p.nama_pelanggan = nama_pelanggan;
    p.tanggal = tanggal;
    p.jumlahPiutang = jumlahPiutang;
    p.bunga = bunga;
    p.sisaSaldo = 0;
    p.periode = periode;
    addPiutang(p);

    // writeFile("piutang.data", savePiutang());
}

void loadAllData()
{
    loadTabelPiutang();
}

void menuUtama();

void menuTagihan()
{
    system("cls");
    printf("\n********************************");
    printf("\n*        Menu Tagihan          *");
    printf("\n********************************");
    printf("\nPilih Menu : ");
    printf("\n1. Lihat Tagihan");
    printf("\n2. Bayar Tagihan");
    printf("\nTekan tombol lainnya untuk keluar\n");

    int pilih;
    scanf("%d", &pilih);
    switch (pilih)
    {
    case 1:

        break;
    case 2:

        break;
    case 3:

        break;
    default:
        menuUtama();
        break;
    }
}

void menuPiutang()
{
    system("cls");
    printf("\n********************************");
    printf("\n*        Menu Piutang          *");
    printf("\n********************************");
    printf("\nPilih Menu : ");
    printf("\n1. Lihat Piutang");
    printf("\n2. Tambah Piutang");
    printf("\nTekan tombol lainnya untuk keluar\n");

    int pilih;
    scanf("%d", &pilih);
    switch (pilih)
    {
    case 1:

        break;
    case 2:
        formPiutang();
        break;
    case 3:

        break;
    default:
        menuUtama();
        break;
    }
}

void menuUtama()
{
    system("cls");
    printf("\n********************************");
    printf("\n*     Aplikasi Piutang         *");
    printf("\n********************************");
    printf("\nPilih Menu : ");
    printf("\n1. Data Piutang");
    printf("\n2. Data Tagihan");
    printf("\nTekan tombol lainnya untuk keluar\n");

    int pilih;
    scanf("%d", &pilih);
    switch (pilih)
    {
    case 1:
        menuPiutang();
        break;
    case 2:
        menuTagihan();
        break;
    default:
        break;
    }
}

int main()
{

    loadAllData();
    menuUtama();

    return 0;
}