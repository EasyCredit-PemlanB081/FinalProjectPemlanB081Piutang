#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
const char seperator[] = "____";
const char enter[] = "\n";

const int ID = 0;
const int ID_PIUTANG = 2;
const int NIK = 10;
const int NAMA = 11;

const double denda = 0.1;

char filePiutang[] = "piutang.data";
char fileTagihan[] = "tagihan.data";

void menuUtama();
void loadAllData();
//Fungsi
int getNow()
{
    return (int)time(NULL);
}

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

void tukarPiutang(int i, int j)
{
    Piutang temp = dataPiutang[i];
    dataPiutang[i] = dataPiutang[j];
    dataPiutang[j] = temp;
}

void cekColumnPiutang(int i, int j, int column)
{
    if (column == ID)
    {
        if (dataPiutang[i].timestamp > dataPiutang[j].timestamp)
        {
            tukarPiutang(i, j);
        }
    }
    else if (column == NIK)
    {
        if (strcmp(dataPiutang[i].nik, dataPiutang[j].nik) > 0)
        {
            tukarPiutang(i, j);
        }
    }
    else if (column == NAMA)
    {
        if (strcmp(dataPiutang[i].nama_pelanggan, dataPiutang[j].nama_pelanggan) > 0)
        {
            tukarPiutang(i, j);
        }
    }
}

void sortingPiutang(int column)
{
    for (int i = 0; i < sizeDataPiutang; i++)
    {

        for (int j = i + 1; j < sizeDataPiutang; j++)
        {
            cekColumnPiutang(i, j, column);
        }
    }
}

void tukarTagihan(int i, int j)
{
    Tagihan temp = dataTagihan[i];
    dataTagihan[i] = dataTagihan[j];
    dataTagihan[j] = temp;
}

void cekColumnTagihan(int i, int j, int column)
{
    if (column == ID)
    {
        if (dataTagihan[i].timestamp > dataTagihan[j].timestamp)
        {
            tukarTagihan(i, j);
        }
    }
    else if (column == NIK)
    {
        if (strcmp(dataTagihan[i].piutang.nik, dataTagihan[j].piutang.nik) > 0)
        {
            tukarTagihan(i, j);
        }
    }
    else if (column == NAMA)
    {
        if (strcmp(dataTagihan[i].piutang.nama_pelanggan, dataTagihan[j].piutang.nama_pelanggan) > 0)
        {
            tukarTagihan(i, j);
        }
    }
    else if (column == ID_PIUTANG)
    {
        if (dataTagihan[i].timestamp_piutang > dataTagihan[j].timestamp_piutang)
        {
            tukarTagihan(i, j);
        }
    }
}

void sortingTagihan(int column)
{
    for (int i = 0; i < sizeDataTagihan; i++)
    {

        for (int j = i + 1; j < sizeDataTagihan; j++)
        {
            cekColumnTagihan(i, j, column);
        }
    }
}

int seachingTagihanByIDPiutang(int timestamp)
{
    if (sizeDataTagihan == 0)
    {
        return -1;
    }
    sortingTagihan(ID_PIUTANG);
    for (int i = 0; i < sizeDataTagihan; i++)
    {
        if (dataTagihan[i].timestamp_piutang == timestamp)
        {
            return i;
        }
    }
}

int searchingPiutangBelumLunasByNIK(char *nik)
{
    if (sizeDataPiutang == 0)
    {
        return -1;
    }
    sortingPiutang(NIK);

    int step = sizeDataPiutang == 1 ? 0 : sqrt(sizeDataPiutang);
    int jumper = step;
    int prev = 0;
    while (strcmp(dataPiutang[jumper].nik, nik) <= 0 && jumper < sizeDataPiutang)
    {

        //proses mencari batesan lompat
        prev = jumper;
        jumper += step;
        if (jumper >= sizeDataPiutang || prev == jumper)
        {
            break;
        }
    }

    int founded = 0;
    for (int i = prev; i <= jumper && i < sizeDataPiutang; i++)
    {

        if (strcmp(dataPiutang[i].nik, nik) == 0 && dataPiutang[i].sisaSaldo > 0)
        {
            return i;
        }
    }
    return -1;
}

int searchingPiutangBelumLunasByNama(char *nama_pelanggan)
{
    if (sizeDataPiutang == 0)
    {
        return -1;
    }
    sortingPiutang(NAMA);

    int step = sizeDataPiutang == 1 ? 0 : sqrt(sizeDataPiutang);
    int jumper = step;
    int prev = 0;
    while (strcmp(dataPiutang[jumper].nama_pelanggan, nama_pelanggan) <= 0 && jumper < sizeDataPiutang)
    {

        //proses mencari batesan lompat
        prev = jumper;
        jumper += step;
        if (jumper >= sizeDataPiutang || prev == jumper)
        {
            break;
        }
    }

    int founded = 0;
    for (int i = prev; i <= jumper && i < sizeDataPiutang; i++)
    {

        if (strcmp(dataPiutang[i].nama_pelanggan, nama_pelanggan) == 0 && dataPiutang[i].sisaSaldo > 0)
        {
            return i;
        }
    }
    return -1;
}

