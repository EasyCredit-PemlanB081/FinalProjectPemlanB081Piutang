#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
const char seperator[] = "____";
const char enter[] = "\n";

char filePiutang[] = "piutang.data";
char fileTagihan[] = "tagihan.data";

void menuUtama();
//Fungsi
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

char *readFileText(char *fileName)
{
    FILE *fp;
    char *text;
    int size;
    fp = fopen(fileName, "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0)
    {
        fclose(fp);
        return NULL;
    }
    else
    {
        fseek(fp, 0, SEEK_SET);
        text = (char *)malloc(sizeof(char) * size);
        fread(text, sizeof(char), size, fp);
        fclose(fp);
        return text;
    }
}

char *getDate(int timestamp)
{
    time_t epoch_time = timestamp;
    struct tm *converted_time;
    converted_time = localtime(&epoch_time);
    tzset();
    char *buffer = malloc(sizeof(char) * (26 + 1));
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", converted_time);
    return buffer;
}

typedef struct Piutang
{
    int timestamp;
    char *nama_pelanggan;
    char *nik;
    char *tanggal;
    double jumlahPiutang;
    double bunga;
    double sisaSaldo;
    double jumlahBayar;
    double sisaCicilan;
    double periode;
    char *klasifikasi;
} Piutang;

typedef struct Tagihan
{
    int timestamp;
    int timestamp_piutang;
    int timestamp_jatuhtempo;
    double jumlahCicilan;
    int cicilanKe;
    char *jatuhtempo;
    int flagbayar;
    Piutang piutang;
} Tagihan;

Tagihan *dataTagihan;
int sizeDataTagihan = 0;

Piutang *dataPiutang;
int sizeDataPiutang = 0;

void addPiutang(Piutang p, int write)
{
    // Masukkan dalam file
    if (write)
    {
        FILE *fp;
        fp = fopen(filePiutang, "a");
        if (fp == NULL)
        {
            makeFile(filePiutang);
            fp = fopen(filePiutang, "a");
        }

        fprintf(fp, "%d", p.timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.nik);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.nama_pelanggan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.tanggal);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.jumlahPiutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.bunga);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.sisaSaldo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.jumlahBayar);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.sisaCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.periode);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.klasifikasi);
        fprintf(fp, "%s", enter);
        fflush(fp);
        fclose(fp);
    }

    // tambahkan data di memori
    int size = sizeDataPiutang;
    dataPiutang = (Piutang *)realloc(dataPiutang, sizeof(Piutang) * (size + 1));
    dataPiutang[sizeDataPiutang] = p;
    sizeDataPiutang++;
}

void addTagihan(Tagihan t)
{

    dataTagihan = (Tagihan *)realloc(dataTagihan, sizeof(Tagihan) * (sizeDataTagihan + 1));
    dataTagihan[sizeDataTagihan] = t;
    sizeDataTagihan++;
}

void generateTagihan(Piutang p)
{
    int timestampJatuhTempo = p.timestamp;
    for (int i = 0; i < p.periode; i++)
    {
        timestampJatuhTempo += (3600 * 24 * 30);
        Tagihan tagihan;
        tagihan.timestamp = (int)time(NULL) + i;
        tagihan.timestamp_piutang = p.timestamp;
        tagihan.timestamp_jatuhtempo = timestampJatuhTempo;
        tagihan.cicilanKe = i + 1;
        tagihan.jumlahCicilan = p.jumlahPiutang * (100 + p.bunga) / 100 / p.periode;
        tagihan.flagbayar = 0;
        tagihan.piutang = p;

        FILE *fp;
        fp = fopen(fileTagihan, "a");
        if (fp == NULL)
        {
            makeFile(fileTagihan);
            fp = fopen(fileTagihan, "a");
        }

        fprintf(fp, "%d", tagihan.timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.timestamp_piutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.timestamp_jatuhtempo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", tagihan.jumlahCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.cicilanKe);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", getDate(tagihan.timestamp_jatuhtempo));
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.flagbayar);
        fprintf(fp, "%s", enter);
        fflush(fp);
        fclose(fp);

        // Tambahkan data di memoria
        int size = sizeof(dataTagihan) / sizeof(dataTagihan[0]);
        dataTagihan = (Tagihan *)realloc(dataTagihan, sizeof(Tagihan) * (size + 1));
        dataTagihan[sizeDataTagihan] = tagihan;
        sizeDataPiutang++;
    }
}