void setKlasifikasi(Piutang p, int klasifikasi)
{
    if (klasifikasi <= 1)
    {
        p.klasifikasi = "Lancar";
    }
    else if (klasifikasi <= 3)
    {
        p.klasifikasi = "Dalam perhatian";
    }
    else if (klasifikasi <= 5)
    {
        p.klasifikasi = "Kurang";
    }
    else if (klasifikasi < 7)
    {
        p.klasifikasi = "Diragukan ";
    }
    else
    {
        p.klasifikasi = "Macet ";
    }
}

void rewritePiutang()
{
    sortingPiutang(ID);

    FILE *fp;
    fp = fopen(filePiutang, "w");
    if (fp == NULL)
    {
        makeFile(filePiutang);
        fp = fopen(filePiutang, "w");
    }
    int size = sizeDataPiutang;
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%d", dataPiutang[i].timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].nik);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].nama_pelanggan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].tanggal);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].jumlahPiutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].bunga);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].sisaSaldo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].jumlahBayar);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].sisaCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].periode);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].klasifikasi);
        fprintf(fp, "%s", enter);
    }

    fflush(fp);
    fclose(fp);
}

void rewriteTagihan()
{
    sortingTagihan(ID);

    FILE *fp;
    fp = fopen(fileTagihan, "w");
    if (fp == NULL)
    {
        makeFile(fileTagihan);
        fp = fopen(fileTagihan, "w");
    }
    int size = sizeDataTagihan;
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%d", dataTagihan[i].timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].timestamp_piutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].timestamp_jatuhtempo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataTagihan[i].jumlahCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].cicilanKe);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", getDate(dataTagihan[i].timestamp_jatuhtempo));
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].flagbayar);
        fprintf(fp, "%s", enter);
    }

    fflush(fp);
    fclose(fp);
}

void cetakTagihanBelumDibayarByIdPiutang(int index)
{
    system("cls");
    Piutang piutang = dataPiutang[index];
    int indexStart = seachingTagihanByIDPiutang(piutang.timestamp);
    printf("Data Piutang\n");
    printf("Pelanggan : %s\n", piutang.nama_pelanggan);
    printf("Total Piutang : %0.f\n", piutang.jumlahPiutang);
    printf("Sisa Piutang : %0.f\n", piutang.sisaSaldo);
    printf("Status %s\n\n", piutang.klasifikasi);

    printf("=================================\n");
    for (int i = indexStart; i < indexStart + piutang.periode; i++)
    {
        if (dataTagihan[i].flagbayar == 0)
        {
            printf("Cicilan ke %d\n", dataTagihan[i].cicilanKe);
            printf("Jumlah Cicilan : %0.f\n", dataTagihan[i].jumlahCicilan);
            printf("Tanggal Jatuh Tempo : %s\n\n", dataTagihan[i].jatuhtempo);
        }
    }
}

void bayarCicilan(int index, int jumlahCicilan)
{
    int indexStart = seachingTagihanByIDPiutang(dataPiutang[index].timestamp);
    for (int i = indexStart; i < indexStart + dataPiutang[index].periode && jumlahCicilan > 0; i++)
    {
        if (dataTagihan[i].flagbayar == 0)
        {
            dataTagihan[i].flagbayar = 1;
            dataPiutang[index].sisaCicilan -= 1;
            dataPiutang[index].sisaSaldo -= dataTagihan[i].jumlahCicilan;
            dataPiutang[index].jumlahBayar += dataTagihan[i].jumlahCicilan;
            jumlahCicilan -= 1;
        }
    }
}

void lunasCicilan(int index)
{
    int indexStart = seachingTagihanByIDPiutang(dataPiutang[index].timestamp);
    for (int i = indexStart; i < indexStart + dataPiutang[index].periode; i++)
    {
        if (dataTagihan[i].flagbayar == 0)
        {
            dataTagihan[i].flagbayar = 1;
            dataPiutang[index].sisaCicilan -= 1;
            dataPiutang[index].jumlahBayar += dataTagihan[i].jumlahCicilan;
            dataPiutang[index].sisaSaldo -= dataTagihan[i].jumlahCicilan;
        }
    }
}

void bayarPiutang()
{
    system("cls");
    printf("\n********************************");
    printf("\n*  Bayar Tagihan Piutang       *");
    printf("\n********************************\n");
    printf("Masukkan NIK Pelanggan : ");
    char nik[30];
    getchar();
    gets(nik);
    int index = searchingPiutangBelumLunasByNIK(nik);

    if (index > -1)
    {
        cetakTagihanBelumDibayarByIdPiutang(index);
        printf("\n\nPilih Metode Pembayaran");
        printf("\n1. 1 Cicilan");
        printf("\n2. Custom");
        printf("\n3. Lunas");

        printf("\n\nPilihan : ");
        int pilihan;
        scanf("%d", &pilihan);
        switch (pilihan)
        {
        case 1:
            bayarCicilan(index, 1);
            break;
        case 2:
            printf("\nMasukkan Jumlah Cicilan : ");
            int jumlahCicilan;
            scanf("%d", &jumlahCicilan);
            bayarCicilan(index, jumlahCicilan);

            printf("\nPembayaran berhasil ...");
            break;
        case 3:
            lunasCicilan(index);
            printf("\nPembayaran berhasil ...");
            break;
        default:
            printf("\nPilihan tidak tersedia");
            break;
        }
        rewritePiutang();
        rewriteTagihan();
        loadAllData();
        system("pause");
        menuUtama();
    }
    else
    {
        printf("\nMohon maaf, Tagihan tidak ditemukan");
        system("pause");
        menuUtama();
    }
}

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
    dataPiutang = (Piutang *)realloc(dataPiutang, sizeof(Piutang) * (sizeDataPiutang + 1));
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
        // Tambahkan 30 hari
        timestampJatuhTempo += (3600 * 24 * 30);
        Tagihan tagihan;
        tagihan.timestamp = getNow() + i;
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
        dataTagihan = (Tagihan *)realloc(dataTagihan, sizeof(Tagihan) * (sizeDataTagihan + 1));
        dataTagihan[sizeDataTagihan] = tagihan;
        sizeDataTagihan++;
    }
}

void printPiutang(int i)
{
    printf("Nama Pelanggan :%s\n", dataPiutang[i].nama_pelanggan);
    printf("NIK :%s\n", dataPiutang[i].nik);
    printf("Tanggal Piutang :%s\n", dataPiutang[i].tanggal);
    printf("Diskon:%d persen\n", dataPiutang[i].jumlahPiutang);
    printf("Jumlah Diterima :%0.f\n", dataPiutang[i].jumlahPiutang);
    printf("Jumlah Bayar :%0.f\n", dataPiutang[i].jumlahBayar);
    printf("Sisa Piutang :%0.f\n", dataPiutang[i].sisaSaldo);
    printf("Status :%s\n\n", dataPiutang[i].klasifikasi);
}

void cariPiutang(int column)
{
    system("cls");
    if (column == NAMA)
    {
        printf("Pencarian Piutang belum lunas berdasarkan Nama\n");
    }
    else
    {
        printf("Pencarian Piutang belum lunas berdasarkan NIK\n");
    }
    char keyword[30];
    printf("Masukkan keyword pencarian : ");
    scanf("%s", keyword);
    int i;
    if (column == NAMA)
    {
        i = searchingPiutangBelumLunasByNama(keyword);
    }
    else
    {
        i = searchingPiutangBelumLunasByNIK(keyword);
    }

    if (i >= 0)
    {
        printPiutang(i);
    }
    else
    {
        printf("Data tidak ditemukan\n");
    }
    system("pause");
    menuUtama();
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
        printPiutang(i);
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
    printf("1. Cari berdasarkan NIK");
    printf("2. Cari berdasarkan Nama");
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
            int klasifikasi = 0;
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
                    klasifikasi = 0;
                }
                klasifikasi++;

                p.piutang = dataPiutang[idpiutang];
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

void deleteDataTagihan(int index)
{
    if (index < sizeDataTagihan)
    {
        for (int i = index; i < sizeDataTagihan - 1; i++)
        {
            dataTagihan[i] = dataTagihan[i + 1];
        }
        sizeDataTagihan--;
    }
}

void deleteDataPiutang(int index)
{
    if (index < sizeDataPiutang)
    {
        int indexStart = seachingTagihanByIDPiutang(dataPiutang[index].timestamp);
        printf("start :%d", indexStart);
        while (dataTagihan[indexStart].timestamp_piutang == dataPiutang[index].timestamp)
        {
            deleteDataTagihan(indexStart);
        }
        rewriteTagihan();
        for (int i = index; i < sizeDataPiutang - 1; i++)
        {
            dataPiutang[i] = dataPiutang[i + 1];
        }

        sizeDataPiutang--;
    }
}