void printAllPiutang()
{
    system("cls");
    printf("\n********************************");
    printf("\n*      Daftar Piutang          *");
    printf("\n********************************\n");
    int size = sizeDataPiutang;
    for (int i = 0; i < size; i++)
    {
        printf("Nama Pelanggan :%s\n", dataPiutang[i].nama_pelanggan);
        printf("NIK :%s\n", dataPiutang[i].nik);
        printf("Tanggal Piutang :%s\n", dataPiutang[i].nama_pelanggan);
        printf("Sisa Piutang :%0.f\n\n", dataPiutang[i].sisaSaldo);
    }

    system("pause");
    menuUtama();
}

void printAllTagihan()
{
    system("cls");
    printf("\n********************************");
    printf("\n*      Daftar Tagihan          *");
    printf("\n********************************\n");
    int size = sizeDataTagihan;
    for (int i = 0; i < size; i++)
    {
        printf("Nama Pelanggan :%s\n", dataTagihan[i].piutang.nama_pelanggan);
        printf("Cicilan ke :%d\n", dataTagihan[i].cicilanKe);
        printf("Jumlah :%0.f\n", dataTagihan[i].jumlahCicilan);
        printf("Jatuh tempo :%s\n\n", dataTagihan[i].jatuhtempo);
    }
    system("pause");
    menuUtama();
}

void loadTabelPiutang()
{
    sizeDataPiutang = 0;
    if (isExistFile(filePiutang))
    {
        char *data = readFileText(filePiutang);

        //Check bila kosong
        if (data != NULL)
        {
            char *a, *c;
            for (a = strtok_r(data, enter, &c); a != NULL; a = strtok_r(NULL, enter, &c))
            {

                Piutang p;
                p.timestamp = atoi(strtok(a, seperator));
                p.nik = strtok(NULL, seperator);
                p.nama_pelanggan = strtok(NULL, seperator);
                p.tanggal = strtok(NULL, seperator);
                p.jumlahPiutang = atof(strtok(NULL, seperator));
                p.bunga = atof(strtok(NULL, seperator));
                p.sisaSaldo = atof(strtok(NULL, seperator));
                p.jumlahBayar = atof(strtok(NULL, seperator));
                p.sisaCicilan = atof(strtok(NULL, seperator));
                p.periode = atof(strtok(NULL, seperator));
                p.klasifikasi = strtok(NULL, seperator);
                addPiutang(p, 0);
            }
        }
    }
    else
    {
        makeFile(filePiutang);
    }
}

void loadTabelTagihan()
{
    sizeDataTagihan = 0;
    if (isExistFile(fileTagihan))
    {
        char *data = readFileText(fileTagihan);

        //Check bila kosong
        if (data != NULL)
        {
            int idpiutang = 0;
            char *a, *c;
            for (a = strtok_r(data, enter, &c); a != NULL; a = strtok_r(NULL, enter, &c))
            {

                Tagihan p;
                p.timestamp = atoi(strtok(a, seperator));
                p.timestamp_piutang = atoi(strtok(NULL, seperator));
                p.timestamp_jatuhtempo = atoi(strtok(NULL, seperator));
                p.jumlahCicilan = atof(strtok(NULL, seperator));
                p.cicilanKe = atoi(strtok(NULL, seperator));
                p.jatuhtempo = strtok(NULL, seperator);
                p.flagbayar = atoi(strtok(NULL, seperator));
                if (p.timestamp_piutang != dataPiutang[idpiutang].timestamp)
                {
                    idpiutang++;
                }
                // printf("%d", idpiutang);
                p.piutang = dataPiutang[idpiutang];
                // printf("%s", p.piutang.nama_pelanggan);
                addTagihan(p);
            }
            // printf("%d", sizeDataTagihan);
        }
    }
    else
    {
        makeFile(fileTagihan);
    }
}

void formPiutang()
{
    printf("Masukkan Pelanggan : ");
    char nama_pelanggan[30];
    getchar();
    gets(nama_pelanggan);
    printf("Masukkan NIK : ");
    char nik[30];
    gets(nik);
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
    p.timestamp = (int)time(NULL);
    p.nama_pelanggan = nama_pelanggan;
    p.nik = nik;
    p.tanggal = getDate(p.timestamp);
    p.jumlahPiutang = jumlahPiutang;
    p.bunga = bunga;
    p.sisaSaldo = jumlahPiutang * (100 + p.bunga) / 100;
    p.klasifikasi = "Lancar";
    p.jumlahBayar = 0;
    p.sisaCicilan = 0;
    p.periode = periode;
    addPiutang(p, 1);
    generateTagihan(p);
}

void loadAllData()
{
    loadTabelPiutang();
    loadTabelTagihan();
}

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
        printAllTagihan();
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
        printAllPiutang();
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

    // printAllPiutang();
    return 0;
}