void formPiutang()
{
    system("cls");
    printf("\n***********************************************");
    printf("\n*                Form Piutang                 *");
    printf("\n***********************************************");
    printf("\nMasukkan Pelanggan : ");
    char nama_pelanggan[30];
    getchar();
    gets(nama_pelanggan);
    printf("Masukkan NIK : ");
    char nik[30];
    gets(nik);
    double sisaUtang = 0;
    int recentUtang = searchingPiutangBelumLunasByNIK(nik);
    if (recentUtang > -1)
    {

        if (strcmp(dataPiutang[recentUtang].klasifikasi, "Lancar") != 0)
        {
            printf("Pelanggan sudah memiliki piutang\n Silahkan inputkan ulang !!\n");
            system("pause");
            formPiutang();
        }
        sisaUtang = dataPiutang[recentUtang].sisaSaldo;
    }
    printf("Masukkan Jumlah Piutang : ");
    double jumlahPiutang = 0;
    scanf("%lf", &jumlahPiutang);
    printf("Masukkan Bunga : ");
    double bunga = 0;
    scanf("%lf", &bunga);
    printf("Masukkan Berapa bulan cicilan : ");
    double periode = 0;
    scanf("%lf", &periode);

    if (recentUtang > -1 && jumlahPiutang < sisaUtang)
    {
        printf("Utang terbaru harus melebihi saldo piutang\n");
        system("pause");
        formPiutang();
    }
    else if (bunga > 100)
    {
        printf("Bunga tidak boleh lebih dari 100\n");
        system("pause");
        formPiutang();
    }
    else if (recentUtang > -1 && jumlahPiutang >= sisaUtang)
    {
        // Lunasi hutang sebelumnya
        lunasCicilan(recentUtang);
        rewritePiutang();
        rewriteTagihan();
    }

    Piutang p;
    p.timestamp = getNow();
    p.nama_pelanggan = nama_pelanggan;
    p.nik = nik;
    p.tanggal = getDate(p.timestamp);
    p.jumlahPiutang = jumlahPiutang - sisaUtang;
    p.bunga = bunga;
    p.sisaSaldo = jumlahPiutang * (100 + p.bunga) / 100;
    p.klasifikasi = "Lancar";
    p.jumlahBayar = 0;
    p.sisaCicilan = periode;
    p.periode = periode;
    addPiutang(p, 1);
    p.jumlahPiutang = jumlahPiutang;
    generateTagihan(p);
    printf("Data berhasil ditambah\n");
    system("pause");
    menuUtama();
}

void formEditPelanggan()
{
    system("cls");
    printf("\n********************************");
    printf("\n*       Edit Profil            *");
    printf("\n********************************");
    printf("\nMasukkan NIK : ");
    char nik[30];
    getchar();
    gets(nik);

    int index = searchingPiutangBelumLunasByNIK(nik);

    if (index != -1)
    {
        printf("Data lama :\n");
        printf("Nama Pelanggan :%s\n", dataPiutang[index].nama_pelanggan);
        printf("NIK :%s\n\n", dataPiutang[index].nik);
        printf("Data Baru :\n");
        printf("Masukkan Nama Pelanggan : ");
        gets(dataPiutang[index].nama_pelanggan);
        printf("Masukkan NIK Pelanggan : ");
        gets(dataPiutang[index].nik);
        rewritePiutang();
    }
    else
    {
        printf("NIK tidak ditemukan");
    }
    system("pause");
    menuUtama();
}

void formDeletePiutang()
{
    system("cls");
    printf("\n***********************************************");
    printf("\n*   Hapus Piutang (Akan menghapus tagihan)    *");
    printf("\n***********************************************");
    printf("\nMasukkan NIK : ");
    char nik[30];
    getchar();
    gets(nik);

    int index = searchingPiutangBelumLunasByNIK(nik);

    if (index != -1)
    {
        printf("Data lama :\n");
        printf("Nama Pelanggan :%s\n", dataPiutang[index].nama_pelanggan);
        printf("NIK :%s\n", dataPiutang[index].nik);
        printf("Piutang :%0.f\n", dataPiutang[index].jumlahPiutang);
        printf("Apakah anda yakin untuk menghapus tekan (1) untuk melanjutkan ? :\n");
        int i = 0;
        scanf("%d", &i);
        if (i == 1)
        {
            printf("Hapus data berhasil\n");
            deleteDataPiutang(index);
            rewritePiutang();
        }
    }
    else
    {
        printf("NIK tidak ditemukan");
    }
    system("pause");
    menuUtama();
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
    printf("\n1. Lihat Tagihan Sudah dibayar");
    printf("\n2. Lihat Tagihan Belum dibayar");
    printf("\n3. Bayar Tagihan");
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
        bayarPiutang();
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
    printf("\n3. Edit Data Pelanggan");
    printf("\n4. Hapus Piutang");
    printf("\n5. Cari Piutang Belum Lunas (Berdasarkan NIK)");
    printf("\n6. Cari Piutang Belum Lunas (Berdasarkan Nama)");
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
        formEditPelanggan();
        break;
    case 4:
        formDeletePiutang();
        break;
    case 5:
        cariPiutang(NIK);
        break;
    case 6:
        cariPiutang(NAMA);
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